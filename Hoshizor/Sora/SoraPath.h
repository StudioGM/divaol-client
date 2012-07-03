//
//  SoraPath.h
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPath_h
#define Sora_SoraPath_h

#include "SoraPlatform.h"
#include "SoraStringConv.h"
#include <vector>

#ifdef OS_WIN32
#include "SoraPathImplWindows.h"
#elif defined(OS_IOS)
#include "SoraPathImpliOS.h"
#elif defined(OS_OSX)
#include "SoraPathImplOSX.h"
#else
#include "SoraPathImplUnix.h"
#endif

namespace sora {
    
    /**
     *  Platform independent path representation
     *  Original form Poco C++ Library by Applied Infomatics
     *  Modified for Hoshizora and add wstring support by Robert Bu
     **/
    
    class SORA_API SoraPath: public SoraPathImpl {
    public:
        enum Style {
            PATH_UNIX,
            PATH_WINDOWS,
            PATH_NATIVE,
            PATH_GUESS
        };
        
        typedef std::vector<std::string> StringVec;
        
        SoraPath();
        SoraPath(bool absolute);
        explicit SoraPath(const std::string& path);
        explicit SoraPath(const std::wstring& path);
        explicit SoraPath(const char* path);
        explicit SoraPath(const wchar_t* path);
        SoraPath(const std::wstring& path, Style style);
        SoraPath(const std::string& path, Style style);
        SoraPath(const wchar_t* path, Style style);
        SoraPath(const char* path, Style style);
        SoraPath(const SoraPath& path);
        
        SoraPath(const SoraPath& parent, const std::string& filename);
        SoraPath(const SoraPath& parent, const std::wstring& filename);
        SoraPath(const SoraPath& parent, const SoraPath& relative);
        
        ~SoraPath();
        
        SoraPath& operator = (const SoraPath& path);
        SoraPath& operator = (const std::string& path);
        SoraPath& operator = (const std::wstring& path);
        SoraPath& operator = (const char* path);
        SoraPath& operator = (const wchar_t* path);
        
        void swap(SoraPath& path);
        
        SoraPath& assign(const char* path);
        SoraPath& assign(const wchar_t* path);
        SoraPath& assign(const std::string& path);
        SoraPath& assign(const std::string& path, Style style);
        
        std::string toString() const;
        std::wstring toWString() const;
        std::string toString(Style style) const;
        std::wstring toWString(Style style) const;
        
        SoraPath& parseDirectory(const std::string& path);
        SoraPath& parseDirectory(const std::wstring& path);
        SoraPath& parseDirectory(const std::string& path, Style style);
        SoraPath& parseDirectory(const std::wstring& path, Style style);
        
        SoraPath& makeDirectory();
        SoraPath& makeFile();
        SoraPath& makeParent();
        SoraPath& makeAbsolute();
        SoraPath& makeAbsolute(const SoraPath& path);
        SoraPath& append(const SoraPath& path);
        SoraPath& resolve(const SoraPath& path);
        
        bool isAbsolute() const;
        bool isRelative() const;
        bool isDirectory() const;
        bool isFile() const;
        
        size_t depth() const;
        
        std::string getBaseName() const;        
        std::string getFileName() const;
        std::string getExtension() const;
        std::string getNode() const;
        std::string getDevice() const;
        
        void setFileName(const std::string& filename);
        void setFileName(const std::wstring& filename);
        
        void setBaseName(const std::string& filename);
        void setBaseName(const std::wstring& filename);
        
        void setExtension(const std::string& ext);
        void setExtension(const std::wstring& ext);
        
        void setNode(const std::string& node);
        void setNode(const std::wstring& node);
        
        void setDevice(const std::string& device);
        void setDevice(const std::wstring& device);
        
        void clear();
        
        void pushDirectory(const std::string& dir);
        void popDirectory();
        
        SoraPath parent() const;
        SoraPath absolute() const;
        SoraPath absolute(const SoraPath& base) const;
        
        static SoraPath forDirectory(const std::string& path);
        static SoraPath forDirectory(const std::wstring& path);
        
        static char separator();
        static char pathSeparator();
        
        static std::string getEnv(const std::string& env);
        static std::string current();
        static std::string home();
        static std::string temp();
        static std::string null();
        // default resource path
        // under most platforms just returns "./"
        // under mobile platforms such as iOS
        // will return the accessable resource path
        static std::string resource();
        // default writtable path
        // under most platforms just returns "./"
        // under mobile platforms such as iOS
        // will return the writtable resource path
        // such as Documents for iOS
        static std::string writtable();
        // default system font path
        // such as :/Windows/System32/Fonts under windows
        static std::string font();
        
        static std::wstring getEnvW(const std::string& env);
        static std::wstring currentW();
        static std::wstring homeW();
        static std::wstring tempW();
        static std::wstring nullW();
        static std::wstring resourceW();
        static std::wstring writtableW();
        static std::wstring fontW();
        
        static std::string expand(const std::string& path);
        static std::string expand(const std::wstring& path);
        
        static void listRoots(std::vector<std::string>& roots);
        
        bool isEmpty() const;
        
    protected:
        void parseUnix(const std::string& path);
        void parseWindows(const std::string& path);
        void parseGuess(const std::string& path);
        std::string buildUnix() const;
        std::string buildWindows() const;
        
    private:
        std::string mName;
        std::string mNode;
        std::string mDevice;
        StringVec mDirs;
        bool mAbsolute;
    };
    
    inline bool SoraPath::isAbsolute() const {
        return mAbsolute;
    }
    
    inline bool SoraPath::isRelative() const {
        return !mAbsolute;
    }
    
    inline bool SoraPath::isDirectory() const {
        return mName.empty();
    }
    
    inline bool SoraPath::isFile() const {
        return !mName.empty();
    }
    
    inline bool SoraPath::isEmpty() const {
        return mName.empty() && mDirs.empty();
    }
    
    inline std::string SoraPath::getFileName() const {
        return mName;
    }
    
    inline size_t SoraPath::depth() const {
        return mDirs.size();
    }
    
    inline SoraPath SoraPath::forDirectory(const std::string& path) {
        SoraPath p;
        return p.parseDirectory(path);
    }
    
    inline SoraPath SoraPath::forDirectory(const std::wstring& path) {
        return forDirectory(sora::ws2s(path));
    }
    
    inline char SoraPath::separator() {
#ifdef OS_WIN32
        return '\\';
#else
        return '/';
#endif
    }
    
    inline char SoraPath::pathSeparator() {
#ifdef OS_WIN32
        return ';';
#else 
        return ':';
#endif
    }
    
    inline void swap(SoraPath& p1, SoraPath& p2) {
        p1.swap(p2);
    }
    
} // namespace sora



#endif
