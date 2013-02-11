/*
 *  gcnBackgroundContainer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/21/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnBackgroundContainer.h"

namespace gcn {
	
	BackgroundContainer::BackgroundContainer(): pBackgroundImage(NULL) {
		setOpaque(false);
		setFrameSize(0);
	}
	
	BackgroundContainer::BackgroundContainer(const std::string& bgImage) {
		pBackgroundImage = Image::load(bgImage);
		
		setOpaque(false);
		setFrameSize(0);
	}
	
	void BackgroundContainer::setImage(const std::string& bgImage) {
		pBackgroundImage = Image::load(bgImage);
		adjustSize();
	}
	
	void BackgroundContainer::draw(Graphics* graphics) {
		if(pBackgroundImage) {
			Rectangle dm = getDimension();
			graphics->drawImage(pBackgroundImage, 0.f, 0.f, dm.x, dm.y, dm.width, dm.height);
		}
		
		drawChildren(graphics);
	}
	
	void BackgroundContainer::adjustSize() {
		if(pBackgroundImage) {
			Rectangle dm = getDimension();
			setDimension(Rectangle(dm.x, dm.y, pBackgroundImage->getWidth(), pBackgroundImage->getHeight()));
		}
	}
	
	Image* BackgroundContainer::getImage() const {
		return pBackgroundImage;
	}

}