/*
 *  gcnDraggableIcon.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnDraggableIcon.h"

namespace gcn {

	DraggableIcon::DraggableIcon(): mDraggable(true), mFrameMouseEntered(false), mDragOffsetX(0), mDragOffsetY(0) {
		setFrameSize(0);
		setSelectionColor(Color(255, 255, 255, 255));
		addMouseListener(this);
	}
	
	DraggableIcon::DraggableIcon(const std::string& icon): mDraggable(true), mFrameMouseEntered(false), mDragOffsetX(0), mDragOffsetY(0), Icon(icon) {
		setFrameSize(0);
		setSelectionColor(Color(255, 255, 255, 255));
		addMouseListener(this);
	}
	
	DraggableIcon::DraggableIcon(Image* image): mDraggable(true), mFrameMouseEntered(false), mDragOffsetX(0), mDragOffsetY(0), Icon(image) {
		setFrameSize(0);	
		setSelectionColor(Color(255, 255, 255, 255));
		addMouseListener(this);
	}
	
	void DraggableIcon::mousePressed(MouseEvent& mouseEvent) {
		// not this button
		/*if(mouseEvent.getSource() != this)
		 return;
		 */
		if(!mDraggable || !mEnabled)
			return;
		
		
		if (mouseEvent.getButton() == MouseEvent::LEFT) {
            mMousePressed = true;
            mouseEvent.consume();
			
			mDragOffsetX = mouseEvent.getX();
			mDragOffsetY = mouseEvent.getY();
		}
	}
	
	void DraggableIcon::mouseReleased(MouseEvent& mouseEvent) {
		if (mouseEvent.getButton() == MouseEvent::LEFT
            && mMousePressed
            && mHasMouse)
        {
            mMousePressed = false;
			/*
			 distribute action only when the button is not dragged
			 */
			if(!mouseEvent.isConsumed())
				distributeActionEvent();
			
			if(!mouseEvent.isConsumed())
				setFrameSize(0);
			
            mouseEvent.consume();
			
        }
        else if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = false;
            mouseEvent.consume();
        }
	}
	
	void DraggableIcon::mouseExited(MouseEvent& mouseEvent) {
        mHasMouse = false;
		if(mFrameMouseEntered)
			setFrameSize(0);
    }
	
    void DraggableIcon::mouseEntered(MouseEvent& mouseEvent) {
        mHasMouse = true;
		if(mFrameMouseEntered)
			setFrameSize(1);
    }
	
	void DraggableIcon::mouseDragged(MouseEvent& mouseEvent) {
		if(mMousePressed) {/*
							if (mouseEvent.isConsumed() || mouseEvent.getSource() != this) {
							return;
							}*/
			
			setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
						mouseEvent.getY() - mDragOffsetY + getY());
			
			mouseEvent.consume();
			
			if(mDragFrame)
				setFrameSize(1);
		}
	}
	
	void DraggableIcon::setImage(const std::string& image) {
		const Image* ii = Image::load(image);
		Icon::setImage(ii);
	}
	
	bool DraggableIcon::isDragEnabled() const {
		return mDraggable;
	}
	
	void DraggableIcon::enableDrag(bool flag) {
		mDraggable = flag;
	}
	
	void DraggableIcon::enableFrameWhenMouseEntered(bool flag) {
		mFrameMouseEntered = flag;
	}
	
	bool DraggableIcon::isFrameEnabledWhenMouseEntered() const {
		return mFrameMouseEntered;
	}
	
	void DraggableIcon::enableDragFrame(bool flag) {
		mDragFrame = flag;
	}
	
	bool DraggableIcon::isDragFrameEnabled() const {
		return mDragFrame;
	}
	
	void DraggableIcon::draw(Graphics* graphics) {
		Icon::draw(graphics);
		
		if(getFrameSize() != 0)
			drawFrame(graphics);
	}
	
	void DraggableIcon::drawFrame(Graphics* graphics) {
		Color faceColor = getSelectionColor();
        int width = getWidth() + getFrameSize() * 2 - 1;
        int height = getHeight() + getFrameSize() * 2 - 1;
		
        unsigned int i;
		graphics->setColor(faceColor);
		
        for (i = 0; i < getFrameSize(); ++i) {
            graphics->drawLine(i,i, width - i, i);
            graphics->drawLine(i,i + 1, i, height - i - 1);
            graphics->drawLine(width - i,i + 1, width - i, height - i);
            graphics->drawLine(i,height - i, width - i - 1, height - i);
        }
	}

} // namespace gcn