#include "DivaEditorLocalization.h"

#include <cstdio>
#include "divaeditor/WJson/wjson.h"
#include "divaeditor/DivaEditorCommon.h"

#ifdef WIN32
#include <Windows.h>
#endif

namespace divaeditor
{

	Localization* Localization::Ptr = NULL;

	void Localization::InitFromLocalizationJsonFile(Localization* local)
	{
		std::wstring language = L"en-us";

		//Read language settings
		FILE* readFile;
		if(_wfopen_s(&readFile, L"language.ini" ,L"rt, ccs=UTF-8")!=0)
		{
#ifdef WIN32
			//Try to find local language
			wchar_t plLCData[128] = {0};

			if(GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_SNAME, plLCData, 128)!=0)
				language = plLCData;
			else
			{
				//try to detect some main languages using lang-identity in Windows XP
				DWORD dwLanguage;

				if(GetLocaleInfoW(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE | LOCALE_RETURN_NUMBER, (LPWSTR) &dwLanguage, sizeof(dwLanguage) / sizeof(wchar_t))==0)
					language = L"unknown";
				else
				{
					unsigned short subLang = dwLanguage >> 10;
					unsigned short primaryLang = dwLanguage - (subLang << 10);

					//Detect traditional chinese
					if(primaryLang == LANG_CHINESE && subLang != SUBLANG_CHINESE_SIMPLIFIED)
						language = L"zh-HK";
					else if(primaryLang == LANG_CHINESE_SIMPLIFIED && subLang == SUBLANG_CHINESE_SIMPLIFIED)
						language = L"zh-CN";
					else if(primaryLang == LANG_JAPANESE && subLang == SUBLANG_JAPANESE_JAPAN)
						language = L"ja-JP";
					else
						language = L"unknown";
				}
			}

			divaeditor::tolower(language);
#endif
		}
		else
		{
			wchar_t buffer[1000];
			fwscanf(readFile,L"%s",buffer);
			language = buffer;
			fclose(readFile);
		}

		//Check again if this language exist
		if(GetFileAttributesW((L"local/"+language).c_str())==INVALID_FILE_ATTRIBUTES)
			language = L"en-us";

		FILE* writeFile;
		if(_wfopen_s(&writeFile, L"language.ini" ,L"wt, ccs=UTF-8")==0)
		{
			fwprintf(writeFile,L"%s",language.c_str());
			fclose(writeFile);
		}

		//try to open language json file
		FILE* localJsonFile;
		if(_wfopen_s(&localJsonFile, (L"local/" + language + L"/localStr.json").c_str() ,L"rt, ccs=UTF-8")!=0)
			return; 

		//read file from json file
		std::wstring jsonStrToParse;
		wchar_t buffer[1000];
		while(fgetws(buffer,sizeof(buffer),localJsonFile))
			jsonStrToParse += std::wstring(buffer);
		fclose(localJsonFile);

		//parse
		WJson::Reader reader;

		WJson::Value root;
		if(!reader.parse(jsonStrToParse,root))
			return;

		local->strMap.clear();
		
		for(WJson::ValueIterator i=root.begin();i!=root.end();i++)
		{
			if(i.key().asString()==L"font")
				local->font = L"local/" + language + L"/" + root[i.key().asString()].asString();
			else
				local->strMap[i.key().asString()] = root[i.key().asString()].asString();
		}
	}

	std::wstring Localization::getLocalStr(std::wstring strID,...)
	{
		va_list varList;
		int varCount;
		
		va_start(varList, strID);

		if(strMap.find(strID)==strMap.end())
			return strID + L" string missing";

		std::wstring formatedStr = strMap[strID];

		wchar_t retStrBuffer[500];

		if(_vswprintf(retStrBuffer, formatedStr.c_str(),varList)<0)
			return strID + L" print failed";
		std::wstring retStr(retStrBuffer);

		return retStr;
	}

	std::wstring Localization::getLocalFontPath()
	{
		return font;
	}


}


