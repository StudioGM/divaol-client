/*
 *  SelectableContainer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnSelectableContainer.h"
#include <cmath>

namespace gcn {

	SelectableContainer::SelectableContainer(): bSelectable(true), bDraggingWidgetList(false) {
		setOpaque(false);
		addMouseListener(this);
		setSelectionColor(Color(255, 255, 255, 155));
	}

	SelectableContainer::~SelectableContainer() {
	}

	void SelectableContainer::mousePressed(MouseEvent& event) {
		// the event have been consumed by a children
		// we do nothing in this case

		if(event.isConsumed() && event.getButton() == MouseEvent::LEFT) {
			Widget* pw = event.getSource();
			if(selectedWidgets.size() != 0) {
				WidgetListIterator itWidget = selectedWidgets.begin();
				while(itWidget != selectedWidgets.end()) {
					if(pw == (*itWidget) && pw->isEnabled()) {
						bDraggingWidgetList = true;
						MouseListener* ml = dynamic_cast<MouseListener*> (pw);
						ml->mouseReleased(event);
						pw->setFrameSize(1);

						mOffsetX = event.getX();
						mOffsetY = event.getY();
						break;
					}
					++itWidget;
				}

			}
			return;
		}

		if(!bSelectable || event.isConsumed()) {
			return;
		}

		if(event.getButton() == MouseEvent::LEFT) {
			selectRect.setAll(event.getX(), event.getY(), 0, 0);
			bSelecting = true;

			mDragOffsetX = event.getX();
			mDragOffsetY = event.getY();

			releaseSelection();
			event.consume();
		} else {
			releaseSelection();
		}
	}

	void SelectableContainer::mouseReleased(MouseEvent& event) {
		if(event.getButton() == MouseEvent::LEFT) {
			bSelecting = false;
			bDraggingWidgetList = false;

			event.consume();

			if(selectRect.width == 0 || selectRect.height == 0)
				releaseSelection();
		}
	}

	void SelectableContainer::mouseDragged(MouseEvent& event) {
		if(bDraggingWidgetList) {
			WidgetListIterator itWidget = selectedWidgets.begin();
			int mdx = event.getX() - mOffsetX;
			int mdy = event.getY() - mOffsetY;
			while(itWidget != selectedWidgets.end()) {
				if((*itWidget)->isEnabled()) {
					(*itWidget)->setPosition((*itWidget)->getX() + mdx, (*itWidget)->getY() + mdy);
				}
				++itWidget;
			}
			mOffsetX = event.getX();
			mOffsetY = event.getY();
		}
		else if(bSelecting) {
			int eventX = event.getX();
			int eventY = event.getY();
			releaseSelection();

			selectRect.setAll(eventX<mDragOffsetX?eventX:mDragOffsetX,
							  eventY<mDragOffsetY?eventY:mDragOffsetY,
							  std::abs((float)(event.getX()-mDragOffsetX)),
							  std::abs((float)(event.getY()-mDragOffsetY)));

			// get and mark all selected widgets
			selectedWidgets = getChildrenInRange(selectRect);
			WidgetListIterator itWidget = selectedWidgets.begin();
			while(itWidget != selectedWidgets.end()) {
				(*itWidget)->setFrameSize(1);
				++itWidget;
			}
		}
	}

	void SelectableContainer::releaseSelection() {
		WidgetListIterator itWidget = selectedWidgets.begin();
		while(itWidget != selectedWidgets.end()) {
			(*itWidget)->setFrameSize(0);
			++itWidget;
		}
		selectedWidgets.clear();
		selectRect.setAll(0, 0, 0, 0);
	}

	void SelectableContainer::setSelectable(bool flag) {
		bSelectable = flag;
	}

	bool SelectableContainer::isSelectable() const {
		return bSelectable;
	}

	SelectableContainer::WidgetList& SelectableContainer::getSelectedChildren() {
		return selectedWidgets;
	}

	SelectableContainer::WidgetList SelectableContainer::getChildrenInRange(const Rectangle& rect) {
		WidgetListIterator itWidget = mWidgets.begin();
		WidgetList newList;
		while(itWidget != mWidgets.end()) {
			if((*itWidget)->isEnabled() && rect.isPointInRect((*itWidget)->getX(), (*itWidget)->getY()))
				newList.push_back(*itWidget);
			++itWidget;
		}
		return newList;
	}

	SelectableContainer::WidgetList SelectableContainer::_getChildrenInRange(const Rectangle& rect, SelectableContainer::SELECT_TYPE type) {
		switch(type) {
			case SELECT_RECT:
				return getChildrenInRange(rect);
			case SELECT_CIRCLE:
				break;
		}
		WidgetList tmpList;
		return tmpList;
	}

	void SelectableContainer::draw(Graphics* graphics) {
		BackgroundContainer::draw(graphics);

		if(bSelectable && bSelecting) {
			graphics->setColor(getSelectionColor());
			graphics->drawLine(selectRect.x, selectRect.y, selectRect.x+selectRect.width, selectRect.y);
			graphics->drawLine(selectRect.x+selectRect.width, selectRect.y, selectRect.x+selectRect.width, selectRect.y+selectRect.height);
			graphics->drawLine(selectRect.x, selectRect.y+selectRect.height, selectRect.x+selectRect.width, selectRect.y+selectRect.height);
			graphics->drawLine(selectRect.x, selectRect.y, selectRect.x, selectRect.y+selectRect.height);
		}
	}

	void SelectableContainer::setSelectionColor(const Color& color) {
		WidgetListIterator itWidget = mWidgets.begin();
		while(itWidget != mWidgets.end()) {
			(*itWidget)->setSelectionColor(color);
			++itWidget;
		}
	}


} // namespace gcn
