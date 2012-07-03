//
//  SoraTypeTraits.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraTypeTraits_h
#define SoraTypeTraits_h

namespace sora {
    
    template<bool cont, typename T1, typename T2>
    struct selector {
        typedef T1 type;
    };
    
    template<typename T1, typename T2>
    struct selector<false, T1, T2> {
        typedef T2 type;
    };
    
    template<typename T>
    struct is_void {
        static const bool value = false;
    };
    
    template<>
    struct is_void<void> {
        static const bool value = true;
    };
    
    template<typename T>
    struct is_pointer {
        static const bool value = false;
    };
    
    template<typename T>
    struct is_pointer<T*> {
        static const bool value = true;
    };
    
    template<typename T>
    struct is_member_pointer {
        static const bool value = false;
    };
    
    template<typename T, typename R>
    struct is_member_pointer<R T::*> {
        static const bool value = true;
    };
    
    template<typename T, typename R>
    struct is_member_pointer<R T::* const> {
        static const bool value = true;
    };
    
    template<typename T>
    struct is_reference_wrapper {
        static const bool value = false;
    };
    
    template<typename T>
    struct is_reference_wrapper<T&> {
        static const bool value = true;
    };
    
    template<bool C, typename T1, typename T2>
    struct if_c {
        typedef T1 type;
    };
    
    template<typename T1, typename T2>
    struct if_c<false,T1,T2> {
        typedef T2 type;
    };
    
} // namespace sora

#endif
