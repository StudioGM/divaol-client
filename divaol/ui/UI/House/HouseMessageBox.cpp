#include "ui/UI/House/HouseMessageBox.h"
#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		void MessageSlider::markerPositionChanged(int v)
		{
			HouseUI::Instance()->MessageSliderSlided(v);
		}

		//

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

		void HouseMessageBox::firstIndexChanged(int v)
		{
			HouseUI::Instance()->MessageBoxFirstIndexChanged(v);
		}

		void HouseMessageBox::itemChanged()
		{
			HouseUI::Instance()->MessageBoxItemChanged();
		}

	}
}