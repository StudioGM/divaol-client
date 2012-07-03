//
//  SoraEntity.h
//  Sora
//
//  Created by Robert Bu on 7/31/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraEntity_h
#define Sora_SoraEntity_h


#include "SoraEntityListener.h"
#include "SoraComponentHolder.h"

namespace sora {
    
#define SORA_DEF_ENTITY(class, description) \
public: \
static ::sora::SoraEntity* create() { \
::sora::SoraEntity* ent = new class(); \
return ent; \
} \
static void destroy(::sora::SoraEntity* ent) { \
if(ent) \
delete ent; \
} \
static class* cast(::sora::SoraEntity*);
    
#define SORA_IMPL_ENTITY(class) \
class* class::cast(::sora::SoraEntity* ent) { \
return dynamic_cast<class*>(ent); \
}
    
    typedef DynRttiClassKeyType PropertyId;
    
    class SORA_API SoraEntity: public SoraObject, public SoraEntityListenerManager {
    public:
		SoraEntity();
		virtual ~SoraEntity();
        
        typedef EntityFsmType::StateType FsmStateType;
        typedef EntityFsmType::EventType FsmEventType;
        
        void addFsmState(const FsmStateType&);
        void addFsmTransition(const FsmStateType&, const FsmEventType&, const FsmStateType&);
        void setFsmState(const FsmStateType&);
        void processFsmEvent(const FsmEventType&);
        const FsmStateType& getCurrentFsmState() const;
        
        bool attachVM(ScriptVMPtr, const SoraString& tag);
        ScriptVMPtr detachVM(const SoraString& tag);
        ScriptVMPtr getVM(const SoraString& tag) const;
        
        bool hasComponent(const SoraString& name) const;
        void addComponent(SoraComponent* co);
        void addComponent(const std::string& name);
        SoraComponent* removeComponent(const SoraString& name);
        SoraComponent* removeComponent(const SoraComponent* co);
        SoraComponent* getComponent(const SoraString& name);
        
        void sendMessage(SoraMessageEvent* message);
        void sendMessageTo(const SoraString& comp, SoraMessageEvent* message);
        
        virtual void render();
        virtual int32 update(float dt);
        virtual void onUpdate(float dt);
        virtual void onMessage(SoraMessageEvent* evt);
        
        size_t getPropertySize() const;
        bool hasProperty(const PropertyId& pid);
        void addProperty(SoraPropertyBase* prop);
        SoraPropertyBase* getPropertyBase(const PropertyId& pid) const;
        SoraPropertyBase* removeProperty(const PropertyId& pid, bool release=true);
        
        // template property accessors
        template<typename T>
        void addProperty(const PropertyId& pid, const T& prop);
        template<typename T>
        void setProperty(const PropertyId& pid, const T& prop);
        template<typename T>
        T getProperty(const PropertyId& pid);
        template<typename T>
        SoraProperty<T>* getPropertyPtr(const PropertyId& pid) const;
        template<typename T>
        T getProperty(const PropertyId& pid, const T& defaultValue) const;
        
        // deep copy properties to another entity
        void copyPropertyMap(SoraEntity* entity);
        
        SORA_DEF_ENTITY(SoraEntity, "SoraEntity")
        
        template<typename T>
        SoraConnection subscribeToVMAttached(T);
        template<typename T>
        SoraConnection subscribeToVMDetached(T);
                
    private:
        void onEnter(const EntityFsmType&, const EntityFsmType::StateType&);
        void onExit(const EntityFsmType&, const EntityFsmType::StateType&);
        
    protected:
        EntityFsmType mFsm;
        
        SoraComponentHolder mComponents;
        SoraPropertyHolder mHolder;
        SoraScriptVMHolder mScriptVM;
    };
    
    
    template<typename T>
    inline void SoraEntity::addProperty(const PropertyId& pid, const T& prop) {
        return mHolder.addProperty(pid, prop);
    }
    
    template<typename T>
    inline void SoraEntity::setProperty(const PropertyId& pid, const T& prop) {
        SoraStringTokenlizer tokens(pid);
        mHolder.setProperty(pid, prop);
    }
    
    template<typename T>
    inline T SoraEntity::getProperty(const PropertyId& pid) {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            if(tprop)
                return tprop->get();
        }
        return SoraTypeSerializer::defaultValue<T>();
    }
    
    template<typename T>
    inline SoraProperty<T>* SoraEntity::getPropertyPtr(const PropertyId& pid) const {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            return tprop;
        }
        return 0;
    }
    
    template<typename T>
    inline T SoraEntity::getProperty(const PropertyId& pid, const T& defaultValue) const {
        SoraPropertyBase* prop = getPropertyBase(pid);
        if(prop) {
            SoraProperty<T>* tprop = PropertyCast<T>(prop);
            if(tprop)
                return tprop->get();
        }
        return defaultValue;
    }
    
    template<typename T>
    inline SoraConnection SoraEntity::subscribeToVMAttached(T fn) {
        return mScriptVM.subscribeToVMAttached(fn);
    }
    
    template<typename T>
    inline SoraConnection SoraEntity::subscribeToVMDetached(T fn) {
        return mScriptVM.subscribeToVMDetached(fn);
    }
} // namespace sora


#endif
