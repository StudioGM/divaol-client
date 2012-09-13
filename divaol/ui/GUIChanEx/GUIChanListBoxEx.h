#pragma once

#include "guichan.hpp"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "SoraSprite.h"
#include "SoraCore.h"
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include <string>
#include <vector>

namespace gcn
{
	/*
	 * an expended vision of gcn::ListBox.
	 * It supports that every item has different renderer.
	 */

	class ListBoxEx : 
		public Container, 
		public MouseListener, 
		public KeyListener
	{
	protected:
		bool isMousePressed;
		int firstIndex;
		int maxItem;
		int selectIndex;
		int highlightItemIndex;
		Rectangle firstRect;
		int itemGap;
		int itemHeight;
		std::vector<ListItemEx*> items;
		Image* image;
		Rectangle srcRect;
		bool isHorizontal;

		void changeHighlightItem(int mx, int my, gcn::Rectangle firstRect, int itemGap);

		virtual int getItemState(int index);

	public:
		ListBoxEx();
		~ListBoxEx();

		void loadImage(const std::wstring& filename, Rectangle srcRect);

		void pushItem(ListItemEx* item);
		void insertItem(ListItemEx* item, int index);
		void removeItem(int index);

		void setGap(Rectangle firstRect, int itemGap);
		void setMaxItem(int num);

		void setItemHeight(int height);

		void setFirstIndex(int index);
		int getFirstIndex() const;

		void setHorizontal(bool v = false);

		int getItemCount() const;

		const std::vector<ListItemEx*>& getItems() const;
		void setItems(const std::vector<ListItemEx*>& v);

		virtual int getDisplayedItems() const;
		int getMaxIndex() const;

		void clearItems();


		virtual void draw(Graphics* graphics);

		virtual void mouseMoved(MouseEvent& mouseEvent);

		virtual void mouseEntered(MouseEvent& mouseEvent);

		virtual void mouseExited(MouseEvent& mouseEvent);

		virtual void mousePressed(MouseEvent& mouseEvent);

		virtual void mouseReleased(MouseEvent& mouseEvent);

		virtual void mouseClicked(MouseEvent& mouseEvent);

		virtual void itemClicked(int itemIndex);

		virtual void itemChanged();

		virtual void firstIndexChanged(int v);

		virtual void highlightItemChanged(int index);

		virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);
		virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

		virtual void scrollNext();
		virtual void scrollLast();

		//virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

		//virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

		//virtual void mouseDragged(MouseEvent& mouseEvent);
	};
}