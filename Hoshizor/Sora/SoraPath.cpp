//
//  SoraPath.cpp
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPath.h"

#include "SoraException.h"

namespace sora {
    
    SoraPath::SoraPath(): mAbsolute(false) {
        
    }
    
    SoraPath::SoraPath(bool absolute): mAbsolute(absolute) {
        
    }
    
    SoraPath::SoraPath(const std::string& path) {
        assign(path);
    }
    
    SoraPath::SoraPath(const std::wstring& path) {
        assign(sora::ws2s(path));
    }
    
    SoraPath::SoraPath(const std::string& path, Style style)  {
        assign(path, style);
    }
    
    SoraPath::SoraPath(const std::wstring& path, Style style)  {
        assign(sora::ws2s(path), style);
    }
    
    SoraPath::SoraPath(const char* path) {
        assign(path);
    }
    
    SoraPath::SoraPath(const wchar_t* path) {
        assign(path);
    }
    
    SoraPath::SoraPath(const char* path, Style style)  {
        assign(path, style);
    }
    
    SoraPath::SoraPath(const wchar_t* path, Style style)  {
        assign(sora::ws2s(path), style);
    }
    
    SoraPath::SoraPath(const SoraPath& path):
    mName(path.mName),
    mDirs(path.mDirs),
	mDevice(path.mDevice),
	mNode(path.mNode),
    mAbsolute(path.mAbsolute) {
        
    }
    
    SoraPath::SoraPath(const SoraPath& parent, const std::string& filename):
    mName(parent.mName),
    mDirs(parent.mDirs),
	mDevice(parent.mDevice),
	mNode(parent.mNode),
    mAbsolute(parent.mAbsolute) {
        makeDirectory();
        mName = filename;
    }
    
    SoraPath::SoraPath(const SoraPath& parent, const std::wstring& filename):
    mName(parent.mName),
    mDirs(parent.mDirs),
	mDevice(parent.mDevice),
	mNode(parent.mNode),
    mAbsolute(parent.mAbsolute) {
        makeDirectory();
        mName = sora::ws2s(filename);
    }
    
    SoraPath::SoraPath(const SoraPath& parent, const SoraPath& relative):
    mName(parent.mName),
    mDirs(parent.mDirs),
	mDevice(parent.mDevice),
	mNode(parent.mNode),
    mAbsolute(parent.mAbsolute) {
        resolve(relative);
    }
    
    SoraPath::~SoraPath() {
    }
    
    SoraPath& SoraPath::operator = (const SoraPath& path) {
        if(&path != this) {
			mName = path.mName;
			mDevice = path.mDevice;
			mNode = path.mNode;
			mDirs = path.mDirs;
			mAbsolute = path.mAbsolute;
		}
		return *this;
    }
    
    SoraPath& SoraPath::operator = (const std::string& path) {
        return assign(path);
    }
    
    SoraPath& SoraPath::operator = (const std::wstring& path) {
        return assign(sora::ws2s(path));
    }
    
    SoraPath& SoraPath::operator = (const char* path) {
        return assign(path);
    }
    
    SoraPath& SoraPath::operator = (const wchar_t* path) {
        return assign(path);
    }
    
    void SoraPath::swap(SoraPath& path) {
        std::swap(mName, path.mName);
        std::swap(mDirs, path.mDirs);
		std::swap(mDevice, path.mDevice);
		std::swap(mNode, path.mNode);
        std::swap(mAbsolute, path.mAbsolute);
    }
    
    SoraPath& SoraPath::assign(const std::string& path) {
#ifdef OS_WIN32
        parseWindows(path);
#else 
        parseUnix(path);
#endif
        return *this;
    }
    
    SoraPath& SoraPath::assign(const char* path) {
#ifdef OS_WIN32
        parseWindows(path);
#else 
        parseUnix(path);
#endif
        return *this;
    }
    
    SoraPath& SoraPath::assign(const wchar_t* path) {
        assign(sora::ws2s(path));
        return *this;
    }
    
    SoraPath& SoraPath::assign(const std::string& path, Style style) {
        switch(style) {
            case PATH_UNIX:
                parseUnix(path);
                break;
            case PATH_WINDOWS:
                parseWindows(path);
                break;
            case PATH_NATIVE:
                assign(path);
                break;
            case PATH_GUESS:
                parseGuess(path);
                break;
        }
        return *this;
    }
    
    std::string SoraPath::toString() const {
#ifdef OS_WIN32
        return buildWindows();
#else
        return buildUnix();
#endif
    }
    
    std::string SoraPath::toString(Style style) const {
        switch (style) {
            case PATH_UNIX:
                return buildUnix();
            case PATH_WINDOWS:
                return buildWindows();
            case PATH_NATIVE:
            case PATH_GUESS:
                return toString();
        }
        return std::string();
    }
    
    std::wstring SoraPath::toWString() const {
        return s2ws(toString());
    }
    
    SoraPath& SoraPath::parseDirectory(const std::string& path) {
        assign(path);
        return makeDirectory();
    }
    
    SoraPath& SoraPath::parseDirectory(const std::string& path, Style style) {
        assign(path, style);
        return makeDirectory();
    }
    
    SoraPath& SoraPath::parseDirectory(const std::wstring& path) {
        assign(sora::ws2s(path));
        return makeDirectory();
    }
    
    SoraPath& SoraPath::parseDirectory(const std::wstring& path, Style style) {
        assign(sora::ws2s(path), style);
        return makeDirectory();
    }
    
    SoraPath& SoraPath::makeDirectory() {
        mDirs.push_back(mName);
        mName.clear();
        return *this;
    }
    
    SoraPath& SoraPath::makeFile() {
        if(!mDirs.empty() && mName.empty()) {
            mName = mDirs.back();
            mDirs.pop_back();
        }
        return *this;
    }
    
    SoraPath& SoraPath::makeAbsolute() {
        return makeAbsolute(SoraPath(current()));
    }
    
    SoraPath& SoraPath::makeAbsolute(const SoraPath& base) {
        if(!mAbsolute) {
            SoraPath tmp = base;
            tmp.makeDirectory();
            for(StringVec::const_iterator it = mDirs.begin(); 
                it != mDirs.end(); 
                ++it) {
                tmp.pushDirectory(*it);
            }
            mDirs = tmp.mDirs;
			mDevice = tmp.mDevice;
			mNode = tmp.mNode;
            mAbsolute = base.mAbsolute;
        }
        return *this;
    }
    
    SoraPath SoraPath::absolute() const {
        SoraPath result(*this);
        if(!result.mAbsolute) {
            result.makeAbsolute();
        }
        return result;
    }
    
    SoraPath SoraPath::absolute(const SoraPath& base) const {
        SoraPath result(*this);
        if(!result.mAbsolute) {
            result.makeAbsolute(base);
        }
        return result;
    }
    
    SoraPath SoraPath::parent() const {
        SoraPath result(*this);
        return result.makeParent();
    }
    
    SoraPath& SoraPath::makeParent() {
        if(mName.empty()) {
            if(mDirs.empty()) {
                if(!mAbsolute) {
                    mDirs.push_back("..");
                }
            } else {
                if(mDirs.back() == "..")
                    mDirs.push_back("..");
                else
                    mDirs.pop_back();
            }
        } else {
            mName.clear();
        }
        return *this;
    }
    
    SoraPath& SoraPath::append(const SoraPath& path) {
        makeDirectory();
        mDirs.insert(mDirs.end(), path.mDirs.begin(), path.mDirs.end());
        mName = path.mName;
        return *this;
    }
    
    SoraPath& SoraPath::resolve(const SoraPath& path) {
        if(path.isAbsolute()) {
            *this = path;
        } else {
            for(int i=0; i<path.depth(); ++i) {
                pushDirectory(path.mDirs[i]);
            }
            mName = path.mName;
        }
        return *this;
    }
    
    void SoraPath::pushDirectory(const std::string& dir) {
        if(!dir.empty() && dir != ".") {
            if(dir == "..") {
                if(!mDirs.empty() && mDirs.back() != "..")
                    mDirs.pop_back();
                else if(!mAbsolute)
                    mDirs.push_back(dir);
            } else
                mDirs.push_back(dir);
        }
    }
    
    void SoraPath::popDirectory() {
        if(!mDirs.empty())
            mDirs.pop_back();
    }
    
    void SoraPath::setFileName(const std::string& name) {
        mName = name;
    }
    
    void SoraPath::setFileName(const std::wstring& name) {
        setFileName(sora::ws2s(name));
    }
    
    void SoraPath::setBaseName(const std::string& name) {
        std::string ext = getExtension();
        mName = name;
        if(!ext.empty()) {
            mName.append(".");
            mName.append(ext);
        }
    }
    
    void SoraPath::setBaseName(const std::wstring& name) {
        setBaseName(sora::ws2s(name));
    }
    
    void SoraPath::setNode(const std::string& node) {
        mNode = node;
        mAbsolute = mAbsolute || mNode.empty();
    }
    
    void SoraPath::setNode(const std::wstring& node) {
        setNode(sora::ws2s(node));
    }
    
    void SoraPath::setDevice(const std::string& device) {
        mDevice = device;
        mAbsolute = mAbsolute || mDevice.empty();
    }
    
    void SoraPath::setDevice(const std::wstring& device) {
        setDevice(sora::ws2s(device));
    }
    
    std::string SoraPath::getDevice() const {
        return mDevice;
    }
    
    std::string SoraPath::getNode() const {
        return mNode;
    }
    
    std::string SoraPath::getBaseName() const {
        std::string::size_type pos = mName.rfind('.');
        if(pos != std::string::npos) {
            return mName.substr(0, pos);
        } else 
            return mName;
    }
    
    void SoraPath::setExtension(const std::string& extension) {
        mName = getBaseName();
        if(!extension.empty()) {
            mName.append(".");
            mName.append(extension);
        }
    }
    
    void SoraPath::setExtension(const std::wstring& extension) {
        setExtension(sora::ws2s(extension));
    }
    
    std::string SoraPath::getExtension() const {
        std::string::size_type pos = mName.rfind('.');
        if(pos != std::string::npos) {
            return mName.substr(pos+1);
        } else 
            return std::string();
    }
    
    void SoraPath::clear() {
        mName.clear();
        mDirs.clear();
        mDevice.clear();
        mNode.clear();
        mAbsolute = false;
    }
    
    std::string SoraPath::getEnv(const std::string& env) {
        return getEnvImpl(env);
    }
    
    std::string SoraPath::current() {
        return SoraPathImpl::currentImpl();
    }
    
    std::string SoraPath::home() {
        return SoraPathImpl::homeImpl();
    }
    
    std::string SoraPath::temp() {
        return SoraPathImpl::tempImpl();
    }
    
    std::string SoraPath::null() {
        return SoraPathImpl::nullImpl();
    }
    
    std::string SoraPath::resource() {
        return SoraPathImpl::resourceImpl();
    }
    
    std::string SoraPath::writtable() {
        return SoraPathImpl::writtableImpl();
    }
    
    std::wstring SoraPath::getEnvW(const std::string& env) {
        return sora::s2ws(getEnv(env));
    }
    
    std::wstring SoraPath::currentW() {
        return sora::s2ws(current());
    }
    
    std::wstring SoraPath::homeW() {
        return sora::s2ws(home());
    }
    
    std::wstring SoraPath::tempW() {
        return sora::s2ws(temp());
    }
    
    std::wstring SoraPath::nullW() {
        return sora::s2ws(null());
    }
    
    std::wstring SoraPath::resourceW() {
        return sora::s2ws(resource());
    }
    
    std::wstring SoraPath::writtableW() {
        return sora::s2ws(writtable());
    }
    
    std::string SoraPath::font() {
        return fontImpl();
    }
    
    std::wstring SoraPath::fontW() {
        return sora::s2ws(font());
    }
    
    std::string SoraPath::expand(const std::string& path) {
        return SoraPathImpl::expandImpl(path);
    }
    
    std::string SoraPath::expand(const std::wstring& path) {
        return SoraPathImpl::expandImpl(sora::ws2s(path));
    }
    
    void SoraPath::listRoots(std::vector<std::string>& roots) {
        SoraPathImpl::listRootsImpl(roots);
    }
    
    void SoraPath::parseUnix(const std::string& path) {
        clear();
        
        std::string::const_iterator it = path.begin();
        std::string::const_iterator end = path.end();
        if(it != end) {
            if(*it == '/') {
                mAbsolute = true; ++it;
            } else if(*it == '~') {
                ++it;
                if(it == end || *it == '/') {
                    SoraPath cwd(home());
                    mDirs = cwd.mDirs;
					mDevice = cwd.mDevice;
					mNode = cwd.mNode;
                    mAbsolute = true;
                } else 
                    --it;
            } else if(*it == '.') {
                ++it;
                if(it == end || *it == '/') {
                    SoraPath cwd(current());
                    mDirs = cwd.mDirs;
					mDevice = cwd.mDevice;
					mNode = cwd.mNode;
                    mAbsolute = true;
                }
            }
            
            while(it != end) {
                std::string name;
                while(it != end && *it != '/') {
                    name += *it++;
                }
                if(it != end) {
                    if(!mDirs.empty()) {
                        if(!name.empty() && *(name.rbegin()) == ':')
                            mDevice.assign(name, 0, name.length()-1);
                        else
                           pushDirectory(name);
                    } else 
                           pushDirectory(name);
                } else 
                    mName = name;
                if(it != end) 
                    ++it;
            }
        }
    }
    
    void SoraPath::parseWindows(const std::string& path) {
        clear();
        
        std::string::const_iterator it = path.begin();
        std::string::const_iterator end = path.end();
        if(it != end) {
            if(*it == '\\' || *it == '/') {
                mAbsolute = true;
                ++it;
            } else if(*it == '.') {
                ++it;
                if(it == end || *it == '/') {
                    SoraPath cwd(current());
                    mDirs = cwd.mDirs;
					mDevice = cwd.mDevice;
					mNode = cwd.mNode;
                    mAbsolute = true;
                }
            }

            if(mAbsolute && it != end && (*it == '\\' || *it == '/')) {
                ++it;
                while(it != end && *it != '\\' && *it != '/')
                    mNode += *it++;
                if(it != end)
                    ++it;
            } else if(it != end) {
                char d = *it++;
                if(it != end && *it == ':') {
                    if(mAbsolute || !((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z'))) 
                        THROW_SORA_EXCEPTION(InvalidArgumentException, "Error parsing path");
                    mAbsolute = true;
                    mDevice += d;
                    ++it;
                    if(it == end || (*it != '\\' && *it != '/'))
                       THROW_SORA_EXCEPTION(InvalidArgumentException, "Error parsing path");
                    ++it;
                } else 
                    --it;
            }
            
            while(it != end) {
                std::string name;
                while(it != end && *it != '\\' && *it != '/') 
                    name += *it++;
                if(it != end)
                    pushDirectory(name);
                else
                    mName = name;
                if(it != end) 
                    ++it;
            }
            
            if(!mNode.empty() && mDirs.empty() && !mName.empty())
                makeDirectory();
            
        }
    }
    
    void SoraPath::parseGuess(const std::string& path) {
        bool hasBackslash = false;
        bool hasSlash = false;
        bool isWindows = path.length() > 2 && path[1] == ':' && (path[2] == '/' || path[2] == '\\');
        std::string::const_iterator end    = path.end();    
        
        if(!isWindows) {
            for(std::string::const_iterator it = path.begin();
                it != end;
                ++it) {
                switch(*it) {
                    case '\\': hasBackslash = true; break;
                    case '/': hasSlash = true; break;
                }
            }
        }
        if(hasBackslash || isWindows)
            parseWindows(path);
        else
            parseUnix(path);
    }
    
    std::string SoraPath::buildUnix() const {
        std::string result;
        if(!mDevice.empty()) {
            result.append("/");
            result.append(mDevice);
            result.append(":/");
        } else if(mAbsolute) {
            result.append("/");
        } 
        for(StringVec::const_iterator it = mDirs.begin();
            it != mDirs.end();
            ++it) {
            result.append(*it);
            result.append("/");
        }
        result.append(mName);
        return result;
    }
    
    std::string SoraPath::buildWindows() const {
        std::string result;
        if(!mDevice.empty()) {
            result.append(mDevice);
            result.append(":\\");
        } else if(!mNode.empty()) {
            result.append("\\\\");
            result.append(mNode);
            result.append("\\");
        }  
        for(StringVec::const_iterator it = mDirs.begin();
            it != mDirs.end();
            ++it) {
            result.append(*it);
            result.append("\\");
        }
        result.append(mName);
        return result;
    }
    
    
} // namespace sora
