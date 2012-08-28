
#include "divaeditor/Widgets/WButton.h"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

#include "guichansetup.h"

namespace gcn
{
    WButton::WButton()
    {
		resetInit();

        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    WButton::WButton(const std::wstring& caption)
            : mCaption(caption)
    {
		resetInit();

        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

	void WButton::resetInit()
	{
		mHasMouse = false;
		mKeyPressed = false;
		mMousePressed = false;
		mAlignment = Graphics::CENTER;
		mSpacing = 4;
		image_normal = NULL;
		image_down = NULL;
		image_on = NULL;
		isUsingPic = false;
		checked = false;
	}

    void WButton::setCaption(const std::wstring& caption)
    {
        mCaption = caption;
    }

    const std::wstring& WButton::getCaption() const
    {
        return mCaption;
    }

	void WButton::setNormalImage(std::wstring filename, gcn::Rectangle srcRect)
	{
		if(filename!=L"")
		{
			if(image_normal)
				image_normal->free();
			image_normal = Image::load(filename);
		}

		srcRect_normal = srcRect;

		if(image_normal)
			isUsingPic = true;
	}

	void WButton::setOnImage(std::wstring filename, gcn::Rectangle srcRect)
	{
		if(filename!=L"")
		{
			if(image_on)
				image_on->free();
			image_on = Image::load(filename);
		}

		srcRect_on = srcRect;

		if(image_on)
			isUsingPic = true;
	}

	void WButton::setDownImage(std::wstring filename, gcn::Rectangle srcRect)
	{
		if(filename!=L"")
		{
			if(image_down)
				image_down->free();
			image_down = Image::load(filename);
		}

		srcRect_down = srcRect;
		if(image_down)
			isUsingPic = true;
	}

	void WButton::setChecked(bool checked)
	{
		this->checked = checked;
	}

    void WButton::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment WButton::getAlignment() const
    {
        return mAlignment;
    }

    void WButton::setSpacing(unsigned int spacing)
    {
        mSpacing = spacing;
    }

    unsigned int WButton::getSpacing() const
    {
      return mSpacing;
    }

    void WButton::draw(Graphics* graphics)
    {
		
        Color faceColor = getBaseColor();
        Color highlightColor, shadowColor;
        int alpha = getBaseColor().a;

        if (isPressed() || checked)
        {
			if(image_down!=NULL)
			{
				graphics->setColor(gcn::Color(255,255,255,255));
				graphics->drawImage(image_down,srcRect_down.x,srcRect_down.y,0,0,srcRect_down.width,srcRect_down.height);
			}
			else
			{
				faceColor = faceColor - 0x303030;
				faceColor.a = alpha;
				highlightColor = faceColor - 0x303030;
				highlightColor.a = alpha;
				shadowColor = faceColor + 0x303030;
				shadowColor.a = alpha;
			}
        }
		else if(isHasMouse() && image_on!=NULL)
		{
			graphics->setColor(gcn::Color(255,255,255,255));
			graphics->drawImage(image_on,srcRect_on.x,srcRect_on.y,0,0,srcRect_on.width,srcRect_on.height);
		}
        else
        {
			if(image_normal!=NULL)
			{
				graphics->setColor(gcn::Color(255,255,255,255));
				graphics->drawImage(image_normal,srcRect_normal.x,srcRect_normal.y,0,0,srcRect_normal.width,srcRect_normal.height);
			}
			else
			{
				highlightColor = faceColor + 0x303030;
				highlightColor.a = alpha;
				shadowColor = faceColor - 0x303030;
				shadowColor.a = alpha;
			}
        }

		if(!isUsingPic)
		{
			graphics->setColor(faceColor);
			graphics->fillRectangle(Rectangle(1, 1, getDimension().width-1, getHeight() - 1));

			graphics->setColor(highlightColor);
			graphics->drawLine(0, 0, getWidth() - 1, 0);
			graphics->drawLine(0, 1, 0, getHeight() - 1);

			graphics->setColor(shadowColor);
			graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
			graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);
		}

		if(getCaption()==L"")
			return;

        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        switch (getAlignment())
        {
          case Graphics::LEFT:
              textX = mSpacing;
              break;
          case Graphics::CENTER:
              textX = getWidth() / 2;
              break;
          case Graphics::RIGHT:
              textX = getWidth() - mSpacing;
              break;
          default:
              throw GCN_EXCEPTION("Unknown alignment.");
        }

		graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        if (isPressed())
        {
            graphics->drawTextW(getCaption(), textX + 1, textY + 1, getAlignment());
        }
        else
        {
            graphics->drawTextW(getCaption(), textX, textY, getAlignment());

            if (!isUsingPic && isFocused())
            {
                graphics->drawRectangle(Rectangle(2, 2, getWidth() - 4,
                                                  getHeight() - 4));
            }
        }
		
    }

    void WButton::adjustSize()
    {
        setWidth(getFont()->getWidthW(mCaption) + 2*mSpacing);
        setHeight(getFont()->getHeight() + 2*mSpacing);
    }

    bool WButton::isPressed() const
    {
        if (mMousePressed)
        {
            return mHasMouse;
        }
        else
        {
            return mKeyPressed;
        }
    }

    void WButton::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = true;
            mouseEvent.consume();
        }
    }

    void WButton::mouseExited(MouseEvent& mouseEvent)
    {
        mHasMouse = false;
    }

    void WButton::mouseEntered(MouseEvent& mouseEvent)
    {
        mHasMouse = true;
    }

    void WButton::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT
            && mMousePressed
            && mHasMouse)
        {
            mMousePressed = false;
            distributeActionEvent();
            mouseEvent.consume();
        }
        else if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            mMousePressed = false;
            mouseEvent.consume();
        }
    }

    void WButton::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void WButton::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::ENTER
            || key.getValue() == Key::SPACE)
        {
            mKeyPressed = true;
            keyEvent.consume();
        }
    }

    void WButton::keyReleased(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::ENTER
             || key.getValue() == Key::SPACE)
            && mKeyPressed)
        {
            mKeyPressed = false;
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void WButton::focusLost(const Event& event)
    {
        mMousePressed = false;
        mKeyPressed = false;
    }
    
    bool WButton::isHasMouse() const {
        return mHasMouse;
    }
}
