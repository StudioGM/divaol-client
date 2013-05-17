#pragma once

#include "SoraCore.h"
#include "SoraTask.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraShader.h"

#include "ui/GUIChanEx/GUIChanEx.h"
#include "SoraSoundManager/SoraBGMManager.h"
#include "Lib/wjson/wjson.h"
#include "timer/DivaTimeCounterImplSora.h"

#include "TitleScreenWheel.h"
#include "TitleScreenExpander.h"
#include "TitleScreenPressAnyKey.h"
#include "TitleScreenMovingShader.h"

#include "SoraLuaObject.h"
#include <string>
#include <vector>

namespace diva
{

	class TitleScreenUI
	{
	private:
		static TitleScreenUI* mainPtr;
		TitleScreenUI();
		
		std::wstring style;
		TitleScreenWheel* inner;
		TitleScreenWheel* outer;
		WJson::Value conf;
		gcn::Container* top;
		gcn::ContainerEx* circle01;
		gcn::ContainerEx* circle04;
		gcn::ContainerEx* circle02;
		gcn::ContainerEx* bigCircle;

		std::vector<gcn::AnimeEx*> animes;

		TitleScreenWheel* CreateWheel(const std::wstring& name);
		gcn::ContainerEx* CreateStaticImage(const std::wstring& name);
		TitleScreenPressAnyKey* CreatePressAnyKey(const std::wstring& name);
		gcn::ContainerEx* CreateMovingShader(const std::wstring& name);
		TitleScreenExpander* CreateExpander(const std::wstring& name);
		std::wstring& GetRandomStyle();
		void ParseJson(const std::wstring& filename);

	public:
		virtual ~TitleScreenUI();

		static void Release();
		static TitleScreenUI* Instance() {
			if (mainPtr == NULL)
				mainPtr = new TitleScreenUI;
			return mainPtr;
		}

		void Render();
		void Update(float dt);

		void Enter();
		void Leave();
		void BeginLeave();
	};
}