//
//  SoraSingletonHolder.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraSingletonHolder_h
#define Sora_SoraSingletonHolder_h

namespace sora {
    
    /**
     * This is a helper template class for managing singleton objects allocated on the heap
     * static SoraSingletonHolder<MyClass> MyClass etc
     **/
    
    template<class T, typename P>
    class SORA_API SoraSingletonHolder {
    public:
        SoraSingletonHolder():
        mInstance(0) {
            
        }
        
        ~SoraSingletonHolder() {
            if(mInstance)
                delete mInstance;
        }
        
        T* get(P arg) {
            if(!mInstance) mInstance = new T(arg);
            return mInstance;
        }
        
    private:
        T* mInstance;
    };
    
    template<typename T>
    class SORA_API SoraSingletonHolder<T, void> {
    public:
        SoraSingletonHolder():
        mInstance(0) {
            
        }
        
        ~SoraSingletonHolder() {
            if(mInstance)
                delete mInstance;
        }
        
        T* get(void) {
            if(!mInstance) mInstance = new T();
            return mInstance;
        }
        
    private:
        T* mInstance;
    };
    
} // namespace sora


#endif
