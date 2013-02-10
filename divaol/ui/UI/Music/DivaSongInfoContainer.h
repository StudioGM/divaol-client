#pragma once

#include "ui/structure.h"
#include "ui/GUIChanEx/GUIChanContainerEx.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{
	namespace MusicUI
	{

		using namespace gcn;

		class DivaSongInfoContainer :
			public ContainerEx
		{
		private:
			divamap::DivaMap mapInfo;
			bool textVisible;

		public:
			DivaSongInfoContainer();
			void setMap(const divamap::DivaMap& mapInfo);
			void setTextVisible(bool v);

			virtual void draw(Graphics *graphics);
		};
	}
}