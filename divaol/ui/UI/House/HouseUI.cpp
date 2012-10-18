#include "HouseUI.h"

#include <fstream>
#include "ui/Config/DivaUIConfig.h"

#include "luaclass.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

#include "ui/DivaNetwork/DivaNetwork.h"
#include "ui/Player/PlayerManager.h"
#include "SoraFontManager.h"

#include "HouseGameState.h"
#include "divacore/Core/DivaCore.h"
#include "divacore/Mode/DivaMultiplay.h"
#include "divacore/Mode/DivaPairPlay.h"
#include "divacore/Mode/DivaRelayPlay.h"
#include "HouseGameState.h"

#define SONICMISORA_MODIFYHYF

#include "divanetwork/DivaAuthClient.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace HouseUI
	{
		HouseUI::HouseUI()
		{
			attachObserver();

			//for (int i=1; i<=2; i++)
			//	MAPMGR.SelectedMap_Add(1, divamap::DivaMap::Normal);
			
			msgSendId = -1;
			

			top = new gcn::Container();
			top->setSize(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
			top->setOpaque(false);

			// initialize window mgr
			mgr = new WindowMgr(top);

			// why Enter() here? it will cause Enter invoked twice.
			//Enter();

			roomTop = new gcn::WindowEx();
			roomTop->setSize(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
			roomTop->setOpaque(false);
			//roomTop->setEnabled(false);
			roomTop->setVisible(true);
			roomTop->SetMovable(false);
			mgr->OpenWindow(roomTop);

			// lua init
			//sora::SoraLuaObject lo;
			//gge::LuaClass<HouseUI>(lo.getState(), "UI", this).
			//	def("ParseJson", &HouseUI::ParseJson).
			//	def("CreateStaticImage", &HouseUI::CreateStaticImage).
			//	def("CreateButton", &HouseUI::CreateButton).
			//	def("CreateLoginWindow", &HouseUI::CreateLoginWindow);
			//lo.doScript(L"HouseUI.lua");
			
			state = STATE_LOGINWINDOW;
			
			

			// parse json
			ParseJson(L"uiconfig/house.json", L"uiconfig/stage.json", L"uiconfig/RoomList_PlayerList.json");

			// message Box
			gcn::MessageBoxEx* mbex = new gcn::MessageBoxEx();
			{
				WJson::Reader reader;
				WJson::Value t;
				reader.parse(ReadJsonFile(L"uiconfig/house/MessageBox.json"), t);
				mbex->LoadFromJsonFile(t);
				mgr->RegisterMessageBox(mbex);
			}

			UIHelper::setGlobalFinishTime(conf[L"Time/Config"][L"globalAnimationTime"].asInt());

			// back ground
			roomTop->add(CreateStaticImage(conf, L"TestBackground"));

			// --------- room
			top->add(roomTop, 0, 0);

			songListPanel = CreateSongList(sconf);
			songListPanel->setVisible(true);
			roomTop->add(songListPanel);

			// shop button
			shopButton = CreateButton(conf, L"ToolButtons/Normal/Shop_Normal", L"ToolButtons/MouseOn/Shop_MouseOn", L"ToolButtons/MouseDown/Shop_MouseDown", L"ToolButtons/Normal/Shop_Normal");
			roomTop->add(shopButton);

			// stage button
			stageButton = CreateButton(conf, L"ToolButtons/Normal/Stage_Normal", L"ToolButtons/MouseOn/Stage_MouseOn", L"ToolButtons/MouseDown/Stage_MouseDown", L"ToolButtons/Normal/Stage_Normal");
			roomTop->add(stageButton);
			stageButton->addMouseListener(new LoginButton_MouseListener());

			// option button
			optionButton = CreateButton(conf, L"ToolButtons/Normal/Option_Normal", L"ToolButtons/MouseOn/Option_MouseOn", L"ToolButtons/MouseDown/Option_MouseDown", L"ToolButtons/Normal/Option_Normal");
			roomTop->add(optionButton);

			// modify button
			modifyButton = CreateButton(conf, L"ToolButtons/Normal/Modify_Normal", L"ToolButtons/MouseOn/Modify_MouseOn", L"ToolButtons/MouseDown/Modify_MouseDown", L"ToolButtons/Normal/Modify_Normal");
			roomTop->add(modifyButton);

			// clothes button
			clothesButton = CreateButton(conf, L"ToolButtons/Normal/Clothes_Normal", L"ToolButtons/MouseOn/Clothes_MouseOn", L"ToolButtons/MouseDown/Clothes_MouseDown", L"ToolButtons/Normal/Clothes_Normal");
			roomTop->add(clothesButton);

			// exit button
			exitButton = CreateButton(conf, L"ToolButtons/Normal/Exit_Normal", L"ToolButtons/MouseOn/Exit_MouseOn", L"ToolButtons/MouseDown/Exit_MouseDown", L"ToolButtons/Normal/Exit_Normal");
			exitButton->addMouseListener(new LoginButton_MouseListener());
			roomTop->add(exitButton);
			

			// select music button
			selectMusicButton = CreateButton(sconf, L"ToolBar/Normal/btn_selectmusic_normal", L"ToolBar/MouseOn/btn_selectmusic_mouseon", L"ToolBar/MouseDown/btn_selectmusic_mousedown", L"ToolBar/Normal/btn_selectmusic_normal");
			roomTop->add(selectMusicButton);
			selectMusicButton->addMouseListener(new LoginButton_MouseListener());
			selectMusicButton->setVisible(true);
			selectMusicButton->setEnabled(false);

			// message area
			messagePanel = CreateMessagePanel(conf);
			roomTop->add(messagePanel);


			//gcn::TextBox * bb = new gcn::TextBox("sdfsdfjjkwefkjndfkjsndfjksdnfkjsnfkjsdnfk");
			//bb->setSize(100, 100);
			//roomTop->add(bb, 400, 400);

			///////////////////////////////////////////////// stage buttons
			// up down button
			udButton = CreateButton(sconf, L"ToolBar/Normal/btn_UD_normal", L"ToolBar/MouseOn/btn_UD_mouseon", L"ToolBar/MouseDown/btn_UD_mousedown", L"ToolBar/Normal/btn_UD_normal");
			roomTop->add(udButton);
			udButton->setVisible(false);

			// settings button
			settingsButton = CreateButton(sconf, L"ToolBar/Normal/btn_settings_normal", L"ToolBar/MouseOn/btn_settings_mouseon", L"ToolBar/MouseDown/btn_settings_mousedown", L"ToolBar/Normal/btn_settings_normal");
			roomTop->add(settingsButton);
			settingsButton->setVisible(false);

			// exitStageButton button
			exitStageButton = CreateButton(sconf, L"ToolBar/Normal/btn_exitstage_normal", L"ToolBar/MouseOn/btn_exitstage_mouseon", L"ToolBar/MouseDown/btn_exitstage_mousedown", L"ToolBar/Normal/btn_exitstage_normal");
			roomTop->add(exitStageButton);
			exitStageButton->setVisible(false);
			exitStageButton->addMouseListener(new LoginButton_MouseListener());

#ifdef DIVA_GNET_OPEN
			openGameButton = new ButtonEx();
			openGameButton->setSize(410,169);
			//DivaRoomInfo rinfo = playerList->getRoomInfo(); 
			//if (rinfo.myId == rinfo.hostId)
			openGameButton->setLook(L"res/UI2.png",
				gcn::Rectangle(979,104,450,206),
				gcn::Rectangle(979,310,450,206),
				gcn::Rectangle(979,516,450,206),
				gcn::Rectangle(979,104,450,206),
				-19, -20);
			roomTop->add(openGameButton, 1200, 870);
			openGameButton->setVisible(false);
			openGameButton->addMouseListener(new LoginButton_MouseListener());

			//readyButton = CreateButton(sconf, L"ToolBar/Normal/btn_opengame_normal", L"ToolBar/Normal/btn_opengame_normal", L"ToolBar/Normal/btn_opengame_normal", L"ToolBar/Normal/btn_opengame_normal");
			readyButton = new ButtonEx();
			readyButton->setSize(410,169);
			readyButton->setLook(L"res/UI2.png",
				gcn::Rectangle(1431,207,450,206),
				gcn::Rectangle(1430,414,450,206),
				gcn::Rectangle(1430,619,450,206),
				gcn::Rectangle(1431,207,450,206),
				-19, -20);
			roomTop->add(readyButton, 1200, 870);
			readyButton->setVisible(false);
			readyButton->addMouseListener(new LoginButton_MouseListener());
			//readyButton->setPosition(readyButton->getX()+200,readyButton->getY());
#endif

			// decorate button
			decorateButton = CreateButton(sconf, L"ToolBar/Normal/btn_decorate_normal", L"ToolBar/MouseOn/btn_decorate_mouseon", L"ToolBar/MouseDown/btn_decorate_mousedown", L"ToolBar/Normal/btn_decorate_normal");
			roomTop->add(decorateButton);
			decorateButton->setVisible(false);

			// login 
			loginPanel = CreateLoginWindow(conf, L"Login");
			loginPanel->SetMovable(true);
			mgr->OpenWindow(loginPanel, conf[L"Login/Config"][L"blackAlpha"].asInt());

			//top->add(loginPanel);
			//loginPanel->addModifier(new

			//  ----- status panel
			statusPanel = CreateStatusPanel(conf);
			roomTop->add(statusPanel);
			
			//Player list
			sPlayerListPanel = CreatePlayerListPanel(sconf);
			sPlayerListPanel->setVisible(false);
			//Refresh_sPlayerList();
			//sPlayerList->setFirstIndex(8);
			roomTop->add(sPlayerListPanel);

			//sPlayerListPanel->setVisible(false);

			usernameInput->setText(L"test");
			passwordInput->setText(L"test");
			
			roomTop->setEnabled(false);

			// 

			roomTop->add(CreateThingList(sconf));

			roomTop->add(CreateTeamList(sconf));

			// ---------------

			roomListPanel = CreateRoomListWindow(rconf);
			roomListPanel->setVisible(true);
			roomListPanel->SetMovable(true);
			//top->add(roomListPanel);

			stageList = CreateStageList(rconf);
			stageList->setVisible(false);
			roomTop->add(stageList);

			MessageChannelChange(CHANNEL_WORLD);
			//////////////////////////////////////////////////////////////////////////

			// ---------------------------  test

			//mgr->GetMB()->Show(L"登录错误啊啊啊啊", L"测试", gcn::MessageBoxEx::TYPE_YESNO); 

		
			//LoginButtonClicked();
			// ------------- Refresh All
			//RefreshStatus();

			//Refresh_hostInfo();

			//LabelEx* lb = new LabelEx(L"¹þ¹þ¹þ¹þ¹þ¹þ");
			//lb->setHeight(lb->getFont()->getHeight());
			//lb->setWidth(50);
			//roomTop->add(lb, 0, 0);
		}

		HouseUI::~HouseUI()
		{
			detachObserver();

			if (playerListFont)
			{
				delete playerListFont;
			}
			if (statusPanelFont)
			{
				delete statusPanelFont;
			}
			if (messageAreaFont)
			{
				delete messageAreaFont;
			}
		}

		void HouseUI::observer_auth(divanet::Notification msg)
		{
			if(msg.msg()==divanet::AuthClient::NOTIFY_AUTH_REPLAY) {
				if (msg.description()=="ok")
				{
					PlayerInfo info;
					int t;
					wchar_t un[100], nm[100];

					MY_PLAYER_INFO.setUid(NET_INFO.uid);
					info.id = Base::String(NET_INFO.uid).toAny<int>();
					info.username = Base::String(NET_INFO.username);
					info.nickname = Base::String(NET_INFO.nickname);
					PlayerManager::Instance()->SetHostInfo(info);
					setState(STATE_ROOM);
					//loginPanel->setVisible(false);
					//roomTop->setEnabled(true);

					//PlayerManager::Instance()->GetStageGuests().push_back("SonicMisora");
					PlayerManager::Instance()->SetOnline(true);
					//RefreshStatus();
					Refresh_hostInfo();
					// prepare
					MAPMGR.PrepareDivaMapListInfo();


					CHAT_CLIENT.login();
					SCHEDULER_CLIENT.login();
					STAGE_CLIENT.login();
					//divanet::NetworkManager::instance().core()->send("auth#setuid","%s",MY_PLAYER_INFO.uid().c_str());
					messagePanelChatBox->addText(("[系统] 欢迎您, "+NET_INFO.nickname).unicode_str(), gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"system"]));

				}
				else if(msg.description()=="already")
				{
					mgr->GetMB()->Show(L"该账号已经登录。", L"提示");
				}
				else if(msg.description()=="wrongpasswd")
				{
					mgr->GetMB()->Show(L"账号或密码错误。", L"提示");
				}
				else if(msg.description()=="unactived")
				{
					mgr->GetMB()->Show(L"账号尚未激活。",L"提示");
				}
				else
				{
					mgr->GetMB()->Show(L"登录发生意外。请稍后再试。", L"提示");
				}

				if(msg.description()!="ok")
				{
					LOGGER->log("中断连接%s", msg.description().c_str());
					disconnectServer();
				}
			}
			else if(msg.msg()==divanet::AuthClient::NOTIFY_CONNECT) {
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 认证服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else
					messagePanelChatBox->addText(L"[提示] 认证服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
			}
			else if(msg.msg()==divanet::AuthClient::NOTIFY_TIMEOUT) {
				mgr->GetMB()->RegisterCallback(MessageBoxEx::Callback(&HouseUI::cb_connect_break, this));
				mgr->GetMB()->Show(L"网络连接中断。",L"提示");
				disconnectServer();
			}
		}

		void HouseUI::observer_chat(divanet::Notification msg)
		{
			divanet::GPacket *packet = static_cast<divanet::GPacket*>(msg.extra());
			switch(msg.msg()) 
			{
			case divanet::ChatClient::NOTIFY_CHAT_AUTH:
				break;
			case divanet::ChatClient::NOTIFY_CHAT_JOIN:
				break;
			case divanet::ChatClient::NOTIFY_CHAT_MSG:
				{
					Base::String msg;
					//msg += L"["+Base::s2ws(packet->getItem(3)->getString())+L"] ";
					msg = gnet::ItemUtility::getWString(packet->getItem(4));
					gcn::Color color;
					if (msg[1] == L'W')
						color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"world"]);
					else if (msg[1] == L'P')
						color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"private"]);
					messagePanelChatBox->addText(msg(3, -1), color);
					break;
				}
			case divanet::ChatClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 聊天服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else
					messagePanelChatBox->addText(L"[提示] 聊天服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::ChatClient::NOTIFY_TIMEOUT:
				messagePanelChatBox->addText(L"[提示] 聊天服务器断开连接..", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			}
		}

		void HouseUI::observer_scheduler(divanet::Notification msg)
		{
			switch(msg.msg())
			{
			case divanet::SchedulerClient::NOTIFY_UPDATE_ROOMLIST:
				{
					roomListView->clearItems();
					const divanet::RoomInfos &infos = SCHEDULER_CLIENT.getRoomList();
					for(int i = 0; i < infos.size(); i++)
					{
						if(infos[i].state==divanet::RoomInfo::STAGE) {
							//RoomListItem* b = SetRoomListItemInfo(rconf,L"RoomList/RoomItem_normal", L"RoomList/RoomItem_on", L"RoomList/RoomItem_down");
							Network::RoomInfo info;
							info.maxPlayerNum = infos[i].capacity;
							info.owner = Base::s2ws(infos[i].ownerId);
							info.playerNum = infos[i].playernum;
							info.selectedSong.push_back("songID : "+Base::String::any2string(infos[i].sondId));
							info.stageName = Base::String::any2string(infos[i].playernum)+"/"+Base::String::any2string(infos[i].capacity);
							//b->setInfo(info);

							roomListView->pushRoomItem(info);
						}
					}
				}
				break;
			case divanet::SchedulerClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 舞台服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else
					messagePanelChatBox->addText(L"[提示] 舞台服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::SchedulerClient::NOTIFY_TIMEOUT:
				messagePanelChatBox->addText(L"[提示] 舞台服务器断开连接..", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			}
		}

		void HouseUI::observer_stage(divanet::Notification msg)
		{
			switch(msg.msg())
			{
			case divanet::StageClient::NOTIFY_STAGE_CLOSED:
				if(state==STATE_STAGE) {
					setState(STATE_ROOM);
					mgr->GetMB()->Show(L"房主离开舞台。");
				}
				break;
			case divanet::StageClient::NOTIFY_STAGE_JOIN_RESPONSE:
				if(msg.description()=="ok")
				{
					//StateChange_ROOMLIST_STAGE();
					mgr->GetMB()->Destroy();
					setState(STATE_STAGE);
					roomId = static_cast<divanet::GPacket*>(msg.extra())->getItem(2)->getString();
				}
				else
					mgr->GetMB()->Show(L"加入房间出错，请稍后再试。");
				break;
			case divanet::StageClient::NOTIFY_STAGE_JOIN:
				{
					int index = msg.arg();
					PlayerInfo playerInfo;
					playerInfo.id = Base::String::string2any<int>(STAGE_CLIENT.info().waiters[index-1].uid);
					playerInfo.nickname = STAGE_CLIENT.info().waiters[index-1].nickname;
					StageListItem::StagePlayerInfo info;
					info.playerInfo = playerInfo;
					info.slot = index;
					info.status = 0;
					info.teamIndex = 0;
					dynamic_cast<StageListItem*>(stageList->getItems()[index-1])->setInfo(info);
					//mgr->GetMB()->Destroy();
					Refresh_sPlayerList();
				}
				break;
			case divanet::StageClient::NOTIFY_UPDATE_READY:
				{
					int index = msg.arg();
					StageListItem* item = dynamic_cast<StageListItem*>(stageList->getItems()[index-1]);
					if(msg.description()=="ready")
					{
						StageListItem::StagePlayerInfo info = item->getInfo();
						info.status = 1;
						item->setInfo(info);
					}
					else
					{
						StageListItem::StagePlayerInfo info = item->getInfo();
						info.status = 0;
						item->setInfo(info);
					}
				}
				break;
			case divanet::StageClient::NOTIFY_STAGE_LEAVE:
				{
					int index = msg.arg();
					PlayerInfo playerInfo;
					playerInfo.id = 0;
					StageListItem::StagePlayerInfo info;
					info.playerInfo = playerInfo;
					info.slot = 0;
					info.status = 0;
					info.teamIndex = 0;
					dynamic_cast<StageListItem*>(stageList->getItems()[index-1])->setInfo(info);

					Refresh_sPlayerList();
				} 
				break;
			case divanet::StageClient::NOTIFY_STAGE_START:
				if(msg.description()=="start")
				{
					if(mgr->GetMB()->isTopWindow())
						mgr->GetMB()->Destroy();
					
					Base::Path songPath = MAPMGR.GetDivaOLFilePath(STAGE_CLIENT.info().songId[0].songId, static_cast<divamap::DivaMap::LevelType>(STAGE_CLIENT.info().songId[0].level)); 
					CORE_PTR->setSong(songPath.filePath().str(), songPath.fileName());
					
					CORE_PTR->setInitState("net_load");
					divacore::MultiPlay *multiplay = NULL;
					if(STAGE_CLIENT.info().mode=="multiplay")
						multiplay = new divacore::MultiPlay;
					CORE_PTR->registerGameMode(multiplay);
					multiplay->registerNetworkEvent();
					NextState = "core";
				}
				else if(msg.description()=="notify")
				{
					mgr->GetMB()->Show(L"准备开始游戏...", L"提示", gcn::MessageBoxEx::TYPE_NONE); 
				}
				else if(msg.description()=="failed")
				{
					Base::String info = ((divanet::GPacket*)msg.extra())->getItem(2)->getString();
					if(info=="noselect")
						mgr->GetMB()->Show(L"开始失败，没有选择歌曲", L"提示", gcn::MessageBoxEx::TYPE_OK); 
					else
						mgr->GetMB()->Show(L"开始失败，没有准备或非法队伍人数", L"提示", gcn::MessageBoxEx::TYPE_OK); 
				}
				break;

			case divanet::StageClient::NOTIFY_UPDATE_INFO:
				stageList->clearItems();
				for(int i = 0; i < STAGE_CLIENT.info().waiters.size(); i++)
				{
					//for (int i=1; i<=8; i++)
					StageListItem *item = new StageListItem(rconf[L"PlayerList/playerItem_back"][L"filename"].asString(), GetRect(rconf[L"PlayerList/playerItem_back"]));
					PlayerInfo playerInfo;
					playerInfo.id = Base::String::string2any<int>(STAGE_CLIENT.info().waiters[i].uid);
					playerInfo.nickname = STAGE_CLIENT.info().waiters[i].nickname;
					StageListItem::StagePlayerInfo info;
					info.playerInfo = playerInfo;
					info.slot = i + 1;
					info.status = STAGE_CLIENT.info().waiters[i].status == divanet::WaiterInfo::READY;
					info.teamIndex = 0;
					item->setInfo(info);
					item->setTeamColor(STAGE_CLIENT.info().waiters[i].color);
					stageList->pushItem(item);


				}

				{
				int color = STAGE_CLIENT.myInfo().color;
				for (int i=0; i<teamListButtons.size(); i++)
					teamListButtons[i]->setSelected(color == i);
				}

				Refresh_sPlayerList();
				
				STAGE_CLIENT.refreshMusic();
				Refresh_SongList();

				break;
			case divanet::StageClient::NOTIFY_UPDATE_COLOR:
				{
					int index = msg.arg();
					if(STAGE_CLIENT.isMe(index))
					{
						int color = STAGE_CLIENT.myInfo().color;
						for (int i=0; i<teamListButtons.size(); i++)
							teamListButtons[i]->setSelected(color == i);
					}
					int color = (STAGE_CLIENT.info().waiters[index-1]).color;
					((StageListItem*)(stageList->getItems()[index - 1]))->setTeamColor(color);
					break;
				}

				Refresh_sPlayerList();
			case divanet::StageClient::NOTIFY_UPDATE_SONG:
				if(!STAGE_CLIENT.owner()) 
				{
					STAGE_CLIENT.refreshMusic();
					Refresh_SongList();
				}
				messagePanelChatBox->addText(L"[提示] 房主更改了歌曲列表", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::StageClient::NOTIFY_STAGE_LEAVE_RESPONSE:
				break;
			case divanet::StageClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 游戏服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else
					messagePanelChatBox->addText(L"[提示] 游戏服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::StageClient::NOTIFY_TIMEOUT:
				messagePanelChatBox->addText(L"[提示] 游戏服务器断开连接..", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			}
		}

		void HouseUI::attachObserver() {
			AUTH_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_auth,this));
			CHAT_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_chat,this));
			SCHEDULER_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_scheduler,this));
			STAGE_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_stage,this));
		}

		void HouseUI::detachObserver() {
			AUTH_CLIENT.clearObserver();
			CHAT_CLIENT.clearObserver();
			SCHEDULER_CLIENT.clearObserver();
			STAGE_CLIENT.clearObserver();
		}

		bool HouseUI::connectServer() {
#ifdef DIVA_GNET_OPEN
			try
			{
				if(!AUTH_CLIENT.isConnect()) {
					if(!AUTH_CLIENT.connect())
						return false;
					if(!CHAT_CLIENT.connect())
						return false;
					if(!SCHEDULER_CLIENT.connect())
						return false;
					if(!STAGE_CLIENT.connect())
						return false;
				}
				return true;
			}
			catch(const char *ev)
			{
				LOGGER->log(ev);
			}
			return false;
#endif
		}
		void HouseUI::disconnectServer() {
#ifdef DIVA_GNET_OPEN
			AUTH_CLIENT.disconnect();
			CHAT_CLIENT.disconnect();
			SCHEDULER_CLIENT.disconnect();
			STAGE_CLIENT.disconnect();
#endif
		}
		void HouseUI::request_roomList() {
#ifdef DIVA_GNET_OPEN
			SCHEDULER_CLIENT.updateRoomList();
			//divanet::NetworkManager::instance().scheduler()->send("scheduler#roomlist");
#endif
		}
		void HouseUI::cb_connect_break() {
			mgr->GetMB()->RegisterCallback();
			exit(0);
		}
		void HouseUI::open_stage() {
#ifdef DIVA_GNET_OPEN
			STAGE_CLIENT.create(8);
			//divanet::NetworkManager::instance().core()->send("stage#create","%d",2);
			roomId = MY_PLAYER_INFO.uid();
#endif
		}
		void HouseUI::start_game() {
#ifdef DIVA_GNET_OPEN
			if(STAGE_CLIENT.start())
				mgr->GetMB()->Show(L"准备开始游戏...", L"提示", gcn::MessageBoxEx::TYPE_NONE); 
#endif
		}
		void HouseUI::leave_stage() {
#ifdef DIVA_GNET_OPEN
			STAGE_CLIENT.leave();
			//divanet::NetworkManager::instance().core()->send("stage#leave");
#endif
		}

		void HouseUI::Enter()
		{
			sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
			top->setVisible(true);
			Refresh_SongList();
			sora::SoraBGMManager::Instance()->play(config[L"lobbyMusicFilename"].asString(), false);
		}
		
		void HouseUI::Leave()
		{
			sora::GCN_GLOBAL->getTop()->remove(top);
			top->setVisible(false);
			sora::SoraBGMManager::Instance()->stop(false);
			
#ifndef SONICMISORA_MODIFYHYF
			//disconnectServer();
#endif
		}

		void HouseUI::Render()
		{
		}

		void HouseUI::Update(float dt)
		{
			RecvMsg();

			MAPQUEUE* q = MAPMGR.GetQueue();
			while (!q->empty())
			{
				const divamap::DivaMapEventMessage &t = (*((*q).begin()));
				switch (t.eventType)
				{
				case divamap::DivaMapEventMessage::PrepareMapList :
					if (!t.error && t.finish)
						selectMusicButton->setEnabled(true);
					else
						throw "fuck it!";
					
					break;
				}
				q->pop_front();
			}

			if(state==STATE_ROOMLIST) {
				BASE_PER_PERIOD_BEGIN(dt,1.0);
				
				Refresh_RoomList(true);

				BASE_PER_PERIOD_END();
			}
		}

		void HouseUI::RecvMsg()
		{
#ifndef DIVA_GNET_OPEN
			while (!Net::Network::isEmpty())
			{
				std::wstring msg, arg;
				Net::Network::Recv(msg, arg);

				if (msg == L"LOGIN_RET")
				{
					if (arg[0] == '0')
					{
						//throw "LOGIN FAILED";
					}
					else
					{
						PlayerInfo info;
						int t;
						wchar_t un[100], nm[100];
						swscanf(arg.c_str(), L"%d%s%s%d", &t, un, nm, &info.id);
						info.username = un;
						info.nickname = nm;
						PlayerManager::Instance()->SetHostInfo(info);
						state = STATE_ROOM;
						loginPanel->setVisible(false);
						roomTop->setEnabled(true);

						//PlayerManager::Instance()->GetStageGuests().push_back("SonicMisora");
						PlayerManager::Instance()->SetOnline(true);
						//RefreshStatus();
						Refresh_hostInfo();
					}
				}
			}
#endif
		}

		void HouseUI::Refresh_SongList()
		{
			WJson::Value t2 = sconf[L"SongList/songitem_background"];
			songList->clearItems();

			for (vector<divamap::DivaMapSelectedItem>::iterator i = SELECTEDMAPS.begin(); i != SELECTEDMAPS.end(); i++)
			{
				divamap::DivaMap& m = MAPS[i->id];
				SongListItem* item = new SongListItem(t2[L"filename"].asString(), GetRect(t2));
				item->setText(m.header.name + L"(" + config[L"difNames"][(int)i->level].asString() + L":" +
					gcn::iToWS(m.levels[i->level].difficulty) + L",BPM:" + gcn::iToWS(m.header.bpm) + L")");
				songList->pushItem(item);
			}

			if (SELECTEDMAPS.size())
			{
				if (MAPS[SELECTEDMAPS[0].id].header.mapType == divamap::DivaMapHeader::Normal)
					setSongListImage(1);
				else
					setSongListImage(2);
			}
			else
				setSongListImage(0);
		}

		void HouseUI::StateChange_ROOM_STAGE()
		{
			state = STATE_STAGE;
			sPlayerListPanel->setVisible(true);

			// set all the room buttons to invisible
			shopButton->setVisible(false);
			stageButton->setVisible(false);
			optionButton->setVisible(false);
			modifyButton->setVisible(false);
			clothesButton->setVisible(false);
			exitButton->setVisible(false);

			// set all the stage buttons to visible

			udButton->setVisible(true);
			settingsButton->setVisible(true);
			exitStageButton->setVisible(true);
			
			decorateButton->setVisible(true);
			thingList->setVisible(true);
			teamList->setVisible(true);
			openGameButton->setVisible(true);
			readyButton->setVisible(true);

			// exit button
			/*exitButton = CreateButton(conf, "ToolButtons/Normal/Exit_Normal", "ToolButtons/MouseOn/Exit_MouseOn", "ToolButtons/MouseDown/Exit_MouseDown", "ToolButtons/Normal/Exit_Normal");
			roomTop->add(exitButton);*/

			Refresh_sPlayerList();
		}

		void HouseUI::StateChange_STAGE_ROOM()
		{
			leave_stage();

			state = STATE_ROOM;
			sPlayerListPanel->setVisible(false);

			// set all the room buttons to visible
			shopButton->setVisible(true);
			stageButton->setVisible(true);
			optionButton->setVisible(true);
			modifyButton->setVisible(true);
			clothesButton->setVisible(true);
			exitButton->setVisible(true);

			// set all the stage buttons to invisible

			udButton->setVisible(false);
			settingsButton->setVisible(false);
			exitStageButton->setVisible(false);
			decorateButton->setVisible(false);
			thingList->setVisible(false);
			teamList->setVisible(false);
			stageList->setVisible(false);
			//songList->setVisible(false);
			openGameButton->setVisible(false);
			readyButton->setVisible(false);
		}

		void HouseUI::StateChange_LOGINWINDOW_ROOM()
		{
			state = STATE_ROOM;
			if (mgr->GetTopWindow() != loginPanel)
				mgr->CloseTopWindow();
			loginPanel->FadeOut(conf[L"Login/Config"][L"disTime"].asInt());
			//mgr->CloseTopWindow();
			//UIHelper::SetUIFade(loginPanel);
			//roomTop->setEnabled(true);
		}

		void HouseUI::StateChange_ROOM_ROOMLIST()
		{
			state = STATE_ROOMLIST;
			
			//roomTop->setEnabled(false);
			//UIHelper::SetUIFade(roomListPanel, 0, 255, -1, true);
			mgr->OpenWindow(roomListPanel, 70);
			roomListPanel->FadeIn(10);

			roomListView->clearItems();
			Refresh_RoomList();
			
			//request_roomList();
		}

		void HouseUI::StateChange_ROOMLIST_ROOM()
		{
			state = STATE_ROOM;

			//roomTop->setEnabled(true);

			//UIHelper::SetUIFade(roomListPanel);
			roomListPanel->FadeOut(10);
		}

		void HouseUI::StateChange_ROOMLIST_STAGE()
		{
			state = STATE_STAGE;
			sPlayerListPanel->setVisible(true);

			// set all the room buttons to invisible
			shopButton->setVisible(false);
			stageButton->setVisible(false);
			optionButton->setVisible(false);
			modifyButton->setVisible(false);
			clothesButton->setVisible(false);
			exitButton->setVisible(false);

			// set all the stage buttons to visible

			udButton->setVisible(true);
			settingsButton->setVisible(true);
			exitStageButton->setVisible(true);
			selectMusicButton->setVisible(true);
			decorateButton->setVisible(true);
			thingList->setVisible(true);
			teamList->setVisible(true);
			stageList->setVisible(true);
			songList->setVisible(true);
			if (STAGE_CLIENT.owner())
				openGameButton->setVisible(true);
			else
				readyButton->setVisible(true);

			//
			//roomListPanel->setVisible(false);
			//roomTop->setEnabled(true);
			roomListPanel->FadeOut(10);
			Refresh_sPlayerList();
		}

		void HouseUI::setState(int des)
		{
			if (state == STATE_ROOM && des == STATE_STAGE)
			{
				StateChange_ROOM_STAGE();
				return;
			}
			if (state == STATE_STAGE && des == STATE_ROOM)
			{
				StateChange_STAGE_ROOM();
				return;
			}
			if (state == STATE_LOGINWINDOW && des == STATE_ROOM)
			{
				StateChange_LOGINWINDOW_ROOM();
				return;
			}
			if (state == STATE_ROOM && des == STATE_ROOMLIST)
			{
				StateChange_ROOM_ROOMLIST();
				return;
			}
			if (state == STATE_ROOMLIST && des == STATE_ROOM)
			{
				StateChange_ROOMLIST_ROOM();
				return;
			}
			if (state == STATE_ROOMLIST && des == STATE_STAGE)
			{
				
				StateChange_ROOMLIST_STAGE();
				return;
			}
		}

		void HouseUI::MessageSliderSlided(int v)
		{
			messagePanelChatBox->setFirstIndex(v);
		}

		void HouseUI::MessageBoxFirstIndexChanged(int v)
		{
			messageSlider->setMarkPosition(v);
		}

		void HouseUI::MessageBoxItemChanged()
		{
			messageSlider->setMarkScale(0, messagePanelChatBox->getMaxIndex(), messagePanelChatBox->getItemCount());
		}

		void HouseUI::ParseJson(const std::wstring& filename, const std::wstring& stage, const std::wstring& room)
		{
			//std::ifstream fs(filename);
			WJson::Reader reader;
			if (!reader.parse(ReadJsonFile(filename), conf))
				throw "failed reading WJson.";

			//std::ifstream fs2(stage);
			if (!reader.parse(ReadJsonFile(stage), sconf))
				throw "failed.";

			if (!reader.parse(ReadJsonFile(room), rconf))
				throw "failed.";
		}

		gcn::WindowEx* HouseUI::CreateMyWindow(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv;
			tv = conf[name];
			WindowEx* con = new WindowEx();
			con->load(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			con->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return con;
		}

		gcn::ContainerEx* HouseUI::CreateStaticImage(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv;
			tv = conf[name];
			ContainerEx* con = new ContainerEx();
			con->load(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			con->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return con;
		}

		//gcn::ContainerEx* HouseUI::CreateStaticImage(const std::wstring& name)
		//{
		//	using namespace gcn;
		//	Json::Value tv;
		//	tv = conf[name];
		//	ContainerEx* con = new ContainerEx();
		//	con->load(tv[L"filename"].asString(), 
		//		gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
		//	con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		//	top->add(con,  tv[L"desX"].asInt(), tv[L"desY"].asInt());
		//	return con;
		//}

		//gcn::SuperButtonEx* HouseUI::CreateButton(const std::wstring& normal, const std::wstring& on,
		//	const std::wstring& down, const std::wstring& disable)
		//{
		//	using namespace gcn;
		//	SuperButtonEx* b = _CreateButton(normal, on, down, disable);
		//	top->add(b);
		//	return b;
		//}

		gcn::SuperButtonEx* HouseUI::CreateButton(const WJson::Value& conf, const std::wstring& normal, const std::wstring& on,
			const std::wstring& down, const std::wstring& disable)
		{
			using namespace gcn;
			WJson::Value tv1 = conf[normal], tv2 =conf[on], tv3 = conf[down], tv4 = conf[disable];
			SuperButtonEx* b = new SuperButtonEx();
			b->setLook(tv1[L"filename"].asString(),
				gcn::Rectangle(tv1[L"srcX"].asInt(), tv1[L"srcY"].asInt(), tv1[L"width"].asInt(), tv1[L"height"].asInt()),
				gcn::Rectangle(tv2[L"srcX"].asInt(), tv2[L"srcY"].asInt(), tv2[L"width"].asInt(), tv2[L"height"].asInt()),
				gcn::Rectangle(tv3[L"srcX"].asInt(), tv3[L"srcY"].asInt(), tv3[L"width"].asInt(), tv3[L"height"].asInt()),
				gcn::Rectangle(tv4[L"srcX"].asInt(), tv4[L"srcY"].asInt(), tv4[L"width"].asInt(), tv4[L"height"].asInt()),
				0, 0,
				tv2[L"desX"].asInt() - tv1[L"desX"].asInt(), tv2[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv3[L"desX"].asInt() - tv1[L"desX"].asInt(), tv3[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv4[L"desX"].asInt() - tv1[L"desX"].asInt(), tv4[L"desY"].asInt() - tv1[L"desY"].asInt());
			b->setSize(tv1[L"width"].asInt(), tv1[L"height"].asInt());
			b->setPosition(tv1[L"desX"].asInt(), tv1[L"desY"].asInt());
			
			return b;
		}

		RoomListItem* HouseUI::SetRoomListItemInfo(const WJson::Value conf, const std::wstring& normal, const std::wstring& on, const std::wstring& down)
		{
			using namespace gcn;
			WJson::Value tv1 = conf[normal], tv2 =conf[on], tv3 = conf[down];
			//RoomListItem* b = new RoomListItem();
			roomListView->setRoomItemBaseInfo(tv1[L"filename"].asString(),
				gcn::Rectangle(tv1[L"srcX"].asInt(), tv1[L"srcY"].asInt(), tv1[L"width"].asInt(), tv1[L"height"].asInt()),
				tv2[L"filename"].asString(),
				gcn::Rectangle(tv2[L"srcX"].asInt(), tv2[L"srcY"].asInt(), tv2[L"width"].asInt(), tv2[L"height"].asInt()),
				tv2[L"filename"].asString(),
				gcn::Rectangle(tv3[L"srcX"].asInt(), tv3[L"srcY"].asInt(), tv3[L"width"].asInt(), tv3[L"height"].asInt()));
			

			return NULL;
			//return b;
		}

		gcn::WTextField* HouseUI::CreateInput(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::WTextField* text = new WTextField();
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}

		gcn::LabelEx* HouseUI::CreateLabel(const WJson::Value& conf, const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::LabelEx* text = new LabelEx(tv[L"text"].asString());
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}

		gcn::MarkerEx* HouseUI::CreateMarker(const WJson::Value conf, const std::wstring& up1, const std::wstring& up2, const std::wstring &up3, const std::wstring& down1, const std::wstring& down2, const std::wstring &down3, const std::wstring& mid1, const std::wstring& mid2, const std::wstring &mid3)
		{
			using namespace gcn;
			gcn::MarkerEx* marker = new MarkerEx();
			marker->setLook(conf[up1][L"filename"].asString(),
				GetRect(conf[up1]), GetRect(conf[up2]), GetRect(conf[up3]),
				GetRect(conf[down1]), GetRect(conf[down2]), GetRect(conf[down3]),
				GetRect(conf[mid1]), GetRect(conf[mid2]), GetRect(conf[mid3]));
			marker->setOffset(
				0, 0,
				conf[up2][L"desX"].asInt() - conf[up1][L"desX"].asInt(), conf[up2][L"desY"].asInt() - conf[up1][L"desY"].asInt(),
				conf[up3][L"desX"].asInt() - conf[up1][L"desX"].asInt(), conf[up3][L"desY"].asInt() - conf[up1][L"desY"].asInt(),
				0, 0,
				conf[down2][L"desX"].asInt() - conf[down1][L"desX"].asInt(), conf[down2][L"desY"].asInt() - conf[down1][L"desY"].asInt(),
				conf[down3][L"desX"].asInt() - conf[down1][L"desX"].asInt(), conf[down3][L"desY"].asInt() - conf[down1][L"desY"].asInt(),
				0, 0,
				conf[mid2][L"desX"].asInt() - conf[mid1][L"desX"].asInt(), conf[mid2][L"desY"].asInt() - conf[mid1][L"desY"].asInt(),
				conf[mid3][L"desX"].asInt() - conf[mid1][L"desX"].asInt(), conf[mid3][L"desY"].asInt() - conf[mid1][L"desY"].asInt());
			marker->setWidth(conf[up1][L"width"].asInt());
			return marker;
		}


		gcn::WindowEx* HouseUI::CreateLoginWindow(const WJson::Value& conf, const std::wstring& prefix)
		{
			using namespace gcn;
			WJson::Value tv;
			
			// panel
			WindowEx* con = CreateMyWindow(conf, prefix + L"/BackGround");
			int topX = con->getX(), topY = con->getY();

			//tv = conf[L"Login/Config"];
			//con->setVisible(true);
			//con->setPosition(tv[L"desX_1"].asInt(), tv[L"desY_1"].asInt());
			//con->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_2"].asInt(), tv[L"desY_2"].asInt()),
			//	tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));

			// username text box
			WTextField* username = CreateInput(conf, prefix + L"/textBox_1");
			con->add(username, username->getX() - topX, username->getY() - topY);
			username->setId("LoginWindow_UsernameInput");
			sora::SoraGUI::Instance()->registerGUIResponser(username, this, "LoginWindow_UsernameInput", sora::RESPONSEACTION);
			//sora::GCN_GLOBAL->getTop()->focusNext();
			usernameInput = username;

			// pass word textbox
			WTextField* password = CreateInput(conf, prefix + L"/textBox_2");
			con->add(password, password->getX() - topX, password->getY() - topY);
			password->setId("LoginWindow_PasswordInput");
			sora::SoraGUI::Instance()->registerGUIResponser(password, this, "LoginWindow_PasswordInput", sora::RESPONSEACTION);
			password->setPasswordMode(true);
			passwordInput = password;

			// login button
			SuperButtonEx* lb = CreateButton(conf, prefix + L"/LoginButton_Normal",
				prefix + L"/LoginButton_MouseOn",
				prefix + L"/LoginButton_MouseDown",
				prefix + L"/LoginButton_Normal");
			con->add(lb, lb->getX() - topX, lb->getY() - topY);
			lb->addMouseListener(new LoginButton_MouseListener());
			loginButton = lb;
			//lb->setBaseColor(gcn::Color(255, 0, 0));

			//loginButton->addModifier(new GUIAnimation_Position(gcn::Point(0, 0), 120, GUIAnimation_Float_Linear, GUIAnimation_Mode::NONE));

			// user login label
			LabelEx* lb1 = CreateLabel(conf, prefix + L"/Label_1");
			con->add(lb1, lb1->getX() - topX, lb1->getY() - topY);
			

			// username label
			LabelEx* lb2 = CreateLabel(conf, prefix + L"/Label_2");
			con->add(lb2, lb2->getX() - topX, lb2->getY() - topY);

			// password label
			LabelEx* lb3 = CreateLabel(conf, prefix + L"/Label_3");
			con->add(lb3, lb3->getX() - topX, lb3->getY() - topY);

			con->setVisible(true);
			
			
			//top->add(con);
			return con;
		}

		WindowEx* HouseUI::CreateRoomListWindow(const WJson::Value conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"RoomList/Config"];

			WindowEx* panel = CreateMyWindow(conf, L"RoomList/background");

			//////////////////////////////////////////////////////////////////////////
			
			SuperButtonEx* closeButton = CreateButton(conf, L"RoomList/btn_close/btn_close_normal", L"RoomList/btn_close/btn_close_on",
				L"RoomList/btn_close/btn_close_down", L"RoomList/btn_close/btn_close_normal");
			closeButton->setPosition(closeButton->getX() - panel->getX(), closeButton->getY() - panel->getY());
			closeButton->addMouseListener(new LoginButton_MouseListener());
			panel->add(closeButton);
			roomListCloseButton = closeButton;

			//////////////////////////////////////////////////////////////////////////

			SuperButtonEx* openButton = CreateButton(conf, L"RoomList/btn_openStage/btn_openStage_normal", L"RoomList/btn_openStage/btn_openStage_on",
				L"RoomList/btn_openStage/btn_openStage_down", L"RoomList/btn_openStage/btn_openStage_normal");
			openButton->setPosition(openButton->getX() - panel->getX(), openButton->getY() - panel->getY());
			openButton->setText(tv[L"openStageText"].asString());
			openButton->addMouseListener(new LoginButton_MouseListener());
			panel->add(openButton);
			roomListOpenButton = openButton;
			
			
			//////////////////////////////////////////////////////////////////////////

			RoomList* list = new RoomList();
			list->setGap(gcn::Rectangle(tv[L"firstX"].asInt(), tv[L"firstY"].asInt(), tv[L"firstWidth"].asInt(), tv[L"firstHeight"].asInt()),
				tv[L"xGap"].asInt(), tv[L"yGap"].asInt());
			list->setMaxItem(tv[L"xItemCount"].asInt(), tv[L"yItemCount"].asInt());
			list->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			list->adjustSize();
			list->setPosition(list->getX() - panel->getX(), list->getY() - panel->getY());
			panel->add(list);
			RoomListItem::setTextPosition(tv[L"x1"].asInt(), tv[L"y1"].asInt(),
				tv[L"x2"].asInt(), tv[L"y2"].asInt(),
				tv[L"x3"].asInt(), tv[L"y3"].asInt(),
				tv[L"x4"].asInt(), tv[L"y4"].asInt());
			roomListView = list;

			//////////////////////////////////////////////////////////////////////////

			tv = conf[L"RoomList/scrollBar/Config"];
			RoomListSlider* slider = new RoomListSlider();
			//slider->setSize(
			// marker
			MarkerEx* marker = CreateMarker(conf, L"RoomList/scrollBar/slider/slider_normal_up",
				L"RoomList/scrollBar/slider/slider_on_up",
				L"RoomList/scrollBar/slider/slider_down_up",
				L"RoomList/scrollBar/slider/slider_normal_down",
				L"RoomList/scrollBar/slider/slider_on_down",
				L"RoomList/scrollBar/slider/slider_down_down",
				L"RoomList/scrollBar/slider/slider_normal_mid",
				L"RoomList/scrollBar/slider/slider_on_mid",
				L"RoomList/scrollBar/slider/slider_down_mid");

			//marker->setLook(
			slider->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			slider->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			slider->setLook(CreateButton(conf, L"RoomList/scrollBar/btn_up_normal", L"RoomList/scrollBar/btn_up_on", L"RoomList/scrollBar/btn_up_down", L"RoomList/scrollBar/btn_up_normal"),
				CreateButton(conf, L"RoomList/scrollBar/btn_down_normal", L"RoomList/scrollBar/btn_down_on", L"RoomList/scrollBar/btn_down_down", L"RoomList/scrollBar/btn_down_normal"),
				marker);
			slider->setMarkScale(0, list->getFullMaxPage(), list->getItemCount()); 
			slider->setPosition(slider->getX() - panel->getX(), slider->getY() - panel->getY());
			panel->add(slider);
			roomListSlider = slider;

			//////////////////////////Set Base RoomItem information///////////////////////////////
			
			SetRoomListItemInfo(conf, L"RoomList/RoomItem_normal", L"RoomList/RoomItem_on", L"RoomList/RoomItem_down");
			for (int i=1; i<=20; i++)
			{
				Network::RoomInfo info;
				info.maxPlayerNum = 10;
				info.owner = L"拥有者";
				info.playerNum = i%8 + 1;
				info.selectedSong.push_back(L"第一首歌");
				info.stageName = L"这里是舞台" + iToWS(i);
				roomListView->pushRoomItem(info);
				//list->pushItem();
			}
			Refresh_RoomList(false);

			return panel;
		}

		ContainerEx* HouseUI::CreateTeamList(const WJson::Value& conf)
		{
			using namespace gcn;

			WJson::Value tv = conf[L"TeamSelection/Config"];
			int itemCount = tv[L"itemCount"].asInt();

			WJson::Value t2 = conf[L"TeamSelection/Normal/btn_Team1_Normal"];
			WJson::Value t3 = conf[L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(itemCount)) + (L"_Normal")];

			teamList = new ContainerEx;
			teamList->setOpaque(false);
			teamList->setSize(t3[L"desX"].asInt() - t2[L"desX"].asInt() + t3[L"width"].asInt(),
				t3[L"height"].asInt());
			teamList->setPosition(t2[L"desX"].asInt(), t2[L"desY"].asInt());

			
			for (int i = 0; i < itemCount; i++)
			{
				SuperButtonEx* b = CreateButton(conf, 
					L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Normal"),
					L"TeamSelection/MouseOn/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_mouseon"),
					L"TeamSelection/Selected/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Selected"),
					L"TeamSelection/Normal/btn_Team" + s2ws(sora::int_to_str(i+1)) + (L"_Normal"));
				b->setSelected(i == 2);
				b->setPosition(b->getX() - teamList->getX(), b->getY() - teamList->getY());
				b->addMouseListener(new TeamSelect_MouseListener());
				b->userData = (void*)i;
				teamListButtons.push_back(b);
				
				teamList->add(b);
				//teamList->pushItem(new ThingListItem(t2[L"filename"].asString(),
				//	gcn::Rectangle(t2[L"srcX"].asInt(), t2[L"srcY"].asInt(), t2[L"width"].asInt(), t2[L"height"].asInt()),
				//	t3[L"filename"].asString(),
				//	gcn::Rectangle(t3[L"srcX"].asInt(), t3[L"srcY"].asInt(), t3[L"width"].asInt(), t3[L"height"].asInt())));
			}
			teamList->setVisible(false);

			return teamList;
		}

		void HouseUI::Refresh_sPlayerList(bool netRefresh)
		{
			//if (netRefresh)
			//{
			//	sPlayerList->clearItems();
			//	WJson::Value tv = sconf[L"RoomInfo/Config"];
			//	std::vector<PlayerInfo>& stageGuests= PlayerManager::Instance()->GetStageGuests();
			//	int c = 0;
			//	for (std::vector<PlayerInfo>::iterator i = stageGuests.begin(); i!=stageGuests.end(); i++, c++)
			//		sPlayerList->pushItem(new HouseUIRoomInfoListItem(i->nickname, tv[L"colorList"][c%(tv[L"maxItem"].asInt())].asUInt()));
			//}
			sPlayerList->clearItems();
			for (int i=0; i<stageList->getItemCount(); i++)
			{
				if (((StageListItem*)(stageList->getItems()[i]))->getInfo().playerInfo.id != 0)
					sPlayerList->pushItem(new HouseUIRoomInfoListItem(((StageListItem*)(stageList->getItems()[i]))->getInfo().playerInfo.nickname,
																		Base::String::any2string<int>(((StageListItem*)(stageList->getItems()[i]))->getInfo().playerInfo.id),
																		0xFFFFFFFF));
			}


			int page = sPlayerList->getItemCount() / sPlayerList->getMaxItem() + 1;
			if (playerListNowPage >= page)
				playerListNowPage = page - 1;
			if (playerListNowPage < 0)
				playerListNowPage = 0;
			sPlayerList->setFirstIndex(playerListNowPage * sPlayerList->getMaxItem());
			playerListPage->setText(iToWS(playerListNowPage + 1) + L"/" + iToWS(page));
		}

		void HouseUI::MessageChannelChange(int ch, int id /* = -1 */, const std::wstring &text)
		{
			msgChannelState = ch;
			if (ch == CHANNEL_WORLD)
			{
				messageChannel->setText(conf[L"MessageChannels"][L"world"].asString());
				messageToSomeOne->setText(L"");
			}
			else if (ch == CHANNEL_PRIVATE)
			{
				messageChannel->setText(conf[L"MessageChannels"][L"private"].asString());
				if (id != -1)
					msgSendId = id;
				if (msgSendId != -1)
					messageToSomeOne->setText(L"To " + text);
				else
					messageToSomeOne->setText(L"尚未选择");
			}
		}

		void HouseUI::Refresh_hostInfo()
		{
			PlayerInfo pf = PlayerManager::Instance()->GetHostInfo();
			wchar_t temp[200];
			_swprintf(temp, L"昵称：%s\nID：%d", pf.nickname.c_str(), pf.id);
			hostInfoLabel->setText(temp);
		}


		void HouseUI::RefreshStatus()
		{
			// Host Info Refresh
			

			//if (state == STATE_ROOM)
			//{
			//	roomTop->setEnabled(true);
			//	sPlayerListPanel->setVisible(false);
			//	loginPanel->setVisible(false);
			//}
			//else if (state == STATE_STAGE)
			//{
			//	// visible set
			//	roomTop->setEnabled(true);
			//	loginPanel->setVisible(false);
			//	sPlayerListPanel->setVisible(true);

			//	// playerlist refresh
			//	Refresh_sPlayerList();
			//}
			//else if (state == STATE_LOGINWINDOW)
			//{
			//	roomTop->setEnabled(false);
			//	sPlayerListPanel->setVisible(false);
			//	loginPanel->setVisible(true);
			//}
		}

		void HouseUI::Refresh_RoomList(bool updateRoomInfo)
		{
			if (updateRoomInfo)
				request_roomList();
			roomListSlider->setMarkScale(0, roomListView->getFullMaxPage(), roomListView->getMaxPage());
		}

		void HouseUI::RoomListSliderSlided(int v)
		{
			roomListView->setFirstPage(v);
		}

		void HouseUI::RoomListFirstPageChanged(int v)
		{
			roomListSlider->setMarkPosition(v);
		}

		void HouseUI::setSongListImage(int v)
		{
			if (v == 0)
			{
				songListImage->setVisible(false);
			}
			else
			{
				std::wstring st = L"";
				if (v == 1)
					st = L"SongList/GameMode/SingleMode";
				else if (v == 2)
					st = L"SongList/GameMode/CoupleMode";
				else if (v == 3)
					st = L"SongList/GameMode/RelayMode";
				songListImage->setVisible(true);
				WJson::Value t = sconf[st];
				songListImage->load(t[L"filename"].asString(), GetRect(t));
				songListImage->setPosition(t[L"desX"].asInt() - songListPanel->getX(), t[L"desY"].asInt() - songListPanel->getY());

			}
		}

		gcn::ContainerEx* HouseUI::CreateSongList(const WJson::Value& conf)
		{
			using namespace gcn;

			WJson::Value tv = conf[L"SongList/Config"],
				t2 = conf[L"SongList/songitem_background"];
			
			gcn::ContainerEx* con = new ContainerEx();
			con->setOpaque(false);
			con->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());

			gcn::ListBoxEx* list = new ListBoxEx();
			list->setGap(gcn::Rectangle(0, 0, t2[L"width"].asInt(), t2[L"height"].asInt()), tv[L"gap"].asInt());
			list->setMaxItem(tv[L"maxItem"].asInt());
			list->setWidth(t2[L"width"].asInt());
			list->adjustMyHeight();
			list->setPosition(t2[L"desX"].asInt() - con->getX(), t2[L"desY"].asInt() - con->getY());
			songListFont = new SoraGUIFont(L"msyh.ttf", tv[L"fontSize"].asInt());
			list->setFont(songListFont);
			con->add(list);
			songList = list;

			gcn::ContainerEx* image = CreateStaticImage(conf, L"SongList/GameMode/CoupleMode");
			image->setPosition(image->getX() - con->getX(), image->getY() - con->getY());
			songListImage = image;
			con->add(image);

			con->setSize(list->getWidth(), list->getHeight() + list->getY() - image->getY());
			setSongListImage(0);

			return con;
			
		}

		ThingList* HouseUI::CreateThingList(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"itemlist/Config"];
			WJson::Value t2 = conf[L"itemlist/item_itemback_normal"];
			WJson::Value t3 = conf[L"itemlist/item_itemback_selected"];

			thingList = new ThingList;
			thingList->setMaxItem(6);
			thingList->setGap(gcn::Rectangle(0, 0, t2[L"width"].asInt(), t2[L"height"].asInt()), tv[L"itemGap"].asInt());
			thingList->setSize(tv[L"items"].size() * (t2[L"width"].asInt() + tv[L"itemGap"].asInt()) - tv[L"itemGap"].asInt(), 
				t2[L"height"].asInt());
			thingList->setPosition(t3[L"desX"].asInt(), t3[L"desY"].asInt());

			for (WJson::Value::iterator i = tv[L"items"].begin(); i!= tv[L"items"].end(); i++)
			{
				thingList->pushItem(new ThingListItem(t2[L"filename"].asString(),
					gcn::Rectangle(t2[L"srcX"].asInt(), t2[L"srcY"].asInt(), t2[L"width"].asInt(), t2[L"height"].asInt()),
					t3[L"filename"].asString(),
					gcn::Rectangle(t3[L"srcX"].asInt(), t3[L"srcY"].asInt(), t3[L"width"].asInt(), t3[L"height"].asInt())));
			}
			thingList->setVisible(false);
			
			return thingList;
		}

		gcn::ContainerEx* HouseUI::CreateStatusPanel(const WJson::Value& conf)
		{
			using namespace gcn;
			ContainerEx* panel = CreateStaticImage(conf, L"StatusArea");
			WJson::Value tv = conf[L"StatusPanel/Config"];
			//bool b = conf.isMember("StatusPanel/Config");
			
			LabelEx* lb = new LabelEx(L"");
			lb->setSize(tv[L"lbWidth"].asInt(), tv[L"lbHeight"].asInt());
			panel->add(lb, tv[L"lbX"].asInt(), tv[L"lbY"].asInt());
			statusPanelFont = new gcn::SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			lb->setFont(statusPanelFont);
			hostInfoLabel = lb;

			return panel;
		}

		gcn::ContainerEx* HouseUI::CreatePlayerListPanel(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"RoomInfo/Config"];
			ContainerEx* panel = CreateStaticImage(conf, L"RoomInfo/background");
			playerListFont = new gcn::SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			
			RoomInfoList* list = new RoomInfoList();
			list->setMaxItem(tv[L"maxItem"].asInt());
			list->setSize(panel->getWidth(), panel->getHeight());
			list->setGap(gcn::Rectangle(tv[L"gap"][L"x"].asInt(),
				tv[L"gap"][L"y"].asInt(),
				tv[L"gap"][L"width"].asInt(),
				tv[L"gap"][L"height"].asInt()), 
				tv[L"gap"][L"itemGap"].asInt());
			//Font* font = new gcn::SoraGUIFont(L"res/msyh.ttf", 16);
			list->setFont(playerListFont);
			//list->setHorizontal(true);
			panel->add(list, 0, 0);
			sPlayerList = list;

			SuperButtonEx* b1 = CreateButton(conf, L"RoomInfo/buttons/btn_left", 
				L"RoomInfo/buttons/btn_left_mouseon", L"RoomInfo/buttons/btn_left_mousedown", L"RoomInfo/buttons/btn_left");
			b1->setPosition(b1->getX() - panel->getX(), b1->getY() - panel->getY());
			b1->addMouseListener(new LoginButton_MouseListener());
			playerListButton1 = b1;
			panel->add(b1);

			SuperButtonEx* b2 = CreateButton(conf, L"RoomInfo/buttons/btn_right", 
				L"RoomInfo/buttons/btn_right_mouseon", L"RoomInfo/buttons/btn_right_mousedown", L"RoomInfo/buttons/btn_right");
			b2->setPosition(b2->getX() - panel->getX(), b2->getY() - panel->getY());
			b2->addMouseListener(new LoginButton_MouseListener());
			playerListButton2 = b2;
			panel->add(b2);

			ContainerEx* pagenum = CreateStaticImage(conf, L"RoomInfo/buttons/label_pagenum");
			pagenum->setPosition(pagenum->getX() - panel->getX(), pagenum->getY() - panel->getY());
			panel->add(pagenum);
			playerListPagenumFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"pagenumFontSize"].asInt());
			pagenum->setFont(playerListPagenumFont);
			pagenum->setText(L"1/1");

			playerListPage = pagenum;
			playerListNowPage = 0;

			//for (int i=1; i<=9; i++)
			//list->pushItem(new HouseUIRoomInfoListItem("SonicMisora",  (tv[L"colorList"][2]).asInt()));
			//list->pushItem(new HouseUIRoomInfoListItem("SMBean",  (tv[L"colorList"][1]).asInt()));
			
			return panel;
		}

		gcn::ListBoxEx* HouseUI::CreateStageList(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"PlayerList/Config"], t2 = conf[L"PlayerList/playerItem_back"];
			WJson::Value colors = sconf[L"TeamColors"];

			StageListItem::TeamColors tc;
			tc.clear();
			for (WJson::Value::iterator i = colors.begin(); i != colors.end(); i++)
			{
				int zero = 0;
				tc.push_back((*i)[2].asUInt() + ((*i)[1].asUInt() << 8) + ((*i)[zero].asUInt() << 16));
			}
			StageListItem::setTeamColors(tc);

			ListBoxEx* list = new ListBoxEx();
			list->setOpaque(false);
			list->setMaxItem(tv[L"maxItem"].asInt());
			list->setGap(gcn::Rectangle(0, 0, tv[L"width"].asInt(), tv[L"height"].asInt()), tv[L"gap"].asInt());
			list->setWidth(t2[L"width"].asInt());
			list->adjustMyHeight();
			list->setPosition(t2[L"desX"].asInt(), t2[L"desY"].asInt());
			list->setFont(messageInputFont);

			tv = conf[L"PlayerList/ReadyIcon"];
			t2 = conf[L"PlayerList/HostIcon"];
			StageListItem::setIcons(tv[L"filename"].asString(), gcn::Helper::GetRect(tv), t2[L"filename"].asString(), gcn::Helper::GetRect(t2));

			return list;
		}

		gcn::ContainerEx* HouseUI::CreateMessagePanel(const WJson::Value& conf)
		{
			using namespace gcn;
			//WJson::Value tv = conf[];

			ContainerEx* panel = CreateStaticImage(conf, L"MessageArea/InputArea_BackGround");

			WJson::Value tv;

			//////////////////////////////////////////////////////////////////////////
			// chat box
			tv = conf[L"MessageArea/TextBox"];
			HouseMessageBox* chatBox = new HouseMessageBox();
			chatBox->setMaxLine(tv[L"maxLine"].asInt());
			chatBox->setWidth(tv[L"width"].asInt());
			chatBox->setPosition(tv[L"desX"].asInt() - panel->getX(), tv[L"desY"].asInt() - panel->getY());
			messageAreaFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			chatBox->setFont(messageAreaFont);
			chatBox->adjust();
			
			panel->add(chatBox);
			messagePanelChatBox = chatBox;

			// message slider

			MessageSlider* slider = new MessageSlider();
			WJson::Reader reader;
			reader.parse(ReadJsonFile(L"uiconfig/house/ChatSlider.json"), tv);
			MarkerEx* marker = CreateMarker(tv, L"up",
				L"up",
				L"up",
				L"down",
				L"down",
				L"down",
				L"mid",
				L"mid",
				L"mid");

			//marker->setLook(
			slider->setSize(tv[L"Config"][L"width"].asInt(), tv[L"Config"][L"height"].asInt());
			slider->setPosition(tv[L"Config"][L"desX"].asInt(), tv[L"Config"][L"desY"].asInt());
			gcn::ButtonEx* nullButton = new gcn::ButtonEx(), *nullButton2 = new gcn::ButtonEx();
			nullButton->setSize(1, 0);
			nullButton2->setSize(1, 0);
			slider->setLook(nullButton, nullButton2, marker);
			slider->setMarkScale(0, chatBox->getMaxIndex(), chatBox->getItemCount());
			//slider->setPosition(slider->getX() - panel->getX(), slider->getY() - panel->getY());

			panel->add(slider);
			messageSlider = slider;

			tv = conf[L"MessageArea/TextBox"];
			chatBox->setText(tv[L"testText"].asString());

			//////////////////////////////////////////////////////////////////////////

			// back
			ContainerEx* back = CreateStaticImage(conf, L"MessageArea/InputBoxBack");
			back->setPosition(back->getX() - panel->getX(), back->getY() - panel->getY());
			panel->add(back);

			// message channel
			tv = conf[L"MessageArea/ChannelList"];
			MessageChannelList* list = new MessageChannelList();
			list->setFirstIndex(0);
			list->setMaxItem(tv[L"items"].size());
			list->setWidth(tv[L"width"].asInt());
			list->setHeight(tv[L"height"].asInt() * tv[L"items"].size());
			list->setGap(gcn::Rectangle(0, 0, list->getWidth(), tv[L"height"].asInt()), 0);
			list->setPosition(tv[L"desX_2"].asInt() - panel->getX(), tv[L"desY_2"].asInt() - panel->getY());
			gcn::Rectangle rect(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt());
			for (WJson::Value::iterator i = tv[L"items"].begin(); i != tv[L"items"].end(); i++)
			{
				list->pushItem(new MessageChannelListItem(tv[L"filename"].asString(), rect, (*i).asString()));
			}
			list->setVisible(false);
			panel->add(list);

			messageChannelList = list;

			// message to
			ContainerEx* messageToBox = CreateStaticImage(conf, L"MessageArea/MessageTo/ToSomeOne");
			messageToBox->setPosition(messageToBox->getX() - panel->getX(), messageToBox->getY() - panel->getY());
			panel->add(messageToBox);
			messageToSomeOne = messageToBox;
			messageToBox->setForegroundColor(gcn::Color(255, 255, 255));

			// channel
			ContainerEx* channelBox = CreateStaticImage(conf, L"MessageArea/MessageTo/Channel");
			channelBox->setPosition(channelBox->getX() - panel->getX(), channelBox->getY() - panel->getY());
			channelBox->setForegroundColor(gcn::Color(255, 255, 255));
			//channelBox->setId("MessagePanel_Channel");
			//sora::SoraGUI::Instance()->registerGUIResponser(channelBox, this, "MessagePanel_Channel", sora::RESPONSEMOUSE);
			channelBox->addMouseListener(new LoginButton_MouseListener());
			panel->add(channelBox);
			messageChannel = channelBox;

			// input box
			tv = conf[L"MessageArea/InputBox"];
			TextFieldEx* inputBox = new TextFieldEx();
			inputBox->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			inputBox->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			inputBox->setImage(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			messageInputFont = new SoraGUIFont(L"res/msyh.ttf", tv[L"fontSize"].asInt());
			inputBox->setFont(messageInputFont);
			inputBox->setForegroundColor(tv[L"fontColor"].asUInt());
			inputBox->setPosition(inputBox->getX() - panel->getX(), inputBox->getY() - panel->getY());
			inputBox->setId("MessagePanel_InputBox");
			sora::SoraGUI::Instance()->registerGUIResponser(inputBox, this, "MessagePanel_InputBox", sora::RESPONSEACTION);
			panel->add(inputBox);
			messagePanelInputBox = inputBox;
			





			return panel;
		}

		void HouseUI::login() {
			if(!connectServer())
			{
				mgr->GetMB()->Show(L"无法连接服务器", L"错误", gcn::MessageBoxEx::TYPE_OK);
				disconnectServer();
				return;
			}
			AUTH_CLIENT.login(Base::ws2s(usernameInput->getText()),Base::ws2s(passwordInput->getText()));
		}

		void HouseUI::LoginButtonClicked()
		{
			using namespace Net;
#ifdef DIVA_GNET_OPEN
			if(!mAsyncTask)
			{
				mAsyncTask.set(Base::Function<void()>(&HouseUI::login,this));
				mAsyncTask();
			}
			/*if(!connectServer())
			{
			mgr->GetMB()->Show(L"无法连接服务器", L"错误", gcn::MessageBoxEx::TYPE_OK);
			disconnectServer();
			return;
			}
			AUTH_CLIENT.login(Base::ws2s(usernameInput->getText()),Base::ws2s(passwordInput->getText()));*/
			mgr->GetMB()->Show(L"登录中...", L"提示", gcn::MessageBoxEx::TYPE_NONE); 
			//divanet::NetworkManager::instance().auth()->send("auth#login","%s%s",Base::ws2s(usernameInput->getText()).c_str(),Base::ws2s(passwordInput->getText()).c_str());
#else
			Network::Send(L"LOGIN", usernameInput->getText() + L" " + passwordInput->getText());
#endif
		}



		void HouseUI::SetWidgetInvisible(gcn::Widget* widget)
		{
			widget->setVisible(false);
		}

		void HouseUI::MouseClicked(gcn::MouseEvent& mouseEvent)
		{
			//using namespace gcn;
			if (mouseEvent.getSource() == (gcn::Widget*) loginButton && loginButton->checkIsEnabled())
			{
				LoginButtonClicked();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) stageButton) //&& stageButton->checkIsEnabled())
			{
				//LoginButtonClicked();
				if (state == STATE_ROOM)
				{
					setState(STATE_ROOMLIST);
					//RefreshStatus();
				}
				else if (state == STATE_STAGE)
				{
					setState(STATE_ROOM);
					//RefreshStatus();
				}
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) exitStageButton && exitStageButton->checkIsEnabled())
			{
				setState(STATE_ROOM);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) messageChannel)
			{
				MessagePanelChannelClicked();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) roomListCloseButton)
			{
				setState(STATE_ROOM);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) roomListOpenButton)
			{
				//setState(STATE_STAGE);
				open_stage();
				mgr->GetMB()->Show(L"开设房间中...", L"提示", gcn::MessageBoxEx::TYPE_NONE);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) selectMusicButton)
			{
				houseGameState->beginLeave("music");
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) openGameButton)
			{
				start_game();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) readyButton)
			{
				if(STAGE_CLIENT.isReady())
					STAGE_CLIENT.unready();
				else
					STAGE_CLIENT.ready();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) playerListButton1)
			{
				int page = sPlayerList->getItemCount() / sPlayerList->getMaxItem() + 1;
				if (playerListNowPage > 0)
				{
					playerListNowPage--;
					Refresh_sPlayerList(false);
					//playerListPage->setText(iToWS(playerListNowPage + 1) + L"/" + iToWS(page)); 
				}
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) playerListButton2)
			{
				int page = sPlayerList->getItemCount() / sPlayerList->getMaxItem() + 1;
				if (playerListNowPage < page - 1)
				{
					playerListNowPage++;
					Refresh_sPlayerList(false);
					//playerListPage->setText(iToWS(playerListNowPage + 1) + L"/" + iToWS(page)); 
				}
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) exitButton)
			{
				//sora::SoraCore::Instance()->shutDown();
				exit(0);
			}
		}

		void HouseUI::SetFatherState(HouseGameState* state)
		{
			houseGameState = state;
		}

		void HouseUI::MessagePanelChannelListClicked(int index)
		{
			WJson::Value tv = conf[L"MessageArea/ChannelList"];
			if (index != -1)
			{
				//messageChannel->setText(conf[L"MessageArea/ChannelList"][L"items"][index].asString());
				if (index == 0)
					MessageChannelChange(CHANNEL_WORLD);
				else if (index == 2)
					MessageChannelChange(CHANNEL_PRIVATE);
			}
			messageChannelList->setPosition(tv[L"desX_2"].asInt() - messagePanel->getX(), tv[L"desY_2"].asInt() - messagePanel->getY());
			messageChannelList->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_1"].asInt() - messagePanel->getX(),
				tv[L"desY_1"].asInt() - messagePanel->getY()),
				tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearCos)); 
				//sora::Bind(this, &HouseUI::SetWidgetInvisible)));
			//messageChannelList->setAlpha(255);
			//messageChannelList->addModifier(new GUIAnimation_Alpha(0, tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearCos));
			UIHelper::SetUIFade(messageChannelList, 255, 0,  tv[L"animationTime"].asInt(), false);
		}

		void HouseUI::MessagePanelChannelClicked()
		{
			if (messageChannelList->isVisible())
			{
				MessagePanelChannelListClicked(-1);
				return;
			}
			WJson::Value tv = conf[L"MessageArea/ChannelList"];
			messageChannelList->setVisible(true);
			messageChannelList->setAlpha(0);
			messageChannelList->setPosition(tv[L"desX_1"].asInt() - messagePanel->getX(), tv[L"desY_1"].asInt() - messagePanel->getY());
			messageChannelList->addModifier(new GUIAnimation_Position(gcn::Point(tv[L"desX_2"].asInt() - messagePanel->getX(),
				tv[L"desY_2"].asInt() - messagePanel->getY()),
				tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));
			messageChannelList->addModifier(new GUIAnimation_Alpha(255, tv[L"animationTime"].asInt(), GUIAnimation_Float_LinearSin));
		}

		void HouseUI::MessagePanelInputBoxEnterPressed()
		{
			if (messagePanelInputBox->getText() == L"")
				return;
#ifdef DIVA_GNET_OPEN
			if (msgChannelState == CHANNEL_WORLD)
				CHAT_CLIENT.send("global", L"#W#[世界] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
			else if (msgChannelState == CHANNEL_PRIVATE)
			{
				if (msgSendId != -1)
					CHAT_CLIENT.sendTo(Base::String::any2string<int>(msgSendId), L"#P#[私聊] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
				else
					messagePanelChatBox->addText(L"[提示] 请先选择您要私聊的对象", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
			}//CHAT_CLIENT.sendTo("691",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
			//divanet::NetworkManager::instance().chat()->send("chat#sendmsg","%s%W","global",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
#else
			messagePanelChatBox->addText(PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
#endif
			messagePanelInputBox->setText(L"");

		}

		void HouseUI::action()
		{
			if (getID() == "MessagePanel_InputBox")
			{
				MessagePanelInputBoxEnterPressed();
				return;
			}
			if (getID() == "LoginWindow_UsernameInput" || getID() == "LoginWindow_PasswordInput")
			{
				LoginButtonClicked();
				return;
			}
		}

		void HouseUI::mouseClicked(const gcn::MouseEvent& mouseEvent)
		{
			if (getID() == "MessagePanel_Channel")
			{
				//MessagePanelChannelClicked();
			}
		}

		void HouseUI::RoomLInfoListClicked(int index)
		{
			MessageChannelChange(CHANNEL_PRIVATE, 
				Base::String::string2any<int>(dynamic_cast<HouseUIRoomInfoListItem*>(sPlayerList->getItems()[index])->getUid()), dynamic_cast<HouseUIRoomInfoListItem*>(sPlayerList->getItems()[index])->getText());
			//messageToSomeOne->setText(L"To " + PlayerManager::Instance()->GetStageGuests()[index].nickname);
		}

		void HouseUI::TeamListClicked(gcn::MouseEvent& mouseEvent)
		{
			int t = (int)((ButtonEx*)mouseEvent.getSource())->userData;
#ifndef DIVA_GNET_OPEN
			for (int i=0; i<teamListButtons.size(); i++)
				teamListButtons[i]->setSelected(t == i);
#else
			STAGE_CLIENT.draw(t);
#endif
		}


		// ------------------------------------ Event ----------------------------------

		using namespace gcn;

		void LoginButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->MouseClicked(mouseEvent);
		}

		void TeamSelect_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->TeamListClicked(mouseEvent);
		}
	}
}