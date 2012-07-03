//
//  SoraConfigParserImpl_Json.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConfigParserImpl_Json_h
#define Sora_SoraConfigParserImpl_Json_h

#include "SoraConfigParserImpl.h"
#include "SoraPath.h"

#include "json/json.h"

namespace sora {
    
    class SoraConfigParserJsonImpl: public SoraConfigParserImpl {
    public:
        SoraConfigParserJsonImpl();
        
        bool open(const StringType& path);
        bool open(void* data, uint32 size);
        bool create();
        bool writeToFile(const StringType& path);
        std::string writeToString() const;
        
        void close();
        
        bool hasNode(const std::string& node) const;
        std::string getCurrentNodeName() const;
        std::string getCurrentNodePath() const;
        
        bool toNode(const std::string& node);
        
        bool toFirstChild(const std::string& name=std::string());
        bool toNextChild(const std::string& name=std::string());
        bool toParent();
        
        bool hasAttribute(const std::string& attr) const;
        std::vector<std::string> getAttributes() const;
        
        std::string getString(const std::string& attr) const;
        std::wstring getWString(const std::string& attr) const;
        bool getBool(const std::string& attr) const;
        int32 getInt(const std::string& attr) const;
        float getFloat(const std::string& attr) const;
        
        std::string getString(const std::string& attr, const std::string& opt) const;
        std::wstring getWString(const std::string& attr, const std::wstring& opt) const;
        bool getBool(const std::string& attr, bool opt) const;
        int32 getInt(const std::string& attr, int32 opt) const;
        float getFloat(const std::string& attr, float opt) const;
        
        bool beginNode(const std::string& name);
        void endNode();
                
        void setValue(const std::string& val);
        void setString(const std::string& attr, const std::string& val);
        void setBool(const std::string& attr, bool val);
        void setInt(const std::string& attr, int32 val);
        void setFloat(const std::string& attr, float val);
        
    private:
        Json::Value& redNode(const std::string& path);
        
        Json::Reader mParser;
        Json::Value mRootValue;
        Json::Value mCurrValue;
        
        int mCurrChildIndex;
        Json::Value mTmpParent;
        
        bool mIsWritting;
        std::string mWritingNodeName;
        Json::Value mWrittingNode;
        
        SoraPath mCurrPath;
        
        bool mOpened;
    };
    
} // namespace sora

#endif
