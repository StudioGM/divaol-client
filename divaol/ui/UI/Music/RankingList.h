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
			int ranking;
			int score;
			int combo;
			std::wstring playerName;
			int backColor;
			int isNull;
			
			static Image* image;
			static gcn::Rectangle rect;
			static PointEx scoreP, comboP, rankingP, playerP;

		public:
			RankingListItem();
			~RankingListItem();
			void SetNull(int v);
			void SetInfo(const int& score, const int& combo, const std::wstring& playerName);
			void SetRanking(int ranking);
			void SetColor(int color);
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