/*
 *  DivaPath.h
 *
 *  Created by Hyf042 on 1/27/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PATH
#define DIVA_PATH

#include "Core/DivaCommon.h"
#include "SoraSprite.h"
#include "Core/DivaCore.h"
#include "Utility/DivaColor.h"

namespace divacore
{
	using namespace sora;

	namespace path
	{
		class Line
		{
		public:
			static Point RandomDir(){
				float angle = (float)rand()/RAND_MAX*6.2831853f;
				return Point(cos(angle),sin(angle));
			}
			static void renderLine(const Point &p0, const Point &p1, sora::SoraSprite *meta)
			{
				// increase inc by effect level
				float factor = 4*(1-EFFECT_SYSTEM_PTR->getEffectLevel()/2.0)+1;
				float LINE_INC = max(1.f,0.1f*factor*min(meta->getSpriteWidth()*meta->getHScale(),meta->getSpriteHeight()*meta->getVScale()));
				
				Point p = p0;
				float D = (p1-p0).mod();
				Point p_inc = (p1-p0).unit()*LINE_INC;
				for(float i = 0; i <= D; i+=LINE_INC)
				{
					Core::Ptr->render(p.x,p.y,meta,"line_meta+");
					p  = p+p_inc;
				}
			}
			static void renderCometLine(const Point &p0, const Point &p1, NotePtr note)
			{
				// increase inc by effect level
				int level = EFFECT_SYSTEM_PTR->getEffectLevel();
				if(level == 0)
					return;
				float LINE_INC = 5.f + (2-EFFECT_SYSTEM_PTR->getEffectLevel()) * 20;

				Point p = p0;
				float D = (p1-p0).mod();
				Point p_inc = (p1-p0).unit()*LINE_INC;
				for(float i = 0; i <= D; i+=LINE_INC)
				{
					Point dP = RandomDir();
					if(RENDER_SYSTEM_PTR->InsideDrawRange(p))
						EFFECT_SYSTEM_PTR->addParticle("comet",
						ParticleData(p.x,p.y,dP.x*32-(p1-p0).x,dP.y*32-(p1-p0).y,0),note);
					p  = p+p_inc;
				}
			}
		};
		
		class Bezier
		{
		public:
			static Point getBezierPoint(const Point &p0, const Point &p1, float t)
			{
				Point m1 = p0+(p1-p0)*0.25+(p1-p0).normal()*0.15;
				Point m2 = p0+(p1-p0)*0.75-(p1-p0).normal()*0.15;
				float tt = 1-t;
				return p0*tt*tt*tt+m1*3*t*tt*tt+m2*3*t*t*tt+p1*t*t*t;
			}
			static void renderBezier(const Point &p0, const Point &p1, float l, float r, sora::SoraSprite *meta)
			{
				static const int TOTAL_SEGMENT_NUMBER = 64;
				static const float INC = 1.0f/TOTAL_SEGMENT_NUMBER;
				Point p = getBezierPoint(p0,p1,l), q;
				for( float t=l; t < r; t+=INC)
				{
					q = getBezierPoint(p0,p1,t);
					Line::renderLine(p,q,meta);
					p = q;
				}
				q = getBezierPoint(p0,p1,r);
				Line::renderLine(p,q,meta);
			}
		};
	}
}

#endif