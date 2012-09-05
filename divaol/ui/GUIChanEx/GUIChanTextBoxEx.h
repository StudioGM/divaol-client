#pragma once

#include "ui/GUIChanEx/GUIChanListBoxEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"

namespace gcn
{
	class TextBoxItemEx : public ListItemEx
	{
	protected:
		std::wstring text;
	public:
		TextBoxItemEx();
		TextBoxItemEx(const std::wstring& str);

		void setText(const std::wstring& str);
		void draw(Graphics* graphics, Font* font, int state, int alpha);
	};

	class TextBoxEx : public ListBoxEx
	{
	protected:
		

	public:
		TextBoxEx();
		~TextBoxEx();


		void adjust();
		void setMaxLine(int v);
		void setText(const std::wstring& str);
		void addText(const std::wstring& str);
		void clearText();
	};
}