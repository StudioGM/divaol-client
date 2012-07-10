/*
 *  LabelSliderLinker.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "LabelSliderLinker.h"

namespace gcn {

	LabelSliderLinker::LabelSliderLinker(Slider* slider, const std::string& prefix):
	mSlider(slider),
	mPrefix(prefix) {
	}
	
	void LabelSliderLinker::update(Widget* widget) {
		Label* mLabel = dynamic_cast<Label*> (widget);
		if(mLabel) {
			char buf[64];
			sprintf(buf, "%.3f", mSlider->getValue());
			if(mPrefix.size() != 0)
				mLabel->setCaption(mPrefix + ": " + buf);
			else
				mLabel->setCaption(buf);
		}
	}

} // namespace gcn