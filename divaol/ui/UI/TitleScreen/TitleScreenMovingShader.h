#pragma once

#include "ui/GUIChanEx/GUIChanAnimeEx.h"

namespace diva
{
	using namespace gcn;

	class TitleScreenMovingShader : public AnimeEx
	{
	protected:
		float now;
		float period;

	public:
		TitleScreenMovingShader();

		virtual void restart();
		void setPara(float period);

		virtual void update(float dt);
	};

}