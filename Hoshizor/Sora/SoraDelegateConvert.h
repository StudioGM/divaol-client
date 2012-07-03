//
//  SoraDelegateConvert.h
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDelegateConvert_h
#define Sora_SoraDelegateConvert_h

#include "SoraDelegate.h"

namespace sora {
        
    template<typename TARG, bool RETVAL=true>
    class SoraVoidDelegate2Bool: public SoraAbstractDelegate<TARG, bool> {
    public:
        SoraVoidDelegate2Bool(const SoraAbstractDelegate<TARG, void>& del):
        SoraAbstractDelegate<TARG, bool>(del.target()),
        mDelegate(del.clone()) {
            mRetVal = RETVAL;
        }
        
        SoraVoidDelegate2Bool(const SoraVoidDelegate2Bool& rhs):
        SoraAbstractDelegate<TARG, bool>(rhs.target()),
        mDelegate(rhs.mDelegate->clone()) {
            mRetVal = RETVAL;
        }
        
        ~SoraVoidDelegate2Bool() {
            destroy();
        }
        
        void destroy() {
            if(mDelegate) {
                delete mDelegate;
                mDelegate = NULL;
            }
        }
        
        bool notify(const void* sender, TARG& arugments) {
            assert(mDelegate);
            mDelegate->notify(sender, arugments);
            return mRetVal;
        }
        
        SoraAbstractDelegate<TARG, bool>* clone() const {
            return new SoraVoidDelegate2Bool(*this);
        }
        
    private:
        SoraAbstractDelegate<TARG, void>* mDelegate;
        bool mRetVal;
    };
    
    template<typename TARG>
    class SoraBoolDelegate2Void: public SoraAbstractDelegate<TARG, void> {
    public:
        SoraBoolDelegate2Void(const SoraAbstractDelegate<TARG, bool>& del):
        SoraAbstractDelegate<TARG, void>(del.target()),
        mDelegate(del.clone()) {
            
        }
        
        SoraBoolDelegate2Void(const SoraBoolDelegate2Void& rhs):
        SoraAbstractDelegate<TARG, void>(rhs.target()),
        mDelegate(rhs.mDelegate->clone()) {
            
        }
        
        ~SoraBoolDelegate2Void() {
            destroy();
        }
        
        void destroy() {
            if(mDelegate) {
                delete mDelegate;
                mDelegate = NULL;
            }
        }
        
        void notify(const void* sender, TARG& arugments) {
            assert(mDelegate);
            mDelegate->notify(sender, arugments);
        }
        
        SoraAbstractDelegate<TARG, void>* clone() const {
            return new SoraBoolDelegate2Void(*this);
        }
        
    private:
        SoraAbstractDelegate<TARG, bool>* mDelegate;
    };
    
    template<typename TARG, int RETVAL=0>
    class SoraVoidDelegate2Int: public SoraAbstractDelegate<TARG, int> {
    public:
        SoraVoidDelegate2Int(const SoraAbstractDelegate<TARG, void>& del):
        SoraAbstractDelegate<TARG, int>(del.target()),
        mDelegate(del.clone()) {
            mRetVal = RETVAL;
        }
        
        SoraVoidDelegate2Int(const SoraVoidDelegate2Int& rhs):
        SoraAbstractDelegate<TARG, int>(rhs.target()),
        mDelegate(rhs.mDelegate->clone()) {
            mRetVal = RETVAL;
        }
        
        ~SoraVoidDelegate2Int() {
            destroy();
        }
        
        void destroy() {
            if(mDelegate) {
                delete mDelegate;
                mDelegate = NULL;
            }
        }
        
        int notify(const void* sender, TARG& arugments) {
            assert(mDelegate);
            mDelegate->notify(sender, arugments);
            return mRetVal;
        }
        
        SoraAbstractDelegate<TARG, int>* clone() const {
            return new SoraVoidDelegate2Int(*this);
        }
        
    private:
        SoraAbstractDelegate<TARG, void>* mDelegate;
        int mRetVal;
    };
    
    template<typename TARG>
    class SoraIntDelegate2Void: public SoraAbstractDelegate<TARG, void> {
    public:
        SoraIntDelegate2Void(const SoraAbstractDelegate<TARG, int>& del):
        SoraAbstractDelegate<TARG, void>(del.target()),
        mDelegate(del.clone()) {
        }
        
        SoraIntDelegate2Void(const SoraIntDelegate2Void& rhs):
        SoraAbstractDelegate<TARG, void>(rhs.target()),
        mDelegate(rhs.mDelegate->clone()) {
        }
        
        ~SoraIntDelegate2Void() {
            destroy();
        }
        
        void destroy() {
            if(mDelegate) {
                delete mDelegate;
                mDelegate = NULL;
            }
        }
        
        void notify(const void* sender, TARG& arugments) {
            assert(mDelegate);
            mDelegate->notify(sender, arugments);
        }
        
        SoraAbstractDelegate<TARG, void>* clone() const {
            return new SoraIntDelegate2Void(*this);
        }
        
    private:
        SoraAbstractDelegate<TARG, int>* mDelegate;
    };
    
} // namespace sora

#endif
