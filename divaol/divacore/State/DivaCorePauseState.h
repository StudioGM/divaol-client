/*
 *  DivaCorePauseState.h
 *
 *  Created by Hyf042 on 1/21/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_CORE_PAUSE_STATE
#define DIVA_CORE_PAUSE_STATE

#include "Core/DivaCoreState.h"
#include "Utility/DivaSettings.h"
namespace divacore
{
	using namespace sora;

	/*
	CoreLoadState
	A state for loading resource and show loading interface
	*/
	class CorePauseState : public CoreState
	{
		sora::SoraText mText;

	public:
		void onInitiate()
		{
#ifdef _DEBUG
			sora::SoraFont* mFont = sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 20);

			mText.setFont(mFont);
#endif
			mText.setText(L"|#FF0000|Paused\nPress P to resume game");
		}

		void onEnter()
		{
			core->pause();
		}

		void onLeave()
		{
			core->resume();
		}

		void onRender()
		{
			mText.render();
		}

		void onUpdate(float dt)
		{
		}

		void onKeyReleased(SoraKeyEvent& event)
		{
			if(event.getKey()==SORA_KEY_P)
				core->setState("play");
		}
	};
}

#endif