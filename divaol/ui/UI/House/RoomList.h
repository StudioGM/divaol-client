#pragma once

#include "ui/GUIChanEx/GUIChanListViewEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "ui/GUIChanEx/GUIChanSliderEx.h"
#include "ui/structure.h"
#include "ui/DivaNetwork/DivaNetwork.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class SongListItem : public ListItemEx
		{
		private:
			Image *image;
			gcn::Rectangle srcRect;
			std::wstring str;

		public:
			SongListItem(const std::wstring& filename, gcn::Rectangle srcRect);
			virtual ~SongListItem();

			//void setLook();
			void setText(const std::wstring& str);
			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};

		class StageListItem : public ListItemEx
		{
		public:

			struct StagePlayerInfo
			{
				PlayerInfo playerInfo;
				int teamIndex;
				int slot;
				int status;
			};

			typedef std::vector<unsigned int> TeamColors;

		private:
			static TeamColors teamColors;
			static Image* readyImage;
			static Image* hostImage;
			static gcn::Rectangle readyRect;
			static gcn::Rectangle hostRect;

			Image *image;
			gcn::Rectangle srcRect;
			StagePlayerInfo info;

		public:

			StageListItem(const std::wstring& filename, gcn::Rectangle srcRect);
			virtual ~StageListItem();

			//void setLook();
			void setInfo(const StagePlayerInfo& info) {this->info=info;}
			const StagePlayerInfo& getInfo() const;
			void setTeamColor(int colorIndex);
			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

			static void setTeamColors(const TeamColors& v);
			static void setIcons(const std::wstring& rFile, const gcn::Rectangle& rRect, 
				const std::wstring& hFile, const gcn::Rectangle& hRect);
		};

		class RoomListItem : public ListItemEx
		{
		private:
			Image *image1, *image2, *image3;
			gcn::Rectangle r1, r2, r3;
			Network::RoomInfo info;
			static int x1, y1, x2, y2, x3, y3, x4, y4;

		public:

			RoomListItem();
			~RoomListItem();
			void setInfo(const Network::RoomInfo& info);
			Network::RoomInfo getInfo() const {return info;}
			void setLook(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3);
			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

			static void setTextPosition(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
		};

		class RoomListSlider : public SliderEx
		{
		private:

		public:
			virtual void markerPositionChanged(int v);
		};

		class RoomList : public ListViewEx
		{
		private:
			std::wstring f1, f2, f3;
			gcn::Rectangle r1, r2, r3;
			bool isWaiting;
			float nowWaitingRest;
			static const int WaitTimeLimit = 5;

		public:
			RoomList();
			~RoomList();

			void waiting();
			void roomCallback() {isWaiting = false;}
			bool isWaitingRoomCallback() const {return isWaiting;}

			void setRoomItemBaseInfo(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3);
			void pushRoomItem(const Network::RoomInfo& info);

			virtual void firstPageChanged(int newPage);
			virtual void itemClicked(int itemIndex);
		};
	}
}