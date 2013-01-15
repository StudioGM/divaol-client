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
		setBaseColor(Color(255, 255, 255));
		setForegroundColor(Color(255, 255, 255));
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

	bool ButtonEx::getSelected() const
	{
		return isSelected;
	}

	void ButtonEx::setSelected(bool v /* = false */)
	{
		isSelected = v;
	}

	void ButtonEx::draw(Graphics* graphics)
	{
		if (image == NULL)
			return;
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
			if (!isSelected)
				rect = highlightRect;
			else
				rect = pressedRect;
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

		Color color = getBaseColor();
		graphics->setColor(Color(color.r, color.g, color.b, getAlpha()));
		graphics->drawImage(image, rect.x, rect.y, offx, offy, rect.width, rect.height);

		if (text != L"")
		{
			graphics->setFont(getFont());
			color = getForegroundColor();
			graphics->setColor(Color(color.r, color.g, color.b, getAlpha()));
			graphics->drawTextW(text, (getWidth() - getFont()->getWidthW(text))/2, (getHeight() - getFont()->getHeight())/2);
		}
	}
}