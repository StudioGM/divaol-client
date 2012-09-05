/*
 *  SoraBGMManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_BGM_MANAGER_H_
#define SORA_BGM_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"
#include "SoraSingleton.h"
#include "SoraAutoUpdate.h"
#include "SoraStringId.h"

#include "signal/SoraSignal.h"

namespace sora {
	
	typedef SoraStringId BGMId;
	
	/*
	 simple interface for bgm management
	 also with some sound management features
	 a bgm would play in loop mode
	 */
	
	class SoraBGMManager: public SoraSingleton<SoraBGMManager>, public SoraEventHandler, public SoraAutoUpdate {
		friend class SoraSingleton<SoraBGMManager>;
	 
	protected:
		SoraBGMManager();
		~SoraBGMManager();
		
		void onUpdate(float dt);
		const SoraString getName() const { return "SoraBGMManager"; }
		
		void onPlaybackEvent(SoraPlaybackEvent* event);
		
	public:
		/*
		 plays a bgm
		 @param bgmPath, the path of the bgm to play
		 @param addToQueue, is add the bgm to the bgm queue
		*/
		bool   play(const std::wstring& bgmPath, bool addToQueue);
		/*
		 stops the bgm
		 @param stopAtEnd, is stop the bgm queue at the end of it
				notice if random bgm playback is enabled, 
				the queue may never reach the end
		 */
		void   stop(bool stopAtEnd);
		
		/*
		 plays a looping background sound that mixes with the bgm
		 @param bgmsPath, the path of the sound
		 @param bgsid, the user defined id of the bgs
		 @param looptime, the loop time of the bgs, pass -1 for infinite loop
		 @param volume, the volume of the bg sound, this is a scale relative to the bgmVolume, 0.0 - 1.0
		 @param bgmVolume adjustment, this is a scale relative to the bgmVolume, use 0.0 - 1.0 to adjust bgmvolume to a certain percentage
		 @return the id represents the bg sound, it is the same with the user defined id if no error happened
				if the user defined id had already be token in the bgs queue, then the returned id would be the
				first available id after it
				return -1 means failed open the bg sound file
		*/
		int32	playBGS(const std::wstring& bgmsPath, uint32 bgsid, int32 looptimes, float volumeScale, float bgmVolumeScale);
		/*
		 adjust the volume of a certain bgs
		 @param volume, the new volume for the bgs
		 */
		void	adjustBGSVolume(uint32 bgsid, float volume);
		/*
		 stops a certain bgs
		 @param bgsid, the id of the bgs, see @playBGS
		*/
		void	stopBGS(uint32 bgsid);
		
		
		
		void	pause();
		void	resume();
		
		/*
		 free all BGMs in the bgm queue
		 */
		void   freeAllBGMs();
		
		/*
		 force skip to next/prev bgm file in the bgm queue
		 */
		void toNextBGM();
		void toPrevBGM();
		
		// get the size of current bgm queue
		uint32 getQueueSize() const;
		
		/*
		 set bgm playback properties
		 applies to all bgms in the bgm queue
		 */
		
		// 0 - 100.0
		void setVolume(float volume);
		float getVolume() const;
		
		// 0.5 - 2.0
		void setPitch(float pitch);
		float getPitch() const;
		
		// -1.0 - 1.0
		void setPan(float pan);
		float getPan() const;
		
		/*
		 if these times are not 0, 
		 then when a new bgm starts, it will fade in fadeInTime seconds, 
		 the old bgm would fade out in fadeOutTime seconds
		 default are zero
		 */
		void setFadeTime(float fadeInTime, float fadeOutTime);
		float getFadeInTime() const;
		float getFadeOutTime() const;
		
		/*
		 if this is enabled, them when a bgm ends, new bgm would be a random item in the bgm queue
		 default false
		 */
		void enableRandomBGMQueuePlay(bool flag);
		bool isRandomBGMQueuePlayEnabled() const;
        
        template<typename T>
        SoraConnection subscribeToPlaybackEvent(const T& func);
		
	private:
		inline void _clearBGMQueue();
		inline void _playBGM(SoraMusicFile* musicFile, uint32 newBGMId);
		
		typedef std::vector<SoraMusicFile*> BGM_QUEUE;
		BGM_QUEUE mBGMQueue;
		
		int32 mPrevBGMId;
		int32 mCurrBGMId;
		
		float bgmVolume;
		float bgmPitch;
		float bgmPan;
		
		float mCurrFadeInTime;
		float mCurrFadeOutTime;
		float mFadeInTime;
		float mFadeOutTime;
		
		bool mStopAtEnd;
		bool mRandomBGMQueuePlay;
		bool mPaused;
		
		class BGSInfo: public SoraEventHandler {
		public:
			BGSInfo() {}
			~BGSInfo() {
				if(bgsFile)
					delete bgsFile;
			}
			
			std::wstring path;
			
			uint32 bgsid;
			SoraMusicFile* bgsFile;
			
			uint32 currLoopTimes;
			uint32 loopTimes;
			
			float volumeScale;
			float bgmVolumeScale;
			
			bool bPlayFinished;
			
			BGSInfo(uint32 id, uint32 lt, float vol, float bgmvol, SoraMusicFile* file): 
				bgsid(id), loopTimes(lt), volumeScale(vol), bgmVolumeScale(bgmvol), currLoopTimes(0), bgsFile(file) {
					if(loopTimes != -1) {
						registerEventFunc(this, &BGSInfo::onPlaybackEvent);
						if(bgsFile) {
							bgsFile->registerEventHandler(this);
							bgsFile->enableEventPublish(true);
						}
					}
			}
			
			void onPlaybackEvent(SoraPlaybackEvent* event);
		};
		typedef std::map<uint32, BGSInfo*> BGS_MAP;
		BGS_MAP mBGSounds;
        
        typedef SoraSignal<void(SoraPlaybackEvent*)> PlaybackSignal;
        PlaybackSignal mPlaybackSignal;
	};
	
} // namespace sora

#endif // SORA_BGM_MANAGER_H_