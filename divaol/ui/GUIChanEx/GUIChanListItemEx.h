#pragma once

#include "guichan.hpp"
#include "SoraSprite.h"
#include "SoraCore.h"
#include <string>

namespace gcn
{
	class ListItemEx
	{
	private:

	public:
		ListItemEx();
		~ListItemEx();

		virtual void draw(Graphics* graphics, Font* font, int state, int alpha) = 0;
	};
}