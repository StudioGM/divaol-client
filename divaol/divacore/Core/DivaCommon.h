/*
 *  DivaCommon.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_COMMON
#define DIVA_COMMON

#include <set>
#include "DivaException.h"
//#include "DivaDefault.h"
#include "DivaConstant.h"
#include "DivaArgument.h"
#include "DivaStringUtil.h"
#include "DivaKey.h"
#include "DivaLogger.h"
#include "Base/Common.h"

#define SAFE_DELETE(instance)\
	if(instance)\
	{\
		delete instance;\
		instance = 0;\
	}
#define SAFE_DELETE_SPRITE(instance)\
	if(instance)\
	{\
	instance->setTexture(0);\
	delete instance;\
	instance = 0;\
	}
#define SAFE_DELETE_SPRITE_TEXTURE(instance)\
	if(instance)\
	{\
	sora::SoraCore::Ptr->releaseTexture(instance->getTexture());\
	delete instance;\
	instance = 0;\
	}

#define DEAL_PER_PERIOD_BEGIN(dt,time)\
	{\
	static float __secondPeriod__ = time;\
	__secondPeriod__ -= dt;\
	if(__secondPeriod__<=0)\
		{\
		__secondPeriod__ += time;

#define DEAL_PER_PERIOD_END()\
		}\
	}

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace divacore
{
	typedef unsigned char byte;

	template<typename T>
	inline T min(const T &a, const T &b) {return a<b?a:b;}
	template<typename T>
	inline T max(const T &a, const T &b) {return a>b?a:b;}

	const double PI = 3.1415926;
	const double EPS = 1e-10;

	class Point
	{
	public:
		float x,y;
		Point():x(0),y(0) {}
		Point(const float &_x, const float &_y):x(_x),y(_y) {}
		float mod() {return sqrt(x*x+y*y);}
		Point unit() {float tmp = mod(); return Point(x/tmp,y/tmp);}
		Point normal() {return Point(y,-x);}
		friend Point operator+(const Point &a, const Point &b) {return Point(a.x+b.x,a.y+b.y);}
		friend Point operator-(const Point &a, const Point &b) {return Point(a.x-b.x,a.y-b.y);}
		friend Point operator*(const Point &a, const float &b) {return Point(a.x*b,a.y*b);}
		friend float operator*(const Point &a, const Point &b) {return a.x*b.x+a.y*b.y;}
		friend bool operator==(const Point &a, const Point &b) {return abs(a.x-b.x)<EPS&&abs(a.y-b.y)<EPS;}
		friend float cross(const Point &a, const Point &b) {return a.x*b.y-b.x*a.y;}
	};

	class Rect
	{
	public:
		float x,y,w,h;
		Rect():x(0),y(0),w(0),h(0) {}
		Rect(float x, float y, float w, float h):x(x),y(y),w(w),h(h) {}
		Rect(const Point &a, const Point &b):x(a.x),y(a.y),w(b.x),h(b.y) {}
		Point position() const {return Point(x,y);}
		Point size() const {return Point(w,h);}
		Point center() const {return Point(x+w/2,y+h/2);}
		static Rect get(const Rect &a, const Rect &b, float c)
		{
			return Rect(a.position()+(b.position()-a.position())*c,a.size()+(b.size()-a.size())*c);
		}
	};
}

#endif