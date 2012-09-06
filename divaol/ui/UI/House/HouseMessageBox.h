#pragma once

#include "ui/GUIChanEx/GUIChanTextBoxEx.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class HouseMessageBox : public TextBoxEx
		{
		private:
		public:
			virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);
			virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);
		};

	}
}