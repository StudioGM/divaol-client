#include "ui/UI/House/HouseMessageBox.h"

namespace diva
{
	namespace HouseUI
	{
		void HouseMessageBox::mouseWheelMovedUp(MouseEvent& mouseEvent)
		{
			if (getFirstIndex() > 0)
				setFirstIndex(getFirstIndex() - 1);
		}

		void HouseMessageBox::mouseWheelMovedDown(MouseEvent& mouseEvent)
		{
			if (items.size() <= maxItem)
				return;
			if (getFirstIndex() < items.size() - maxItem)
				setFirstIndex(getFirstIndex() + 1);
		}
	}
}