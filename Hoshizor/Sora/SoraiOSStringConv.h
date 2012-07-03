/*
 *  SoraiOSStringConv.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */

#ifndef SORA_IOS_STRING_CONV_H_
#define SORA_IOS_STRING_CONV_H_

#include "SoraPlatform.h"

namespace sora {
	
    std::wstring iOSString2WString(const std::string& str);
	std::string iOSWString2String(const std::wstring& str);
    
} // namespace sora

#endif