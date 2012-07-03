//
//  SoraMiscTool_Win32.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraMiscTool_Win32.h"

#ifdef OS_WIN32

#include <windows.h>
#include "SoraLogger.h"

namespace sora {
    
    SoraMiscToolWin32::SoraMiscToolWin32(): 
    _hWnd(0) { }
    
	int32 SoraMiscToolWin32::messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode) {
		return MessageBoxW(_hWnd, sMessage.wc_str(), sTitle.wc_str(), msgCode);
	}
    
	void SoraMiscToolWin32::setMainWindowHandle(SoraHandle mainWindowHandle) { 
        _hWnd = (HWND)mainWindowHandle;
    }
	
	StringType SoraMiscToolWin32::fileOpenDialog(const char* filter, const char* defaultPath) {
        char rfilter[128];
		if(filter != NULL) {
			strcpy(rfilter, std::string(std::string("My Files(")+filter+") ").c_str());
			int32 pos = strlen(rfilter);
            
			SoraStringTokenlizer tokens(filter);
            SoraStringTokenlizer::iterator it = tokens.begin();
            while(it != tokens.end()) {
                strcat(rfilter, "*.");
				strcat(rfilter, (*it).c_str());
				if(it != tokens.end()-1)
					strcat(rfilter, ";");
                ++it;
            }
			rfilter[pos-1] = '\0';
            rfilter[strlen(rfilter)] = '\0';
			rfilter[strlen(rfilter)+1] = '\0';
		}
        
        OPENFILENAMEA ofn;
        ofn.lStructSize       = sizeof(OPENFILENAME);
        ofn.hwndOwner         = _hWnd;
        ofn.hInstance         = NULL;
        ofn.lpstrFilter       = 0;
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter    = 0;
        ofn.nFilterIndex      = 0;
        ofn.lpstrFile         = NULL;         
        ofn.nMaxFile          = MAX_PATH;
        ofn.lpstrFileTitle    = NULL;         
        ofn.nMaxFileTitle     = MAX_PATH;
        ofn.lpstrInitialDir   = NULL;
        ofn.lpstrTitle        = 0;
        ofn.Flags             = 0;           
        ofn.nFileOffset       = 0;
        ofn.nFileExtension    = 0;
        ofn.lpstrDefExt       = 0;
        ofn.lCustData         = 0L;
        ofn.lpfnHook          = NULL;
        ofn.lpTemplateName    = NULL;
        
        char fileName[512];
        char titleName[128];
        fileName[0] = L'\0';
        titleName[0] = L'\0';
        ofn.lpstrFile         = fileName ;
        ofn.lpstrFileTitle    = titleName ;
        ofn.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
        if(defaultPath)
            ofn.lpstrInitialDir = defaultPath;
        ofn.lpstrDefExt = NULL;
        if(filter)
            ofn.lpstrFilter = filter != NULL ? rfilter : NULL;
        
        if(GetOpenFileNameA(&ofn))
			return fileName;
        return "\0";
	}
	
	StringType SoraMiscToolWin32::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		char rfilter[128];
		if(filter != NULL) {
			strcpy(rfilter, std::string(std::string("My Files(")+filter+") ").c_str());
			int32 pos = strlen(rfilter);
            
			SoraStringTokenlizer tokens(filter);
            SoraStringTokenlizer::iterator it = tokens.begin();
            while(it != tokens.end()) {
                strcat(rfilter, "*.");
				strcat(rfilter, (*it).c_str());
				if(it != tokens.end()-1)
					strcat(rfilter, ";");
                ++it;
            }
			rfilter[pos-1] = '\0';
            rfilter[strlen(rfilter)] = '\0';
			rfilter[strlen(rfilter)+1] = '\0';
		}
        
        OPENFILENAMEA ofn;
        ofn.lStructSize       = sizeof(OPENFILENAME);
        ofn.hwndOwner         = _hWnd;
        ofn.hInstance         = NULL;
        ofn.lpstrFilter       = 0;
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter    = 0;
        ofn.nFilterIndex      = 0;
        ofn.lpstrFile         = NULL;         
        ofn.nMaxFile          = MAX_PATH;
        ofn.lpstrFileTitle    = NULL;         
        ofn.nMaxFileTitle     = MAX_PATH;
        ofn.lpstrInitialDir   = NULL;
        ofn.lpstrTitle        = 0;
        ofn.Flags             = 0;           
        ofn.nFileOffset       = 0;
        ofn.nFileExtension    = 0;
        ofn.lpstrDefExt       = 0;
        ofn.lCustData         = 0L;
        ofn.lpfnHook          = NULL;
        ofn.lpTemplateName    = NULL;
        
        char fileName[512];
        char titleName[128];
        fileName[0] = L'\0';
        titleName[0] = L'\0';
        ofn.lpstrFile         = fileName ;
        ofn.lpstrFileTitle    = titleName ;
        ofn.Flags             = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST ;
        if(defaultPath)
            ofn.lpstrInitialDir = defaultPath;
        if(defaultExt)
			ofn.lpstrDefExt = defaultExt;
        if(filter)
            ofn.lpstrFilter = filter != NULL ? rfilter : NULL;
        
        if(GetSaveFileNameA(&ofn))
            return fileName;
        return "\0";
	}
    
    uint32 SoraMiscToolWin32::getProcessorSpeed() const {
        LONG Error;
        
        HKEY Key;
        Error = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                              __TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"),
                              0, KEY_READ, &Key);
        
        if(Error != ERROR_SUCCESS)
            return 0;
        
        DWORD Speed = 0;
        DWORD Size = sizeof(Speed);
        Error = RegQueryValueExW(Key, __TEXT("~MHz"), NULL, NULL, (LPBYTE)&Speed, &Size);
        
        RegCloseKey(Key);
        
        if (Error != ERROR_SUCCESS)
            return 0;
        else
            return Speed;
    }
    
    StringType SoraMiscToolWin32::getOSVersion() const {
        OSVERSIONINFO osvi;
        
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        
        GetVersionEx(&osvi);
        
        return vamssg("Windows %d.%d Build %d %s", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber, osvi.szCSDVersion);
    }
    
    uint64 SoraMiscToolWin32::getSystemMemorySize() const {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    }
    
} // namespace sora

#endif 