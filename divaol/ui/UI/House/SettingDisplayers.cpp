#include "SettingDisplayers.h"

namespace diva
{
	namespace HouseUI
	{
		SpecialItemDisplayer::SpecialItemDisplayer()
		{
			selectedIndex = -1;
			items.clear();
			setRepeat(false);
		}

		SpecialItemDisplayer::~SpecialItemDisplayer()
		{
		}

		int SpecialItemDisplayer::getSelectedIndex() const
		{
			return selectedIndex;
		}

		void SpecialItemDisplayer::setSelectedIndex(int v)
		{
			selectedIndex = v;
			if (selectedIndex >= items.size())
				selectedIndex = items.size() - 1;
		}

		const std::wstring& SpecialItemDisplayer::getSelectedItem() const
		{
			if (selectedIndex == -1)
				throw "no item!";
			return items[selectedIndex];
		}

		void SpecialItemDisplayer::pushItem(const std::wstring& str)
		{
			items.push_back(str);
			if (selectedIndex == -1)
				selectedIndex = 0;
		}

		void SpecialItemDisplayer::draw(Graphics* graphics, Font* font, int alpha)
		{
			if (items.size() == 0)
				return;
			int w = graphics->getCurrentClipArea().width;
			int h = graphics->getCurrentClipArea().height;
			graphics->setColor(gcn::Color(255, 255, 255, alpha));
			graphics->setFont(font);
			graphics->drawTextW(items[selectedIndex], w/2 - font->getWidthW(items[selectedIndex]) / 2, h/2 - font->getHeight() / 2);
		}

		void SpecialItemDisplayer::setRepeat(bool v)
		{
			repeat = v;
		}

		void SpecialItemDisplayer::ToNext()
		{
			selectedIndex ++;
			if (selectedIndex >= items.size())
			{
				if (repeat)
					selectedIndex = 0;
				else
					selectedIndex--;
			}
		}

		void SpecialItemDisplayer::ToLast()
		{
			selectedIndex --;
			if (selectedIndex < 0)
			{
				if (repeat)
					selectedIndex = items.size() - 1;
				else
					selectedIndex ++;
			}
		}
	}
}