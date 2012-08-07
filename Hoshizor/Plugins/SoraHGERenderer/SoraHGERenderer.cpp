/*
 *  SoraHGERenderer.cpp
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraHGERenderer.h"

#include "SoraStringConv.h"

#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "SoraInfiniteRendererCallback.h"

#ifdef SORA_USE_SHADER
#include "SoraShader/SoraCGD3D9Shader.h"
#endif

#include "SoraEnvValues.h"
#include "SoraLogger.h"

#include "SoraWindowInfo.h"
#include "SoraTexture.h"

#include "SoraDXRenderBuffer.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace sora{
	
	SoraHGERenderer::SoraHGERenderer() {
		pHGE = hgeCreate(HGE_VERSION);
		
		if(!pHGE) {
			throw SORA_EXCEPTION(SystemException, "Error initiazing HGE");
		}

		currShader = 0;
	}

	SoraHGERenderer::~SoraHGERenderer() {
	//	shutdown();
	}

	void SoraHGERenderer::flush() {
		pHGE->_render_batch(false);
	}

	void SoraHGERenderer::shutdown() {
		if(pHGE) {
			pHGE->System_Shutdown();
			pHGE->Release();
		}
	}

	bool SoraHGERenderer::update() {
		return false;
	}

	void SoraHGERenderer::beginScene(uint32 color, SoraHandle target, bool clear) {
		pHGE->Gfx_BeginScene(target);
		if(clear)
			pHGE->Gfx_Clear(color);
	}

	void SoraHGERenderer::endScene() {
		currShader = 0;
		pHGE->Gfx_EndScene();
	}

	void SoraHGERenderer::start(SoraTimer* timer) {
		pTimer = timer;
		sora::g_timer = pTimer;
		assert(pTimer != 0);

		pHGE->System_Start();
		while(true) {
			MSG msg;
			if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) { 
				if (msg.message == WM_QUIT)	break;
				// TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			if(g_timer->update()) {
				pHGE->_UpdateMouse();
				sora::SoraCore::Ptr->update();
				pHGE->_ClearQueue();
			}
		}
	}

	SoraRenderBuffer::Ptr SoraHGERenderer::createVertexBuffer(SoraRenderBuffer::Access accessHint,
                                                         SoraRenderBuffer::Usage usage,
                                                         uint32 desired_count,
                                                         void* initData,
														 const SoraVertexFormat& vertexFormat) {
		return MakeSharedPtr<SoraDXVertexBuffer>(accessHint, usage, desired_count, initData, vertexFormat);
	}
        
	SoraRenderBuffer::Ptr SoraHGERenderer::createIndexBuffer(SoraRenderBuffer::Access accessHint,
                                                        SoraRenderBuffer::Usage usage,
                                                        uint32 desired_count,
                                                        void* initData) {
		return MakeSharedPtr<SoraDXIndexBuffer>(accessHint, usage, desired_count, initData);
	}
        
    void SoraHGERenderer::renderBuffer(SoraTexture* tex, RenderMode mode, SoraRenderBuffer::Ptr vertexBuffer, SoraRenderBuffer::Ptr indexBuffer) {
		vertexBuffer->active();
		if(indexBuffer.isValid())
			indexBuffer->active();

		pHGE->Gfx_RenderBuffer((DWORD)vertexBuffer.get(),
								(DWORD)indexBuffer.get(),
								(HTEXTURE)tex->mTextureID,
								_modeToDXMode(mode),
								vertexBuffer->count(),
								indexBuffer->count());
	}

	SoraRenderSystem::RenderSystemType SoraHGERenderer::renderSystemType() const {
		return SoraRenderSystem::DirectX;
	}

	void SoraHGERenderer::fillDeviceCaps(SoraGraphicDeviceCaps& caps) {
		IDirect3DDevice9* device = (IDirect3DDevice9*)getVideoDeviceHandle();
		D3DCAPS9 d9caps;
		device->GetDeviceCaps(&d9caps);

		caps.max_indices = d9caps.MaxVertexIndex;
		caps.max_texture_height = d9caps.MaxTextureHeight;
		caps.max_texture_width = d9caps.MaxTextureWidth;
		caps.max_vertices = d9caps.MaxPrimitiveCount;
		// unknown now?
		caps.max_vertex_texture_units = -1;
		caps.max_pixel_texture_units = -1;
		caps.max_texture_cube_map_size = -1;
	}

	SoraWindowHandle SoraHGERenderer::createWindow(SoraWindowInfoBase* windowInfo) {
//		if(!windowInfo->isWindowSubWindow()) {
//			int32 nFSAA = GetRenderSystemExtensionParam(SORA_EXTENSION_FSAA);
			pHGE->System_SetState(HGE_FSAA, 0);

			pHGE->System_SetState(HGE_FRAMEFUNC, bool_updateFrame);
		
			pHGE->System_SetState(HGE_SCREENWIDTH, windowInfo->getWindowWidth());
			pHGE->System_SetState(HGE_SCREENHEIGHT, windowInfo->getWindowHeight());
			pHGE->System_SetState(HGE_WINDOWED, true); //modify new by hyf
			pHGE->System_SetState(HGE_HIDEMOUSE, windowInfo->hideMouse());
			pHGE->System_SetState(HGE_TITLE, windowInfo->getWindowName().c_str());
			pHGE->System_SetState(HGE_DONTSUSPEND, true);
			
			pHGE->System_SetState(HGE_FAKE_FULLSCREEN, !windowInfo->isWindowed()); //add new by hyf

			
			if(windowInfo->getIcon() != 0) {
				pHGE->System_SetState(HGE_ICON, windowInfo->getIcon());
			}
		
			if(windowInfo->getCursor() != 0) {
				pHGE->System_SetState(HGE_CURSOR, windowInfo->getCursor());
			}

			pHGE->System_SetState(HGE_ZBUFFER, true);
			pHGE->System_Initiate();
			
			setVerticalSync(false);
        
            SET_ENV_STRING("RENDERER_DRIVER", pHGE->getDeviceInfo());

			pMainWindow = windowInfo;
			return (SoraWindowHandle)pMainWindow;
	//	}
		return 0;
	}

	void SoraHGERenderer::setWindowSize(int32 w, int32 h) {
		pHGE->_Resize(w, h);
		pHGE->_AdjustWindow();
	}

	void SoraHGERenderer::setWindowTitle(const StringType& title) {
		pHGE->System_SetState(HGE_TITLE, ws2s(title).c_str());
	}

	void SoraHGERenderer::setWindowPos(int32 px, int32 py) {
		return;
	}

	void SoraHGERenderer::setVerticalSync(bool flag) {
		if(flag)
			pHGE->System_SetState(HGE_FPS, HGEFPS_VSYNC);
		else
			pHGE->System_SetState(HGE_FPS, 0);
	}

	void SoraHGERenderer::setFullscreen(bool flag) {
		pHGE->System_SetState(HGE_WINDOWED, !flag);
	}

	bool SoraHGERenderer::isFullscreen() {
		return pHGE->System_GetState(HGE_WINDOWED);
	}

	SoraHandle SoraHGERenderer::getMainWindowHandle() {
		return (SoraHandle)pHGE->System_GetState(HGE_HWND);
	}
	
	SoraWindowInfoBase* SoraHGERenderer::getMainWindow() {
		return pMainWindow;
	}
    
    SoraShaderContext* SoraHGERenderer::createShaderContext() {
#ifdef SORA_USE_SHADER
        return new SoraCGD3D9ShaderContext;
#endif
		return NULL;
    }

	void SoraHGERenderer::attachShaderContext(SoraShaderContext* context) {
#ifdef SORA_USE_SHADER
		pHGE->_render_batch();

		currShader = context;
		if(currShader && !currShader->attachShaderList()) {
			log_error("SoraHGERenderer: Unable to attach shader list");
		}
#endif
	}

	void SoraHGERenderer::detachShaderContext() {
#ifdef SORA_USE_SHADER
		if(currShader != NULL) {
			currShader->detachShaderList();
			currShader = 0;
		}
#endif
	}

	SoraTexture* SoraHGERenderer::createTexture(const StringType& sTexturePath, bool bMipmap) {
		HTEXTURE htex = pHGE->Texture_Load(ws2s(sTexturePath).c_str(), 0UL, bMipmap);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<SoraHandle>(htex);
			tex->mTextureWidth		= pHGE->Texture_GetWidth(htex);
			tex->mTextureHeight		= pHGE->Texture_GetHeight(htex);
			tex->mOriginalWidth		= pHGE->Texture_GetWidth(htex, true);
			tex->mOriginalHeight		= pHGE->Texture_GetHeight(htex, true);
			return tex;
		}
		return 0;
	}
	
	// to do
	SoraTexture* SoraHGERenderer::createTextureFromMem(void* ptr, uint32 size, bool bMipmap) {
		HTEXTURE htex = pHGE->Texture_Load(static_cast<const char*>(ptr), size, bMipmap);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<SoraHandle>(htex);
			tex->mTextureWidth		= pHGE->Texture_GetWidth(htex);
			tex->mTextureHeight		= pHGE->Texture_GetHeight(htex);	
			tex->mOriginalWidth		= pHGE->Texture_GetWidth(htex, true);
			tex->mOriginalHeight	= pHGE->Texture_GetHeight(htex, true);
			return tex;
		}
		return 0;
	}

	SoraTexture* SoraHGERenderer::createTextureWH(int w, int h) {
		HTEXTURE htex = pHGE->Texture_Create(w, h);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<SoraHandle>(htex);
			tex->mTextureWidth		= w;
			tex->mTextureHeight		= h;
			tex->mOriginalHeight	= h;
			tex->mOriginalWidth		= w;
			return tex;
		}
		return 0;
	}

	SoraTexture* SoraHGERenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
		HTEXTURE htex = pHGE->Texture_Create(w, h);
		unsigned long* td = pHGE->Texture_Lock(htex, false, 0, 0, w, h);
		if(!td) return 0;
		memcpy(td, data, 4*w*h);
		pHGE->Texture_Unlock(htex);
		if(htex) {
			SoraTexture* tex = new SoraTexture;
			tex->mTextureID = static_cast<SoraHandle>(htex);
			tex->mTextureWidth		= w;
			tex->mTextureHeight		= h;
			tex->mOriginalHeight	= h;
			tex->mOriginalWidth		= w;
			return tex;
		}
		return 0;
	}

	uint32* SoraHGERenderer::textureLock(SoraTexture* ht) {
		return (uint32*)pHGE->Texture_Lock(ht->mTextureID, false, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight);
	}

	void SoraHGERenderer::textureUnlock(SoraTexture* h) {
		pHGE->Texture_Unlock(h->mTextureID);
	}

	void SoraHGERenderer::releaseTexture(SoraTexture* tex) {
		if(tex) {
			if(tex->mTextureID) {
				pHGE->Texture_Free(static_cast<HTEXTURE>(tex->mTextureID));
			}
			delete tex;
			tex = 0;
		}
	}

	void SoraHGERenderer::renderQuad(SoraQuad& quad) {
		hgeQuad hquad;
		if(quad.tex) {
			memcpy(hquad.v, quad.v, sizeof(hgeVertex)*4);
			hquad.tex = ((SoraTexture*)quad.tex)->mTextureID;
			hquad.blend = quad.blend;
		}
		else {
			hquad.tex = 0;
			memcpy(hquad.v, quad.v, sizeof(hgeVertex)*4);
			hquad.blend = quad.blend;
		}
		
		pHGE->Gfx_RenderQuad(&hquad);
		
		if(currShader)
			pHGE->_render_batch();
	}

	int32 SoraHGERenderer::_modeToDXMode(int32 mode) {
		switch(mode) {
			case Line: return HGEPRIM_LINES;
			case LineLoop: return HGEPRIM_LINE_LOOP;
			case Triangle: return HGEPRIM_TRIPLES;
			case TriangleFan: return HGEPRIM_TRIPLES_FAN;
			case TriangleStrip: return HGEPRIM_TRIPLES_STRIP;
			case Quad: return HGEPRIM_QUADS;
		}
		return HGEPRIM_TRIPLES;
	}

	void SoraHGERenderer::renderWithVertices(SoraTexture* tex, int32 blendMode, SoraVertex* vertices, uint32 size, RenderMode mode) {
		int32 maxSize;
		hgeVertex* vtxBuffer = pHGE->Gfx_StartBatch(_modeToDXMode(mode), tex!=0?tex->mTextureID:0, blendMode, &maxSize);
		int32 leftSize = size;
		while(leftSize > maxSize && vtxBuffer) {
			memcpy(vtxBuffer, &vertices[size-leftSize], sizeof(hgeVertex)*maxSize);
			pHGE->Gfx_FinishBatch(maxSize);
//			pHGE->_render_batch();
			hgeVertex* vtxBuffer = pHGE->Gfx_StartBatch(_modeToDXMode(mode), tex!=0?tex->mTextureID:0, blendMode, &maxSize);
			leftSize -= maxSize;
		}
		memcpy(vtxBuffer, &vertices[size-leftSize], sizeof(hgeVertex)*leftSize);
		pHGE->Gfx_FinishBatch(leftSize);
		pHGE->_render_batch();
	}

	bool SoraHGERenderer::isActive() {
		return pHGE->System_GetState(HGE_ACTIVE);
	}

	void SoraHGERenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
		pHGE->Gfx_SetClipping(x, y, w, h);
	}

	void SoraHGERenderer::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		pHGE->Gfx_SetTransform(x, y, dx, dy, rot, hscale, vscale);
	}

	void SoraHGERenderer::setTransformMatrix(const SoraMatrix4& mat) {
		pHGE->Gfx_SetTransformMatrix(&mat.x[0]);
	}
	
	SoraMatrix4 SoraHGERenderer::getTransformMatrix() const {
		return SoraMatrix4(pHGE->Gfx_GetTransformMatrix());
	}
	
	void SoraHGERenderer::setTransformWindowSize(float w, float h) {
		return;
	}

	SoraHandle SoraHGERenderer::createTarget(int width, int height, bool zbuffer) {
		return pHGE->Target_Create(width, height, zbuffer);
	}

	void SoraHGERenderer::freeTarget(SoraHandle t) {
		pHGE->Target_Free(t);
	}

	SoraHandle SoraHGERenderer::getTargetTexture(SoraHandle t) {
		HTEXTURE tex = pHGE->Target_GetTexture(t);
		TARGET_TEX_MAP::iterator itTex = mTargetTextures.find(tex);
		if(itTex != mTargetTextures.end())
			return itTex->second;
		
		SoraTexture* ptex = new SoraTexture(tex, pHGE->Texture_GetWidth(tex),
												pHGE->Texture_GetHeight(tex),
												pHGE->Texture_GetWidth(tex, true),
												pHGE->Texture_GetHeight(tex, true));
		mTargetTextures[tex] = (SoraHandle)ptex;
		return (SoraHandle)ptex;
	}

	StringType SoraHGERenderer::videoInfo() const {
		StringType str = pHGE->getDeviceInfo();
		return str;
	}

	SoraHandle SoraHGERenderer::getVideoDeviceHandle() {
		return pHGE->getVideoDeviceHandle();
	}
    
    void SoraHGERenderer::snapshot(const StringType& path) {
        pHGE->System_Snapshot(path.c_str());
    }
    
    void SoraHGERenderer::setViewPoint(float x, float y, float z) {
        
    }

	void SoraHGERenderer::setIcon(const StringType& icon) {
		pHGE->System_SetState(HGE_ICON, icon.c_str());
	}

	void SoraHGERenderer::setCursor(const StringType& cursor) {
		pHGE->System_SetState(HGE_CURSOR, cursor.c_str());
	}

	void SoraHGERenderer::showMouse(bool bShow) {
		pHGE->System_SetState(HGE_HIDEMOUSE, !bShow);
	}
    
    void SoraHGERenderer::renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z) {
		sora::SoraQuad quad;
		
		quad.tex = NULL;
		
        SoraVector p1(x1, y1);
        SoraVector p2(x2, y2);
        
        // Compute the extrusion direction
        SoraVector normal = p1.normal(p2);
        normal *= width / 2;
        
        SoraVector q1 = p1 - normal;
        SoraVector q2 = p2 - normal;
        SoraVector q3 = p2 + normal;
        SoraVector q4 = p1 + normal;
        
        quad.v[0].x = q1.x; quad.v[0].y = q1.y;
        quad.v[1].x = q2.x; quad.v[1].y = q2.y;
        quad.v[2].x = q3.x; quad.v[2].y = q3.y;
        quad.v[3].x = q4.x; quad.v[3].y = q4.y;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
            quad.v[i].col = color;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
	}
    
    void SoraHGERenderer::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
        sora::SoraQuad quad;
		
		quad.tex = NULL;
		
		quad.v[0].x   = x1;
		quad.v[0].y   = y1;
		quad.v[0].col = color;
		
		quad.v[1].x   = x2;
		quad.v[1].y   = y1;
		quad.v[1].col = color;
		
		quad.v[2].x   = x2;
		quad.v[2].y   = y2;
		quad.v[2].col = color;
		
		quad.v[3].x   = x1;
		quad.v[3].y   = y2;
		quad.v[3].col = color;
		
		int i;
		for (i = 0; i < 4; ++i) {
			quad.v[i].z = z;
		}
		
		quad.blend = BLEND_DEFAULT;
		
		renderQuad(quad);
    }
	
	void SoraHGERenderer::renderBox(float x1, float y1, float x2, float y2, uint32 color, float lineWidth, float z) {
		renderLine(x1, y1, x2, y1+1.f, color, lineWidth, z);
		renderLine(x2, y1, x2+1.f, y2, color, lineWidth, z);
		renderLine(x2, y2, x1, y2+1.f, color, lineWidth, z);
		renderLine(x1, y2, x1+1.f, y1, color, lineWidth, z);
	}
	
	void SoraHGERenderer::getDesktopResolution(int* w, int *h) {
		DWORD dwWidth = GetSystemMetrics(SM_CXSCREEN);
		DWORD dwHeight = GetSystemMetrics(SM_CYSCREEN);
		*w = dwWidth;
		*h = dwHeight;	
	}

	void SoraHGERenderer::setQueryVideoModeCallback(QueryVideoMode func) {
		DEVMODE mode;
		mode.dmSize = sizeof(DEVMODE);
		int n = 0;

		while(EnumDisplaySettings(0, n, &mode) != 0) {
			func(mode.dmPelsWidth, mode.dmPelsHeight);
			++n;
		}
	}

	void SoraHGERenderer::multTransformMatrix(const SoraMatrix4& matrix) {
		pHGE->Gfx_MultTransformMatrix(&matrix.x[0]);
	}

	void SoraHGERenderer::setProjectionMatrix(const SoraMatrix4& matrix) {
		pHGE->Gfx_SetProjectionMatrix(&matrix.x[0]);
	}

    SoraMatrix4 SoraHGERenderer::getProjectionMatrix() const {
		return pHGE->Gfx_GetProjectionMatrix();
	}
		
	void SoraHGERenderer::setRenderState(RenderStateType type, int32 param) {
		pHGE->Gfx_SetRenderStateParam((int)type, (int)param);
	}

    int32 SoraHGERenderer::getRenderState(RenderStateType type) const {
		return (int32)pHGE->Gfx_GetRenderStateParam((int)type);
	}

	void SoraHGERenderer::switchTo2D() {
        pHGE->ResetProjectionMatrix(0.f, 1.f);
		setTransform();
    }
    
    void SoraHGERenderer::switchTo3D() {
        pHGE->ResetProjectionMatrix(-1000.f, 1000.f);
		setTransform();
    }

#ifdef SORA_AUTOMATIC_REGISTER
	SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerRenderSystem(new SoraHGERenderer));
#endif

} // namespace sora