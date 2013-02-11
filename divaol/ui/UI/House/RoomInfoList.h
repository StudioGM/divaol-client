#pragma once

#include "HouseUIRoomInfoListItem.h"
#include "ui/GUIChanEx/GUIChanListBoxEx.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class RoomInfoList
			: public ListBoxEx
		{
		private:
		public:
			virtual void itemClicked(int itemIndex);
			virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);
			virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);
		};

	}
}