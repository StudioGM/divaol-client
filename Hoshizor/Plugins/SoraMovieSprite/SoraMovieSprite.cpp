/*
 *  SoraMovieSprite.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraMovieSprite.h"
#include "SoraCore.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraMovieSprite::SoraMovieSprite(SoraMoviePlayer* moviePlayer, const SoraWString& videoPath):
    mMoviePlayer(moviePlayer) {
        assert(moviePlayer);
        
        if(!moviePlayer->openMedia(videoPath))
            log_error("Error open movie media");
        else {
            setTexture(SoraCore::Instance()->createTextureWH(moviePlayer->getWidth(),   moviePlayer->getHeight()));
        }
    }
    
    SoraMovieSprite::~SoraMovieSprite() {
        mMoviePlayer->stop();
    }
    
    void SoraMovieSprite::play() {
        if(mMoviePlayer)
            mMoviePlayer->play();
    }
    
    void SoraMovieSprite::addMovieFilter(SoraMovieFilter* movieFilter) {
        mMovieFilters.push_back(movieFilter);
    }
    
    void SoraMovieSprite::addPostProcessor(SoraShader* postProcessor) {
        mPostProcessors.push_back(postProcessor);
        attachShader(postProcessor);
    }
    
    void SoraMovieSprite::delMovieFilter(SoraMovieFilter* movieFilter) {
        mMovieFilters.remove(movieFilter);
    }
    
    void SoraMovieSprite::delPostProcessor(SoraShader* postProcessor) {
        mPostProcessors.remove(postProcessor);
        detachShader(postProcessor);
    }
    
    int32 SoraMovieSprite::update(float dt) {
        if(mQuad.tex == NULL || mMoviePlayer == NULL)
            return 0;
        
        if(mMoviePlayer->isPlaying() && mMoviePlayer->frameChanged()) {
            void* colorData = mMoviePlayer->getPixelData();
            uint32* texData = getPixelData();
            
            if(colorData && texData) {
                memcpy(texData, colorData, 
                       mMoviePlayer->getWidth()*mMoviePlayer->getHeight()*sizeof(uint32));
            }
            
            MOVIE_FILTER_LIST::iterator itMovieFilter = mMovieFilters.begin();
            while(itMovieFilter != mMovieFilters.end()) {
                (*itMovieFilter)->filterFrame(texData, mMoviePlayer->getWidth(), mMoviePlayer->getHeight());
                ++itMovieFilter;
            }
            
            unlockPixelData();
            mMoviePlayer->setFinish();
        }
        return 0;
    }
    
} // namespace sora

