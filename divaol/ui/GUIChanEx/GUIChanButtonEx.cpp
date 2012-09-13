#include "ui/GUIChanEx/GUIChanButtonEx.h"
#include "SoraSoundManager/SoraBGMManager.h"

#include "soraguigraphics.hpp"

namespace gcn
{
	ButtonEx::ButtonEx()
	{
		image = NULL;
		offx = 0;
		offy = 0;
		text = L"";
		isSelected = false;
		setFrameSize(0);
	}

	ButtonEx::~ButtonEx()
	{
		if (image)
		{
			delete image;
			image = NULL;
		}
	}

	void ButtonEx::setLook(std::wstring filename, Rectangle normal, Rectangle highlight, Rectangle pressed, Rectangle disabled, int ox, int oy)
	{
		using namespace sora;

		if (image)
		{
			delete image;
			image = NULL;
		}
		image = Image::load(filename);

		normalRect = normal;
		highlightRect = highlight;
		pressedRect = pressed;
		disabledRect = disabled;

		offx = ox;
		offy = oy;
	}

	void ButtonEx::setSE(const std::wstring &highlighted, const std::wstring clicked)
	{
		hSE = highlighted;
		cSE = clicked;
	}

	void ButtonEx::setText(const std::wstring & text)
	{
		this->text = text;
	}

	void ButtonEx::mouseEntered(MouseEvent& mouseEvent)
	{
		if (!checkIsEnabled())
			return;
		Button::mouseEntered(mouseEvent);
		if (hSE != L"")
		{
			sora::SoraBGMManager::Instance()->playBGS(hSE, 0, 1, 2.0, 1.0); 
		}
	}

	void ButtonEx::mouseClicked(MouseEvent& mouseEvent)
	{
		if (!checkIsEnabled())
			return;
		Button::mouseClicked(mouseEvent);
		if (cSE != L"")
		{
			sora::SoraBGMManager::Instance()->playBGS(hSE, 0, 1, 2.0, 1.0); 
		}
	}

	void ButtonEx::setSelected(bool v /* = false */)
	{
		isSelected = v;
	}

	void ButtonEx::draw(Graphics* graphics)
	{
		Rectangle rect;
		if (!checkIsEnabled())
		{
			//image->setTextureRect(disabledRect);
			rect = disabledRect;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isHasMouse())
		{
			//image->setTextureRect(normalRect);
			if (!isSelected)
				rect = normalRect;
			else
				rect = pressedRect;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else if (!isPressed())
		{
			//image->setTextureRect(highlightRect);
			rect = highlightRect;
			//graphics->drawImage(image, 0, 0);
			//return;
		}
		else
		{
			rect = pressedRect;
			//image->setTextureRect(pressedRect);
		}

		//gcn::Rectangle temp = graphics->getCurrentClipArea();
		//graphics->popClipArea();
		//((SoraGUIGraphics*)graphics)->setNextNoClip();
		//graphics->pushClipArea(temp);

		graphics->setColor(Color(255,255,255,getAlpha()));
		graphics->drawImage(image, rect.x, rect.y, offx, offy, rect.width, rect.height);

		if (text != L"")
		{
			graphics->setFont(getFont());
			graphics->setColor(Color(255,255,255,getAlpha()));
			graphics->drawTextW(text, (getWidth() - getFont()->getWidthW(text))/2, (getHeight() - getFont()->getHeight())/2);
		}
	}
}