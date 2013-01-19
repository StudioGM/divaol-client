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
	private:
		friend class Base::Singleton<Settings>;

		Settings() {}
		~Settings() {}

	public:
		int getWindowWidth() const {return windowWidth;}
		int getWindowheight() const {return windowHeight;}
		float getBGMVolume() const {return bgmVolume;}
		float getSEVolume() const {return seVolume;}

		void InitializeSettings(WJson::Value &config, WJson::Value &setting)
		{
			_ReloadWindowSize(config, setting);
			_RestoreSettings(config, setting);
			_ResetVolume(config, setting);
			_ResetEffect(config, setting);
		}
		void RefreshAll(WJson::Value &config, WJson::Value &setting)
		{
			_ReloadWindowSize(config, setting);
			_RestoreSettings(config, setting);
			sora::SoraCore::Ptr->setWindowSize(windowWidth, windowHeight);
			sora::SoraCore::Ptr->setFullscreen(!isWindowMode);
			_ResetVolume(config, setting);
			_ResetEffect(config, setting);
		}

	private:
		void _ReloadWindowSize(WJson::Value &config, WJson::Value &setting)
		{
			// if fullscreen, set window size to global size
			if(!config[L"isWindowMode"].asBool())
			{
#ifdef OS_WIN32
				config[L"windowWidth"] = GetSystemMetrics(SM_CXSCREEN);
				config[L"windowHeight"] = GetSystemMetrics(SM_CYSCREEN);
#else
				config[L"windowWidth"] = sora::SoraCore::Ptr->getScreenWidth();
				config[L"windowHeight"] = sora::SoraCore::Ptr->getScreenHeight();
#endif
			}
			else
			{
				int resolution = config[L"resolution"].asInt();
				WJson::Value::iterator ptr = setting[L"resolutions_pair"].begin();
				for (int i = 0; i < resolution && ptr != setting[L"resolutions_pair"].end(); i++, ptr++);
				if(ptr == setting[L"resolutions_pair"].end())
				{
					config[L"windowWidth"] = 1280;
					config[L"windowHeight"]  =720;
				}
				else
				{
					config[L"windowWidth"] = (*ptr)[L"width"].asInt();
					config[L"windowHeight"] = (*ptr)[L"height"].asInt();
				}
			}
		}

		void _ResetVolume(WJson::Value &config, WJson::Value &setting)
		{
			MUSIC_MANAGER_PTR->setTagVolume("bgm", bgmVolume);
			MUSIC_MANAGER_PTR->setTagVolume("se", seVolume);
			sora::SoraBGMManager::Instance()->setVolume(bgmVolume);
		}

		void _ResetEffect(WJson::Value &config, WJson::Value &setting)
		{
			EFFECT_SYSTEM_PTR->setEffectLevel(particleLevel);
		}

		void _RestoreSettings(WJson::Value &config, WJson::Value &setting)
		{
			windowWidth = config[L"windowWidth"].asInt();
			windowHeight = config[L"windowHeight"].asInt();
			bgmVolume = config[L"musicVolume"].asInt() / 10.0;
			seVolume = config[L"seVolume"].asInt() / 10.0;
			isWindowMode = config[L"isWindowMode"].asBool();
			particleLevel = config[L"particleSystem"].asInt();
		}

	protected:
		int windowWidth;
		int windowHeight;
		float bgmVolume;
		float seVolume;
		bool isWindowMode;
		int particleLevel;
	};
}

#endif