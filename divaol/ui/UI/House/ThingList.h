#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;



		class ThingListItem : public ListItemEx
		{
		private:
			Image* backImage;
			Image* backImageSelected;
			gcn::Rectangle backR;
			gcn::Rectangle backRS;

		public:
			ThingListItem(const std::wstring& backFilename, gcn::Rectangle backRect, const std::wstring& backSFilename, gcn::Rectangle backSRect);
			~ThingListItem();

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};

		class TeamListItem : public ListItemEx
		{
		private:
			Image *image1, *image2, *image3;
			gcn::Rectangle r1, r2, r3;
		public:

			TeamListItem(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3);
			~TeamListItem();

			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);
		};

		class ThingList : public ListBoxEx
		{
		protected:
			int selectedItem;
			virtual int getItemState(int index);

		public:
			ThingList();
			~ThingList();
			
			void setSelectedItem(int index);
			int getSelectedItem() const;

			void itemClicked(int itemIndex);
		};

	}
}