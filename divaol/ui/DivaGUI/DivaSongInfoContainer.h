#pragma once

#include "ui/structure.h"
#include "ui/GUIChanEx/GUIChanContainerEx.h"

namespace diva
{
	using namespace gcn;

	class DivaSongInfoContainer :
		public ContainerEx
	{
	private:
		Network::Map mapInfo;
		bool textVisible;

	public:
		DivaSongInfoContainer();
		void setMap(const Network::Map& mapInfo);
		void setTextVisible(bool v);

		virtual void draw(Graphics *graphics);
	};
}