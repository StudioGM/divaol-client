//
//  SoraConfigParserImpl_Json.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraConfigParserImpl_Json.h"
#include "SoraResourceFile.h"
#include "SoraStringTokenlizer.h"
#include "SoraStringConv.h"
#include "SoraMemoryBuffer.h"

namespace sora {

    SoraConfigParserJsonImpl::SoraConfigParserJsonImpl() {
        mCurrChildIndex = -1;
        mIsWritting = false;
    }
    
    bool SoraConfigParserJsonImpl::open(const StringType& path) {
        SoraResourceFile file(path);
        if(file.isValid()) {
            return open(file, file.size());
        }
        return false;
    }
    
    bool SoraConfigParserJsonImpl::open(void* data, uint32 size) {
        if(mOpened)
            close();
        
        const char* cdata = static_cast<const char*>(data);
        if(mParser.parse(cdata, cdata+size, mRootValue)) {
            mOpened = true;
            mCurrValue = mRootValue;
            mCurrPath = "/";
        }
        else
            mOpened = false;
        return mOpened;
    }
    
    bool SoraConfigParserJsonImpl::create() {
        if(mOpened) {
            mRootValue = Json::Value();
            mCurrChildIndex = -1;
            mCurrPath = "/";
        }
        mOpened = true;
        return true;
    }
    
    void SoraConfigParserJsonImpl::close() {
        mOpened = false;
    }
    
    bool SoraConfigParserJsonImpl::hasNode(const std::string& node) const {
        sora_assert(mOpened);
        
        SoraStringTokenlizer tokens(node, "/");
        SoraStringTokenlizer::iterator it = tokens.begin();
        
        Json::Value nextNode;
        if(node[0] != '/')
            nextNode = mCurrValue;
        else
            nextNode = mRootValue;
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
                continue;
            }
            
            if(nextNode.isArray() ||
               nextNode.empty())
                return false;
            
            nextNode = nextNode[*it];
        }
        return true;
    }
    
    std::string SoraConfigParserJsonImpl::getCurrentNodeName() const {
        sora_assert(mOpened);
        
        return mCurrPath.getNode();
    }
    
    std::string SoraConfigParserJsonImpl::getCurrentNodePath() const {
        sora_assert(mOpened);
        
        return mCurrPath.toString();
    }
    
    bool SoraConfigParserJsonImpl::toNode(const std::string& node) {
        sora_assert(mOpened);
        
        if(node.size() == 1 && node[0] == '/') {
            mCurrValue = mRootValue;
            return true;
        }
        
        SoraStringTokenlizer tokens(node, "/");
        SoraStringTokenlizer::iterator it = tokens.begin();
        
        Json::Value nextNode;
        if(node[0] != '/')
            nextNode = mCurrValue;
        else
            nextNode = mRootValue;
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
               continue;
            }
            
            if(nextNode.isArray() ||
               nextNode.empty())
                return false;

            nextNode = nextNode[*it];
        }
        mCurrValue = nextNode;
        if(node[0] == '/')
            mCurrPath = node + "/";
        else
            mCurrPath.assign(mCurrPath.toString() + "/" + node + "/"); 
        mCurrChildIndex = -1;
        return true;
    }
    
    bool SoraConfigParserJsonImpl::toFirstChild(const std::string& name) {
        sora_assert(mOpened);
        
        if(mCurrValue.size() >= 1) {
            mCurrChildIndex = 0;
            mTmpParent = mCurrValue;
            mCurrValue = mTmpParent[mCurrChildIndex];
            return true;
        }
        return false;
    }
    
    bool SoraConfigParserJsonImpl::toNextChild(const std::string& name) {
        sora_assert(mOpened);
        
        if(mCurrChildIndex == -1)
            return false;
        ++mCurrChildIndex;
        if(mCurrValue.size() <= mCurrChildIndex) {
            --mCurrChildIndex;
            return false;
        }
        
        mCurrValue = mTmpParent[mCurrChildIndex];
        return true;
    }
    
    bool SoraConfigParserJsonImpl::toParent() {
        sora_assert(mOpened);
        
        if(mCurrChildIndex != -1) {
            mCurrValue = mTmpParent;
            mCurrChildIndex = -1;
            return true;
        }
        if(mCurrPath.depth() > 0)
            mCurrPath.popDirectory();
        else
            return false;
        return toNode(mCurrPath.toString());
    }
    
    bool SoraConfigParserJsonImpl::hasAttribute(const std::string& attr) const {
        sora_assert(mOpened);
        
        return mCurrValue.isMember(attr);
    }
    
    std::vector<std::string> SoraConfigParserJsonImpl::getAttributes() const {
        sora_assert(mOpened);
        
        return mCurrValue.getMemberNames();
    }
    
    std::string SoraConfigParserJsonImpl::getString(const std::string& attr) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asString();
        if(mCurrValue.isMember(attr))
            return mCurrValue[attr].asString();
        return std::string();
    }
    
    std::wstring SoraConfigParserJsonImpl::getWString(const std::string& attr) const {
        sora_assert(mOpened);
        
        return s2ws(getString(attr));
    }
    
    bool SoraConfigParserJsonImpl::getBool(const std::string& attr) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asBool();
        if(mCurrValue.isMember(attr))
            return mCurrValue[attr].asBool();
        return false;
    }
    
    int32 SoraConfigParserJsonImpl::getInt(const std::string& attr) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asInt();
        if(mCurrValue.isMember(attr))
            return mCurrValue[attr].asInt();
        return false;
    }
    
    float SoraConfigParserJsonImpl::getFloat(const std::string& attr) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return (float)mCurrValue.asDouble();
        if(mCurrValue.isMember(attr))
            return (float)mCurrValue[attr].asDouble();
        return 0.f;
    }
    
    std::string SoraConfigParserJsonImpl::getString(const std::string& attr, const std::string& opt) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asString();
        if(mCurrValue.isMember(attr))
            return mCurrValue.get(attr, opt).asString();
        return std::string();
    }
    
    std::wstring SoraConfigParserJsonImpl::getWString(const std::string& attr, const std::wstring& opt) const {
        sora_assert(mOpened);
        
        return s2ws(getString(attr, ws2s(opt)));
    }
    
    bool SoraConfigParserJsonImpl::getBool(const std::string& attr, bool opt) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asBool();
        if(mCurrValue.isMember(attr))
            return mCurrValue.get(attr, opt).asBool();
        return false;
    }
    
    int32 SoraConfigParserJsonImpl::getInt(const std::string& attr, int32 opt) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return mCurrValue.asInt();
        if(mCurrValue.isMember(attr))
            return mCurrValue.get(attr, opt).asInt();
        return 0;
    }
    
    float SoraConfigParserJsonImpl::getFloat(const std::string& attr, float opt) const {
        sora_assert(mOpened);
        
        if(attr.empty())
            return (float)mCurrValue.asDouble();
        if(mCurrValue.isMember(attr))
            return (float)mCurrValue.get(attr, opt).asDouble();
        return 0.f;
    }
    
    std::string SoraConfigParserJsonImpl::writeToString() const {
        sora_assert(mOpened);
        return mRootValue.toStyledString();
    }
    
    bool SoraConfigParserJsonImpl::writeToFile(const StringType& file) {
        sora_assert(mOpened);
        
        Json::StyledWriter writer;
        std::string data = writer.write(mRootValue);
        
        SoraMemoryBuffer buffer((void*)data.c_str(), data.size());
        return buffer.writeToFile(file);
    }
    
    bool SoraConfigParserJsonImpl::beginNode(const std::string& name) {
        sora_assert(mOpened);
        
        if(mCurrValue.isMember(name)) {
            mCurrValue = mCurrValue[name];
            mCurrPath.assign(mCurrPath.toString() + "/" + name + "/");
        }
        else {
            mWritingNodeName = name;
            mWrittingNode = Json::Value();
            mIsWritting = true;
        }
        return true;
    }
    
    void SoraConfigParserJsonImpl::endNode() {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mIsWritting = false;
            mCurrValue[mWritingNodeName] = mWrittingNode;
        }
    }
    
    void SoraConfigParserJsonImpl::setValue(const std::string& val) {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mWrittingNode = Json::Value(val);
        }
    }
    
    void SoraConfigParserJsonImpl::setString(const std::string& attr, const std::string& val) {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mWrittingNode[attr] = val;
        } else {
            mCurrValue[attr] = val;
        }
    }
    
    void SoraConfigParserJsonImpl::setBool(const std::string& attr, bool val) {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mWrittingNode[attr] = val;
        } else {
            mCurrValue[attr] = val;
        }
    }
    
    void SoraConfigParserJsonImpl::setInt(const std::string& attr, int32 val) {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mWrittingNode[attr] = val;
        } else {
            mCurrValue[attr] = val;
        }
    }
    
    void SoraConfigParserJsonImpl::setFloat(const std::string& attr, float val) {
        sora_assert(mOpened);
        
        if(mIsWritting) {
            mWrittingNode[attr] = val;
        } else {
            mCurrValue[attr] = val;
        }
    }
    
} // namespace sora

