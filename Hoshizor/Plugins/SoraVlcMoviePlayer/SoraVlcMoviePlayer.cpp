//
//  SoraVlcMoviePlayer.cpp
//  Sora
//
//  Created by Griffin Bu on 3/21/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraVlcMoviePlayer.h"
#include "SoraException.h"
#include "SoraPlaybackEvent.h"
#include "SoraLogger.h"
#include "SoraTexture.h"
#include "Core/DivaStringUtil.h"

namespace sora {
    
    static void* lock(void* data, void** pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
		if(ctx->pixels) {
			if(ctx->internal == 2) {
				free(ctx->dummy);
				ctx->dummy = malloc(ctx->videoWidth*ctx->videoHeight*4);
			}
			ctx->internal = 0;
			*pixels = ctx->pixels;
		}
		else {
			ctx->internal = 1;
			*pixels = ctx->dummy;
		}
        ++ctx->frameCount;
    
        return 0;
    }
    
    static void unlock(void* data, void* id, void* const* pixels) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
		if(ctx->pixels && ctx->internal != 1) {
			ctx->bChanged = true;
			memcpy(ctx->dummy, ctx->pixels, ctx->pPlayer->getWidth()*ctx->pPlayer->getHeight()*4);
		}
    }
    
    static void display(void* data, void* id) {
       // SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;
        
        (void) data;
    }
    
    static void eventHandle(const libvlc_event_t* ev, void* data) {
        SoraVlcMoviePlayer::MP_CTX* ctx = (SoraVlcMoviePlayer::MP_CTX*)data;

        switch (ev->type) {
            case libvlc_MediaPlayerStopped: 
                ctx->bStopped = true; 
                ctx->bPlaying = false; 
                ctx->pPlayer->publishEvent(SoraPlaybackEvent::Stopped);
                break;
            case libvlc_MediaPlayerPaused: 
                ctx->bPaused = true; 
                ctx->bPlaying = false;
                ctx->pPlayer->publishEvent(SoraPlaybackEvent::Paused);
                break;
            case libvlc_MediaPlayerPlaying:  {
                ctx->pPlayer->publishEvent(ctx->bPaused?SoraPlaybackEvent::Resumed:SoraPlaybackEvent::Started);
                ctx->bPlaying = true;
                break;
			}
            case libvlc_MediaPlayerEndReached: 
                ctx->bPlaying = false; 
                ctx->bStopped = true; 
                ctx->pPlayer->publishEvent(SoraPlaybackEvent::Ended);
                break;
			case libvlc_MediaPlayerPositionChanged: {
				uint32 w = 0, h;
				if(libvlc_video_get_size(ctx->mp, 0, &w, &h) == -1) {
					libvlc_media_player_stop(ctx->mp);
					ctx->internal = 3;
					log_error("SoraVlcMoviePlayer: error getting video dimensions, exit");
				}
				ctx->pPlayer->setMediaInfo(w, h);
				ctx->bChanged = true;
				//libvlc_media_player_stop(ctx->mp);
				libvlc_event_detach(ctx->evtManager, libvlc_MediaPlayerPositionChanged, eventHandle, ctx);
				break;
			}
				
        }
    }
    
    SoraVlcMoviePlayer::SoraVlcMoviePlayer(bool bNoAudio): 
    vlcInstance(0), 
    mp(0), 
    media(0), 
    evtManager(0), 
    videoWidth(0), 
    videoHeight(0),
    mTexture(0),
	mIsPaused(true){
        const char* vlc_argv[] = {
            "--plugin-path=./Plugins",
            "--ignore-config",
			"--no-video-title",
			"-I", "dummy", "--noaudio"
           // ,"--verbose", "2"
        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
		if(!bNoAudio)
			vlc_argc--;

        vlcInstance = libvlc_new(vlc_argc, vlc_argv);
        
		mp = NULL;
        evtManager = NULL;
    }
    
    SoraVlcMoviePlayer::~SoraVlcMoviePlayer() {
		if(mp)
		{
			libvlc_media_player_release(mp);
			mp = 0;
		}
        if(vlcInstance)
            libvlc_release(vlcInstance);
    }
    
	bool SoraVlcMoviePlayer::openMedia()
	{
		if(mFilePath=="")
			return false;
		//mp = 0;
		return openMedia(mFilePath);
	}

    bool SoraVlcMoviePlayer::openMedia(const StringType& filePath) {
        if(!vlcInstance)
            return false;

        if(media) { libvlc_media_release(media); }

		mFilePath = filePath;

		media = libvlc_media_new_path(vlcInstance, divacore::GBKToUTF8(filePath).c_str());
		
		if(mp) {
			libvlc_media_player_release(mp);
		}
		mp = libvlc_media_player_new_from_media(media);

		if(!mp) {
			log_error("SoraVlcMoviePlayer: Error creating vlc media player");
			return false;
		}
		evtManager = libvlc_media_player_event_manager(mp);
        if(!evtManager) {
			log_error("SoraVlcMoviePlayer: Error creating event manager for the media player, event cannot not work");
			return false;
		}
		else {
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPaused, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPlaying, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerStopped, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerEndReached, eventHandle, &frameData);
			libvlc_event_attach(evtManager, libvlc_MediaPlayerPositionChanged, eventHandle, &frameData);
		}
        libvlc_media_release(media);
		media = 0;

		displayFormat = "RGBA";
#ifdef OS_WIN32
        displayFormat = "RV32";
#endif

        frameData.videoWidth = 0;
        frameData.videoHeight = 0;
        frameData.pixels = NULL;
        frameData.dummy = malloc(1024*1024*4);
		
		frameData.bPaused = frameData.bPlaying = frameData.bChanged = frameData.bStopped = false;
		
		frameData.mp = mp;
        frameData.pPlayer = this;
		frameData.evtManager = evtManager;
		
		libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);

		// play for first frame to get video infomartion
		libvlc_media_player_play(mp);

		while(!frameData.bChanged);

		frameData.bChanged = false;
		libvlc_media_player_stop(mp);
		
		if(frameData.internal == 3) {
			if(mp) {
				libvlc_media_player_release(mp);
			}
			return false;
		}
		
		return true;
	}
    
    void SoraVlcMoviePlayer::setMediaInfo(uint32 w, uint32 h) {
        if(!vlcInstance || !mp)
            return;
        
	    videoWidth = frameData.videoWidth = w;
		videoHeight = frameData.videoHeight = h;
		
		frameData.pixels = malloc(w*h*4);
		frameData.internal = 2;
	}
    
    void SoraVlcMoviePlayer::play() {
        if(!vlcInstance || !mp)
            return;
        
		libvlc_video_set_callbacks(mp, lock, unlock, display, &frameData);
		libvlc_video_set_format(mp, displayFormat.c_str(), videoWidth, videoHeight, videoWidth*4);

		frameData.bStopped = false;
		frameData.bPlaying = true;
        libvlc_media_player_play(mp);

		mIsPaused = false;
    }
    
    void SoraVlcMoviePlayer::resume() {
        if(!vlcInstance || !mp)
            return;
        
        libvlc_media_player_set_pause(mp, 0);
        frameData.bPlaying = true;
        frameData.bPaused = false;

		mIsPaused = false;
    }
    
    void SoraVlcMoviePlayer::stop() {
        if(!isPlaying()) return;

        libvlc_media_player_stop(mp);

		mIsPaused = true;
    }
    
    void SoraVlcMoviePlayer::pause() {
        if(!isPlaying()) return;

        libvlc_media_player_pause(mp);

		mIsPaused = true;
    }
    
    void SoraVlcMoviePlayer::setVolume(int32 vol) {
        if(!mp)
            return ;
        
        libvlc_audio_set_volume(mp, vol);
    }
    
    int32 SoraVlcMoviePlayer::getVolume() const {
        if(!mp)
            return 0;
        
        return libvlc_audio_get_volume(mp);
    }
    
    void SoraVlcMoviePlayer::setMute(bool flag) {
        if(!mp)
            return ;
        
        libvlc_audio_set_mute(mp, flag);
    }
    
    bool SoraVlcMoviePlayer::getMute() {
        if(!mp)
            return false;
        
        return libvlc_audio_get_mute(mp);
    }
    
    uint64 SoraVlcMoviePlayer::getLength() const {
        if(!mp)
            return 0;
        
        return libvlc_media_player_get_length(mp);
    }
    
    void SoraVlcMoviePlayer::setTime(uint64 newtime) {
        //while(!isPlaying()) 
		//	Sleep(1),resume();
        
        if(newtime > getLength()) newtime = getLength();
        
        libvlc_media_player_set_time(mp, (libvlc_time_t)newtime);
    }
    
    uint64 SoraVlcMoviePlayer::getTime() const {
        if(!mp)
            return 0;
        return libvlc_media_player_get_time(mp);
    }
    
    float SoraVlcMoviePlayer::getFPS() const {
        if(!mp)
            return 0.f;
        return libvlc_media_player_get_fps(mp);
    }
    
    int32 SoraVlcMoviePlayer::getAudioTrackNum() const {
        if(!mp)
            return 0;
        return libvlc_audio_get_track_count(mp);
    }
    
    int32 SoraVlcMoviePlayer::getAudioChannelNum() const {
        if(!mp)
            return 0;
        return libvlc_audio_get_channel(mp);
    }
    
    void SoraVlcMoviePlayer::toNextFrame() {
        if(!isPlaying()) return;
    
        libvlc_media_player_next_frame(mp);
    }

    float SoraVlcMoviePlayer::getPlayRate() const {
        if(!mp)
            return 0.f;
        
        return libvlc_media_player_get_rate(mp);
    }
    
    void SoraVlcMoviePlayer::setPlayRate(float rate) {
        if(!mp)
            return;
        
        libvlc_media_player_set_rate(mp, rate);
    }
    
    uint32 SoraVlcMoviePlayer::getWidth() const { 
        return videoWidth;
    }
    
    uint32 SoraVlcMoviePlayer::getHeight() const { 
        return videoHeight;
    }
   
    uint32 SoraVlcMoviePlayer::getFrameCount() const { 
        return frameData.frameCount;
    }        
    
    bool SoraVlcMoviePlayer::frameChanged() const { 
        return frameData.bChanged;
    }
    
    void SoraVlcMoviePlayer::setFinish() { 
        frameData.bChanged = false;
    }
    
    void* SoraVlcMoviePlayer::getPixelData() const { 
        return frameData.dummy;
    }
    
    bool SoraVlcMoviePlayer::isStopped() const { 
        return frameData.bStopped;
    }
    
    bool SoraVlcMoviePlayer::isPlaying() const { 
        return frameData.bPlaying; 
    }
    
    bool SoraVlcMoviePlayer::isPaused() const { 
        return frameData.bStopped; 
    }
    
    void SoraVlcMoviePlayer::bindTexture(SoraTextureHandle tex) {
        mTexture = tex;
    }
    
    void SoraVlcMoviePlayer::onUpdate(float dt) {
        if(mTexture != 0) {
            void* data = sora::SoraTexture::GetData(mTexture);
            
            sora_assert(SoraTexture::GetWidth(mTexture) >= getWidth() &&
                        SoraTexture::GetHeight(mTexture) >= getHeight());
            
            if(data) {
                memcpy(data, getPixelData(), getWidth() * getHeight() * sizeof(uint32));
            }
            sora::SoraTexture::PutData(mTexture);
        }

		//fix stupid bug
		if(isPlaying()==mIsPaused)
		{
			if(mIsPaused)
				pause();
			else
				resume();
		}
    }
} // namespace sora