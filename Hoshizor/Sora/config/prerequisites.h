//
//  prerequisites.h
//  Sora
//
//  Created by Ruiwei Bu on 9/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_prerequisites_h
#define Sora_prerequisites_h

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraMemoryBuffer.h"
#include "SoraString.h"

namespace sora {
    
    enum ConfigType {
        // reserved, DO NOT USE
        CONFIG_UNKNOWN = -1,
        
        CONFIG_XML = 1,
        CONFIG_JSON,
        CONFIG_INI,
        
        CONFIG_AUTO_DETECT = 10,
    };
    
} // namespace sora



#endif
