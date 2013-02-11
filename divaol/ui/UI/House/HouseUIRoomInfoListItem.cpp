#include "HouseUIRoomInfoListItem.h"

namespace diva
{
	namespace HouseUI
	{

		void HouseUIRoomInfoListItem::setColor(UINT color)
		{
			this->color = color;
		}

		HouseUIRoomInfoListItem::HouseUIRoomInfoListItem(const std::wstring& text, const std::string &uid, UINT color)
		{
			setUid(uid);
			setText(text);
			setColor(color);
		}

		const std::wstring& HouseUIRoomInfoListItem::getText() const
		{
			return text;
		}

		void HouseUIRoomInfoListItem::setText(const std::wstring& text)
		{
			this->text = text;
		}

		void HouseUIRoomInfoListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			if (state == 1)
			{
				graphics->setColor(gcn::Color(255, 255, 255, 100));
				graphics->fillRectangle(gcn::Rectangle(0, 0, graphics->getCurrentClipArea().width, graphics->getCurrentClipArea().height));
			}

			graphics->setColor(gcn::Color(color));
			graphics->setFont(font);
			graphics->drawTextW(text, 0, 0);
		}
	}
}