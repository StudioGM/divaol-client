#include "GUIChanListViewEx.h"

namespace gcn
{
	ListViewEx::ListViewEx()
	{
		xGap = 0;
		yGap = 0;
		xItemCount = 2;
		yItemCount = 2;
		firstPage = 0;
	}

	ListViewEx::~ListViewEx()
	{
	}

	void ListViewEx::setGap(Rectangle firstRect, int xGap, int yGap)
	{
		this->firstRect = firstRect;
		this->xGap = xGap;
		this->yGap = yGap;
	}

	void ListViewEx::setMaxItem(int xItemCount, int yItemCount)
	{
		this->xItemCount = xItemCount;
		this->yItemCount = yItemCount;
	}

	int ListViewEx::getMaxPage() const
	{
		if (getItemCount() == 0)
			return 0;
		return (getItemCount()-1) / xItemCount;
	}

	int ListViewEx::getFullMaxPage() const
	{
		if (getItemCount() <= xItemCount * yItemCount)
			return 0;
		return ((getItemCount() - xItemCount * yItemCount) - 1) / xItemCount + 1;
	}

	int ListViewEx::getDisplayedItems() const
	{
		int t = getItemCount() - getFirstPage() * xItemCount;
		if (t <= xItemCount * yItemCount)
			return t;
		return xItemCount * yItemCount;
	}

	void ListViewEx::adjustSize()
	{
		setWidth(xItemCount * (firstRect.width + xGap) - xGap);
		setHeight(yItemCount * (firstRect.height + yGap) - yGap);
	}

	void ListViewEx::setFirstPage(int v)
	{
		if (v < 0 || v > getMaxPage())
			throw "NULL";
		int moto = firstPage;
		firstPage = v;
		if (firstPage != moto)
			firstPageChanged(firstPage);
	}

	int ListViewEx::getFirstPage() const
	{
		return firstPage;
	}

	int ListViewEx::calcNewHighLightItem(int x, int y)
	{
		Rectangle r = firstRect;
		int w = (r.width + xGap) * xItemCount - xGap;
		int h = (r.height + yGap) * yItemCount - yGap;
		
		x -= r.x;
		y -= r.y;

		if (x < 0 || x >= w || y < 0 || y >= h)
			return -1;

		int xc = x / (r.width + xGap);
		x %= (r.width + xGap);
		int yc = y / (r.height + yGap);
		y %= (r.height + yGap);

		if (!(x < r.width && y < r.height))
			return -1;
		int index = yc * xItemCount + xc;
		if (index >= getDisplayedItems())
			return -1;

		return getFirstPage() * xItemCount + index;
	}

	void ListViewEx::firstPageChanged(int newPage)
	{

	}

	void ListViewEx::mouseMoved(MouseEvent& mouseEvent)
	{
		int res = calcNewHighLightItem(mouseEvent.getX(), mouseEvent.getY());
		if (res != highlightItemIndex)
		{
			highlightItemIndex = res;
			highlightItemChanged(highlightItemIndex);
		}
	}

	void ListViewEx::scrollNext()
	{
		if (getFirstPage() < getFullMaxPage())
			setFirstPage(getFirstPage() + 1);
	}

	void ListViewEx::scrollLast()
	{
		if (getFirstPage() > 0)
			setFirstPage(getFirstPage() - 1);
	}

	void ListViewEx::draw(Graphics* graphics)
	{
		if (image)
		{
			graphics->setColor(Color(255,255,255,getAlpha()));
			graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);
		}

		// draw my items
		int b1 = firstPage * xItemCount, b2 = b1 + getDisplayedItems();
		for (int i = b1; i < b2; i++)
		{
			int r = (i - b1) / xItemCount;
			int c = (i - b1) % xItemCount;
			graphics->pushClipArea( Rectangle(firstRect.x + c * (xGap + firstRect.width),
				firstRect.y + r * (yGap + firstRect.height), 
				firstRect.width, 
				firstRect.height) );
			items[i]->draw(graphics, getFont(), getItemState(i), getAlpha());
			graphics->popClipArea();
		}

		// father draw
		Container::draw(graphics);
	}
}