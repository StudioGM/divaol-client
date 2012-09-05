/*
 *  gcnImageButton2.cpp
 *  Sora
 *
 *  Created by GriffinBu on 12/15/10.
 *  Copyright 2010 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "gcnImageButton2.h"

namespace gcn {
	
	ImageButton2::ImageButton2() {
		mImage = 0;
		
		mInternalImage = false;
		mFrameMouseEntered = false;
		
		mClickSound = NULL;
		mEnterSound = NULL;
		
		setFrameSize(0);
		addActionListener(this);
	}
	
	ImageButton2::~ImageButton2() {
		if(mInternalImage) {
			if(mImage) {
				delete mImage;
				mImage = 0;
			}
		}
		if(mClickSound) {
			delete mClickSound;
			mClickSound = NULL;
		}
		if(mEnterSound) {
			delete mEnterSound;
			mEnterSound = NULL;
		}
	}
	
	ImageButton2::ImageButton2(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed) {
		mImage = Image::load(sprNormal);
		
		normalTex = normal;
		pushedTex = pushed;
		unavailTex = normalTex;
		focusTex = normalTex;
		mInternalImage = true;
		//unavailTex = unavail;
		
		adjustSize();
		
		setFrameSize(0);
	}
	
	void ImageButton2::setImage(const std::string& sprNormal, const Rectangle& normal, const Rectangle& pushed) {
		if(mInternalImage && mImage)
			delete mImage;
		mImage = Image::load(sprNormal);
		
		normalTex = normal;
		pushedTex = pushed;
		unavailTex = normalTex;
		focusTex = normalTex;
		mInternalImage = true;
		//unavailTex = unavail;
		
		adjustSize();
	}
	
	void ImageButton2::setImage(Image* image, const Rectangle& normal, const Rectangle& pushed) {
		if(mInternalImage && mImage)
			delete mImage;
		mImage = image;
		
		normalTex = normal;
		pushedTex = pushed;
		unavailTex = normalTex;
		focusTex = normalTex;
		mInternalImage = false;
		
		adjustSize();
	}
	
	void ImageButton2::setDrawRect(const Rectangle& normal, const Rectangle& pushed) {
		normalTex = normal;
		pushedTex = pushed;
	}
	
	Rectangle ImageButton2::getNormalRect() const {
		return normalTex;
	}
	
	Rectangle ImageButton2::getPushedRect() const {
		return pushedTex;
	}
	
	Image* ImageButton2::getImage() const {
		return mImage;
	}
	
	void ImageButton2::adjustSize() {
		setWidth(normalTex.width);
		setHeight(normalTex.height);
	}
	
	void ImageButton2::drawFrame(Graphics* graphics) {
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

	void ImageButton2::draw(Graphics* graphics) {
		if(!isVisible())
			return;
		
        const int textX = (getWidth() - (mImage ? normalTex.width: 0) ) / 2;
        const int textY = (getHeight() - (mImage ? normalTex.height : 0) ) / 2;
		
		if(getFrameSize() != 0)
			drawFrame(graphics);
		
		if(!isEnabled()) {
			if(mImage) {
                graphics->drawImage(mImage, unavailTex.x, unavailTex.y, textX, textY, unavailTex.width, unavailTex.height);
			}
		}
        else if(isPressed()) {
            if(mImage) {
                graphics->drawImage(mImage, pushedTex.x, pushedTex.y, textX, textY, pushedTex.width, pushedTex.height);
			}
        }
        else if(isFocused()) {
			if(mImage) {
				graphics->drawImage(mImage, focusTex.x, focusTex.y, textX, textY, focusTex.width, focusTex.height);
			}
		}
		else {
			if(mImage) {
				graphics->drawImage(mImage, normalTex.x, normalTex.y, textX, textY, normalTex.width, normalTex.height);
			}
        }
	}
	
	void ImageButton2::mouseEntered(MouseEvent& mouseEvent) {
		if(mouseEvent.getSource() == this) {
			if(mEnterSound)
				mEnterSound->play();
		}
	}
	
	void ImageButton2::action(const ActionEvent& actionEvent) {
		if(actionEvent.getSource() == this) {
			if(mClickSound)
				mClickSound->play();
		}
	}
	
	void ImageButton2::enableFrameWhenMouseEntered(bool flag) {
		mFrameMouseEntered = flag;
	}
	
	bool ImageButton2::isFrameEnabledWhenMouseEntered() const {
		return mFrameMouseEntered;
	}
	
	void ImageButton2::setFocusRect(const Rectangle& rect) {
		focusTex = rect;
	}
	
	Rectangle ImageButton2::getFocusRect() const {
		return focusTex;
	}
	
	void ImageButton2::setUnavailableRect(const Rectangle& rect) {
		unavailTex = rect;
	}
	
	Rectangle ImageButton2::getUnavailableRect() const {
		return unavailTex;
	}
	
	void ImageButton2::setClickSound(const std::string& soundName) {
		mClickSound = Sound::load(soundName);
	}
	
	void ImageButton2::setEnterSound(const std::string& soundName) {
		mEnterSound = Sound::load(soundName);
	}
	
} // namespace gcn