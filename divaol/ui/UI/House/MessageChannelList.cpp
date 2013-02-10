#include "MessageChannelList.h"
#include "soraguiimage.hpp"

#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		MessageChannelListItem::MessageChannelListItem()
		{
			image = NULL;
		}

		MessageChannelListItem::MessageChannelListItem(const std::wstring& filename, gcn::Rectangle srcRect, const std::wstring& text)
		{
			setLook(filename, srcRect, text);
		}

		MessageChannelListItem::~MessageChannelListItem()
		{
			if (image)
				delete image;
		}

		void MessageChannelListItem::setLook(const std::wstring& filename, gcn::Rectangle srcRect, const std::wstring& text)
		{
			image = gcn::SoraGUIImage::load(filename);
			if (!image)
				throw "fuck";
			this->srcRect = srcRect;
			this->text = text;
		}

		void MessageChannelListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);

			graphics->setFont(font);
			graphics->drawTextW(text, 
				(graphics->getCurrentClipArea().width - font->getWidthW(text)) / 2,
				(graphics->getCurrentClipArea().height - font->getHeight()) / 2);
		}

		std::wstring MessageChannelListItem::getText() const
		{
			return text;
		}

		//////////////////////////////////////////////////////////////////////////

		MessageChannelList::MessageChannelList()
		{
			//setGap(gcn::Rectangle(0, 0, 
		}

		void MessageChannelList::itemClicked(int itemIndex)
		{
			HouseUI::Instance()->MessagePanelChannelListClicked(itemIndex);
		}
	}
}