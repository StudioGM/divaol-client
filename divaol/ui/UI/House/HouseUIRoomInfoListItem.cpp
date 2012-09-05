#include "HouseUIRoomInfoListItem.h"

namespace diva
{
	namespace HouseUI
	{

		void HouseUIRoomInfoListItem::setColor(UINT color)
		{
			this->color = color;
		}

		HouseUIRoomInfoListItem::HouseUIRoomInfoListItem(const std::wstring& text, UINT color)
		{
			setText(text);
			setColor(color);
		}

		void HouseUIRoomInfoListItem::setText(const std::wstring& text)
		{
			this->text = text;
		}

		void HouseUIRoomInfoListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(color));
			graphics->setFont(font);
			graphics->drawTextW(text, 0, 0);
		}
	}
}