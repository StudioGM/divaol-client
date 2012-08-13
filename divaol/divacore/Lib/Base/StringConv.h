/****************************************************
 *  StringConv.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A String that combine ansi and unicode
 ****************************************************/
#ifndef _BASE_STRING_CONVERT_H_
#define _BASE_STRING_CONVERT_H_

#include "Type.h"

namespace Base
{
	namespace StringConvImpl
	{
#ifdef BASE_OS_WINDOWS
		inline base_string ws2sImpl(const base_wstring& ws)
		{
			int nLen = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, NULL, 0, NULL, NULL);
			if (nLen<= 0) return base_string("");
			char* pszDst = new char[nLen];
			if (NULL == pszDst) return base_string("");
			WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, pszDst, nLen, NULL, NULL);
			pszDst[nLen -1] = 0;
			base_string s(pszDst);
			delete [] pszDst;
			return s;
		}

		inline base_wstring s2wsImpl(const base_string& s)
		{
			if(s.size() == 0)
				return L"\0";
			int nSize = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size(), 0, 0);
			if(nSize <= 0) return NULL;
			WCHAR *pwszDst = new WCHAR[nSize+1];
			if( NULL == pwszDst) return NULL;
			MultiByteToWideChar(CP_ACP, 0,(LPCSTR)s.c_str(), s.size(), pwszDst, nSize);
			pwszDst[nSize] = 0;
			if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
				for(int i = 0; i < nSize; i ++)
					pwszDst[i] = pwszDst[i+1];
			base_wstring ws(pwszDst);
			delete pwszDst;
			return ws;
		}
		inline base_wstring utf82wsImpl(const base_string& s)
		{
			int nLenOfWcharStr=MultiByteToWideChar(CP_UTF8,0, s.c_str(),-1,NULL,0); //得到转换成unicode需要的character（不是byte）数目。
			PWSTR pWideCharStr=(PWSTR)malloc(nLenOfWcharStr*sizeof(wchar_t));
			if(pWideCharStr!=NULL)
			{
				MultiByteToWideChar(CP_UTF8,0, s.c_str(),-1,pWideCharStr,nLenOfWcharStr);
				std::wstring ret = pWideCharStr;
				delete pWideCharStr;
				return ret;
			}
			return L"";
		}
		inline base_string s2utf8Impl(const base_string& s)
		{
			std::string strOutUTF8 = "";
			WCHAR * str1;
			int n = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, 0);
			str1 = new WCHAR[n];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, str1, n);
			n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
			char * str2 = new char[n];
			WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
			strOutUTF8 = str2;
			delete[]str1;
			str1 = NULL;
			delete[]str2;
			str2 = NULL;
			return strOutUTF8;
		}
#elif defined(BASE_OS_FAMILY_APPLE)
		inline base_string ws2sImpl(const base_wstring& ws){
			return "";
		}

		inline base_wstring s2wsImpl(const base_string& s){
			return L"";
		}

		inline base_wstring utf82wsImpl(const base_string &s) {
			return s2wsImpl(s);
		}

		inline base_string s2utf8(const base_string &s) {
			return s;
		}
#else
		inline base_string ws2sImpl(const base_wstring& ws) {
			base_string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
			setlocale(LC_ALL, "chs");
			const wchar_t* _Source = ws.c_str();
			size_t _Dsize = 2 * ws.size() + 1;
			char *_Dest = new char[_Dsize];
			memset(_Dest,0,_Dsize);
			wcstombs(_Dest,_Source,_Dsize);
			base_string result = _Dest;
			delete []_Dest;
			setlocale(LC_ALL, curLocale.c_str());
			return result;
		}

		inline base_wstring s2wsImpl(const base_string& s)
		{
			setlocale(LC_ALL, "chs"); 
			const char* _Source = s.c_str();
			size_t _Dsize = s.size() + 1;
			wchar_t *_Dest = new wchar_t[_Dsize];
			wmemset(_Dest, 0, _Dsize);
			mbstowcs(_Dest,_Source,_Dsize);
			base_wstring result = _Dest;
			delete []_Dest;
			setlocale(LC_ALL, "C");
			return result;
		}

		inline base_wstring utf82wsImpl(const base_string &s) {
			return s2wsImpl(s);
		}

		inline base_string s2utf8(const base_string &s) {
			return s;
		}
#endif
	}
	
	static base_string ws2s(const base_wstring& ws) {
		return StringConvImpl::ws2sImpl(ws);
	}

	static base_wstring s2ws(const base_string& s)
	{
		return StringConvImpl::s2wsImpl(s);	
	}

	static base_wstring utf82ws(const base_string& s)
	{
		return StringConvImpl::utf82wsImpl(s);
	}

	static base_string s2utf8(const base_string &s)
	{
		return StringConvImpl::s2utf8Impl(s);
	}

	static base_string ws2utf8(const base_wstring &ws)
	{
		return s2utf8(ws2s(ws));
	}

	static base_string utf82s(const base_string &s)
	{
		return ws2s(utf82ws(s));
	}
}

#endif