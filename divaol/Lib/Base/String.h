/****************************************************
 *  String.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A String that combine ansi and unicode
 ****************************************************/

#ifndef _BASE_STRING_H_
#define _BASE_STRING_H_

#include "Type.h"
#include "Exception.h"
#include "StringConv.h"

#include <vector>
#include <sstream>
#include <fstream>
 
namespace Base
{
	class String;
	typedef std::vector<String> Strings;
	
	class String
	{
	public:
		typedef int32 IndexVar;
		static const IndexVar npos;

		String() {}
		String(const char *s, bool isUtf8 = false) {
			mContent = isUtf8?utf82ws(s):s2ws(s);}
		String(const wchar *s):mContent(s) {}
		String(const wchar *s, size_t size) {
			mContent = L"";
			*this = append(size);
			memcpy(&mContent[0],s,sizeof(wchar)*size);
		}
		String(const base_string &s, bool isUtf8 = false) {
			mContent = isUtf8?utf82ws(s):s2ws(s);
		}
		String(const base_wstring &ws):mContent(ws) {}
		String(char c) {
			base_string s; 
			s += c;
			mContent = s2ws(s);
		}
		String(wchar c) {
			mContent += c;
		}
		//it will cause some implicit problem
		//template<typename T>
		//String(T value) {*this=any2string(value);}
		~String() {}

		String substr(IndexVar index, IndexVar len) const{
			if(index<0||len<=0||index+len>size())
				return L"";
			return mContent.substr(index,len);
		}
		IndexVar find(const String &sub, IndexVar off = 0) const{
			return mContent.find(sub.mContent,off);
		}
		IndexVar rfind(const String &sub, IndexVar off = npos) const{
			return mContent.rfind(sub.mContent,off);
		}
		String lower() const;
		String upper() const;
		String append(size_t size, const String &token = L" ") const;
		String reverse() const;
		String replace(const String &src, const String &dst) const;
		Strings split(Strings tokens=Strings()) const;
		String strip(Strings tokens=Strings()) const;

		void clear() {mContent.clear();}
		bool empty() const {return mContent.empty();}
		size_t size() const{return mContent.size();}
		size_t length() const{return mContent.length();}
		void resize(size_t size, wchar newChar) {mContent.resize(size,newChar);}
		wchar getWChar(IndexVar index) const{return mContent[index];}
		char getChar(IndexVar index) const{
			return static_cast<char>(getWChar(index)&0xff);
		}

		//operator
		friend std::ostream& operator<<(std::ostream &out, const String &data) {
			out << data.asAnsi();
			return out;
		}
		friend std::wostream& operator<<(std::wostream &out, const String &data) {
			out << data.asUnicode();
			return out;
		}

		wchar operator[](IndexVar index) const {return getWChar(index);}
		IndexVar operator()(const String& sub, IndexVar off = npos, bool isReverse = false) {
			if(isReverse)
				return rfind(sub,off);
			else
				return find(sub,off==npos?0:off);
		}
		char operator()(IndexVar index) const {return getChar(index);}
		String operator()(IndexVar l, IndexVar r) const {
			return mContent.substr(l,r<0?size()-l:r-l);
		}
		String& operator+=(const String& other) {
			mContent+=other.mContent; 
			return*this;
		}
		String& operator*=(uint32 size) {
			base_wstring origin = mContent;
			for(uint32 i = 1; i < size; i++)
				mContent += origin;
			return *this;
		}
		friend String operator+(const String &a, const String &b) {return a.mContent+b.mContent;}
		friend bool operator<(const String &a, const String &b) {return a.mContent<b.mContent;}
		friend bool operator>(const String &a, const String &b) {return a.mContent>b.mContent;}
		friend bool operator<=(const String &a, const String &b) {return a.mContent<=b.mContent;}
		friend bool operator>=(const String &a, const String &b) {return a.mContent>=b.mContent;}
		friend bool operator==(const String &a, const String &b) {return a.mContent==b.mContent;}
		friend bool operator!=(const String &a, const String &b) {return a.mContent!=b.mContent;}
		friend String operator*(const String &a, uint32 b) {
			String ret = a;
			ret *= b;
			return ret;
		}
		operator base_string() const {
			return asAnsi();
		}
		operator base_wstring() const {
			return asUnicode();
		}

		//utility funcs
		bool isInt() const;
		bool isReal() const;
		bool isNumber() const {return isReal()|isInt();}
		template<typename T>
		T as() const {return string2any<T>(*this);}
		base_string asAnsi() const{return ws2s(mContent);}
		base_string asUTF8() const{return ws2utf8(mContent);}
		base_wstring asUnicode() const{return mContent;}
		const char* ansi_str() const{static base_string tmp; tmp = ws2s(mContent); return tmp.c_str();}
		const wchar* unicode_str() const{return mContent.c_str();}
		template<typename T>
		static String any2string(T tmp) {
			std::stringstream ss;
			ss << tmp;
			return ss.str();
		}
		template<typename T>
		static T string2any(const String &s) {
			T tmp;
			std::stringstream ss;
			ss << s.ansi_str();
			ss >> tmp;
			return tmp;
		}
		template<typename T>
		T toAny() {
			return string2any<T>(*this);
		}
		static String format(const char* fmt, ...);
		static String unEscape(String origin) {
			return Base::unEscape(origin);
		}

	private:
		base_wstring mContent;
	};
}

#endif