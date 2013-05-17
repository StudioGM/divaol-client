#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"

namespace gcn
{
	class TextBoxItemEx : public ListItemEx
	{
	protected:
		std::wstring text;
		Color color;
	public:
		TextBoxItemEx();
		TextBoxItemEx(const std::wstring& str, const Color& color);

		void setColor(const Color& color);
		void setText(const std::wstring& str);
		void draw(Graphics* graphics, Font* font, int state, int alpha);
	};

	class TextBoxEx : public ListBoxEx
	{
	protected:
		int totalMaxLine;
		void adjustTotalMaxLine();

	public:
		TextBoxEx();
		virtual ~TextBoxEx();


		void adjust();
		void setTotalMaxLine(int v);
		void setMaxLine(int v);
		void setText(const std::wstring& str);
		void setText(const std::wstring& str, const Color& color);
		void addText(const std::wstring& str);
		void addText(const std::wstring& str, const Color& color);

		void clearText();
	};
}