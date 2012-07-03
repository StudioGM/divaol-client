//
//  SoraConfigManager.h
//  Sora
//
//  Created by Ruiwei Bu on 9/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraConfigManager_h
#define Sora_SoraConfigManager_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"

#include "prerequisites.h"
#include <map>

namespace sora {
    
    class SoraConfigParser;
    
    /**
     * A simple container class for store/get config parsers
     */

    class SoraConfigManager: SoraUncopyable {
    public:
        SoraConfigManager();
        ~SoraConfigManager();
        
        SoraConfigParser* addConfig(const StringType& path, ConfigType type, const StringType& tag=StringType());
        SoraConfigParser* getConfig(const StringType& tag) const;
        
        /**
         * Tags would be config file name
         **/
        void loadFolder(const StringType& folder);
        
        void removeConfig(const StringType& tag);
        
    private:
        typedef std::map<StringType /*tag*/, SoraConfigParser*> ConfigMap;
        ConfigMap mConfigs;
    };
    
} // namespace sora


#endif
