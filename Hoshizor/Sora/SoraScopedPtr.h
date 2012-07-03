//
//  SoraScopedPtr.h
//  Sora
//
//  Created by Robert Bu on 8/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraScopedPtr_h
#define Sora_SoraScopedPtr_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"

namespace sora {
    
    /**
     * Simple boost::scoped_ptr impl
     **/
    
    template<class T>
    class SoraScopedPtr: SoraUncopyable {
    public:
        explicit SoraScopedPtr(T* _ptr):
        ptr(_ptr) {
        }
        SoraScopedPtr():
        ptr(0) {
            
        }
        ~SoraScopedPtr() {
            if(ptr) {
                delete ptr;
                ptr = 0;
            }
        }
        
        void reset(T* p = 0) {
            ptr = p;
        }
        
        T& operator*() const {
            return *ptr;
        }
        T* operator->() const {
            return ptr;
        }
        
        T* get() const {
            return ptr;
        }
        
        void swap(SoraScopedPtr& b);
        
    private:
        T* ptr;
    };
    
    template<class T>
    void swap(SoraScopedPtr<T>& a, SoraScopedPtr<T>& b) {
        std::swap(a.ptr, b.ptr);
    }
    
} // namespace sora

#endif
