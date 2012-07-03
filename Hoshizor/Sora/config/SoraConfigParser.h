//
//  SoraConfigurationParser.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConfigParser_h
#define Sora_SoraConfigParser_h

#include "prerequisites.h"
#include "SoraUncopyable.h"

#include <vector>

namespace sora {
    
    class SoraConfigParserImpl;
    
    class SORA_API SoraConfigParser: SoraUncopyable {
    public:
        SoraConfigParser();
        ~SoraConfigParser();
        
        bool open(const StringType& path, ConfigType type=CONFIG_AUTO_DETECT);
        bool open(void* data, uint32 size, ConfigType type=CONFIG_AUTO_DETECT);
        bool openString(const std::string& string, ConfigType type=CONFIG_AUTO_DETECT);
        bool create();
        bool writeToFile(const StringType& path);
        std::string writeToString() const;
        
        void close();
        
        bool hasNode(const std::string& node) const;
        std::string getCurrentNodeName() const;
        std::string getCurrentNodePath() const;
        
        /**
         * set current node to a node, / for root, otherwise use relative path
         * /config/window/ etc
         **/
        bool toNode(const std::string& node);
        
        /**
         * set current node to first node child
         **/
        bool toFirstChild(const std::string& name=std::string());
        /**
         * set current node to next slibling child
         **/
        bool toNextChild(const std::string& name=std::string());
        /**
         * set current node to node parent
         **/
        bool toParent();
        
        
        bool hasAttribute(const std::string& attr) const;
        /**
         * get all attributes of current node
         **/
        std::vector<std::string> getAttributes() const;
        
        /**
         * get node attribute as string
         */
        std::string getString(const std::string& attr) const;
        /**
         * get node attribute as bool
         */
        std::wstring getWString(const std::string& attr) const;
        /**
         * get node attribute as bool
         */
        bool getBool(const std::string& attr) const;
        /**
         * get node attribute as bool
         */
        int32 getInt(const std::string& attr) const;
        /**
         * get node attribute as bool
         */
        float getFloat(const std::string& attr) const;
        
        /**
         * get node attribute as optinal string
         */
        std::string getString(const std::string& attr, const std::string& opt) const;
        /**
         * get node attribute as optinal bool
         */
        std::wstring getWString(const std::string& attr, const std::wstring& opt) const;
        /**
         * get node attribute as optinal bool
         */
        bool getBool(const std::string& attr, bool opt) const;
        /**
         * get node attribute as optinal bool
         */
        int32 getInt(const std::string& attr, int32 opt) const;
        /**
         * get node attribute as optinal bool
         */
        float getFloat(const std::string& attr, float opt) const;
        
        
        /**
         * Begin a new node under current node
         **/
        bool beginNode(const std::string& nodeName);
        void endNode();
        
        /**
         * set the value of current node
         **/
        void setValue(const std::string& val);
        /**
         * set a string attribute under current node
         **/
        void setString(const std::string& attr, const std::string& val);
        /**
         * set a bool attribute under current node
         **/
        void setBool(const std::string& attr, bool val);
        /**
         * set a int attribute under current node
         **/
        void setInt(const std::string& attr, int32 val);
        /**
         * set a long attribute under current node
         **/
        void setLong(const std::string& attr, uint32 val);
        /**
         * set a float attribute under current node
         **/
        void setFloat(const std::string& attr, float val);
        
    private:
        ConfigType fileNameToConfigType(const StringType& path);
        ConfigType fileDataToConfigType(void* data, uint32 size);
        
        SoraConfigParserImpl* mImpl;
    };
    
    
} // namespace sora



#endif
