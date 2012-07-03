/*
 *  SoraMovieFilter.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MOVIE_FILTER_H_
#define SORA_MOVIE_FILTER_H_

#include "SoraPlatform.h"

namespace sora {
	
	/**
	 *  Base class for a movie filter
	 **/
	
	class SoraMovieFilter {
	public:
		virtual void filterFrame(uint32* colorData, uint32 width, uint32 height) = 0;
	};
	
} // namespace sora

#endif // SORA_MOVIE_PLAYER_H_