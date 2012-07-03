//
//  SoraConstantStrings.h
//  Sora
//
//  Created by Griffin Bu on 4/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_CONSTANT_STRINGS_H_
#define SORA_CONSTANT_STRINGS_H_

#include "SoraStringId.h"

namespace sora {
	/*
	long　num=100L;
	long　long　num=100LL;
	unsigned　long=100UL;
	unsigned　long　long　num=100ULL;
	*/
    
    // GetUniqueStringId("xxx") constants
    static const char* STR_ID_PLAY_STARTED      = "PlayStarted";
    static const char* STR_ID_PLAY_ENDED        = "PlayEnded";
    static const char* STR_ID_PLAY_PAUSED       = "PlayPaused";
    static const char* STR_ID_PLAY_RESUMED      = "PlayResumed";
    static const char* STR_ID_PLAY_STOPPED      = "PlayStopped";
    
} // namespace sora

#endif