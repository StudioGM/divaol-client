//
//  SoraConfigParserImpl.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConfigParserImpl_h
#define Sora_SoraConfigParserImpl_h

#include "SoraPlatform.h"
#include "SoraString.h"

#include <vector>

namespace sora {
    
    class SoraConfigParserImpl {
    public:
        SoraConfigParserImpl() {}
        virtual ~SoraConfigParserImpl() {}
        
        virtual bool open(const StringType& path) = 0;
        virtual bool open(void* data, uint32 size) = 0;
        virtual bool create() = 0;
        virtual bool writeToFile(const StringType& path) = 0;
        virtual std::string writeToString() const = 0;
        
        virtual void close() = 0;
        
        virtual bool hasNode(const std::string& node) const = 0;
        virtual std::string getCurrentNodeName() const = 0;
        virtual std::string getCurrentNodePath() const = 0;
        
        virtual bool toNode(const std::string& node) = 0;
        
        virtual bool toFirstChild(const std::string& name=std::string()) = 0;
        virtual bool toNextChild(const std::string& name=std::string()) = 0;
        virtual bool toParent() = 0;
        
        virtual bool hasAttribute(const std::string& attr) const = 0;
        virtual std::vector<std::string> getAttributes() const = 0;
        
        virtual std::string getString(const std::string& attr) const = 0;
        virtual std::wstring getWString(const std::string& attr) const = 0;
        virtual bool getBool(const std::string& attr) const = 0;
        virtual int32 getInt(const std::string& attr) const = 0;
        virtual float getFloat(const std::string& attr) const = 0;
        
        virtual std::string getString(const std::string& attr, const std::string& opt) const = 0;
        virtual std::wstring getWString(const std::string& attr, const std::wstring& opt) const = 0;
        virtual bool getBool(const std::string& attr, bool opt) const = 0;
        virtual int32 getInt(const std::string& attr, int32 opt) const = 0;
        virtual float getFloat(const std::string& attr, float opt) const = 0;
        
        virtual bool beginNode(const std::string& name) = 0;
        virtual void endNode() = 0;
                
        virtual void setValue(const std::string& val) = 0;
        virtual void setString(const std::string& attr, const std::string& val) = 0;
        virtual void setBool(const std::string& attr, bool val) = 0;
        virtual void setInt(const std::string& attr, int32 val) = 0;
        virtual void setFloat(const std::string& attr, float val) = 0; 
    };
    
}



#endif
