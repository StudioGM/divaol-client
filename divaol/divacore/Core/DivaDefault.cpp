/*
 *  DivaDefault.cpp
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include "DivaDefault.h"
#include "Utility/DivaConfigLoader.h"
#include "SoraFileUtility.h"

namespace divacore
{
	void DefaultInfo::setDefaultInfo()
	{
		config.add("module","standard");
		config.add("note","__note__");
		config.add("ui","__ui__");
		config.add("render","__render__");
		config.add("effect","__effect__");
		config.add("result","__result__");
		config.add("hookConfig","/hook/hookConfig.json");

		if(SoraFileUtility::FileExists(getDefaultFile()))
			configloader::loadWithJson(config,getDefaultFile());
	}
}