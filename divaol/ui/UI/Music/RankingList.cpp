#include "RankingList.h"
#include "lib/Base/String.h"
#include "ui/GUIChanEx/GUIChanJsonHelper.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace MusicUI
	{
		Image* RankingListItem::image, *RankingListItem::rankImage = NULL;
		gcn::Rectangle RankingListItem::rect;
		gcn::PointEx RankingListItem::scoreP;
		gcn::PointEx RankingListItem::comboP;
		gcn::PointEx RankingListItem::rankingP;
		gcn::PointEx RankingListItem::playerP;
		gcn::PointEx RankingListItem::maxCTP;
		gcn::PointEx RankingListItem::modeP;
		std::vector<gcn::Rectangle> RankingListItem::rankTexRect;
		std::vector<gcn::PointEx> RankingListItem::rankDrawPos;

		RankingListItem::RankingListItem()
		{
			SetRanking(1);
			SetInfo(0, 0, 0, 0, 0, L"none");
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
				graphics->drawTextW(L"分数:" + Base::String::any2string<int>(score), scoreP.x, scoreP.y);
				graphics->drawTextW(L"#" + Base::String::any2string<int>(ranking), rankingP.x, rankingP.y);
				graphics->drawTextW(L"连击:" + Base::String::any2string<int>(combo), comboP.x, comboP.y);

				// temporarily hard code here
				static const wchar_t* CTText[] = {L"零段",L"一段",L"二段",L"三段",L"四段",L"五段",L"六段",L"爆表"};
				
				graphics->drawTextW(CTText[maxCT], maxCTP.x, maxCTP.y);
				graphics->drawTextW(L"模式:" + MAPMGR.GetModeStr(mode), modeP.x, modeP.y);
				graphics->drawTextW(L"By:" + playerName, playerP.x, playerP.y);

				if (rankImage && rankResult < rankDrawPos.size() && rankResult < rankTexRect.size()) {
					graphics->setColor(gcn::Color(CARGB(0,255,255,255), alpha));

					graphics->drawImage(rankImage, rankTexRect[rankResult].x, rankTexRect[rankResult].y,
												   rankDrawPos[rankResult].x, rankDrawPos[rankResult].y,
												   rankTexRect[rankResult].width, rankTexRect[rankResult].height);
				}
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

		void RankingListItem::SetInfo(const int& score, const int& combo, const int &mode, const int &maxCT, const int &rankResult, const std::wstring& playerName)
		{
			this->score = score;
			this->combo = combo;
			this->playerName = playerName;
			this->mode = mode;
			this->maxCT = maxCT;
			this->rankResult = rankResult;
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

			rankTexRect.clear();
			rankDrawPos.clear();
			if(conf.isMember(L"ranks")) {
				const WJson::Value &ranks = conf[L"ranks"];
					if(ranks[L"texture"].asString() != L"") {
					rankImage = Image::load(ranks[L"texture"].asString());

					for(int i = 0; i < ranks[L"positions"].size(); i++) {
						rankTexRect.push_back(Helper::GetRect(ranks[L"positions"][i][L"src"]));
						rankDrawPos.push_back(Helper::GetPoint(ranks[L"positions"][i][L"tar"]));
					}
				}
			}
		}
	}
}