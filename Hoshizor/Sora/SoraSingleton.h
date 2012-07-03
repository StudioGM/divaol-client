/*
 *  SoraSingleton.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_SINGLETON_H_
#define SORA_SINGLETON_H_

#include "SoraPlatform.h"

namespace sora {
	
	template<class T>
	class SoraSingleton {
	public:
		static T* Instance() {
			if(!pInstance) pInstance = new T();
			return pInstance;
		}
        static T& GetInstance() {
			if(!pInstance) pInstance = new T();
			return *pInstance;
		}

		static void Destroy() {
			if(pInstance) 
				delete pInstance;
			pInstance = NULL;
		}

	private:
		static T* pInstance;
        
	protected:
		SoraSingleton() {}
		virtual ~SoraSingleton() {}
	};

	template<class T>
	T* SoraSingleton<T>::pInstance = NULL;
    
    /**
     *  You add the following for use with the singleton
     *  template<> T* SoraDirectSingleton<T>::_singleton = NULL
     **/
    
    template<class T>
    class SoraDirectSingleton {
    protected:
        static T * _singleton;
   
    public:
        SoraDirectSingleton() {
            assert( !_singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200
            int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
            _singleton = (T*)((int)this + offset);
#else
            _singleton = static_cast< T* >( this );
#endif
        }
        
        virtual ~SoraDirectSingleton() {
            assert(_singleton);
            _singleton = NULL;
        }
        
        static T& RefInstance() {
            assert(_singleton);
            return(*_singleton);
        }
        
        static T* Instance() {
            return (_singleton);
        }
    };
 
	
} // namespace sora

#endif // SORA_SINGLETON_H