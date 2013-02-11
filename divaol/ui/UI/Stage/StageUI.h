#pragma once

#include "SoraCore.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraShader.h"

#include "ui/GUIChanEx/GUIChanEx.h"
#include "SoraSoundManager/SoraBGMManager.h"
#include "Lib/wjson/wjson.h"
#include "timer/DivaTimeCounterImplSora.h"

#include "SoraLuaObject.h"
#include <string>
#include <vector>

namespace diva
{

	namespace StageUI
	{

		class StageUI
		{
		private:
			StageUI();

			void ParseJson(const std::wstring& filename);

			gcn::ContainerEx* _CreateStaticImage(const std::wstring& name);
			gcn::SuperButtonEx* CreateButton(const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			gcn::SuperButtonEx* _CreateButton(const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			//gcn::ContainerEx* CreateLoginWindow(const std::wstring& prefix);
			gcn::TextField* _CreateInput(const std::wstring& name);
			gcn::LabelEx* _CreateLabel(const std::wstring& name);



			gcn::Container* top;
			WJson::Value conf;

		public:
			~StageUI();

			static StageUI* Instance() {static StageUI i; return &i;}

			void Render();
			void Update(float dt);

			void Enter();
			void Leave();
		};

	}
}