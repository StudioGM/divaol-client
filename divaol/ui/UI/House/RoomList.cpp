#include "RoomList.h"

#include "soraguiimage.hpp"

namespace diva
{
	namespace HouseUI
	{
		int RoomListItem::x1;
		int RoomListItem::x2;
		int RoomListItem::x3;
		int RoomListItem::x4;
		int RoomListItem::y1;
		int RoomListItem::y2;
		int RoomListItem::y3;
		int RoomListItem::y4;

		RoomListItem::RoomListItem()
		{
			image1 = NULL;
			image2 = NULL;
			image3 = NULL;
		}

		RoomListItem::~RoomListItem()
		{
			if (image1)
				delete image1;
			if (image2)
				delete image2;
			if (image3)
				delete image3;
		}

		void RoomListItem::setInfo(const Network::RoomInfo& info)
		{
			this->info = info;
		}

		void RoomListItem::setLook(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3)
		{
			this->image1 = gcn::SoraGUIImage::load(f1);
			this->r1 = r1;
			this->image2 = gcn::SoraGUIImage::load(f2);
			this->r2 = r2;
			this->image3 = gcn::SoraGUIImage::load(f3);
			this->r3 = r3;

			if (!image1 || !image2 || !image3)
				throw "fuck!";
		}

		void RoomListItem::setTextPosition(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
		{
			RoomListItem::x1 = x1;
			RoomListItem::y1 = y1;
			RoomListItem::x2 = x2;
			RoomListItem::y2 = y2;
			RoomListItem::x3 = x3;
			RoomListItem::y3 = y3;
			RoomListItem::x4 = x4;
			RoomListItem::y4 = y4;
		}

		void RoomListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			// image1是普通，image2是mouseon，image3是selected
			if (state == 0)
				graphics->drawImage(image1, r1.x, r1.y, 0, 0, r1.width, r1.height);
			else if (state == 1)
				graphics->drawImage(image2, r2.x, r2.y, 0, 0, r2.width, r2.height);
			else if (state == 2)
				graphics->drawImage(image3, r3.x, r3.y, 0, 0, r3.width, r3.height);
			
			graphics->drawTextW(info.stageName, x1, y1);
			if (info.selectedSong.size() != 0)
				graphics->drawTextW(info.selectedSong[0], x2, y2);
			else
				graphics->drawTextW(L"尚未选择歌曲", x2, y2);
			graphics->drawTextW(info.owner, x3, y3);
			graphics->drawTextW(sora::s2ws(sora::int_to_str(info.playerNum)), x4, y4);
		}

		// --------------------------

		RoomList::RoomList()
		{
		}

		RoomList::~RoomList()
		{
		}


	}
}