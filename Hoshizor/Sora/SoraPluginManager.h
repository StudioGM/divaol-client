/*
 *  PluginConcept.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_MANAGER_H
#define SORA_PLUGIN_MANAGER_H

#include "SoraPlatform.h"
#include <map>

namespace sora {
    
    class SoraPlugin;

    class SORA_API SoraPluginManager {
    private:
        typedef std::map<std::string, SoraPlugin*> PluginMap;
        
    public:
        SoraPluginManager() {}
        virtual ~SoraPluginManager();
        
        virtual void registerPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* unistallPlugin(const std::string& name);
        virtual SoraPlugin* unistallPlugin(SoraPlugin* plugin);
        
        virtual SoraPlugin* getPlugin(const std::string& pluginname) const;
        
        static void RegisterFunc(const std::string& funcName, void* func);
        static void* GetFunc(const std::string& funcName);
                
    private:
        typedef std::map<std::string, void*> FuncMap;
        static FuncMap mFuncs;
        
        PluginMap mPluginMap;
    };
    
#define SORA_REGISTER_PLUGIN_FUNC(name, func) \
    ::sora::SoraPluginManager::RegisterFunc(name, func);
    
#define SORA_GET_PLUGIN_FUNC(name) \
    ::sora::SoraPluginManager::GetFunc(name);

} // namespace sora

#endif // SORA_PLUGIN_MANAGER_H