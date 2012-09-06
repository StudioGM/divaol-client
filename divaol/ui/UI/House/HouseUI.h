#pragma once

#include "SoraCore.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraShader.h"

#include "SoraGUI.h"

#include "ui/GUIChanEx/GUIChanEx.h"
#include "SoraSoundManager/SoraBGMManager.h"
#include "WJson/wjson.h"
#include "timer/DivaTimeCounterImplSora.h"

#include "ui/GUIChanEx/WTextfield.h"
#include "Animation/SoraGUIAnimation.h"

#include "HouseMessageBox.h"
#include "MessageChannelList.h"
#include "RoomInfoList.h"

#include "SoraLuaObject.h"
#include <string>
#include <vector>

namespace diva
{

	namespace HouseUI
	{

		class HouseUI : 
			public sora::SoraGUIResponser
		{
		private:
			enum {STATE_OFFLINE, STATE_LOGINWINDOW, STATE_LOGINING, STATE_LOGINFAILED, STATE_ROOM, STATE_STAGE};

			HouseUI();

			void ParseJson(const std::wstring& filename, const std::wstring& stage);

			gcn::ContainerEx* CreateStaticImage(const WJson::Value& conf, const std::wstring& name);
			gcn::SuperButtonEx* CreateButton(const WJson::Value& conf, const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			//gcn::SuperButtonEx* _CreateButton(const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			gcn::ContainerEx* CreateLoginWindow(const WJson::Value& conf, const std::wstring& prefix);
			gcn::WTextField* CreateInput(const WJson::Value& conf, const std::wstring& name);
			gcn::LabelEx* CreateLabel(const WJson::Value& conf, const std::wstring& name);
			gcn::ContainerEx* CreateStatusPanel(const WJson::Value& conf);
			gcn::ContainerEx* CreatePlayerListPanel(const WJson::Value& conf);
			gcn::ContainerEx* CreateMessagePanel(const WJson::Value& conf);
			gcn::SliderEx* CreateMessageSlider(const WJson::Value& conf);

			void RefreshStatus();
			void Refresh_sPlayerList();
			void Refresh_hostInfo();

			void setState(int des);
			void StateChange_ROOM_STAGE();
			void StateChange_STAGE_ROOM();
			void StateChange_LOGINWINDOW_ROOM();

			int state;

			gcn::Container* top;
			WJson::Value conf;
			WJson::Value sconf;

			// private
			// --------- Room
			gcn::SuperButtonEx* shopButton;
			gcn::SuperButtonEx* stageButton;
			gcn::SuperButtonEx* optionButton;
			gcn::SuperButtonEx* modifyButton;
			gcn::SuperButtonEx* clothesButton;
			gcn::SuperButtonEx* exitButton;
			gcn::ContainerEx* roomTop;
			gcn::ContainerEx* statusPanel;
			gcn::LabelEx* hostInfoLabel;
			gcn::SuperButtonEx* udButton;
			gcn::SuperButtonEx* settingsButton;
			gcn::SuperButtonEx* selectMusicButton;
			gcn::SuperButtonEx* decorateButton;
			gcn::SuperButtonEx* exitStageButton;
			gcn::ContainerEx* messagePanel;
			gcn::TextFieldEx* messagePanelInputBox;
			gcn::TextBoxEx* messagePanelChatBox;
			gcn::ListBoxEx* messageChannelList;
			gcn::ContainerEx* messageChannel;
			gcn::ContainerEx* messageToSomeOne;

			gcn::Container* sPlayerListPanel;
			gcn::ListBoxEx* sPlayerList;

			// --------- Login
			gcn::ContainerEx* loginPanel;
			gcn::SuperButtonEx* loginButton;
			gcn::WTextField* usernameInput;
			gcn::WTextField* passwordInput;

			// --------- font
			gcn::Font* playerListFont;
			gcn::Font* statusPanelFont;
			gcn::Font* messageAreaFont;
			gcn::Font* messageInputFont;

			void RecvMsg();

		public:
			~HouseUI();

			static HouseUI* Instance() {static HouseUI i; return &i;}

			void Render();
			void Update(float dt);

			void Enter();
			void Leave();

			// Event
			void LoginButtonClicked();
			void MouseClicked(gcn::MouseEvent& mouseEvent);
			void MessagePanelInputBoxEnterPressed();
			void MessagePanelChannelClicked();
			void MessagePanelChannelListClicked(int index);
			void RoomLInfoListClicked(int index);

			void SetMessageChannelListInvisible(gcn::Widget* widget);

			virtual void action();
			virtual void mouseClicked(const gcn::MouseEvent& mouseEvent);
		};

		class LoginButton_MouseListener : public gcn::MouseListener
		{
		public:
			void mouseClicked(gcn::MouseEvent& mouseEvent);
		};
	}
}