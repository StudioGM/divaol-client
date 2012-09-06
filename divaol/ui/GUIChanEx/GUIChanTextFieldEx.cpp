#include "GUIChanTextFieldEx.h"

#include "guichan/font.hpp"
#include "soraguifont.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"



namespace gcn
{
	void TextFieldEx::setImage(const std::wstring& filename, const gcn::Rectangle& srcRect)
	{
		image = Image::load(filename);
		this->srcRect = srcRect;
	}

	void TextFieldEx::draw(Graphics* graphics)
	{
		// back ground
		graphics->drawImage(image, srcRect.x, srcRect.y, 0, 0, srcRect.width, srcRect.height);

		// content
		if (isFocused())
		{
			graphics->setColor(getSelectionColor());
			graphics->drawRectangle(Rectangle(0, 0, getWidth() - 2, getHeight() - 2));
			graphics->drawRectangle(Rectangle(1, 1, getWidth() - 4, getHeight() - 4));
		}

		if (isFocused())
		{
			drawCaret(graphics, getFont()->getWidthW(mText.substr(0, mCaretPosition)) - mXScroll);
		}

		graphics->setColor(getForegroundColor());
		graphics->setFont(getFont());
		graphics->drawTextW(mText.c_str(), 3 - mXScroll, 1);
	}
}