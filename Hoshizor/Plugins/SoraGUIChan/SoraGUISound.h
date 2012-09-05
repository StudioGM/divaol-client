/*
 *  SoraSound.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_SOUND_H_
#define SORA_GCN_SOUND_H_

#include "guichan.hpp"
#include "SoraSoundFile.h"

namespace gcn {
	
	class SoraGUISound: public Sound {
		friend class SoraGUISoundLoader;
		
		SoraGUISound(const std::string& name);
		SoraGUISound();
		
	public:
		virtual ~SoraGUISound();
		
		void play();
		void stop();
		
	private:
		sora::SoraSoundEffectFile* pfile;
	};
	
} // namespace sora

#endif // SORA_SOUND_H_