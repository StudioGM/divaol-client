/*
 *  DivaStringUtil.cpp
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include <stdarg.h>

#include "SoraPlatform.h"


#ifdef OS_WIN32
#include <windows.h>
#endif

#include <string>
#include <iostream>
#include <sstream>

#include "DivaStringUtil.h"

namespace divacore
{
	std::string iToS(int n) 
	{
        std::ostringstream strStream;
        strStream<<n;
        return strStream.str();
	}

	int sToI(const std::string &number)
	{
		int ret = 0;
		for(int i = (number[0]=='-'?1:0); i < number.size(); i++)
			ret = ret*10+number[i]-'0';
		if(number[0]=='-')
			ret = -ret;
		return ret;
	}

	std::string GBKToUTF8(const std::string& strGBK)
	{
#ifdef OS_WIN32
		std::string strOutUTF8 = "";
		WCHAR * str1;
		int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
		str1 = new WCHAR[n];
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		char * str2 = new char[n];
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
		strOutUTF8 = str2;
		delete[]str1;
		str1 = NULL;
		delete[]str2;
		str2 = NULL;
        return strOutUTF8;

#endif
        return strGBK;
	}

	std::wstring UTF8toUnicode(const std::string& strUTF8)
	{
#ifdef OS_WIN32
		int nLenOfWcharStr=MultiByteToWideChar(CP_UTF8,0, strUTF8.c_str(),-1,NULL,0); //得到转换成unicode需要的character（不是byte）数目。
		PWSTR pWideCharStr=(PWSTR)malloc(nLenOfWcharStr*sizeof(wchar_t));
		if(pWideCharStr!=NULL)
		{
			MultiByteToWideChar(CP_UTF8,0, strUTF8.c_str(),-1,pWideCharStr,nLenOfWcharStr);
			std::wstring ret = pWideCharStr;
			delete pWideCharStr;
			return ret;
		}
#endif
		return L"";
	}

	std::string format(const char *format,...)
	{
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);

		return Message;
	}
}
