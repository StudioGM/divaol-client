#include "MusicUI.h"
#include "SoraResourceFile.h"
#include "ui/Config/DivaUIConfig.h"
#include "MusicGameState.h"
#include "divanetwork/DivaNetworkManager.h"
#include "divanetwork/DivaNetInfo.h"
#include "lib/Base/String.h"
#include "Utility/DivaSettings.h"

namespace diva
{
	namespace MusicUI
	{
		void MusicUI::initNet()
		{
			// SMDivaNetCode
			//netCore.init();
			//netCore.connect();

			//if (!netCore.isConnected())
			//	throw "failed connect with server.";

			//netCore.send(DIVA_NET_CTS_LOBBY_LOGIN, 0, "%d%s", config["myId"].asInt(), config["myName"].asCString());
			//netState = NETSTATE_OFFLINE;
			// CodeEnd
		}

		void MusicUI::Enter()
		{
			sora::GCN_GLOBAL->getTop()->add(trueTop, 0, 0);
			//top->setVisible(true);
			trueTop->setVisible(true);
			trueTop->setEnabled(true);
			refreshSelectedSongList();
			refreshSongList();
		}

		void MusicUI::Leave()
		{
			sora::GCN_GLOBAL->getTop()->remove(trueTop);
			trueTop->setVisible(false);
			sora::SoraBGMManager::Instance()->stop(false);
		}

		void MusicUI::BeginLeave()
		{
			trueTop->setEnabled(false);
		}



		MusicUI::MusicUI()
		{
			// initialize net
			WJson::Value tv;
			//initNet();
			maxRankPage = 4;

			//////////////////////////////////////////////////////////////////////////
			trueTop = new gcn::Container();
			trueTop->setSize(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
			trueTop->setOpaque(false);
			mgr = new WindowMgr(trueTop);
			
			top = new gcn::WindowEx();
			top->setSize(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
			top->setOpaque(false);
			//top->SetMovable(true);
			mgr->OpenWindow(top);

			gcn::MessageBoxEx* mbex = new gcn::MessageBoxEx();
			{
				WJson::Value t;
				gcn::Helper::ReadJsonFromFile(L"config/uiconfig/house/MessageBox.json", t);
				mbex->LoadFromJsonFile(t);
				mgr->RegisterMessageBox(mbex);
			}
			
			
			countStarted = false;

			// load font
			artistFont = new gcn::SoraGUIFont(SETTINGS.getGlobalFontName(), 30);
			uiImage1 = gcn::Image::load("res/UI1.png");
			uiImage2 = gcn::Image::load("res/UI2.png");

			// Game Mode
			for (int i = 0; i < config[L"gameModeNames"].size(); i++)
				modeNames.push_back(config[L"gameModeNames"][i].asString());
		
			// Background
			background = new gcn::ImageDisplayerEx();
			background->setSize(1920, 1080);
			top->add(background, 0, 0);

			// BackPanel
			backPanel = new gcn::ContainerEx();
			backPanel->setSize(1859, 997);
			backPanel->load(L"res/UI1.png", gcn::Rectangle(0, 890, 1859, 997));
			top->add(backPanel, 30, 51);

			// SongListBox
			songListBox = new DivaSongList();
			songListBox->setSelectMode(true);
			songListBox->setMaxItem(6);
			songListBox->setSize(609, 873);
			songListBox->setGap(gcn::Rectangle(53, 107, 571, 139), -15);
			songListBox->loadImage(L"res/UI1.png", gcn::Rectangle(613, 0, 609, 873));
			songListBox->loadButtonImage(L"res/UI1.png",
				gcn::Rectangle(111, 326, 64, 51),
				gcn::Rectangle(111, 204, 64, 51),
				gcn::Rectangle(111, 262, 64, 51),
				-14, -10,
				gcn::Rectangle(235, 326, 64, 51),
				gcn::Rectangle(235, 204, 64, 51),
				gcn::Rectangle(235, 262, 64, 51),
				-14, -10);
			//songListBox->pushItem(
			//
			top->add(songListBox, 83, 94);

			leftArtistButton = new gcn::ButtonEx();
			leftArtistButton->setLook(L"res/UI1.png", 
				gcn::Rectangle(1432, 0, 105, 154),
				gcn::Rectangle(1222, 0, 105, 154),
				gcn::Rectangle(1327, 0, 105, 154),
				gcn::Rectangle(1432, 0, 105, 154),
				-20, -20);
			leftArtistButton->setSize(65, 114);
			leftArtistButton->setSE(L"stone2.ogg", L"stone2.ogg");
			leftArtistButton->addMouseListener(new ArtistButton_MouseListener());
			top->add(leftArtistButton, 83, 224);

			leftSongButton = new gcn::ButtonEx();
			leftSongButton->setLook(L"res/UI1.png",
				gcn::Rectangle(1432, 154, 105, 154),
				gcn::Rectangle(1222, 154, 105, 154),
				gcn::Rectangle(1327, 154, 105, 154),
				gcn::Rectangle(1432, 154, 105, 154), 
				-20, -20);
			leftSongButton->setSize(65, 114);
			leftSongButton->addMouseListener(new SongButton_MouseListener());
			top->add(leftSongButton, 83, 344);

			// SLIDER
			slider = new DivaSongSlider();
			{
				gcn::MarkerEx* marker = new gcn::MarkerEx();
				marker->setLook(L"res/UI2.png",
					gcn::Rectangle(1447, 0, 41, 13),
					gcn::Rectangle(1365, 0, 41, 13),
					gcn::Rectangle(1406, 0, 41, 13),
					gcn::Rectangle(1447, 25, 41, 13),
					gcn::Rectangle(1365, 25, 41, 13),
					gcn::Rectangle(1406, 25, 41, 13),
					gcn::Rectangle(1447, 13, 41, 12),
					gcn::Rectangle(1365, 13, 41, 12),
					gcn::Rectangle(1406, 13, 41, 12),
					-7);
				marker->setSize(27, 24);

				gcn::ButtonEx* up = new gcn::ButtonEx();
				up->setLook(L"res/UI2.png",
					gcn::Rectangle(1300, 0, 65, 52),
					gcn::Rectangle(1170, 0, 65, 52),
					gcn::Rectangle(1235, 0, 65, 52),
					gcn::Rectangle(1300, 0, 65, 52),
					-16, -16);
				up->setSize(33, 17);

				gcn::ButtonEx* down = new gcn::ButtonEx();
				down->setLook(L"res/UI2.png",
					gcn::Rectangle(1300, 52, 65, 52),
					gcn::Rectangle(1170, 52, 65, 52),
					gcn::Rectangle(1235, 52, 65, 52),
					gcn::Rectangle(1300, 52, 65, 52),
					-16, -16);
				down->setSize(33, 17);

				slider->setLook(up, down, marker);
			}
			slider->setMarkScale(0, 4, 6);
			slider->setMarkPosition(0);
			slider->setSize(33, 500);
			top->add(slider, 118, 458);

			// 

			noimageFileName = L"res/UI2.png";
			randomFileName = L"res/UI2.png";
			noimageRect = gcn::Rectangle(412,0,368,207);
			randomRect = gcn::Rectangle(412,207,368,207);

			// SongListLogo
			songListLogo = new gcn::ContainerEx();
			songListLogo->load(L"res/UI2.png", gcn::Rectangle(803,1260,627,112));
			songListLogo->setSize(627, 112);
			top->add(songListLogo, 74, 96);

			// ThumbContainer

			thumbImage = new gcn::ContainerEx();
			thumbImage->load(noimageFileName, noimageRect, true);
			thumbImage->setSize(368,207);
			top->add(thumbImage, 793, 132);

			// ThumbFrameContainer

			thumbFrameContainter = new gcn::ContainerEx();
			thumbFrameContainter->load(L"res/UI2.png", gcn::Rectangle(0,0,412,251));
			thumbFrameContainter->setSize(412,251);
			top->add(thumbFrameContainter, 771,110);

			// SongInfoContainer

			songInfoFont = new gcn::SoraGUIFont(SETTINGS.getGlobalFontName(), 16);

			songInfoContainer = new DivaSongInfoContainer();
			songInfoContainer->setFont(songInfoFont);
			songInfoContainer->load(L"res/UI2.png", gcn::Rectangle(412,414,553,295));
			songInfoContainer->setSize(553,295);
			top->add(songInfoContainer, 700, 346);
		
			// SelectedListBox

			selectedListBox = new DivaSelectedListBox();
			top->add(selectedListBox, 693 + 19, 710 - 19 + 35);

			int offx = -670, offy = 245;

			// ModeLeftButton
			modeLeftButton = new gcn::ButtonEx();
			modeLeftButton->setLook(L"res/UI1.png", gcn::Rectangle(111, 326, 64, 51),
				gcn::Rectangle(111, 204, 64, 51),
				gcn::Rectangle(111, 262, 64, 51),
				gcn::Rectangle(111, 326, 64, 51),
				0, 0);
			modeLeftButton->addMouseListener(new GameModeSelectLeft_MouseListener());
			modeLeftButton->setSize(36, 25);
			top->add(modeLeftButton, 1525 + offx, 680 + offy);

			// ModeRightButton
			modeRightButton = new gcn::ButtonEx();
			modeRightButton->setLook(L"res/UI1.png", gcn::Rectangle(235, 326, 64, 51),
				gcn::Rectangle(235, 204, 64, 51),
				gcn::Rectangle(235, 262, 64, 51),
				gcn::Rectangle(235, 326, 64, 51),
				0, 0);
			modeRightButton->addMouseListener(new GameModeSelectRight_MouseListener());
			modeRightButton->setSize(36, 25);
			top->add(modeRightButton, 1731 + offx, 680 + offy);

			// ModeTextLabel
			modeTextLabel = new gcn::LabelEx(modeNames[gameMode]);
			modeTextLabel->setSize(90, 50);
			modeTextLabel->setForegroundColor(gcn::Color(0, 0, 0));
			modeTextLabel->setFont(artistFont);
			top->add(modeTextLabel, 1625 + offx, 670 + offy);

			// RankPageLeftButton
			rankPageLeftBtn = new gcn::ButtonEx();
			rankPageLeftBtn->setLook(L"res/UI1.png", gcn::Rectangle(111, 326, 64, 51),
				gcn::Rectangle(111, 204, 64, 51),
				gcn::Rectangle(111, 262, 64, 51),
				gcn::Rectangle(111, 326, 64, 51),
				0, 0);
			rankPageLeftBtn->addMouseListener(new RankPageLeft_MouseListener());
			rankPageLeftBtn->setSize(36, 25);
			top->add(rankPageLeftBtn, 1460, 726);

			// RankPageRightButton
			rankPageRightBtn = new gcn::ButtonEx();
			rankPageRightBtn->setLook(L"res/UI1.png", gcn::Rectangle(235, 326, 64, 51),
				gcn::Rectangle(235, 204, 64, 51),
				gcn::Rectangle(235, 262, 64, 51),
				gcn::Rectangle(235, 326, 64, 51),
				0, 0);
			rankPageRightBtn->addMouseListener(new RankPageRight_MouseListener());
			rankPageRightBtn->setSize(36, 25);
			top->add(rankPageRightBtn, 1660, 726);

			// RankPageTextLabel
			rankPageText = new gcn::LabelEx(L"1~4");
			rankPageText->setSize(90, 50);
			rankPageText->setForegroundColor(gcn::Color(0, 0, 0));
			rankPageText->setFont(artistFont);
			top->add(rankPageText, 1560, 726);

			// PlayerList
			//playerList = new DivaPlayerList();
			//playerList->setFont(artistFont);
			//playerList->init();
			//top->add(playerList, 1385, 179);

			// SongSelectButton
			{
				// SongSelectButton
				gcn::Helper::ReadJsonFromFile(L"config/uiconfig/music/song_select.json", tv);
				
				songSelectButton = gcn::Helper::CreateButton(tv);
				songSelectButton->addMouseListener(new SongSelectButton_MouseListener());
				top->add(songSelectButton);
			}

			// StartButton
			{
				gcn::Helper::ReadJsonFromFile(L"config/uiconfig/music/btn_select.json", tv);

				startButton = gcn::Helper::CreateButton(tv);
				startButton->addMouseListener(new PlayButton_MouseListener());
				top->add(startButton);

			}

			// Ranking List
			{
				gcn::Helper::ReadJsonFromFile(L"config/uiconfig/music/list_ranking.json", tv);
				rankingList = Helper::CreateList<RankingList>(tv);
				for (int i=0; i<5; i++)
				{
					RankingListItem* item = new RankingListItem();
					RankingListItem::LoadBack(tv[L"itemBackImage"][L"filename"].asString(), Helper::GetRect(tv[L"itemBackImage"][L"srcRect"]));
					RankingListItem::LoadFromJson(tv[L"positions"]);
					item->SetRanking(i+1);
					item->SetInfo(100 + i * 50, 50 + i * 10, 0, 0, L"none");
					item->SetNull(1);
					rankingList->pushItem(item);
					if (i == 4)
						item->SetColor(0x00FFFF, 0xFF0000);
					else
						item->SetColor(0xFFFFFF, 0xFFFFFF);
				}
				top->add(rankingList);
			}

			//////////////////////////////////////////////////////////////////////////


			// ------
			divamap::DivaMap tMap;
			tMap.levels[tMap.Easy];
			tMap.levels[tMap.Normal];
			tMap.levels[tMap.Hard];
			tMap.levels[tMap.Extra];
			tMap.levels[tMap.Die];
			//
			
			songListOrigItems.clear();
			using namespace divamap;
			std::vector<DivaMap> orig;
			for (std::map<int, DivaMap>::iterator i = MAPS.begin(); i != MAPS.end(); i++)
				orig.push_back(i->second);
			
			gcn::Image* image = gcn::Image::load("res/UI1.png");
			gcn::Image* notDI = gcn::Image::load("res/download.png");
			songListOrigItems.push_back(new SongListItem(image, tMap, SongListItem::RANDOM));
			for (int i=0; i<orig.size(); i++)
				songListOrigItems.push_back(new SongListItem(image, orig[i], SongListItem::SONG, NULL, notDI));
			songListBox->setItems(songListOrigItems);

			state = SONGLIST_ORIG;

			artistListItems.clear();
			for (int i=0; i<orig.size(); i++)
				for (std::vector<std::wstring>::const_iterator j = orig[i].header.artists.cbegin(); j != orig[i].header.artists.cend(); j++)
				{
					if (artistListItems.find(*j) == artistListItems.end())
					{
						tMap.header.artists.clear();
						tMap.header.artists.push_back(*j);
						SongListItem* p = new SongListItem(image, tMap, SongListItem::ARTIST, artistFont);
						artistListItems[*j] = p;
					}
				}

			refreshSongList();
			//////////////////////////////////////////////////////////////////////////
			
			// Sound Initialize
			//sora::SoraBGMManager::Instance()->play(L"D:\\KuGou\\philosophyz.mp3", true);

		}

		MusicUI::~MusicUI()
		{
			//if (sprite)
			//	delete sprite;
			for (int i=0; i<songListOrigItems.size(); i++)
				delete songListOrigItems[i];
			for (std::map<std::wstring, ListItemEx*>::iterator i = artistListItems.begin(); i != artistListItems.end(); i++)
				delete (i->second);
			if (songInfoFont)
				delete songInfoFont;
			if (artistFont)
				delete artistFont;
		}

		void MusicUI::refreshSelectedSongList()
		{
			selectedListBox->clearItems();
			for (int i=0; i<SELECTEDMAPS.size(); i++)
			{
				int index = songListBox->getIndexByMapId(SELECTEDMAPS[i].id);
				if (index == -1)
					throw "Fatal Error! Cann't find this Map!";
				selectedListBox->pushItem(MAPS[SELECTEDMAPS[i].id], SELECTEDMAPS[i].level, (SongListItem*)songListBox->getItem(index), DivaSelectedListBox::SPECIFIC, SELECTEDMAPS[i].mode);
			}
		}

		void MusicUI::SetFatherState(MusicGameState* state)
		{
			musicGameState = state;
		}

		void MusicUI::Render()
		{
			//sprite->render(0, 0);
		}

		void MusicUI::updateNet(float delta)
		{
			// SMDivaNetCode
			//using namespace divacore::network;

			//// check connect
			//if (!netCore.isConnected())
			//	netState = NETSTATE_OFFLINE;

			//// update first
			//netCore.update(delta);

			//// deal with the msg queue
			//while (!netCore.isQueueEmpty())
			//{
			//	Packet pack = netCore.pick();
			//	
			//	switch (pack.getID())
			//	{
			//	case DIVA_NET_STC_LOBBY_YOUR_INFO:
			//		netState = NETSTATE_ONLINE;
			//		break;
			//	case DIVA_NET_STC_LOBBY_ALL_INFO:
			//		playerList->updateAll(pack);
			//		break;
			//	case DIVA_NET_STC_LOBBY_START:
			//		//////////////////////////////////////////////////////////////////////////
			//		// 同样我假设尾巴上有int表示难易度012 EASY NORMAL HARD
			//		gameStartingMode = pack.readInt32();
			//		gameStartingMyPos = pack.readInt32();
			//		gameStartingMapId = pack.readInt32();
			//		gameStartingDif = pack.readInt32();
			//		netCore.send(DIVA_NET_CTS_LOBBY_READY_START);
			//		break;
			//		//////////////////////////////////////////////////////////////////////////
			//	case DIVA_NET_STC_LOBBY_LOGIN:
			//		//////////////////////////////////////////////////////////////////////////
			//		// 未完成，login信息不足
			//		//////////////////////////////////////////////////////////////////////////
			//		{
			//			int pos = pack.readInt32();
			//			DivaPlayerInfo pinfo = playerList->getRoomInfo().players[pos];
			//			pack.readInt32();
			//			pinfo.iconIndex = pack.readInt32();
			//			pinfo.name = pack.readString();
			//			pinfo.isHost == (pos==0);
			//			pinfo.isReady = false;
			//			pinfo.isPlaying = false;
			//			pinfo.isVoid =false;
			//			pinfo.slot = pos;
			//			playerList->ModifyPlayer(pos, pinfo);
			//		}
			//		
			//		break;
			//	case DIVA_NET_STC_LOBBY_LOGOUT:
			//		int t = pack.readInt32();
			//		DivaPlayerInfo pinfo = playerList->getRoomInfo().players[t];
			//		pinfo.isVoid = false;
			//		playerList->ModifyPlayer(t, pinfo);
			//		break;
			//	case DIVA_NET_STC_LOBBY_GAME_ON:
			//		GameStart(gameStartingMapId, gameStartingDif, gameStartingMode, gameStartingMyPos);
			//		break;
			//	case DIVA_NET_STC_LOBBY_UPDATE_SINGLE:
			//		int pos = pack.readInt32();
			//		DivaPlayerInfo pinfo = playerList->getRoomInfo().players[pos];
			//		pinfo.isReady = pack.readBool();
			//		pinfo.isPlaying = pack.readBool();
			//		playerList->ModifyPlayer(pos, pinfo);
			//		break;
			//	}

			//	netCore.pop();
			//}
			// CodeEnd
		}

		void MusicUI::PlayListeningPreview(int index)
		{
			if (index == -1)
				return;
			SongListItem* item = (SongListItem*)songListBox->getItems()[index];
			if ( item->hasListening() )
			{
				playTimer.reset();
				sora::SoraBGMManager::Instance()->stop(false);
				sora::SoraBGMManager::Instance()->play(item->getListening(), false);
				countStarted = false;
			}
			else
			{
				MAPMGR.PrepareDivaMapAudioPreview(item->getMapInfo().id);
			}
		}
		
		void MusicUI::refreshRankingList(bool topRank, bool myRank)
		{
			if (songListBox->getSelectedIndex() == -1)
				return;
			SongListItem* item = (SongListItem*)songListBox->getItem(songListBox->getSelectedIndex());

			// set loading state to all the rankList item
			for (int i = 0; i < rankingList->getItemCount(); i++)
				((RankingListItem*)rankingList->getItem(i))->SetLoading(true);

			if (topRank)
			{
				int be = rankPage * 4 + 1;
				int en = be + 3;
				MAPMGR.PrepareRecordByRank(item->getMapInfo().id, item->getMapInfo().getLevel(item->getDifIndex()), be, en, (void*)rankPage);
			}

			if (myRank)
			{
				MAPMGR.PrepareRecordByUser(item->getMapInfo().id, item->getMapInfo().getLevel(item->getDifIndex()),
					Base::String(divanet::NetInfo::instancePtr()->username).unicode_str());
			}
		}

		void MusicUI::Update(float delta)
		{
			// NetUpdate
			//updateNet(delta);

			// play delay
			if (countStarted)
			{
				double tt = playTimer.getTime();
				if (tt >= config[L"listenDelay"].asDouble())
				{
					PlayListeningPreview(nextListeningIndex);
				}
			}

			// recieve
			MAPQUEUE* q = MAPMGR.GetQueue();
			while (!q->empty())
			{
				const divamap::DivaMapEventMessage &t = (*((*q).begin()));
				switch (t.eventType)
				{
				case divamap::DivaMapEventMessage::PrepareRecordByRank:
					if (t.error)
					{
						//mgr->GetMB()->Show(L"下载缩略图文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						break;
					}
					if (!t.error && t.finish)
					{
						WJson::Reader reader;
						WJson::Value tv;
						reader.parse( s2ws(std::string(t.additionalMessage)), tv);

						int selectedIndex = songListBox->getSelectedIndex();
						if(selectedIndex < 0 || selectedIndex >= songListBox->getItemCount())
							break;
						SongListItem *item = (SongListItem*) songListBox->getItem(selectedIndex);
						if (t.effectedMapID !=  item->getMapInfo().id)
							break;
						if (t.effectedMapLevel != item->getMapInfo().getLevel(item->getDifIndex()))
							break;
						if (rankPage != (int)t.extraPTR)
							break;
						if (!tv.isMember(L"error"))
							break;
						int l = tv[L"rank"].size();
						for (int i = 0; i<l; i++)
						{
							WJson::Value t = tv[L"rank"][i];
							WJson::Value addData;
							reader.parse(t[L"add_data"].asString(), addData);
							((RankingListItem*)rankingList->getItem(i))->SetInfo(t[L"score"].asInt(),
								addData[L"combo"].asInt(),
								addData[L"mode"].asInt(),
								addData[L"maxCT"].asInt(),
								t[L"name"].asString());
							((RankingListItem*)rankingList->getItem(i))->SetNull(0);
						}
						for (int i = l; i < rankingList->getItemCount() -1; i++)
							((RankingListItem*)rankingList->getItem(i))->SetNull(1);

						// set isLoading to false
						for (int i = 0; i < rankingList->getItemCount() -1; i++)
							((RankingListItem*)rankingList->getItem(i))->SetLoading(false);
						//std::string fuck = t.additionalMessage;
					}
					break;
				case divamap::DivaMapEventMessage::PrepareRecordByUser:
					if (t.error)
					{
						//mgr->GetMB()->Show(L"下载缩略图文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						//q->pop_front();
						break;
					}
					if (!t.error && t.finish)
					{
						WJson::Reader reader;
						WJson::Value tv;
						reader.parse(s2ws(std::string(t.additionalMessage)), tv);
						//if (tv[L"error"] != "E_")

						int selectedIndex = songListBox->getSelectedIndex();
						if(selectedIndex < 0 || selectedIndex >= songListBox->getItemCount())
							break;
						SongListItem *item = (SongListItem*) songListBox->getItem(selectedIndex);
						if (t.effectedMapID !=  item->getMapInfo().id)
							break;
						if (t.effectedMapLevel != item->getMapInfo().getLevel(item->getDifIndex()))
							break;
						if (rankPage != (int)t.extraPTR)
							break;
						if (!tv.isMember(L"error"))
							break;

						if (!tv.isMember(L"error"))
							break;
						if (tv[L"error"].asString() == L"E_OK")
						{
							WJson::Value addData;
							reader.parse(tv[L"add_data"].asString(), addData);
							((RankingListItem*)rankingList->getItem(rankingList->getMaxItem() - 1))->SetInfo(tv[L"score"].asInt(),
								addData[L"combo"].asInt(),
								addData[L"mode"].asInt(),
								addData[L"maxCT"].asInt(),
								tv[L"name"].asString());
							((RankingListItem*)rankingList->getItem(rankingList->getMaxItem() - 1))->SetRanking(tv[L"rank"].asInt());
							((RankingListItem*)rankingList->getItem(rankingList->getMaxItem() - 1))->SetNull(0);
						}
						else
						{
							((RankingListItem*)rankingList->getItem(rankingList->getMaxItem() - 1))->SetNull(2);
						}

						// set isLoading to false
						((RankingListItem*)rankingList->getItem(rankingList->getMaxItem() - 1))->SetLoading(false);
					}
					break;
				case divamap::DivaMapEventMessage::PrepareThumbFile :
					if (t.error)
					{
						mgr->GetMB()->Show(L"下载缩略图文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						return;
					}
					if (!t.error && t.finish)
					{
						//selectMusicButton->setEnabled(true);
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						((SongListItem*)songListBox->getItems()[index])->setPreview(MAPMGR.GetThumbFilePath(t.effectedMapID));
					}
					else
						throw "fuck it!";

					break;
				case divamap::DivaMapEventMessage::PrepareAudioPreviewFile :
					if (t.error)
					{
						mgr->GetMB()->Show(L"下载试听文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						break;
					}
					if (!t.error && t.finish)
					{
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						((SongListItem*)songListBox->getItems()[index])->setListening(MAPMGR.GetAudioPreviewFilePath(t.effectedMapID));
						if (songListBox->getHighlightItemIndex() == index)
							sora::SoraBGMManager::Instance()->play(((SongListItem*)songListBox->getItems()[index])->getListening(), false);
					}
					else
						throw "fuck it!";
					break;
				case divamap::DivaMapEventMessage::PrepareMapDataFile :
					if (t.error)
					{
						mgr->GetMB()->Show(L"下载歌曲文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						break;
					}
					if (t.finish)
					{
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						//((SongListItem*)songListBox->getItem(index))->setDownloadFinished(true);
						if(index >= 0) {
							((SongListItem*)songListBox->getItem(index))->setDownloadProgress(MAPMGR.GetMapDownloadProgress(t.effectedMapID));
							if(index == songListBox->getSelectedIndex()) {
								songSelectButton->setText(L"下载中...");
								songSelectButton->setEnabled(false);
							}
						}
					}
					else
					{
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						if(index >= 0) {
							((SongListItem*)songListBox->getItem(index))->setDownloadProgress(MAPMGR.GetMapDownloadProgress(t.effectedMapID));
							if(index == songListBox->getSelectedIndex()) {
								songSelectButton->setText(L"下载中...");
								songSelectButton->setEnabled(false);
							}
						}
					}
					break;
				case divamap::DivaMapEventMessage::UnpackMapDataFile :
					if (t.error)
					{
						mgr->GetMB()->Show(L"解压歌曲文件出错。歌曲名：" + MAPS[t.effectedMapID].header.name);
						break;
					}
					if (t.finish)
					{
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						if(index >= 0) {
							((SongListItem*)songListBox->getItem(index))->setDownloadFinished(true);
							((SongListItem*)songListBox->getItem(index))->setDownloadProgress(1.0);

							if(songListBox->getSelectedIndex() == index)
							{
								songSelectButton->setText(L"选择歌曲");
								songSelectButton->setEnabled(true);
							}
						}
					}
					else
					{
						int index = songListBox->getIndexByMapId(t.effectedMapID);
						if(index >= 0)
							((SongListItem*)songListBox->getItem(index))->setDownloadProgress(MAPMGR.GetMapDownloadProgress(t.effectedMapID));
					}
					break;
				}
				q->pop_front();
			}
		}

		void MusicUI::refreshSongList()
		{
			if (state == SONGLIST_ORIG || state == SONGLIST_SPEART)
			{
				//songListBox->getItems()[index
				int totItem = songListBox->getDisplayedItems();
				for (int i=0; i < totItem; i++)
				{
					SongListItem* t = (SongListItem*)songListBox->getItems()[songListBox->getFirstIndex() + i];
					if (!t->hasPreview())
						MAPMGR.PrepareDivaMapThumb(t->getMapInfo().id);
					if (MAPMGR.isMapDownloaded(t->getMapInfo().id))
					{
						t->setDownloadFinished(true);
						t->setDownloadProgress(1.0);
					}
					else
					{
						t->setDownloadProgress(MAPMGR.GetMapDownloadProgress(t->getMapInfo().id));
					}
				}
			}
		}

		int MusicUI::GetRandomSong(divamap::DivaMap::LevelType level)
		{
			if (state == SONGLIST_ART)
				throw "haha";

			std::vector<int> tv;

			for (int i = 1; i < songListBox->getItemCount(); i++)
			{
				const divamap::DivaMap::Levels& t = ((SongListItem*)(songListBox->getItem(i)))->getMapInfo().levels;
				if (t.find(level) != t.end() && MAPMGR.isMapLeagal(((SongListItem*)(songListBox->getItem(i)))->getMapInfo().id, level))
					tv.push_back(i);
			}

			if (tv.size() == 0)
				return -1;
			
			return tv[Base::Random::RandomInt(0, tv.size())];
		}

		void MusicUI::SongListItemClicked(int index)
		{
			if (index == -1)
			{
				//sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;

				songSelectButton->setText(L"");
				songSelectButton->setEnabled(false);
				return;
			}
			if (state == SONGLIST_ART)
			{
				thumbImage->load(noimageFileName, noimageRect, true);
				songInfoContainer->setTextVisible(false);
				//sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;

				songSelectButton->setText(L"查看艺术家");
				songSelectButton->setEnabled(true);
				return;
			}

			SongListItem* item = (SongListItem*)songListBox->getItems()[index];
			if (item->getLook() == SongListItem::RANDOM)
			{
				thumbImage->load(randomFileName, randomRect, true);
				songInfoContainer->setTextVisible(false);
				//sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;
				return;
			}

			// refresh select button
			if(item->getDownloadFinished())
				songSelectButton->setText(L"选择歌曲");
			else
				songSelectButton->setText(L"下载歌曲");
			songSelectButton->setEnabled(true);
		
			// DISPLAY SONGLIST
			if (item->hasPreview())
			{
				thumbImage->load(item->getPreviewFilename(), gcn::Rectangle(0, 0, 0, 0), true);
				background->display(item->getPreviewFilename(), gcn::Rectangle(0, 0, 0, 0), true);
			}
			else
				thumbImage->load(noimageFileName, noimageRect, true);

			PlayListeningPreview(index);

			songInfoContainer->setMap(item->getMapInfo());
			songInfoContainer->setTextVisible(true);

			rankPage = 0;
			refreshRankPageText();
			refreshRankingList();
		}

		void MusicUI::SongSelectButtonClicked()
		{
			int index = songListBox->getSelectedIndex();

			if(index >= 0)
				SongListItemDoubleClicked(index);
		}

		void MusicUI::SongListItemDoubleClicked(int index)
		{
			if (state == SONGLIST_ART)
			{
				std::wstring des = ((SongListItem*)songListBox->getItems()[index])->getMapInfo().header.artists[0];
				songListBox->clearItems();
		
				songListBox->pushItem(songListOrigItems[0]);
				for (int i=0; i<songListOrigItems.size(); i++)
				{
					const std::vector<std::wstring>& p = ((SongListItem*)songListOrigItems[i])->getMapInfo().header.artists;
					for (int j=0; j<p.size(); j++)
					{
						if (p[j] == des)
						{
							songListBox->pushItem(songListOrigItems[i]);
							break;
						}
					}
				}

				songListBox->setDifButtonDisplayed(true);
				state = SONGLIST_SPEART;
				refreshSongList();
				return;
			}

			if (selectedListBox->getItemCount() >= 5)
				return;
		
			if (index == 0)
			{
				int t = GetRandomSong((divamap::DivaMap::LevelType)((SongListItem*)songListBox->getItem(0))->getDifIndex());
				if (t == -1)
				{
					mgr->GetMB()->Show(L"没有已下载的当前难度的歌曲");
					return;
				}
				SongListItem* item = (SongListItem*)songListBox->getItems()[t];
				selectedListBox->pushItem(item->getMapInfo(), (divamap::DivaMap::LevelType)((SongListItem*)songListBox->getItems()[0])->getDifIndex(), item, DivaSelectedListBox::SPECIFIC, gameMode);
				AdjustStartButton();
			}
			else
			{
				SongListItem* item = (SongListItem*)songListBox->getItems()[index];
				if (item->getDownloadFinished())
					selectedListBox->pushItem(item->getMapInfo(), item->getMapInfo().getLevel(item->getDifIndex()), item, DivaSelectedListBox::SPECIFIC, gameMode);
				else
				{
					MAPMGR.PrepareDivaMapData(item->getMapInfo().id);
					songSelectButton->setText(L"下载中...");
					songSelectButton->setEnabled(false);
				}
				AdjustStartButton();
			}
		}

		void MusicUI::AdjustStartButton()
		{
			startButton->setEnabled(true);
			return;

			// if no song
			if (selectedListBox->getItemCount() == 0)
			{
				startButton->setEnabled(false);
				return;
			}

			// if still players playing
			DivaRoomInfo info = playerList->getRoomInfo();
			for (int i=0; i<info.players.size(); i++)
				if (info.players[i].isVoid && info.players[i].isPlaying)
				{
					startButton->setEnabled(false);
					return;
				}
		
			// if select double player but first song isn't double player
			if (gameMode == 2 && selectedListBox->getSong(0).mapInfo.header.Couple != divamap::DivaMapHeader::Couple)
			{
				startButton->setEnabled(false);
				return;
			}

			// if select double player but not even people
			if (gameMode == 2)
			{
				int t = 1;
				for (int i=0; i<info.players.size(); i++)
					if (!info.players[i].isHost && info.players[i].isReady)
						t++;
				if (t&1)
				{
					startButton->setEnabled(false);
					return;
				}
			}

			// if relay but 1 person
			if (gameMode == 1)
			{
				int t = 1;
				for (int i=0; i<info.players.size(); i++)
					if (!info.players[i].isHost && info.players[i].isReady)
						t++;
				if (t == 1)
				{
					startButton->setEnabled(false);
					return;
				}
			}

			startButton->setEnabled(true);
		}

		void MusicUI::BackToUI()
		{
			if (playerList->getRoomInfo().myId != playerList->getRoomInfo().hostId)
				return;
			if (selectedListBox->getItemCount() == 0)
				return;
			selectedListBox->removeItem(0);
			AdjustStartButton();
		}

		void MusicUI::SongListHighlightItemChanged(int index)
		{
			
		}

		void MusicUI::GameStartPost(int mapId, int dif, int gameMode)
		{
			//SMDivaNetCode
			//////////////////////////////////////////////////////////////////////////
			// 有歌曲id还不够，我多写一个难易度指示，012表示easy normal hard
			//netCore.send(DIVA_NET_CTS_LOBBY_START, 0, "%d%d%d", gameMode + 1, mapId, dif);
			//////////////////////////////////////////////////////////////////////////
			//end
		}

		void MusicUI::GameStart(int mapId, int dif, int gameMode, int myPos)
		{
		}

		void MusicUI::GameOver()
		{
			//netCore.send(DIVA_NET_CTS_LOBBY_GAME_OVER);
		}

		void MusicUI::ReadyPost(int index, bool p)
		{

		}

		void MusicUI::refreshRankPageText()
		{
			int be = rankPage * 4 + 1;
			int en = be + 3;
			rankPageText->setText(iToWS(be) + L"~" + iToWS(en));
		}

		// DivaSlider

		void DivaSongSlider::markerPositionChanged(int v)
		{
			MusicUI::Instance()->songListBox->setFirstIndex(v);
		}

		//////////////////////////////////////////////////////////////////////////

		void ArtistButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			ui->songListBox->clearItems();

			for (std::map<std::wstring, ListItemEx*>::iterator i = ui->artistListItems.begin(); i != ui->artistListItems.end(); i++)
			{
				ui->songListBox->pushItem(i->second);
			}
			ui->songListBox->setDifButtonDisplayed(false);

			ui->state = MusicUI::SONGLIST_ART;
		}

		void SongButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI::Instance()->state = MusicUI::SONGLIST_ORIG;
			MusicUI::Instance()->songListBox->setDifButtonDisplayed(true);
			MusicUI::Instance()->songListBox->setItems(MusicUI::Instance()->songListOrigItems);
			MusicUI::Instance()->refreshSongList();
		}

		void GameModeSelectLeft_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			if (ui->gameMode > 0)
				ui->gameMode --;
			else
				ui->gameMode = ui->modeNames.size() - 1;
			ui->modeTextLabel->setText(ui->modeNames[ui->gameMode]);
			MusicUI::Instance()->AdjustStartButton();
		}

		void GameModeSelectRight_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			if (ui->gameMode < ui->modeNames.size() - 1)
				ui->gameMode ++;
			else
				ui->gameMode = 0;
			ui->modeTextLabel->setText(ui->modeNames[ui->gameMode]);
			MusicUI::Instance()->AdjustStartButton();
		}

		void RankPageLeft_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			if (ui->rankPage > 0)
			{
				ui->rankPage --;
				ui->refreshRankPageText();
				ui->refreshRankingList(true, false);
			}
		}

		void RankPageRight_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			if (ui->rankPage < ui->maxRankPage)
			{
				ui->rankPage ++;
				ui->refreshRankPageText();
				ui->refreshRankingList(true, false);
			}
		}

		void SongSelectButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			// !!
			MusicUI* ui = MusicUI::Instance();
			ui->SongSelectButtonClicked();
		}

		void PlayButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			//NextState = "house";
			MAPMGR.SelectedMap_Clear();
			int count = ui->selectedListBox->getItemCount();
			for (int i=0; i<count; i++)
			{
				const SongInfo& t= ui->selectedListBox->getSong(i);
				MAPMGR.SelectedMap_Add(t.mapInfo.id, t.getLevel(), static_cast<divamap::DivaMap::ModeType>(t.mode));
			}
			STAGE_CLIENT.refreshMusic();
			ui->musicGameState->beginLeave("house");
			//ui->GameStart(ui->selectedListBox->getSong(0).mapInfo.id, 
			//	ui->selectedListBox->getSong(0).difIndex, ui->gameMode, 0);

			//if (info.myId == info.hostId)
			//{
			//	ui->GameStartPost(ui->selectedListBox->getSong(0).mapInfo.mapId, 
			//		ui->selectedListBox->getSong(0).difIndex, ui->gameMode);
			//}
			//else
			//{
			//	ui->playerList->setPlayerReady(info.myId, info.players[info.myId].isReady ^ 1);
			//	//ui->ReadyPost(info.myId, info.players[info.myId].isReady ^ 1);
			//}
		}
	}
}