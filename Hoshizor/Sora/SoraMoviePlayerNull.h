//
//  SoraMoviePlayerNull.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/10/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraMoviePlayerNull_h
#define Sora_iPhone_SoraMoviePlayerNull_h

#include "SoraMoviePlayer.h"

namespace sora {
    
    class SoraMoviePlayerNull: public SoraMoviePlayer {
    public:
        virtual bool openMedia(const SoraWString& filePath, const SoraString& dis="RGBA") { return false; }
        virtual void play() {} 
        virtual void stop() {}
        virtual void pause() { }
        
        virtual void setVolume(int32 vol) { }
        virtual int32 getVolume() const { return 0; }
        
        virtual void setMute(bool flag) { }
        virtual bool getMute() { return false; }
        
        virtual void setTime(uint64 newtime) { }
        virtual uint64 getTime() const { return 0; }
        
        virtual bool isStopped() const { return false; }
        virtual bool isPlaying() const { return false; }
        virtual bool isPaused() const { return false; }
        
        virtual float32 getPlayRate() const { return 0.f; }
        virtual void setPlayRate(float32 rate) { }
        
        virtual uint64 getLength() const { return 0; }
        virtual float getFPS() const { return 0.f; }
        
        virtual uint32 getWidth() const { return 0; }
        virtual uint32 getHeight() const { return 0; }
        
        virtual void* getPixelData() const { return NULL; }
        virtual uint32 getFrameCount() const { return 0; }
        virtual bool frameChanged() const { return false; }
        virtual void setFinish() { }
    };
    
}


#endif
