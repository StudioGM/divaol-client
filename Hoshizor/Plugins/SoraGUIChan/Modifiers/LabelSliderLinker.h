/*
 *  LabelSliderLinker.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_LABEL_SLIDER_LINKER_H_
#define SORA_GCN_LABEL_SLIDER_LINKER_H_

#include "../guichan.hpp"
#include "../guichan/modifier.hpp"

namespace gcn {
	
	/*
	 a linker that links a label and a slider
	 the label would show the number of the slider
	 */
	class LabelSliderLinker: public Modifier {
	public:
		LabelSliderLinker(Slider* slider, const std::string& prefix);
		
		virtual void update(Widget* widget);
        
        std::string getName() const {
            return "LabelSliderLinker";
        }
		
	private:
		Slider* mSlider;
		
		std::string mPrefix;
	};
}

#endif // SORA_GCN_LABEL_SLIDER_LINKER_H_