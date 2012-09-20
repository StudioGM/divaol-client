/****************************************************
 *  TypeTraits.h
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  
 ****************************************************/

#ifndef _BASE_TYPE_TRAITS_H_
#define _BASE_TYPE_TRAITS_H_

namespace Base
{
	namespace Traits {
		struct Unusable {
			Unusable() {}
			template<typename T> Unusable(const T&) {}
		};

		template<bool cont, typename T1, typename T2>
		struct if_selector {
			typedef T1 type;
		};

		template<typename T1, typename T2>
		struct if_selector<false, T1, T2> {
			typedef T2 type;
		};

		template<typename T>
		struct normalize_void {
			typedef T type;
		};
		
		template<>
		struct normalize_void<void> {
			typedef Unusable type;
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
		struct strip_point {
			typedef T type;
		};

		template<typename T>
		struct strip_point<T*> {
			typedef T type;
		};

		template<typename T>
		struct strip_qualifier {
			typedef T type;
		};

		template<typename T>
		struct strip_qualifier<T*> {
			typedef T type;
		};

		template<typename T>
		struct strip_qualifier<T&> {
			typedef T type;
		};

		template<typename T>
		struct strip_qualifier<const T> {
			typedef T type; 
		};

		template<typename T>
		struct strip_qualifier<volatile T> {
			typedef T type;
		};

		#define BASE_STRIP_QUALIFIER_1(T) Base::Traits::strip_qualifier<T>::type
		#define BASE_STRIP_QUALIFIER_2(T) Base::Traits::strip_qualifier<BASE_STRIP_QUALIFIER_1(T)>::type
		#define BASE_STRIP_QUALIFIER_3(T) Base::Traits::strip_qualifier<BASE_STRIP_QUALIFIER_2(T)>::type
		#define BASE_STRIP_QUALIFIER(T) BASE_STRIP_QUALIFIER_3(T)  

		template<typename T>
		struct strip_array;

		template<typename T, size_t LENGTH>
		struct strip_array<T[LENGTH]> {
			typedef T type;

			enum{ Length = LENGTH };
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
		struct is_referrence_wrapper {
			static const bool value = false;
		};

		template<typename T>
		struct is_referrence_wrapper<T&> {
			static const bool value = true;
		};

		template<typename T>
		struct is_pod {
			enum {value = false};
		};

		template<> struct is_pod<int32>         { enum { value = true }; };
		template<> struct is_pod<uint32>        { enum { value = true }; };
		template<> struct is_pod<intPtr>        { enum { value = true }; };
		template<> struct is_pod<uintPtr>       { enum { value = true }; };
		template<> struct is_pod<uint8>         { enum { value = true }; };
		template<> struct is_pod<uint16>        { enum { value = true }; };
		template<> struct is_pod<int8>          { enum { value = true }; };
		template<> struct is_pod<int16>         { enum { value = true }; };
		template<> struct is_pod<uint64>        { enum { value = true }; };
		template<> struct is_pod<int64>         { enum { value = true }; };
		template<> struct is_pod<char>          { enum { value = true }; };
		template<> struct is_pod<wchar_t>       { enum { value = true }; };
		template<typename T> struct is_pod<T*>  { enum { value = true }; };
		template<typename T> struct is_pod<T&>  { enum { value = true }; };

		template<typename T, intPtr size> struct is_pod<T[size]> { enum { value = is_pod<T>::value }; };
		template<typename T, typename C> struct is_pod<T C::*> { enum { value = true }; };
		template<typename T> struct is_pod<const T>	 { enum { value = is_pod<T>::value }; };
		template<typename T> struct is_pod<volatile T> { enum { value = is_pod<T>::value }; };
	}
}

#endif