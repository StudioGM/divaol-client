#ifndef SORA_WINDOW_INFO_H
#define SORA_WINDOW_INFO_H

#include "SoraPlatform.h"
#include "SoraEvent.h"

namespace sora {

	class SORA_API SoraWindowInfoBase: public SoraEventHandler {
	public:
        SoraWindowInfoBase(): mActive(true) {}
        
		virtual int32 getWindowWidth() = 0;
		virtual int32 getWindowHeight() = 0;
		
		virtual int32 getWindowPosX() = 0;
		virtual int32 getWindowPosY() = 0;
		
		virtual StringType getWindowName() = 0;
		virtual SoraString getWindowId() = 0;
		
		virtual void init() = 0;
		
		virtual bool isWindowed() = 0;
		virtual bool hideMouse() = 0;

		virtual bool renderFunc() = 0;
		virtual bool updateFunc() = 0;

		virtual const char* getIcon() { return NULL; }
		virtual const char* getCursor() { return NULL; }

		virtual void pauseFunc() {}
        
        virtual void reinit() {}
        
        bool isActive() const { return mActive; }
        
        virtual bool isConsoleApp() { return false; }
        
    private:
        friend class SoraCore;
        void setActive(bool flag) { mActive = flag; }
        
        bool mActive;
	};
	
    typedef SoraWindowInfoBase SoraWindow;
    
} // namespace sora
	

#endif