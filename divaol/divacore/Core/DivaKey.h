/*
 *  DivaKey.h
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_KEY
#define DIVA_KEY

namespace divacore
{
	enum{
		NO_KEY = -1,
		DIVA_KEY_TYPE_0 = 0,
		DIVA_KEY_TYPE_1,
		DIVA_KEY_TYPE_2,
		DIVA_KEY_TYPE_3,
		DIVA_KEY_TYPE_4,
		DIVA_KEY_TYPE_5,
		DIVA_KEY_TYPE_6,
		DIVA_KEY_TYPE_7,
		DIVA_KEY_CT,
		//DIVA_KEY_RELAY_WANT_TO_CHANGE,
		//DIVA_KEY_RELAY_WANT_TO_PLAY,
		DIVA_KEY_RELAY,
		DIVA_KEY_SPACE
	};
	class KeyEvent
	{
	public:
		int32 key;
	};
}

#endif