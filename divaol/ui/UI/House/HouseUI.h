#pragma once

#include "SoraCore.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraShader.h"

#include "SoraGUI.h"

#include "ui/GUIChanEx/GUIChanEx.h"
#include "SoraSoundManager/SoraBGMManager.h"
#include "Lib/wjson/wjson.h"
#include "timer/DivaTimeCounterImplSora.h"

#include "ui/GUIChanEx/WTextfield.h"
#include "Animation/SoraGUIAnimation.h"

#include "HouseMessageBox.h"
#include "MessageChannelList.h"
#include "RoomInfoList.h"
#include "ThingList.h"
#include "RoomList.h"
#include "divasongmgr/DivaMapManager.h"

#include "SoraLuaObject.h"
#include <string>
#include <vector>

#include "divanetwork/DivaNetworkManager.h"
#include "SettingDisplayers.h"

namespace diva
{

	namespace HouseUI
	{
		class HouseGameState;

		class HouseUI : 
			public sora::SoraGUIResponser
		{
		public:
			enum {STATE_OFFLINE, STATE_LOGINWINDOW, STATE_LOGINING, STATE_LOGINFAILED, STATE_ROOM, STATE_STAGE, STATE_ROOMLIST, STATE_PLAYING};

		private:
			HouseUI();

			void ParseJson(const std::wstring& filename, const std::wstring& stage, const std::wstring& room);

			gcn::WindowEx* CreateMyWindow(const WJson::Value& conf, const std::wstring& name);
			gcn::ContainerEx* CreateStaticImage(const WJson::Value& conf, const std::wstring& name);
			gcn::SuperButtonEx* CreateButton(const WJson::Value& conf, const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			//gcn::SuperButtonEx* _CreateButton(const std::wstring& normal, const std::wstring& on, const std::wstring& down, const std::wstring& disable);
			gcn::WindowEx* CreateLoginWindow(const WJson::Value& conf, const std::wstring& prefix);
			gcn::WTextField* CreateInput(const WJson::Value& conf, const std::wstring& name);
			gcn::LabelEx* CreateLabel(const WJson::Value& conf, const std::wstring& name);
			gcn::ContainerEx* CreateStatusPanel(const WJson::Value& conf);
			gcn::ContainerEx* CreatePlayerListPanel(const WJson::Value& conf);
			gcn::ContainerEx* CreateMessagePanel(const WJson::Value& conf);
			MessageSlider* CreateMessageSlider(const WJson::Value& conf);
			ThingList* CreateThingList(const WJson::Value& conf);
			gcn::ContainerEx* CreateTeamList(const WJson::Value& conf);
			gcn::WindowEx* CreateRoomListWindow(const WJson::Value conf);
			RoomListItem* SetRoomListItemInfo(const WJson::Value conf, const std::wstring& normal, const std::wstring& on, const std::wstring& down);
			gcn::MarkerEx* CreateMarker(const WJson::Value conf, const std::wstring& up1, const std::wstring& up2, const std::wstring &up3, 
				 const std::wstring& down1, const std::wstring& down2, const std::wstring &down3,
				  const std::wstring& mid1, const std::wstring& mid2, const std::wstring &mid3);
			gcn::ListBoxEx* CreateStageList(const WJson::Value& conf);
			gcn::ContainerEx* CreateSongList(const WJson::Value& conf);
			gcn::WindowEx* CreateModeWindow(const WJson::Value& conf);
			gcn::WindowEx* CreateSettingWindow();
			gcn::SelectorEx* CreateSelector(const WJson::Value& sconf, const WJson::Value& dconf, const WJson::Value& sizeConf);

			void RefreshStatus();
			void Refresh_sPlayerList(bool netRefresh = true);
			void Refresh_hostInfo();
			void Refresh_SongList();
			void BeginSongListAnimation();
			void EndSongListAnimation(gcn::Widget*);
			void Refresh_RoomList(bool updateRoomInfo = true);

			void setState(int des);
			void StateChange_ROOM_STAGE();
			void StateChange_STAGE_ROOM();
			void StateChange_LOGINWINDOW_ROOM();
			void StateChange_ROOM_ROOMLIST();
			void StateChange_ROOMLIST_ROOM();
			void StateChange_ROOMLIST_STAGE();
			void StateChange_STAGE_PLAYING();
			void StateChange_PLAYING_STAGE();
			void StateChange_PLAYING_ROOM();

			void setSongListImage(int v);

			//-------network---------------------------------------------------
			bool connectServer();
			void disconnectServer();
			void request_roomList();
			void start_game();
			void open_stage();
			void leave_stage();
			void cb_connect_break();
			void login();
			void attachObserver();
			void detachObserver();
			void observer_auth(divanet::Notification);
			void observer_chat(divanet::Notification);
			void observer_scheduler(divanet::Notification);
			void observer_stage(divanet::Notification);
			//void gnet_login(divanet::GPacket *packet);
			std::string roomId;

			enum {CHANNEL_WORLD, CHANNEL_PRIVATE, CHANNEL_STAGE};
			int msgChannelState;
			int msgSendId;
			bool isSongListAnimating;
			//-----------------------------------------------------------------

			void MessageChannelChange(int ch, int id = -1, const std::wstring &text = L"unknown");

			//////////////////////////////////////////////////////////////////////////
			int state;
			HouseGameState* houseGameState;
			Base::Thread<void()> mAsyncTask;

			gcn::Container* top;
			WJson::Value conf;
			WJson::Value sconf;
			WJson::Value rconf;
			
			

			// private
			// --------- Room
			gcn::SuperButtonEx* shopButton;
			gcn::SuperButtonEx* stageButton;
			gcn::SuperButtonEx* optionButton;
			gcn::SuperButtonEx* modifyButton;
			gcn::SuperButtonEx* clothesButton;
			gcn::SuperButtonEx* exitButton;
			gcn::WindowEx* roomTop;
			gcn::ContainerEx* statusPanel;
			gcn::LabelEx* hostInfoLabel;
			gcn::SuperButtonEx* udButton;
			gcn::SuperButtonEx* settingsButton;
			gcn::SuperButtonEx* selectMusicButton;
			gcn::SuperButtonEx* decorateButton;
			gcn::SuperButtonEx* exitStageButton;
			gcn::ButtonEx* openGameButton;
			gcn::ButtonEx* readyButton;
			gcn::ContainerEx* messagePanel;
			gcn::TextFieldEx* messagePanelInputBox;
			gcn::TextBoxEx* messagePanelChatBox;
			gcn::ListBoxEx* messageChannelList;
			gcn::ContainerEx* messageChannel;
			gcn::ContainerEx* messageToSomeOne;
			ThingList* thingList;
			gcn::ContainerEx* teamList;
			std::vector<gcn::SuperButtonEx*> teamListButtons;
			gcn::ListBoxEx* stageList;
			gcn::ContainerEx* songListPanel;
			gcn::ListBoxEx* songList;
			gcn::SuperButtonEx* playerListButton1;
			gcn::SuperButtonEx* playerListButton2;
			int playerListNowPage;
			gcn::ContainerEx* playerListPage;
			RoomListSlider* roomListSlider;
			gcn::ContainerEx* songListImage;
			MessageSlider* messageSlider;
			gcn::ButtonEx* modeConfirmButton;
			gcn::ButtonEx* modeButton;
			gcn::WindowEx* modeWindow;
			std::vector<SuperButtonEx*> modeButtonList;
			gcn::AnimeBoxEx* testAnimeBox;
			gcn::SuperButtonEx* settingConfirmBtn;
			gcn::SuperButtonEx* settingCancelBtn;
			gcn::ContainerEx* playingHint;

			gcn::WindowEx* settingWindow;

			gcn::ListBoxEx* avatarList;
			WJson::Value avatarListInfo;
			WJson::Value songListInfo;

			gcn::ContainerEx* sPlayerListPanel;
			gcn::ListBoxEx* sPlayerList;

			// -----RoomList
			gcn::WindowEx* roomListPanel;
			gcn::SuperButtonEx* roomListCloseButton;
			gcn::SuperButtonEx* roomListOpenButton;
			RoomList* roomListView;

			// --------- Login
			gcn::WindowEx* loginPanel;
			gcn::SuperButtonEx* loginButton;
			gcn::WTextField* usernameInput;
			gcn::WTextField* passwordInput;

			// --------- font
			gcn::Font* playerListFont;
			gcn::Font* statusPanelFont;
			gcn::Font* messageAreaFont;
			gcn::Font* messageInputFont;
			gcn::Font* playerListPagenumFont;
			gcn::Font* songListFont;

			void RecvMsg();

		public:
			gcn::WindowMgr* mgr;

			~HouseUI();

			static HouseUI* Instance() {static HouseUI i; return &i;}

			void Render();
			void Update(float dt);

			void Enter();
			void Leave();
			void BeginLeave();
			void SetFatherState(HouseGameState* state);
			int getState() {return state;}
			void StartOfflineGame();

			// Event
			void LoginButtonClicked();
			void MouseClicked(gcn::MouseEvent& mouseEvent);
			void MessagePanelInputBoxEnterPressed();
			void MessagePanelChannelClicked();
			void MessagePanelChannelListClicked(int index);
			void RoomLInfoListClicked(int index);
			void TeamListClicked(gcn::MouseEvent& mouseEvent);
			void SetWidgetInvisible(gcn::Widget* widget);
			void RoomListSliderSlided(int v);
			void RoomListFirstPageChanged(int v);
			void MessageSliderSlided(int v);
			void MessageBoxFirstIndexChanged(int v);
			void MessageBoxItemChanged();


			virtual void action();
			virtual void mouseClicked(const gcn::MouseEvent& mouseEvent);
			void ModeButtonClicked(int index);
			void ModeButtonRefresh();
			
		};

		class LoginButton_MouseListener : public gcn::MouseListener
		{
		public:
			void mouseClicked(gcn::MouseEvent& mouseEvent);
		};

		class TeamSelect_MouseListener : public gcn::MouseListener
		{
		public:
			void mouseClicked(gcn::MouseEvent& mouseEvent);
		};

		class Mode_MouseListener : public gcn::MouseListener
		{
		public:
			void mouseClicked(gcn::MouseEvent& mouseEvent);
		};

		class NotSupportInAlpha_MouseListener : public gcn::MouseListener
		{
		public:
			void mouseClicked(gcn::MouseEvent& mouseEvent);
		};
	}
}