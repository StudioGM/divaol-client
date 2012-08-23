
#include "divaeditor/Widgets/WButton.h"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{
    WButton::WButton()
        : mHasMouse(false),
          mKeyPressed(false),
          mMousePressed(false),
          mAlignment(Graphics::CENTER),
          mSpacing(4)
    {
        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    WButton::WButton(const std::wstring& caption)
            : mCaption(caption),
              mHasMouse(false),
              mKeyPressed(false),
              mMousePressed(false),
              mAlignment(Graphics::CENTER),
              mSpacing(4)
    {
        setFocusable(true);
        adjustSize();
        setFrameSize(1);

        addMouseListener(this);
        addKeyListener(this);
        addFocusListener(this);
    }

    void WButton::setCaption(const std::wstring& caption)
    {
        mCaption = caption;
    }

    const std::wstring& WButton::getCaption() const
    {
        return mCaption;
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

        if (isPressed())
        {
            faceColor = faceColor - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor + 0x303030;
            shadowColor.a = alpha;
        }
        else
        {
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;
        }

        graphics->setColor(faceColor);
        graphics->fillRectangle(Rectangle(1, 1, getDimension().width-1, getHeight() - 1));

        graphics->setColor(highlightColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, getHeight() - 1);

        graphics->setColor(shadowColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, getHeight() - 1);
        graphics->drawLine(1, getHeight() - 1, getWidth() - 1, getHeight() - 1);

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

            if (isFocused())
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
