/*
 *  DivaRenderSystem.h
 *
 *  Created by Hyf042 on 1/27/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_RENDER_SYSTEM
#define DIVA_RENDER_SYSTEM

#include "DivaCommon.h"
#include "DivaMapInfo.h"
#include "DivaConfig.h"
#include "DivaEventManager.h"
#include "SoraSprite.h"
#include "DivaConfig.h"

namespace divacore
{
	/*
	RenderSystem
	*/
	class RenderSystem : public EventHandler
	{
	public:
		virtual void prepare(Config &config) = 0;
		virtual void render() = 0;
		virtual void draw() = 0;
		virtual sora::SoraSprite* renderToCanvas(float width = 0, float height = 0) = 0;
		virtual void render(sora::SoraSprite *sprite, const std::string &tag = "") = 0;
		virtual void render(float x, float y, sora::SoraSprite *sprite, const std::string &tag = "") = 0;
		virtual bool InsideDrawRange(const Point &p) = 0;
		virtual void fadeOut(uint32 color) {}
		virtual void fadeIn(uint32 color) {}
		virtual bool isFade() {return false;}
		virtual int getGameWidth() = 0;
		virtual int getGameHeight() = 0;
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
	};

	typedef RenderSystem* RenderSystemPtr;
}

#endif