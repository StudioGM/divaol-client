#include "RoomList.h"

#include "soraguiimage.hpp"
#include "divanetwork/DivaNetworkManager.h"
#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		SongListItem::SongListItem(const std::wstring& filename, gcn::Rectangle srcRect)
		{
			image = gcn::SoraGUIImage::load(filename);
			this->srcRect = srcRect;

			str = L"";
		}

		SongListItem::~SongListItem()
		{
			if (image)
				delete image;
		}

		void SongListItem::setText(const std::wstring& str)
		{
			this->str = str;
		}

		void SongListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);

			graphics->setFont(font);
			graphics->drawTextW(str, 20, 15);
		}

		//////////////////////////////////////////////////////////////////////////

		StageListItem::TeamColors StageListItem::teamColors;
		Image* StageListItem::readyImage = NULL;
		Image* StageListItem::hostImage = NULL;
		gcn::Rectangle StageListItem::readyRect;
		gcn::Rectangle StageListItem::hostRect;

		StageListItem::StageListItem(const std::wstring& filename, gcn::Rectangle srcRect)
		{
			//teamColors.clear();
			image = gcn::SoraGUIImage::load(filename);
			this->srcRect = srcRect;
		}

		StageListItem::~StageListItem()
		{
			if (image)
				delete image;
		}

		void StageListItem::setIcons(const std::wstring& rFile, const gcn::Rectangle& rRect, const std::wstring& hFile, const gcn::Rectangle& hRect)
		{
			readyImage = gcn::SoraGUIImage::load(rFile);
			readyRect = rRect;

			hostImage = gcn::SoraGUIImage::load(hFile);
			hostRect = hRect;
		}

		const StageListItem::StagePlayerInfo& StageListItem::getInfo() const
		{
			return info;
		}

		void StageListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			if (info.playerInfo.id == 0)
				return;

			graphics->setColor(teamColors[info.teamIndex] + (alpha << 24));
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);

			// ---- Draw text
			graphics->setFont(font);
			//graphics->setColor(((~teamColors[info.teamIndex]) & 0xFFFFFF) + (alpha << 24));
			graphics->setColor(0);
			graphics->drawTextW(Base::String::any2string(info.playerInfo.id), 112, 17);
			graphics->setColor(0xFFFFFFFF);
			graphics->drawTextW(Base::String::any2string(info.playerInfo.id), 110, 15);
			
			// ---- Draw Icon
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			if (info.slot == 1)
				graphics->drawImage(hostImage, hostRect.x, hostRect.y, 60, 15, hostRect.width, hostRect.height);
			else if (info.status)
				graphics->drawImage(readyImage, readyRect.x, readyRect.y, 54, 12, readyRect.width, readyRect.height);
		}

		void StageListItem::setTeamColors(const StageListItem::TeamColors& v)
		{
			teamColors = v;
		}

		void StageListItem::setTeamColor(int v)
		{
			info.teamIndex = v;
		}

		//////////////////////////////////////////////////////////////////////////

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

		void RoomList::setRoomItemBaseInfo(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3)
		{
			this->f1 = f1;
			this->r1 = r1;
			this->f2 = f2;
			this->r2 = r2;
			this->f3 = f3;
			this->r3 = r3;
		}
		
		void RoomList::pushRoomItem(const Network::RoomInfo& info)
		{
			RoomListItem* b = new RoomListItem();
			b->setInfo(info);
			b->setLook(f1, r1, f2, r2, f3, r3);
			pushItem(b);
		}

		void RoomList::itemClicked(int itemIndex)
		{
			RoomListItem * item = dynamic_cast<RoomListItem*>(items[itemIndex]);
			STAGE_CLIENT.join(Base::String(item->getInfo().owner));
			HouseUI::Instance()->mgr->GetMB()->Show(L"加入房间中...", L"提示", gcn::MessageBoxEx::TYPE_NONE);
		}

		void RoomList::firstPageChanged(int newPage)
		{
			HouseUI::Instance()->RoomListFirstPageChanged(newPage);
		}

		//////////////////////////////////////////////////////////////////////////

		void RoomListSlider::markerPositionChanged(int v)
		{
			HouseUI::Instance()->RoomListSliderSlided(v);
		}
	}
}