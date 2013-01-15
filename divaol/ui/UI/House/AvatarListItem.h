#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
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
			Image* avatarImage;

		public:
			
			AvatarListItem(const std::wstring& avatarFilename, const gcn::Rectangle& avatarRect, const PointEx& playerNamePos, const std::wstring& name);
			static AvatarListItem* FromJson(const WJson::Value& v, const std::wstring& name);

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};
	}
}