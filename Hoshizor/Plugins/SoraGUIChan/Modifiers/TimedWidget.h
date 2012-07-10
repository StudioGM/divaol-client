/*
 *  TimedWidget.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GNC_TIMED_WIDGET_H_
#define SORA_GNC_TIMED_WIDGET_H_

#include "../guichan.hpp"

namespace gcn {
	
	class TimedWidget {
	public:
		TimedWidget();
		TimedWidget(Widget* widget, float time);
	};
	
} // namespace gcn

#endif // SORA_TIMED_WIDGET_H_