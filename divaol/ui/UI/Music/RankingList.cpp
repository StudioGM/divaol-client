#include "RankingList.h"
#include "lib/Base/String.h"
#include "ui/GUIChanEx/GUIChanJsonHelper.h"
#include "divasongmgr/DivaMapManager.h"

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
		gcn::PointEx RankingListItem::maxCTP;
		gcn::PointEx RankingListItem::modeP;

		RankingListItem::RankingListItem()
		{
			SetRanking(1);
			SetInfo(0, 0, 0, 0, L"none");
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
				graphics->drawTextW(L"���ڶ�ȡ...", playerP.x, playerP.y);
			}
			else if (isNull == 0)
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"Score:" + Base::String::any2string<int>(score), scoreP.x, scoreP.y);
				graphics->drawTextW(Base::String::any2string<int>(ranking), rankingP.x, rankingP.y);
				graphics->drawTextW(L"Combo:" + Base::String::any2string<int>(combo), comboP.x, comboP.y);
				graphics->drawTextW(L"MaxCT:" + Base::String::any2string<int>(maxCT), maxCTP.x, maxCTP.y);
				graphics->drawTextW(L"Mode:" + MAPMGR.GetModeDescription(mode), modeP.x, modeP.y);
				graphics->drawTextW(L"By:" + playerName, playerP.x, playerP.y);
			}
			else if (isNull == 1)
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"���޼�¼", playerP.x, playerP.y);
			}
			else
			{
				graphics->setColor(gcn::Color(fontColor, alpha));
				graphics->setFont(font);
				graphics->drawTextW(L"��û���������", playerP.x, playerP.y);
			}
		}

		void RankingListItem::SetInfo(const int& score, const int& combo, const int &mode, const int &maxCT, const std::wstring& playerName)
		{
			this->score = score;
			this->combo = combo;
			this->playerName = playerName;
			this->mode = mode;
			this->maxCT = maxCT;
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
			maxCTP = Helper::GetPoint(conf[L"maxCT"]);
			modeP = Helper::GetPoint(conf[L"mode"]);
			playerP = Helper::GetPoint(conf[L"player"]);
		}
	}
}