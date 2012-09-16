#include "MusicUI.h"
#include "SoraResourceFile.h"
#include "ui/Config/DivaUIConfig.h"

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
			sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
			top->setVisible(true);
		}

		void MusicUI::Leave()
		{
			sora::GCN_GLOBAL->getTop()->remove(top);
			top->setVisible(false);
		}



		MusicUI::MusicUI()
		{
			// initialize net

			//initNet();

			//////////////////////////////////////////////////////////////////////////

			top = new gcn::Container();
			top->setSize(1920, 1080);
			top->setOpaque(false);
			
			countStarted = false;

			// load font
			artistFont = new gcn::SoraGUIFont(L"msyh.ttf", 30);
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
			top->add(thumbImage, 803, 132);

			// ThumbFrameContainer

			thumbFrameContainter = new gcn::ContainerEx();
			thumbFrameContainter->load(L"res/UI2.png", gcn::Rectangle(0,0,412,251));
			thumbFrameContainter->setSize(412,251);
			top->add(thumbFrameContainter, 781,110);

			// SongInfoContainer

			songInfoFont = new gcn::SoraGUIFont(L"msyh.ttf", 16);

			songInfoContainer = new DivaSongInfoContainer();
			songInfoContainer->setFont(songInfoFont);
			songInfoContainer->load(L"res/UI2.png", gcn::Rectangle(412,414,553,295));
			songInfoContainer->setSize(553,295);
			top->add(songInfoContainer, 700, 391);
		
			// SelectedListBox

			selectedListBox = new DivaSelectedListBox();
			top->add(selectedListBox, 693 + 19, 710 - 19);


			// ModeLeftButton
			modeLeftButton = new gcn::ButtonEx();
			modeLeftButton->setLook(L"res/UI1.png", gcn::Rectangle(111, 326, 64, 51),
				gcn::Rectangle(111, 204, 64, 51),
				gcn::Rectangle(111, 262, 64, 51),
				gcn::Rectangle(111, 326, 64, 51),
				-14, -10);
			modeLeftButton->addMouseListener(new GameModeSelectLeft_MouseListener());
			modeLeftButton->setSize(36, 25);
			top->add(modeLeftButton, 1539 - 50, 690 + 20);

			// ModeRightButton
			modeRightButton = new gcn::ButtonEx();
			modeRightButton->setLook(L"res/UI1.png", gcn::Rectangle(235, 326, 64, 51),
				gcn::Rectangle(235, 204, 64, 51),
				gcn::Rectangle(235, 262, 64, 51),
				gcn::Rectangle(235, 326, 64, 51),
				-14, -10);
			modeRightButton->addMouseListener(new GameModeSelectRight_MouseListener());
			modeRightButton->setSize(36, 25);
			top->add(modeRightButton, 1745 - 50, 690 + 20);

			// ModeTextLabel
			modeTextLabel = new gcn::LabelEx(L"普通");
			modeTextLabel->setSize(90, 50);
			modeTextLabel->setForegroundColor(gcn::Color(0, 0, 0));
			modeTextLabel->setFont(artistFont);
			top->add(modeTextLabel, 1625 - 50, 670 + 20);

			// PlayerList
			playerList = new DivaPlayerList();
			playerList->setFont(artistFont);
			playerList->init();
			top->add(playerList, 1385, 179);

			// StartButton
			startButton = new gcn::ButtonEx();
			startButton->setSize(410,169);
			DivaRoomInfo rinfo = playerList->getRoomInfo(); 
			if (rinfo.myId == rinfo.hostId)
			{
				startButton->setLook(L"res/UI2.png",
					gcn::Rectangle(979,104,450,206),
					gcn::Rectangle(979,310,450,206),
					gcn::Rectangle(979,516,450,206),
					gcn::Rectangle(979,104,450,206),
					-19, -20);
				startButton->setEnabled(false);
			}
			else
			{
				startButton->setLook(L"res/UI2.png",
					gcn::Rectangle(1430,207,450,206),
					gcn::Rectangle(1430,413,450,206),
					gcn::Rectangle(1430,619,450,206),
					gcn::Rectangle(1430,207,450,206),
					-19, -20);
			}
			startButton->addMouseListener(new PlayButton_MouseListener());
			top->add(startButton, 1397, 818);


			//////////////////////////////////////////////////////////////////////////


			// ------
			divamap::DivaMap tMap;
			//
			
			songListOrigItems.clear();
			using namespace divamap;
			std::vector<DivaMap> orig;
			for (std::map<int, DivaMap>::iterator i = MAPS.begin(); i != MAPS.end(); i++)
				orig.push_back(i->second);
			
			gcn::Image* image = gcn::Image::load("res/UI1.png");
			songListOrigItems.push_back(new SongListItem(image, tMap, SongListItem::RANDOM));
			for (int i=0; i<orig.size(); i++)
				songListOrigItems.push_back(new SongListItem(image, orig[i], SongListItem::SONG));
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
					playTimer.reset();
					sora::SoraBGMManager::Instance()->play(listenFileName, false);
					countStarted = false;
				}
			}
		}

		void MusicUI::SongListItemClicked(int index)
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
				return;
			}

			if (selectedListBox->getItemCount() >= 5)
				return;
		
			if (index == 0)
			{
				int t = sora::SoraCore::Instance()->RandomInt(1, songListBox->getItemCount() - 1);
				SongListItem* item = (SongListItem*)songListBox->getItems()[t];
				selectedListBox->pushItem(item->getMapInfo(), ((SongListItem*)songListBox->getItems()[0])->getDifIndex(), DivaSelectedListBox::RANDOM); 
				AdjustStartButton();
			}
			else
			{
				SongListItem* item = (SongListItem*)songListBox->getItems()[index];
				selectedListBox->pushItem(item->getMapInfo(), item->getDifIndex(), DivaSelectedListBox::SPECIFIC);
				AdjustStartButton();
			}
		}

		void MusicUI::AdjustStartButton()
		{
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
			if (index == -1)
			{
				sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;
				return;
			}
			if (state == SONGLIST_ART)
			{
				thumbImage->load(noimageFileName, noimageRect, true);
				songInfoContainer->setTextVisible(false);
				sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;
				return;
			}
			SongListItem* item = (SongListItem*)songListBox->getItems()[index];
			if (item->getLook() == SongListItem::RANDOM)
			{
				thumbImage->load(randomFileName, randomRect, true);
				songInfoContainer->setTextVisible(false);
				sora::SoraBGMManager::Instance()->stop(false);
				countStarted = false;
				return;
			}
		
			// DISPLAY SONGLIST
			//if (item->getMapInfo().mapThumbFileName != L"NONE")
			//{
			//	thumbImage->load(item->getMapInfo().mapThumbFileName, gcn::Rectangle(0, 0, 0, 0), true);
			//	background->display(item->getMapInfo().mapThumbFileName, gcn::Rectangle(0, 0, 0, 0), true);
			//}
			//else
			//	thumbImage->load(noimageFileName, noimageRect, true);

			//if (item->getMapInfo().listenFileName != L"NONE" && item->getMapInfo().listenFileName != L"")
			//{
			//	countStarted = true;
			//	listenFileName = item->getMapInfo().listenFileName;
			//	playTimer.reset();
			//	playTimer.start();
			//}
			//else
			//{
			//	sora::SoraBGMManager::Instance()->stop(false);
			//	countStarted = false;
			//}

			songInfoContainer->setMap(item->getMapInfo());
			songInfoContainer->setTextVisible(true);
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

		void PlayButton_MouseListener::mouseClicked(MouseEvent& mouseEvent)
		{
			MusicUI* ui = MusicUI::Instance();
			DivaRoomInfo info = ui->playerList->getRoomInfo();
			ui->GameStart(ui->selectedListBox->getSong(0).mapInfo.id, 
				ui->selectedListBox->getSong(0).difIndex, ui->gameMode, 0);
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