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

	void LabelEx::draw(Graphics* graphics)
	{
		if (text != L"")
		{
			graphics->setFont(getFont());
			graphics->setColor(getForegroundColor());
			graphics->drawTextW(text, 0, (getHeight() - getFont()->getHeight())/2);
		}
	}
}