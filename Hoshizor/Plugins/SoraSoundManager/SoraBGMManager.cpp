/*
 *  SoraBGMManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraBGMManager.h"
#include "SoraCore.h"
#include "SoraEnvValues.h"

namespace sora {

	SoraBGMManager::SoraBGMManager(): bgmVolume(100.f), bgmPitch(-2.f), bgmPan(-2.f), 
										mCurrFadeInTime(-1.f), mCurrFadeOutTime(-1.f), mFadeInTime(0.f), mFadeOutTime(0.f), 
										mStopAtEnd(false), mRandomBGMQueuePlay(false), mPaused(false),
										mPrevBGMId(-1), mCurrBGMId(-1) {
		registerEventFunc(this, &SoraBGMManager::onPlaybackEvent);
	
		SET_ENV_FLOAT("BgmVolume", bgmVolume);
	}
	
	void SoraBGMManager::_clearBGMQueue() {
		if(!mBGMQueue.empty()) {
			for(uint32 i=0; i<mBGMQueue.size(); ++i) {
				if(mBGMQueue[i] != NULL) {
					delete mBGMQueue[i];
					mBGMQueue[i] = NULL;
				}
			}
			mBGMQueue.clear();
		}
		
		mCurrFadeInTime = -1.f;
		mCurrFadeOutTime = -1.f;
		mPrevBGMId = -1;
		mCurrBGMId = -1;
		mStopAtEnd = false;
		mPaused = false;
	}
	
	SoraBGMManager::~SoraBGMManager() {
		_clearBGMQueue();
	}
	
	bool SoraBGMManager::play(const std::wstring& bgmPath, bool addToQueue, bool isReapeat) {
		if(!addToQueue)
			_clearBGMQueue();
	
		SoraMusicFile* pmfile = sora::SoraCore::Ptr->createMusicFile(bgmPath);
		if(pmfile != NULL) {
			mBGMQueue.push_back(pmfile);
			if(!addToQueue || mBGMQueue.size() == 1) {
				_playBGM(pmfile, 0, isReapeat);
			}
			return true;
		}
		return false;
	}

	bool SoraBGMManager::playSE(const std::wstring &sePath, float volume) {
		SoraMusicFile* pmfile = sora::SoraCore::Ptr->createMusicFile(sePath);
		if(pmfile != NULL) {
			pmfile->play();
			pmfile->setRepeat(false);
			pmfile->setVolume(volume);
			return true;
		}
		return false;
	}

	void SoraBGMManager::_playBGM(SoraMusicFile* musicFile, uint32 newBGMId, bool isReapeat) {
		if(mFadeOutTime != 0.f && mCurrBGMId != -1) {
			mPrevBGMId = mCurrBGMId;
			mCurrFadeOutTime = 0.f;
		}
		else {
			if(mCurrBGMId >= 0 && mCurrBGMId < (int32)mBGMQueue.size()) {
				mBGMQueue[mCurrBGMId]->stop();
			}
		}
		if(mFadeInTime != 0.f) 
			mCurrFadeInTime = 0.f;
		
		musicFile->enableEventPublish(true);
		musicFile->registerEventHandler(this);
		
		musicFile->play();
        musicFile->setRepeat(isReapeat);
		musicFile->setVolume(bgmVolume);
		if(bgmPitch != -2.f)
			musicFile->setPitch(bgmPitch);
		if(bgmPan != -2.f)
			musicFile->setPan(bgmPan);
		
		mCurrBGMId = newBGMId;
		mPaused = false;
	}
	
	void SoraBGMManager::pause() {
		if(mPaused)
			return;
		
		mPaused = true;
		if(mCurrBGMId >= 0 && mCurrBGMId < (int32)mBGMQueue.size())
			mBGMQueue[mCurrBGMId]->pause();
		else
			mPaused = false;
	}
	
	void SoraBGMManager::resume() {
		if(mPaused) {
			if(mCurrBGMId >= 0 && mCurrBGMId < (int32)mBGMQueue.size())
				mBGMQueue[mCurrBGMId]->resume();
			mPaused = false;
		}
	}
	
	void SoraBGMManager::stop(bool bStopAtEnd) {
		if(!bStopAtEnd) {
			_clearBGMQueue();
		} else
			mStopAtEnd = bStopAtEnd;
	}
	
	void SoraBGMManager::freeAllBGMs() {
		_clearBGMQueue();
	}
	
	void SoraBGMManager::toNextBGM() {
		if(mRandomBGMQueuePlay) {
			uint32 newId = SoraCore::Ptr->RandomInt(0, mBGMQueue.size());
			_playBGM(mBGMQueue[newId], newId);
		}
		else if(mCurrBGMId != -1 && mCurrBGMId < mBGMQueue.size()-1) {
			_playBGM(mBGMQueue[mCurrBGMId+1], mCurrBGMId+1);
		} else {
			if(mStopAtEnd)
				_clearBGMQueue();
			else {
				if(mBGMQueue.size() != 0) {
					_playBGM(mBGMQueue[0], 0);
				}
			}
		}
	}
	
	void SoraBGMManager::toPrevBGM() {
		if(mCurrBGMId > 0) {
			_playBGM(mBGMQueue[mCurrBGMId-1], mCurrBGMId-1);
		}
	}
	
	uint32 SoraBGMManager::getQueueSize() const {
		return mBGMQueue.size();
	}
	
	void SoraBGMManager::setVolume(float volume) { 
		SET_ENV_FLOAT("BgmVolume", bgmVolume);
		
		if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size())
			mBGMQueue[mCurrBGMId]->setVolume(volume);
		bgmVolume = volume;
	}
	
	float SoraBGMManager::getVolume() const {
		return bgmVolume;
	}
	
	void SoraBGMManager::setPitch(float pitch) {
		bgmPitch = pitch;
	}
	
	float SoraBGMManager::getPitch() const {
		return bgmPitch;
	}
	
	void SoraBGMManager::setPan(float pan) {
		bgmPan = pan;
	}
	
	float SoraBGMManager::getPan() const {
		return bgmPan;
	}
	
	void SoraBGMManager::setFadeTime(float fadeInTime, float fadeOutTime) {
		mFadeInTime	 = fadeInTime;
		mFadeOutTime = fadeOutTime;
	}
	
	float SoraBGMManager::getFadeInTime() const {
		return mFadeInTime;
	}
	
	float SoraBGMManager::getFadeOutTime() const {
		return mFadeOutTime;
	}
	
	void SoraBGMManager::enableRandomBGMQueuePlay(bool flag) {
		mRandomBGMQueuePlay = flag;
	}
	
	bool SoraBGMManager::isRandomBGMQueuePlayEnabled() const {
		return mRandomBGMQueuePlay;
	}
	
	void SoraBGMManager::onUpdate(float delta) {
		if(mPaused)
			return; 
				
		if(mCurrFadeOutTime != -1.f && mPrevBGMId != -1) {
			if(mCurrFadeOutTime < mFadeOutTime) {
				mCurrFadeOutTime += delta;
				if(mCurrFadeOutTime >= mFadeOutTime) {
					mBGMQueue[mPrevBGMId]->stop();
					
					mCurrFadeOutTime = -1.f;
					mPrevBGMId = -1;
				} else {
					mBGMQueue[mPrevBGMId]->setVolume(bgmVolume * (1.f - (mCurrFadeOutTime / mFadeOutTime)));
				}
			}
		}
		if(mCurrFadeInTime != -1.f && mCurrBGMId != -1) {
			if(mCurrFadeInTime < mFadeInTime) {
				mCurrFadeInTime += delta;
				if(mCurrFadeInTime >= mFadeInTime) {
					mCurrFadeInTime = -1.f;
					mBGMQueue[mCurrBGMId]->setVolume(bgmVolume);
				} else {
					mBGMQueue[mCurrBGMId]->setVolume(bgmVolume * (mCurrFadeInTime / mFadeInTime));
				}
			}
		}
	}
	
	void SoraBGMManager::onPlaybackEvent(SoraPlaybackEvent* event) {
		if(event->getEventType() == SoraPlaybackEvent::Ended) {
			toNextBGM();
		}
        
        mPlaybackSignal(event);
	}
	
	
	int32 SoraBGMManager::playBGS(const std::wstring& bgmsPath, uint32 bgsid, int32 looptimes, float volumeScale, float bgmVolumeScale) {
		BGS_MAP::iterator itBgs = mBGSounds.find(bgsid);
		while(itBgs != mBGSounds.end()) {
			if(bgmsPath.compare(itBgs->second->path) != 0) {
				if(itBgs->second->bgsFile == NULL || itBgs->second->bPlayFinished == true) {
					delete itBgs->second;
					mBGSounds.erase(itBgs);
					break;
				}
			} else {
				if(itBgs->second->bgsFile != NULL) {
					itBgs->second->bgsFile->setVolume(volumeScale * bgmVolume);
					itBgs->second->bgsFile->play();
					itBgs->second->bPlayFinished = false;
					if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size()) {
						mBGMQueue[mCurrBGMId]->setVolume(bgmVolume * bgmVolumeScale);
					}
					return bgsid;
				}
			}
			
			++bgsid;
			itBgs = mBGSounds.find(bgsid);
		}
		
		SoraMusicFile* mfile = SoraCore::Ptr->createMusicFile(bgmsPath);
		if(mfile) {
			BGSInfo* bgsinfo = new BGSInfo(bgsid, looptimes, volumeScale, bgmVolumeScale, mfile);
			bgsinfo->path = bgmsPath;
			
			mfile->setVolume(volumeScale * bgmVolume);
			
			if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size()) {
				mBGMQueue[mCurrBGMId]->setVolume(bgmVolume * bgmVolumeScale);
			}
			mBGSounds[bgsid] = bgsinfo;
			mfile->play();
			
			return bgsid;
		}
		return -1;
	}
	
	void SoraBGMManager::BGSInfo::onPlaybackEvent(SoraPlaybackEvent* event) {
		if(event->getEventType() == SoraPlaybackEvent::Ended) {
			++currLoopTimes;
			if(currLoopTimes >= loopTimes) {
				
				SoraBGMManager::Instance()->setVolume(SoraBGMManager::Instance()->getVolume());
				bPlayFinished = true;
				
			} else {
				bgsFile->play();
			}
		}
	}
	
	void SoraBGMManager::adjustBGSVolume(uint32 bgsid, float volumeScale) {
		BGS_MAP::iterator itBgs = mBGSounds.find(bgsid);
		if(itBgs != mBGSounds.end()) {
			if(itBgs->second->bgsFile != NULL) {
				itBgs->second->bgsFile->setVolume(volumeScale * bgmVolume);
			}
		}
	}
	
	void SoraBGMManager::stopBGS(uint32 bgsid) {
		BGS_MAP::iterator itBgs = mBGSounds.find(bgsid);
		if(itBgs != mBGSounds.end()) {
			delete itBgs->second;
			mBGSounds.erase(itBgs);
		}
	}
    
    template<typename T>
    SoraConnection SoraBGMManager::subscribeToPlaybackEvent(const T& func) {
        return mPlaybackSignal.connect(func);
    }

} // namespace sora