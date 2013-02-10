/*
 *  gcnImageDraggable.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_DRAGGABLE_IMAGE_H_
#define GCN_DRAGGABLE_IMAGE_H_

#include "gcnImageButton2.h"

namespace gcn {
	
	/*
		a widget that can drag using the mouse
		also can treat as a button which can both move and triggered
	 */
	class DraggableImageButton: public ImageButton2 {
	public:
		DraggableImageButton();
		virtual ~DraggableImageButton() {}
		/*
		 inheritant from mouse listener
		 */
		virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);
		virtual void mouseEntered(MouseEvent& mouseEvent);
		virtual void mouseExited(MouseEvent& mouseEvent);
		
		void enableDrag(bool flag);
		bool isDragEnabled() const;
		
		void enableDragFrame(bool flag);
		bool isDragFrameEnabled() const;
		
		void enableFrameWhenMouseEntered(bool flag);
		bool isFrameEnabledWhenMouseEntered() const;
				
	protected:
		int mDragOffsetX, mDragOffsetY;
		
		bool mDraggable;
		bool mDragged;
		bool mDragFrame;
		bool mFrameMouseEntered;
	};
	
} // namespace gcn

#endif