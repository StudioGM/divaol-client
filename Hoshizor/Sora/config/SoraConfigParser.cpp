//
//  SoraConfigParser.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraConfigParser.h"

#include "SoraConfigParserImpl_Xml.h"
#include "SoraConfigParserImpl_Json.h"

#include "SoraResourceFile.h"

namespace sora {
    
    SoraConfigParser::SoraConfigParser():
    mImpl(0) {
        
    }
    
    SoraConfigParser::~SoraConfigParser() {
        close();
    }
    
    ConfigType SoraConfigParser::fileNameToConfigType(const StringType& path) {
        std::string name = path.get();
        if(name.find(".xml") != std::string::npos) {
            return CONFIG_XML;
        } else if(name.find(".json") != std::string::npos) {
            return CONFIG_JSON;
        } else if(name.find(".ini") != std::string::npos) {
            return CONFIG_INI;
        }
        return CONFIG_UNKNOWN;
    }
    
    inline bool isSpace(char c) {
        return c == '\n' || c == ' ' || c == '\r' || c == '\t';
    }
    
    ConfigType SoraConfigParser::fileDataToConfigType(void* data, uint32 size) {
        const char* cdata = static_cast<const char*>(data);
        size_t pos = 0;
        while(pos < size && isSpace(cdata[pos])) {
            ++pos;
        }
        switch(cdata[pos]) {
            // xml starts with '<'
            case '<':
                return CONFIG_XML;
            // json starts with '{'
            case '{':
                return CONFIG_JSON;
                
            default:
                return CONFIG_INI;
        }
        return CONFIG_UNKNOWN;
    }
    
    bool SoraConfigParser::open(const StringType& path, ConfigType type) {
        if(mImpl) {
            close();
        }
        switch(type) {
            case CONFIG_JSON:
                mImpl = new SoraConfigParserJsonImpl;
                break;
            case CONFIG_XML:
                mImpl = new SoraConfigParserXmlImpl;
                break;
            case CONFIG_AUTO_DETECT:
                ConfigType type = fileNameToConfigType(path);
                if(type != CONFIG_UNKNOWN)
                    return open(path, type);
                else {
                    SoraResourceFile resource(path);
                    if(resource.isValid()) {
                        return open(resource, resource.size(), CONFIG_AUTO_DETECT);
                    }
                    return false;
                }
                break;
                
        }
        if(mImpl) {
            return mImpl->open(path);
        }
        return false;
    }
    
    bool SoraConfigParser::open(void* data, uint32 size, ConfigType type) {
        if(mImpl) {
            close();
        }
        switch(type) {
            case CONFIG_JSON:
                mImpl = new SoraConfigParserJsonImpl;
                break;
            case CONFIG_XML:
                mImpl = new SoraConfigParserXmlImpl;
                break;
            case CONFIG_AUTO_DETECT:
                ConfigType type = fileDataToConfigType(data, size);
                if(type != CONFIG_UNKNOWN) {
                    return open(data, size, type);
                }
                return false;
                break;
                
        }
        if(mImpl) {
            return mImpl->open(data, size);
        }
        return false;
    }
    
    bool SoraConfigParser::openString(const std::string& string, ConfigType type) {
        return open((void*)string.c_str(), (uint32)string.size(), type);
    }
    
    bool SoraConfigParser::writeToFile(const StringType& path) {
        sora_assert(mImpl);
        return mImpl->writeToFile(path);
    }
    
    std::string SoraConfigParser::writeToString() const {
        sora_assert(mImpl);
        return mImpl->writeToString();
    }
    
    void SoraConfigParser::close() {
        if(mImpl) {
            mImpl->close();
            delete mImpl;
            mImpl = 0;
        }
    }
    
    bool SoraConfigParser::hasNode(const std::string& node) const {
        sora_assert(mImpl);
        return mImpl->hasNode(node);
    }
    
    std::string SoraConfigParser::getCurrentNodeName() const {
        sora_assert(mImpl);
        return mImpl->getCurrentNodeName();
    }
    
    std::string SoraConfigParser::getCurrentNodePath() const {
        sora_assert(mImpl);
        return mImpl->getCurrentNodePath();
    }
    
    bool SoraConfigParser::toNode(const std::string& node) {
        sora_assert(mImpl);
        return mImpl->toNode(node);
    }
    
    bool SoraConfigParser::toFirstChild(const std::string& name) {
        sora_assert(mImpl);
        return mImpl->toFirstChild(name);
    }
    
    bool SoraConfigParser::toNextChild(const std::string& name) {
        sora_assert(mImpl);
        return mImpl->toNextChild(name);
    }
    
    bool SoraConfigParser::toParent() {
        sora_assert(mImpl);
        return mImpl->toParent();
    }

    bool SoraConfigParser::hasAttribute(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->hasAttribute(attr);
    }
    
    std::vector<std::string> SoraConfigParser::getAttributes() const {
        sora_assert(mImpl);
        return mImpl->getAttributes();
    }
    
    std::string SoraConfigParser::getString(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getString(attr);
    }
    
    std::wstring SoraConfigParser::getWString(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getWString(attr);
    }
    
    bool SoraConfigParser::getBool(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getBool(attr);
    }
    
    int32 SoraConfigParser::getInt(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getInt(attr);
    }
    
    float SoraConfigParser::getFloat(const std::string& attr) const {
        sora_assert(mImpl);
        return mImpl->getFloat(attr);
    }
    
    std::string SoraConfigParser::getString(const std::string& attr, const std::string& opt) const {
        sora_assert(mImpl);
        return mImpl->getString(attr, opt);
    }
    
    std::wstring SoraConfigParser::getWString(const std::string& attr, const std::wstring& opt) const {
        sora_assert(mImpl);
        return mImpl->getWString(attr, opt);
    }
    
    bool SoraConfigParser::getBool(const std::string& attr, bool opt) const {
        sora_assert(mImpl);
        return mImpl->getBool(attr, opt);
    }
    
    int32 SoraConfigParser::getInt(const std::string& attr, int32 opt) const {
        sora_assert(mImpl);
        return mImpl->getInt(attr, opt);
    }
    
    float SoraConfigParser::getFloat(const std::string& attr, float opt) const {
        sora_assert(mImpl);
        return mImpl->getFloat(attr, opt);
    }
    
    bool SoraConfigParser::beginNode(const std::string& nodeName) {
        sora_assert(mImpl);
        return mImpl->beginNode(nodeName);
    }
    
    void SoraConfigParser::endNode() {
        sora_assert(mImpl);
        return mImpl->endNode();
    }
    
    void SoraConfigParser::setString(const std::string& attr, const std::string& val) {
        sora_assert(mImpl);
        return mImpl->setString(attr, val);
    }
    
    void SoraConfigParser::setBool(const std::string& attr, bool val) {
        sora_assert(mImpl);
        return mImpl->setBool(attr, val);
    }
    
    void SoraConfigParser::setInt(const std::string& attr, int32 val) {
        sora_assert(mImpl);
        return mImpl->setInt(attr, val);
    }
    
    void SoraConfigParser::setFloat(const std::string& attr, float val) {
        sora_assert(mImpl);
        return mImpl->setFloat(attr, val);
    }
    
    void SoraConfigParser::setValue(const std::string& val) {
        sora_assert(mImpl);
        mImpl->setValue(val);
    }
    
} // namespace sora