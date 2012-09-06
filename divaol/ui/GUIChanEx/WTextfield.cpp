
//#include <Windows.h>

#include "WTextfield.h"

#include "Hoshizora.h"

#include "guichan/font.hpp"
#include "soraguifont.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"

#include "ui/Config/DivaUIConfig.h"

namespace gcn
{
	WTextField::WTextField()
	{
		mCaretPosition = 0;
		mCaretPositionFlashCount=0;
		mXScroll = 0;
		mNumericMode = false;

		setFocusable(true);

		addMouseListener(this);
		addKeyListener(this);
	}

	WTextField::WTextField(const std::wstring& text)
	{
		mCaretPosition = 0;
		mCaretPositionFlashCount=0;
		mXScroll = 0;

		mText = text;
		adjustSize();

		setFocusable(true);

		addMouseListener(this);
		addKeyListener(this);
	}

	void WTextField::setText(const std::wstring& text)
	{
		if(text.size() < mCaretPosition )
		{
			mCaretPosition = text.size();
		}

		mText = text;
	}

	void WTextField::enableNumericMode(bool flag) {
		mNumericMode = flag;
	}

	bool WTextField::isNumericModeEnabled() const { 
		return mNumericMode;
	}

	int WTextField::getNumber() const {
		if(mNumericMode)
		{
			std::string strInS = sora::ws2sfast(getText());
			return atoi(strInS.c_str());
		}
		return 0;
	}

	double WTextField::getDouble(){
		return wsTod(mText);
	}

	float WTextField::getFloat(){
		return wsTof(mText);
	}

	void WTextField::draw(Graphics* graphics)
	{
		Color faceColor = getBaseColor();
		Color highlightColor, shadowColor;
		int alpha = getBaseColor().a;
		highlightColor = faceColor + 0x303030;
		highlightColor.a = alpha;
		shadowColor = faceColor - 0x303030;
		shadowColor.a = alpha;

		// Draw a border.
		graphics->setColor(shadowColor);
		graphics->drawLine(0, 0, getWidth() - 1, 0);
		graphics->drawLine(0, 1, 0, getHeight() - 2);
		graphics->setColor(highlightColor);
		graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
		graphics->drawLine(0, getHeight() - 1, getWidth() - 1, getHeight() - 1);

		// Push a clip area so the other drawings don't need to worry
		// about the border.
		graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, getHeight() - 2));

		graphics->setColor(getBackgroundColor());
		graphics->fillRectangle(Rectangle(0, 0, getWidth(), getHeight()));

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

		graphics->popClipArea();

	}



	void WTextField::drawCaret(Graphics* graphics, int x)
	{
		// Check the current clip area as a clip area with a different
		// size than the widget might have been pushed (which is the
		// case in the draw method when we push a clip area after we have
		// drawn a border).
		mCaretPositionFlashCount = (mCaretPositionFlashCount+1)%(mCaretPositionFlashMax*2);
		if(mCaretPositionFlashCount<mCaretPositionFlashMax)
		{
			const Rectangle clipArea = graphics->getCurrentClipArea();

			graphics->setColor(getForegroundColor());
			graphics->drawLine(x + 1, clipArea.height - 2, x + 1, 1);
		}
		
	}

	void WTextField::mousePressed(MouseEvent& mouseEvent)
	{
		if (mouseEvent.getButton() == MouseEvent::LEFT)
		{
			mCaretPosition = getFont()->getStringIndexAtW(mText, mouseEvent.getX() + mXScroll);
			fixScroll();
		}
	}

	void WTextField::mouseDragged(MouseEvent& mouseEvent)
	{
		mouseEvent.consume();
	}

	void WTextField::keyPressed(KeyEvent& keyEvent)
	{
		Key key = keyEvent.getKey();

		if(key.getValue() == Key::ESCAPE)  //Lost focus
		{
			setFocusable(false);
			setFocusable(true);
		}

		if (key.getValue() == Key::LEFT && mCaretPosition > 0)
		{
			--mCaretPosition;
		}

		else if (key.getValue() == Key::RIGHT && mCaretPosition < mText.size())
		{
			++mCaretPosition;
		}

		else if (key.getValue() == 1013 && mCaretPosition < mText.size())
		{
			mText.erase(mCaretPosition, 1);
		}
		//         else if (key.getValue() == Key::DELETE && mCaretPosition < mText.size())
		//         {
		//             mText.erase(mCaretPosition, 1);
		//         }

		else if (key.getValue() == Key::BACKSPACE && mCaretPosition > 0)
		{
			mText.erase(mCaretPosition - 1, 1);
			--mCaretPosition;
		}

		else if (key.getValue() == Key::ENTER)
		{
			distributeActionEvent();
		}

		else if (key.getValue() == Key::HOME)
		{
			mCaretPosition = 0;
		}

		else if (key.getValue() == Key::END)
		{
			mCaretPosition = mText.size();
		}

		else if ((key.isCharacter() && key.getValue() != Key::TAB)
					|| key.isIME())// ak add ime
		{
			if(!mNumericMode) {
				mText.insert(mCaretPosition, std::wstring(1,(wchar_t)key.getValue()));
				++mCaretPosition;
			} else {
				if(key.isNumber()||key.getValue()=='.'||key.getValue()=='-') {
					mText.insert(mCaretPosition, std::wstring(1,(wchar_t)key.getValue()));
					++mCaretPosition;
				}
			}
		}

		if (key.getValue() != Key::TAB)
		{
			keyEvent.consume();
		}

		fixScroll();

		mCaretPositionFlashCount=0;
	}

	void WTextField::adjustSize()
	{
		setWidth(getFont()->getWidthW(mText) + 7);
		adjustHeight();

		fixScroll();
	}

	void WTextField::adjustHeight()
	{
		setHeight(getFont()->getHeight() + 4);
	}

	void WTextField::fixScroll()
	{
		if (isFocused())
		{
			int caretX = getFont()->getWidthW(mText.substr(0, mCaretPosition));

			if (caretX - mXScroll >= getWidth() - 4)
			{
				mXScroll = caretX - getWidth() + 4;
			}
			else if (caretX - mXScroll <= 0)
			{
				mXScroll = caretX - getWidth() / 2;

				if (mXScroll < 0)
				{
					mXScroll = 0;
				}
			}
		}
	}

	void WTextField::setCaretPosition(unsigned int position)
	{
		if (position > mText.size())
		{
			mCaretPosition = mText.size();
		}
		else
		{
			mCaretPosition = position;
		}

		fixScroll();
	}

	unsigned int WTextField::getCaretPosition() const
	{
		return mCaretPosition;
	}

	const std::wstring& WTextField::getText() const
	{
		return mText;
	}

	void WTextField::fontChanged()
	{
		fixScroll();
	}
}
