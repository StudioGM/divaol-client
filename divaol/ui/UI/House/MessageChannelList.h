#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class MessageChannelListItem : public ListItemEx
		{
		private:
			Image* image;
			gcn::Rectangle srcRect;
			std::wstring text;

		public:
			MessageChannelListItem(const std::wstring& filename, gcn::Rectangle srcRect, const std::wstring& text);
			MessageChannelListItem();
			virtual ~MessageChannelListItem();
			void setLook(const std::wstring& filename, gcn::Rectangle srcRect, const std::wstring& text);

			std::wstring getText() const;

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};

		class MessageChannelList
			: public ListBoxEx
		{
		private:
			
		public:
			MessageChannelList();

			virtual void itemClicked(int itemIndex);
		};
	}
}