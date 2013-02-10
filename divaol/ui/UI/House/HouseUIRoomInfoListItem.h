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
			std::string uid;
			std::wstring text;

		public:
			HouseUIRoomInfoListItem(const std::wstring& text = L"", const std::string &uid="", UINT color = 0);
			std::string getUid() {return uid;}
			void setUid(const std::string &uid) {this->uid=uid;}
			void setColor(UINT color);
			void setText(const std::wstring& text);
			const std::wstring& getText() const;
			void draw(Graphics* graphics, Font* font, int state, int alpha);
		};
	}
}