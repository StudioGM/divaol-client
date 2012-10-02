#pragma once

#include "SoraCore.h"
#include "SoraTask.h"
#include "guichan.hpp"
#include "guichansetup.h"
#include "SoraShader.h"

#include "ui/structure.h"
#include "ui/network.h"
#include "divasongmgr/DivaMapManager.h"

#include "ui/GUIChanEx/GUIChanEx.h"
#include "DivaSongListItem.h"
#include "DivaSongList.h"
#include "DivaSongInfoContainer.h"
#include "DivaSelectedListBox.h"
#include "DivaPlayerItem.h"
#include "DivaPlayerList.h"
#include "SoraSoundManager/SoraBGMManager.h"
#include "Lib/wjson/wjson.h"
#include "timer/DivaTimeCounterImplSora.h"

#include <map>

namespace diva
{
	namespace MusicUI
	{
		class DivaSongSlider:
			public gcn::SliderEx
		{
		public:
			virtual void markerPositionChanged(int v);
		};

		class MusicGameState;

		//////////////////////////////////////////////////////////////////////////
		class DivaSelectedListBox;

		class MusicUI
		{
		public:
			MusicGameState* musicGameState;

			int gameStartingMapId;
			int gameStartingMyPos;
			int gameStartingMode;
			int gameStartingDif;

			divacore::TimeCounterImpl playTimer;
			bool countStarted;
			std::wstring listenFileName;

			std::vector<ListItemEx*> songListOrigItems;
			std::map<std::wstring, ListItemEx*> artistListItems;
			gcn::Container* top;

			gcn::ImageDisplayerEx* background;
			gcn::ContainerEx* backPanel;
			DivaSongList* songListBox;
			gcn::ButtonEx* filterButton;
			gcn::ButtonEx* leftArtistButton;
			gcn::ButtonEx* leftSongButton;
			DivaSongSlider* slider;
			gcn::ContainerEx* thumbFrameContainter;
			gcn::ContainerEx* thumbImage;
			DivaSongInfoContainer* songInfoContainer;
			gcn::Font* songInfoFont;
			gcn::Font* artistFont;
			DivaSelectedListBox* selectedListBox;
			gcn::ButtonEx* modeLeftButton;
			gcn::ButtonEx* modeRightButton;
			gcn::LabelEx* modeTextLabel;
			gcn::ButtonEx* startButton;
			DivaPlayerList* playerList;
			gcn::ContainerEx* songListLogo;

			std::wstring noimageFileName;
			gcn::Rectangle noimageRect;
			std::wstring randomFileName;
			gcn::Rectangle randomRect;

			std::vector<std::wstring> modeNames;
			int maxMode;
			int gameMode;

			Image* uiImage1, *uiImage2;

			SongListItem* item1;

			sora::SoraTask* taskLogin;
			sora::SoraSprite* sprite;

			Network::HostInfo hostInfo;

			//NetCore netCore;

			MusicUI();
			int state;
			enum {SONGLIST_ORIG, SONGLIST_ART, SONGLIST_SPEART};
			enum {NETSTATE_OFFLINE, NETSTATE_ONLINE};

			int netState;
			void refreshSelectedSongList();

		public:
			static MusicUI* Instance() {static MusicUI i; return &i;}
			~MusicUI();
			void Render();
			void Update(float delta);

			void Enter();
			void Leave();
			void SetFatherState(MusicGameState* state);

			void SongListItemClicked(int index);
			void SongListHighlightItemChanged(int index);

			// Event
			void LoginButton_Clicked();
			void LoginWindowCancelButton_Clicked();
			void LoginWindowLoginButton_Clicked();
			void LoginTask(sora::SoraTask* task);

			void BackToUI();
			void AdjustStartButton();

			// net
			void GameStartPost(int mapId, int dif, int gameMode);
			void GameStart(int mapId, int dif, int gameMode, int myPos);
			void ReadyPost(int index, bool p);
			void GameOver();
			void initNet();
			void updateNet(float delta);

			Network::LoginResult Net_Login();
		};

		//////////////////////////////////////////////////////////////////////////

	
		class ArtistButton_MouseListener
			: public gcn::MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent);
		};

		class SongButton_MouseListener
			: public gcn::MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent);
		};

		class GameModeSelectLeft_MouseListener
			: public gcn::MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent);
		};

		class GameModeSelectRight_MouseListener
			: public gcn::MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent);
		};

		class PlayButton_MouseListener
			:public gcn::MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent);
		};
	}
}