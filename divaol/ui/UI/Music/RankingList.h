#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanPointEx.h"
#include "lib/wjson/wjson.h"

namespace diva
{
	namespace MusicUI
	{
		using namespace gcn;

		class RankingListItem : public ListItemEx
		{
		private:
			int rankResult;
			int ranking;
			int score;
			int combo;
			int mode;
			int maxCT;
			std::wstring playerName;
			int backColor;
			int fontColor;
			int isNull;
			bool isLoading;
			
			static Image* image, *rankImage;
			static gcn::Rectangle rect;
			static PointEx scoreP, comboP, rankingP, playerP, maxCTP, modeP;
			static std::vector<gcn::Rectangle> rankTexRect;
			static std::vector<gcn::PointEx> rankDrawPos;

		public:
			RankingListItem();
			~RankingListItem();
			void SetNull(int v);
			void SetLoading(bool v);
			void SetInfo(const int& score, const int& combo, const int &mode, const int &maxCT, const int &rankResult, const std::wstring& playerName);
			void SetRanking(int ranking);
			void SetColor(int backColor, int fontColor);
			void SetRankImage();
			int GetRanking() const;
			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

			static void LoadBack(const std::wstring& filename, const gcn::Rectangle& rect);
			static void LoadFromJson(const WJson::Value& conf);
		};

		class RankingList : public ListBoxEx
		{
		private:

		public:

		};
	}
}