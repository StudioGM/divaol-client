/*
 *  SoraOGLRenderer.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/23/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef _SORA_OGL_RENDERER_H_
#define _SORA_OGL_RENDERER_H_

#include "SoraRenderSystem.h"
#include "SoraOGLRenderTarget.h"

#include <list>

namespace sora {

	class SoraOGLRenderer: public SoraRenderSystem {
	public:
		SoraOGLRenderer();
		~SoraOGLRenderer();

		bool update();

		void beginScene(uint32 color=0, SoraTargetHandle target=0, bool clear=true);
		void endScene();
		
		void beginFrame();
		void endFrame();

		void shutdown();
		bool isActive();
        
        void switchTo2D();
        void switchTo3D();

		void start(SoraTimer* timer);

		SoraTargetHandle createTarget(int width, int height, bool zbuffer=true);
		void             freeTarget(SoraTargetHandle t);
		SoraTargetHandle getTargetTexture(SoraTargetHandle t);

		SoraWindowHandle createWindow(SoraWindowInfoBase* windowInfo);
		void setWindowSize(int32 w, int32 h);
		void setWindowTitle(const StringType& title);
		void setWindowPos(int32 px, int32 py);
		void setFullscreen(bool flag);
		bool isFullscreen();

		SoraTexture* createTexture(const StringType& sTexturePath, bool bMipmap=false);
		SoraTexture* createTextureWH(int w, int h);
		SoraTexture* createTextureFromMem(void* ptr, uint32 size, bool bMipmap=false);
		SoraTexture* createTextureFromRawData(unsigned int* data, int32 w, int32 h);

		uint32*		 textureLock(SoraTexture*);
		void		 textureUnlock(SoraTexture*);
		void		 releaseTexture(SoraTexture* tex);

		void renderQuad(SoraQuad& quad);
		void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode);
        
        void renderLine     (float x1, float y1, float x2, float y2, uint32 color, float width=1.f, float z=0.f);
		void renderBox		(float x1, float y1, float x2, float y2, uint32 color, float lineWidth=1.f, float z=0.f);
        void fillBox        (float x1, float y1, float x2, float y2, uint32 color, float z=0.f);

		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=1.f, float vscale=1.f);
        
        void setTransformMatrix(const SoraMatrix4& matrix);
        void multTransformMatrix(const SoraMatrix4& matrix);
        SoraMatrix4 getTransformMatrix() const;
        
        void setProjectionMatrix(const SoraMatrix4& matrix);
        SoraMatrix4 getProjectionMatrix() const;
        
        void setVerticalSync(bool flag);

		SoraHandle getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();

        SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		StringType          videoInfo() const;
        RenderSystemType    renderSystemType() const;
        void                fillDeviceCaps(SoraGraphicDeviceCaps& caps);
		SoraHandle          getVideoDeviceHandle();

		void flush();
        
        void snapshot(const StringType& path);
        
        void setIcon(const StringType& icon);
        void setCursor(const StringType& cursor);
        void showMouse(bool bShow);
        
        void getDesktopResolution(int* w, int* h);
        void setQueryVideoModeCallback(QueryVideoMode func);
        
        void setRenderState(RenderStateType, int32);
        int32 getRenderState(RenderStateType) const;
        
        SoraRenderBuffer::Ptr createVertexBuffer(SoraRenderBuffer::Access accessHint,
                                                 SoraRenderBuffer::Usage usage,
                                                 uint32 desired_count,
                                                 void* initData,
                                                 const SoraVertexFormat& vertexFormat);
        SoraRenderBuffer::Ptr createIndexBuffer(SoraRenderBuffer::Access accessHint,
                                                SoraRenderBuffer::Usage usage,
                                                uint32 desired_count,
                                                void* initData);
        void renderBuffer(SoraTexture* tex, RenderMode mode, SoraRenderBuffer::Ptr vertexBuffer, SoraRenderBuffer::Ptr indexBuffer);

	private:
		void applyTransform();
		void bindTexture(SoraTexture* tex);

		inline void _glInitialize();
		inline void _glEndFrame();
		inline void _glBeginFrame();
		inline void _glBeginScene(uint32 color, SoraHandle target, bool clear);
		inline void _glEndScene();
		inline int32 _glTextureGetWidth(int32 tex, bool bOriginal=false);
		inline int32 _glTextureGetHeight(int32 tex, bool bOriginal=false);
		inline void _glSetProjectionMatrix(int32 w, int32 h);
		inline void _glSetBlendMode(int32 mode);

		// checks if opengl 2.0 is available
		inline bool _glVersionCheck();
		// checks if glsl is avaiable
		inline bool _glShaderCheck();
		const char* _glGetShaderLog(GLuint shader);

		inline bool _glCheckError();

		SoraWindowInfoBase* mainWindow;
		int32 windowWidth;
		int32 windowHeight;

		struct _SoraOGLWindowInfo {
			float x, y, z;
			float dx, dy;
			float rot;
			float hscale, vscale;
			int32 width, height;
			
			_SoraOGLWindowInfo(): hscale(1.f), vscale(1.f), x(0.f), y(0.f), z(0.f), dx(0.f), dy(0.f), rot(0.f), width(0), height(0) {}
		};
		_SoraOGLWindowInfo _oglWindowInfo;

		int32 CurBlendMode;
		int32 CurDrawMode;

		SoraTimer* pTimer;
        
		std::list<SoraRenderTargetOG*> liTargets;
		SoraRenderTargetOG* pCurTarget;

		bool bShaderAvailable;
		GLuint uGLShaderProgram;

		GLuint mCurrTexture;

		SoraShaderContext* currShader;

		bool bFullscreen;
		int iFrameStart;
	};
} // namespace sora

#endif
