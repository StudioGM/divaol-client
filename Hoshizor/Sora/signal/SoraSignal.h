//
//  SoraSignal.h
//  Sora
//
//  Created by Robert Bu on 8/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSignal_h
#define Sora_SoraSignal_h

#include "SoraPlatform.h"
#include "SoraAny.h"
#include "SoraAutoPtr.h"
#include "SoraDelegate.h"
#include "function/SoraFunction.h"

#include "SoraUncopyable.h"

#include <map>

namespace sora {
    
    /**
     * A Simple Signal Implemention based on the thought of boost::signal
     *
     * Users can connect to a signal
     * Users can disconnect to the signal at anytime
     * When a specific event happened, all connection would be notified
     **/
    
    namespace signal {
        class ConnectionBase;
    }
    
    class SORA_API SoraConnection {
    public:
        SoraConnection():
        isControlling(false) {
        }
        ~SoraConnection() {
            if(isControlling) {
                disconnect();
            }
        }
        bool isValid();
        void disconnect();
        
        bool operator ==(const SoraConnection &rhs) {
            sora_assert(con.get());
            return con.get() == rhs.con.get();
        }
        
        /**
         * Signal managed functions
         * You should not call these functions
         **/
        
        signal::ConnectionBase* get_con_base() {
            return con.get();
        }
        
        void setControl(bool flag) {
            isControlling = flag;
        }
        
        void reset(signal::ConnectionBase* _con) {
            con.reset(_con);
        }
        
    private:
        bool isControlling;
        SoraAutoPtr<signal::ConnectionBase> con;
    };
    
    namespace signal {
 
        class SORA_API SignalImpl {
        public:                        
            typedef std::multimap<int, SoraConnection> ConnectionMap;
            typedef std::multimap<int, SoraConnection>::iterator IteratorType;
            ConnectionMap mConnections;
            
            // set controlling flag to false to avoid repeat destruction in disconnect
            virtual ~SignalImpl();
            
            void disconnect(void* sig, void* data) {                
                std::auto_ptr<IteratorType> slot(reinterpret_cast<IteratorType*>(data));
                mConnections.erase(*slot);
            }
        };
        
        template<typename SIG>
        class SORA_API Slot {
        public:
            bool isValid() {
                return fn;
            }
            
            typedef sora::SoraFunction<SIG> FuncType;
            FuncType fn;
        };
        
        class SORA_API ConnectionBase {
        public:
            virtual bool isValid() = 0;
            virtual void disconnect() = 0;
            
            SignalImpl* signal;
            SignalImpl::IteratorType* iter;

            typedef void (SignalImpl::*DisconnectFunc)(void*, void*);
            DisconnectFunc signal_disconnet;
        };
        
        template<typename SIG>
        class SORA_API BasicConnection: public ConnectionBase {
        public:
            typedef typename Slot<SIG>::FuncType FuncType;
            
            sora::SoraAutoPtr<Slot<SIG> > slot;
            
            bool isValid() {
                return slot.get() && signal_disconnet && signal;
            }
            
            void disconnect() {
                if(isValid()) {
                    (signal->*signal_disconnet)(signal, iter);
                    signal = NULL;
                    signal_disconnet = NULL;
                }
            }
            
            bool operator ==(const BasicConnection &rhs) {
                return (slot.get()->fn == rhs.slot.get()->fn && signal == rhs.signal);
            }
        };
        
    } // namespace signal
  
        inline bool SoraConnection::isValid() {
            if(con.get())
                return con->isValid();
            return false;
        }
        
        inline void SoraConnection::disconnect() {
            if(con.get())
                con->disconnect();
        }
    
    template<typename SIG>
    class SORA_API SoraSignalBase: public signal::SignalImpl {
    public:
        typedef SoraConnection ConnectionType; 
        typedef typename signal::BasicConnection<SIG>::FuncType slot_type;

        template<typename F>
        ConnectionType connect(const F& f) {
            return connect(0, f);
        }
        
        template<typename F>
        ConnectionType connect(int32 priority, const F& f) {
            signal::BasicConnection<SIG>* con = new signal::BasicConnection<SIG>;
            SoraConnection slotcon;
            slotcon.reset(con);
            
            std::auto_ptr<IteratorType> saved_iter(new IteratorType());
            IteratorType iter = mConnections.insert(std::make_pair(priority, slotcon));
            *saved_iter = iter;
            
            // connection controller
            iter->second.setControl(true);
            
            con->slot.reset(new signal::Slot<SIG>);
            con->slot->fn = f;
            con->signal = this;
            con->signal_disconnet = &signal::SignalImpl::disconnect;
            con->iter = saved_iter.release();
            
            return slotcon;
        }
    };
    
    template<typename SIG>
    class SoraSignal;
    
    // signal 1
    template<typename RT>
    class SORA_API SoraSignal<RT(void)>: public SoraSignalBase<RT(void)> {
    public:
        RT sig() {
            typedef SoraSignalBase<RT(void)> base_type;
            
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT()>* con = static_cast<signal::BasicConnection<RT()>* >(itConnection->second.get_con_base());
                rtVal = con->slot.get()->fn();
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()() { sig(); }
    };
    
    // signal 1 void
    template<>
    class SORA_API SoraSignal<void(void)>: public SoraSignalBase<void(void)> {
    public:
        void sig() {
            typedef SoraSignalBase<void(void)> base_type;
            base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void()>* con = static_cast<signal::BasicConnection<void()>* >(itConnection->second.get_con_base());
                con->slot.get()->fn();
                ++itConnection;
            }
        }
        
        void operator()() { sig(); }
    };
    
    template<typename RT, typename T0>
    class SORA_API SoraSignal<RT(T0)>: public SoraSignalBase<RT(T0)> {
    public:
        RT sig(T0 a0) {
            typedef SoraSignalBase<RT(T0)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0)>* con = 
                    static_cast<signal::BasicConnection<RT(T0)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0) { sig(a0); }
    };
    
    template<typename T0>
    class SORA_API SoraSignal<void(T0)>: public SoraSignalBase<void(T0)> {
    public:
        void sig(T0 a0) {
            typedef SoraSignalBase<void(T0)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0)>* con = 
                    static_cast<signal::BasicConnection<void(T0)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0) { sig(a0); }
    };
    
    template<typename RT, typename T0, typename T1>
    class SORA_API SoraSignal<RT(T0, T1)>: public SoraSignalBase<RT(T0, T1)> {
    public:
        RT sig(T0 a0, T1 a1) {
            typedef SoraSignalBase<RT(T0, T1)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1) { sig(a0, a1); }
    };
    
    template<typename T0, typename T1>
    class SORA_API SoraSignal<void(T0, T1)>: public SoraSignalBase<void(T0, T1)> {
    public:
        void sig(T0 a0, T1 a1) {
            typedef SoraSignalBase<void(T0, T1)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1) { sig(a0, a1); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2>
    class SORA_API SoraSignal<RT(T0, T1, T2)>: public SoraSignalBase<RT(T0, T1, T2)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2) {
            typedef SoraSignalBase<RT(T0, T1, T2)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2) { sig(a0, a1, a2); }
    };
    
    template<typename T0, typename T1, typename T2>
    class SORA_API SoraSignal<void(T0, T1, T2)>: public SoraSignalBase<void(T0, T1, T2)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2) {
            typedef SoraSignalBase<void(T0, T1, T2)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2) { sig(a0, a1, a2); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3)>: public SoraSignalBase<RT(T0, T1, T2, T3)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3) { sig(a0, a1, a2, a3); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3>
    class SORA_API SoraSignal<void(T0, T1, T2, T3)>: public SoraSignalBase<void(T0, T1, T2, T3)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3) {
            typedef SoraSignalBase<void(T0, T1, T2, T3)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3) { sig(a0, a1, a2, a3); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3, typename T4>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3, T4)>: public SoraSignalBase<RT(T0, T1, T2, T3, T4)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3, T4)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3, T4)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3, T4)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3, a4);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) { sig(a0, a1, a2, a3, a4); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3, typename T4>
    class SORA_API SoraSignal<void(T0, T1, T2, T3, T4)>: public SoraSignalBase<void(T0, T1, T2, T3, T4)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) {
            typedef SoraSignalBase<void(T0, T1, T2, T3, T4)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3, T4)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3, T4)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3, a4);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4) { sig(a0, a1, a2, a3, a4); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3, T4, T5)>: public SoraSignalBase<RT(T0, T1, T2, T3, T4, T5)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3, T4, T5)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3, a4, a5);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) { sig(a0, a1, a2, a3, a4, a5); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    class SORA_API SoraSignal<void(T0, T1, T2, T3, T4, T5)>: public SoraSignalBase<void(T0, T1, T2, T3, T4, T5)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
            typedef SoraSignalBase<void(T0, T1, T2, T3, T4, T5)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3, T4, T5)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3, T4, T5)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3, a4, a5);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) { sig(a0, a1, a2, a3, a4, a5); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3, T4, T5, T6)>: public SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) { sig(a0, a1, a2, a3, a4, a5, a6); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    class SORA_API SoraSignal<void(T0, T1, T2, T3, T4, T5, T6)>: public SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
            typedef SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) { sig(a0, a1, a2, a3, a4, a5, a6); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3, T4, T5, T6, T7)>: public SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6, T7)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6, T7 a7)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6, T7)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6, T7)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6, a7);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) { sig(a0, a1, a2, a3, a4, a5, a6, a7); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    class SORA_API SoraSignal<void(T0, T1, T2, T3, T4, T5, T6, T7)>: public SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6, T7)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
            typedef SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6, T7)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6, T7)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6, T7)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6, a7);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) { sig(a0, a1, a2, a3, a4, a5, a6, a7); }
    };
    
    template<typename RT, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class SORA_API SoraSignal<RT(T0, T1, T2, T3, T4, T5, T6, T7, T8)>: public SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6, T7, T8)> {
    public:
        RT sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            typedef SoraSignalBase<RT(T0, T1, T2, T3, T4, T5, T6, T7 a7, T8 a8)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            RT rtVal;
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* con = 
                    static_cast<signal::BasicConnection<RT(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* >(itConnection->second.get_con_base());
                
                rtVal = con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6, a7, a8);
                ++itConnection;
            }
            return rtVal;
        }
        
        RT operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) { sig(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
    };
    
    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class SORA_API SoraSignal<void(T0, T1, T2, T3, T4, T5, T6, T7, T8)>: public SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6, T7, T8)> {
    public:
        void sig(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
            typedef SoraSignalBase<void(T0, T1, T2, T3, T4, T5, T6, T7, T8)> base_type;
            typename base_type::ConnectionMap::iterator itConnection = base_type::mConnections.begin();
            
            while(itConnection != base_type::mConnections.end()) {
                signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* con = 
                    static_cast<signal::BasicConnection<void(T0, T1, T2, T3, T4, T5, T6, T7, T8)>* >(itConnection->second.get_con_base());
                
                con->slot.get()->fn(a0, a1, a2, a3, a4, a5, a6, a7, a8);
                ++itConnection;
            }
        }
        
        void operator()(T0 a0, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) { sig(a0, a1, a2, a3, a4, a5, a6, a7, a8); }
    };
    
} // namespace sora


#endif
