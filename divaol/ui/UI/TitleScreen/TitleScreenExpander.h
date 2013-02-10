#pragma once

#include "ui/GUIChanEx/GUIChanAnimeEx.h"

namespace diva
{
	using namespace gcn;

	class TitleScreenExpander : public AnimeEx
	{
	protected:
		float fPeriod, period, sc;
		float now;

	public:

		TitleScreenExpander();
		~TitleScreenExpander();

		virtual void restart();
		void setPara(float period, float fPeriod, float sc);

		virtual void update(float dt);
	};

}