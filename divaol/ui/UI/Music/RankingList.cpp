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
			SetColor(0xFFFFFF, 0xFFFFFF);
			isNull = false;
			isLoading = false;
		}

		RankingListItem::~RankingListItem()
		{
		}

		void RankingListItem::SetNull(int v)
		{
			isNull = v;
		}

		void RankingListItem::SetLoading(bool v)
		{
			isLoading = v;
		}

		void RankingListItem::SetColor(int backColor, int fontColor)
		{
			this->backColor = backColor;
			this->fontColor = fontColor;
		}

		void RankingListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			if (image)
			{
				graphics->setColor(gcn::Color(backColor, alpha));
				graphics->drawImage(image, rect.x, rect.y, 0, 0, rect.width, rect.height);
			}

			if (isLoading)
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"正在读取...", playerP.x, playerP.y);
			}
			else if (isNull == 0)
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"Score:" + Base::String::any2string<int>(score), scoreP.x, scoreP.y);
				graphics->drawTextW(Base::String::any2string<int>(ranking), rankingP.x, rankingP.y);
				graphics->drawTextW(L"Combo:" + Base::String::any2string<int>(combo), comboP.x, comboP.y);
				graphics->drawTextW(L"By:" + playerName, playerP.x, playerP.y);
			}
			else if (isNull == 1)
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"暂无记录", playerP.x, playerP.y);
			}
			else
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"尚没有你的排名", playerP.x, playerP.y);
			}
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