/*
 *  DivaUIPainter.h
 *
 *  Created by Hyf042 on 2/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_UI_PAINTER
#define DIVA_UI_PAINTER

#include "DivaCommon.h"
#include "Core/DivaConfig.h"
#include "DivaEventManager.h"

namespace divacore
{
	class UIPainter : public EventHandler
	{
		Config config;
	public:
		virtual void hide(const std::string &tag = "") {}
		virtual void display(const std::string &tag = "") {}

		virtual void update(float dt) {}
		virtual void render() {}
		virtual void addWidget(std::string name) {}

		virtual void onMouseClicked(SoraMouseEvent& event) {}
		virtual void onMouseReleased(SoraMouseEvent& event) {}
		virtual void onMouseMoved(SoraMouseEvent& event) {}
	};

	typedef UIPainter* UIPainterPtr;
}

#endif