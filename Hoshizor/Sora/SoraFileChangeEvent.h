/*
 *  SoraFileChangeEvent.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_FILE_CHANGE_EVENT_H_
#define SORA_FILE_CHANGE_EVENT_H_

#include "SoraEvent.h"
#include "SoraTimerEvent.h"
#include "SoraString.h"
#include "timer/SoraSimpleTimerManager.h"

#include <list>

namespace sora {
		
	class SORA_API SoraFileChangeEvent: public SoraEvent {
	public:
		SoraFileChangeEvent() {}
		SoraFileChangeEvent(const StringType& file): mChangedFile(file) {}
		
		void setChangedFile(const StringType& file) {
			mChangedFile = file;
		}
		
		StringType getChangedFile() const {
			return mChangedFile;
		}
		
		SORA_EVENT_IDENTIFIER(18446744073686077189ULL);
	
	private:
		StringType mChangedFile;
	};
	
	class SORA_API SoraFileChangeEventPublisher: public SoraEventHandler {
	public:
		SoraFileChangeEventPublisher();
		
		void setInterval(float interval);
		void addEventHandler(const StringType& file, SoraEventHandler* handler);
		void delEventHandler(SoraEventHandler* handler);
		bool onTimer(SoraSimpleTimer*, float);
		
	private:

		struct FileChangeInfo {		
            std::string mMD5;
			typedef std::list<SoraEventHandler*> ChangeHandlers;
			ChangeHandlers mHandlers;
		};
		
		typedef std::map<StringType, FileChangeInfo> FileChangeMap;
		FileChangeMap mChangeListeners;
		
		float mCheckInternval;
        SimpleTimerPtr mTimer;
	};
	
	
} // namespace sora


#endif // SORA_FILE_CHANGE_EVENT_H_