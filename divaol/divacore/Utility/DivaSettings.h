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
#include "SoraBGMManager.h"

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
		int getGameWidth() const {return gameWidth;}
		int getGameHeight() const {return gameHeight;}
		int getScreenWidth() const {return screenWidth;}
		int getScreenHeight() const {return screenHeight;}
		float getBGMVolume() const {return bgmVolume;}
		float getSEVolume() const {return seVolume;}
		bool getIsAutoCT() const {return isAutoCT;}
		const Base::String& getUserModule() const {return userModule;}
		const Base::String& getGlobalFontName() const {return globalFontName;}

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
		void DefaultSettings(WJson::Value &config)
		{
			config[L"gameHeight"] = 1080;
			config[L"gameWidth"] = 1920;
			config[L"globalFont"] = L"res/msyh.ttf";
			config[L"isWindowMode"] = false;
			config[L"uiMod"] = 0;
			config[L"language"] = 0;
			config[L"musicVolume"] = 10;
			config[L"seVolume"] = 6;
			config[L"particleSystem"] = 2;
			config[L"resolution"] = 2;
			config[L"screenFadeTime"] = 0.5;
			config[L"bgmFadeTime"] = 1.0;
			config[L"isAutoCT"] = true;
		}
		void RefreshMusicSettings()
		{
			MUSIC_MANAGER_PTR->setTagVolume("bgm", bgmVolume);
			MUSIC_MANAGER_PTR->setTagVolume("se", seVolume);
			sora::SoraBGMManager::Instance()->setVolume(bgmVolume);
		}

	private:
		void _ReloadWindowSize(WJson::Value &config, WJson::Value &setting)
		{
#ifdef OS_WIN32
			screenWidth = GetSystemMetrics(SM_CXSCREEN);
			screenHeight = GetSystemMetrics(SM_CYSCREEN);
#else
			screenWidth = sora::SoraCore::Ptr->getScreenWidth();
			screenHeight = sora::SoraCore::Ptr->getScreenHeight();
#endif
			// if fullscreen, set window size to global size
			if(!config[L"isWindowMode"].asBool())
			{
				config[L"windowWidth"] = screenWidth;
				config[L"windowHeight"] = screenHeight;
			}
			else
			{
				int resolution = config[L"resolution"].asInt();
				WJson::Value::iterator ptr = setting[L"resolutions_pair"].begin();
				for (int i = 0; ptr != setting[L"resolutions_pair"].end(); i++, ptr++)
					if(setting[L"resolutions_pair"][i][L"width"] > screenWidth || setting[L"resolutions_pair"][i][L"height"] > screenHeight)
					{
						resolution = i-1;
						break;
					}
					else if(i == resolution)
						break;

				if(resolution < 0 || ptr == setting[L"resolutions_pair"].end())
				{
					config[L"windowWidth"] = screenWidth;
					config[L"windowHeight"] = screenHeight;
				}
				else
				{
					config[L"windowWidth"] = setting[L"resolutions_pair"][resolution][L"width"].asInt();
					config[L"windowHeight"] = setting[L"resolutions_pair"][resolution][L"height"].asInt();
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
			gameWidth = config[L"gameWidth"].asInt();
			gameHeight = config[L"gameHeight"].asInt();
			bgmVolume = Base::Math::Util::Clamp(config[L"musicVolume"].asInt(), 0, 10) / 10.0;
			seVolume = Base::Math::Util::Clamp(config[L"seVolume"].asInt(), 0, 10) / 10.0;
			isWindowMode = config[L"isWindowMode"].asBool();
			particleLevel = Base::Math::Util::Clamp(config[L"particleSystem"].asInt(), 0, 2);
			globalFontName = config[L"globalFont"].asString();
			isAutoCT = config[L"isAutoCT"].asBool();
			int uiMod = config[L"uiMod"].asInt();
			if(setting.isMember(L"uiModFiles")&&setting[L"uiModFiles"].size() > uiMod)
				userModule = setting[L"uiModFiles"][uiMod].asString();
			else
				userModule = "";
		}

	protected:
		int windowWidth;
		int windowHeight;
		int gameWidth;
		int gameHeight;
		int screenWidth;
		int screenHeight;
		float bgmVolume;
		float seVolume;
		bool isWindowMode;
		bool isAutoCT;
		int particleLevel;
		Base::String globalFontName;
		Base::String userModule;
	};

#define SETTINGS (divacore::Settings::instance())
}

#endif