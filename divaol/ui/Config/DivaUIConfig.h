#ifndef DIVA_UI_CONFIG
#define DIVA_UI_CONFIG

#include "Lib/wjson/wjson.h"
#include <string>
#include <Windows.h>

namespace diva
{
	extern WJson::Value config;

	void initialize_config(const std::wstring& filename);

	std::wstring ReadJsonFile(const std::wstring& filename);

}

namespace gcn
{
	//Utility Functions
	std::wstring iToWS(int n);
	std::wstring fTows(float f,int bit);
	std::wstring dTows(double d,int bit);
	std::wstring secondToTimeWstr(float second);
	float wsTof(std::wstring &s);
	double wsTod(std::wstring &s);
	double round(double r); 
	void tolower(std::wstring &str);
	std::string to_utf8(const wchar_t* buffer, int len);
	std::string to_utf8(const std::wstring& str);
}


#endif