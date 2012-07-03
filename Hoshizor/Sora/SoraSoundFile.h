#ifndef SORA_SOUND_FILE_H
#define SORA_SOUND_FILE_H

#include "SoraPlatform.h"
#include "SoraEvent.h"
#include "SoraConstantStrings.h"
#include "SoraPlaybackEvent.h"
#include "SoraString.h"
#include "io/SoraFileBase.h"

namespace sora {
	
	/*
	 virtual sound file base classes
	 */
    
	class SORA_API SoraMusicFile: public SoraFileBase, public SoraPlaybackEventHandler {
	public:
		virtual ~SoraMusicFile() {}
		
		virtual int32 readFile(const StringType& sFilePath) = 0;
		virtual int32 readFileMem(void* ptr, uint32 size) = 0;
		virtual void closeFile() = 0;

		virtual void play() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;

		// 0 - 1
		virtual void setVolume(float vol) = 0;
		// -1.0 - 1.0
		virtual void setPan(float pan) = 0;
		// 0.5 - 2.0
		virtual void setPitch(float pitch) = 0;

		virtual float getPan() const = 0;
		virtual float getPitch() const = 0;
		virtual float getVolume() const = 0;

		virtual void setPosition(float x, float y, float z) = 0;
		virtual void getPosition(float& x, float& y, float& z) = 0;
        
        virtual void setMusicPosition(int32 pos) = 0;
        virtual int32 getMusicPosition() const = 0;
		
		virtual int32 getLength() const = 0;
		
		virtual void setRepeat(bool flag) = 0;
		virtual bool isRepeat() const = 0;
		virtual bool isPlaying() const = 0;
	};

	class SORA_API SoraSoundEffectFile: public SoraFileBase, public SoraPlaybackEventHandler {
	public:
		virtual ~SoraSoundEffectFile() {}
		
		virtual int32 readFile(const StringType& sFilePath) = 0;
		virtual int32 readFileMem(void* ptr, uint32 size) = 0;
		virtual void closeFile() = 0;

		virtual void play() = 0;
		virtual void stop() = 0;
        
        virtual void setPosition(float x, float y, float z) = 0;
        virtual void getPosition(float& x, float& y, float& z) = 0;

		// 0 - 1
		virtual void setVolume(float vol) = 0;
		// -1.0 - 1.0
		virtual void setPan(float pan) = 0;
		// 0.5 - 2.0
		virtual void setPitch(float pitch) = 0;

		virtual float getPan() const = 0;
		virtual float getPitch() const = 0;
		virtual float getVolume() const = 0;
        
        virtual void setRepeat(bool flag) = 0;
		virtual bool isRepeat() const = 0;
		virtual bool isPlaying() const = 0;
	};


} // namespace sora

#endif