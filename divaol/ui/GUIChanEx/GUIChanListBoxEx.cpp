#include "ui/GUIChanEx/GUIChanListBoxEx.h"

namespace gcn
{
	ListBoxEx::ListBoxEx()
	{
		setOpaque(false);
		setHorizontal(false);
		setSelectMode(false);
		setOutline(false);

		selectedItemIndex = -1;
		highlightItemIndex = -1;
		selectIndex = -1;
		maxItem = 5;
		firstIndex = 0;
		image = NULL;
		firstRect = Rectangle(0, 0, 100, 100);
		itemGap = 10;
		items.clear();
		isMousePressed = false;

		addMouseListener(this);
		addKeyListener(this);
	}

	ListBoxEx::~ListBoxEx()
	{
		if (image)
		{
			delete image;
			image = NULL;
		}
		for (int i=0; i<items.size(); i++)
			delete items[i];
	}

	void ListBoxEx::setOutline(bool v)
	{
		isDrawOutline = v;
	}

	ListItemEx* ListBoxEx::getItem(int index) const
	{
		return items[index];
	}

	void ListBoxEx::setHorizontal(bool v)
	{
		isHorizontal = v;
	}
	
	void ListBoxEx::setMaxItem(int num)
	{
		maxItem = num;
	}

	void ListBoxEx::setFirstIndex(int index)
	{
		int moto = firstIndex;
		firstIndex = index;
		if (firstIndex < 0)
			firstIndex = 0;
		if (firstIndex >= items.size())
			firstIndex = items.size();
		if (moto!=firstIndex)
			firstIndexChanged(firstIndex);
	}

	void ListBoxEx::setSelectMode(bool v)
	{
		isSelectMode = v;
	}

	int ListBoxEx::getMaxIndex() const
	{
		if (items.size() <= maxItem)
			return 0;
		return items.size() - maxItem;
	}

	int ListBoxEx::getFirstIndex() const
	{
		return firstIndex;
	}

	int ListBoxEx::getDisplayedItems() const
	{
		if (items.size() - firstIndex > maxItem)
			return maxItem;
		return items.size() - firstIndex;
	}

	int ListBoxEx::getHighlightItemIndex() const
	{
		return highlightItemIndex;
	}

	void ListBoxEx::pushItem(ListItemEx* item)
	{
		items.push_back(item);
		itemChanged();
	}

	void ListBoxEx::insertItem(ListItemEx* item, int index)
	{
		items.insert(items.begin() + index, item);
		itemChanged();
	}

	void ListBoxEx::removeItem(int index)
	{
		items.erase(items.begin() + index);
		if (selectedItemIndex != -1)
		{
			if (selectedItemIndex == index)
				clearSelect();
			else
			{
				if (selectedItemIndex > index)
					selectedItemIndex --;
				if (selectedItemIndex >= items.size())
					clearSelect();
			}
		}
		
		itemChanged();
	}

	void ListBoxEx::loadImage(const std::wstring& filename, Rectangle srcRect)
	{
		if (image)
		{
			delete image;
			image = NULL;
		}

		image = Image::load(filename, true);
		this->srcRect = srcRect;
	}

	void ListBoxEx::setGap(Rectangle firstRect, int itemGap)
	{
		this->firstRect = firstRect;
		this->itemGap = itemGap;
	}

	int ListBoxEx::getMaxItem() const
	{
		return maxItem;
	}

	void ListBoxEx::clearItems()
	{
		clearSelect();
		if (items.size() == 0)
			return;
		items.clear();
		itemChanged();
	}

	void ListBoxEx::adjustMyWidth()
	{
		if (!isHorizontal)
			setWidth(firstRect.width + firstRect.x);
		else
			setWidth(firstRect.x + maxItem * (firstRect.width + itemGap) - itemGap);
	}

	void ListBoxEx::clearSelect()
	{
		selectedItemIndex = -1;
	}

	int ListBoxEx::getSelectedIndex() const
	{
		return selectedItemIndex;
	}

	int ListBoxEx::getItemState(int index)
	{
		if (index != highlightItemIndex && index != selectedItemIndex)
			return 0;
		if (index == selectedItemIndex)
			return 2;
		if (!isMousePressed)
			return 1;
		return 2;
	}

	void ListBoxEx::draw(Graphics* graphics)
	{
		if (image)
		{
			graphics->setColor(Color(255,255,255,getAlpha()));
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);
		}

		// draw my items
		for (int i=firstIndex; i<items.size() && i<firstIndex+maxItem; i++)
		{
			if (!isHorizontal)
			{
				graphics->pushClipArea( Rectangle(firstRect.x, 
					firstRect.y + (i - firstIndex) * (itemGap + firstRect.height), 
					firstRect.width, 
					firstRect.height) );
			}
			else
			{
				graphics->pushClipArea( Rectangle(firstRect.x + (i - firstIndex) * (itemGap + firstRect.width), 
					firstRect.y, 
					firstRect.width, 
					firstRect.height) );
			}
			items[i]->draw(graphics, getFont(), getItemState(i), getAlpha());
			graphics->popClipArea();
		}

		// draw my outline
		if (isDrawOutline)
			graphics->drawRectangle(gcn::Rectangle(0, 0, getWidth() - 1, getHeight() - 1));

		// father draw
		Container::draw(graphics);
	}

	void ListBoxEx::mouseEntered(MouseEvent& mouseEvent)
	{
	}

	void ListBoxEx::mouseExited(MouseEvent& mouseEvent)
	{
		int ori = highlightItemIndex;
		highlightItemIndex = -1;
		if (ori != highlightItemIndex)
			highlightItemChanged(highlightItemIndex);
	}

	void ListBoxEx::changeHighlightItem(int mx, int my, gcn::Rectangle firstRect, int itemGap)
	{
		int ori = highlightItemIndex;

		int x = mx, y = my;
		y -= firstRect.y;
		x -= firstRect.x;
		if (x > firstRect.width || x < 0)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		if (y < 0)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		int ind = y / (firstRect.height + itemGap);
		y %= firstRect.height + itemGap;
		if (y > firstRect.height)
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}
		if (ind >= getDisplayedItems())
		{
			highlightItemIndex = -1;
			if (ori != highlightItemIndex)
				highlightItemChanged(highlightItemIndex);
			return;
		}

		highlightItemIndex = ind + firstIndex;
		if (ori != highlightItemIndex)
			highlightItemChanged(highlightItemIndex);
	}

	void ListBoxEx::mouseMoved(MouseEvent& mouseEvent)
	{
		if (!isHorizontal)
			changeHighlightItem(mouseEvent.getX(), mouseEvent.getY(), firstRect, itemGap);
		else
			changeHighlightItem(mouseEvent.getY(), mouseEvent.getX(), gcn::Rectangle(firstRect.y, firstRect.x, firstRect.height, firstRect.width), itemGap);
	}

	void ListBoxEx::mousePressed(MouseEvent& mouseEvent)
	{
		isMousePressed = true;
	}

	void ListBoxEx::mouseReleased(MouseEvent& mouseEvent)
	{
		isMousePressed = false;
	}

	void ListBoxEx::mouseClicked(MouseEvent& mouseEvent)
	{
		if (mouseEvent.getButton() == MouseEvent::LEFT && highlightItemIndex >= 0 && highlightItemIndex < items.size())
		{
			if (isSelectMode)
			{
				int lastIndex = selectedItemIndex;
				selectedItemIndex = highlightItemIndex;
				itemClickedSelectedMode(highlightItemIndex, lastIndex);
			}
			itemClicked(highlightItemIndex);
		}
	}

	void ListBoxEx::itemClickedSelectedMode(int itemIndex, int lastSelectedIndex)
	{
	}

	void ListBoxEx::itemClicked(int itemIndex)
	{

	}

	void ListBoxEx::itemChanged()
	{

	}

	void ListBoxEx::firstIndexChanged(int v)
	{

	}

	void ListBoxEx::highlightItemChanged(int index)
	{

	}

	int ListBoxEx::getItemCount() const
	{
		return items.size();
	}

	std::vector<ListItemEx*>& ListBoxEx::getItems()
	{
		return items;
	}

	void ListBoxEx::setItems(const std::vector<ListItemEx*>& v)
	{
		items.clear();
		for (int i=0; i<v.size(); i++)
			items.push_back(v[i]);
		clearSelect();
		itemChanged();

	}

	void ListBoxEx::mouseWheelMovedUp(MouseEvent& mouseEvent)
	{
		scrollLast();
	}

	void ListBoxEx::mouseWheelMovedDown(MouseEvent& mouseEvent)
	{
		scrollNext();
	}

	void ListBoxEx::adjustMyHeight()
	{
		if (!isHorizontal)
			setHeight(firstRect.y + maxItem * (firstRect.height + itemGap) - itemGap);
		else
			setHeight(firstRect.height + firstRect.y);
	}

	void ListBoxEx::scrollNext()
	{
		if (items.size() <= maxItem)
			return;
		if (getFirstIndex() < items.size() - maxItem)
			setFirstIndex(getFirstIndex() + 1);
	}

	void ListBoxEx::scrollLast()
	{
		if (getFirstIndex() > 0)
			setFirstIndex(getFirstIndex() - 1);
	}
}