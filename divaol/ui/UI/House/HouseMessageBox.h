#pragma once

#include "ui/GUIChanEx/GUIChanTextBoxEx.h"
#include "ui/GUIChanEx/GUIChanSliderEx.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class MessageSlider : public SliderEx
		{
		private:
		public:
			virtual void markerPositionChanged(int v);
		};

		class HouseMessageBox : public TextBoxEx
		{
		private:
		public:
			virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);
			virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);
			virtual void firstIndexChanged(int v);
			virtual void itemChanged();
		};

	}
}