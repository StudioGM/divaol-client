#include "GUIChanSuperButtonEx.h"

#include "soraguigraphics.hpp"

namespace gcn
{
	void SuperButtonEx::setLook(std::wstring filename, Rectangle normal, Rectangle highlight, Rectangle pressed, Rectangle disabled, int x1 /* = 0 */, int y1 /* = 0 */, int x2 /* = 0 */, int y2 /* = 0 */, int x3 /* = 0 */, int y3 /* = 0 */, int x4 /* = 0 */, int y4 /* = 0 */)
	{
		ButtonEx::setLook(filename, normal, highlight, pressed, disabled, 0, 0);
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->x3 = x3;
		this->y3 = y3;
		this->x4 = x4;
		this->y4 = y4;
	}

	void SuperButtonEx::draw(Graphics* graphics)
	{
		Rectangle rect;
		int ox = 0, oy = 0;
		if (!checkIsEnabled())
		{
			//image->setTextureRect(disabledRect);
			rect = disabledRect;
			ox = x4;
			oy = y4;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isHasMouse())
		{
			//image->setTextureRect(normalRect);
			rect = normalRect;
			ox = x1;
			oy = y1;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isPressed())
		{
			//image->setTextureRect(highlightRect);
			rect = highlightRect;
			ox = x2;
			oy = y2;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else
		{
			rect = pressedRect;
			ox = x3;
			oy = y3;
			//image->setTextureRect(pressedRect);
		}


		gcn::Rectangle temp = graphics->getCurrentClipArea();
		//
		//graphics->popClipArea();
		//((SoraGUIGraphics*)graphics)->setNextNoClip();
		//graphics->pushClipArea(temp);

		graphics->setColor(Color(255,255,255,getAlpha()));
		graphics->drawImage(image, rect.x, rect.y, ox, oy, rect.width, rect.height);

		if (text != L"")
		{
			graphics->setFont(getFont());
			graphics->setColor(Color(255,255,255,getAlpha()));
			graphics->drawTextW(text, 20, (getHeight() - getFont()->getHeight())/2);
		}
	}
}