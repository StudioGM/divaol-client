#include "ui/GUIChanEx/GUIChanListBoxEx.h"

namespace gcn
{
	ListBoxEx::ListBoxEx()
	{
		setOpaque(false);

		highlightItemIndex = -1;
		selectIndex = -1;
		maxItem = 5;
		firstIndex = 0;
		image = NULL;
		firstRect = Rectangle(0, 0, 100, 100);
		itemGap = 10;
		items.clear();

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

	void ListBoxEx::clearItems()
	{
		if (items.size() == 0)
			return;
		items.clear();
		itemChanged();
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
			graphics->pushClipArea( Rectangle(firstRect.x, 
				firstRect.y + (i - firstIndex) * (itemGap + firstRect.height), 
				firstRect.width, 
				firstRect.height) );
			if (i!=highlightItemIndex)
				items[i]->draw(graphics, getFont(), 0, 255);
			else if (i==highlightItemIndex && !isMousePressed)
				items[i]->draw(graphics, getFont(), 1, 255);
			else
				items[i]->draw(graphics, getFont(), 2, 255);
			graphics->popClipArea();
		}

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

	void ListBoxEx::mouseMoved(MouseEvent& mouseEvent)
	{
		int ori = highlightItemIndex;

		int x = mouseEvent.getX(), y = mouseEvent.getY();
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
		if (highlightItemIndex >= 0 && highlightItemIndex < items.size())
			itemClicked(highlightItemIndex);
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

	const std::vector<ListItemEx*>& ListBoxEx::getItems() const
	{
		return items;
	}

	void ListBoxEx::setItems(const std::vector<ListItemEx*>& v)
	{
		items.clear();
		for (int i=0; i<v.size(); i++)
			items.push_back(v[i]);
		itemChanged();
	}
}