/*
 *  DivaParticleSystem.h
 *
 *  Created by Hyf042 on 3/22/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PARTICLE_SYSTEM
#define DIVA_PARTICLE_SYSTEM

#include "DivaCommon.h"
#include "DivaMapInfo.h"

namespace divacore
{
	class ParticleData
	{
	public:
		float x,y;
		float dx,dy;
		float size;
		ParticleData(float x, float y, float dx, float dy, float size):x(x),y(y),dx(dx),dy(dy),size(size) {}
	};
}
#endif