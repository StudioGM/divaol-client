#include "GUIChanLabelEx.h"

namespace gcn
{
	LabelEx::LabelEx(std::wstring v /* = "" */)
	{
		setOpaque(false);
		setText(v);
	}

	LabelEx::~LabelEx()
	{
	}

	void LabelEx::setText(const std::wstring& v)
	{
		text = v;
	}

	std::wstring LabelEx::getText() const
	{
		return text;
	}

	void LabelEx::adjustLabelSize()
	{
		setWidth(getFont()->getWidthW(text) + 2);
		setHeight(getFont()->getHeight() + 2);
	}

	void LabelEx::draw(Graphics* graphics)
	{
		if (text != L"")
		{
			Color c = getForegroundColor();
			graphics->setFont(getFont());
			graphics->setColor(Color(c.r, c.g, c.b, getAlpha()));
			//graphics->setColor(getForegroundColor());
			graphics->drawTextW(text, 0, (getHeight() - getFont()->getHeight())/2);
		}
	}
}