/*
 *  gcnPanel.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_PANEL_H_
#define GCN_PANEL_H_

#include "SoraGUIChan/guichan.hpp"

#include "gcnWidgetSwitcher.h"

namespace gcn {
	
	/*
	 unlike a container, a panel would only display one widget at a time
	 you can switcher between the wiget queue to display different widgets
	 relies on WidgetSwitcher to switch between widgets
	 */
	class Panel: public Container {
	public:
		
		virtual void draw(Graphics* graphics);
		virtual void logic();
		
		void toNext(unsigned int switchFrame=1);
		void toPrev(unsigned int switchFrame=1);
		void toWidget(Widget* widget, unsigned int switchFrame=1);
		
		void setTop(Widget* widget);
		
		void setSwitchEffector(WidgetSwitchEffector* effector);
		/*
		 inherited from Container
		 */
		virtual void add(Widget* widget);
		
	private:
		Widget* mTopWidget;
		
		WidgetSwitcher mSwitcher;
	};
	
} // namespace gcn


#endif // GCN_PANEL_H_