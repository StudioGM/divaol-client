//
//  SoraPathImplUnix.h
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPathImplUnix_h
#define Sora_SoraPathImplUnix_h

#include "SoraPlatform.h"
#include "SoraException.h"

#include <pwd.h>

namespace sora {
    
    class SoraPathImpl {
    public:
        static std::string getEnvImpl(const std::string& name) {
            const char* val = getenv(name.c_str());
            if (val)
                return std::string(val);
            else
                THROW_SORA_EXCEPTION("Cannot get env string");
        }
        static std::string currentImpl() {
            std::string path;
            char cwd[PATH_MAX];
            if(getcwd(cwd, sizeof(cwd)))
                path = cwd;
            else 
                THROW_SORA_EXCEPTION("Cannot get current directory");
            std::string::size_type n = path.size();
            if(n > 0 && path[n-1] != '/')
                path.append("/");
            return path;
        }
        static std::string homeImpl() {
            std::string path;
            struct passwd* pwd = getpwuid(getuid());
            if(pwd)
                path = pwd->pw_dir;
            else {
                pwd = getpwuid(getuid());
                if(pwd) 
                    path = pwd->pw_dir;
                else 
                    path = getEnvImpl("HOME");
            }
            std::string::size_type n = path.size();
            if(n > 0 && path[n-1] != '/')
                path.append("/");
            return path;
        }
        static std::string tempImpl() {
            std::string path;
            char* tmp = getenv("TMPDIR");
            if(tmp) {
                path = tmp;
                std::string::size_type n = path.size();
                if(n > 0 && path[n-1] != '/')
                    path.append("/");
                else 
                    path = "/tmp/";
            }
            return path;
        }
        static std::string nullImpl() {
            return "/dev/null";
        }
        static std::string expandImpl(const std::string path) {
            std::string result;
            std::string::const_iterator it  = path.begin();
            std::string::const_iterator end = path.end();
            if (it != end && *it == '~') {
                ++it;
                if (it != end && *it == '/') {
                    result += homeImpl(); ++it;
                }
                else result += '~';
            }
            while (it != end) {
                if (*it == '$') {
                    std::string var;
                    ++it;
                    if (it != end && *it == '{') {
                        ++it;
                        while (it != end && *it != '}') var += *it++;
                        if (it != end) ++it;
                    }
                    else {
                        while (it != end && (((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z') || (*it >= '0' && *it <= '9')) || *it == '_')) var += *it++;
                    }
                    char* val = getenv(var.c_str());
                    if (val) result += val;
                }
                else result += *it++;
            }
            return result;
        }
        static void listRootsImpl(std::vector<std::string>& roots) {
            roots.clear();
            roots.push_back("/");
        }
        static std::string resourceImpl() {
            return "./";
        }
        static std::string writtableImpl() {
            return "./";
        }
        static std::string fontImpl() {
#ifdef OS_OSX
            // osx specific
            return "/Library/Fonts/";
#else
            // *nix
            return "/usr/share/fonts/";
#endif // OS_OSX
        }
    };

} // namespace
#endif
