/****************************************************
 *  Any.h
 *
 *  Created by tempbuffer on 9/17/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A object to contain anything
 ****************************************************/

#ifndef _BASE_ANY_H_
#define _BASE_ANY_H_

#include "Type.h"
#include "Ptr.h"

#include <vector>

namespace Base
{
	class Any
	{
	public:
		Any():mContent(0) {
		}

		template<typename ValueType>
		Any(const ValueType &value)
			:mContent(new Holder<ValueType>(value)) {
		}

		Any(const Any &rhs)
			:mContent(rhs.mContent?rhs.mContent->clone():0) {
		}

		~Any() {
		}

		template<typename Type>
		void as(Type &value, bool unsafe = false) {
			value = get<Type>(unsafe);
		}

		template<typename Type>
		Type as(bool unsafe = false) {
			return get<Type>(unsafe);
		}

		template<typename Type>
		const Type as(bool unsafe = false) const {
			return get<Type>(unsafe);
		}

		template<typename Type>
		Type& get(bool unsafe = false) {
			if(mContent && (unsafe || type() == typeid(Type)) )
				return static_cast<Any::Holder<Type>*>(mContent.get())->value();
			else
				BASE_THROW_EXCEPTION(String("Can not convert ")+type().name()+" to "+typeid(Type).name());
		}

		template<typename Type>
		const Type& get(bool unsafe = false) const {
			if(mContent && (unsafe || type() == typeid(Type)) )
				return static_cast<Any::Holder<Type>*>(mContent.get())->value();
			else
				BASE_THROW_EXCEPTION(String("Can not convert ")+type().name()+" to "+typeid(Type).name());
		}

		Any& swap(Any &rhs) {
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType>
		Any& operator=(const ValueType &rhs) {
			Any(rhs).swap(*this);
			return *this;
		}

		Any& operator=(const Any &rhs) {
			Any(rhs).swap(*this);
			return *this;
		}

		bool empty() const {
			return !mContent;
		}

		const std::type_info& type() const {
			return mContent ? mContent->type() : typeid(void);
		}

	protected:
		class HolderBase {
		public:
			virtual ~HolderBase() {

			}

			virtual const std::type_info& type() const = 0;
			virtual SharedPtr<HolderBase> clone() const = 0;
		};

		template<typename ValueType>
		class Holder : public HolderBase {
			typedef ValueType value_type;
		public:
			Holder(const ValueType &value):
				mValue(value) {
			}

			virtual const std::type_info& type() const {
				return typeid(ValueType);
			}

			virtual SharedPtr<HolderBase> clone() const {
				return new Holder(mValue);
			}

			ValueType& value() {return mValue;}
			const ValueType& value() const {return mValue;}

		private:
			ValueType mValue;
		};

	private:
		SharedPtr<HolderBase> mContent;
	};

	typedef std::vector<Any> AnyArray;
}

#endif