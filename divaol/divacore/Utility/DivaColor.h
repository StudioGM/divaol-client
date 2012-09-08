/*
 *  DivaColor.h
 *
 *  Created by Hyf042 on 9/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_COLOR
#define DIVA_COLOR

#include "Core/DivaCommon.h"
#include "SoraSprite.h"
#include "Core/DivaCore.h"

namespace divacore
{
	using namespace sora;

	class Color
	{
	public:
		static uint32 getHSVColor(float h, float s, float v, float a = 1.0)
		{
			return sora::SoraColorHSV(h,s,v,a).getHWColor();
		}
		static uint32 getRainbowColor(float ratio)
  		{
			int n = ratio*320;
			uint32 color;
			int a = n*6/320;
			int b = (n*1530/320)%255;
			//nmax=320；n从1增加到320；所以a最小值0，最大值6；b最小值0，最大值255
			if (a == 0 || a == 6) {
				color = (255 << 16 | b << 8 | 0);
				//FF0000--FFFF00段
			} else if (a == 1) {
				color = ((255-b) << 16 | 255 << 8 | 0);
				//FFFF00--00FF00段
			} else if (a == 2) {
				color = (0 | 255 << 8 | b);
			} else if (a == 3) {
				color = (0 << 16 | (255-b) << 8 | 255);
			} else if (a == 4) {
				color = (b << 16 | 0 | 255);
				//0000FF--FF00FF段
			} else if (a == 5) {
				color = (255 << 16 | 0 | (255-b));
				//FF00FF--FF0000段
			}
			color |= 0xFF000000;
			return color;
		}
	};
}

#endif