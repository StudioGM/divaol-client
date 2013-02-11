#include "ThingList.h"

#include "soraguiimage.hpp"

namespace diva
{
	namespace HouseUI
	{

		TeamListItem::TeamListItem(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3)
		{
			this->image1 = gcn::SoraGUIImage::load(f1);
			this->r1 = r1;
			this->image2 = gcn::SoraGUIImage::load(f2);
			this->r2 = r2;
			this->image3 = gcn::SoraGUIImage::load(f3);
			this->r3 = r3;

			if (!image1 || !image2 || !image3)
				throw "fuck!";
		}

		TeamListItem::~TeamListItem()
		{
			if (image1)
				delete image1;
			if (image2)
				delete image2;
			if (image3)
				delete image3;
		}

		void TeamListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			// image1是普通，image2是mouseon，image3是selected
			if (state == 0)
				graphics->drawImage(image1, r1.x, r1.y, 0, 0, r1.width, r1.height);
			else if (state == 1)
				graphics->drawImage(image3, r3.x, r3.y, 0, 0, r3.width, r3.height);
			else if (state == 2)
				graphics->drawImage(image2, r2.x, r2.y, 0, 0, r2.width, r2.height);
		}

		//////////////////////////////////////////////////////////////////////////

		ThingListItem::ThingListItem(const std::wstring& backFilename, gcn::Rectangle backRect,
			const std::wstring& backSFilename, gcn::Rectangle backSRect)
		{
			backImage = gcn::SoraGUIImage::load(backFilename);
			if (!backImage)
				throw "fuck";
			backR = backRect;

			backImageSelected = gcn::SoraGUIImage::load(backSFilename);
			if (!backImageSelected)
				throw "haha";
			backRS = backSRect;
		}

		ThingListItem::~ThingListItem()
		{
			if (backImage)
				delete backImage;
			if (backImageSelected)
				delete backImageSelected;
		}

		void ThingListItem::draw(Graphics* graphics, Font* font, int state, int alpha)
		{
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			if (state == 0)
				graphics->drawImage(backImage, backR.x, backR.y, 0, 0, backR.width, backR.height);
			else
				graphics->drawImage(backImageSelected, backRS.x, backRS.y, 0, 0, backRS.width, backRS.height);
		}

		//////////////////////////////////////////////////////////////////////////

		ThingList::ThingList()
		{
			setHorizontal(true);
		}

		ThingList::~ThingList()
		{
		}

		void ThingList::setSelectedItem(int index)
		{
			if (index < -1 || index >= getItemCount() || index == selectedItem)
				selectedItem = -1;
			else
				selectedItem = index;
		}

		int ThingList::getSelectedItem() const
		{
			return selectedItem;
		}

		void ThingList::itemClicked(int itemIndex)
		{
			setSelectedItem(itemIndex);
		}

		int ThingList::getItemState(int index)
		{
			if (index == selectedItem)
				return 1;
			if (index == highlightItemIndex)
				return 2;
			return 0;
		}
	}

}