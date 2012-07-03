/*
 *  DivaCoreState.cpp
 *
 *  Created by Hyf042 on 1/14/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include "DivaCoreState.h"

namespace divacore
{
	void CoreState::setCoreState(int state)
	{
		core->setState(state);
	}
	void CoreState::setMapInfo(MapInfoPtr info) 
	{
		core->mapInfo = info;
	}
}