//
//  SoraConfigManager.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraConfigManager.h"
#include "SoraConfigParser.h"
#include "SoraPath.h"
#include "SoraDirectoryIterator.h"
#include "SoraCommon.h"

namespace sora {
    
    SoraConfigManager::SoraConfigManager() {
        
    }
    
    SoraConfigManager::~SoraConfigManager() {
        for_each(mConfigs.begin(), mConfigs.end(), DeleteSTLPairPtr());
        mConfigs.clear();
    }
    
    SoraConfigParser* SoraConfigManager::addConfig(const StringType& path, ConfigType type, const StringType& tag) {
       
        SoraConfigParser* parser = new SoraConfigParser;
        sora_assert(parser);
        
        if(parser->open(path, type)) {
            if(tag.empty()) {
                mConfigs.insert(std::make_pair(SoraPath(path.get()).getFileName(), parser));
            } else {
                mConfigs.insert(std::make_pair(tag, parser));
            }
            return parser;
        } else {
            delete parser;
        }
        return 0;
    }
    
    SoraConfigParser* SoraConfigManager::getConfig(const StringType& tag) const {
        ConfigMap::const_iterator it = mConfigs.find(tag);
        if(it != mConfigs.end()) {
            return it->second;
        }
        return 0;
    }
    
    void SoraConfigManager::loadFolder(const StringType& folder) {
        SoraDirectoryIterator iterator(folder.get());
        while(!iterator.isEnd()) {
            addConfig(iterator.path().toString(), CONFIG_AUTO_DETECT);
            
            ++iterator;
        }
    }
    
    void SoraConfigManager::removeConfig(const StringType& tag) {
        ConfigMap::iterator it = mConfigs.find(tag);
        if(it != mConfigs.end()) {
            mConfigs.erase(it);
        }
    }
    
    
} // namespace sora