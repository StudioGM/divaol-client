#pragma once

#include <string>
#include "ui/GUIChanEx/GUIChanSuperButtonEx.h"
#include "Guichan.hpp"

namespace gcn
{
	class MarkerEx :
		public ButtonEx
	{
	private:
		int upX1, upY1;
		int upX2, upY2;
		int upX3, upY3;
		int downX1, downY1;
		int downX2, downY2;
		int downX3, downY3;
		int midX1, midY1;
		int midX2, midY2;
		int midX3, midY3;

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
		void setOffset(int upX1,int upY1,
			int upX2, int upY2,
			int upX3, int upY3,
			int downX1, int downY1,
			int downX2, int downY2,
			int downX3, int downY3,
			int midX1, int midY1,
			int midX2, int midY2,
			int midX3, int midY3);

		virtual void draw(Graphics* graphics);
	};
}