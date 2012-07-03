//
//  SoraPathImplWindows.h
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPathImplWindows_h
#define Sora_SoraPathImplWindows_h

#include "SoraPlatform.h"
#include "SoraException.h"

#ifdef OS_WIN32
#include <windows.h>

namespace sora {
    
    class SoraPathImpl {
    public:
        static std::string getEnvImpl(const std::string& name) {
            DWORD len = GetEnvironmentVariableA(name.c_str(), 0, 0);
            if (len == 0) THROW_SORA_EXCEPTION(RuntimeException, "Cannot get current directory");
            char* buffer = new char[len];
            GetEnvironmentVariableA(name.c_str(), buffer, len);
            std::string result(buffer);
            delete [] buffer;
            return result;
        }
        static std::string currentImpl() {
            char buffer[_MAX_PATH];
            DWORD n = GetCurrentDirectoryA(sizeof(buffer), buffer);
            if(n > 0 && n < sizeof(buffer)) {
                std::string result(buffer, n);
                if(result[n-1] != '\\')
                    result.append("\\");
                return result;
            }
            else THROW_SORA_EXCEPTION(RuntimeException, "Cannot get current directory");
        }
        static std::string homeImpl() {
            std::string result = getEnvImpl("HOMEDRIVE");
            result.append(getEnvImpl("HOMEPATH"));
            std::string::size_type n = result.size();
            if(n > 0 && result[n-1] != '\\')
                result.append("\\");
            return result;
        }
        static std::string tempImpl() {
            char buffer[_MAX_PATH];
            DWORD n = GetTempPathA(sizeof(buffer), buffer);
            if(n > 0 && n < sizeof(buffer)) {
                std::string result(buffer, n);
                if(result[n-1] != '\\')
                    result.append("\\");
                return result;
            } else
                THROW_SORA_EXCEPTION(RuntimeException, "Cannot get temp directory");
        }
        static std::string nullImpl() {
            return "NUL:";
        }
        
        static std::string expandImpl(const std::string path) {
            char buffer[_MAX_PATH];
            DWORD n = ExpandEnvironmentStringsA(path.c_str(), buffer, sizeof(buffer));
            if(n > 0 && n < sizeof(buffer)) {
                return std::string(buffer, n-1);
            } else
                return path;
        }
        static void listRootsImpl(std::vector<std::string>& roots) {
            roots.clear();
            char buffer[128];
            DWORD n = GetLogicalDriveStringsA(sizeof(buffer)-1, buffer);
            char* it = buffer;
            char* end = buffer + (n > sizeof(buffer) ? sizeof(buffer): n);
            while(it < end) {
                std::string dev;
                while(it < end && *it)
                    dev += *it++;
                roots.push_back(dev);
                ++it;
            }
        }
        static std::string resourceImpl() {
            return "./";
        }
        static std::string writtableImpl() {
            return "./";
        }
        static std::string fontImpl() {
            char buffer[_MAX_PATH];
            GetWindowsDirectoryA(buffer, _MAX_PATH-1);
            std::string sfontpath(buffer);
            return sfontpath + "\\Fonts\\";
        }
    };

} // namespace sora

#endif // OS_WIN32

#endif
