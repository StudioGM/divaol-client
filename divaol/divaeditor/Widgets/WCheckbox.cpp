#include "divaeditor/Widgets/WCheckbox.h"

#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseinput.hpp"

namespace gcn
{

    WCheckBox::WCheckBox()
    {
        setSelected(false);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    WCheckBox::WCheckBox(const std::wstring &caption, bool selected)
    {
        setCaption(caption);
        setSelected(selected);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    void WCheckBox::draw(Graphics* graphics)
    {
        drawBox(graphics);

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());

        const int h = getHeight() + getHeight() / 2;

        graphics->drawTextW(getCaption(), h - 2, 0);
    }

    void WCheckBox::drawBox(Graphics *graphics)
    {
        const int h = getHeight() - 2;
        const int alpha = getBaseColor().a;
        Color faceColor = getBaseColor();
        faceColor.a = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        Color shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(shadowColor);
        graphics->drawLine(1, 1, h, 1);
        graphics->drawLine(1, 1, 1, h);

        graphics->setColor(highlightColor);
        graphics->drawLine(h, 1, h, h);
        graphics->drawLine(1, h, h - 1, h);

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(Rectangle(2, 2, h - 2, h - 2));

        graphics->setColor(getCheckForeGroundColor());

        if (isFocused())
        {
            graphics->drawRectangle(Rectangle(0, 0, h + 2, h + 2));
        }

        if (mSelected)
        {
            graphics->drawLine(3, 5, 3, h - 2);
            graphics->drawLine(4, 5, 4, h - 2);

            graphics->drawLine(5, h - 3, h - 2, 4);
            graphics->drawLine(5, h - 4, h - 4, 5);
        }
    }

    bool WCheckBox::isSelected() const
    {
        return mSelected;
    }

    void WCheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    const std::wstring &WCheckBox::getCaption() const
    {
        return mCaption;
    }

    void WCheckBox::setCaption(const std::wstring& caption)
    {
        mCaption = caption;
    }

    void WCheckBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::ENTER ||
            key.getValue() == Key::SPACE)
        {
            toggleSelected();
            keyEvent.consume();
        }
    }

    void WCheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            toggleSelected();
        }
    }

    void WCheckBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void WCheckBox::adjustSize()
    {
        int height = getFont()->getHeight();

        setHeight(height+3);
        setWidth(getFont()->getWidthW(mCaption) + height + height / 2);
    }

    void WCheckBox::toggleSelected()
    {
        mSelected = !mSelected;
        distributeActionEvent();
    }

	void WCheckBox::setCheckForeGroundColor(Color color)
	{
		checkForeGroundColor = color;
	}

	Color WCheckBox::getCheckForeGroundColor()
	{
		return checkForeGroundColor;
	}
}

