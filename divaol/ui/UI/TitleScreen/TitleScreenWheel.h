#pragma once

#include "ui/GUIChanEx/GUIChanAnimeEx.h"

namespace diva
{
	using namespace gcn;

	class TitleScreenWheel : public AnimeEx
	{
	protected:
		float rotation;
		float speed;

	public:
		
		TitleScreenWheel();

		void setRSpeed(float speed);
		void update(float dt);
		void restart();
	};

}