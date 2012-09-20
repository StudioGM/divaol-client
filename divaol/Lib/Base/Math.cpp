/****************************************************
 *  Math.cpp
 *
 *  Created by tempbuffer on 9/19/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Several Classes about Math and other utilities
 ****************************************************/

#include "Math.h"

namespace Base
{
	namespace Math {
		float Util::FastInvSqrt(float x)
		{
			union
			{
				int intPart;
				float realPart;
			} convertor;

			convertor.realPart = x;
			convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
			return convertor.realPart*(1.5f - 0.4999f*x*convertor.realPart*convertor.realPart);
		}
	}
}
