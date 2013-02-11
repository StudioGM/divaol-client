/*
 *  gcnPanel.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnPanel.h"

namespace gcn {

	void Panel::draw(Graphics* graphics) {
		if(!mSwitcher.isSwitching()) {
			if(mTopWidget)
				(mTopWidget)->draw(graphics);
		} else {
			mSwitcher.draw(graphics);
		}
	}
	
	void Panel::logic() {
		if(!mSwitcher.isSwitching()) {
			if(mTopWidget)
				(mTopWidget)->logic();
		} else {
			mSwitcher.logic();
		}
	}
	
	void Panel::add(Widget* widget) {
		mWidgets.push_back(widget);
		mTopWidget = mWidgets.front();
	}
	
	void Panel::setTop(Widget* widget) {
		WidgetList::iterator itWidget = std::find(mWidgets.begin(), mWidgets.end(), widget);
		if(itWidget != mWidgets.end())
			mTopWidget = widget;
	}
	
	void Panel::toNext(unsigned int switchFrame) {
		if(mTopWidget) {
			WidgetList::iterator itWidget = std::find(mWidgets.begin(), mWidgets.end(), mTopWidget);
			++itWidget;
			if(itWidget != mWidgets.end()) 
				mSwitcher.start(mTopWidget, *itWidget, switchFrame);
		}
	}
	
	void Panel::toPrev(unsigned int switchFrame) {
		if(mTopWidget) {
			WidgetList::iterator itWidget = std::find(mWidgets.begin(), mWidgets.end(), mTopWidget);
			--itWidget;
			if(itWidget != mWidgets.end()) 
				mSwitcher.start(mTopWidget, *itWidget, switchFrame);
		}
	}

	void Panel::toWidget(Widget* widget, unsigned int switchFrame) {
		WidgetList::iterator nextWidget = std::find(mWidgets.begin(), mWidgets.end(), widget);
		if(nextWidget != mWidgets.end()) {
			mSwitcher.start(mTopWidget, *nextWidget, switchFrame);
		}
	}
	
	void Panel::setSwitchEffector(WidgetSwitchEffector* effector) {
		mSwitcher.setSwitchEffector(effector);
	}
} // namespace gcn