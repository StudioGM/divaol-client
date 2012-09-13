#pragma once

#include "GUIChanListBoxEx.h"

namespace gcn
{
	class ListViewEx : public ListBoxEx
	{
	private:
		int xGap;
		int yGap;
		int xItemCount;
		int yItemCount;
		int firstPage;

		int calcNewHighLightItem(int x, int y);

	public:
		ListViewEx();
		~ListViewEx();
		
		void setGap(Rectangle firstRect, int xGap, int yGap);
		void setMaxItem(int xItemCount, int yItemCount);
		int getFirstPage() const;
		void setFirstPage(int v);
		int getMaxPage() const;
		int getFullMaxPage() const;
		virtual int getDisplayedItems() const;

		virtual void adjustSize();

		virtual void mouseMoved(MouseEvent& mouseEvent);
		virtual void firstPageChanged(int newPage);

		virtual void draw(Graphics* graphics);

		virtual void scrollNext();
		virtual void scrollLast();
	};
}