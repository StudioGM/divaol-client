#include "SoraCore.h"

#include "SoraTextureMap.h"
#include "SoraFileUtility.h"
#include "SoraEventManager.h"
#include "SoraConstantStrings.h"
#include "SoraInputQueue.h"
#include "SoraModifierAdapter.h"
#include "SoraFullscreenBufferHandler.h"
#include "SoraSystemEvents.h"
#include "SoraPlaybackEvent.h"
#include "SoraTimerEvent.h"
#include "SoraKeyInfo.h"
#include "SoraFileChangeEvent.h"
#include "SoraAutoUpdate.h"
#include "SoraFrameListener.h"

#include "SoraRenderSystem.h"
#include "SoraResourceManager.h"
#include "SoraInput.h"
#include "SoraMiscTool.h"
#include "SoraPluginManager.h"
#include "SoraTimer.h"
#include "SoraFontManager.h"
#include "SoraSoundSystem.h"
#include "SoraFileSystem.h"
#include "SoraShader.h"
#include "SoraFrameListener.h"
#include "SoraEnvValues.h"
#include "SoraResourceFile.h"
#include "SoraDelegate.h"
#include "SoraInputListeners.h"
#include "SoraHotkey.h"
#include "SoraLogger.h"
#include "SoraWindowInfo.h"

#include "physics/SoraPhysicWorld.h"

#include "Defaults/SoraDefaultMiscTool.h"
#include "Defaults/SoraDefaultTimer.h"

#include "event/SoraEventFactory.h"
#include "event/SoraEventWorld.h"

#include "Defaults/SoraTimer_OSX.h"
#include "Defaults/SoraTimer_Win32.h"
#include "Defaults/SoraTimer_PSP.h"
#include "Defaults/SoraMiscTool_Win32.h"
#include "Defaults/SoraMiscTool_OSX.h"
#include "Defaults/SoraMiscTool_iOS.h"

#include "SoraLogger.h"
#include "SoraString.h"

#include "Debug/SoraAutoProfile.h"

#include "helpers/SoraInputSimulator.h"
#include "helpers/SoraZSorter.h"
#include "helpers/SoraBooter.h"
#include "helpers/SoraMenuBar.h"

#include "message/SoraMessageEvent.h"

#include "factory/SoraSpriteFactory.h"

#include "timer/SoraSimpleTimerManager.h"

#include "cmd/SoraConsole.h"

#include "io/SoraFolderResourceManager.h"
#include "io/zip/SoraZipResourceManager.h"

#include "SoraMemoryUsage.h"
#include "SoraMath.h"
#include "SoraCamera.h"
#include "SoraSoundSystemThread.h"

#ifdef OS_IOS
#include "SoraiOSDeviceHelper.h"
#include "SoraiOSGLRenderer/SoraiOSWrapper.h"
#endif

#include "SoraPath.h"
#include "SoraCpuInfo.h"
#include "SoraCamera.h"

#include <stack>

extern "C" {
#include "Random/SFMT.h"
}

namespace sora {
    
    SoraCore* SoraCore::mInstance = NULL;
    SoraCore* SoraCore::Ptr = 0;
    
    int SoraCore::iRandomSeed = rand();
    bool SoraCore::mIs3DRendering = true;
    SoraVector3 SoraCore::mFarPoint = SoraVector3();
    
    struct ClippingData {
        int32 x;
        int32 y;
        int32 w;
        int32 h;
        
        inline void set(int32 _x, int32 _y, int32 _w, int32 _h) {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
        }
    };
    
    namespace {
        static std::stack<SoraMatrix4> g_TransformStack;
        static std::stack<ClippingData> g_ClippingStack;
        
        static SoraMatrix4 g_CurrentTransform;
        static ClippingData g_CurrentClipping;
        
        SoraKeyPoll* g_keypoll;
    }
    
    SoraCore* SoraCore::Instance() {
        if(!mInstance) {
            mInstance = new SoraCore;
            Ptr = mInstance;
        }
        return mInstance;
    }
	
	SoraCore::SoraCore() {
		bMainWindowSet = false;
		bMessageBoxErrorPost = false;
		bInitialized = false;
		bFrameSync = false;
		bHasInput = false;
		bZBufferSort = false;
        bMainScene = false;
		bPaused = false;
		bPauseRender = false;
		bPauseSound = false;
		bPluginDetection = false;
        bEnableScreenBuffer = false;
        bScreenBufferAttached = false;
        bSeperateSoundSystemThread = false;
        bMultithreadedRendering = false;
        mIs3DRendering = false;

		pInput = NULL;
		pFontManager = NULL;
		pRenderSystem = NULL;
		pSoundSystem = NULL;
        pPhysicWorld = NULL;
        
		mMainWindow = NULL;
		
		mPrevShaderContext = NULL;
        mScreenBuffer = NULL;
        mSystemFont = 0;
        mCamera = 0;
        
        log_notice("Initializing base systems...");
        
		_initializeTimer();
		_initializeMiscTool();
		
		SoraEventManager::Instance();
		SoraInternalLogger::Instance();
        
        pPluginManager = new SoraPluginManager;
		pFileSystem = new SoraFileSystem;
		pFileSystem->attachResourceManager(new SoraFolderResourceManager);
#ifdef SORA_ZIP_PACK_SUPPORT
        pFileSystem->attachResourceManager(new SoraZipResourceManager);
#endif
        
		SetRandomSeed(rand());
        
        _registerEventTypes();
        _regGlobalProducts();
        
        new SoraSimpleTimerManager;
        
        log_mssg(getSoraVersion().get());
        log_mssg(getOSVersion().get());
        log_mssg(vamssg("CPU Speed: %d mhz", getCPUSpeed()));
        log_mssg(vamssg("Memory Size: %d kb", getSystemMemorySize() / 1024));
        
        SoraCpuInfo cpuinfo;
        log_mssg(vamssg("CPU: %s, %s, Cores: %d Threads: %d", 
                        cpuinfo.getCPUString().c_str(),
                        cpuinfo.getCPUBrandString().c_str(),
                        cpuinfo.getNumCores(),
                        cpuinfo.getNumHWThreads()));
	}
    
    void SoraCore::_regGlobalProducts() {
    }
    
    void SoraCore::init(const Feature& param) {
        if(param.LoadPlugins) {
            enableCoreFeature(FeatureLoadPlugin);
        }
        if(param.MessageBoxErrorPost) {
            enableCoreFeature(FeatureMessageBoxErrorPost);
        }
        if(param.RenderToBuffer) {
            enableCoreFeature(FeatureRenderToBuffer);
        }
        if(param.DebugRender) {
            enableCoreFeature(FeatureDebugRendering);
        }
        if(param.SeperateSoundSystemThread) {
            enableCoreFeature(FeatureSeperateSoundSystem);
        }
    }
    
    void SoraCore::enableCoreFeature(CoreFeatures feature) {
        switch(feature) {
            case FeatureLoadPlugin:
                SoraBooter::loadExPlugins(L"plugins");
                log_notice("Loading plugins...");
                break;
            case FeatureRenderToBuffer:
                bEnableScreenBuffer = true;
                break;
            case FeatureMessageBoxErrorPost:
                bMessageBoxErrorPost = true;
                break;
            case FeatureSeperateSoundSystem:
                bSeperateSoundSystemThread = true;
                break;
            case FeatureDebugRendering:
                bDebugRender = true;
                break;
        }
    }
    
    void SoraCore::disableCoreFeature(CoreFeatures feature) {
        switch(feature) {
            case FeatureRenderToBuffer:
                bEnableScreenBuffer = false;
                break;
            case FeatureMessageBoxErrorPost:
                bMessageBoxErrorPost = false;
                break;
            case FeatureSeperateSoundSystem:
                bSeperateSoundSystemThread = false;
                break;
            case FeatureDebugRendering:
                bDebugRender = false;
                break;
        }
    }

	void SoraCore::_initializeTimer() {
#if defined(OS_OSX) || defined(OS_IOS)
		pTimer = new SoraOSXTimer; return;
#elif defined(OS_WIN32)
		pTimer = new SoraWin32Timer; return;
#elif defined(OS_PSP)
		pTimer = new SoraPSPTimer; return;
#endif
		pTimer = new SoraDefaultTimer;
	}

	void SoraCore::_initializeMiscTool() {
#ifdef OS_WIN32
		pMiscTool = new SoraMiscToolWin32;  return;
#elif defined(OS_OSX)
		pMiscTool = new SoraMiscToolOSX; return;
#elif defined(OS_IOS)
		pMiscTool = new SoraMiscTooliOS; return;
#endif
		pMiscTool = new SoraDefaultMiscTool;
	}

	void SoraCore::start() { 
        try {
            // no main window created
            if(!bMainWindowSet) {
                THROW_SORA_EXCEPTION(SystemException, "No main window created");
                shutDown();
            }
            if(!bInitialized) {
                THROW_SORA_EXCEPTION(SystemException, "Sora not initialized");
                shutDown();
            }
        } catch(const SoraException& exp) {
            messageBox(exp.what(), "fatal error", MB_OK);
            shutDown();
        }
        
        _checkCoreComponents();
        
        // registering global keypool
        g_keypoll = new SoraKeyPoll;
        
        // startup consoles
        sora::SoraConsole::Instance();
        
        try {
        // create render target for debug renderer
#ifdef DEBUG
//            DEBUG_RENDERER->createTarget();
#endif
            
            if(bEnableScreenBuffer) {
                mScreenBuffer = pRenderSystem->createTarget(iScreenWidth, iScreenHeight);
                if(!mScreenBuffer) {
                    bEnableScreenBuffer = false;
                    THROW_SORA_EXCEPTION(SystemException, "Error creating fullscreen buffer, fullscreen posteffect maybe disabled");
                }
                SoraFullscreenBufferHandler::Instance();
            }
            
        } catch(const SoraException& exp) {
            _postError(exp.what());
        }
                
        if(bSeperateSoundSystemThread && pSoundSystem != 0) {
            SoraSoundSystemThread::Instance()->setSoundSystem(pSoundSystem);
            SoraSoundSystemThread::Instance()->start();
        }
        
        pRenderSystem->fillDeviceCaps(mGraphicDeviceCaps);
        log_notice(vamssg("Graphic Device Caps:\n\t\tMaxTextureSize=(%d,%d)\n\t\tMaxIndices=%d, MaxVertices=%d\n\t\tMaxCubeMapSize=%d\n\t\tMaxPixelTextureUnits=%d, MaxVertexTextureUnit=%d", 
                          mGraphicDeviceCaps.max_texture_width,
                          mGraphicDeviceCaps.max_texture_height,
                          mGraphicDeviceCaps.max_indices, 
                          mGraphicDeviceCaps.max_vertices,
                          mGraphicDeviceCaps.max_texture_cube_map_size,
                          mGraphicDeviceCaps.max_pixel_texture_units,
                          mGraphicDeviceCaps.max_vertex_texture_units));
        pRenderSystem->switchTo2D();
        pRenderSystem->start(pTimer);
	}

	void SoraCore::_frameListenerStart() {
		FRAME_LISTENER_CONT::iterator p = frameListeners.begin();
		while( p != frameListeners.end() ) {
			(*p)->onFrameStart();
			++p;
		}
	}

	void SoraCore::_frameListenerEnd() {
		FRAME_LISTENER_CONT::iterator p = frameListeners.begin();
		while( p != frameListeners.end() ) {
			(*p)->onFrameEnd();
			++p;
		}
	}

	void SoraCore::addFrameListener(SoraFrameListener* listener) {
		frameListeners.push_back(listener);
	}

	void SoraCore::delFrameListener(SoraFrameListener* listener) {
		frameListeners.remove(listener);
	}

	void SoraCore::update() {
		sora_assert(bInitialized == true);
        float dt = bFrameSync?1.f:pTimer->getDelta() * SoraTimer::TimeScale;
#ifdef PROFILE_CORE_UPDATE
		SORA_PROFILE("CoreUpdate");
#endif

     //   try {
            if(!bPaused && !bPauseSound) {
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UpdateSoundSystem");
#endif
                    if(!bSeperateSoundSystemThread) {
                        if(pSoundSystem) 
                            pSoundSystem->update();
                    } else {
                        SoraSoundSystemThread::Instance()->onUpdate(dt);
                    }
                }
            }
            
            
            if(!bPaused) {
                SoraInputSimulator::clear();
                
                pRenderSystem->beginFrame();
                
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UpdateMainWindow");
#endif
                    SoraSimpleTimerManager::Instance()->update(dt);
                    
                    mMainWindow->updateFunc();
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UpdateEventManager");
#endif
                    SoraEventManager::Instance()->update(dt);
                    SoraEventWorld::defaultWorld().update(dt);
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("FrameListenerStart");
#endif
                    _frameListenerStart();
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UpdateAutoUpdate");
#endif
                    SoraAutoUpdate::UpdateList(dt);
                }
                
                if(pPhysicWorld) {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UpdatePhysicalWorld");
#endif
                        pPhysicWorld->update(dt);
                }
                
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("UPDATE_RENDERSYSTEM");
#endif
                    pRenderSystem->update();
                }
            } else 
                mMainWindow->pauseFunc();
            
            if(!bPaused && !bPauseRender) {
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("RenderMainWindow");
#endif
                    mMainWindow->renderFunc();
                }
            }
            
            if(!bPaused) {
                {
#ifdef PROFILE_CORE_UPDATE
                    SORA_PROFILE("FRAMELISTENER_END");
#endif
                    _frameListenerEnd();
                }
                
                if(!bPauseRender) {                    
                    _updateEnd();
                }
                
                SoraTimer::FrameCount++;
                
                pRenderSystem->endFrame();
            }
     /*   } catch (const SoraException& exp) {
            messageBox(exp.what(), "A Error Happened :(", MB_OK | MB_ICONERROR);
        }*/
        
	}
    
    void SoraCore::_updateEnd() {
        if(bMainScene) {
            bMainScene = false;
            pRenderSystem->endScene();
            
            if(mScreenBuffer && bEnableScreenBuffer) {
                pRenderSystem->beginScene(0, 0, true);
                SoraFullscreenBufferHandler::Instance()->onBufferRender(getTargetTexture(mScreenBuffer));
                pRenderSystem->endScene();
                bScreenBufferAttached = false;
            }
        }
    }

	void SoraCore::_checkCoreComponents() {
		if(!pInput) {
			bHasInput = false;
			log_warning("SoraCore::CheckCoreComponents: No Input provided");
		}

		if(!pPluginManager) {
			pPluginManager = new SoraPluginManager;
			log_warning("SoraCore::CheckCoreComponents: No PluginManager registered, using default plugin manager");
		}

		if(!pMiscTool) {
			pMiscTool = new SoraDefaultMiscTool;
			log_warning("SoraCore::CheckCoreComponents: No MiscTool registered, using default tool");
		}

		if(!pFontManager) {
			_postError("SoraCore::CheckCoreComponents: no FontManager available");
			log_warning("No FontManager available, font related API disabled");
		}
		
		if(!pSoundSystem) {
			_postError("SoraCore::CheckCoreComponents: no SoundSystem available");
			log_warning("No SoundSystem available, sound related API disabled");
		}

		// no render system available, fatal error
		// cause currently there's no cross-platform render system HoshiNoSora implemented
		if(!pRenderSystem) {
			THROW_SORA_EXCEPTION(SystemException, "no render system available");
		}

		bInitialized = true;
	}

	void SoraCore::postError(const SoraString& string) {
		_postError(string);
	}

	void SoraCore::_postError(const SoraString& string) {
		if(!bMessageBoxErrorPost)
			log_mssg(string, LOG_LEVEL_ERROR);
		else
			pMiscTool->messageBox(string, "Some Error Happened :(", MB_ICONERROR | MB_OK);
	}
    
    void SoraCore::_registerEventTypes() {
        SoraEventFactory* factory = SoraEventFactory::Instance();
        factory->registerEvent<SoraPlaybackEvent>("PlaybackEvent");
        factory->registerEvent<SoraFileChangeEvent>("FileChangeEvent");
        factory->registerEvent<SoraKeyEvent>("KeyEvent");

        factory->registerEvent<SoraTimerEvent>("TimerEvent");
        factory->registerEvent<SoraConsoleEvent>("ConsoleEvent");
        factory->registerEvent<SoraMenuBarClickEvent>("MenubarEvent");
        factory->registerEvent<SoraHotkeyEvent>("HotkeyEvent");
        factory->registerEvent<SoraMessageEvent>("MessageEvent");
    }

	SoraHandle SoraCore::getMainWindowHandle() const {
		return mWindowHandle;
	}

	SoraWindowInfoBase* SoraCore::getMainWindow() const {
		return mMainWindow;
	}

	bool SoraCore::isActive() {
		return pRenderSystem->isActive();
	}

	void SoraCore::shutDown() {        
        SoraInternalLogger::Instance()->writeToFile(SoraFileUtility::GetWrittablePath("log.txt").c_str());
        
		//SoraTextureMap::Instance()->Destroy();
        delete pFileSystem;

        if(mMainWindow)
            delete mMainWindow;
        
		if(bHasInput) delete pInput;
		if(pPluginManager) delete pPluginManager;
		if(pTimer) delete pTimer;
		if(pFontManager) delete pFontManager;
        if(pPhysicWorld) delete pPhysicWorld;
        
		if(bInitialized) {
			pRenderSystem->shutdown();
			delete pRenderSystem;
		}
		
		if(pSoundSystem) {
            if(bSeperateSoundSystemThread)
                SoraSoundSystemThread::Destroy();
            else
                pSoundSystem->shutdown();
			delete pSoundSystem;
		}
		
        if(g_keypoll) delete g_keypoll;
		if(pMiscTool) delete pMiscTool;
		// force exiting
		exit(0);
	}

	bool SoraCore::createWindow(SoraWindowInfoBase* info) {
        sora_assert(info);
        
        if(info->isConsoleApp()) {
            mMainWindow = info;
            bMainWindowSet = true;
            mMainWindow = info;
            mMainWindow->setActive(true);
            mMainWindow->init();
            return true;
        }
		if(mMainWindow == NULL) {
			iScreenWidth = info->getWindowWidth();
			iScreenHeight = info->getWindowHeight();
            
            SET_ENV_INT("ScreenWidth", iScreenWidth);
            SET_ENV_INT("ScreenHeight", iScreenHeight);
			
			SoraHandle result = pRenderSystem->createWindow(info);
			if(result) {
                log_mssg(getVideoInfo().get());
				mWindowHandle = result;

                if(pInput != NULL)
					pInput->setWindowHandle(result);
#ifdef OS_WIN32
				static_cast<SoraMiscToolWin32*>(pMiscTool)->setMainWindowHandle(result);
#endif
				bMainWindowSet = true;
				mMainWindow = info;
                mMainWindow->setActive(true);
                mMainWindow->init();

				log_notice(vamssg("Created MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mMainWindow->getWindowName().c_str()));
                
			} else {
				_postError("SoraCore::createWindow, createWindow failed");
				return false;
			}
		} else {
            mMainWindow->setActive(false);
			mMainWindow = info;
            if(mMainWindow->isActive())
                mMainWindow->init();
            else 
                mMainWindow->reinit();
            mMainWindow->setActive(true);
            
			log_notice(vamssg("Set MainWindow, Width=%d, Height=%d, Title=%s", iScreenWidth, iScreenHeight, mMainWindow->getWindowName().c_str()));
			
			if(info->getWindowWidth() != iScreenWidth || info->getWindowHeight() != iScreenHeight) {
				iScreenWidth = info->getWindowWidth();
				iScreenHeight = info->getWindowHeight();
				
				pRenderSystem->setWindowSize(iScreenWidth, iScreenHeight);
				pRenderSystem->setWindowTitle(s2ws(info->getWindowName()));
				pRenderSystem->setWindowPos(info->getWindowPosX(), info->getWindowPosY());
			}
		}
        
        SET_ENV_INT("ScreenWidth", iScreenWidth);
		SET_ENV_INT("ScreenHeight", iScreenHeight);
		
        g_CurrentClipping.set(0, 0, iScreenWidth, iScreenHeight);
        
        pRenderSystem->fillDeviceCaps(mGraphicDeviceCaps);
        
		return true;
	}

	void SoraCore::setWindowSize(int32 w, int32 h) {
		sora_assert(bInitialized == true);
		if(SoraConsole::Instance()->getWidth() == iScreenWidth-2) {
			SoraConsole::Instance()->setSize(w-2.f, SoraConsole::Instance()->getHeight()>h?h:SoraConsole::Instance()->getHeight());
		}
		
		iScreenWidth = w;
		iScreenHeight = h;
		pRenderSystem->setWindowSize(w, h);
	}

	void SoraCore::setWindowTitle(const StringType& title) {
		sora_assert(bInitialized == true);

		pRenderSystem->setWindowTitle((SoraWString)title);
	}

	void SoraCore::setWindowPos(int32 px, int32 py) {
		sora_assert(bInitialized == true);

		pRenderSystem->setWindowPos(px, py);
	}

	void SoraCore::setFullscreen(bool flag) {
		sora_assert(bInitialized == true);

		pRenderSystem->setFullscreen(flag);
	}

	bool SoraCore::isFullscreen() {
		sora_assert(bInitialized == true);

		return pRenderSystem->isFullscreen();
	}

	void SoraCore::registerRenderSystem(SoraRenderSystem* renderSystem) {
		if(pRenderSystem) {
			pRenderSystem->shutdown();
			delete pRenderSystem;
		}
		pRenderSystem = renderSystem;
		bInitialized = true;
        
        log_mssg("RenderSystem registered");
	}

	void SoraCore::registerInput(SoraInput* input) {
		if(bHasInput) delete pInput;
		pInput = input;
		bHasInput = true;
        
        SoraKeyPoll::setQueueInput(input);
        
        log_mssg("Input registered");
	}

	void SoraCore::registerResourceManager(SoraResourceManager* pResourceManager) {
		pFileSystem->attachResourceManager(pResourceManager);
	}

	void SoraCore::registerMiscTool(SoraMiscTool* miscTool) {
		if(pMiscTool) delete pMiscTool;
		pMiscTool = miscTool;
    }

	void SoraCore::registerPluginManager(SoraPluginManager* pluginManager) {
		if(pPluginManager) delete pPluginManager;
		pPluginManager = pluginManager;
	}

	void SoraCore::registerTimer(SoraTimer* timer) {
		if(pTimer) delete pTimer;
		pTimer = timer;
	}
	
	void SoraCore::registerSoundSystem(SoraSoundSystem* ss) {
		if(pSoundSystem) {
			pSoundSystem->shutdown();
			delete pSoundSystem;
		}
		pSoundSystem = ss;
		pSoundSystem->init();
        
        log_mssg("SoundSystem registered");
	}

	void SoraCore::registerFontManager(SoraFontManager* fontManager) {
		if(pFontManager) delete pFontManager;
		pFontManager = fontManager;
        
        log_mssg("FontManager registered");
	}
    
    void SoraCore::registerPhysicWorld(SoraPhysicWorld* physicWorld) {
        if(pPhysicWorld) delete pPhysicWorld;
        pPhysicWorld = physicWorld;
        
        log_mssg("PhysicalWorld registered");
    }
    
    SoraRenderSystem* SoraCore::getRenderSystem() const {
        return pRenderSystem;
    }
    
    SoraInput* SoraCore::getInput() const {
        return pInput;
    }
    
    SoraMiscTool* SoraCore::getMiscTool() const {
        return pMiscTool;
    }
    
    SoraPluginManager* SoraCore::getPluginManager() const {
        return pPluginManager;
    }
    
    SoraTimer* SoraCore::getTimer() const {
        return pTimer;
    }
    
    SoraFontManager* SoraCore::getFontManager() const {
        return pFontManager;
    }
    
    SoraSoundSystem* SoraCore::getSoundSystem() const {
        return pSoundSystem;
    }
    
    SoraPhysicWorld* SoraCore::getPhysicWorld() const {
        return pPhysicWorld;
    }
    
    SoraFileSystem* SoraCore::getFileSystem() const {
        return pFileSystem;
    }

	float SoraCore::getFPS() {
		return pTimer->getFPS();
	}

	float SoraCore::getDelta() {
		if(!bFrameSync)
			return pTimer->getDelta() * SoraTimer::TimeScale;
		return 1.f;
	}
    
    float SoraCore::getAbsoluteDelta() {
        return pTimer->getDelta();
    }

	uint64 SoraCore::GetRunningTime() {
        static SoraTimestamp g_starttime = SoraTimestamp();
        return g_starttime.elapsed();
	}

	uint64 SoraCore::GetCurrentSystemTime() {
		return SoraTimestamp::CurrentTime();
	}

	void SoraCore::setFPS(int32 fps) {
		SET_ENV_INT("Fps", fps);
		
		pTimer->setFPS(fps);
	}

	uint64 SoraCore::GetFrameCount() {
        return SoraTimer::FrameCount;
	}

	void SoraCore::SetTimeScale(float scale) {
		SET_ENV_FLOAT("TimeScale", scale);
        SoraTimer::TimeScale = scale;
	}

	float SoraCore::GetTimeScale() {
		return SoraTimer::TimeScale;
	}
    
    void SoraCore::setVerticalSync(bool flag) {
        if(!bInitialized) return;
        pRenderSystem->setVerticalSync(flag);
    }

	SoraResourceHandle SoraCore::loadResourcePack(const StringType& file) {
		return pFileSystem->loadResourcePack(file);
	}

	void SoraCore::detachResourcePack(SoraResourceHandle handle) {
		pFileSystem->detachResourcePack(handle);
	}

	void* SoraCore::getResourceFile(const StringType& sfile, uint32& size) {
		return pFileSystem->getResourceFile(sfile, size);
	}

	void SoraCore::freeResourceFile(void* p) {
		return pFileSystem->freeResourceFile(p);
	}

	void* SoraCore::readResourceFile(const StringType& sfile, uint32 pos, uint32 size) {
		return pFileSystem->readResourceFile(sfile, pos, size);
	}

	uint32 SoraCore::getResourceFileSize(const StringType& file) {
		return pFileSystem->getResourceFileSize(file);
	}
    
    SoraShaderContext* SoraCore::createShaderContext() {
        sora_assert(bInitialized == true);
        return pRenderSystem->createShaderContext();
    }
    
	void SoraCore::attachShaderContext(SoraShaderContext* context) {
        sora_assert(bInitialized == true);
        if(!bZBufferSort)
            pRenderSystem->attachShaderContext(context);
		
		mPrevShaderContext = context;
	}

	void SoraCore::detachShaderContext() {
        sora_assert(bInitialized == true);
        if(!bZBufferSort)
            pRenderSystem->detachShaderContext();
        
		mPrevShaderContext = NULL;
	}
    
	SoraTextureHandle SoraCore::createTexture(const StringType& sTexturePath, bool bCache, bool bMipmap)	{
        sora_assert(bInitialized == true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
#ifdef OS_IOS
        std::string realPath = sTexturePath;
        std::string tmpPath = realPath;
        size_t dotpos = tmpPath.rfind(".");
        bool isRetinaTexture = false;
        
        // ios resource auto get
        if(_IS_RETINA_DISPLAY()) {
            tmpPath.insert(dotpos, "@2x");
            if(SoraFileUtility::fileExists(SoraPath::resource() + tmpPath)) {
                realPath = tmpPath;
                isRetinaTexture = true;
            }
        } else if(_IS_IPAD()) {
            tmpPath.insert(dotpos, "@ipad");
            if(SoraFileUtility::fileExists(SoraPath::resource() + tmpPath)) {
                realPath = tmpPath;
            }
        }
        
        sora_assert(bInitialized==true);
		SoraTextureHandle tex;
		if((tex = SoraTextureMap::Instance()->get(SoraPath::resource() + realPath)) != 0) return tex;
		uint32 size;
		void* data = getResourceFile(realPath, size);
		if(data) {
			tex = (SoraTextureHandle)pRenderSystem->createTextureFromMem(data, size, bMipmap);
			if(tex) {
                ((SoraTexture*)tex)->mIsRetinaTexture = isRetinaTexture;
                
                if(bCache) {
                    SoraTextureMap::Instance()->add(realPath, tex);
                }
            }
            
			freeResourceFile(data);
			return tex;
		}
		return 0;
#else
		SoraTextureHandle tex;
		if((tex = SoraTextureMap::Instance()->get(sTexturePath)) != 0) return tex;
		uint32 size;
		void* data = getResourceFile(sTexturePath, size);
		if(data) {
			tex = (SoraTextureHandle)pRenderSystem->createTextureFromMem(data, size, bMipmap);
			if(bCache) {
				SoraTextureMap::Instance()->add(sTexturePath, tex);
			}

			freeResourceFile(data);
			return tex;
		}
		return 0;
#endif
	}

	SoraTextureHandle SoraCore::createTextureWH(int w, int h) {
        sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return (SoraTextureHandle)pRenderSystem->createTextureWH(w, h);
	}

	SoraTextureHandle SoraCore::createTextureFromRawData(uint32* data, int32 w, int32 h) {
        sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return (SoraTextureHandle)pRenderSystem->createTextureFromRawData(data, w, h);
	}

	SoraTextureHandle SoraCore::createTextureFromMem(void* data, uint32 size) {
		sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return (SoraTextureHandle)pRenderSystem->createTextureFromMem(data, size);
	}

	uint32* SoraCore::textureLock(SoraTextureHandle ht) {
		sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return pRenderSystem->textureLock((SoraTexture*)ht);
	}

	void SoraCore::textureUnlock(SoraTextureHandle h) {
        sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		pRenderSystem->textureUnlock((SoraTexture*)h);
	}
	
	void SoraCore::releaseTexture(SoraTextureHandle pTexture) {
		sora_assert(bInitialized==true);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
        if(!pTexture) return;
		SoraTextureMap::Instance()->del(pTexture);
        pRenderSystem->releaseTexture((SoraTexture*)pTexture);
	}

	SoraHandle	SoraCore::getTextureId(SoraTextureHandle tex) {
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			return ptex->mTextureID;
		return 0;
	}

	void SoraCore::clearTextureMap() {
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		SoraTextureMap::Instance()->clear();
	}

	SoraHandle SoraCore::getVideoDeviceHandle() {
		sora_assert(bInitialized==true);
		return pRenderSystem->getVideoDeviceHandle();
	}

	SoraSprite* SoraCore::createSprite(const StringType& sPath) {
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		sora_assert(bInitialized==true);
		SoraTextureHandle tex;
		if((tex = SoraTextureMap::Instance()->get(sPath)) != 0) {
			SoraSprite* pspr = new SoraSprite(tex);
			pspr->setName(sPath.uniqueId());
			return pspr;
		} else {
			tex = createTexture(sPath);
			if(!tex)
				return new SoraSprite(tex);
			else {
				SoraSprite* pspr = new SoraSprite(tex);
				pspr->setName(sPath.uniqueId());
				return pspr;
			}
		}
	}
	
	void SoraCore::renderQuad(SoraQuad& quad) {
		sora_assert(bInitialized==true);
        
		if(bZBufferSort) {
#ifdef SORA_ENABLE_MULTI_THREAD
            if(bMultithreadedRendering)
                MUTEX_LOCK(mRenderingLock);
#endif
			SoraZSorter::renderQuad(quad, mPrevShaderContext);
		} else 
			pRenderSystem->renderQuad(quad);
	}
	
	void SoraCore::renderWithVertices(SoraTextureHandle tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode) {
		sora_assert(bInitialized == true);
        
		if(bZBufferSort) {
#ifdef SORA_ENABLE_MULTI_THREAD
            if(bMultithreadedRendering)
                MUTEX_LOCK(mRenderingLock);
#endif
			SoraZSorter::renderWithVertices(tex, blendMode, vertices, vsize, mode, mPrevShaderContext);
		} else 	
			pRenderSystem->renderWithVertices((SoraTexture*)tex, blendMode, vertices, vsize, mode);
	}

	void SoraCore::renderLine(float x1, float y1, float x2, float y2, uint32 color, float w, float z) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
		pRenderSystem->renderLine(x1, y1, x2, y2, color, w, z);
	}
	
	void SoraCore::renderBox(float x1, float y1, float x2, float y2, uint32 color, float w, float z) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
		pRenderSystem->renderBox(x1, y1, x2, y2, color, w, z);
	}
    
    void SoraCore::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
		pRenderSystem->fillBox(x1, y1, x2, y2, color, z);
	}

	void SoraCore::setClipping(int32 x, int32 y, int32 w, int32 h) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
		pRenderSystem->setClipping(x, y, w, h);
        
        g_CurrentClipping.set(x, y, w, h);
	}

	void SoraCore::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
		pRenderSystem->setTransform(x, y, dx, dy, rot, hscale, vscale);
        
        g_CurrentTransform = pRenderSystem->getTransformMatrix();
	}
    
	void SoraCore::beginScene(uint32 c, SoraHandle t, bool clear) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
        if(mScreenBuffer && bEnableScreenBuffer) {
            if(t == 0) {
                bMainScene = true;
                bScreenBufferAttached = true;
                pRenderSystem->beginScene(0x00000000, mScreenBuffer, true);
            } else {
                bMainScene = false;
                bScreenBufferAttached = false;
                pRenderSystem->endScene();
                pRenderSystem->beginScene(c, t, clear);
            }
        } else {
            if(t == 0) {
                bMainScene = true;
            } else bMainScene = false;
            pRenderSystem->beginScene(c, t, clear);
        }
	}
    
	void SoraCore::endScene() {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		if(!bMainScene) {
            pRenderSystem->endScene();
            if(mScreenBuffer && bEnableScreenBuffer && bScreenBufferAttached) 
                pRenderSystem->beginScene(0, mScreenBuffer, false);
        }
	}
    
    void SoraCore::switchTo2D() {
        sora_assert(bInitialized = true);
        pRenderSystem->switchTo2D();
        
        mIs3DRendering = false;
    }
    
    void SoraCore::switchTo3D() {
        sora_assert(bInitialized = true);
        pRenderSystem->switchTo3D();
        
        if(mCamera)
            mCamera->apply();
        mIs3DRendering = true;
    }

	SoraTargetHandle SoraCore::createTarget(int width, int height, bool zbuffer) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return pRenderSystem->createTarget(width==0?iScreenWidth:width, height==0?iScreenHeight:height, zbuffer);
	}

	void SoraCore::freeTarget(SoraHandle t) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		pRenderSystem->freeTarget(t);
	}

	SoraHandle SoraCore::getTargetTexture(SoraHandle t) {
		sora_assert(bInitialized==true);
#ifdef SORA_ENABLE_MULTI_THREAD
        sora_assert(!bMultithreadedRendering);
#endif
        
		return pRenderSystem->getTargetTexture(t);
	}

	int32 SoraCore::getScreenHeight() {
#ifndef OS_IOS
		return iScreenHeight;
#else
        return sora::getScreenHeight();
#endif
	}
    
	int32 SoraCore::getScreenWidth() {
#ifndef OS_IOS
		return iScreenWidth;
#else
        return sora::getScreenWidth();
#endif
	}

	void SoraCore::getMousePos(float *x, float *y) {
		if(bHasInput) pInput->getMousePos(x, y);
	}

	float SoraCore::getMousePosX() {
		if(bHasInput) return pInput->getMousePosX();
		return 0.f;
	}

	float SoraCore::getMousePosY() {
		if(bHasInput) return pInput->getMousePosY();
		return 0.f;
	}

	void SoraCore::setMousePos(float x, float y) {
		if(bHasInput) pInput->setMousePos(x, y);
	}

	int	SoraCore::getMouseWheel() {
		if(bHasInput) return pInput->getMouseWheel();
		return 0;
	}

	bool SoraCore::isMouseOver() {
		if(bHasInput) return pInput->isMouseOver();
		return false;
	}

	bool SoraCore::keyDown(int32 key) {
		if(sora::SoraConsole::Instance()->isActive())
			return false;

		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState == SORA_INPUT_KEYDOWN;
		
		if(bHasInput) return pInput->keyDown(key);
		return false;
	}

	bool SoraCore::keyUp(int32 key) {
		if(sora::SoraConsole::Instance()->isActive())
			return false;

		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState == SORA_INPUT_KEYUP;
		
		if(bHasInput) return pInput->keyUp(key);
		return false;
	}
	
	void SoraCore::SimulateKey(int32 key, int32 state) {
		SoraInputSimulator::simulateKey(key, state);
	}

	int32 SoraCore::getKeyState(int32 key) {
		if(sora::SoraConsole::Instance()->isActive())
			return SORA_INPUT_KEYUP;

		int32 simulateState = SoraInputSimulator::getKeyState(key);
		if(simulateState != -1)
			return simulateState;
		
		if(bHasInput) return pInput->getKeyState(key);
		return SORA_INPUT_KEYUP;
	}

	char* SoraCore::getKeyName(int32 key) {
		if(bHasInput) return pInput->getKeyName(key);
		return 0;
	}

	bool SoraCore::getKeyEvent(SoraKeyEvent& ev) {
		if(bHasInput) return SoraKeyPoll::getQueueEvent(ev);
		return false;
	}

	bool SoraCore::joyKeyPressed(int32 key) {
		if(bHasInput) return pInput->joyKeyPressed(key);
		return false;
	}

	bool SoraCore::joyKeyDown(int32 key) {
		if(bHasInput) return pInput->joyKeyDown(key);
		return false;
	}

	bool SoraCore::joyKeyUp(int32 key) {
		if(bHasInput) return pInput->joyKeyUp(key);
		return false;
	}

	bool SoraCore::joyKeyState(int32 key, unsigned char state) {
		if(bHasInput) return pInput->joyKeyState(key, state);
		return false;
	}

	bool SoraCore::setJoyKey(int32 key) {
		if(bHasInput) return pInput->setJoyKey(key);
		return false;
	}

	bool SoraCore::hasJoy() {
		if(bHasInput) return pInput->hasJoy();
		return false;
	}
    
    int32 SoraCore::RegisterGlobalHotkey(const SoraHotkey& key, SoraEventHandler* handler) {
        return SoraKeyPoll::addGlobalHotKey(key, handler);
    }
    
    void SoraCore::SetGlobalHotkey(int32 hid, const SoraHotkey& key) {
        SoraKeyPoll::setGlobalHotkey(hid, key);
    }
    
    void SoraCore::UnregisterGlobalHotkey(int32 hid) {
        SoraKeyPoll::delGlobalHotkey(hid);
    }
    
    void SoraCore::ClearGlobalHotkeys() {
        SoraKeyPoll::clearGlobalHotkeys();
    }
	
	StringType SoraCore::fileOpenDialog(const char* filter, const char* defaultPath) {
		if(!pMiscTool)
			return StringType();
		
		return pMiscTool->fileOpenDialog(filter, defaultPath);
	}
	
	StringType SoraCore::fileSaveDialog(const char* filter, const char* defaultPath, const char* defaultExt) {
		if(!pMiscTool)
			return StringType();
		
		return pMiscTool->fileSaveDialog(filter, defaultPath, defaultExt);
	}

	int32 SoraCore::messageBox(const StringType& sMssg, const StringType& sTitle, int32 iCode) {
		if(!pMiscTool)
			return 0;
		
		return pMiscTool->messageBox((SoraWString)sMssg, (SoraWString)sTitle, iCode);
	}

	void SoraCore::registerPlugin(SoraPlugin* pPlugin) {
		pPluginManager->registerPlugin(pPlugin);
        
        log_mssg(vamssg("Plugin %s registered", pPlugin->getName().c_str()));
	}

	SoraPlugin* SoraCore::unistallPlugin(SoraPlugin* pPlugin) {
		SoraPlugin* plugin = pPluginManager->unistallPlugin(pPlugin);
        
        if(plugin)
            log_mssg(vamssg("Plugin %s unistalled", pPlugin->getName().c_str()));
        return plugin;
	}

	SoraPlugin* SoraCore::unistallPlugin(const SoraString& sPluginName) {
		SoraPlugin* plugin = pPluginManager->unistallPlugin(sPluginName);
        
        if(plugin)
            log_mssg(vamssg("Plugin %s unistalled", sPluginName.c_str()));
        return plugin;
	}

	SoraPlugin* SoraCore::getPlugin(const SoraString& sPluginName) {
		return pPluginManager->getPlugin(sPluginName);
	}

	StringType SoraCore::getVideoInfo() {
		sora_assert(bInitialized==true);
		return pRenderSystem->videoInfo();
	}
    
    uint64 SoraCore::getSystemMemorySize() {
        return pMiscTool->getSystemMemorySize();
    }
    
    uint32 SoraCore::getCPUSpeed() {
        return pMiscTool->getProcessorSpeed();
    }
    
    StringType SoraCore::getOSVersion() {
        return pMiscTool->getOSVersion();
    }
    
    StringType SoraCore::getSoraVersion() {
        return vamssg("Hoshizora Version %d.%d Rev %d", SORA_VERSION_MAJOR, SORA_VERSION_MINOR, SORA_VERSION_REV);
    }

	void SoraCore::SetRandomSeed(int32 seed) {
		init_gen_rand(seed);
		iRandomSeed = seed;
	}

	int32 SoraCore::GetRandomSeed() {
		return iRandomSeed;
	}

	int32 SoraCore::RandomInt(int32 min, int32 max) {
		return (int32)(min+genrand_real1()*(max-min));
	}

	float SoraCore::RandomFloat(float min, float max) {
		return (float)(min+genrand_real1()*(max-min));
	}

	int32 SoraCore::RandomIntNoRange() {
		return gen_rand32();
	}

	float SoraCore::RandomFloatNoRange() {
		return (float)genrand_real1();
	}

	SoraFont* SoraCore::createFont(const StringType& font, int32 size) {
		if(!pFontManager) {
			_postError("FontManager not available");
			return 0;
		}

#ifndef OS_IOS
		if(pFontManager->fontExists(font.c_str())) {
            return pFontManager->getFont(font.c_str(), size, 0, 0);
		}

		uint32 s;
		void* p = getResourceFile(font, s);
		if(p) {
			SoraFont* f = pFontManager->getFont((const char*)p, size, s-1, font.c_str());
			if(getSystemFont() == 0)
                setSystemFont(f);
			return f;
		}
#else
		SoraFont* ff = pFontManager->getFont(font.c_str(), size);
		if(!ff)
            _postError("Error loading Font: "+font);
        else {
            if(getSystemFont() == 0)
                setSystemFont(ff);
        }
        return ff;
#endif
		return 0;
	}
    
    SoraFont* SoraCore::createFont(void* data, uint32 size, uint32 fontSize, const StringType& fileName) {
        if(!pFontManager) {
			_postError("FontManager not available");
			return 0;
		}
        
#ifndef OS_IOS		
		if(pFontManager->fontExists(fileName.c_str())) {
            return pFontManager->getFont(fileName.c_str(), size, 0, 0);
		}
		
		SoraFont* f = pFontManager->getFont((const char*)data, fontSize, size, fileName.c_str());
        if(getSystemFont() == 0)
            setSystemFont(f);
        return f;
#else
        // ios not supporting memory font now
        SoraFont* f = pFontManager->getFont((const char*)data, fontSize, size-1, fileName.c_str());
        if(getSystemFont() == 0)
            setSystemFont(f);
        return f;
        return 0;
#endif
		return 0;
    }

	void SoraCore::releaseFont(SoraFont* font) {
		if(!pFontManager) {
			_postError("FontManager not available");
			return;
		}
		
        if(font == this->getSystemFont()) {
            setSystemFont((SoraFont*)0);
        }
		pFontManager->releaseFont(font);
	}
	
	void SoraCore::enumFilesInFolder(std::vector<StringType>& cont, const StringType& folder) {
		pFileSystem->enumFiles(cont, folder);
	}

	void SoraCore::setFrameSync(bool flag) {
		bFrameSync = flag;
	}
    
    bool SoraCore::isMainWindowSet() { 
        return bMainWindowSet;
    }
	
	SoraMusicFile* SoraCore::createMusicFile(const StringType& musicName, bool bStream) {
		if(!pSoundSystem) {
			_postError("SoraCore::createMusicFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createMusicFile(musicName, bStream);
	}
	
	SoraSoundEffectFile* SoraCore::createSoundEffectFile(const StringType& se) {
		if(!pSoundSystem) {
			_postError("SoraCore::createSoundEffectFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createSoundEffectFile(se);
	}
    
    SoraMusicFile* SoraCore::createMusicFile(bool bStream) {
        if(!pSoundSystem) {
			_postError("SoraCore::createMusicFile: no soundsystem available");
			return NULL;
		}
        
        return pSoundSystem->createMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraCore::createSoundEffectFile() {
        if(!pSoundSystem) {
			_postError("SoraCore::createSoundEffectFile: no soundsystem available");
			return NULL;
		}
		
		return pSoundSystem->createSoundEffectFile();
    }
    
    void SoraCore::Execute(const SoraString& appPath, const SoraString& args) {
        system((appPath+" "+args).c_str());
    }
    
    void SoraCore::snapshot(const SoraString& path) {
        sora_assert(bInitialized == true);
        return pRenderSystem->snapshot(path);
    }

	uint64 SoraCore::getEngineMemoryUsage() const {
		return getMemoryUsage();
	}
	
    SoraFont* SoraCore::getSystemFont() const {
        return mSystemFont;
    }
    
    void SoraCore::setSystemFont(SoraFont* font) {
        mSystemFont = font;
        
		SoraConsole::Instance()->setFont(mSystemFont);
		SoraMenuBar::Instance()->setFont(mSystemFont);
    }
	
	void SoraCore::setSystemFont(const wchar_t* font, int32 fontSize) {
		mSystemFont = createFont(font, fontSize);
		SoraConsole::Instance()->setFont(mSystemFont);
		SoraMenuBar::Instance()->setFont(mSystemFont);
	}
	
	void SoraCore::beginZBufferSort() {
		bZBufferSort = true;
	}

	void SoraCore::endZBufferSort() {
		bZBufferSort = false;
		SoraZSorter::endSortAndRender();
	}

	void SoraCore::pause(bool render, bool sound) {
		bPauseRender = render;
		bPauseSound = sound;
		
		bPaused = true;
	}
	
	void SoraCore::resume() {
		bPaused = false;
	}
	
	bool SoraCore::isPaused() {
		return bPaused;
	}

	void SoraCore::setIcon(const SoraString& icon) {
		sora_assert(bInitialized);
		pRenderSystem->setIcon(icon);
	}

	void SoraCore::setCursor(const SoraString& cursor) {
		sora_assert(bInitialized);
		pRenderSystem->setCursor(cursor);
	}

	void SoraCore::showMouse(bool bShow) {
		sora_assert(bInitialized);
		pRenderSystem->showMouse(bShow);
	}
    
    void SoraCore::registerFullscreenBufferDelegate(const SoraFunction<void(SoraTextureHandle)>& delegate) {
        SoraFullscreenBufferHandler::Instance()->registerDelegate(delegate);
    }
    
    void SoraCore::unregisterFullscreenBufferDelegate() {
        SoraFullscreenBufferHandler::Instance()->unregisterDelegate();
    }
    
    void SoraCore::pushTransformMatrix() {
        sora_assert(bInitialized);
        
        g_TransformStack.push(pRenderSystem->getTransformMatrix());
    }
    
    void SoraCore::popTransformMatrix() {
        sora_assert(bInitialized);
        
        if(!g_TransformStack.empty()) {
            g_CurrentTransform = g_TransformStack.top();
            g_TransformStack.pop();
            
            pRenderSystem->setTransformMatrix(g_CurrentTransform);
        }
    }
   
    void SoraCore::pushClippingMatrix() {
        g_ClippingStack.push(g_CurrentClipping);
    }
    
    void SoraCore::popClippingMatrix() {
        if(!g_ClippingStack.empty()) {
            g_CurrentClipping = g_ClippingStack.top();
            g_ClippingStack.pop();
            
            setClipping(g_CurrentClipping.x,
                        g_CurrentClipping.y,
                        g_CurrentClipping.w,
                        g_CurrentClipping.h);
        }
    }
    
    uint64 SoraCore::getResourceMemoryUsage() const {
        return SoraFileSystem::ResourceMemory / 1024;
    }
    
    void SoraCore::addInputListener(SoraInputListener* listener, int priority) {
        SoraKeyPoll::AddInputListener(listener, priority);
    }
    
    void SoraCore::delInputListener(SoraInputListener* listener) {
        SoraKeyPoll::DelInputListener(listener);
    }
    
    void SoraCore::SetFarPoint(const SoraVector3& ptFar) {
        mFarPoint = ptFar;
    }
    
    SoraVector3 SoraCore::GetFarPoint() {
        return mFarPoint;
    }
    
    float SoraCore::Transform3DPoint(SoraVector3* point) {
        if(mFarPoint.z == 0.f)
            return 1.f;
        
        float scale = (mFarPoint.z - point->z) / mFarPoint.z;
        point->x = (point->x - mFarPoint.x) * scale + mFarPoint.x;
        point->y = (point->y - mFarPoint.y) * scale + mFarPoint.y;
        return scale;
    }
    
    float SoraCore::Transform3DPoint(float* x, float* y, float* z) {
        if(mFarPoint.z == 0.f)
            return 1.f;
        
        float scale = (mFarPoint.z - *z) / mFarPoint.z;
        *x = (*x - mFarPoint.x) * scale + mFarPoint.x;
        *y = (*y - mFarPoint.y) * scale + mFarPoint.y;
        return scale;
    }
    
    bool SoraCore::Is3DRendering() {
        return mIs3DRendering;
    }
    
    SoraMusicFile* SoraCore::createMusicFileFromMemory(void* pdata, uint32 size) {
        if(pSoundSystem)
            return pSoundSystem->createMusicFileFromMemory(pdata, size);
        return 0;
    }
    
    SoraSoundEffectFile* SoraCore::createSoundEffectFileFromMemory(void* pdata, uint32 size) {
        if(pSoundSystem)
            return pSoundSystem->createSoundEffectFileFromMemory(pdata, size);
        return 0;
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    
    void SoraCore::beginMultithreadedRendering() {
        bMultithreadedRendering = true;
        beginZBufferSort();
    }
    
    void SoraCore::endMultithreadedRendering() {
        bMultithreadedRendering = false;
        endZBufferSort();
    }
    
#endif    
    
    void SoraCore::set3DCamera(SoraCamera* camera) {
        mCamera = camera;
    }
    
    SoraCamera* SoraCore::get3DCamera() const {
        return mCamera;
    }
    
    SoraGraphicDeviceCaps SoraCore::getGraphicDeviceCaps() const {
        return mGraphicDeviceCaps;
    }
    
} // namespace sora
