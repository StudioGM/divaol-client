/*
 *  SoraHGERenderer.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// for HGE

#ifndef _SORA_HGE_RENDERER_H_
#define _SORA_HGE_RENDERER_H_

#include "hge/hge.h"
#include "SoraRenderSystem.h"

namespace sora {
	class SoraHGERenderer: public SoraRenderSystem {
	public:
		SoraHGERenderer();
		~SoraHGERenderer();

		bool update();

		void beginScene(uint32 color=0, SoraHandle target=0, bool clear=true);
		void endScene();
		
		void beginFrame() {}
		void endFrame() {}

		void shutdown();
		bool isActive();

		void start(SoraTimer* timer);

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

		RenderSystemType renderSystemType() const;
        void fillDeviceCaps(SoraGraphicDeviceCaps& caps);

		SoraHandle	createTarget(int width, int height, bool zbuffer=true);
		void		freeTarget(SoraHandle t);
		SoraHandle	getTargetTexture(SoraHandle t);

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
		void renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 size, RenderMode mode);

		void setClipping(int32 x=0, int32 y=0, int32 w=0, int32 h=0);
		void setTransform(float x=0.f, float y=0.f, float dx=0.f, float dy=0.f, float rot=0.f, float hscale=0.f, float vscale=0.f);
		void setTransformWindowSize(float w, float h);
        void setViewPoint(float x=0.f, float y=0.f, float z=0.f);
		void setVerticalSync(bool flag);
		
		SoraHandle getMainWindowHandle();
		SoraWindowInfoBase* getMainWindow();

		SoraShaderContext* createShaderContext();
		void attachShaderContext(SoraShaderContext* context);
		void detachShaderContext();

		StringType videoInfo() const;
		SoraHandle getVideoDeviceHandle();
        void snapshot(const StringType& path);

		void flush();

		void setIcon(const StringType& icon);
		void setCursor(const StringType& cursor);
		void showMouse(bool bShow);
		
		void renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z);
        void renderBox(float x1, float y1, float x2, float y2, uint32 color, float width, float z);
		void fillBox(float x1, float y1, float x2, float y2, uint32 color, float z);

		void getDesktopResolution(int* w, int *y);
		void setQueryVideoModeCallback(QueryVideoMode func);
		
		void setTransformMatrix(const SoraMatrix4& mat);
		void multTransformMatrix(const SoraMatrix4& matrix);
		SoraMatrix4 getTransformMatrix() const;

		void setProjectionMatrix(const SoraMatrix4& matrix);
        SoraMatrix4 getProjectionMatrix() const;
		
		void setRenderState(RenderStateType, int32);
        int32 getRenderState(RenderStateType) const;

		void switchTo2D();
        void switchTo3D();

	private:
		inline int32 _modeToDXMode(int32 mode);
		
		typedef std::map<HTEXTURE, SoraTextureHandle> TARGET_TEX_MAP;
		TARGET_TEX_MAP mTargetTextures;
		
		HGE* pHGE;

		SoraTimer* pTimer;
		SoraWindowInfoBase* pMainWindow;

		bool bFullscreen;

		SoraShaderContext* currShader;
	};
} // namespace sora

#endif