/*
 *  CloseEffector.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_GCN_CLOSE_EFFECTOR_H_
#define SORA_GCN_CLOSE_EFFECTOR_H_

#include "../guichan.hpp"

namespace gcn {
	
	class CloseModifier: public Modifier {
	public:
		enum {
			// left to right
			MODE_LEFT_TO_RIGHT,
			// right to left
			MODE_RIGHT_TO_LEFT,
			// up to down
			MODE_UP_TO_DOWN,
			// down to up
			MODE_DOWN_TO_UP,
			
			// upperleft to lower right
			MODE_UL_TO_LR,
			// lower right to upper left
			MODE_LR_TO_UL,
			// upper right to lower left
			MODE_UR_TO_LL,
			// lower left to upper right
			MODE_LL_TO_UR,
		};
		
		CloseModifier(int inFrame, int mode);

		
		virtual void update(Widget* widget);
		
	private:
		int mMode;
		float mTime;
				
		int mWidgetWidth, mWidgetHeight;
		int mInFrame, mCurrFrame;
	};
	
} // namespace gcn

#endif // SORA_GCN_CLOSE_EFFECTOR_H_