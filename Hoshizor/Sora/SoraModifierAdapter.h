//
//  SoraImageEffectAutomator.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraImageEffectAutomator_h
#define Sora_SoraImageEffectAutomator_h

#include "SoraPlatform.h"
#include "SoraModifier.h"
#include "SoraFunction.h"
#include <list>

namespace sora {
    
    /**
     *  Adapter that helps apply modifiers to any class that can manage a modifier
     *  Such as SoraObject
     **/
    
    class SORA_API SoraAbstractModifierAdapter {
    public:
        virtual ~SoraAbstractModifierAdapter() {}

        virtual bool update(float dt) = 0;
    };
    
    template<typename T>
    class SORA_API SoraModifierAdapter: public SoraAbstractModifierAdapter {
    public:
        SoraModifierAdapter(T* obj, SoraModifier<T>* modi, bool insert=false) {
            this->mObj = obj;
            this->mModifier = modi;
            
            this->mInsert = insert;
            if(this->mInsert)
                obj->addModifierAdapter(this);
        }
        
        virtual ~SoraModifierAdapter() {
            if(this->mInsert)
                this->mObj->removeModifierAdapter(this);
        }
        
        typedef SoraFunction<void(T*)> FinishNotification;
        void setFinishNotification(const FinishNotification& func) {
            this->mOnFinish = func;
        }
        
        bool update(float dt) {
            if(this->mModifier) {
                bool result = this->mModifier->update(dt);
                this->mModifier->modify(this->mObj);
                
                if(result) { 
                    if(this->mOnFinish) {
                        this->mOnFinish(this->mObj);
                    }
                    
                    if(this->mModifier->isAutoRelease()) {
                        this->mModifier->release();
                        this->mModifier = 0;
                        return true;
                    }
                }
            }
            return false;
        }
        
    protected:
        SoraModifierAdapter();
        
        T* mObj;
        SoraModifier<T>* mModifier;
        
        FinishNotification mOnFinish;
    
        bool mInsert;
    };
    
    template<typename MT>
    inline SoraModifierAdapter<MT>* CreateModifierAdapter(MT* obj, SoraModifier<MT>* modifier, bool insert=false) {
        return new SoraModifierAdapter<MT>(obj, modifier, insert);
    }
    
    template<typename MT>
    inline SoraModifierAdapter<MT>* CreateModifierAdapterWithNotification(MT* obj, 
                                                                          SoraModifier<MT>* modifier, 
                                                                          const SoraFunction<void(MT*)>& notify,
                                                                          bool insert=false) {
        SoraModifierAdapter<MT>* adapter = new SoraModifierAdapter<MT>(obj, modifier, insert);
        adapter->setFinishNotification(notify);
        return adapter;
    }
} // namespace sora



#endif
