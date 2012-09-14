/****************************************************
 *  Ptr.h
 *
 *  Created by tempbuffer on 8/13/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Several kinds of smart pointers
 ****************************************************/

#ifndef _BASE_PTR_H_
#define _BASE_PTR_H_

#include "Type.h"
#include "Exception.h"
#include "Uncopyable.h"

namespace Base
{
	template<typename T, class RP>
	class SharedPtr;

	template<typename T, class RP>
	class ScopedPtr;

	template<typename T>
	class WeakPtr;

	class RefCounter
	{
	public:
		RefCounter(): mRef(1), mWeakRef(0) {}

		void incRef() {
			++mRef;
		}

		int32 decRef() {
			return --mRef;
		}

		int32 getRef() const {
			return mRef;
		}

		void incWeakRef() {
			++mWeakRef;
		}

		int32 decWeakRef() {
			return --mWeakRef;
		}

		int32 getWeakRef() const {
			return mWeakRef;
		}

		void destruct() {
			delete this;
		}

		void release() {
			if(getRef() != 0)
				return;
			weakRelease();
		}

		void weakRelease() {
			if(getWeakRef()==0)
				destruct();
		}
		
	private:
		int32 mRef;
		int32 mWeakRef;
	};

	template<typename T>
	struct PtrReleasePolicy {
		static T* Alloc() {
			return new T();
		}

		static void Release(T *obj) {
			if(obj)
				delete obj;
		}
	};
	template<typename T>
	struct PtrReleaseArrayPolicy {
		static T* Alloc() {
			return 0;
		}

		static void Release(T* obj) {
			if(obj)
				delete []obj;
		}
	};
	template<typename T>
	struct PtrFreeReleasePolicy {
		static T* Alloc() {
			return base_malloc_withtype(T, 1);
		}

		static void Release(T *obj) {
			if(obj)
				base_free(obj);
		}
	};

	template<typename T, class RP=PtrReleasePolicy<T>>
	class SharedPtr
	{
		#define BASE_SHARED_PTR_AUTO_OPERATOR(op)\
			bool operator op (T* other) const { return get() op other; }\
			bool operator op (const T* other) const { return get() op other; }\
			bool operator op (const SharedPtr &other) const { return get() op other.get(); }

	public:
		typedef T element_type;

		SharedPtr():mPtr(0),mCounter(new RefCounter) {}
		SharedPtr(T *ptr):mPtr(ptr),mCounter(new RefCounter) {}
		template<class Other, class OtherRP>
		SharedPtr(const SharedPtr<Other, OtherRP>& rhs):
			mPtr(static_cast<T*>(rhs.get())),mCounter(rhs.mCounter)
		{
			mCounter->incRef();
		}
		SharedPtr(const SharedPtr& rhs):
			mPtr(rhs.mPtr),mCounter(rhs.mCounter)
		{
			mCounter->incRef();
		}
		~SharedPtr() {
			release();
		}

		SharedPtr& assign(T* _ptr) {
			if(get() != _ptr) {
				RefCounter *tmp = new RefCounter();
				release();
				mCounter = tmp;
				mPtr = _ptr;
			}
			return *this;
		}
		SharedPtr& assign(const SharedPtr &ptr) {
			if(this != &ptr) {
				SharedPtr tmp(ptr);
				swap(tmp);
			}
			return *this;
		}
		template<class Other, class OtherRP>
		SharedPtr& assign(const SharedPtr<Other,OtherRP> &ptr) {
			if(mPtr != ptr.get()) {
				SharedPtr tmp(ptr);
				swap(tmp);
			}
			return tmp;
		}
		void reset(T *_ptr=0) {
			assign(_ptr);
		}
		void alloc() {
			reset(RP::Alloc());
		}

		SharedPtr& operator=(const SharedPtr &other) {
			return assign(other);
		}
		SharedPtr& operator=(T *other){
			return assign(other);
		}
		template<class Other, class OtherRP>
		SharedPtr& operator=(const SharedPtr<Other,OtherRP> &other) {
			return assign(other);
		}

		void swap(SharedPtr &other) {
			std::swap(mPtr,other.mPtr);
			std::swap(mCounter,other.mCounter);
		}

		template<class Other, class OtherRP>
		SharedPtr<Other, OtherRP> cast() const {
			Other *other = dynamic_cast<Other*>(mPtr);
			if(other)
				return SharedPtr<Other,OtherRP>(other,mCounter);

			return SharedPtr<Other,OtherRP>();
		}

		template<class Other, class OtherRP>
		SharedPtr<Other, OtherRP> unsafeCast() const {
			Other *other = static_cast<Other*>(mPtr);
			return SharedPtr<Other, OtherRP>(other,mCounter);
		}

		inline T* get() const {
			return mPtr;
		}
		inline T* deref() const {
			if(!mPtr)
				BASE_THROW_EXCEPTION("this ptr is invalid");
			return mPtr;
		}

		inline T* operator->() { return this->deref(); }
		inline T* operator->() const { return this->deref(); }
		inline T& operator*() { return *this->deref(); }
		inline T& operator*() const { return *this->deref(); }
		operator T*() { return mPtr; }
		operator const T*() const { return mPtr; }
		operator bool() { return mPtr!=0; }
		bool operator!() const { return mPtr==0; }
		intPtr operator-(const SharedPtr& other) const {return get()-other.get();}
		intPtr operator-(const T* other) const {return get()-other;}
		intPtr operator-(T* other) const {return get()-other;}
		BASE_SHARED_PTR_AUTO_OPERATOR(==);
		BASE_SHARED_PTR_AUTO_OPERATOR(!=);
		BASE_SHARED_PTR_AUTO_OPERATOR(>);
		BASE_SHARED_PTR_AUTO_OPERATOR(<);
		BASE_SHARED_PTR_AUTO_OPERATOR(>=);
		BASE_SHARED_PTR_AUTO_OPERATOR(<=);

		bool isValid() const {
			return mPtr!=0;
		}
		bool isNull() const {
			return mPtr==0;
		}
		int32 useCount() const {
			return mCounter->getRef();
		}

	private:
		void release() {
			base_assert(mCounter);
			if(mCounter->decRef()==0) {
				RP::Release(mPtr);

				mPtr = 0;
				mCounter->release();
			}
		}

		SharedPtr(T* ptr, RefCounter *pCounter)
			:mCounter(pCounter), mPtr(ptr) {
			base_assert(pCounter);
			pCounter->incRef();
		}
	private:
		T *mPtr;
		RefCounter *mCounter;

		template<class Other, class OtherRP>
		friend class SharedPtr;
		template<class Other>
		friend class WeakPtr;
	};

	template<typename T>
	class WeakPtr 
	{
	public:
		typedef T element_type;

		WeakPtr():mPtr(0),mCounter(0) {}
		WeakPtr(const WeakPtr<T> &ptr)
			:mCounter(ptr.mCounter) {
			if(mCounter) {
				mCounter->incWeakRef();
				mPtr = ptr.lock().get();
			}
		}
		template<class Other, class OtherRP>
		WeakPtr(const SharedPtr<Other, OtherRP>& ptr)
			:mPtr(ptr.get()), mCounter(ptr.mCounter) {
			if(mCounter)
				mCounter->incWeakRef();
		}
		template<class Other>
		WeakPtr(const WeakPtr<Other>& ptr)
			:mCounter(ptr.mCounter) {
			if(mCounter) {
				mCounter->incWeakRef();
				mPtr = ptr.lock().get();
			}
		}
		~WeakPtr() {
			if(mCounter&&mCounter->decWeakRef()==0)
				mCounter->release();
		}

		WeakPtr& operator=(const WeakPtr &other) {
			WeakPtr tmp(other);
			swap(tmp);
			return *this;
		}
		template<class Other, class OtherRP>
		WeakPtr& operator=(const SharedPtr<Other,OtherRP> &other) {
			WeakPtr tmp(other);
			swap(tmp);
			return *this;
		}
		template<class Other>
		WeakPtr& operator=(const WeakPtr<Other> &other) {
			WeakPtr tmp(other);
			swap(tmp);
			return *this;
		}
		int32 use_count() const {
			return mCounter ? mCounter->getRef() : 0;
		}
		int32 weak_count() const {
			return mCounter ? mCounter->getWeakRef() : 0;
		}
		bool expired() const {
			return !mCounter || (mCounter && mCounter->getRef()==0);
		}
		SharedPtr<T> lock() const {
			return SharedPtr<T>(mPtr, mCounter);
		}
		void reset() {
			mPtr = 0;
			if(mCounter)
				mCounter->decWeakRef();
			mCounter = 0;
		}
		void swap(WeakPtr<T> &other) {
			std::swap(mPtr, other.mPtr);
			std::swap(mCounter, other.mCounter);
		}

	private:
		T* mPtr;
		RefCounter *mCounter;

		template<class Other>
		friend class WeakPtr;
	};

	template<typename T, class RP=PtrReleasePolicy<T>>
	class ScopedPtr : public Uncopyable 
	{
	public:
		typedef T element_type;

		explicit ScopedPtr(T* _ptr):mPtr(_ptr) {}
		ScopedPtr():mPtr(0) {}
		~ScopedPtr() {
			RP::Release(mPtr);
		}

		void reset(T* _ptr = 0) {
			mPtr = _ptr;
		}
		void alloc() {
			reset(RP::Alloc());
		}

		inline T& operator*() const {
			return *deref();
		}
		inline T* operator->() const {
			return deref();
		}
		inline T* get() const {
			return mPtr;
		}
		inline T* deref() const {
			base_assert(mPtr!=0);
			return mPtr;
		}
		operator bool() const {
			return mPtr!=0;
		}
		bool operator!() const {
			return mPtr==0;
		}
		void swap(ScopedPtr &other) {
			std::swap(mPtr,other.mPtr);
		}

	private:
		T* mPtr;
	};
}

#endif