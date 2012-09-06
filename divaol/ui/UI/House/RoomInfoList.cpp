#include "RoomInfoList.h"

#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		void RoomInfoList::itemClicked(int itemIndex)
		{
			HouseUI::Instance()->RoomLInfoListClicked(itemIndex);
		}
		
		void RoomInfoList::mouseWheelMovedUp(MouseEvent& mouseEvent)
		{
		}

		void RoomInfoList::mouseWheelMovedDown(MouseEvent& mouseEvent)
		{

		}
	}
}