/*
 *  SoraStringConv.cpp
 *  KuiIP
 *
 *  Created by Robert Bu on 5/6/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraStringConv.h"

#include <algorithm>

#if defined(OS_IOS) || defined(OS_OSX)
#include "SoraiOSStringConv.h"
#endif

namespace sora {
	
    std::string int_to_str(int32 n)
    {
        char *str = NULL;                  
        int length = 1;                     
        int temp = 0;
        int sign = 1;                      
        
        if(n<0) {
            sign = -1;
            n = -n;
            ++length;                         
        }
        
        temp = n;
        do {
            ++length;
        }
        while((temp /= 10)>0);
        
        str = (char*)malloc(length);        
        
        if(sign<0)                      
            str[0] = '-';                    
        
        str[--length] = '\0';             
        
        do {
            str[--length] = '0'+n%10;
        }while((n /= 10) > 0);
        
        std::string buffer(str);
        delete str;
        return buffer;
    }
    
    std::string fp_to_str(float x)
    {
        char *str = NULL;            
        char *integral = NULL;           
        char *fraction = NULL;          
        size_t length = 0;            
        
        std::string inte = int_to_str((int)x);
        integral = const_cast<char*>(inte.c_str());    
        
        if(x<0)
            x = -x;
        std::string frac = int_to_str((int)(100.0*(x+0.005-(int)x)));
        fraction = const_cast<char*>(frac.c_str());
        
        length = strlen(integral)+strlen(fraction)+2;  /* Total length including point and terminator */
        
        /* Fraction must be two digits so allow for it */
        if(strlen(fraction)<2)
            ++length;
        
        str = (char*)malloc(length);        /* Allocate memory for total */
        strcpy(str, integral);              /* Copy the integral part    */
        strcat(str, ".");                   /* Append decimal point      */
        
        if(strlen(fraction)<2)              /* If fraction less than two digits */
            strcat(str,"0");                  /* Append leading zero       */
        
        strcat(str, fraction);              /* Append fractional part    */
        
        
        std::string buffer(str);
        delete str;
        return buffer;
    }
    
#ifdef WIN32
	std::string WChar2Ansi(const std::wstring& pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, NULL, 0, NULL, NULL);
		if (nLen<= 0) return std::string("");
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP, 0, pwszSrc.c_str(), -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen -1] = 0;
		std::string strTemp(pszDst);
		delete [] pszDst;
		return strTemp;
	}
    
	std::wstring Ansi2WChar(const std::string& pszSrc, int nLen)
	{
		if(pszSrc.size() == 0)
			return L"\0";
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszSrc.c_str(), nLen, 0, 0);
		if(nSize <= 0) return NULL;
		WCHAR *pwszDst = new WCHAR[nSize+1];
		if( NULL == pwszDst) return NULL;
		MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc.c_str(), nLen, pwszDst, nSize);
		pwszDst[nSize] = 0;
		if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
			for(int i = 0; i < nSize; i ++)
				pwszDst[i] = pwszDst[i+1];
		std::wstring wcharString(pwszDst);
		delete pwszDst;
		return wcharString;
	}
#endif
    
	std::string ws2sfast(const std::wstring& ws) {
		std::string buffer(ws.length(), ' ');
		std::copy(ws.begin(), ws.end(), buffer.begin());
		return buffer;
	}
	
	std::wstring s2wsfast(const std::string& s) {
		std::wstring buffer(s.length(), L' ');
		std::copy(s.begin(), s.end(), buffer.begin());
		return buffer;
	}
	
    std::string ws2s(const std::wstring& ws) {
#ifdef WIN32
        return WChar2Ansi(ws);
#elif defined(OS_IOS) || defined(OS_OSX)
        return iOSWString2String(ws);
#endif
        std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
        setlocale(LC_ALL, "chs");
        const wchar_t* _Source = ws.c_str();
        size_t _Dsize = 2 * ws.size() + 1;
        char *_Dest = new char[_Dsize];
        memset(_Dest,0,_Dsize);
        wcstombs(_Dest,_Source,_Dsize);
        std::string result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, curLocale.c_str());
        return result;
    }
    
    std::wstring s2ws(const std::string& s)
    {
#ifdef WIN32
        return Ansi2WChar(s, s.size());
#elif defined(OS_IOS) || defined(OS_OSX)
        return iOSString2WString(s);
#endif
        setlocale(LC_ALL, "chs"); 
        const char* _Source = s.c_str();
        size_t _Dsize = s.size() + 1;
        wchar_t *_Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        mbstowcs(_Dest,_Source,_Dsize);
        std::wstring result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, "C");
        return result;
    }
	
    inline size_t skipSpaces(const std::string& str, size_t currPos) {
        char c = str[currPos];
        while((c == ' ' || c == '\n' || c == '\r' || c == '\t') && currPos<str.size()-1) {
            ++currPos;
            c = str[currPos];
        }
        return currPos;
    }
    
    void deliStr(std::vector<std::string>& c, const std::string& str, char deli) {
        size_t start = 0, end = 0;
        while(end < str.size()) {
            while(str[end] != deli && end < str.size()) { ++end; }
            if(end == str.size()) {
                c.push_back( str.substr(start, str.size()) );
                break;
            }
            c.push_back( str.substr(start, end-start) );
            
            ++end; end = skipSpaces(str, end);
            start = end;
        }
    }
    
    std::wstring SoraStringConverter::stringToWString(const std::string& str) {
        return s2ws(str);
    }
    
    std::string SoraStringConverter::wstringToString(const std::wstring& str) {
        return ws2s(str);
    }

    std::string tolowerstring(const std::string& s) {
        std::string result = s;
        std::transform(s.begin(), s.end(), result.begin(), tolower);
		return result;
    }
    
    std::string toupperstring(const std::string& s) {
        std::string result = s;
        std::transform(s.begin(), s.end(), result.begin(), toupper);
		return result;
    }

} // namespace sora