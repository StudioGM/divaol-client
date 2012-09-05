//
//  SoraFMODSoundFile.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_FILE_H_
#define SORA_FMOD_SOUND_FILE_H_

#include "SoraSoundFile.h"
#include "SoraFMODSystem.h"

namespace sora {
    
    class SoraFMODMusicFile: public SoraMusicFile {
    public:
        SoraFMODMusicFile(bool bStream);
        SoraFMODMusicFile(const StringType& sFilePath, bool bStream);
        ~SoraFMODMusicFile();
		
		int32 readFile(const StringType& sFilePath);
		int32 readFileMem(void* ptr, uint32 size);
		void closeFile();
        
		void play();
		void stop();
		void pause();
		void resume();
        
		// 0 - 1
		void setVolume(float vol);
		// -1.0 - 1.0
		void setPan(float pan);
		// 0.5 - 2.0
		void setPitch(float pitch);
        
		float getPan() const;
		float getPitch() const;
		float getVolume() const;
        
		void setPosition(float x, float y, float z);
		void getPosition(float& x, float& y, float& z);
        
        void setMusicPosition(int32 pos);
        int32 getMusicPosition() const;
		
		int32 getLength() const;
		
		void setRepeat(bool flag);
		bool isRepeat() const;
		bool isPlaying() const;
        
    private:
        FMOD::System* pSystem;
        FMOD::Sound* pSound;
        FMOD::Channel* pChannel;
        
        bool bIsStream;
        void *pSoundData;
		
		float volume;
		float pitch;
		float pan;
    };
    
    class SoraFMODSoundEffectFile : public SoraSoundEffectFile {
    public:
        SoraFMODSoundEffectFile();
        SoraFMODSoundEffectFile(const StringType& path);
        ~SoraFMODSoundEffectFile();
		
		int32 readFile(const StringType& sFilePath);
		int32 readFileMem(void* ptr, uint32 size);
		void closeFile();
        
		void play();
		void stop();
        
        void setPosition(float x, float y, float z);
        void getPosition(float& x, float& y, float& z);
        
		// 0 - 1
		void setVolume(float vol);
		// -1.0 - 1.0
		void setPan(float pan);
		// 0.5 - 2.0
		void setPitch(float pitch);
        
		float getPan() const;
		float getPitch() const;
		float getVolume() const;
        
        void setRepeat(bool flag);
		bool isRepeat() const;
		bool isPlaying() const;
        
    private:
        FMOD::System* pSystem;
        
        FMOD::Sound* pSound;
        FMOD::Channel* pChannel;
		
		float volume;
		float pitch;
		float pan;
        
        void *pSoundData;
    };
    
} // namespace sora

#endif