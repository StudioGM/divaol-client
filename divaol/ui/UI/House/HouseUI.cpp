#include <WinSock2.h>
#include <windows.h>

#include "HouseUI.h"

#include <fstream>
#include "ui/Config/DivaUIConfig.h"

#include "luaclass.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

#include "ui/DivaNetwork/DivaNetwork.h"
#include "ui/Player/PlayerManager.h"
#include "AvatarListItem.h"
#include "SoraFontManager.h"

#include "HouseGameState.h"
#include "divacore/Core/DivaCore.h"
#include "divacore/Mode/DivaMultiplay.h"
#include "divacore/Mode/DivaPairPlay.h"
#include "divacore/Mode/DivaRelayPlay.h"
#include "HouseGameState.h"

#define SONICMISORA_MODIFYHYF

#include "divanetwork/DivaAuthClient.h"
#include "divanetwork/DivaNetCommand.h"
#include "divasongmgr/DivaMapManager.h"
#include "divacore/Utility/DivaSettings.h"
#include "Utility/DivaVersion.h"
#include "Lib/CSHA1/SHA1.h"
#include "divapomelo/diva/Client.h"

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
			
			isSongListAnimating = false;
			top = new gcn::Container();
			top->setSize(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
			top->setOpaque(false);

			// initialize window mgr
			mgr = new WindowMgr(top);

			// why Enter() here? it will cause Enter invoked twice.
			//Enter();

			roomTop = new gcn::WindowEx();
			roomTop->setSize(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
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
			ParseJson(L"config/uiconfig/house.json", L"config/uiconfig/stage.json", L"config/uiconfig/RoomList_PlayerList.json");

			// message Box
			gcn::MessageBoxEx* mbex = new gcn::MessageBoxEx();
			{
				WJson::Reader reader;
				WJson::Value t;
				reader.parse(ReadJsonFile(L"config/uiconfig/house/MessageBox.json"), t);
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
			shopButton->addMouseListener(new NotSupportInAlpha_MouseListener());

			// stage button
			stageButton = CreateButton(conf, L"ToolButtons/Normal/Stage_Normal", L"ToolButtons/MouseOn/Stage_MouseOn", L"ToolButtons/MouseDown/Stage_MouseDown", L"ToolButtons/Normal/Stage_Normal");
			roomTop->add(stageButton);
			stageButton->addMouseListener(new ClickButton_MouseListener());

			// option button
			optionButton = CreateButton(conf, L"ToolButtons/Normal/Option_Normal", L"ToolButtons/MouseOn/Option_MouseOn", L"ToolButtons/MouseDown/Option_MouseDown", L"ToolButtons/Normal/Option_Normal");
			roomTop->add(optionButton);
			optionButton->addMouseListener(new ClickButton_MouseListener());
			//optionButton->addMouseListener(new NotSupportInAlpha_MouseListener());

			// modify button
			modifyButton = CreateButton(conf, L"ToolButtons/Normal/Modify_Normal", L"ToolButtons/MouseOn/Modify_MouseOn", L"ToolButtons/MouseDown/Modify_MouseDown", L"ToolButtons/Normal/Modify_Normal");
			roomTop->add(modifyButton);
			modifyButton->addMouseListener(new NotSupportInAlpha_MouseListener());

			// clothes button
			clothesButton = CreateButton(conf, L"ToolButtons/Normal/Clothes_Normal", L"ToolButtons/MouseOn/Clothes_MouseOn", L"ToolButtons/MouseDown/Clothes_MouseDown", L"ToolButtons/Normal/Clothes_Normal");
			roomTop->add(clothesButton);
			clothesButton->addMouseListener(new NotSupportInAlpha_MouseListener());

			// exit button
			exitButton = CreateButton(conf, L"ToolButtons/Normal/Exit_Normal", L"ToolButtons/MouseOn/Exit_MouseOn", L"ToolButtons/MouseDown/Exit_MouseDown", L"ToolButtons/Normal/Exit_Normal");
			exitButton->addMouseListener(new ClickButton_MouseListener());
			roomTop->add(exitButton);
			

			// select music button
			selectMusicButton = CreateButton(sconf, L"ToolBar/Normal/btn_selectmusic_normal", L"ToolBar/MouseOn/btn_selectmusic_mouseon", L"ToolBar/MouseDown/btn_selectmusic_mousedown", L"ToolBar/Disabled/btn_selectmusic_disabled");
			roomTop->add(selectMusicButton);
			selectMusicButton->addMouseListener(new ClickButton_MouseListener());
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
			udButton->addMouseListener(new NotSupportInAlpha_MouseListener());

			// settings button
			settingsButton = CreateButton(sconf, L"ToolBar/Normal/btn_settings_normal", L"ToolBar/MouseOn/btn_settings_mouseon", L"ToolBar/MouseDown/btn_settings_mousedown", L"ToolBar/Normal/btn_settings_normal");
			roomTop->add(settingsButton);
			settingsButton->setVisible(false);
			settingsButton->addMouseListener(new LoginButton_MouseListener());

			// exitStageButton button
			exitStageButton = CreateButton(sconf, L"ToolBar/Normal/btn_exitstage_normal", L"ToolBar/MouseOn/btn_exitstage_mouseon", L"ToolBar/MouseDown/btn_exitstage_mousedown", L"ToolBar/Normal/btn_exitstage_normal");
			roomTop->add(exitStageButton);
			exitStageButton->setVisible(false);
			exitStageButton->addMouseListener(new LoginButton_MouseListener());

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
			openGameButton->setVisible(true);
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

			// decorate button
			decorateButton = CreateButton(sconf, L"ToolBar/Normal/btn_decorate_normal", L"ToolBar/MouseOn/btn_decorate_mouseon", L"ToolBar/MouseDown/btn_decorate_mousedown", L"ToolBar/Normal/btn_decorate_normal");
			roomTop->add(decorateButton);
			decorateButton->setVisible(false);
			decorateButton->addMouseListener(new NotSupportInAlpha_MouseListener());

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

			usernameInput->setText(Base::String(MY_PLAYER_INFO.username()));
			passwordInput->setText(L"");
			
			roomTop->setEnabled(false);

			// 

			modeButton = Helper::CreateButton(sconf[L"DefaultButton"]);
			modeButton->setPosition(sconf[L"ModeButton"][L"desX"].asInt(), sconf[L"ModeButton"][L"desY"].asInt());
			modeButton->setVisible(false);
			modeButton->setText(sconf[L"ModeButton"][L"text"].asString());
			modeButton->addMouseListener(new ClickButton_MouseListener());
			roomTop->add(modeButton);

			modeWindow = CreateModeWindow(sconf);

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

			// temp value
			WJson::Value tv;
			Helper::ReadJsonFromFile(L"config/uiconfig/house/AvatarListBox.json", tv);
			avatarList = Helper::CreateList<ListBoxEx>(tv);
			avatarList->setHorizontal(true);
			avatarList->setOutline(false);
			avatarList->setVisible(false);
			roomTop->add(avatarList);
			avatarListInfo = tv;
			for (int i = 0; i < 2; i++)
			{
				avatarList->pushItem(AvatarListItem::FromJson(tv, L"SonicMisora" + iToWS(i)));
			}
			
			// ------- setting window
			settingWindow = CreateSettingWindow();

			// ------- playing hint
			playingHint = Helper::CreateStaticImage(sconf[L"PlayingHint"]);
			roomTop->add(playingHint);
			playingHint->setVisible(false);
			
			// just for test
			//WJson::Value tv;
			//Helper::ReadJsonFromFile(L"uiconfig/house/AnimeBox.json", tv);
			//AnimeBoxEx* box = Helper::CreateAnimeBox(tv);
			//roomTop->add(box);
			//testAnimeBox = box;

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

#if defined(DIVA_USE_GNET)
		void HouseUI::observer_auth(divanet::Notification msg)
		{
			if(msg.msg()==divanet::AuthClient::NOTIFY_AUTH_REPLAY) {
				if (msg.description()=="ok")
				{
					PlayerInfo info;
					int t;
					wchar_t un[100], nm[100];

					MY_PLAYER_INFO.setUid(NET_INFO.uid);
					MY_PLAYER_INFO.loadFromNetInfo(NET_INFO.username);
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
				else if(msg.description()=="timeout")
				{
					mgr->GetMB()->Show(L"登陆超时，请重试。", L"提示");
				}
				else if(msg.description()=="unactived")
				{
					mgr->GetMB()->Show(L"账号尚未激活。",L"提示");
				}
				else if(msg.description()=="db_fail")
				{
					mgr->GetMB()->Show(L"数据库连接出错。",L"提示");
				}
				else if(msg.description()=="no_user")
				{
					mgr->GetMB()->Show(L"无此用户。",L"提示");
				}
				else
				{
					mgr->GetMB()->Show(L"尚未激活alpha测试或登录发生意外，请稍后再试。", L"提示");
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
			else if(msg.msg()==divanet::AuthClient::NOTIFY_PING_RESPONSE) {
				divanet::GPacket *packet = static_cast<divanet::GPacket*>(msg.extra());
				uint64 sendTime = packet->getItem(2)->getUInt();
				uint32 deltaMs = uint32((Base::TimeUtil::currentTime() - sendTime) * 1000 / Base::TimeUtil::resolution());
				messagePanelChatBox->addText(Base::String::format("[提示] 网络延迟 %d ms", deltaMs), gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
			}
			else if(msg.msg()==divanet::AuthClient::NOTIFY_AUTH_KICK) {
				if(msg.description()=="system")
					mgr->GetMB()->Show(L"你被管理员踢出游戏。", L"提示");
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
					msg = Base::String(gnet::ItemUtility::getString(packet->getItem(4)),true);
					gcn::Color color;
					if(msg.size() > 0) {
						if (msg[1] == L'W')
							color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"world"]);
						else if (msg[1] == L'P')
							color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"private"]);
						else if (msg[1] == L'S')
							color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"system"]);
						else if (msg[1] == L'T')
							color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"stage"]);
					}
					messagePanelChatBox->addText(msg(3, -1), color);
					break;
				}
			case divanet::ChatClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 聊天服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else if(msg.description()=="failed")
					messagePanelChatBox->addText(L"[提示] 聊天服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else if(msg.description()=="reconnect")
					messagePanelChatBox->addText(L"[提示] 尝试重新连接聊天服务器...", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
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
							info.ownerNickname = infos[i].ownerNickname;
							info.ownerID = Base::String(infos[i].ownerId);
							info.playerNum = infos[i].playernum;
							info.selectedSong.clear();
							if(infos[i].songId == 0)
								info.selectedSong.push_back(L"尚未选择");
							else
								info.selectedSong.push_back(MAPMGR.GetMapDescription(infos[i].songId,infos[i].level,infos[i].mode));
							info.stageName = infos[i].ownerNickname+"的舞台";
							//b->setInfo(info);

							roomListView->pushRoomItem(info);
						}
					}
				}
				break;
			case divanet::SchedulerClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 舞台服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else if(msg.description()=="failed")
					messagePanelChatBox->addText(L"[提示] 舞台服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else if(msg.description()=="reconnect")
					messagePanelChatBox->addText(L"[提示] 尝试重新连接舞台服务器...", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
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
					if (msg.description() == "closed")
						mgr->GetMB()->Show(L"房主离开舞台。");
					else if (msg.description() == "kicked")
						messagePanelChatBox->addText(L"[警告] 您被踢出舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
				}
				break;
			case divanet::StageClient::NOTIFY_STAGE_JOIN_RESPONSE:
				roomListView->roomCallback();
				if(msg.description()=="ok")
				{
					//StateChange_ROOMLIST_STAGE();
					if(mgr->GetMB()->isTopWindow())
						mgr->GetMB()->Destroy();
					
					MessageChannelChange(CHANNEL_STAGE);

					setState(STATE_STAGE);
					roomId = static_cast<divanet::GPacket*>(msg.extra())->getItem(2)->getString();
					if(!STAGE_CLIENT.owner())
						selectMusicButton->setEnabled(!STAGE_CLIENT.isReady());
				}
				else
					mgr->GetMB()->Show(L"该舞台还在游戏中，请稍后再试。");
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
					avatarList->pushItem(AvatarListItem::FromJson(avatarListInfo, playerInfo.nickname));
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
					if (!STAGE_CLIENT.owner())
						selectMusicButton->setEnabled(!STAGE_CLIENT.isReady());
				}
				break;
			case divanet::StageClient::NOTIFY_STAGE_LEAVE:
				{
					int index = msg.arg();
					std::wstring nickname = dynamic_cast<StageListItem*>(stageList->getItems()[index-1])->getInfo().playerInfo.nickname;
					PlayerInfo playerInfo;
					playerInfo.id = 0;
					StageListItem::StagePlayerInfo info;
					info.playerInfo = playerInfo;
					info.slot = 0;
					info.status = 0;
					info.teamIndex = 0;
					dynamic_cast<StageListItem*>(stageList->getItems()[index-1])->setInfo(info);
					for (int i = 0; i < avatarList->getItemCount(); i++)
					{
						if (dynamic_cast<AvatarListItem*>(avatarList->getItem(i))->getName() == nickname)
						{
							avatarList->removeItem(i);
							break;
						}
					}
					Refresh_sPlayerList();

					if (msg.description() == "leave")
						messagePanelChatBox->addText(L"[提示] "+nickname+L"离开了舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					else if(msg.description() == "kick")
						messagePanelChatBox->addText(L"[提示] "+nickname+L"被踢出舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				} 
				break;
			case divanet::StageClient::NOTIFY_STAGE_START:
				if(msg.description()=="start")
				{
					if(mgr->GetMB()->isTopWindow())
						mgr->CloseTopWindow();
					 
					Base::Path songPath = MAPMGR.GetDivaOLFilePath(STAGE_CLIENT.info().songId[0].songId, static_cast<divamap::DivaMap::LevelType>(STAGE_CLIENT.info().songId[0].level)); 
					if(!MAPMGR.isMapLeagal(STAGE_CLIENT.info().songId[0].songId, static_cast<divamap::DivaMap::LevelType>(STAGE_CLIENT.info().songId[0].level)))
					{
						mgr->GetMB()->Show(L"您的视听文件未通过CK大大验证！", L"提示", gcn::MessageBoxEx::TYPE_OK); 
						STAGE_CLIENT.back();
						return;
					}

					CORE_PTR->setSong(songPath.filePath().str(), songPath.fileName());
					
					CORE_PTR->setInitState("net_load");
					divacore::MultiPlay *multiplay = NULL;
					int gameMode = STAGE_CLIENT.info().songId[0].mode;
					if(gameMode == divamap::DivaMap::NormalMode)
						multiplay = new divacore::MultiPlay;
					else if(gameMode == divamap::DivaMap::RelayMode)
						multiplay = new divacore::RelayPlay;
					else
					{
						mgr->GetMB()->Show(L"尚不支持的游戏模式！", L"提示", gcn::MessageBoxEx::TYPE_OK); 
						STAGE_CLIENT.back();
						return;
					}
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
					{
						if (STAGE_CLIENT.owner())
							mgr->GetMB()->Show(L"开始失败，没有准备或非法队伍人数", L"提示", gcn::MessageBoxEx::TYPE_OK); 
						else
						{
							if(mgr->GetMB()->isTopWindow())
								mgr->CloseTopWindow();
							messagePanelChatBox->addText(L"[警告] 开始失败，没有准备或非法队伍人数", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
						}
					}
				}
				break;

			case divanet::StageClient::NOTIFY_UPDATE_INFO:
				stageList->clearItems();
				avatarList->clearItems();
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
					if (playerInfo.id != 0)
						avatarList->pushItem(AvatarListItem::FromJson(avatarListInfo, playerInfo.nickname));
				}

				{
				int color = STAGE_CLIENT.myInfo().color;
				for (int i=0; i<teamListButtons.size(); i++)
					teamListButtons[i]->setSelected(color == i);
				}
				MAPMGR.SelectedMode_Set(STAGE_CLIENT.info().hooks);
				ModeButtonRefresh();

				Refresh_sPlayerList();
				
				STAGE_CLIENT.refreshMusic();
				//Refresh_SongList();
				BeginSongListAnimation();

				messagePanelChatBox->addText(L"[提示] 更新房间信息", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				if (!STAGE_CLIENT.owner())
					selectMusicButton->setEnabled(!STAGE_CLIENT.isReady());
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
					//Refresh_SongList();
					BeginSongListAnimation();
					if(STAGE_CLIENT.isReady())
						STAGE_CLIENT.unready();
				}
				messagePanelChatBox->addText(L"[提示] 房主更改了歌曲列表", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::StageClient::NOTIFY_REFRESH_SONG_UI:
				//Refresh_SongList();
				BeginSongListAnimation();
				messagePanelChatBox->addText(L"[提示] 歌曲列表刷新", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));				
				break;
			case divanet::StageClient::NOTIFY_UPDATE_HOOK:
				MAPMGR.SelectedMode_Set(STAGE_CLIENT.info().hooks);
				ModeButtonRefresh();
				if (!STAGE_CLIENT.owner())
				{
					modeButton->setForegroundColor(gcn::Color(255, 0, 0, modeButton->getAlpha()));
					if(STAGE_CLIENT.isReady())
						STAGE_CLIENT.unready();
				}
				messagePanelChatBox->addText(L"[提示] 房主更改了游戏模式", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::StageClient::NOTIFY_STAGE_LEAVE_RESPONSE:
				break;
			case divanet::StageClient::NOTIFY_CONNECT:
				if(msg.description()=="ok")
					messagePanelChatBox->addText(L"[提示] 游戏服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				else if(msg.description()=="failed")
					messagePanelChatBox->addText(L"[警告] 游戏服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
				else if(msg.description()=="reconnect")
					messagePanelChatBox->addText(L"[提示] 尝试重新连接游戏服务器...", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				break;
			case divanet::StageClient::NOTIFY_TIMEOUT:
				messagePanelChatBox->addText(L"[警告] 游戏服务器断开连接..", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
				// 返回主舞台
				setState(STATE_ROOM);
				break;
			case divanet::StageClient::NOTIFY_STAGE_RETURN:
				//if(mgr->GetMB()->isTopWindow())
				//	mgr->CloseTopWindow();
				//mgr->GetMB()->Show(L"开始游戏失败", L"提示", gcn::MessageBoxEx::TYPE_OK);
				if(msg.description() == "start_failed")
					messagePanelChatBox->addText(L"[警告] 开始游戏失败!", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
				else if(msg.description() == "game_over")
				{
					if(STAGE_CLIENT.state() == divanet::StageClient::GAME)
					{
						if(STAGE_CLIENT.getPlayerNum() > 1)
							messagePanelChatBox->addText(L"[提示] 其他玩家尚未退出游戏，请稍等...", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						setState(STATE_PLAYING);
					}
				}
				break;
			case divanet::StageClient::NOTIFY_GAME_OVER:
				messagePanelChatBox->addText(L"[提示] 游戏结束!", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				if(state==STATE_PLAYING)
					setState(STATE_STAGE);
				break;
			}
		}
#endif

		void HouseUI::observer_pomelo(Base::Notification bundle) {
			std::string route = bundle.description();
			int status = bundle.arg();
			void *extra = bundle.extra();
			Json::Value msg(NULL);

			if (extra != NULL)
				msg = *static_cast<Json::Value*>(extra);

			switch(bundle.msg()) {
				case divapomelo::ON_CONNECT:
					if (status == 0) {
						messagePanelChatBox->addText(L"[提示] 服务器连接成功", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						POMELO_CLIENT.login(usernameInput->getText(), passwordInput->getText());
					}
					else {
						if(mgr->GetMB()->isTopWindow())
							mgr->GetMB()->Destroy();
						messagePanelChatBox->addText(L"[提示] 服务器连接失败", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						mgr->GetMB()->Show(L"无法连接服务器", L"错误", gcn::MessageBoxEx::TYPE_OK);
					}
					break;
				case divapomelo::ON_LOGIN:
					if (status == 0)
					{
						PlayerInfo info;
						int t;
						wchar_t un[100], nm[100];

						MY_PLAYER_INFO.setUid(POMELO_USER_INFO.uid);
						MY_PLAYER_INFO.loadFromNetInfo(POMELO_USER_INFO.username);
						info.id = Base::String::string2any<int>(POMELO_USER_INFO.uid);
						info.username = Base::String(POMELO_USER_INFO.username);
						info.nickname = Base::String(POMELO_USER_INFO.nickname);
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

						//divanet::NetworkManager::instance().core()->send("auth#setuid","%s",MY_PLAYER_INFO.uid().c_str());
						messagePanelChatBox->addText(("[系统] 欢迎您, "+POMELO_USER_INFO.nickname).unicode_str(), gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"system"]));
					}
					else if(status == 1) {
						std::string code = msg.asString();

						if(mgr->GetMB()->isTopWindow())
							mgr->GetMB()->Destroy();
						if(code=="auth_already_login")
						{
							mgr->GetMB()->Show(L"该账号已经登录。", L"提示");
						}
						else if(code=="auth_wrong_pwd")
						{
							mgr->GetMB()->Show(L"账号或密码错误。", L"提示");
						}
						else if(code=="auth_timeout")
						{
							mgr->GetMB()->Show(L"登陆超时，请重试。", L"提示");
						}
						else if(code=="auth_not_activated")
						{
							mgr->GetMB()->Show(L"账号尚未激活。",L"提示");
						}
						else if(code=="auth_conn_db_fail")
						{
							mgr->GetMB()->Show(L"数据库连接出错。",L"提示");
						}
						else if(code=="auth_no_user")
						{
							mgr->GetMB()->Show(L"无此用户。",L"提示");
						}
						else if(code=="auth_fail_network")
						{
							mgr->GetMB()->Show(L"网络错误。",L"提示");
						}
						else
						{
							mgr->GetMB()->Show(L"尚未激活alpha测试或登录发生意外，请稍后再试。", L"提示");
						}
					}
					else {
						mgr->GetMB()->Show(L"服务器连接发生意外错误。", L"提示");
					}

					if (status != 0) {
						disconnectServer();
					}
					break;
				case divapomelo::ON_CLOSE:
					if (state != STATE_LOGINWINDOW) {
						mgr->GetMB()->RegisterCallback(MessageBoxEx::Callback(&HouseUI::cb_connect_break, this));
						mgr->GetMB()->Show(L"网络连接中断。",L"提示");
						disconnectServer();
					}
					break;
				case divapomelo::ON_CHAT:
					if (status == 0) {
						Base::String content(msg["content"].asString(), true);

						gcn::Color color;
						if(content.size() > 0) {
							if (content[1] == L'W')
								color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"world"]);
							else if (content[1] == L'P')
								color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"private"]);
							else if (content[1] == L'S')
								color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"system"]);
							else if (content[1] == L'T')
								color = gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"stage"]);
						}
						messagePanelChatBox->addText(content(3, -1), color);
					}
					break;
				case divapomelo::PUSH_LOBBY_GETSTAGELIST:
					if (status == 0) {
						roomListView->clearItems();
						const divapomelo::RoomInfos &infos = POMELO_LOBBY_PEER->getRoomList();
						for(int i = 0; i < infos.size(); i++)
						{
							if(infos[i].state==divapomelo::RoomInfo::STAGE) {
								//RoomListItem* b = SetRoomListItemInfo(rconf,L"RoomList/RoomItem_normal", L"RoomList/RoomItem_on", L"RoomList/RoomItem_down");
								Network::RoomInfo info;
								info.maxPlayerNum = infos[i].capacity;
								info.ownerNickname = infos[i].ownerNickname;
								info.ownerID = Base::String(infos[i].ownerId);
								info.playerNum = infos[i].playernum;
								info.selectedSong.clear();
								info.stageId = infos[i].stageId;
								if(infos[i].songId == 0)
									info.selectedSong.push_back(L"尚未选择");
								else
									info.selectedSong.push_back(MAPMGR.GetMapDescription(infos[i].songId,infos[i].level,infos[i].mode));
								info.stageName = infos[i].ownerNickname+"的舞台";
								//b->setInfo(info);

								roomListView->pushRoomItem(info);
							}
						}
					}
					break;
				case divapomelo::ON_STAGE_CLOSE:
					if(state==STATE_STAGE) {
						setState(STATE_ROOM);

						//!FIXME : here use MB will cause black screen when exit game with stage closed
						/*if(mgr->GetMB()->isTopWindow())
							mgr->GetMB()->Destroy();
						mgr->GetMB()->Show(L"房主离开舞台。");*/
						messagePanelChatBox->addText(L"[提示] 房主离开舞台。", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					}
					break;
				case divapomelo::PUSH_LOBBY_CREATESTAGE:
				case divapomelo::PUSH_LOBBY_JOINSTAGE:
					roomListView->roomCallback();
					if (status == 0) {
						//StateChange_ROOMLIST_STAGE();
						if(mgr->GetMB()->isTopWindow())
							mgr->GetMB()->Destroy();
					
						MessageChannelChange(CHANNEL_STAGE);

						setState(STATE_STAGE);
						roomId = POMELO_STAGE_PEER->getRoomID();
						if(!POMELO_STAGE_PEER->owner())
							selectMusicButton->setEnabled(!POMELO_STAGE_PEER->getIsReady());

						_onRefreshStageInfo();
					}
					else {
						if(mgr->GetMB()->isTopWindow())
							mgr->GetMB()->Destroy();
						mgr->GetMB()->Show(L"该舞台还在游戏中，请稍后再试。");
					}
					break;
				case divapomelo::ON_STAGE_JOIN:
					if (status == 0) {
						int index = msg["slot"].asInt();
						PlayerInfo playerInfo;
						playerInfo.id = Base::String::string2any<int>(POMELO_STAGE_PEER->getInfo().waiters[index].uid);
						playerInfo.nickname = POMELO_STAGE_PEER->getInfo().waiters[index].nickname;
						StageListItem::StagePlayerInfo info;
						info.playerInfo = playerInfo;
						info.slot = index+1;
						info.status = 0;
						info.teamIndex = 0;
						dynamic_cast<StageListItem*>(stageList->getItems()[index])->setInfo(info);
						avatarList->pushItem(AvatarListItem::FromJson(avatarListInfo, playerInfo.nickname));
						//mgr->GetMB()->Destroy();
						Refresh_sPlayerList();

						messagePanelChatBox->addText(L"[提示] "+playerInfo.nickname+L"加入了舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					}
					break;
				case divapomelo::ON_STAGE_LEAVE:
					if (status == 0) {
						int index = msg["slot"].asInt();
						std::wstring nickname = dynamic_cast<StageListItem*>(stageList->getItems()[index])->getInfo().playerInfo.nickname;
						PlayerInfo playerInfo;
						playerInfo.id = 0;
						StageListItem::StagePlayerInfo info;
						info.playerInfo = playerInfo;
						info.slot = 0;
						info.status = 0;
						info.teamIndex = 0;
						dynamic_cast<StageListItem*>(stageList->getItems()[index])->setInfo(info);
						for (int i = 0; i < avatarList->getItemCount(); i++)
						{
							if (dynamic_cast<AvatarListItem*>(avatarList->getItem(i))->getName() == nickname)
							{
								avatarList->removeItem(i);
								break;
							}
						}
						Refresh_sPlayerList();

						if (msg["reason"].asString() != "kick")
							messagePanelChatBox->addText(L"[提示] "+nickname+L"离开了舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						else
							messagePanelChatBox->addText(L"[提示] "+nickname+L"被踢出舞台！", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					}
					break;
				case divapomelo::ON_STAGE_KICK:
					if(state==STATE_STAGE) {
						setState(STATE_ROOM);
						mgr->GetMB()->Show(L"您被踢出舞台。");
					}
					break;
				case divapomelo::ON_STAGE_ALLINFO:
					_onRefreshStageInfo();
					break;
				case divapomelo::ON_STAGE_READY:
				case divapomelo::ON_STAGE_UNREADY:
					if (status == 0) {
						int index = msg["slot"].asInt();
						StageListItem* item = dynamic_cast<StageListItem*>(stageList->getItems()[index]);
						if(bundle.msg() == divapomelo::ON_STAGE_READY)
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
						if (!POMELO_STAGE_PEER->owner())
							selectMusicButton->setEnabled(!POMELO_STAGE_PEER->getIsReady());
					}
					break;
				case divapomelo::ON_STAGE_DRAW:
					if (status == 0) {
						int index = msg["slot"].asInt();
						if(POMELO_STAGE_PEER->isMe(index))
						{
							int color = POMELO_STAGE_PEER->myInfo().color;
							for (int i=0; i<teamListButtons.size(); i++)
								teamListButtons[i]->setSelected(color == i);
						}
						int color = (POMELO_STAGE_PEER->getInfo().waiters[index]).color;
						((StageListItem*)(stageList->getItems()[index]))->setTeamColor(color);

						//!FIXME: I don't know whether the sentence should be executed or not.
						//Refresh_sPlayerList();
					}
					break;
				case divapomelo::ON_STAGE_SETSONG:
					if (status == 0) {
						if(!POMELO_STAGE_PEER->owner()) 
						{
							POMELO_STAGE_PEER->refreshMusic();
							//Refresh_SongList();
							BeginSongListAnimation();
							if(POMELO_STAGE_PEER->getIsReady())
								POMELO_STAGE_PEER->unready();
						}
						messagePanelChatBox->addText(L"[提示] 房主更改了歌曲列表", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					}
					break;
				case divapomelo::LOCAL_UPDATE_SONG_UI:
					BeginSongListAnimation();
					messagePanelChatBox->addText(L"[提示] 歌曲列表刷新", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));				
					break;
				case divapomelo::ON_STAGE_SETHOOK:
					if (status == 0) {
						MAPMGR.SelectedMode_Set(POMELO_STAGE_PEER->getInfo().hooks);
						ModeButtonRefresh();
						if (!POMELO_STAGE_PEER->owner())
						{
							modeButton->setForegroundColor(gcn::Color(255, 0, 0, modeButton->getAlpha()));
							if(POMELO_STAGE_PEER->getIsReady())
								POMELO_STAGE_PEER->unready();
						}
						messagePanelChatBox->addText(L"[提示] 房主更改了游戏模式", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					}
					break;
				case divapomelo::ON_STAGE_START:
					if (status == 0) {
						if (msg["flag"].asBool()) {
							if(mgr->GetMB()->isTopWindow())
								mgr->CloseTopWindow();
					 
							Base::Path songPath = MAPMGR.GetDivaOLFilePath(POMELO_STAGE_PEER->getInfo().song[0].songId, static_cast<divamap::DivaMap::LevelType>(POMELO_STAGE_PEER->getInfo().song[0].level)); 
							if(!MAPMGR.isMapLeagal(POMELO_STAGE_PEER->getInfo().song[0].songId, static_cast<divamap::DivaMap::LevelType>(POMELO_STAGE_PEER->getInfo().song[0].level)))
							{
								mgr->GetMB()->Show(L"您的视听文件未通过CK大大验证！", L"提示", gcn::MessageBoxEx::TYPE_OK); 
								POMELO_STAGE_PEER->back();
								return;
							}

							CORE_PTR->setSong(songPath.filePath().str(), songPath.fileName());
					
							CORE_PTR->setInitState("net_load");
							divacore::MultiPlay *multiplay = NULL;
							int gameMode = POMELO_STAGE_PEER->getInfo().song[0].mode;

							multiplay = new divacore::MultiPlay;
							if(gameMode == divamap::DivaMap::NormalMode)
								multiplay = new divacore::MultiPlay;
							else if(gameMode == divamap::DivaMap::RelayMode)
								multiplay = new divacore::RelayPlay;
							else
							{
								mgr->GetMB()->Show(L"尚不支持的游戏模式！", L"提示", gcn::MessageBoxEx::TYPE_OK); 
								POMELO_STAGE_PEER->back();
								return;
							}

							CORE_PTR->registerGameMode(multiplay);
							multiplay->registerNetworkEvent();
							NextState = "core";
						}
						else {
							Base::String info = msg["reason"].asString();

							if(mgr->GetMB()->isTopWindow())
								mgr->CloseTopWindow();
							if(info=="not_select_song")
								mgr->GetMB()->Show(L"开始失败，没有选择歌曲", L"提示", gcn::MessageBoxEx::TYPE_OK); 
							else
							{
								if (POMELO_STAGE_PEER->owner()) {
									mgr->GetMB()->Show(L"开始失败，没有准备或非法队伍人数", L"提示", gcn::MessageBoxEx::TYPE_OK); 
								}
								else
								{
									messagePanelChatBox->addText(L"[警告] 开始失败，没有准备或非法队伍人数", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
								}
							}
						}
					}
				case divapomelo::PUSH_STAGE_START:
					if(status == 1) {
						//mgr->GetMB()->Show(Base::String("开始失败，服务器错误:" + msg.asString()), L"提示", gcn::MessageBoxEx::TYPE_OK); 
					}
					else if(status != 0) {
						mgr->GetMB()->Show(Base::String("开始失败，未知错误"), L"提示", gcn::MessageBoxEx::TYPE_OK); 
					}
					break;
				case divapomelo::ON_STAGE_RETURN:
					if (status == 0) {
						_onRefreshStageInfo();
						//if(mgr->GetMB()->isTopWindow())
						//	mgr->CloseTopWindow();
						//mgr->GetMB()->Show(L"开始游戏失败", L"提示", gcn::MessageBoxEx::TYPE_OK);
						if(msg["phase"] == "loading") {
							messagePanelChatBox->addText(L"[警告] 开始游戏失败!", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"warning"]));
							if(state==STATE_PLAYING)
								setState(STATE_STAGE);
						}
						else
						{
							messagePanelChatBox->addText(L"[提示] 游戏结束!", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
							if(state==STATE_PLAYING)
								setState(STATE_STAGE);
						}
						break;
					}
					break;
				case divapomelo::PUSH_GAME_BACK:
					if (status == 0) {
						if(!msg["over"].isBool()) {
							messagePanelChatBox->addText(L"[提示] 其他玩家尚未退出游戏，请稍等...", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						}
						if (state == STATE_STAGE)
							setState(STATE_PLAYING);
					}
					break;
			}
		}

		void HouseUI::_onRefreshStageInfo() {
			stageList->clearItems();
			avatarList->clearItems();
			for(int i = 0; i < POMELO_STAGE_PEER->getInfo().waiters.size(); i++)
			{
				//for (int i=1; i<=8; i++)
				StageListItem *item = new StageListItem(rconf[L"PlayerList/playerItem_back"][L"filename"].asString(), GetRect(rconf[L"PlayerList/playerItem_back"]));
				PlayerInfo playerInfo;
				playerInfo.id = Base::String::string2any<int>(POMELO_STAGE_PEER->getInfo().waiters[i].uid);
				playerInfo.nickname = POMELO_STAGE_PEER->getInfo().waiters[i].nickname;
				StageListItem::StagePlayerInfo info;
				info.playerInfo = playerInfo;
				info.slot = i+1;
				info.status = POMELO_STAGE_PEER->getInfo().waiters[i].status == divapomelo::WaiterInfo::READY;
				info.teamIndex = 0;
				item->setInfo(info);
				item->setTeamColor(POMELO_STAGE_PEER->getInfo().waiters[i].color);
				stageList->pushItem(item);
				if (playerInfo.id != 0)
					avatarList->pushItem(AvatarListItem::FromJson(avatarListInfo, playerInfo.nickname));
			}

			{
			int color = POMELO_STAGE_PEER->myInfo().color;
			for (int i=0; i<teamListButtons.size(); i++)
				teamListButtons[i]->setSelected(color == i);
			}
			MAPMGR.SelectedMode_Set(POMELO_STAGE_PEER->getInfo().hooks);
			ModeButtonRefresh();

			Refresh_sPlayerList();
				
			POMELO_STAGE_PEER->refreshMusic();
			//Refresh_SongList();
			BeginSongListAnimation();

			messagePanelChatBox->addText(L"[提示] 更新房间信息", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
			if (!POMELO_STAGE_PEER->owner())
				selectMusicButton->setEnabled(!POMELO_STAGE_PEER->getIsReady());
		}

		void HouseUI::attachObserver() {
#if defined(DIVA_USE_GNET)
			AUTH_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_auth,this));
			CHAT_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_chat,this));
			SCHEDULER_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_scheduler,this));
			STAGE_CLIENT.attachObserver(divanet::Observer(&HouseUI::observer_stage,this));
#else
			POMELO_CLIENT.attachObserver(Base::Observer(&HouseUI::observer_pomelo, this));
#endif
		}

		void HouseUI::detachObserver() {
#if defined(DIVA_USE_GNET)
			AUTH_CLIENT.clearObserver();
			CHAT_CLIENT.clearObserver();
			SCHEDULER_CLIENT.clearObserver();
			STAGE_CLIENT.clearObserver();
#endif
		}

		bool HouseUI::connectServer() {
#if defined(DIVA_USE_GNET)
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
#else
			POMELO_CLIENT.connect();
			return true;
#endif
		}
		void HouseUI::disconnectServer() {
#if defined(DIVA_USE_GNET)
			AUTH_CLIENT.disconnect();
			CHAT_CLIENT.disconnect();
			SCHEDULER_CLIENT.disconnect();
			STAGE_CLIENT.disconnect();
#else
			POMELO_CLIENT.disconnect();
#endif
		}
		void HouseUI::request_roomList() {
#if defined(DIVA_USE_GNET)
			//SCHEDULER_CLIENT.updateRoomList();
#else
			POMELO_LOBBY_PEER->getStageList();
#endif
			//divanet::NetworkManager::instance().scheduler()->send("scheduler#roomlist");

		}
		void HouseUI::cb_connect_break() {
			mgr->GetMB()->RegisterCallback();
			exit(0);
		}
		void HouseUI::cb_exit_game() {
			mgr->GetMB()->RegisterCallback();
			if (mgr->GetMB()->GetResult() == MessageBoxEx::RES::RES_YES)
			{
				logout();
				exit(0);
			}
		}
		void HouseUI::cb_exit_stage() {
			mgr->GetMB()->RegisterCallback();
			if (mgr->GetMB()->GetResult() == MessageBoxEx::RES::RES_YES)
				setState(STATE_ROOM);
		}
		void HouseUI::cb_kick_player() {
			mgr->GetMB()->RegisterCallback();
			if (mgr->GetMB()->GetResult() == MessageBoxEx::RES::RES_YES)
				kick_player(stageList->getSelectedIndex());
		}
		void HouseUI::open_stage() {
#if defined(DIVA_USE_GNET)
			STAGE_CLIENT.create(8);
			//divanet::NetworkManager::instance().core()->send("stage#create","%d",2);
			roomId = MY_PLAYER_INFO.uid();
#else
			POMELO_STAGE_PEER->create();
			roomId = MY_PLAYER_INFO.uid();
#endif
		}
		void HouseUI::start_game() {
#if defined(DIVA_USE_GNET)
			if(STAGE_CLIENT.start())
				mgr->GetMB()->Show(L"准备开始游戏...", L"提示", gcn::MessageBoxEx::TYPE_NONE); 
#else
			if(POMELO_STAGE_PEER->start())
				mgr->GetMB()->Show(L"准备开始游戏...", L"提示", gcn::MessageBoxEx::TYPE_NONE); 
#endif
		}
		void HouseUI::leave_stage() {
#if defined(DIVA_USE_GNET)
			STAGE_CLIENT.leave();
			//divanet::NetworkManager::instance().core()->send("stage#leave");
#else
			POMELO_STAGE_PEER->leave();
#endif
		}
		void HouseUI::kick_player(int index)
		{
			if (index > 0 && index < stageList->getItemCount())
			{
				StageListItem *item = dynamic_cast<StageListItem*>(stageList->getItem(index));
				if (item->getInfo().playerInfo.id == 0)
					return;
				
#if defined(DIVA_USE_GNET)
				STAGE_CLIENT.kick(item->getInfo().playerInfo.id);
#else
				POMELO_STAGE_PEER->kick(Base::String::any2string(item->getInfo().playerInfo.id));
#endif
			}
		}
		void HouseUI::Enter()
		{
			sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
			top->setVisible(true);
			top->setEnabled(true);
			//Refresh_SongList();
			BeginSongListAnimation();
			sora::SoraBGMManager::Instance()->play(config[L"lobbyMusicFilename"].asString(), false, false);
			sora::SoraBGMManager::Instance()->play(config[L"lobbyLoopMusicFilename"].asString(), true, true);
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
					if (t.error)
					{
						messagePanelChatBox->addText(L"[提示] 与歌曲列表服务器连接发生错误。", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						//throw "fuck it!";
						break;
					}
					if (t.finish)
					{
						messagePanelChatBox->addText(L"[提示] 与歌曲列表服务器连接成功，歌曲列表已更新。", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						selectMusicButton->setEnabled(true);
						break;
					}
					break;
				case divamap::DivaMapEventMessage::UnpackMapDataFile :
					if (t.error)
					{
						messagePanelChatBox->addText(L"[提示] 您的歌曲【" + MAPS[t.effectedMapID].header.name + L"】下载发生错误。", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
						break;
					}
					if (t.finish)
						messagePanelChatBox->addText(L"[提示] 您的歌曲【" + MAPS[t.effectedMapID].header.name + L"】已经下载完成。", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
					break;
				}
				q->pop_front();
			}

			/*if(state==STATE_ROOMLIST) {
				BASE_PER_PERIOD_BEGIN(dt,10.0);
				
				Refresh_RoomList(true);

				BASE_PER_PERIOD_END();
			}*/

			if (state == STATE_STAGE)
			{
				for (int i = 0; i < avatarList->getItemCount(); i++)
					dynamic_cast<AvatarListItem*>(avatarList->getItem(i))->update(dt);
			}

			if (roomListView->isWaitingRoomCallback())
				roomListView->waiting();

			//testAnimeBox->update(dt);
		}

		void HouseUI::RecvMsg()
		{
#if !defined(DIVA_USE_GNET) && !defined(DIVA_USE_POMELO)
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

		void HouseUI::EndSongListAnimation(gcn::Widget* ui)
		{
			Refresh_SongList();
			isSongListAnimating = false;
			PointEx desP = Helper::GetPoint(songListInfo[L"oriDesPos"]);
			songListPanel->addModifier(new GUIAnimation_Position(gcn::Point(desP.x,
				desP.y),
				songListInfo[L"animeTime"].asInt(), GUIAnimation_Float_LinearSin));
		}

		void HouseUI::BeginSongListAnimation()
		{
			if (isSongListAnimating)
				return;
			isSongListAnimating = true;
			PointEx desP = Helper::GetPoint(songListInfo[L"animeDesPos"]);
			songListPanel->addModifier(new GUIAnimation_Position(gcn::Point(desP.x,
				desP.y),
				songListInfo[L"animeTime"].asInt(), GUIAnimation_Float_LinearSin, gcn::NONE, NULL, sora::Bind(this, &HouseUI::EndSongListAnimation) ));
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
					gcn::iToWS(m.levels[i->level].difficulty) + L",BPM:" + gcn::iToWS(m.header.bpm) + L","+config[L"gameModeNames"][(*i).mode].asString()+L")");
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
			avatarList->setVisible(false);
			//songList->setVisible(false);
			modeButton->setVisible(false);
			openGameButton->setVisible(true);
			readyButton->setVisible(false);

			MessageChannelChange(CHANNEL_WORLD);
			selectMusicButton->setEnabled(true);
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

		void HouseUI::_ClearLoginInfo()
		{
			passwordInput->setText(L"");
			messagePanelChatBox->clearText();
			MAPMGR.SelectedMap_Clear();
			Refresh_SongList();
			PlayerManager::Instance()->ClearHostInfo();
			hostInfoLabel->setText(L"");
		}

		void HouseUI::StateChange_ROOM_LOGINWINDOW()
		{
			state = STATE_LOGINWINDOW;
			_ClearLoginInfo();
			loginPanel->FadeIn(conf[L"Login/Config"][L"inTime"].asInt());
			mgr->OpenWindow(loginPanel, conf[L"Login/Config"][L"blackAlpha"].asInt());
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
			//songList->setVisible(true);
			modeButton->setVisible(true);
			avatarList->setVisible(true);
#if defined(DIVA_USE_GNET)
			if (STAGE_CLIENT.owner())
#else
			if (POMELO_STAGE_PEER->owner())
#endif
			{
				openGameButton->setVisible(true);
				readyButton->setVisible(false);
			}
			else
			{
				openGameButton->setVisible(false);
				readyButton->setVisible(true);
			}
			//
			//roomListPanel->setVisible(false);
			//roomTop->setEnabled(true);
			roomListPanel->FadeOut(10);
			Refresh_sPlayerList();
			MessageChannelChange(CHANNEL_STAGE);
		}

		void HouseUI::StateChange_STAGE_PLAYING()
		{
			state = STATE_PLAYING;

			udButton->setEnabled(false);
			selectMusicButton->setEnabled(false);
			decorateButton->setEnabled(false);
			thingList->setEnabled(false);
			teamList->setEnabled(false);
			modeButton->setEnabled(false);
			openGameButton->setEnabled(false);
			readyButton->setEnabled(false);

			playingHint->setVisible(true);
		}

		void HouseUI::StateChange_PLAYING_STAGE()
		{
			state = STATE_STAGE;

			udButton->setEnabled(true);
			selectMusicButton->setEnabled(true);
			decorateButton->setEnabled(true);
			thingList->setEnabled(true);
			teamList->setEnabled(true);
			modeButton->setEnabled(true);
			openGameButton->setEnabled(true);
			readyButton->setEnabled(true);

			playingHint->setVisible(false);
		}

		void HouseUI::StateChange_PLAYING_ROOM()
		{
			StateChange_STAGE_ROOM();

			udButton->setEnabled(true);
			selectMusicButton->setEnabled(true);
			decorateButton->setEnabled(true);
			thingList->setEnabled(true);
			teamList->setEnabled(true);
			modeButton->setEnabled(true);
			openGameButton->setEnabled(true);
			readyButton->setEnabled(true);

			playingHint->setVisible(false);
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
			if (state == STATE_ROOM && des == STATE_LOGINWINDOW)
			{
				StateChange_ROOM_LOGINWINDOW();
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
			if (state == STATE_STAGE && des == STATE_PLAYING)
			{
				
				StateChange_STAGE_PLAYING();
				return;
			}
			if (state == STATE_PLAYING && des == STATE_STAGE)
			{
				
				StateChange_PLAYING_STAGE();
				return;
			}
			if (state == STATE_PLAYING && des == STATE_ROOM)
			{
				
				StateChange_PLAYING_ROOM();
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

		void HouseUI::BeginLeave()
		{
			top->setEnabled(false);
		}

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
			lb->addMouseListener(new ClickButton_MouseListener());
			loginButton = lb;
			//lb->setBaseColor(gcn::Color(255, 0, 0));

			SuperButtonEx* closeButton = CreateButton(conf, L"Login/btn_close/btn_close_normal", L"Login/btn_close/btn_close_on",
				L"Login/btn_close/btn_close_down", L"Login/btn_close/btn_close_normal");
			con->add(closeButton, closeButton->getX() - topX, closeButton->getY() - topY);
			//closeButton->setPosition(800,100);
			closeButton->addMouseListener(new LoginButton_MouseListener());
			loginCloseButton = closeButton;

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
			openButton->addMouseListener(new ClickButton_MouseListener());
			panel->add(openButton);
			roomListOpenButton = openButton;

			//////////////////////////////////////////////////////////////////////////

			openButton = CreateButton(conf, L"RoomList/btn_openStage/btn_refreshStage_normal", L"RoomList/btn_openStage/btn_refreshStage_on",
				L"RoomList/btn_openStage/btn_refreshStage_down", L"RoomList/btn_openStage/btn_refreshStage_normal");
			openButton->setPosition(openButton->getX() - panel->getX(), openButton->getY() - panel->getY());
			openButton->setText(tv[L"refreshStageText"].asString());
			openButton->addMouseListener(new ClickButton_MouseListener());
			panel->add(openButton);
			roomListRefreshButton = openButton;
			
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
				info.ownerNickname = L"拥有者";
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
			else if (ch == CHANNEL_STAGE)
			{
				messageChannel->setText(conf[L"MessageChannels"][L"stage"].asString());
				messageToSomeOne->setText(L"");
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
				PointEx oriP = Helper::GetPoint(songListInfo[L"oriDesPos"]);
				songListImage->setPosition(t[L"desX"].asInt() - oriP.x, t[L"desY"].asInt() - oriP.y);

			}
		}

		gcn::SelectorEx* HouseUI::CreateSelector(const WJson::Value& sconf, const WJson::Value& dconf, const WJson::Value& sizeConf)
		{
			SelectorEx* resSelector = SelectorEx::FromJson(sconf);
			SpecialItemDisplayer* displayer = new SpecialItemDisplayer();
			for (WJson::Value::iterator i = dconf.begin(); i != dconf.end(); i++)
			{
				displayer->pushItem((*i).asString());
			}
			resSelector->setDisplayer(displayer);
			resSelector->setDimension(Helper::GetRect(sizeConf));
			resSelector->adjustButtonPos();
			//win->add(resSelector);
			return resSelector;
		}

		gcn::SelectorEx* HouseUI::CreateResolutionSelector(const WJson::Value& sconf, const WJson::Value& dconf, const WJson::Value& sizeConf)
		{
			SelectorEx* resSelector = SelectorEx::FromJson(sconf);
			SpecialItemDisplayer* displayer = new SpecialItemDisplayer();
			for (WJson::Value::iterator i = dconf.begin(); i != dconf.end(); i++)
			{
				Base::String str = (*i).asString();
				int width = Base::String::string2any<int>(str(0, str.find("*")));
				int height = Base::String::string2any<int>(str(str.find("*")+1, -1));
				
				if(width <= SETTINGS.getScreenWidth() && height <= SETTINGS.getScreenHeight())
					displayer->pushItem((*i).asString());
			}
			resSelector->setDisplayer(displayer);
			resSelector->setDimension(Helper::GetRect(sizeConf));
			resSelector->adjustButtonPos();
			//win->add(resSelector);
			return resSelector;
		}

		void HouseUI::SaveSettings()
		{
			// first update config
			setConfig[L"resolution"] = ((SpecialItemDisplayer*)setResSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"musicVolume"] = ((SpecialItemDisplayer*)setMvSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"seVolume"] = ((SpecialItemDisplayer*)setSevSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"isWindowMode"] = ((SpecialItemDisplayer*)setWindowModeSelector->getDisplayer())->getSelectedIndex() == 0;
			setConfig[L"uiMod"] = ((SpecialItemDisplayer*)setUiModSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"language"] = ((SpecialItemDisplayer*)setLanSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"particleSystem"] = ((SpecialItemDisplayer*)setPsSelector->getDisplayer())->getSelectedIndex();
			setConfig[L"isAutoCT"] = ((SpecialItemDisplayer*)setAutoCtSelector->getDisplayer())->getSelectedIndex() == 0;

			// save config with specific items (without window width&height)
			WJson::Value saveSetting = WJson::Value();
			saveSetting[L"resolution"] = ((SpecialItemDisplayer*)setResSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"musicVolume"] = ((SpecialItemDisplayer*)setMvSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"seVolume"] = ((SpecialItemDisplayer*)setSevSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"isWindowMode"] = ((SpecialItemDisplayer*)setWindowModeSelector->getDisplayer())->getSelectedIndex() == 0;
			saveSetting[L"uiMod"] = ((SpecialItemDisplayer*)setUiModSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"language"] = ((SpecialItemDisplayer*)setLanSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"particleSystem"] = ((SpecialItemDisplayer*)setPsSelector->getDisplayer())->getSelectedIndex();
			saveSetting[L"gameWidth"] = setConfig[L"gameWidth"].asInt();
			saveSetting[L"gameHeight"] = setConfig[L"gameHeight"].asInt();
			saveSetting[L"globalFont"] = setConfig[L"globalFont"].asString();
			saveSetting[L"screenFadeTime"] = setConfig[L"screenFadeTime"].asDouble();
			saveSetting[L"bgmFadeTime"] = setConfig[L"bgmFadeTime"].asDouble();
			saveSetting[L"isAutoCT"] = setConfig[L"isAutoCT"].asBool();

			WriteJsonFile("config/config.json", saveSetting);

			// Refresh all the settings
			divacore::Settings::instance().RefreshAll(setConfig, config);
		}

		void HouseUI::LoadSettings()
		{
			((SpecialItemDisplayer*)setResSelector->getDisplayer())->setSelectedIndex(setConfig[L"resolution"].asInt());
			((SpecialItemDisplayer*)setMvSelector->getDisplayer())->setSelectedIndex(setConfig[L"musicVolume"].asInt());
			((SpecialItemDisplayer*)setSevSelector->getDisplayer())->setSelectedIndex(setConfig[L"seVolume"].asInt());
			if (setConfig[L"isWindowMode"].asBool())
				((SpecialItemDisplayer*)setWindowModeSelector->getDisplayer())->setSelectedIndex(0);
			else
				((SpecialItemDisplayer*)setWindowModeSelector->getDisplayer())->setSelectedIndex(1);
			((SpecialItemDisplayer*)setUiModSelector->getDisplayer())->setSelectedIndex(setConfig[L"uiMod"].asInt());
			((SpecialItemDisplayer*)setLanSelector->getDisplayer())->setSelectedIndex(setConfig[L"language"].asInt());
			((SpecialItemDisplayer*)setPsSelector->getDisplayer())->setSelectedIndex(setConfig[L"particleSystem"].asInt());
			if(!setConfig.isMember(L"isAutoCT"))
				setConfig[L"isAutoCT"] = true;
			if ( setConfig[L"isAutoCT"].asBool())
				((SpecialItemDisplayer*)setAutoCtSelector->getDisplayer())->setSelectedIndex(0);
			else
				((SpecialItemDisplayer*)setAutoCtSelector->getDisplayer())->setSelectedIndex(1);
		}

		gcn::WindowEx* HouseUI::CreateSettingWindow()
		{
			WJson::Value tv;
			Helper::ReadJsonFromFile(L"config/uiconfig/house/SettingWindow.json", tv);
			WindowEx* win = Helper::CreateWindow(tv[L"window"]);
			win->SetMovable(true);

			SuperButtonEx* confirmButton = Helper::CreateButton(tv[L"DefaultButton"]);
			PointEx pos = Helper::GetPoint(tv[L"confirmBtnPos"]);
			confirmButton->setPosition(pos.x, pos.y);
			confirmButton->setText(L"确定");
			confirmButton->addMouseListener(new LoginButton_MouseListener());
			win->add(confirmButton);
			settingConfirmBtn = confirmButton;

			SuperButtonEx* cancelButton = Helper::CreateButton(tv[L"DefaultButton"]);
			pos = Helper::GetPoint(tv[L"cancelBtnPos"]);
			cancelButton->setPosition(pos.x, pos.y);
			cancelButton->setText(L"取消");
			cancelButton->addMouseListener(new LoginButton_MouseListener());
			win->add(cancelButton);
			settingCancelBtn = cancelButton;

			//SelectorEx* resSelector = SelectorEx::FromJson(tv[L"selector"]);
			//SpecialItemDisplayer* displayer = new SpecialItemDisplayer();
			//for (WJson::Value::iterator i = config[L"resolutions"].begin(); i != config[L"resolutions"].end(); i++)
			//{
			//	displayer->pushItem((*i)[L"name"].asString());
			//}
			//resSelector->setDisplayer(displayer);
			
			// 分辨率
			SelectorEx* resSelector = CreateResolutionSelector(tv[L"selector"], config[L"resolutions_description"], tv[L"resSelDesRect"]);
			((SpecialItemDisplayer*)resSelector->getDisplayer())->setRepeat(true);
			win->add(resSelector);
			win->add(Helper::CreateLabel(tv[L"resLabel"]));
			setResSelector = resSelector;
			// 窗口模式
			SelectorEx* windowModeSelector = CreateSelector(tv[L"selector"], config[L"windowModes"], tv[L"windowModeSelDesRect"]);
			win->add(windowModeSelector);
			((SpecialItemDisplayer*)windowModeSelector->getDisplayer())->setRepeat(true);
			win->add(Helper::CreateLabel(tv[L"windowModeLabel"]));
			setWindowModeSelector = windowModeSelector;
			// 音乐音量
			SelectorEx* mvSelector = CreateSelector(tv[L"selector"], config[L"musicVolumes"], tv[L"mvSelDesRect"]);
			win->add(mvSelector);
			win->add(Helper::CreateLabel(tv[L"mvLabel"]));
			setMvSelector = mvSelector;
			// 音效音量
			SelectorEx* sevSelector = CreateSelector(tv[L"selector"], config[L"seVolumes"], tv[L"sevSelDesRect"]);
			win->add(sevSelector);
			win->add(Helper::CreateLabel(tv[L"sevLabel"]));
			setSevSelector = sevSelector;
			// 按键MOD (Abandon)
			// SelectorEx* keyModSelector = CreateSelector(tv[L"selector"], config[L"keyMods"], tv[L"keyModSelDesRect"]);
			// win->add(keyModSelector);
			// win->add(Helper::CreateLabel(tv[L"keyModLabel"]));
			// setKeyModSelector = keyModSelector;
			// 界面MOD
			SelectorEx* uiModSelector = CreateSelector(tv[L"selector"], config[L"uiMods"], tv[L"uiModSelDesRect"]);
			win->add(uiModSelector);
			win->add(Helper::CreateLabel(tv[L"uiModLabel"]));
			setUiModSelector = uiModSelector;
			// 语言
			SelectorEx* lanSelector = CreateSelector(tv[L"selector"], config[L"languages"], tv[L"lanSelDesRect"]);
			win->add(lanSelector);
			win->add(Helper::CreateLabel(tv[L"lanLabel"]));
			setLanSelector = lanSelector;
			// 例子效果
			SelectorEx* psSelector = CreateSelector(tv[L"selector"], config[L"particleSystems"], tv[L"psSelDesRect"]);
			win->add(psSelector);
			win->add(Helper::CreateLabel(tv[L"psLabel"]));
			setPsSelector = psSelector;
			// 自动爆气
			SelectorEx* autoCtSelector = CreateSelector(tv[L"selector"], config[L"autoCt"], tv[L"autoCtSelDesRect"]);
			win->add(autoCtSelector);
			((SpecialItemDisplayer*)autoCtSelector->getDisplayer())->setRepeat(true);
			win->add(Helper::CreateLabel(tv[L"autoCtLabel"]));
			setAutoCtSelector = autoCtSelector;

			return win;
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
			songListFont = new SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"fontSize"].asInt());
			list->setFont(songListFont);
			con->add(list);
			songList = list;

			gcn::ContainerEx* image = CreateStaticImage(conf, L"SongList/GameMode/CoupleMode");
			image->setPosition(image->getX() - con->getX(), image->getY() - con->getY());
			songListImage = image;
			con->add(image);

			con->setSize(list->getWidth(), list->getHeight() + list->getY() - image->getY());
			setSongListImage(0);
			songListInfo = tv;

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

		gcn::WindowEx* HouseUI::CreateModeWindow(const WJson::Value& conf)
		{
			using namespace gcn;
			WindowEx* win = Helper::CreateWindow(conf[L"DefaultWindowBack"]);
			win->SetMovable(true);

			WJson::Value tv = conf[L"ModeWindow"];
			int modeNum = tv[L"num"].asInt();
			int per = tv[L"itemPerRow"].asInt();

			modeButtonList.clear();
			for (int i = 0; i < modeNum; i++)
			{
				SuperButtonEx* b = Helper::CreateButton(conf[L"DefaultButton"]);
				b->setText(tv[L"names"][i].asString());
				b->setPosition(tv[L"beginX"].asInt() + (i % per) * tv[L"gapX"].asInt(), tv[L"beginY"].asInt() + (i / per) * tv[L"gapY"].asInt());
				b->userData = (void*)i;
				b->addMouseListener(new Mode_MouseListener());
				win->add(b);
				modeButtonList.push_back(b);
			}

			modeConfirmButton = Helper::CreateButton(conf[L"DefaultButton"]);
			modeConfirmButton->setText(tv[L"confirmName"].asString());
			modeConfirmButton->setPosition(tv[L"confirmX"].asInt(), tv[L"confirmY"].asInt());
			modeConfirmButton->addMouseListener(new LoginButton_MouseListener());
			win->add(modeConfirmButton);

			// caption
			win->add(Helper::CreateLabel(tv[L"Caption"]));

			return win;
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
			statusPanelFont = new gcn::SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"fontSize"].asInt());
			lb->setFont(statusPanelFont);
			hostInfoLabel = lb;

			return panel;
		}

		gcn::ContainerEx* HouseUI::CreatePlayerListPanel(const WJson::Value& conf)
		{
			using namespace gcn;
			WJson::Value tv = conf[L"RoomInfo/Config"];
			ContainerEx* panel = CreateStaticImage(conf, L"RoomInfo/background");
			playerListFont = new gcn::SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"fontSize"].asInt());
			
			RoomInfoList* list = new RoomInfoList();
			list->setMaxItem(tv[L"maxItem"].asInt());
			list->setSize(panel->getWidth(), panel->getHeight());
			list->setGap(gcn::Rectangle(tv[L"gap"][L"x"].asInt(),
				tv[L"gap"][L"y"].asInt(),
				tv[L"gap"][L"width"].asInt(),
				tv[L"gap"][L"height"].asInt()), 
				tv[L"gap"][L"itemGap"].asInt());
			//Font* font = new gcn::SoraGUIFont(SETTINGS.getGlobalFontName(), 16);
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
			playerListPagenumFont = new SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"pagenumFontSize"].asInt());
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
			list->addMouseListener(new LoginButton_MouseListener());
			list->setSelectMode(true);

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
			messageAreaFont = new SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"fontSize"].asInt());
			chatBox->setFont(messageAreaFont);
			chatBox->setTotalMaxLine(config[L"chatAreaMaxLine"].asInt());
			chatBox->adjust();
			
			panel->add(chatBox);
			messagePanelChatBox = chatBox;

			// message slider

			MessageSlider* slider = new MessageSlider();
			WJson::Reader reader;
			reader.parse(ReadJsonFile(L"config/uiconfig/house/ChatSlider.json"), tv);
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
			messageInputFont = new SoraGUIFont(SETTINGS.getGlobalFontName(), tv[L"fontSize"].asInt());
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
#ifndef _DEBUG
			VERSION.RequireVersion();
			while(VERSION.getState() == divacore::Version::REQUIREING)
				Base::TimeUtil::mSleep(1);
			if(VERSION.getState() == divacore::Version::UNREADY)
			{
				mgr->GetMB()->Show(L"无法获取版本信息，请重试。", L"错误", gcn::MessageBoxEx::TYPE_OK);
				return;
			}
			else if(!VERSION.CheckVersion())
			{
				mgr->GetMB()->Show(L"版本已过时，请更新游戏。", L"错误", gcn::MessageBoxEx::TYPE_OK);
				return;
			}
#endif
			if(!connectServer())
			{
				mgr->GetMB()->Show(L"无法连接服务器", L"错误", gcn::MessageBoxEx::TYPE_OK);
				disconnectServer();
				return;
			}

#if defined(DIVA_USE_GNET)
			AUTH_CLIENT.login(Base::String(usernameInput->getText()).lower(),Base::String(encryptPW).lower());
			//AUTH_CLIENT.login(Base::ws2s(usernameInput->getText()),Base::String(passwordInput->getText()));
#endif
		}

		void HouseUI::logout() {
#if defined(DIVA_USE_GNET)
			STAGE_CLIENT.logout();
			SCHEDULER_CLIENT.logout();
			CHAT_CLIENT.logout();
			AUTH_CLIENT.logout();
#endif
			disconnectServer();
		}

		void HouseUI::LoginButtonClicked()
		{
			using namespace Net;
#if defined(DIVA_USE_GNET) || defined(DIVA_USE_POMELO)
			CSHA1 sha1;
			string passwd = Base::String(passwordInput->getText());
			sha1.Update((UINT_8*)passwd.c_str(), passwd.size() * sizeof(char));
			sha1.Final();
			sha1.ReportHashStl(encryptPW, CSHA1::REPORT_HEX_SHORT);

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

		void HouseUI::LoginCloseButtonClicked()
		{
			exit(0);
		}

		void HouseUI::StageListClicked()
		{
			int tmp = stageList->getSelectedIndex();
			if (tmp > 0 && stageList->getItemCount() > tmp)
			{
				StageListItem *item = dynamic_cast<StageListItem*>(stageList->getItem(tmp));
				if (item->getInfo().playerInfo.id == 0)
					return;
				sora::SoraBGMManager::Instance()->playSE(soundConfig[L"clickButton"].asString(), SETTINGS.getSEVolume());

				mgr->GetMB()->RegisterCallback(MessageBoxEx::Callback(&HouseUI::cb_kick_player, this));
				mgr->GetMB()->Show(L"确定要踢出"+item->getInfo().playerInfo.nickname+L"吗", L"提示", gcn::MessageBoxEx::TYPE_YESNO);
			}
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
			if (mouseEvent.getSource() == (gcn::Widget*) stageList && stageList->checkIsEnabled())
			{
#if defined(DIVA_USE_GNET)
				if (STAGE_CLIENT.state() == divanet::StageClient::STAGE && STAGE_CLIENT.owner())
#else
				if (POMELO_STAGE_PEER->isIdleStage() && POMELO_STAGE_PEER->owner())
#endif
					StageListClicked();
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) roomListRefreshButton && roomListRefreshButton->checkIsEnabled())
			{
				Refresh_RoomList(true);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) loginCloseButton && loginCloseButton->checkIsEnabled())
			{
				LoginCloseButtonClicked();
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
				mgr->GetMB()->RegisterCallback(MessageBoxEx::Callback(&HouseUI::cb_exit_stage, this));
				mgr->GetMB()->Show(L"确定要离开舞台吗？", L"提示", gcn::MessageBoxEx::TYPE_YESNO);
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
				if (!songList->getItemCount())
				{	
					mgr->GetMB()->Show(L"至少选择一首歌以开始游戏。");
					return;
				}
				if (!MAPMGR.isMapLeagal(SELECTEDMAPS[0].id, SELECTEDMAPS[0].level))
				{
					mgr->GetMB()->Show(L"该歌曲未下载或尚未下载完成，无法开始游戏。");
					return;
				}

				sora::SoraBGMManager::Instance()->playSE(soundConfig[L"gameStart"].asString(), SETTINGS.getSEVolume());
		
				if (state == STATE_ROOM)
					StartOfflineGame();
				else
					start_game();	
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) readyButton)
			{
#if defined(DIVA_USE_GNET)
				if(STAGE_CLIENT.isReady())
					STAGE_CLIENT.unready();
#else
				if(POMELO_STAGE_PEER->getIsReady())
					POMELO_STAGE_PEER->unready();
#endif
				else
				{
					if (!songList->getItemCount())
					{
						mgr->GetMB()->Show(L"房主至少选择一首歌才能够准备。");
						return;
					}
					if (!MAPMGR.isMapLeagal(SELECTEDMAPS[0].id, SELECTEDMAPS[0].level))
					{
						mgr->GetMB()->Show(L"该歌曲未下载或尚未下载完成，无法准备。");
						return;
					}
#if defined(DIVA_USE_GNET)
					STAGE_CLIENT.ready();
#else
					POMELO_STAGE_PEER->ready();
#endif
				}
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
				//logout();
				//sora::SoraCore::Instance()->shutDown();
				//setState(STATE_LOGINWINDOW);
				mgr->GetMB()->RegisterCallback(MessageBoxEx::Callback(&HouseUI::cb_exit_game, this));
				mgr->GetMB()->Show(L"确定要退出游戏吗？", L"提示", gcn::MessageBoxEx::TYPE_YESNO);

			}
			if (mouseEvent.getSource() == (gcn::Widget*) modeButton)
			{
				mgr->OpenWindow(modeWindow);
				modeButton->setForegroundColor(gcn::Color(255, 255, 255, modeButton->getAlpha()));
				modeWindow->FadeIn(10);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) modeConfirmButton)
			{
				modeWindow->FadeOut(10);
#if defined(DIVA_USE_GNET)
				STAGE_CLIENT.setHooks(MAPMGR.GetSelectedModeInt());
#else
				POMELO_STAGE_PEER->setHooks(MAPMGR.GetSelectedModeInt());
#endif
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) optionButton || mouseEvent.getSource() == (gcn::Widget*) settingsButton)
			{
				LoadSettings();
				mgr->OpenWindow(settingWindow);
				settingWindow->FadeIn(10);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) settingCancelBtn)
			{
				settingWindow->FadeOut(10);
				return;
			}
			if (mouseEvent.getSource() == (gcn::Widget*) settingConfirmBtn)
			{
				SaveSettings();
				settingWindow->FadeOut(10);
				return;
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
				else if (index == 1)
					MessageChannelChange(CHANNEL_STAGE);
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
#if defined(DIVA_USE_GNET)
			if(!NET_COMMAND.Analysis(messagePanelInputBox->getText())) {

                    if (msgChannelState == CHANNEL_WORLD)
                            CHAT_CLIENT.send("global", L"#W#[世界] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
                    else if (msgChannelState == CHANNEL_PRIVATE)
                    {
                            if (msgSendId != -1)
                                    CHAT_CLIENT.sendTo(Base::String::any2string<int>(msgSendId), L"#P#[私聊] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
                            else
                                    messagePanelChatBox->addText(L"[提示] 请先选择您要私聊的对象", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
                    }
                    else if (msgChannelState == CHANNEL_STAGE)
                    {
                            CHAT_CLIENT.send(STAGE_CLIENT.getRoomID()+"_stage_room", L"#T#[舞台] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
                    }
                    //CHAT_CLIENT.sendTo("691",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
                    //divanet::NetworkManager::instance().chat()->send("chat#sendmsg","%s%W","global",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
            }
#else
			if(!NET_COMMAND.Analysis(messagePanelInputBox->getText())) {

				if (msgChannelState == CHANNEL_WORLD)
					POMELO_CHAT_PEER->sendGlobal(L"#W#[世界] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
				else if (msgChannelState == CHANNEL_PRIVATE)
				{
					if (msgSendId != -1)
						POMELO_CHAT_PEER->sendTo(Base::String::any2string<int>(msgSendId), L"#P#[私聊] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
					else
						messagePanelChatBox->addText(L"[提示] 请先选择您要私聊的对象", gcn::Helper::GetColor(conf[L"MessageArea/TextColors"][L"hint"]));
				}
				else if (msgChannelState == CHANNEL_STAGE)
				{
					POMELO_CHAT_PEER->sendInStage(L"#T#[舞台] " + PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
				}
				//CHAT_CLIENT.sendTo("691",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
				//divanet::NetworkManager::instance().chat()->send("chat#sendmsg","%s%W","global",PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
			}
			//messagePanelChatBox->addText(PlayerManager::Instance()->GetHostInfo().nickname + L"：" + messagePanelInputBox->getText());
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
#if defined(DIVA_USE_GNET)
			STAGE_CLIENT.draw(t);
#elif defined(DIVA_USE_POMELO)
			POMELO_STAGE_PEER->draw(t);
#else
			for (int i=0; i<teamListButtons.size(); i++)
				teamListButtons[i]->setSelected(t == i);
#endif
		}

		void HouseUI::ModeButtonClicked(int index)
		{
#if defined(DIVA_USE_GNET)
			if (STAGE_CLIENT.owner())
#else
			if (POMELO_STAGE_PEER->owner())
#endif
			{
				bool b = modeButtonList[index]->getSelected();
				MAPMGR.SelectedMode_ToggleMode((divamap::DivaMapManager::GameMode)index, !b);
				ModeButtonRefresh();
			}
		}

		void HouseUI::ModeButtonRefresh()
		{
			for (int i = 0; i < 10; i++)
				modeButtonList[i]->setSelected(MAPMGR.IsModeSelected((divamap::DivaMapManager::GameMode)i));
		}

		void HouseUI::StartOfflineGame()
		{
			if(getState() == HouseUI::STATE_ROOM)
			{
				if(mgr->GetMB()->isTopWindow())
					mgr->CloseTopWindow();

				Base::Path songPath = MAPMGR.GetDivaOLFilePath(MAPMGR.GetSelectedMaps()[0].id, static_cast<divamap::DivaMap::LevelType>(MAPMGR.GetSelectedMaps()[0].level)); 
			
				CORE_PTR->setSong(songPath.filePath().str(), songPath.fileName());

				CORE_PTR->setInitState("load");
				CORE_PTR->registerGameMode(new divacore::SinglePlay);

				NextState = "core";
			}
		}


		// ------------------------------------ Event ----------------------------------

		using namespace gcn;

		void LoginButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->MouseClicked(mouseEvent);
		}
		void ClickButton_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->MouseClicked(mouseEvent);
		}
		void ClickButton_MouseListener::mousePressed(gcn::MouseEvent& mouseEvent)
		{
			sora::SoraBGMManager::Instance()->playSE(soundConfig[L"clickButton"].asString(), SETTINGS.getSEVolume());
		}

		void TeamSelect_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->TeamListClicked(mouseEvent);
		}

		void Mode_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->ModeButtonClicked( (int)((SuperButtonEx*)(mouseEvent.getSource()))->userData );
		}

		void NotSupportInAlpha_MouseListener::mouseClicked(gcn::MouseEvent& mouseEvent)
		{
			HouseUI::Instance()->mgr->GetMB()->Show(L"Alpha测试中不包含此功能。", L"提示", gcn::MessageBoxEx::TYPE_OK);
		}

		void NotSupportInAlpha_MouseListener::mousePressed(gcn::MouseEvent& mouseEvent)
		{
			sora::SoraBGMManager::Instance()->playSE(soundConfig[L"clickButton"].asString(), SETTINGS.getSEVolume());
		}

	}
}