/*
 *  SoraRenderSystem.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/24/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_RENDER_SYSTEM_H_
#define SORA_RENDER_SYSTEM_H_

#include "SoraPlatform.h"
#include "SoraException.h"

#include "SoraString.h"
#include "SoraRenderBuffer.h"
#include "SoraGraphicDeviceCaps.h"

#include "math/SoraMatrix4.h"

#include <map>

namespace sora {
	/**
		Abstarct Render System Base
	*/
    
    class SoraTimer;
    class SoraShaderContext;
    class SoraShader;
    class SoraWindowInfoBase;
    class SoraTexture;
    struct SoraVertex;
    struct SoraQuad;

	class SORA_API SoraRenderSystem {
	public:
        enum RenderSystemType {
            // Windows
            DirectX,
            // Mac OS X & Linux 
            OpenGL,
            // Mobile Platforms
            OpenGLES,
            OpenGLES2,
        };
        
        /*
         Render System Info
         */
        virtual StringType       videoInfo() const = 0;
        virtual RenderSystemType renderSystemType() const = 0;
        virtual void             fillDeviceCaps(SoraGraphicDeviceCaps& caps) = 0;
        
    public:
        /*
         Update & scene
         */
		virtual bool update() = 0;

		virtual void beginScene(uint32 c, SoraTargetHandle t, bool clear=true) = 0;
		virtual void endScene() = 0;
		
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void start(SoraTimer* timer) = 0;
        
        virtual void switchTo2D() = 0;
        virtual void switchTo3D() = 0;
        
        virtual SoraWindowHandle createWindow(SoraWindowInfoBase* windowInfo) = 0;
        
    public:
        /* 
         Render Targets(FBOs)
         */

		virtual SoraTargetHandle createTarget(int width, int height, bool zbuffer=true) = 0;
		virtual void             freeTarget(SoraTargetHandle t) = 0;
		virtual SoraTargetHandle getTargetTexture(SoraTargetHandle t) = 0;
        
    public:
        /*
         Render Buffers(IBOs & VBOs)
         */
        virtual SoraRenderBuffer::Ptr createVertexBuffer(SoraRenderBuffer::Access accessHint,
                                                         SoraRenderBuffer::Usage usage,
                                                         uint32 desired_count,
                                                         void* initData,
                                                         const SoraVertexFormat& vertexFormat) = 0;
        
        virtual SoraRenderBuffer::Ptr createIndexBuffer(SoraRenderBuffer::Access accessHint,
                                                        SoraRenderBuffer::Usage usage,
                                                        uint32 desired_count,
                                                        void* initData) = 0;
        
        virtual void renderBuffer(SoraTexture* tex, RenderMode mode, SoraRenderBuffer::Ptr vertexBuffer, SoraRenderBuffer::Ptr indexBuffer) = 0;
         
         
    public:
        /*
         Textures
         */
		virtual SoraTexture* createTexture(const StringType& sTexturePath, bool bMipmap=false) = 0;
		virtual SoraTexture* createTextureWH(int32 w, int32 h) = 0;
		virtual SoraTexture* createTextureFromMem(void* ptr, uint32 size, bool bMipmap=false) = 0;
		virtual SoraTexture* createTextureFromRawData(unsigned int* data, int32 w, int32 h) = 0;

		virtual uint32*  	 textureLock(SoraTexture* ht) = 0;
		virtual void         textureUnlock(SoraTexture* ht) = 0;
        virtual void		 releaseTexture(SoraTexture* pTexture) = 0;

    public:
        /*
         Rendering
         */
		virtual void renderQuad(SoraQuad& quad) = 0;
		virtual void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode) = 0;

		virtual void renderLine (float x1, float y1, float x2, float y2, uint32 color, float width=1.f, float z=0.f) = 0;
		virtual void renderBox  (float x1, float y1, float x2, float y2, uint32 color, float lineWidth=1.f, float z=0.f) = 0;
        virtual void fillBox    (float x1, float y1, float x2, float y2, uint32 color, float z=0.f) = 0;
        
    public:
        /* 
         Clipping and transform
         */
		virtual void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0) = 0;
		virtual void setTransform(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=0.f, float vscale=0.f) = 0;
       
        virtual void        setTransformMatrix(const SoraMatrix4& matrix) = 0;
        virtual void        multTransformMatrix(const SoraMatrix4& matrix) = 0;
        virtual SoraMatrix4 getTransformMatrix() const = 0;
        
        virtual void        setProjectionMatrix(const SoraMatrix4& matrix) = 0;
        virtual SoraMatrix4 getProjectionMatrix() const = 0;

		virtual SoraHandle getMainWindowHandle() = 0;
		virtual SoraWindowInfoBase* getMainWindow() = 0;
        
    public:
        /*
         States
         */
        virtual bool isActive() = 0;

        virtual void             setRenderState(RenderStateType type, int32 param) = 0;
        virtual int32            getRenderState(RenderStateType type) const = 0;
                
        virtual SoraHandle getVideoDeviceHandle() = 0;

    public:
        /*
         Shaders
         */
        virtual SoraShaderContext*  createShaderContext() = 0;
		virtual	void                attachShaderContext(SoraShaderContext* context) = 0;
		virtual void                detachShaderContext() = 0;

    public:
        /*
         Window
         */
		virtual void setWindowSize(int32 w, int32 h) = 0;
		virtual void setWindowTitle(const StringType& title) = 0;
		virtual void setWindowPos(int32 px, int32 py) = 0;
		virtual void setFullscreen(bool flag) = 0;
		virtual bool isFullscreen() = 0;
        virtual void setVerticalSync(bool flag) = 0;
        
        /*
         Misc
         */
        virtual void snapshot(const StringType& path) = 0;

		virtual void setIcon(const StringType& icon) = 0;
		virtual void setCursor(const StringType& cursor) = 0;
        virtual void showMouse(bool bShow) = 0;

        typedef void (*QueryVideoMode)(int w, int h);
        virtual void getDesktopResolution(int* w, int* h) = 0;
        virtual void setQueryVideoModeCallback(QueryVideoMode func) = 0;
        
        virtual void shutdown() = 0;
	};

} // namespace sora

#endif // SORA_RENDER_SYSTEM_H
