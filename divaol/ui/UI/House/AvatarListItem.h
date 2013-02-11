#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "ui/GUIChanEx/GUIChanAnimeBoxEx.h"
#include "ui/GUIChanEx/GUIChanPointEx.h"
#include "lib/wjson/wjson.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class AvatarListItem : 
			public ListItemEx
		{
		private:
			std::wstring name;
			PointEx playerNamePos;
			gcn::Rectangle avatarRect;
			AnimeBoxEx* animeBox;
			ImageTileEx* shadow;
			gcn::Rectangle shadowDesRect;

		public:
			
			AvatarListItem(AnimeBoxEx* animeBox, const gcn::Rectangle& avatarRect, const PointEx& playerNamePos, const std::wstring& name);
			const std::wstring& getName() const;
			void update(float dt);
			static AvatarListItem* FromJson(const WJson::Value& v, const std::wstring& name);

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};
	}
}