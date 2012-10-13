#include "GUIChanTextBoxEx.h"

namespace gcn
{
	TextBoxItemEx::TextBoxItemEx()
	{
		setText(L"");
		setColor(Color(255, 255, 255));
	}

	TextBoxItemEx::TextBoxItemEx(const std::wstring& str, const Color& color)
	{
		setText(str);
		setColor(color);
	}

	void TextBoxItemEx::setText(const std::wstring& str)
	{
		text = str;
	}

	void TextBoxItemEx::draw(Graphics* graphics, Font* font, int state, int alpha)
	{
		graphics->setFont(font);
		graphics->setColor(color);
		graphics->drawTextW(text, 0, 0);
	}

	void TextBoxItemEx::setColor(const Color& color)
	{
		this->color = color;
	}

	////////////////////////////////////
	
	TextBoxEx::TextBoxEx()
	{
		//setGap(gcn::Rectangle(0, 0, getWidth(), getHeight()), 0);
		setForegroundColor(Color(255, 255, 255));
		adjust();
	}

	TextBoxEx::~TextBoxEx()
	{
	}

	void TextBoxEx::setMaxLine(int v)
	{
		setMaxItem(v);
		adjust();
	}

	void TextBoxEx::adjust()
	{
		setHeight(getFont()->getHeight() * maxItem);
		setGap(gcn::Rectangle(0, 0, getWidth(), getFont()->getHeight()), 0);
	}

	void TextBoxEx::setText(const std::wstring& str)
	{
		setText(str, getForegroundColor());
	}

	void TextBoxEx::setText(const std::wstring& str, const Color& color)
	{
		clearText();
		addText(str, color);
	}

	void TextBoxEx::addText(const std::wstring& str, const Color& color)
	{
		std::wstring temp;
		Font* font = getFont();
		int w = getWidth();
		const wchar_t *p = str.c_str();
		int i = 0;

		while (*p)
		{
			temp = L"";
			while (*p)
			{
				std::wstring t2 = temp + (*p);
				int tt = font->getWidthW(t2);
				if (tt <= w && *p != L'\n')
					temp = t2;
				else
				{
					if (*p == L'\n')
						p++;
					break;
				}
				p++;
			}
			if (font->getWidthW(temp) <= w)
				pushItem(new TextBoxItemEx(temp, color));
		}

		setFirstIndex(getMaxIndex());
	}

	void TextBoxEx::addText(const std::wstring& str)
	{
		addText(str, getForegroundColor());
	}

	void TextBoxEx::clearText()
	{
		clearItems();
	}
}