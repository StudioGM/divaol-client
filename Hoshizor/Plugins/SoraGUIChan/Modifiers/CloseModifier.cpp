/*
 *  CloseModifier.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "CloseModifier.h"
#include <cassert>

namespace gcn {
	
	CloseModifier::CloseModifier(int inFrame, int mode):
	mInFrame(inFrame),
	mMode(mode) {				
		mCurrFrame = 0;
	}
	
	void CloseModifier::update(Widget* mWidget) {
		if(!mWidget || mFinished)
			return;
		
		mCurrFrame += 1;
		if(mCurrFrame >= mInFrame) {
			mWidget->setEnabled(false);	
			mWidget->setVisible(false);
			
			mFinished = true;
		}
		
		float percentage = (float)mCurrFrame / mInFrame;
		mWidgetWidth = mWidget->getWidth();
		mWidgetHeight = mWidget->getHeight();
		
		switch (mMode) {
			case MODE_LEFT_TO_RIGHT:
				mWidget->showPart(Rectangle(mWidgetWidth*percentage, 0, mWidgetWidth, mWidgetHeight));
				break;
				
			case MODE_RIGHT_TO_LEFT:
				mWidget->showPart(Rectangle(0, 0, mWidgetWidth*(1.f-percentage), mWidgetHeight));
				break;
				
			case MODE_UP_TO_DOWN:
				mWidget->showPart(Rectangle(0, mWidgetHeight*percentage, mWidgetWidth, mWidgetHeight));
				break;
				
			case MODE_DOWN_TO_UP:
				mWidget->showPart(Rectangle(0, 0, mWidgetWidth, mWidgetWidth*(1.f-percentage)));
				break;
				
			case MODE_UL_TO_LR:
				mWidget->showPart(Rectangle(mWidgetWidth*percentage, mWidgetHeight*percentage, mWidgetWidth, mWidgetHeight));
				break;
				
			case MODE_UR_TO_LL:
				mWidget->showPart(Rectangle(0, mWidgetHeight*percentage, mWidgetWidth*(1.f-percentage), mWidgetHeight));
				break;
				
			case MODE_LL_TO_UR:
				mWidget->showPart(Rectangle(mWidgetWidth*percentage, 0, mWidgetWidth, mWidgetHeight*(1.f-percentage)));
				break;
				
			case MODE_LR_TO_UL:
				mWidget->showPart(Rectangle(0, 0, mWidgetWidth*(1.f-percentage), mWidgetHeight*(1.f-percentage)));
				break;
		}
	}
								  
} // namespace gcn
								  
