/*
 *  gcnMenuContainer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnMenuContainer.h"

namespace gcn {

	MenuContainer::MenuContainer(): bottomPosX(0), bottomPosY(0) {
		titleImage = NULL;
	}
	
	MenuContainer::MenuContainer(const std::string& title, bool bIsImage) {
		if(bIsImage)
			titleImage = Image::load(title);
		else
			titleString = title;
	}
	
	MenuContainer::~MenuContainer() {
		if(titleImage)
			delete titleImage;
		if(mMoveSound)
			delete mMoveSound;
	}
	
	void MenuContainer::setTitleImage(const std::string& title, bool bIsImage) {
		if(titleImage) {
			delete titleImage;
			titleImage = NULL;
		}
		
		if(bIsImage) {
			titleImage = Image::load(title);
			titleString.clear();
			if(titleImage)
				bottomPosY += titleImage->getHeight();
		}
		else {
			titleString = title;
			bottomPosY += getFont()->getHeight();
		}
	}
	
	void MenuContainer::keyPressed(KeyEvent& keyEvent) {
		if(keyEvent.getKey() == Key::DOWN) {
			focusNext();
			
			if(mMoveSound)
				mMoveSound->play();
		} else if(keyEvent.getKey() == Key::UP) {
			focusPrevious();
			
			if(mMoveSound)
				mMoveSound->play();
		}
	}
	
	void MenuContainer::keyReleased(KeyEvent& keyEvent) {
	}
	
	void MenuContainer::add(Widget* widget) {
		Container::add(widget);
		
		switch (alignment) {
			case ALIGN_LEFT: bottomPosX = 0; break;
			case ALIGN_MIDDLE: bottomPosX = (getWidth()-widget->getWidth())>>1; break;
			case ALIGN_RIGHT: bottomPosX = getWidth()-widget->getWidth(); break;
		}
		widget->setPosition(bottomPosX, bottomPosY);
		bottomPosY += widget->getHeight();
	}
	
	void MenuContainer::setAlignment(MenuItemAlignment align) {
		alignment = align;
	}
	
	MenuContainer::MenuItemAlignment MenuContainer::getAlignment() const {
		return alignment;
	}
	
	void MenuContainer::setMoveSound(const std::string& soundName) {
		mMoveSound = Sound::load(soundName);
	}

} // namespace gcn