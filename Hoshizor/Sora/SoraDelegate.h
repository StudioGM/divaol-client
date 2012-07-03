//
//  SoraDelegate.h
//  Sora
//
//  Created by Robert Bu on 7/8/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDelegate_h
#define Sora_SoraDelegate_h

#include <cassert>

#include "SoraTimestamp.h"
#include "SoraAny.h"
#include "SoraAutoPtr.h"

namespace sora {
    
    /**
     *  Base class for delegate
     */
    template<typename TARG, typename TRVAL=void>
    class SoraAbstractDelegate {
    public:
        SoraAbstractDelegate(void* target): mTarget(target) {
        //    assert(target != NULL);
        }
        
        SoraAbstractDelegate(const SoraAbstractDelegate& del): mTarget(del.mTarget) {
            assert(mTarget != NULL);
        }
        
        virtual ~SoraAbstractDelegate() {}
        
        virtual TRVAL notify(const void* sender, TARG& arguments) = 0;
        virtual SoraAbstractDelegate* clone() const = 0;
        
        bool operator <(const SoraAbstractDelegate<TARG>& rhs) const {
            return mTarget < rhs.mTarget;
        }
    
        void* target() const {
            return mTarget;
        }
        
    protected:
        void* mTarget;
    };
    
    template<class TOBJ, class TARGS, class TRVAL=void, bool withSenter=true>
    class SoraDelegate: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        typedef TRVAL (TOBJ::*NotifyMethod)(const void*, TARGS&);
        
        SoraDelegate(TOBJ* obj, NotifyMethod method):
        SoraAbstractDelegate<TARGS, TRVAL>(obj),
        mReceiver(obj),
        mReceiverMethod(method) {
        }
        
        SoraDelegate(const SoraDelegate& delegate):
        SoraAbstractDelegate<TARGS, TRVAL>(delegate),
        mReceiver(delegate.mReceiver),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraDelegate() {
            
        }
        
        SoraDelegate& operator = (const SoraDelegate& delegate) {
            if(&delegate != this) {
                this->mTarget = delegate.mTarget;
                this->mReceiver = delegate.mReceiver;
                this->mReceiverMethod = delegate.mReceiverMethod;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            return (mReceiver->*mReceiverMethod)(sender, args);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraDelegate(*this);
        }
        
    protected:
        TOBJ* mReceiver;
        NotifyMethod mReceiverMethod;
        
    private:
        SoraDelegate();
    };
    
    template<class TOBJ, class TARGS, class TRVAL>
    class SoraDelegate<TOBJ, TARGS, TRVAL, false>: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        typedef TRVAL (TOBJ::*NotifyMethod)(TARGS&);
        
        SoraDelegate(TOBJ* obj, NotifyMethod method):
        SoraAbstractDelegate<TARGS, TRVAL>(obj),
        mReceiver(obj),
        mReceiverMethod(method) {
        }
        
        SoraDelegate(const SoraDelegate& delegate):
        SoraAbstractDelegate<TARGS, TRVAL>(delegate),
        mReceiver(delegate.mReceiver),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraDelegate() {
            
        }
        
        SoraDelegate& operator = (const SoraDelegate& delegate) {
            if(&delegate != this) {
                this->mTarget = delegate.mTarget;
                this->mReceiver = delegate.mReceiver;
                this->mReceiverMethod = delegate.mReceiverMethod;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            return (mReceiver->*mReceiverMethod)(args);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraDelegate(*this);
        }
        
    protected:
        TOBJ* mReceiver;
        NotifyMethod mReceiverMethod;
        
    private:
        SoraDelegate();
    };
    
    // enables to use a C Function as Delegate
    template<class TARGS, class TRVAL, bool hasSender=true, bool senderIsConst=false>
    class SoraFunctionDelegate: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        typedef TRVAL (*NotifyMethod)(const void*, TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS, TRVAL>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
        
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS, TRVAL>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            return (*mReceiverMethod)(sender, args);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };
    
    template<class TARGS, class TRVAL>
    class SoraFunctionDelegate<TARGS, TRVAL, true, false>: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        typedef TRVAL (*NotifyMethod)(void* sender, TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS, TRVAL>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
            
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS, TRVAL>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            void* ncsender = const_cast<void*>(sender);
            return (*mReceiverMethod)(ncsender, args);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };

    template<class TARGS, class TRVAL, bool senderIsConst>
    class SoraFunctionDelegate<TARGS, TRVAL, false, senderIsConst>: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        typedef TRVAL (*NotifyMethod)(TARGS&);
        
        SoraFunctionDelegate(NotifyMethod method):
        SoraAbstractDelegate<TARGS, TRVAL>(*reinterpret_cast<void**>(&method)),
        mReceiverMethod(method) {
            
        }
        
        SoraFunctionDelegate(const SoraFunctionDelegate& delegate):
        SoraAbstractDelegate<TARGS, TRVAL>(delegate),
        mReceiverMethod(delegate.mReceiverMethod) {
        }
        
        ~SoraFunctionDelegate() {
            
        }
        
        SoraFunctionDelegate& operator=(const SoraFunctionDelegate& rhs) {
            if(&rhs != this) {
                this->mTarget = rhs.mTarget;
                this->mReceiverMethod = rhs.mReceiverMethod;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            return (*mReceiverMethod)(args);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraFunctionDelegate(*this);
        }
        
    protected:
        NotifyMethod mReceiverMethod;
        
    private:
        SoraFunctionDelegate();
    };
    
    template<class TARGS, class TRVAL>
    class SoraExpireDelegate: public SoraAbstractDelegate<TARGS, TRVAL> {
    public:
        SoraExpireDelegate():
        SoraAbstractDelegate<TARGS, TRVAL>(NULL),
        mDelegate(NULL),
        mExpire(0) {}
        
        SoraExpireDelegate(const SoraAbstractDelegate<TARGS>& p, SoraTimestamp::TimeDiff expireMilliseconds):
        SoraAbstractDelegate<TARGS, TRVAL>(p),
        mDelegate(p.clone()),
        mExpire(expireMilliseconds*1000) {
        
        }
        
        SoraExpireDelegate(const SoraExpireDelegate& exp):
        SoraAbstractDelegate<TARGS, TRVAL>(exp),
        mDelegate(exp.mDelegate->clone()),
        mExpire(exp.mExpire),
        mCreationTime(exp.mCreationTime) {
        }
        
        ~SoraExpireDelegate() {
            destroy();
        }
        
        SoraExpireDelegate& operator=(const SoraExpireDelegate& exp) {
            if(&exp != this) {
                delete this->delegate;
                this->mDelegate = exp.mDelegate->clone();
                this->mExpire = exp.mExpire;
                this->mCreationTime = exp.mCreationTime;
                this->mTarget = exp.mTarget;
            }
            return *this;
        }
        
        TRVAL notify(const void* sender, TARGS& args) {
            if(!expired() && mDelegate) {
                return this->mDelegate->notify(sender, args);
            } else
                return false;
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* clone() const {
            return new SoraExpireDelegate(this);
        }
        
        void destroy() {
            if(this->mDelegate) {
                delete this->mDelegate;
                this->mDelegate = NULL;
            }
        }
        
        const SoraAbstractDelegate<TARGS, TRVAL>& getDelegate() const {
            return this->mDelegate;
        }
        
    protected:
        bool expired() const {
            if(mExpire == 0)
                return false;
            return mCreationTime.isElapsed(mExpire);
        }
        
        SoraAbstractDelegate<TARGS, TRVAL>* mDelegate;
        SoraTimestamp::TimeDiff mExpire;
        SoraTimestamp mCreationTime;
        
    private:
    };
    
    template<class TOBJ, class TARGS, class TRVAL>
    static SoraDelegate<TOBJ, TARGS, TRVAL, true> Delegate(TOBJ* obj, TRVAL (TOBJ::*NotifyMethod)(const void*, TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, TRVAL, true>(obj, NotifyMethod);
    }
                            
    template<class TOBJ, class TARGS, class TRVAL>
    static SoraDelegate<TOBJ, TARGS, TRVAL, false> Delegate(TOBJ* obj, TRVAL (TOBJ::*NotifyMethod)(TARGS&)) {
        return SoraDelegate<TOBJ, TARGS, TRVAL, false>(obj, NotifyMethod);
    }
    
    template<class TARGS, class TRVAL>
    static SoraFunctionDelegate<TARGS, TRVAL, true, true> Delegate(TRVAL (*NotifyMethod)(const void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, TRVAL, true, true>(NotifyMethod);
    }
    
    template<class TARGS, class TRVAL>
    static SoraFunctionDelegate<TARGS, TRVAL, true, false> Delegate(TRVAL (*NotifyMethod)(void* sender, TARGS&)) {
        return SoraFunctionDelegate<TARGS, TRVAL, true, false>(NotifyMethod);
    }
    
    template<class TARGS, class TRVAL>
    static SoraFunctionDelegate<TARGS, TRVAL, false> Delegate(TRVAL (*NotifyMethod)(TARGS&)) {
        return SoraFunctionDelegate<TARGS, TRVAL, false>(NotifyMethod);
    }
    
    template<class TOBJ, class TARGS, class TRVAL>
    static SoraExpireDelegate<TARGS, TRVAL> Delegate(TOBJ* obj, TRVAL (TOBJ::*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS, TRVAL>(Delegate<TOBJ, TARGS, TRVAL, true>(obj, NotifyMethod), expireMs);
    }
    
    template<class TOBJ, class TARGS, class TRVAL>
    static SoraExpireDelegate<TARGS, TRVAL> Delegate(TOBJ* obj, TRVAL (TOBJ::*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS, TRVAL>(Delegate<TOBJ, TARGS, TRVAL, false>(obj, NotifyMethod), expireMs);
    }
    
    template<class TARGS, class TRVAL>
    static SoraExpireDelegate<TARGS, TRVAL> Delegate(TRVAL (*NotifyMethod)(const void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS, TRVAL>(Delegate<TARGS, TRVAL, true, true>(NotifyMethod), expireMs);
    }
    
    template<class TARGS, class TRVAL>
    static SoraExpireDelegate<TARGS, TRVAL> Delegate(TRVAL (*NotifyMethod)(void*, TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS, TRVAL>(Delegate<TARGS, TRVAL, true, false>(NotifyMethod), expireMs);
    }
    
    template<class TARGS, class TRVAL>
    static SoraExpireDelegate<TARGS, TRVAL> Delegate(TRVAL (*NotifyMethod)(TARGS&), SoraTimestamp::TimeDiff expireMs) {
        return SoraExpireDelegate<TARGS, TRVAL>(Delegate<TARGS, TRVAL, false>(NotifyMethod), expireMs);
    }     
} // namespace sora


#endif
