//
//  SoraConfigParserImpl_Xml.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraConfigParserImpl_Xml.h"
#include "SoraResourceFile.h"
#include "SoraStringTokenlizer.h"
#include "SoraStringConv.h"

namespace sora {
    
    SoraConfigParserXmlImpl::SoraConfigParserXmlImpl():
    mDocument(0) {
        
    }
    
    SoraConfigParserXmlImpl::~SoraConfigParserXmlImpl() {
        close();
    }
    
    bool SoraConfigParserXmlImpl::open(const StringType& path) {
        SoraResourceFile file(path);
        if(file.isValid()) {
            return open(file, file.size());
        }
        return false;
    }
    
    bool SoraConfigParserXmlImpl::open(void* data, uint32 size) {
        if(mDocument) {
            delete mDocument;
        }
        mDocument = new pugi::xml_document;
        if(mDocument->load_buffer(data, size)) {
            mCurrNode = mDocument->root();
            return true;
        }
        return false;
    }
    
    bool SoraConfigParserXmlImpl::create() {
        if(mDocument) {
            delete mDocument;
        }
        mDocument = new pugi::xml_document;
        mCurrNode = mDocument->root();
        return true;
    }
    
    void SoraConfigParserXmlImpl::close() {
        if(mDocument) {
            delete mDocument;
            mDocument = 0;
        }
    }
    
    bool SoraConfigParserXmlImpl::hasNode(const std::string& node) const {
        sora_assert(mDocument);
        
        pugi::xml_node nextNode = node[0] == '/' ? mCurrNode.root() : mCurrNode;
        
        SoraStringTokenlizer tokens(node, "/");
        if(tokens.size() == 0)
            return false;
        
        SoraStringTokenlizer::iterator it = tokens.begin();
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
                continue;
            }
            
            nextNode = nextNode.child((*it).c_str());
            if(!nextNode)
                return false;
            ++it;
        }
        return true;
    }
    
    std::string SoraConfigParserXmlImpl::getCurrentNodeName() const {
        sora_assert(mDocument);
        
        return mCurrNode.name();
    }
    
    std::string SoraConfigParserXmlImpl::getCurrentNodePath() const {
        sora_assert(mDocument);
        
        return mCurrNode.path();
    }
    
    bool SoraConfigParserXmlImpl::toNode(const std::string& node) {
        sora_assert(mDocument);
        
        if(node.size() == 1 && node[0] == '/') {
            mCurrNode = mDocument->root();
            return true;
        }
        
        SoraStringTokenlizer tokens(node, "/");
        if(tokens.size() == 0)
            return false;
        
        pugi::xml_node nextNode = node[0] == '/' ? mCurrNode.root() : mCurrNode;
        SoraStringTokenlizer::iterator it = tokens.begin();
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
                continue;
            }
            
            nextNode = nextNode.child((*it).c_str());
            if(!nextNode)
                return false;
            ++it;
        }
        
        mCurrNode = nextNode;
        return true;
    }
    
    bool SoraConfigParserXmlImpl::toFirstChild(const std::string& name) {
        sora_assert(mDocument);
        
        pugi::xml_node node;
        if(name.empty()) {
            node = mCurrNode.first_child();
        } else {
            node = mCurrNode.child(name.c_str());
        }
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool SoraConfigParserXmlImpl::toNextChild(const std::string& name) {
        sora_assert(mDocument);
        
        pugi::xml_node node;
        if(name.empty()) {
            node = mCurrNode.next_sibling();
        } else {
            node = mCurrNode.next_sibling(name.c_str());
        }
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool SoraConfigParserXmlImpl::toParent() {
        sora_assert(mDocument);
        
        pugi::xml_node node = mCurrNode.parent();
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool SoraConfigParserXmlImpl::hasAttribute(const std::string& name) const {
        sora_assert(mDocument);
        
        pugi::xml_attribute attr = mCurrNode.attribute(name.c_str());
        if(attr) {
            return true;
        }
        return false;
    }
    
    std::vector<std::string> SoraConfigParserXmlImpl::getAttributes() const {
        sora_assert(mDocument);
        
        pugi::xml_attribute_iterator it = mCurrNode.attributes_begin();
        pugi::xml_attribute_iterator end = mCurrNode.attributes_end();
        
        std::vector<std::string> attributes;
        for(; it != end; ++it) {
            attributes.push_back(std::string(it->value()));
        }
        return attributes;
    }
    
    std::string SoraConfigParserXmlImpl::getString(const std::string& attr) const {
        sora_assert(mDocument);
        
        if(attr.empty()) {
            return mCurrNode.child_value();
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return std::string(attribute.value());
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            return c;
        }
        return std::string();
    }
    
    std::wstring SoraConfigParserXmlImpl::getWString(const std::string& attr) const {
        sora_assert(mDocument);
        
        return s2ws(getString(attr));
    }
    
    bool SoraConfigParserXmlImpl::getBool(const std::string& attr) const {
        sora_assert(mDocument);
        
        if(attr.empty()) {
            return StringType(mCurrNode.child_value()).asBool();
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_bool();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asBool();
        }
        return false;
    }
    
    int32 SoraConfigParserXmlImpl::getInt(const std::string& attr) const {
        sora_assert(mDocument);
        
        if(attr.empty()) {
            return StringType(mCurrNode.child_value()).asInt();
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_int();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asInt();
        }
        return 0;
    }
    
    float SoraConfigParserXmlImpl::getFloat(const std::string& attr) const {
        sora_assert(mDocument);
        
        if(attr.empty()) {
            return StringType(mCurrNode.child_value()).asFloat();
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_float();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asFloat();
        }
        return 0.f;
    }
    
    std::string SoraConfigParserXmlImpl::getString(const std::string& attr, const std::string& opt) const {
        sora_assert(mDocument);
        
        if(attr.empty())
            return mCurrNode.child_value();
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.value();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return c;
        }
        return opt;
    }
    
    std::wstring SoraConfigParserXmlImpl::getWString(const std::string& attr, const std::wstring& opt) const {
        sora_assert(mDocument);
        
        if(attr.empty())
            return s2ws(mCurrNode.child_value());
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return s2ws(attribute.value());
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return s2ws(c);
        }
        return opt;
    }
    
    bool SoraConfigParserXmlImpl::getBool(const std::string& attr, bool opt) const {
        sora_assert(mDocument);
        
        if(attr.empty()) {
            return StringType(mCurrNode.child_value()).asBool();
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_bool();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asBool();
        }
        return opt;
    }
    
    int32 SoraConfigParserXmlImpl::getInt(const std::string& attr, int32 opt) const {
        sora_assert(mDocument);
        
        if(attr.empty())
            return StringType(mCurrNode.child_value()).asInt();
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_int();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asInt();
        }
        return opt;
    }
    
    float SoraConfigParserXmlImpl::getFloat(const std::string& attr, float opt) const {
        sora_assert(mDocument);
        
        if(attr.empty())
            return StringType(mCurrNode.child_value()).asFloat();
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_float();
        } else {
            std::string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return StringType(c).asFloat();
        }
        return opt;
    }

    std::string SoraConfigParserXmlImpl::writeToString() const {
        sora_assert(mDocument);
        
        myWriter writter;
        mDocument->save(writter);
        return writter.str;
    }
    
    bool SoraConfigParserXmlImpl::writeToFile(const StringType& path) {
        sora_assert(mDocument);
        
        return mDocument->save_file(path.c_str());
    }

    bool SoraConfigParserXmlImpl::beginNode(const std::string& name) {
        sora_assert(mDocument);
        
        pugi::xml_node node = mCurrNode.append_child(name.c_str());
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    void SoraConfigParserXmlImpl::endNode() {
        toParent();
    }
    
    void SoraConfigParserXmlImpl::setValue(const std::string& val) {
        sora_assert(mDocument);
        
        mCurrNode.set_value(val.c_str());
    }
    
    void SoraConfigParserXmlImpl::setString(const std::string& attr, const std::string& val) {
        sora_assert(mDocument);
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val.c_str());
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val.c_str());
        }
    }
    
    void SoraConfigParserXmlImpl::setBool(const std::string& attr, bool val) {
        sora_assert(mDocument);
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
    
    void SoraConfigParserXmlImpl::setInt(const std::string& attr, int32 val) {
        sora_assert(mDocument);
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
    
    void SoraConfigParserXmlImpl::setFloat(const std::string& attr, float val) {
        sora_assert(mDocument);
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
} // namespace sora 