/****************************************************
 *  Random.h
 *
 *  Created by tempbuffer on 9/19/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A random utility
 ****************************************************/

#ifndef _BASE_RANDOM_H_
#define _BASE_RANDOM_H_

#include "Type.h"
#include "Ptr.h"
#include <time.h>

namespace Base
{
	class Random {
	public:
		static const uint32 StandardRange = RAND_MAX+1;
		static const uint64 BigRange = StandardRange*StandardRange;

		static void SetRandomSeed(uint32 seed) {
			srand(seed);
			RandomSeed = seed;
		}
		static uint32 GetRandomSeed() {
			return RandomSeed;
		}

		// 0 - 1
		static float RandomFloat() {
			return static_cast<float>(_Rand())/(StandardRange-1);
		}

		static double RandomDouble() {
			return static_cast<double>(_SuperRand())/(BigRange-1);
		}

		static float RandomFloat(float min, float max) {
			return (max-min)*RandomFloat()+min;
		}

		static double RandomDouble(double min, double max) {
			return (max-min)*RandomDouble()+max;
		}

		static int32 RandomInt(int32 min = 0, int32 max = StandardRange) {
			if(max-min<=StandardRange)
				return _Rand()*(max-min)/(StandardRange)+min;
			else
				return static_cast<int32>(_SuperRand()*(max-min)/BigRange+min);
		}

	private:
		static inline uint64 _SuperRand() {
			return uint64(rand())*(RAND_MAX+1)+rand();
		}
		static inline uint32 _Rand() {
			return rand();
		}
	private:
		static uint32 RandomSeed;
	};
}

#endif