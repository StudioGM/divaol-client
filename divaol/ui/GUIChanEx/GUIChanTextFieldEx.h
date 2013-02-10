#pragma once

//#include "WWidgets/WTextfield.h"
#include "WTextfield.h"
#include "guichan.hpp"

namespace gcn
{
	class TextFieldEx :
		public gcn::WTextField
	{
	private:
		Image* image;
		gcn::Rectangle srcRect;

	public:
		TextFieldEx();
		void setImage(const std::wstring& filename, const gcn::Rectangle& srcRect);
		virtual void draw(Graphics* graphics);
	};
}