/*
 *  PluginConcept.cpp
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraPluginManager.h"
#include "SoraPlugin.h"

namespace sora {

    SoraPluginManager::~SoraPluginManager() {
        PluginMap::iterator itPlugin = mPluginMap.begin();
        while(itPlugin != mPluginMap.end()) {
            SoraPlugin* plugin = itPlugin->second;
            
            plugin->shutdown();
            plugin->unistall();
            
            ++itPlugin;
        }
        mPluginMap.clear();
    }
    
    void SoraPluginManager::registerPlugin(SoraPlugin* plugin) {
        sora_assert(plugin);
        sora_assert(!plugin->getName().empty());
        
        plugin->install();
        mPluginMap.insert(std::make_pair(plugin->getName(), plugin));
        
        plugin->initialise();
    }
    
    SoraPlugin* SoraPluginManager::unistallPlugin(const std::string& name) {
        PluginMap::iterator itPlugin = mPluginMap.find(name);
        if(itPlugin != mPluginMap.end()) {
            SoraPlugin* plugin = itPlugin->second;
            
            plugin->shutdown();
            plugin->unistall();
                
            mPluginMap.erase(itPlugin);
            return plugin;
        }
        return 0;
    }
    
    SoraPlugin* SoraPluginManager::unistallPlugin(SoraPlugin* plugin) {
        return unistallPlugin(plugin->getName());
    }
    
    SoraPlugin* SoraPluginManager::getPlugin(const std::string& name) const {
        PluginMap::const_iterator itPlugin = mPluginMap.find(name);
        if(itPlugin != mPluginMap.end())
            return itPlugin->second;
        return 0;
    }
    
    SoraPluginManager::FuncMap SoraPluginManager::mFuncs;
    
    void SoraPluginManager::RegisterFunc(const std::string& funcName, void* func) {
        mFuncs.insert(std::make_pair(funcName, func));
    }
    
    void* SoraPluginManager::GetFunc(const std::string& name) {
        FuncMap::const_iterator it = mFuncs.find(name);
        if(it != mFuncs.end())
            return it->second;
        return 0;
    }

} // namespace sora