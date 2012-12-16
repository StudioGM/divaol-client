/****************************************************
 *  Raw.h
 *
 *  Created by tempbuffer on 12/15/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  RawData to manager binary data
 ****************************************************/

#ifndef _RAW_H_
#define _RAW_H_

#include "Type.h"
#include "String.h"
#include <vector>
using std::vector;

namespace Base
{
	class Raw
	{
		typedef vector<byte> Binary;
		Binary data;
	public:
		Raw() {}
		template<typename T>
		Raw(const T &source) {
			data = Binary(sizeof(source));
			memcpy(&data[0], &source, data.size());
		}
		template<typename T>
		Raw(const vector<T> &source) {
			data = Binary(sizeof(T)*source.size());
			memcpy(&data[0], &source[0], data.size());
		}
		template<>
		Raw(const base_string &source)
		{
			data = Binary(source.size());
			if(source.size()>0)
				memcpy(&data[0], &source[0], data.size());
		}
		template<>
		Raw(const String &source)
		{
			data = Binary(source.size()*sizeof(wchar));
			if(source.size()>0)
				memcpy(&data[0], &source.asUnicode()[0], data.size());
		}
		template<typename T>
		Raw(const T &source, uint8 size) {
			data = Binary(size);
			if(size>0)
				memcpy(&data[0], &source, data.size());
		}

		// base func
		const Binary& get() const {return data;}
		uint32 size() const {return data.size();}
		bool empty() const {return size()==0;}
		void extend(uint32 size, byte value = 0) {
			for(int i = 0; i < size; i++)
				data.push_back(value);
		}
		template<typename T>
		void appendAny(T value) {
			Raw src = value;
			for(int32 index = 0; index < src.size(); index++)
				append(src[index]);
		}
		void append(byte value) {
			data.push_back(value);
		}
		void resize(uint32 newSize) {
			if(newSize>size())
				extend(newSize-size());
			else
				data.resize(newSize);
		}
		Raw reverse() const {
			Raw c;
			for(int32 index = 0; index < data.size(); index++)
				c.append(data[data.size()-index-1]);
			return c;
		}
		int32 clamp(int32 index) const{
			int length = size();
			return (index%length+length)%length;
		}
		const byte& at(int32 index) const {
			if(index<0||index>=size())
				index = clamp(index);
			return data[index];
		}
		byte& pick(int32 index) {
			if(index<0||index>=size())
				index = clamp(index);
			return data[index];
		}
		template<typename T>
		T getAny(int32 index) const {
			Raw any;
			T ret;
			for(int i = 0; i < sizeof(T); i++)
				any.append((index+i)>=size()?0:at(index+i));
			memcpy(&ret, &any[0], sizeof(T));
			return ret;
		}

		// operation
		friend Raw operator+(const Raw &a, const Raw &b) {
			Raw c = a;
			if(!b.empty()) {
				c.extend(b.size());
				memcpy((void*)&c[a.size()], (void*)&b[0], b.size());
			}
			return c;
		}
		friend Raw operator^(const Raw &a, const Raw &b) {
			Raw c;
			uint32 index = 0;
			for(uint32 index = 0; index < a.size() || index < b.size(); index++)
				c.append(a[index]^b[index]);
			return c;
		}
		const byte& operator[](int32 index) const {
			return at(index);
		}
		byte& operator[](int32 index) {
			return pick(index);
		}
		Raw operator()(int32 l, int32 r) const {
			Raw c;
			l = clamp(l), r = clamp(r);
			int32 index = l;
			while(index != r)
			{
				c.append(at(index));
				index = (index+1)==size()?0:(index+1);
			}
			return c;
		}
	};
}

#endif