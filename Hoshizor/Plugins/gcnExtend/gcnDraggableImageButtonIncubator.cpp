/*
 *  gcnDraggableImage_NewItem.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnDraggableImageButtonIncubator.h"
namespace gcn {
	
	DraggableImageButtonIncubator::DraggableImageButtonIncubator(): newImage(NULL), bInternalImage(false), userData(NULL) {
		mMousePressed = false;
		mHasMouse = false;
		mDragged = false;
		addParent = NULL;
		setFrameSize(0);
	}
	
	DraggableImageButtonIncubator::~DraggableImageButtonIncubator() {
		if(bInternalImage && newImage)
			delete newImage;
	}
	
	void DraggableImageButtonIncubator::setNewItemImage(const std::string& image, const Rectangle& normal, const Rectangle& pushed) {
		if(bInternalImage && newImage)
			delete newImage;
		newImage = Image::load(image);
		
		newNormalTex = normal;
		newPushedTex = pushed;
		
		bInternalImage = true;
	}
	
	void DraggableImageButtonIncubator::setNewItemImage(Image* image, const Rectangle& normal, const Rectangle& pushed) {
		if(bInternalImage && newImage)
			delete newImage;
		newImage = image;
		
		newNormalTex = normal;
		newPushedTex = pushed;
		
		bInternalImage = false;
	}

	void DraggableImageButtonIncubator::mousePressed(MouseEvent& mouseEvent) {
		if(mouseEvent.isConsumed() || !mEnabled)
			return;
		
		if (mouseEvent.getButton() == MouseEvent::LEFT) {
			Widget* newDraggableWidget;
			if(!exFunc) {
				newDraggableWidget = new DraggableImageButton;
				if(newImage == NULL)
					((DraggableImageButton*)newDraggableWidget)->setImage(mImage, getNormalRect(), getPushedRect());
				else
					((DraggableImageButton*)newDraggableWidget)->setImage(newImage, newNormalTex, newPushedTex);
			}
			else
				newDraggableWidget = exFunc(userData);
			if(!newDraggableWidget) return;
			
			//newDraggableWidget->adjustSize();
			newDraggableWidget->setSelectionColor(getSelectionColor());
			
			Widget* pParent;
			if(addParent == NULL)
				pParent = getParent();
			else
				pParent = addParent;
			Container* pCont = dynamic_cast<Container*> (pParent);
			bool bAdded = false;
			if(pCont) {
				pCont->add(newDraggableWidget);
				pCont->moveToTop(newDraggableWidget);
				bAdded = true;
			}
			else {
				Window* pWnd = dynamic_cast<Window*>(pParent);
				if(pWnd) {
					pWnd->add(newDraggableWidget);
					pCont->moveToTop(newDraggableWidget);
					bAdded = true;
				}
			}
				
			if(!bAdded) {
				delete newDraggableWidget;
				return;
			}
			
			currentWidget = newDraggableWidget;
			currentWidget->setFrameSize(1);
			MouseListener* ml = dynamic_cast<MouseListener*> (currentWidget);
			if(ml)
				ml->mousePressed(mouseEvent);
			
			mMousePressed = true;
			mWidgetDragged = false;
            mouseEvent.consume();
			
			mDragOffsetX = mouseEvent.getX();
			mDragOffsetY = mouseEvent.getY();
        }
	}
	
	void DraggableImageButtonIncubator::mouseReleased(MouseEvent& mouseEvent) {
		if (mouseEvent.getButton() == MouseEvent::LEFT
            && mMousePressed
            && mHasMouse)
        {
            mMousePressed = false;
            mouseEvent.consume();
			
			if(currentWidget) {
				MouseListener* ml = dynamic_cast<MouseListener*> (currentWidget);
				if(ml)
					ml->mouseReleased(mouseEvent);
				currentWidget->setFrameSize(0);
				
				// we cannot let another widget exist on the same place
				if(!mWidgetDragged) {
					Widget* pParent;
					if(addParent == NULL)
						pParent = getParent();
					else
						pParent = addParent;
					Container* pCont = dynamic_cast<Container*> (pParent);
					if(pCont) {
						pCont->remove(currentWidget);
					}
					else {
						Window* pWnd = dynamic_cast<Window*>(pParent);
						if(pWnd) {
							pWnd->remove(currentWidget);
						}
					}
					delete currentWidget;
				}
				currentWidget = NULL;
			}
        }
        else if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = false;
            mouseEvent.consume();
			
			if(currentWidget) {
				MouseListener* ml = dynamic_cast<MouseListener*> (currentWidget);
				if(ml)
					ml->mouseReleased(mouseEvent);
				currentWidget->setFrameSize(0);
				currentWidget = NULL;
			}
        }
	}
	
	void DraggableImageButtonIncubator::mouseDragged(MouseEvent& mouseEvent) {
		if(mMousePressed) {/*
							if (mouseEvent.isConsumed() || mouseEvent.getSource() != this) {
							return;
							}*/
			
			if (currentWidget) {
				currentWidget->setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
										  mouseEvent.getY() - mDragOffsetY + getY());
				mWidgetDragged = true;
			}
			
			mouseEvent.consume();
		}
	}
	
	void DraggableImageButtonIncubator::registerIncubatorFunc(IncubateFunc func) {
		exFunc = func;
	}

	void DraggableImageButtonIncubator::setUserData(void* data) {
		userData = data;
	}
	
	void* DraggableImageButtonIncubator::getUserData() const {
		return userData;
	}
	
	void DraggableImageButtonIncubator::setAddParent(Widget* parent) {
		addParent = parent;
	}
} // namespace gcn