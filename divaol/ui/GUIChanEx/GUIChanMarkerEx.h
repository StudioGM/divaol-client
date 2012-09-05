#pragma once

#include <string>
#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "Guichan.hpp"

namespace gcn
{
	class MarkerEx :
		public Button
	{
	private:
		int offx, offy;
		Image* image;
		Rectangle upNormal;
		Rectangle upHightlight;
		Rectangle upPressed;
		Rectangle downNormal;
		Rectangle downHightlight;
		Rectangle downPressed;
		Rectangle midNormal;
		Rectangle midHightlight;
		Rectangle midPressed;

	public:

		MarkerEx();
		~MarkerEx();

		void setLook(const std::wstring &filename, 
			Rectangle upNormal,
			Rectangle upHightlight,
			Rectangle upPressed,
			Rectangle downNormal,
			Rectangle downHightlight,
			Rectangle downPressed,
			Rectangle midNormal,
			Rectangle midHightlight,
			Rectangle midPressed, int ox = 0, int oy = 0);

		virtual void draw(Graphics* graphics);
	};
}