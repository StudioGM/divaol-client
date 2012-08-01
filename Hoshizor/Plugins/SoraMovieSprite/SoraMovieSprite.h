/*
 *  SoraMovieSprite.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MOVIE_SPRITE_H_
#define SORA_MOVIE_SPRITE_H_

#include "SoraSprite.h"
#include "SoraMoviePlayer.h"

#include "SoraMovieFilter.h"

namespace sora {
	
	class SoraMovieSprite: public SoraSprite {
	public:
		SoraMovieSprite(SoraMoviePlayer* moviePlayer, const SoraWString& videoPath);
		~SoraMovieSprite();
		
		int32 update(float dt);
        void play();
		
        void addMovieFilter(SoraMovieFilter* movieFilter);
        void addPostProcessor(SoraShader* postProcessor);
        
        void delMovieFilter(SoraMovieFilter* movieFilter);
        void delPostProcessor(SoraShader* postProcessor);
                
	private:
		SoraMoviePlayer* mMoviePlayer;
        
        typedef std::list<SoraMovieFilter*> MOVIE_FILTER_LIST;
        MOVIE_FILTER_LIST mMovieFilters;
        
        typedef std::list<SoraShader*> MOVIE_POST_PROCESSOR_LIST;
        MOVIE_POST_PROCESSOR_LIST mPostProcessors;
	};
	
	
} // namespace sora


#endif // SORA_MOVIE_SPRITE_H_