/*
 *  gcnWidgetSwitcher.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "gcnWidgetSwitcher.h"

namespace gcn {

	WidgetSwitcher::WidgetSwitcher(): mPrevWidget(NULL), mNextWidget(NULL), mCurrFrame(-1), mFrame(0) {
		mEffector = new WidgetSwitchBaseEffector;
	}
	WidgetSwitcher::~WidgetSwitcher() {
		if(mEffector) {
			delete mEffector;
			mEffector = NULL;
		}
	}
	
	void WidgetSwitcher::start(Widget* prev, Widget* next, unsigned int frame) {
		if(prev == NULL || next == NULL) {
			mCurrFrame = -1;
			mFrame = 0;
			return;
		}
		
		mPrevWidget = prev;
		mNextWidget = next;
		
		mFrame = frame;
		mCurrFrame = 0;
		
		mPrevWidget->setEnabled(false);
		mNextWidget->setEnabled(false);
	}
	
	void WidgetSwitcher::draw(Graphics* graphics) {
		if(mPrevWidget)
			mPrevWidget->draw(graphics);
		if(mNextWidget)
			mNextWidget->draw(graphics);
	}
	
	void WidgetSwitcher::logic() {
		Widget::logic();
		
		if(mCurrFrame != -1) {
			if(mPrevWidget)
				mPrevWidget->logic();
			if(mNextWidget)
				mNextWidget->logic();
			
			mEffector->onSwitchFrame(mPrevWidget, mNextWidget, mCurrFrame);
			
			++mCurrFrame;
			if(mCurrFrame >= mFrame) {
				mNextWidget->setEnabled(true);
				mPrevWidget->setEnabled(true);
				
				mPrevWidget = mNextWidget;
				mNextWidget = NULL;
				
				mCurrFrame = -1;
				mFrame = 0;
			}
		}
	}
	
	void WidgetSwitcher::setSwitchEffector(WidgetSwitchEffector* effector) {
		if(mEffector) {
			delete mEffector;
			mEffector = NULL;
		}
		
		mEffector = effector;
	}
	
	bool WidgetSwitcher::isSwitching() {
		return mCurrFrame != -1;
	}
	
} // namespace gcn