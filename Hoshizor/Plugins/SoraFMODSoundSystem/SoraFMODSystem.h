//
//  SoraFMODSystem.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_FMOD_SYSTEM_H_
#define SORA_FMOD_SYSTEM_H_ 

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"

#include "SoraSingleton.h"
#include "SoraLogger.h"

#include "SoraPlatform.h"
#ifdef OS_WIN32
#pragma comment(lib, "fmodex_vc.lib")
#endif

namespace sora {
    
    static bool FMOD_ERROR_CHECK(FMOD_RESULT result) {
        if(result != FMOD_OK) {
            log_mssg(vamssg("FMOD ERROR: error (%d) %s\n", result, FMOD_ErrorString(result)),
						  LOG_LEVEL_ERROR);
            return false;
        }
        return true;
    }

        
    class SoraFMODSystem: public SoraSingleton<SoraFMODSystem> {
        friend class SoraSingleton<SoraFMODSystem>;
        
    protected:
        SoraFMODSystem(): pSystem(NULL) {
        }
        
        ~SoraFMODSystem() {
            if(pSystem) {
                pSystem->close();
            }
        }
        
    public:
        FMOD::System* getSystem() const {
            if(!pSystem)
                THROW_SORA_EXCEPTION(NullPointerException, "No FMODSystem available");
            return pSystem;
        }
        
        bool init(int maxchannel, unsigned int initFlags, char*externalArgs) {
            if(pSystem)
                THROW_SORA_EXCEPTION(ExistsException, "FMOD System already exists");
            else {
                FMOD_RESULT result = FMOD::System_Create(&pSystem);
                if(FMOD_ERROR_CHECK(result)) {
                    result = pSystem->init(maxchannel, initFlags, externalArgs);
                    return FMOD_ERROR_CHECK(result);
                }
            }
            return false;
        }
        
        void update() {
            if(pSystem)
                pSystem->update();
        }
        
    private:
        FMOD::System* pSystem;
    };
    
    
} // namespace sora


#endif