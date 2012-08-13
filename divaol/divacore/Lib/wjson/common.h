#ifndef _WJSON_SPECIAL_COMMON_H_
#define _WJSON_SPECIAL_COMMON_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>
#include <tchar.h>
#endif

#ifdef _UNICODE	
#define tstring std::wstring
#define tistream std::istream
#else	
#define tstring std::string	
#define tistream std::wistream
#endif

#endif