/*
 *  DivaSettings.h
 *
 *  Created by Hyf042 on 1/18/13.
 *  Copyright 2013 Hyf042. All rights reserved.
 *
 *  Implement all the settings
 */

#ifndef DIVA_SETTINGS
#define DIVA_SETTINGS

#include "Core/DivaCommon.h"
#include "SoraSprite.h"
#include "Core/DivaCore.h"
#include "Lib/Base/Base.h"
#include "Lib/WJson/wjson.h"

namespace divacore
{
	class Settings : public Base::Singleton<Settings>
	{
		WJson::Value mConfig;
	private:
		friend class Base::Singleton<Settings>;

		Settings() {}
		~Settings() {}

	public:
		void RefreshAll(WJson::Value config)
		{
			mConfig = config;
		}
	};
}

#endif