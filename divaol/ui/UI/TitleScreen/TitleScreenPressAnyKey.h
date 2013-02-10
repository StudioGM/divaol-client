#pragma once

#include "ui/GUIChanEx/GUIChanAnimeEx.h"

namespace diva
{
	using namespace gcn;

	class TitleScreenPressAnyKey : public AnimeEx
	{
	protected:
		float now;
		float period;

	public:
		TitleScreenPressAnyKey();

		virtual void restart();
		void setPara(float period);

		virtual void update(float dt);
	};

}