/*
 *  gcnConnectable.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnConnectable.h"

namespace gcn {

	ConnectableWidget::ConnectableWidget(): mPair(NULL) {
	}

	ConnectableWidget::~ConnectableWidget() {
	}

	void ConnectableWidget::connect(Widget* pair) {
		mPair = pair;
	}

	Widget* ConnectableWidget::getPair() const {
		return mPair;
	}

	void ConnectableWidget::draw(Graphics* graphics) {
		graphics->setColor(mBaseColor);
		if(mPair != NULL) {
			graphics->drawLine(getX(), getY(), mPair->getX(), mPair->getY());
		}
	}

} // namespace gcn
