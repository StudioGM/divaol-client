/*
 *  gcnWidgetSwitcher.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_WIDGET_SWITCHER_H_
#define GCN_WIDGET_SWITCHER_H_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	class WidgetSwitchEffector {
	public:
		virtual void onSwitchFrame(Widget* prev, Widget* next, unsigned int onFrame) = 0;
	};
	/* 
	 a class that switch between two widgets
	 during the switch process, all widgets would be disabled for input
	 special effects may apply to the switch process
	 */
	
	class WidgetSwitcher: public Widget {
	public:
		WidgetSwitcher();
		~WidgetSwitcher();
		
		void start(Widget* prev, Widget* next, unsigned int inFrame);
		
		
		void setSwitchEffector(WidgetSwitchEffector* effector);
		
		bool isSwitching();
		
		/*
		 inherited from Widget
		 */
		virtual void draw(Graphics* graphics);
		virtual void logic();
		
		
	protected:
		Widget* mPrevWidget;
		Widget* mNextWidget;
		
		int mCurrFrame, mFrame;
		
		WidgetSwitchEffector* mEffector;
	};
	
	class WidgetSwitchBaseEffector: public WidgetSwitchEffector {
	public:
		// do nothing
		void onSwitchFrame(Widget* prev, Widget* next, unsigned int onFrame) {
			
		}
	};
	
} // namespace gcn


#endif // GCN_WIDGET_SWITCHER_H_