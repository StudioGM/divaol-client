#pragma once

#include "ui/GUIChanEx/GUIChanButtonEx.h"

namespace gcn
{
	class SuperButtonEx : public ButtonEx
	{
	private:
		int x1, y1;
		int x2, y2;
		int x3, y3;
		int x4, y4;

	public:
		void setLook(std::wstring filename, Rectangle normal, Rectangle highlight, Rectangle pressed, Rectangle disabled, 
			int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, int x3 = 0, int y3 = 0, int x4 = 0, int y4 = 0);

		virtual void draw(Graphics* graphics);
	};
}