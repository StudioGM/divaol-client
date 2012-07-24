/*
 *  gcnDraggableImage_NewItem.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_GRAGGABLE_IMAGE_NEW_ITEM_H_
#define GCN_GRAGGABLE_IMAGE_NEW_ITEM_H_

#include "gcnDraggableImageButton.h"

namespace gcn {
	
	/*
	 implement this to create custom widget 
	 */
	typedef Widget* (*IncubateFunc)(void *userData);
	
	/*
	 when click and drag, would institiate a new widget for drag & create
	 */
	class DraggableImageButtonIncubator: public DraggableImageButton {
	public:
		DraggableImageButtonIncubator();
		virtual ~DraggableImageButtonIncubator();
		
		virtual void mousePressed(MouseEvent& mouseEvent);
		virtual void mouseReleased(MouseEvent& mouseEvent);
        virtual void mouseDragged(MouseEvent& mouseEvent);
		
		/*
		 set image of new item to institiate, if not, then would use the image of DraggableImageNewItem self's image
		 */
		void setNewItemImage(const std::string& image, const Rectangle& normal, const Rectangle& pushed);
		void setNewItemImage(Image* image, const Rectangle& normal, const Rectangle& pushed);
		
		void registerIncubatorFunc(IncubateFunc func);
		
		void setUserData(void* data);
		void* getUserData() const;
		
		/*
		 set the incubated widget's parent
		 if null then would use this->getParent as a parent container
		 */
		void setAddParent(Widget* parent);
		
	protected:
		Image* newImage;
		Rectangle newNormalTex;
		Rectangle newPushedTex;
		
		Widget* currentWidget;
		Widget* addParent;
		
		bool bInternalImage;
		bool mWidgetDragged;
		IncubateFunc exFunc;
		
		void *userData;
	};
	
} // namespace gcn


#endif