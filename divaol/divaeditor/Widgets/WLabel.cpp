#include "divaeditor/Widgets/Wlabel.h"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"

namespace gcn
{
    WLabel::WLabel()
    {
        mAlignment = Graphics::LEFT;
    }

    WLabel::WLabel(const std::wstring& caption)
    {
        mCaption = caption;
        mAlignment = Graphics::LEFT;

        setWidth(getFont()->getWidthW(caption));
        setHeight(getFont()->getHeight());
    }

    const std::wstring &WLabel::getCaption() const
    {
        return mCaption;
    }

    void WLabel::setCaption(const std::wstring& caption)
    {
        mCaption = caption;
    }

    void WLabel::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment WLabel::getAlignment() const
    {
        return mAlignment;
    }

    void WLabel::draw(Graphics* graphics)
    {
        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        switch (getAlignment())
        {
          case Graphics::LEFT:
              textX = 0;
              break;
          case Graphics::CENTER:
              textX = getWidth() / 2;
              break;
          case Graphics::RIGHT:
              textX = getWidth();
              break;
          default:
              throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawTextW(getCaption(), textX, textY, getAlignment());
    }

    void WLabel::adjustSize()
    {
        setWidth(getFont()->getWidthW(getCaption()));
        setHeight(getFont()->getHeight()+3);
    }
}
