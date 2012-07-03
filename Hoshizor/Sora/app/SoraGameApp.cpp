//
//  SoraGameApp.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGameApp.h"
#include "SoraCore.h"
#include "SoraGameState.h"
#include "SoraInputQueue.h"
#include "SoraFileSystem.h"

#ifdef OS_IOS
#include "SoraiOSGLRenderer/SoraiOSInitializer.h"
#endif

namespace sora {
    
    SoraGameApp::GameAppWindow::GameAppWindow(const SoraGameAppDef& def, SoraFSMManager& manager):
    mDef(def),
    mFSMManager(manager) {
        
    }
    
    bool SoraGameApp::GameAppWindow::updateFunc() {
        float dt = SoraCore::Instance()->getDelta();
        if(mDef.Window)
            mDef.Window->updateFunc();
        mFSMManager.onUpdate(dt);
        return false;
    }
    
    bool SoraGameApp::GameAppWindow::renderFunc() {
        if(mDef.Window)
            mDef.Window->renderFunc();
        mFSMManager.onRender();
        return false;
    }
    
    void SoraGameApp::GameAppWindow::init() {
        if(mDef.Window)
            mDef.Window->init();
    }
    
#ifndef OS_IOS
    
    int32 SoraGameApp::GameAppWindow::getWindowWidth() { 
        if(mDef.Window)
            return mDef.Window->getWindowWidth();
        return mDef.WindowWidth;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowHeight() { 
        if(mDef.Window)
            return mDef.Window->getWindowHeight();
        return mDef.WindowHeight;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowPosX() { 
        if(mDef.Window)
            return mDef.Window->getWindowPosX();
        return mDef.WindowPosX;
    }
    
    int32 SoraGameApp::GameAppWindow::getWindowPosY() { 
        if(mDef.Window)
            return mDef.Window->getWindowPosY();
        return mDef.WindowPosY;
    }
    
    StringType SoraGameApp::GameAppWindow::getWindowName() { 
        if(mDef.Window)
            return mDef.Window->getWindowName();
        return mDef.WindowTitle;
    }
    
    SoraString SoraGameApp::GameAppWindow::getWindowId() { 
        if(mDef.Window)
            return mDef.Window->getWindowId();
        return mDef.WindowId; 
    }
    
    bool SoraGameApp::GameAppWindow::isWindowed() { 
        if(mDef.Window)
            return mDef.Window->isWindowed();
        return mDef.WindowMode; 
    }
    
    bool SoraGameApp::GameAppWindow::hideMouse() { 
        if(mDef.Window)
            return mDef.Window->hideMouse();
        return mDef.HideMouse;
    }
    
#else
    
    void SoraGameApp::GameAppWindow::applicationWillResignActive() {
        if(mDef.Window)
            static_cast<SoraiOSMainWindow*>(mDef.Window)->applicationWillResignActive();
    }
    
    void SoraGameApp::GameAppWindow::applicationDidBecomeActive() {
        if(mDef.Window)
            static_cast<SoraiOSMainWindow*>(mDef.Window)->applicationDidBecomeActive();
    }
    
    void SoraGameApp::GameAppWindow::didReceiveMemoryWarning() {
        if(mDef.Window)
            static_cast<SoraiOSMainWindow*>(mDef.Window)->didReceiveMemoryWarning();
    }
    
    void SoraGameApp::GameAppWindow::didChangeStatusBarOrientation(iOSOrientation newOrientation, iOSOrientation oldOrientation) {
        if(mDef.Window)
            static_cast<SoraiOSMainWindow*>(mDef.Window)->didChangeStatusBarOrientation(newOrientation, oldOrientation);
    }
     
#endif // SoraiOSInitializer::Instance() 
    
    SoraGameApp::SoraGameApp(const SoraGameAppDef& def):
    mWindow(0),
    mAppDef(def) {
        mWindow = new SoraGameApp::GameAppWindow(def, mFSMManager);
        
#ifdef OS_IOS
        if(def.Window &&
           ! dynamic_cast<SoraiOSMainWindow*>(def.Window))
            THROW_SORA_EXCEPTION(SystemException, "iOS must use SoraiOSMainWindow");
#endif
    }
    
    void SoraGameApp::run(const std::string& initState) {
        GameStatePtr state = getState(initState);
        if(state) {
#ifndef OS_IOS
            // first create main window
            SoraCore::Instance()->createWindow(mWindow);
            
            // then init the first state
            mFSMManager.switchToState(initState);
            SoraKeyPoll::AddInputListener(static_cast<SoraGameState*>(state));
            
            SoraFileSystem* resourceFinder = SoraCore::Ptr->getFileSystem();
            for(int i=0; i<mAppDef.ResourceScripts.size(); ++i) {
                resourceFinder->loadResourceScript(mAppDef.ResourceScripts[i]);
            }
            
            // app starts!
            SoraCore::Instance()->start();
#else
            SoraiOSInitializer::Instance()->SoraiOSStart(static_cast<SoraiOSMainWindow*>(mWindow));
#endif
        } else
            THROW_SORA_EXCEPTION(SystemException, "Cannot find initial state");
    }
    
    SoraGameApp& SoraGameApp::addState(GameStatePtr state, const std::string& tag) {
        state->setGameApp(this);
        mFSMManager.defState(state, tag);
        
        return *this;
    }
    
    GameStatePtr SoraGameApp::getState(const std::string& tag) const {
        GameStatePtr state = static_cast<SoraGameState*>(mFSMManager[tag]);
        return state;
    }
    
    SoraWindow* SoraGameApp::getWindow() const {
        return mWindow;
    }
    
    void SoraGameApp::setState(const std::string& tag) {
        SoraFSMState* state = mFSMManager.getCurrentState();
        if(state)
            SoraKeyPoll::DelInputListener(static_cast<SoraGameState*>(state));
        
        mFSMManager.switchToState(tag);
        
        state = mFSMManager.getCurrentState();
        if(state)
            SoraKeyPoll::AddInputListener(static_cast<SoraGameState*>(state));
    }
    
    SoraGameApp& SoraGameApp::defStateTrans(const std::string& state1, const SoraFSMManager::EventType& type, const std::string& state2) {
        mFSMManager.defTrans(state1, type, state2);
        return *this;
    }
    
    void SoraGameApp::postEvent(const SoraFSMManager::EventType& evt) {
        SoraFSMState* state = mFSMManager.getCurrentState();
        if(state)
            SoraKeyPoll::DelInputListener(static_cast<SoraGameState*>(state));
        
        mFSMManager.postEvent(evt);
        
        state = mFSMManager.getCurrentState();
        if(state)
            SoraKeyPoll::AddInputListener(static_cast<SoraGameState*>(state));
    }
    
    void SoraGameApp::BeginScene(uint32 c, SoraTargetHandle h, bool clear) {
        SoraCore::Instance()->beginScene(c, h, clear);
    }
    
    void SoraGameApp::EndScene() {
        SoraCore::Instance()->endScene();
    }
    
    int32 SoraGameApp::getWindowWidth() const {
        if(mWindow) {
            return mWindow->getWindowWidth();
        }
        return 0;
    }
    
    int32 SoraGameApp::getWindowHeight() const {
        if(mWindow) {
            return mWindow->getWindowHeight();
        }
        return 0;
    }
    
} // namespace sora