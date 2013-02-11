/*
 *  gcnDraggableIcon.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_DRAGGABLE_ICON_H_
#define GCN_DRAGGABLE_ICON_H_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	class DraggableIcon: public Icon, public MouseListener {
	public:
		DraggableIcon();
		DraggableIcon(const std::string& icon);
		DraggableIcon(Image* image);
		virtual ~DraggableIcon() {}
		
		void setImage(const std::string& image);
		/*
		 inheritant from mouse listener
		 */
		virtual void mousePressed(MouseEvent& mouseEvent);
        virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);
		virtual void mouseEntered(MouseEvent& mouseEvent);		
        virtual void mouseExited(MouseEvent& mouseEvent);
		
		virtual void drawFrame(Graphics* graphics);
		virtual void draw(Graphics* graphics);
		
		void enableDrag(bool flag);
		bool isDragEnabled() const;
		
		void enableFrameWhenMouseEntered(bool flag);
		bool isFrameEnabledWhenMouseEntered() const;
		
		void enableDragFrame(bool flag);
		bool isDragFrameEnabled() const;
		
	protected:
		int mDragOffsetX, mDragOffsetY;
		
		bool mDraggable;
		bool mMousePressed;
		bool mHasMouse;
		bool mDragFrame;
		bool mFrameMouseEntered;
	};
	
} // namespace gcn

#endif