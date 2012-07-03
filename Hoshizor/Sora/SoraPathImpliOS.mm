//
//  SoraPathImpliOS.mm
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPathImpliOS.h"

#ifdef OS_IOS

#include <Foundation/Foundation.h>
#include "SoraLogger.h"
#include "soraiOSFileUtility.h"

namespace sora {
    
    inline std::string NSString2String(NSString* nss) {
		std::string buffer([nss UTF8String]);
		return buffer;
	}
	
	inline NSString* string2NSString(const std::string& str) {
		NSString* buffer = [[NSString alloc] initWithUTF8String:str.c_str()];
		return buffer;
	}
	
    
    std::string SoraPathImpl::getEnvImpl(const std::string& name) {
        const char* val = getenv(name.c_str());
        if (val)
            return std::string(val);
        else {
            log_error("SoraPath::getEnvImpl: Cannot get env string");
            return std::string();
        }
    }
    
    std::string SoraPathImpl::currentImpl() {
        return [[[NSBundle mainBundle] bundlePath] UTF8String];
    }
    
    std::string SoraPathImpl::homeImpl() {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
		return (NSString2String(basePath)+"/");
    }
    
    std::string SoraPathImpl::tempImpl() {
        return homeImpl();
    }
    
    std::string SoraPathImpl::nullImpl() {
        return currentImpl();
    }
    
    std::string SoraPathImpl::expandImpl(const std::string path) {
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
    void SoraPathImpl::listRootsImpl(std::vector<std::string>& roots) {
        roots.clear();
        roots.push_back(currentImpl());
    }
    
    std::string SoraPathImpl::resourceImpl() {
        std::string path = [[[NSBundle mainBundle] bundlePath] UTF8String];
        return path + "/";
    }
    
    std::string SoraPathImpl::writtableImpl() {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : nil;
        if(basePath) {
            std::string path = [basePath UTF8String];
            return path + "/";
        }
        return "./";
    }
    std::string SoraPathImpl::fontImpl() {
        // ios default
        return "/System/Library/Fonts/";
    }
    
} // namespace sora

#endif // OS_IOS