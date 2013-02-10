/*
 *  gcnImageDraggable.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "gcnDraggableImageButton.h"

namespace gcn {
	
	DraggableImageButton::DraggableImageButton() {
		mDraggable = true;
		mDragFrame = false;
		mFrameMouseEntered = false;
		mDragOffsetX = mDragOffsetY	= 0;
		setFrameSize(0);

	}

	void DraggableImageButton::mousePressed(MouseEvent& mouseEvent) {
		// not this button
		/*if(mouseEvent.getSource() != this)
			return;
		*/
		if(!mDraggable || !mEnabled || mouseEvent.isConsumed())
			return;
		
		
		if (mouseEvent.getButton() == MouseEvent::LEFT) {
            mMousePressed = true;
			mDragged = false;
            mouseEvent.consume();
			
			mDragOffsetX = mouseEvent.getX();
			mDragOffsetY = mouseEvent.getY();
		
        }
	}
	
	void DraggableImageButton::mouseReleased(MouseEvent& mouseEvent) {
		if (mouseEvent.getButton() == MouseEvent::LEFT
            && mMousePressed
            && mHasMouse)
        {
            mMousePressed = false;
			/*
			 distribute action only when the button is not dragged
			 */
			if(!mDragged && !mouseEvent.isConsumed())
				distributeActionEvent();
			
			if(mDragFrame && !mouseEvent.isConsumed())
				setFrameSize(0);
			
            mouseEvent.consume();
			
        }
        else if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = false;
            mouseEvent.consume();
        }
	}
	
	
	void DraggableImageButton::mouseDragged(MouseEvent& mouseEvent) {
		if(mMousePressed) {/*
							if (mouseEvent.isConsumed() || mouseEvent.getSource() != this) {
							return;
							}*/
			
			setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
					mouseEvent.getY() - mDragOffsetY + getY());
			
			mouseEvent.consume();
			mDragged = true;
						
			if(mDragFrame)
				setFrameSize(1);
		}
	}
	
	void DraggableImageButton::mouseExited(MouseEvent& mouseEvent) {
        mHasMouse = false;
		if(mFrameMouseEntered)
			setFrameSize(0);
		ImageButton2::mouseExited(mouseEvent);
    }
	
    void DraggableImageButton::mouseEntered(MouseEvent& mouseEvent) {
        mHasMouse = true;
		if(mFrameMouseEntered)
			setFrameSize(1);
		ImageButton2::mouseEntered(mouseEvent);
    }
	
	bool DraggableImageButton::isDragEnabled() const {
		return mDraggable;
	}
	
	void DraggableImageButton::enableDrag(bool flag) {
		mDraggable = flag;
	}
	
	bool DraggableImageButton::isDragFrameEnabled() const {
		return mDragFrame;
	}
	
	void DraggableImageButton::enableDragFrame(bool flag) {
		mDragFrame = flag;
	}
	
	void DraggableImageButton::enableFrameWhenMouseEntered(bool flag) {
		mFrameMouseEntered = flag;
	}
	
	bool DraggableImageButton::isFrameEnabledWhenMouseEntered() const {
		return mFrameMouseEntered;
	}

} // namespace gcn