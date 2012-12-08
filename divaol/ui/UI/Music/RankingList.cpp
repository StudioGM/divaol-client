#include "RankingList.h"
#include "lib/Base/String.h"
#include "ui/GUIChanEx/GUIChanJsonHelper.h"

namespace diva
{
	namespace MusicUI
	{
		Image* RankingListItem::image;
		gcn::Rectangle RankingListItem::rect;
		gcn::PointEx RankingListItem::scoreP;
		gcn::PointEx RankingListItem::comboP;
		gcn::PointEx RankingListItem::rankingP;
		gcn::PointEx RankingListItem::playerP;

		RankingListItem::RankingListItem()
		{
			SetRanking(1);
			SetInfo(0, 0, L"none");
			SetColor(0xFFFFFFFF);
		}

		RankingListItem::~RankingListItem()
		{
		}

		void RankingListItem::SetColor(int color)
		{
			this->backColor = color;
		}

		void RankingListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			if (image)
			{
				graphics->setColor(backColor);
				graphics->drawImage(image, rect.x, rect.y, 0, 0, rect.width, rect.height);
			}

			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->setFont(font);
			graphics->drawTextW(L"Score:" + Base::String::any2string<int>(score), scoreP.x, scoreP.y);
			graphics->drawTextW(Base::String::any2string<int>(ranking), rankingP.x, rankingP.y);
			graphics->drawTextW(L"Combo:" + Base::String::any2string<int>(combo), comboP.x, comboP.y);
			graphics->drawTextW(L"By:" + playerName, playerP.x, playerP.y);
		}

		void RankingListItem::SetInfo(const int& score, const int& combo, const std::wstring& playerName)
		{
			this->score = score;
			this->combo = combo;
			this->playerName = playerName;
		}

		void RankingListItem::LoadBack(const std::wstring& filename, const gcn::Rectangle& rect)
		{
			image = Image::load(filename);
			RankingListItem::rect = rect;
		}

		void RankingListItem::SetRanking(int ranking)
		{
			this->ranking = ranking;
		}

		void RankingListItem::LoadFromJson(const WJson::Value& conf)
		{
			rankingP = Helper::GetPoint(conf[L"ranking"]);
			scoreP = Helper::GetPoint(conf[L"score"]);
			comboP = Helper::GetPoint(conf[L"combo"]);
			playerP = Helper::GetPoint(conf[L"player"]);
		}
	}
}