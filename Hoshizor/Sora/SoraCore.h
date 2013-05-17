#ifndef SORA_INTERNAL_CORE
#define SORA_INTERNAL_CORE

#include "SoraPlatform.h"
#include "SoraString.h"
#include "SoraMath.h"
#include "SoraKeyInfo.h"
#include "SoraVertex.h"
#include "SoraGraphicDeviceCaps.h"

#include "function/SoraFunction.h"

#ifdef SORA_ENABLE_MULTI_THREAD        
#include "thread/SoraMutex.h"
#endif 

#include <map>
#include <list>

namespace sora {
    
    class SoraSprite;
    class SoraInput;
    class SoraRenderSystem;
    class SoraPluginManager;
    class SoraResourceManager;
    class SoraShader;
    class SoraShaderContext;
    class SoraFrameListener;
    class SoraFontManager;
    class SoraTimer;
    class SoraMiscTool;
    class SoraSoundSystem;
    class SoraEventHandler;
    class SoraMusicFile;
    class SoraSoundEffectFile;
    class SoraPlugin;
    class SoraHotkey;
    class SoraFont;
    class SoraWindowInfoBase;
    class SoraInputListener;
    class SoraPhysicWorld;
    class SoraFileSystem;
    class SoraCamera;
    
	class SORA_API SoraCore {
	protected:
		SoraCore();
		~SoraCore() { }

	public:
        static SoraCore* Instance();
        
        struct Feature {
        public:
            Feature(bool loadPlugins=true, bool renderToBuffer=false, bool postErrorByMessageBox=false, bool seperateSSthread=false, bool debugRender=false):
            LoadPlugins(loadPlugins),
            RenderToBuffer(renderToBuffer),
            MessageBoxErrorPost(postErrorByMessageBox),
            SeperateSoundSystemThread(seperateSSthread), 
            DebugRender(debugRender) {
                
            }
            
            bool LoadPlugins;
            bool RenderToBuffer;
            bool MessageBoxErrorPost;
            bool SeperateSoundSystemThread;
            bool DebugRender;
            bool ExternalLoop;
        };
        
        void init(const Feature& param = Feature());
        
		void start();
		void shutDown();

		void update();
		bool isActive();
		
		void pause(bool bPauseRender=true, bool bPauseSound=true);
		void resume();
		bool isPaused();
        
        /**
         * Core Feature enums
         **/
        enum CoreFeatures {
            FeatureLoadPlugin = 1,
            FeatureRenderToBuffer,
            FeatureMessageBoxErrorPost,
            FeatureSeperateSoundSystem,
            FeatureDebugRendering,
        };
        
        void enableCoreFeature(CoreFeatures feature);
        void disableCoreFeature(CoreFeatures feature);

    public:
        /**
         * components
         **/
		void registerRenderSystem	(SoraRenderSystem* renderSystem);
		void registerResourceManager(SoraResourceManager* resourceManager);
		void registerInput			(SoraInput* input);
		void registerFontManager	(SoraFontManager* fontManager);
		void registerSoundSystem	(SoraSoundSystem* soundSystem);
		void registerMiscTool		(SoraMiscTool* miscTool);
		void registerPluginManager  (SoraPluginManager* pluginManager);
		void registerTimer			(SoraTimer* timer);
        void registerPhysicWorld    (SoraPhysicWorld* physicWorld);
            
        SoraRenderSystem*       getRenderSystem() const;
        SoraInput*              getInput() const;
        SoraMiscTool*           getMiscTool() const;
        SoraPluginManager*      getPluginManager() const;
        SoraTimer*              getTimer() const;
        SoraFontManager*        getFontManager() const;
        SoraSoundSystem*        getSoundSystem() const;
        SoraPhysicWorld*        getPhysicWorld() const;
        SoraFileSystem*         getFileSystem() const;
        
    public:
        /**
         * Plugin and extension
         **/
		void        registerPlugin  (SoraPlugin* pPlugin);
		SoraPlugin* unistallPlugin  (SoraPlugin* pPlugin);
		SoraPlugin* unistallPlugin  (const SoraString& sPluginName);
		SoraPlugin* getPlugin		(const SoraString& sPluginName);
        
    public:
        /**
         * fps
         **/

		void	setFPS(int32 fps);
		float   getFPS();
		float   getDelta();
        float   getAbsoluteDelta();
        void    setVerticalSync(bool flag);
        
		/*
		 if enabled
		 getDelta would always return 1.f
		 */
		void setFrameSync(bool flag);

    public:
        /**
         * Rendering
         **/
        void beginScene(uint32 c=0xFF000000, SoraTargetHandle h=0, bool clear=true);
		void endScene();
        
		int32 getScreenWidth();
		int32 getScreenHeight();

		SoraTargetHandle    createTarget(int width, int height, bool zbuffer=true);
		void                freeTarget(SoraHandle t);
		SoraTextureHandle   getTargetTexture(SoraHandle t);

		SoraTextureHandle   createTexture(const StringType& sTexturePath, bool bCache=true, bool bMipmap=false);
		SoraTextureHandle   createTextureWH(int32 w, int32 h);
		SoraTextureHandle   createTextureFromRawData(uint32* data, int32 w, int32 h);
		SoraTextureHandle   createTextureFromMem(void* data, uint32 size);
		uint32*             textureLock(SoraTextureHandle);
        void                textureUnlock(SoraTextureHandle);
		// depends on render system, may have different meaning
		SoraHandle          getTextureId(SoraTextureHandle);
		void                releaseTexture(SoraTextureHandle);
        void                clearTextureMap();

        SoraShaderContext*  createShaderContext();
		void                attachShaderContext(SoraShaderContext* context);
		void                detachShaderContext();

		SoraSprite* createSprite (const StringType& sPath);

		void renderQuad(SoraQuad& quad);
		void renderWithVertices(SoraTextureHandle tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode=Triangle);

        void renderLine     (float x1, float y1, float x2, float y2, uint32 color, float width=1.f, float z=0.f);
		void renderBox		(float x1, float y1, float x2, float y2, uint32 color, float lineWidth=1.f, float z=0.f);
        void fillBox        (float x1, float y1, float x2, float y2, uint32 color, float z=0.f);
        
		void setClipping	(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform	(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=1.f, float vscale=1.f);
		
		void pushTransformMatrix();
		void popTransformMatrix();
        void pushClippingMatrix();
        void popClippingMatrix();
        
		/*
		 z sort without zbuffer, because there are some problems with transparency when zbuffer is on
		 we don't want our transparency pixels in textures to be omitted when rendering with z
		 implemention for fast sort, accuracy 0.001
		 */
		void beginZBufferSort();
		void endZBufferSort();
        
        void switchTo2D();
        void switchTo3D();
        
        void          set3DCamera(SoraCamera* camera);
        SoraCamera*   get3DCamera() const;

#ifdef SORA_ENABLE_MULTI_THREAD        
        /*
         These calls must exist in the main thread
         After multithreadedRendering begins
         It's safe to call rendering functions in another thread
         */
        void beginMultithreadedRendering();
        void endMultithreadedRendering();
#endif
        
        void snapshot(const SoraString& path);
        
    public:
        /**
         * Static 
         **/
        
		/*generates a int32 random number using SFMT*/
		static void     SetRandomSeed(int32 seed);
		static int32    GetRandomSeed();
		static int32    RandomInt(int32 min, int32 max);
		static float    RandomFloat(float min, float max);
		static int32    RandomIntNoRange();
		/*generates a float random number range [0, 1] using SFMT*/
		static float    RandomFloatNoRange();
                
        /**
         *  register a global hotkey and it's conresponding event handler
         *  @returnval the id of the registered hotkey
         */
        static int32   RegisterGlobalHotkey(const SoraHotkey& key, SoraEventHandler* handler);
        static void    UnregisterGlobalHotkey(int32 hid);
        static void    SetGlobalHotkey(int32 hid, const SoraHotkey& key);
        static void    ClearGlobalHotkeys();
		
		static void    SimulateKey(int32 key, int32 state);
        
        static void    Execute(const SoraString& appPath, const SoraString& args);
        
		static uint64   GetRunningTime();
		static uint64	GetFrameCount();
		static void     SetTimeScale(float scale);
		static float    GetTimeScale();
		static uint64   GetCurrentSystemTime();
        
        /**
         *  transform a 3d point to 2d using a far point as view
         **/
        static void        SetFarPoint(const SoraVector3& ptFar);
        static SoraVector3 GetFarPoint();
        static float       Transform3DPoint(SoraVector3* point);
		static float       Transform3DPoint(float* x, float* y, float* z);
        
        static bool        Is3DRendering();
        
        static SoraCore* Ptr;
        
    public:
        /**
         * Resource
         **/
        
		SoraResourceHandle  loadResourcePack        (const StringType& file);
		void                detachResourcePack		(SoraResourceHandle handle);
		void*               getResourceFile         (const StringType& file, uint32& size);
		void*               readResourceFile		(const StringType& file, uint32 pos, uint32 size);
		uint32              getResourceFileSize     (const StringType& file);
		void                freeResourceFile		(void* p);
		void                enumFilesInFolder		(std::vector<StringType>& cont, const StringType& folder);

    public:
        /**
         * Window handling
         **/
		bool    isMainWindowSet();
		bool    createWindow(SoraWindowInfoBase* info);
		void    setWindowSize(int32 w, int32 h);
		void    setWindowTitle(const StringType& title);
		void    setWindowPos(int32 px, int32 py);
		void    setFullscreen(bool flag);
		bool    isFullscreen();

		/*
		 under windows, this would return HWND, 
		 under other plaforms, just return (SoraHandle)mainWindow
		 */
		SoraHandle              getMainWindowHandle() const;
		SoraWindowInfoBase*     getMainWindow() const;
        SoraGraphicDeviceCaps   getGraphicDeviceCaps() const;
        
    public:
        /**
         * Input
         **/
        
        void	getMousePos(float *x, float *y);
		float   getMousePosX();
		float   getMousePosY();
		void	setMousePos(float x, float y);
		int		getMouseWheel();
		bool	isMouseOver();
		bool	keyDown(int32 key);
		bool	keyUp(int32 key);
		int32	getKeyState(int32 key);
		char*	getKeyName(int32 key);
		bool	getKeyEvent(SoraKeyEvent& ev);
		bool	joyKeyPressed(int32 key);
		bool	joyKeyDown(int32 key);
		bool	joyKeyUp(int32 key);
		bool	joyKeyState(int32 key, unsigned char state);
		bool	setJoyKey(int32 key);
		bool	hasJoy();
        
    public:
        /**
         * Font
         **/
        
		SoraFont*   createFont(const StringType& fontName, int size);
        SoraFont*   createFont(void* data, uint32 size, uint32 fontSize, const StringType& fileName);
		void        releaseFont(SoraFont* font);
		
    public:
        /**
         * Sound engine
         **/
        
        SoraMusicFile*          createMusicFileFromMemory(void* pdata, uint32 size);
        SoraSoundEffectFile*    createSoundEffectFileFromMemory(void* pdata, uint32 size);
		SoraMusicFile* 			createMusicFile(const StringType& musicName, bool bStream=true);
		SoraSoundEffectFile* 	createSoundEffectFile(const StringType& se);
        SoraMusicFile* 			createMusicFile(bool bStream=false);
        SoraSoundEffectFile* 	createSoundEffectFile();
     
    public:
        /**
         * System misc
         **/
		int32	messageBox  (const StringType& sMssg, const StringType& sTitle, int32 iCode);
		
		std::string fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
		std::wstring fileOpenDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL, bool multiFile = false);
		std::string fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);
		std::wstring fileSaveDialogW(const wchar_t* filter = NULL, const wchar_t* defaultPath = NULL, const wchar_t* defaultExt = NULL);

       	/*
		 for directX, return (SoraHandle)(pD3DDevice)
		 for ogl, just return (SoraHandle)(pRenderSystem)
		 */
		SoraHandle   getVideoDeviceHandle();
		StringType   getVideoInfo();
        uint64       getSystemMemorySize();
        uint32       getCPUSpeed();
        StringType   getOSVersion();
        StringType   getSoraVersion();
        
		void setIcon(const SoraString& icon);
		void setCursor(const SoraString& cursor);
		void showMouse(bool bShow);
        
    public:
        /**
         * misc
         **/
        
        void addInputListener(SoraInputListener* listener, int priority=0);
        void delInputListener(SoraInputListener* listener);
        
		void postError(const SoraString& sMssg);
 
		/* frame listener functions */
		void addFrameListener(SoraFrameListener* listener);
		void delFrameListener(SoraFrameListener* listener);
		
        /**
         *  Memory usage calculated by Operating System
         *  libproc required for Linux
         **/
		uint64 getEngineMemoryUsage() const;
        /**
         *  Memory usage calculated by SoraFileSystem
         *  Only calculates resource allocation and free
         *  (Library managed memory not included, such as gl texture)
         **/
        uint64 getResourceMemoryUsage() const;
		
        void setSystemFont(SoraFont* font);
		void setSystemFont(const wchar_t* font, int32 fontSize);
		SoraFont* getSystemFont() const;
        
        typedef SoraFunction<void(SoraTextureHandle)> FullScreenBufferDelegateType;
        void registerFullscreenBufferDelegate(const FullScreenBufferDelegateType& delegate);
        void unregisterFullscreenBufferDelegate();
        
    private:
        static SoraCore* mInstance;
        
		inline void _registerCoreCmds();
		inline void _checkCoreComponents();
		inline void _postError(const SoraString& str);
		inline void _initializeTimer();
		inline void _initializeMiscTool();
        inline void _registerEventTypes();
        inline void _updateEnd();
        inline void _regGlobalProducts();
		inline void _frameListenerStart();
		inline void _frameListenerEnd();
        inline void _modifierAdapterUpdate();

		SoraMiscTool*			pMiscTool;
		SoraRenderSystem*		pRenderSystem;
		SoraFileSystem*         pFileSystem;
		SoraInput*				pInput;
		SoraPluginManager*		pPluginManager;
		SoraTimer*				pTimer;
		SoraFontManager*		pFontManager;
		SoraSoundSystem*		pSoundSystem;
        SoraPhysicWorld*        pPhysicWorld;

		bool bMainWindowSet;
		bool bMessageBoxErrorPost;
		bool bInitialized;
		bool bHasInput;
		bool bPluginDetection;
        bool bSeperateSoundSystemThread;
		bool bDebugRender;
        bool bExternalLoop;
        
		bool bPaused;
		bool bPauseRender;
		bool bPauseSound;

		static int32 iRandomSeed;
		int32 iScreenWidth, iScreenHeight;

        bool bMainScene;
		bool bFrameSync;

        SoraHandle mWindowHandle;
		SoraWindowInfoBase* mMainWindow;
        SoraShaderContext* mPrevShaderContext;
        
        bool bEnableScreenBuffer;
        bool bScreenBufferAttached;
        SoraHandle mScreenBuffer;
				
		typedef std::list<SoraFrameListener*> FRAME_LISTENER_CONT;
		FRAME_LISTENER_CONT frameListeners;
		
		bool bZBufferSort;
        SoraFont* mSystemFont;
        
        static SoraVector3 mFarPoint;
        static bool mIs3DRendering;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        bool bMultithreadedRendering;
        SoraMutexLock mRenderingLock;
#endif
        
        SoraCamera* mCamera;
        SoraGraphicDeviceCaps mGraphicDeviceCaps;
    };
    
    typedef SoraCore::Feature SoraCoreFeature;
        
    
    SORA_STATIC_RUN_CODE_I(sora_start_core, SoraCore::Instance());
    
} // namespace sora

#endif
