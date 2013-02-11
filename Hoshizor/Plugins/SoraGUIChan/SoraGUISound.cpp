/*
 *  SoraSound.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraGUISound.h"
#include "SoraCore.h"

namespace gcn {

	SoraGUISound::SoraGUISound(const std::string& name) {
		pfile = sora::SoraCore::Ptr->createSoundEffectFile(sora::s2ws(name));
	}
	
	SoraGUISound::SoraGUISound() {
		pfile = NULL;
	}
	
	SoraGUISound::~SoraGUISound() {
		if(pfile != NULL) 
			delete pfile;
	}
	
	void SoraGUISound::play() {
		if(pfile)
			pfile->play();
	}
	
	void SoraGUISound::stop() {
		if(pfile)
			pfile->stop();
	}


} // namespace gcn