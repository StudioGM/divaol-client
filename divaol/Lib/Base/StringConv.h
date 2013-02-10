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
		
		enum
		{
			UNICODE_CALC_SIZE = 1,
			UNICODE_GET_BYTES = 2
		};
 
		//将unicode转义字符序列转换为内存中的unicode字符串
		static int unicode_bytes(const char* p_unicode_escape_chars,wchar_t *bytes,int flag)
		{
			/*
			char* p_unicode_escape_chars="pp\\u4fddp\\u5b58\\u6210pp\\u529f0a12";
 
			//通过此函数获知转换后需要的字节数
			int n_length=unicode_bytes(p_unicode_escape_chars,NULL,UNICODE_CALC_SIZE);
 
			//再次调用此函数，取得字节序列
			wchar_t *bytes=new wchar_t[n_length+sizeof(wchar_t)];
			unicode_bytes(p_unicode_escape_chars,bytes,UNICODE_GET_BYTES);
			bytes[n_length]=0;
 
			//此时的bytes中是转换后的字节序列
			delete[] bytes;
			*/
 
			int unicode_count=0;
			int length=strlen(p_unicode_escape_chars);
			for (int char_index=0;char_index<length;char_index++)
			{
			   char unicode_hex[5];
			   memset(unicode_hex,0,5);
 
			   char ascii[2];
			   memset(ascii,0,2);
 
			   if (*(p_unicode_escape_chars+char_index)=='\\')
			   {
				   char_index++;
				   if (char_index<length)
				   {
					  if (*(p_unicode_escape_chars+char_index)=='u')
					  {
						  if (flag==UNICODE_GET_BYTES)
						  {
							 memcpy(unicode_hex,p_unicode_escape_chars+char_index+1,4);
                    
							 //sscanf不可以使用unsigned short类型
							 //否则：Run-Time Check Failure #2 - Stack around the variable 'a' was corrupted.
							 unsigned int a=0;
							 sscanf_s(unicode_hex,"%04x",&a);
							 bytes[unicode_count++]=a;
						  }
						  else if(flag==UNICODE_CALC_SIZE)
						  {
							 unicode_count++;
						  }
						  char_index+=4;
					  }
				   }
			   }
			   else
			   {
				   if (flag==UNICODE_GET_BYTES)
				   {
					  memcpy(ascii,p_unicode_escape_chars+char_index,1);
					  unsigned int a=0;
					  sscanf_s(ascii,"%c",&a);
					  bytes[unicode_count++]=a;
				   }
				   else if(flag==UNICODE_CALC_SIZE)
				   {
					  unicode_count++;
				   }
			   }
			}
 
			return unicode_count;
		}
 
		static std::wstring unEscape(std::string p_unicode_escape_chars)
		{
			int nBytes=unicode_bytes(p_unicode_escape_chars.c_str(),NULL,UNICODE_CALC_SIZE);
 
			wchar_t *p_bytes=new wchar_t[nBytes+sizeof(wchar_t)];
			unicode_bytes(p_unicode_escape_chars.c_str(),p_bytes,UNICODE_GET_BYTES);
			p_bytes[nBytes]=0;
 
			std::wstring cs_return=((wchar_t*)p_bytes);

			delete[] p_bytes;
 
			return cs_return;
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

		inline base_string s2utf8Impl(const base_string &s) {
			return s;
		}
        enum
		{
			UNICODE_CALC_SIZE = 1,
			UNICODE_GET_BYTES = 2
		};
        
		//将unicode转义字符序列转换为内存中的unicode字符串
		static int unicode_bytes(const char* p_unicode_escape_chars,wchar_t *bytes,int flag)
		{
			/*
             char* p_unicode_escape_chars="pp\\u4fddp\\u5b58\\u6210pp\\u529f0a12";
             
             //通过此函数获知转换后需要的字节数
             int n_length=unicode_bytes(p_unicode_escape_chars,NULL,UNICODE_CALC_SIZE);
             
             //再次调用此函数，取得字节序列
             wchar_t *bytes=new wchar_t[n_length+sizeof(wchar_t)];
             unicode_bytes(p_unicode_escape_chars,bytes,UNICODE_GET_BYTES);
             bytes[n_length]=0;
             
             //此时的bytes中是转换后的字节序列
             delete[] bytes;
             */
            
			int unicode_count=0;
			int length=strlen(p_unicode_escape_chars);
			for (int char_index=0;char_index<length;char_index++)
			{
                char unicode_hex[5];
                memset(unicode_hex,0,5);
                
                char ascii[2];
                memset(ascii,0,2);
                
                if (*(p_unicode_escape_chars+char_index)=='\\')
                {
                    char_index++;
                    if (char_index<length)
                    {
                        if (*(p_unicode_escape_chars+char_index)=='u')
                        {
                            if (flag==UNICODE_GET_BYTES)
                            {
                                memcpy(unicode_hex,p_unicode_escape_chars+char_index+1,4);
                                
                                //sscanf不可以使用unsigned short类型
                                //否则：Run-Time Check Failure #2 - Stack around the variable 'a' was corrupted.
                                unsigned int a=0;
                                sscanf(unicode_hex,"%04x",&a);
                                bytes[unicode_count++]=a;
                            }
                            else if(flag==UNICODE_CALC_SIZE)
                            {
                                unicode_count++;
                            }
                            char_index+=4;
                        }
                    }
                }
                else
                {
                    if (flag==UNICODE_GET_BYTES)
                    {
                        memcpy(ascii,p_unicode_escape_chars+char_index,1);
                        unsigned int a=0;
                        sscanf(ascii,"%c",&a);
                        bytes[unicode_count++]=a;
                    }
                    else if(flag==UNICODE_CALC_SIZE)
                    {
                        unicode_count++;
                    }
                }
			}
            
			return unicode_count;
		}
        static std::wstring unEscape(std::string p_unicode_escape_chars)
		{
			int nBytes=unicode_bytes(p_unicode_escape_chars.c_str(),NULL,UNICODE_CALC_SIZE);
            
			wchar_t *p_bytes=new wchar_t[nBytes+sizeof(wchar_t)];
			unicode_bytes(p_unicode_escape_chars.c_str(),p_bytes,UNICODE_GET_BYTES);
			p_bytes[nBytes]=0;
            
			std::wstring cs_return=((wchar_t*)p_bytes);
            
			delete[] p_bytes;
            
			return cs_return;
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

	static base_wstring unEscape(const base_string &s)
	{
		return StringConvImpl::unEscape(s);
	}
}

#endif