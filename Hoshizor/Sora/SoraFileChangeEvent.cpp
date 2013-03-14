/*
 *  SoraFileChangeEvent.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraFileChangeEvent.h"
#include "SoraEventManager.h"
#include "SoraLogger.h"
#include "function/SoraBind.h"
#include "support/md5lib.h"

namespace sora {
	
	SoraFileChangeEventPublisher::SoraFileChangeEventPublisher(): mCheckInternval(2.f) {		
        mTimer = CreateSimpleTimer(Bind(this, &SoraFileChangeEventPublisher::onTimer));
        mTimer->start(mCheckInternval, 0, 0.f);
    }
	
	void SoraFileChangeEventPublisher::setInterval(float interval) {
		mCheckInternval = interval;
        
        mTimer->start(mCheckInternval, 0, 0.f);
	}
	
	void SoraFileChangeEventPublisher::addEventHandler(const StringType& file, SoraEventHandler* handler) {	
		FileChangeMap::iterator itFile = mChangeListeners.find(file);
		if(itFile != mChangeListeners.end()) {
			mChangeListeners[file].mHandlers.push_back(handler);
		} else {
			FileChangeInfo info;
			FILE* fl = fopen(file.c_str(), "rb");
			if(fl) {
				MD5 md5(fl);
				FileChangeInfo info;
				info.mMD5 = md5.hex_digest();
				info.mHandlers.push_back(handler);
				
				mChangeListeners.insert(std::make_pair(file, info));
                
                fclose(fl);
			} else {
				log_error(vamssg("unable to open file %s", file.c_str()));
			}
		}
	}
	
	void SoraFileChangeEventPublisher::delEventHandler(SoraEventHandler* handler) {
		FileChangeMap::iterator itFile = mChangeListeners.begin();
		while(itFile != mChangeListeners.end()) {
			FileChangeInfo::ChangeHandlers::iterator itHandler = itFile->second.mHandlers.begin();
			while(itHandler != itFile->second.mHandlers.end()) {
				if((*itHandler) == handler) {
					itFile->second.mHandlers.erase(itHandler);
					break;
				}
				++itHandler;
			}
			++itFile;
		}
	}
	
    bool SoraFileChangeEventPublisher::onTimer(SoraSimpleTimer* timer, float dt) {
		FileChangeMap::iterator itFile = mChangeListeners.begin();
		while(itFile != mChangeListeners.end()) {
			FILE* file = fopen(itFile->first.c_str(), "rb");
			if(file) {
				MD5 nowmd5(file);
				if(nowmd5.hex_digest() != itFile->second.mMD5) {
					SoraFileChangeEvent event(itFile->first);
					
					FileChangeInfo::ChangeHandlers::iterator itHandler = itFile->second.mHandlers.begin();
					while(itHandler != itFile->second.mHandlers.end()) {
						(*itHandler)->handleEvent(&event);
						++itHandler;
                        
                        if(event.isConsumed())
                            break;
					}
					
					itFile->second.mMD5 = nowmd5.hex_digest();
				}
                fclose(file);
			}
            
			++itFile;
		}
        return true;
	}
	
	
} // namespace sora

