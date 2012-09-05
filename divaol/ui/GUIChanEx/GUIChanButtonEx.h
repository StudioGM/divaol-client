#pragma once

#include "guichan.hpp"
#include "SoraSprite.h"
#include "SoraCore.h"
#include <string>

namespace gcn
{
	class ButtonEx : public Button
	{
	protected:
		int offx, offy;
		Image* image;
		Rectangle normalRect;
		Rectangle highlightRect;
		Rectangle pressedRect;
		Rectangle disabledRect;
		std::wstring text;
		std::wstring hSE;
		std::wstring cSE;

		bool checkIsEnabled();

	public:
		ButtonEx();
		~ButtonEx();

		void setLook(std::wstring filename, Rectangle normal, Rectangle highlight, Rectangle pressed, Rectangle disabled, int ox = 0, int oy = 0);
		void setText(const std::wstring & text);
		void setSE(const std::wstring &highlighted, const std::wstring clicked);

		virtual void mouseEntered(MouseEvent& mouseEvent);
		virtual void mouseClicked(MouseEvent& mouseEvent);

		virtual void draw(Graphics* graphics);

		void* userData;
	};

}