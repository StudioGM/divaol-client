/*
 *  gcnConnectable.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_CONNECTABLE_H_
#define GCN_CONNECTABLE_H_

#include "SoraGUIChan/guichan.hpp"

namespace gcn {
	
	/*
	 a basic connectable widget
	 connection would be drawed as a line with certain color
	 */
	class ConnectableWidget: public Widget {
	public:
		ConnectableWidget();
		~ConnectableWidget();
	
		void connect(Widget* pair);
		Widget* getPair() const;

		void draw(Graphics* graphics);
		
	protected:					 
		Widget* mPair;
	};
	
} // namespace gcn

#endif // GCN_CONNECTABLE_H_