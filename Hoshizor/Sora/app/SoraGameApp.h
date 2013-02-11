//
//  SoraGameApp.h
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGameApp_h
#define Sora_SoraGameApp_h

#include "SoraPlatform.h"
#include "SoraFSM.h"
#include "SoraGameAppDef.h"
#include "SoraString.h"
#include "SoraWindowInfo.h"
#include "SoraAutoPtr.h"

#ifdef OS_IOS
#include "SoraiOSGLRenderer/SoraiOSMainWindow.h"
#endif

namespace sora {
    
    /**
     * A high-level game app control class that works above SoraWindowInfoBase
     * States can be added and find by a tag
     **/
    
    class SoraGameState;
    typedef SoraGameState* GameStatePtr;
    
    class SORA_API SoraGameApp {
    public:
        SoraGameApp(const SoraGameAppDef& def);
        
        void run(const std::string& initState);
        
        SoraGameApp& addState(GameStatePtr state, const std::string& tag);
        GameStatePtr getState(const std::string& tag) const;
        void setState(const std::string& tag);
        
        SoraGameApp& defStateTrans(const std::string& state1, const SoraFSMManager::EventType& evt, const std::string& state2);
        void postEvent(const SoraFSMManager::EventType& evt);
        
        SoraWindow* getWindow() const;
    
        static void BeginScene(uint32 c=0xFF000000, SoraTargetHandle h=0, bool clear=true);
        static void EndScene();
        
        int32 getWindowWidth() const;
        int32 getWindowHeight() const;
        
    private:
#ifndef OS_IOS
        class GameAppWindow: public SoraWindow {
        public:
            GameAppWindow(const SoraGameAppDef& def, SoraFSMManager& manager);
            
            bool updateFunc();
            bool renderFunc();
            void init();
            
            int32 getWindowWidth();
            int32 getWindowHeight();
            
            int32 getWindowPosX();
            int32 getWindowPosY();
            
            StringType getWindowName();
            SoraString getWindowId();

			const char* getIcon();
			const char* getCursor();
            
            bool isWindowed();
            bool hideMouse();
            
        private:
            const SoraGameAppDef& mDef;
            SoraFSMManager& mFSMManager;
        };
#else
        class GameAppWindow: public SoraiOSMainWindow {
        public:
            GameAppWindow(const SoraGameAppDef& def, SoraFSMManager& manager);
            
            bool updateFunc();
            bool renderFunc();
            void init();
            
            void applicationWillResignActive();
            void applicationDidBecomeActive();
        
            void didReceiveMemoryWarning();
            void didChangeStatusBarOrientation(iOSOrientation newOrientation, iOSOrientation oldOrientation);
            
        private:
            const SoraGameAppDef& mDef;
            SoraFSMManager& mFSMManager;
        };
        
#endif
        
        SoraWindow* mWindow;
        SoraGameAppDef mAppDef;
        SoraFSMManager mFSMManager;
    };
    
} // namespace sora


#endif
