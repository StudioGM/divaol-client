/*
 *  SelectableContainer.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_SELECTABLE_CONTAINER_H_
#define GCN_SELECTABLE_CONTAINER_H_

#include "SoraGUIChan/guichan.hpp"
#include "gcnBackgroundContainer.h"

namespace gcn {
	
	/*
	 A container in which you can select it's children widgets by mouse
	 Only apply to ranged select, means you can only use your mouse to select some widgets within a rectangle or other shape by this container
	 Because the concern of gcnDraggableItem extension, you can drag them along without the help of this container
	 Selected Widgets would be highlighted by frame with framesize = 1
	 So make sure your widget can display frame
	 
	 every dragging frame, widget will receive request focus event
	 */
	class SelectableContainer: public BackgroundContainer, public MouseListener {
	public:
		SelectableContainer();
		virtual ~SelectableContainer();
		
		typedef std::list<Widget*> WidgetList;
		WidgetList getChildrenInRange(const Rectangle& box);
		
		// get current selected children
		WidgetList& getSelectedChildren();
		
		virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);
		
		virtual void setSelectable(bool flag);
		virtual bool isSelectable() const;
		
		virtual void draw(Graphics* graphics);
		
		virtual void setSelectionColor(const Color& color);
		
		// release all selected widgets
		void releaseSelection();

	protected:
		typedef enum {
			SELECT_RECT,
			SELECT_CIRCLE,
		} SELECT_TYPE;
		
		inline WidgetList _getChildrenInRange(const Rectangle& box, SELECT_TYPE type);
		
		Rectangle selectRect;	
		int mDragOffsetX, mDragOffsetY;
		WidgetList selectedWidgets;
		
		bool bSelectable;
		bool bSelecting;
		
		int mOffsetX;
		int mOffsetY;
		bool bDraggingWidgetList;
	};
	
}



#endif // GNC_SELECTABLE_CONTAINER_H_
