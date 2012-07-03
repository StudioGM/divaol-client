/*
 *  SoraBooter.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_BOOTER_H_
#define SORA_BOOTER_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraString.h"

namespace sora {
	
	class SORA_API SoraBooter {
	public:
		static void loadExPlugins(const StringType& bootPath);
	};
	
} // namespace sora

#endif // SORA_BOOTER_H_