#pragma once

#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "soraguiimage.hpp"
#include <string>

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class HouseUIRoomInfoListItem : public ListItemEx
		{
		private:
			UINT color;
			std::wstring text;

		public:
			HouseUIRoomInfoListItem(const std::wstring& text = L"", UINT color = 0);
			void setColor(UINT color);
			void setText(const std::wstring& text);
			const std::wstring& getText() const;
			void draw(Graphics* graphics, Font* font, int state, int alpha);
		};
	}
}