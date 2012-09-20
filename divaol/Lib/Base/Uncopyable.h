/****************************************************
 *  Uncopyable.h
 *
 *  Created by tempbuffer on 7/31/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class which is uncopyable
 ****************************************************/

#ifndef _BASE_UNCOPYABLE_
#define _BASE_UNCOPYABLE_

namespace Base
{
	class Uncopyable
	{
	public:
		Uncopyable() {}

	protected:
		Uncopyable(const Uncopyable &);
		Uncopyable& operator=(const Uncopyable &);
	};
}

#endif