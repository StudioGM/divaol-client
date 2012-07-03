/*
 *  hgeRandom.h
 *  Sora
 *
 *  Created by GriffinBu on 1/23/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef HGE_RANDOM_H_
#define HGE_RANDOM_H_

namespace sora {
	
	static int32 g_seed2=0;
	
	inline void Random_Seed(int32 seed)
	{
		if(!seed) g_seed2=0x11228866;
		else g_seed2=seed;
	}
	
	inline int32 Random_int32(int32 min, int32 max)
	{
		g_seed2=214013*g_seed2+2531011;
		return min+(g_seed2 ^ g_seed2>>15)%(max-min+1);
	}
	
	inline float32 Random_float32(float32 min, float32 max)
	{
		g_seed2=214013*g_seed2+2531011;
		return min+(g_seed2>>16)*(1.0f/65535.0f)*(max-min);
	}	
} // namespace sora

#endif