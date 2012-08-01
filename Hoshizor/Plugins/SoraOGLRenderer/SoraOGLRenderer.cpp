/*
 *  SoraOGLRenderer.cpp
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/23/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraOGLRenderer.h"

#include "SoraLogger.h"

#include "SoraStringConv.h"
#include "SoraFileUtility.h"
#include "SoraMath.h"
#include "SoraColor.h"

#include "SoraCore.h"
#include "SoraOGLKeyPoll.h"
#include "SoraEnvValues.h"
#include "SoraWindowInfo.h"
#include "SoraTexture.h"
#include "SoraVertex.h"

#include "SoraOGLRenderBuffer.h"

#include "SoraInfiniteRendererCallback.h"

#ifdef SORA_USE_SHADER
#include "SoraShader/SoraCGGLShader.h"
#endif

#include "glfw/GL/glfw.h"
#include "soil/SOIL.h"

#ifdef OS_OSX
#include "OSXIconWrapper.h"
#endif

#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib, "glfw.lib")
#endif

#include "Convert.h"

const uint32 DEFAULT_VERTEX_BUFFER_SIZE = 512;

#include "util/SoraArray.h"

namespace sora{
    
    static SoraArray<SoraVertex> VertexBuffer;

	SoraOGLRenderer::SoraOGLRenderer() {
		pCurTarget = 0;
		uGLShaderProgram = 0;

		bShaderAvailable = _glShaderCheck();

		mCurrTexture = -1;

		currShader = 0;
		iFrameStart = 1;
		CurBlendMode = 0;
        
        VertexBuffer.reserve(DEFAULT_VERTEX_BUFFER_SIZE);
	}

	SoraOGLRenderer::~SoraOGLRenderer() {
	}

	void SoraOGLRenderer::shutdown() {
		std::list<SoraRenderTargetOG*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			delete (*itt);
			++itt;
		}
		liTargets.clear();
	}

    static void InitPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
    {
        GLfloat xmin, xmax, ymin, ymax;

        ymax = zNear * (GLfloat)tan(fovy * 3.1415962f / 360.0);
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;

        glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
    }

	void SoraOGLRenderer::_glInitialize() {
		glShadeModel(GL_SMOOTH);                    // Enables Smooth Shading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearStencil(0);                          // clear stencil buffer

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);                       // The Type Of Depth Test To Do
		glClearDepth(1.f);                          // Depth Buffer Setup

        glEnable(GL_SCISSOR_TEST);
        //glDepthMask(GL_FALSE);

  //     glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //     glEnable(GL_COLOR_MATERIAL);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

		glDisable(GL_DITHER);
        glDisable(GL_FOG);
        
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
        
        glEnable(GL_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);

        
        glLineWidth(1.0f);
        
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        
		SoraString info("OpenGL Version=");
		info += (char*)glGetString(GL_VERSION);
		SET_ENV_STRING("RENDERER_DRIVER", info);
    }

	void SoraOGLRenderer::_glBeginFrame() {
	}

	void SoraOGLRenderer::_glEndFrame() {
		glFlush();
		glfwSwapBuffers();
	}

	void SoraOGLRenderer::beginFrame() {
		iFrameStart = 1;
	}

	void SoraOGLRenderer::endFrame() {
		_glEndFrame();
	}

	void SoraOGLRenderer::_glSetProjectionMatrix(int32 w, int32 h) {
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();
		glOrtho(0.f, w, h, 0.f, 0.f, 1.f);
	}

	void SoraOGLRenderer::applyTransform() {
        if(!pCurTarget) {
            glViewport(0, 0,
                       _oglWindowInfo.width,
                       _oglWindowInfo.height);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.f,
                    _oglWindowInfo.width,
                    _oglWindowInfo.height
                    , 0.f, 0.f, 1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates


        } else {
            glViewport(0,  0,
                       pCurTarget->getWidth(),
                       pCurTarget->getHeight());
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0,
                    pCurTarget->getWidth(),
                    0
                    , pCurTarget->getHeight(), 0.f, 1.f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            glTranslatef(-_oglWindowInfo.x, -_oglWindowInfo.y, 0.f);
            glRotatef(_oglWindowInfo.rot, -0.f, 0.f, 1.f);
            glScalef(_oglWindowInfo.hscale, _oglWindowInfo.vscale, 1.0f);//Transformation follows order scale->rotation->displacement
            glTranslatef(_oglWindowInfo.x+_oglWindowInfo.dx, _oglWindowInfo.y+_oglWindowInfo.dy, 0.f); //Set Center Coodinates
        }
	}

	void SoraOGLRenderer::_glBeginScene(uint32 color, SoraHandle t, bool clear) {
		int32 width = _oglWindowInfo.width;
		int32 height = _oglWindowInfo.height;

		if(t) {
			pCurTarget = (SoraRenderTargetOG*)t;
			width = pCurTarget->getWidth();
			height = pCurTarget->getHeight();

            pCurTarget->attachToRender();
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
			CurBlendMode = 0;
            glClearColor(CGETR(color)/255.0f, CGETG(color)/255.0f, CGETB(color)/255.0f, CGETA(color)/255.0f);
			
			if(clear)
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			else
				glClear(GL_DEPTH_BUFFER_BIT);
			
        } else {
            glClearColor(CGETR(color)/255.0f, CGETG(color)/255.0f, CGETB(color)/255.0f, CGETA(color)/255.0f);
            
            if(clear)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            else
                glClear(GL_DEPTH_BUFFER_BIT);
        }

	}

	void SoraOGLRenderer::_glEndScene() {
		flush();
		if(pCurTarget != NULL) {
            glFlush();

            pCurTarget->detachFromRender();
            pCurTarget = 0;
            
            setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);
            setClipping();
        } else
            iFrameStart = 0;
	}

	void SoraOGLRenderer::_glSetBlendMode(int32 blend) {
		if(blend != CurBlendMode)
			flush();
		
		glEnable(GL_BLEND); // Enable Blending
		
		if((blend & BLEND_SRCALPHA) != (CurBlendMode & BLEND_SRCALPHA))
			if(blend & BLEND_SRCALPHA)
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);

		if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND)) {
			if(blend & BLEND_ALPHABLEND) {
#ifndef OS_WIN32
                if(pCurTarget)
                    // alpha trick with FBO transparent background
                    // see http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=257628

                    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

                else 
#endif
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      //Alpha blending
            }
			else
				glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Addictive
		}

		if((blend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE)) {
			if(blend & BLEND_ZWRITE) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
		}

		if((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD)) {
			if(blend & BLEND_COLORADD) 
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
			else 
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		CurBlendMode = blend;
	}

	bool SoraOGLRenderer::update() {
#ifdef OS_OSX
        osx_activeCurrentCursor();
#endif
		return false;
	}

	void SoraOGLRenderer::start(SoraTimer* timer) {
		sora::g_timer = timer;
		_glInitialize();
		setTransform(0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f);

		while(true) {
			void_updateFrame();
		}
	}

	void SoraOGLRenderer::beginScene(uint32 color, SoraHandle target, bool clear) {
		_glBeginScene(color, target, clear);
	}

	void SoraOGLRenderer::endScene() {
		currShader = 0;
		_glEndScene();
	}

	SoraWindowHandle SoraOGLRenderer::createWindow(SoraWindowInfoBase* windowInfo) {
		glfwInit();
        
        int FSAASamples = 0;
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, FSAASamples);
        
		if(!glfwOpenWindow(windowInfo->getWindowWidth(), windowInfo->getWindowHeight()
                           , 8, 8, 8, 8, 16, 0, windowInfo->isWindowed()?GLFW_WINDOW:GLFW_FULLSCREEN)) {
            if(FSAASamples == 0) {
                glfwTerminate();
                THROW_SORA_EXCEPTION(SystemException, "Error initializing GLFW");
            } else {
                glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 0);
                if(!glfwOpenWindow(windowInfo->getWindowWidth(), windowInfo->getWindowHeight()
                                  , 8, 8, 8, 8, 16, 0, windowInfo->isWindowed()?GLFW_WINDOW:GLFW_FULLSCREEN)) {
                    glfwTerminate();
                    THROW_SORA_EXCEPTION(SystemException, "Error initializing GLFW");
                }
            }
        }
        if(glfwGetWindowParam(GLFW_FSAA_SAMPLES) != FSAASamples) {
            THROW_SORA_EXCEPTION(SystemException, vamssg("Error creating window with FSAA sample = %d", FSAASamples));
        }
        
        if(FSAASamples != 0)
            glEnable(GL_MULTISAMPLE_ARB);
        
		glfwSetWindowTitle(windowInfo->getWindowName().c_str());
		if(windowInfo->getWindowPosX() != 0.f && windowInfo->getWindowPosY() != 0.f)
			glfwSetWindowPos(windowInfo->getWindowPosX(), windowInfo->getWindowPosY());
		glfwSetWindowCloseCallback(int_exitFunc);
		glfwSetKeyCallback(glfwKeyCallback);

        mainWindow = windowInfo;
		_oglWindowInfo.width = windowInfo->getWindowWidth();
		_oglWindowInfo.height = windowInfo->getWindowHeight();
		windowWidth = _oglWindowInfo.width;
		windowHeight = _oglWindowInfo.height;

		_glInitialize();
		_glSetProjectionMatrix(windowInfo->getWindowWidth(), windowInfo->getWindowHeight());

		if(mainWindow->hideMouse())
			glfwDisable(GLFW_MOUSE_CURSOR);

		bFullscreen = !windowInfo->isWindowed();
        
        if(windowInfo->getIcon() != NULL)
            setIcon(mainWindow->getIcon());
        if(windowInfo->getCursor() != NULL)
            setCursor(mainWindow->getCursor());

		return (SoraHandle)windowInfo;

		return 0;
	}

	void SoraOGLRenderer::setWindowSize(int32 w, int32 h) {
		glfwSetWindowSize(w, h);
		
		_oglWindowInfo.width = w;
		_oglWindowInfo.height = h;
		windowWidth = _oglWindowInfo.width;
		windowHeight = _oglWindowInfo.height;
		
		_glSetProjectionMatrix(w, h);
	}

	void SoraOGLRenderer::setWindowTitle(const StringType& title) {
		glfwSetWindowTitle(ws2s(title).c_str());
	}

	void SoraOGLRenderer::setWindowPos(int32 px, int32 py) {
		glfwSetWindowPos(px, py);
	}

	void SoraOGLRenderer::setFullscreen(bool flag) {
		if(bFullscreen == flag)
			return;

		endScene();

		bFullscreen = flag;
		glfwCloseWindow();
		glfwOpenWindow(mainWindow->getWindowWidth(), mainWindow->getWindowHeight(),
					   8, 8, 8, 8, 16, 0,
					   bFullscreen==true?GLFW_FULLSCREEN:GLFW_WINDOW);
		glfwSetWindowTitle(mainWindow->getWindowName().c_str());
		glfwSetWindowCloseCallback(int_exitFunc);

		_glInitialize();
		_glSetProjectionMatrix(_oglWindowInfo.width, _oglWindowInfo.height);

		beginScene();
	}

	bool SoraOGLRenderer::isFullscreen() {
		return bFullscreen;
	}

	SoraHandle SoraOGLRenderer::getVideoDeviceHandle() {
		return (SoraHandle)this;
	}

	int32 SoraOGLRenderer::_glTextureGetWidth(int32 tex, bool bOriginal) {
		GLint w;
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return w;
	}

	int32 SoraOGLRenderer::_glTextureGetHeight(int32 tex, bool bOriginal) {
		GLint h;
		glBindTexture(GL_TEXTURE_2D, tex);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
		glBindTexture(GL_TEXTURE_2D, mCurrTexture);
		return h;
	}

	SoraHandle SoraOGLRenderer::getMainWindowHandle() { 
		return (SoraHandle)mainWindow;
	}

	SoraWindowInfoBase* SoraOGLRenderer::getMainWindow() { 
		return mainWindow;
	}

	void SoraOGLRenderer::bindTexture(SoraTexture* tex) {
        if(!tex) {
            mCurrTexture = 0;
            glBindTexture(GL_TEXTURE_2D, 0);
            
            return;
        }
		else if (mCurrTexture != tex->mTextureID) {
			flush();

			glBindTexture(GL_TEXTURE_2D, tex->mTextureID);
			mCurrTexture = tex->mTextureID;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void SoraOGLRenderer::flush() {
        if(VertexBuffer.size() == 0)
            return;
        
		glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, sizeof(SoraVertex), &VertexBuffer[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(SoraVertex), &(VertexBuffer[0].tx));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SoraVertex), &(VertexBuffer[0].col));
        
        glDrawArrays(CurDrawMode, 0, VertexBuffer.size());
        
        VertexBuffer.reset();
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
	}

	SoraTexture* SoraOGLRenderer::createTexture(const StringType& sTexturePath, bool bMipmap) {
		// to do
		return 0;
	}

	SoraTexture* SoraOGLRenderer::createTextureFromMem(void* ptr, uint32 size, bool bMipmap) {
		SoraHandle  texid;
		int w, h, channels;

		texid = SOIL_load_OGL_texture_and_info_from_memory((const unsigned char* const)ptr,
														  size,
														  SOIL_LOAD_AUTO,
														  SOIL_CREATE_NEW_ID,
														  (bMipmap ? SOIL_FLAG_MIPMAPS : 0)
														  | /*SOIL_FLAG_POWER_OF_TWO | */SOIL_FLAG_TEXTURE_REPEATS/*| SOIL_FLAG_INVERT_Y*/,
														  &w, &h, &channels);

		SoraTexture* tex = new SoraTexture(texid,
										   _glTextureGetWidth(texid),
										   _glTextureGetHeight(texid),
										   w,
										   h);
		return tex;
	}

	SoraTexture* SoraOGLRenderer::createTextureWH(int w, int h) {
		GLuint texId;

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		uint8* texData = new uint8[w*h*sizeof(uint32)];
		memset(texData, 0, w*h*sizeof(uint32));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		delete texData;

		return new SoraTexture(texId, w, h, w, h);
	}

	SoraTexture* SoraOGLRenderer::createTextureFromRawData(unsigned int* data, int32 w, int32 h) {
        GLuint texId;

        glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

		if(!texId) {
			log_error("Error creating texture");
			return 0;
		}
		SoraTexture* tex = new SoraTexture(texId,
										   _glTextureGetWidth(texId),
										   _glTextureGetHeight(texId),
										   w,
										   h);
		return tex;
	}

	uint32* SoraOGLRenderer::textureLock(SoraTexture* ht) {
        if(ht->mTexData == 0) {
            ht->mTexData = (uint32*)sora_malloc(ht->mOriginalWidth * ht->mOriginalHeight * sizeof(uint32));
            sora_assert(ht->mTexData);
        
            memset(ht->mTexData, 0xff, ht->mOriginalWidth * ht->mOriginalHeight * sizeof(uint32));
		}
        glEnable(GL_TEXTURE_2D);
        
        GLint PreviousTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);
        
        glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ht->mTexData);
        if(glGetError() != GL_NO_ERROR) {
            log_error("SoraOGLRenderer: error when locking texture");
        }
        
        glBindTexture(GL_TEXTURE_2D, PreviousTexture);
        return ht->mTexData;
	}

	void SoraOGLRenderer::textureUnlock(SoraTexture* ht) {
		if(ht->mTexData != 0) {
			glEnable(GL_TEXTURE_2D);

            GLint PreviousTexture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture);

            glBindTexture(GL_TEXTURE_2D, ht->mTextureID);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ht->mOriginalWidth, ht->mOriginalHeight, GL_RGBA, GL_UNSIGNED_BYTE, ht->mTexData);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindTexture(GL_TEXTURE_2D, PreviousTexture);

			sora_free(ht->mTexData);
			ht->mTexData = 0;
		}
	}

	void SoraOGLRenderer::releaseTexture(SoraTexture* tex) {
		glDeleteTextures(1, (const GLuint*)&tex->mTextureID);
		delete tex;
		tex = 0;
	}
    
    void SoraOGLRenderer::setRenderState(RenderStateType type, int32 param) {
        switch(type) {
            case TextureWrap0:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, RenderParamToGLParam(param));
                break;
            case TextureWrap1:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, RenderParamToGLParam(param));
                break;
            case BlendMode:
                _glSetBlendMode(param);
                break;
        }
    }
    
    int32 SoraOGLRenderer::getRenderState(RenderStateType) const {
        
    }
	
	void SoraOGLRenderer::renderWithVertices(SoraTexture* tex, int32 blendMode,  SoraVertex* vertices, uint32 vsize, RenderMode mode) {		
		flush();
		
		glEnable(GL_TEXTURE_2D);
		if(tex) {
            bindTexture(tex);
		} else {
            bindTexture(0);
        }
		_glSetBlendMode(blendMode);
		CurDrawMode = RenderModeToGLMode(mode);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, sizeof(SoraVertex), &vertices[0].x);
        glTexCoordPointer(2, GL_FLOAT, sizeof(SoraVertex), &(vertices[0].tx));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(SoraVertex), &(vertices[0].col));
        
        glDrawArrays(CurDrawMode, 0, vsize);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
	}

	void SoraOGLRenderer::renderQuad(SoraQuad& quad) {
		glEnable(GL_TEXTURE_2D);
		if(quad.tex) {
			bindTexture(quad.tex);
		} else {
            flush();
            bindTexture(0);
        }
		_glSetBlendMode(quad.blend);
		CurDrawMode = GL_QUADS;
   
        SoraVertex tmp = quad.v[0];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        
        tmp = quad.v[1];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
        
        tmp = quad.v[2];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
       
        tmp = quad.v[3];
        tmp.col = CARGB(CGETA(tmp.col), CGETB(tmp.col), CGETG(tmp.col), CGETR(tmp.col));
        VertexBuffer.append(tmp);
		
		if(currShader)
			flush();
		if(!quad.tex)
			flush();
	}

	bool SoraOGLRenderer::isActive() {
		return glfwGetWindowParam(GLFW_ACTIVE) ? true : false;
	}

	void SoraOGLRenderer::setClipping(int32 x, int32 y, int32 w, int32 h) {
        if(w == 0 && h == 0) {
            w = _oglWindowInfo.width;
            h = _oglWindowInfo.height;
        }
        if(!pCurTarget) {
            glScissor(x, _oglWindowInfo.height-(y+h), w, h);
        }
        else {
            glScissor(x, y, w, h);
        }
	}
    
    void SoraOGLRenderer::setVerticalSync(bool flag) {
        if(flag)
            glfwSwapInterval(1); // to device refresh rate
        else
            glfwSwapInterval(0);
    }

	void SoraOGLRenderer::setTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale) {
		_oglWindowInfo.x		=	x;
		_oglWindowInfo.y		=	y;
		_oglWindowInfo.dx		=	dx;
		_oglWindowInfo.dy		=	dy;
		_oglWindowInfo.rot		=	RAD_DGR(rot);
		_oglWindowInfo.hscale	=	hscale!=0.f?hscale:1.f;
		_oglWindowInfo.vscale	=	vscale!=0.f?hscale:1.f;

        flush();
        applyTransform();
	}
    
    void SoraOGLRenderer::setTransformMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&matrix.x[0]);
    }
    
    SoraMatrix4 SoraOGLRenderer::getTransformMatrix() const {        
        glMatrixMode(GL_MODELVIEW);
        float matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, &matrix[0]);
        return SoraMatrix4(matrix);
    }
    
    void SoraOGLRenderer::setProjectionMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&matrix.x[0]);
    }
    
    void SoraOGLRenderer::multTransformMatrix(const SoraMatrix4& matrix) {
        flush();
        
        glMatrixMode(GL_MODELVIEW);
        glMultMatrixf(&matrix.x[0]);
    }
    
    SoraMatrix4 SoraOGLRenderer::getProjectionMatrix() const {
        glMatrixMode(GL_PROJECTION);
        float matrix[16];
        glGetFloatv(GL_PROJECTION_MATRIX, &matrix[0]);
        return SoraMatrix4(matrix);
    }

    void SoraOGLRenderer::switchTo2D() {
        if(!pCurTarget)
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, _oglWindowInfo.width, _oglWindowInfo.height, 0.f, 0.f, 1.f));
        else 
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, pCurTarget->getWidth(), 0.f, pCurTarget->getHeight(), 0.f, 1.f));
        
        setTransform();
        
        glDisable(GL_DITHER);
        glDisable(GL_FOG);
        
		glDisable(GL_LIGHTING);
		glDisable(GL_CULL_FACE);
    }
    
    void SoraOGLRenderer::switchTo3D() {
        if(!pCurTarget)
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, _oglWindowInfo.width, _oglWindowInfo.height, 0.f, -1000.f, 1000.f));
        else 
            setProjectionMatrix(sora::SoraMatrix4::OrthoMat(0.f, pCurTarget->getWidth(), 0.f, pCurTarget->getHeight(), -1000.f, 1000.f));
    }
    
	SoraHandle SoraOGLRenderer::createTarget(int width, int height, bool zbuffer) {
		SoraRenderTargetOG* t = new SoraRenderTargetOG(width, height, zbuffer);
		liTargets.push_back((SoraRenderTargetOG*)t);
		return (SoraHandle)t;
	}

	void SoraOGLRenderer::freeTarget(SoraHandle t) {
		SoraRenderTargetOG* pt = (SoraRenderTargetOG*)t;
		if(!pt) return;
		std::list<SoraRenderTargetOG*>::iterator itt = liTargets.begin();
		while(itt != liTargets.end()) {
			if((*itt) == pt) {
				delete pt;
				liTargets.erase(itt);
				break;
			}
			++itt;
		}
	}

	SoraHandle SoraOGLRenderer::getTargetTexture(SoraHandle t) {
		SoraRenderTargetOG* pt = (SoraRenderTargetOG*)t;
        assert(pt != NULL);

		return pt->getTexture();
	}

	bool SoraOGLRenderer::_glVersionCheck() {
		int majorVersion, minorVersion, rev;
		glfwGetVersion(&majorVersion, &minorVersion, &rev);
		return majorVersion >= 2;
	}

	bool SoraOGLRenderer::_glShaderCheck() {
		// to do
		// check extensions
		// return glfwExtensionSupported("GL_ARB_fragment_shade");
		return _glVersionCheck();
	}


	inline bool SoraOGLRenderer::_glCheckError() {
		return glGetError() != GL_NO_ERROR;
	}

	StringType SoraOGLRenderer::videoInfo() const {
		StringType info = vamssg("OpenGL Version: %s Vender: %s GLSL Version: %s", 
                                 (char*)glGetString(GL_VERSION),
                                 (char*)glGetString(GL_VENDOR),
                                 (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

		return info;
	}
    
    SoraRenderSystem::RenderSystemType SoraOGLRenderer::renderSystemType() const {
        return OpenGL;
    }

    SoraShaderContext* SoraOGLRenderer::createShaderContext() {
#ifdef SORA_USE_SHADER
   //     return new SoraCGGLShaderContext;
#endif
		return NULL;
    }

	void SoraOGLRenderer::attachShaderContext(SoraShaderContext* context) {
		flush();
		
		currShader = context;
		if(!currShader->attachShaderList()) {
			log_error("SoraOGLRenderer: Error attaching shader list");
		}
	}

	void SoraOGLRenderer::detachShaderContext() {
		if(!currShader)
			return;
		
		if(!currShader->detachShaderList())
			log_error("SoraOGLRenderer: Error detaching shader list");
		flush();
		currShader = 0;
	}

    void SoraOGLRenderer::snapshot(const StringType& path) {
        SOIL_save_screenshot(path.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, SoraCore::Ptr->getScreenWidth(), SoraCore::Ptr->getScreenHeight());
    }

    void SoraOGLRenderer::setIcon(const StringType& icon) {
#ifdef OS_OSX
        osx_setDockIcon(icon);
#endif // OS_OSX
    }
    
    void SoraOGLRenderer::setCursor(const StringType& cursor) {
#ifdef OS_OSX
        osx_setAppCursor(cursor);
#endif
    }
    
    void SoraOGLRenderer::renderLine(float x1, float y1, float x2, float y2, uint32 color, float width, float z) {
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
    
    void SoraOGLRenderer::fillBox(float x1, float y1, float x2, float y2, uint32 color, float z) {
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
	
	void SoraOGLRenderer::renderBox(float x1, float y1, float x2, float y2, uint32 color, float lineWidth, float z) {
		renderLine(x1, y1, x2, y1+1.f, color, lineWidth, z);
		renderLine(x2, y1, x2+1.f, y2, color, lineWidth, z);
		renderLine(x2, y2, x1, y2+1.f, color, lineWidth, z);
		renderLine(x1, y2, x1+1.f, y1, color, lineWidth, z);
	}
	
	 void SoraOGLRenderer::getDesktopResolution(int* w, int* h) {
        GLFWvidmode mode;
        glfwGetDesktopMode(&mode);
        *w = mode.Width;
        *h = mode.Height;
    }
    
    void SoraOGLRenderer::setQueryVideoModeCallback(QueryVideoMode func) {
        if(func) {
            // max 24?
            GLFWvidmode mode[24];
            int count = glfwGetVideoModes(&mode[0], 24);
            for(int i=0; i<count; ++i) {
                func(mode[i].Width, mode[i].Height);
            }
        }
    }

    SoraRenderBuffer::Ptr SoraOGLRenderer::createVertexBuffer(SoraRenderBuffer::Access accessHint,
                                                              SoraRenderBuffer::Usage usage,
                                                              uint32 desired_count,
                                                              void* initData,
                                                              const SoraVertexFormat& vertexFormat) {
        return MakeSharedPtr<SoraOGLVertexBuffer>(accessHint, 
                                                  usage, 
                                                  desired_count,
                                                  initData,
                                                  vertexFormat);
    }
    
    SoraRenderBuffer::Ptr SoraOGLRenderer::createIndexBuffer(SoraRenderBuffer::Access accessHint,
                                                             SoraRenderBuffer::Usage usage,
                                                             uint32 desired_count,
                                                             void* initData) {
        return MakeSharedPtr<SoraOGLIndexBuffer>(accessHint, 
                                                 usage, 
                                                 desired_count,
                                                 initData);
    }
        
    void SoraOGLRenderer::renderBuffer(SoraTexture* tex, RenderMode mode, SoraRenderBuffer::Ptr vertexBuffer, SoraRenderBuffer::Ptr indexBuffer) {
        #define BUFFER_OFFSET(i) ((char *)NULL + (i))

        sora_assert(vertexBuffer.isValid());
        
        SoraOGLVertexBuffer* ogl_vertex_buffer = dynamic_cast<SoraOGLVertexBuffer*>(vertexBuffer.get());
        if(!ogl_vertex_buffer) {
            log_error("SoraOGLRenderer: invalid vertex buffer format");
            return;
        }
        
        glEnable(GL_TEXTURE_2D);
		if(tex) {
            bindTexture(tex);
		} else {
            bindTexture(0);
        }

        SoraVertexFormat& format = ogl_vertex_buffer->format();
        vertexBuffer->active();

        if(format.checkFormat(VertexXYZ)) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, 
                            GL_FLOAT, 
                            format.totalSize(), 
                            BUFFER_OFFSET(format.offsetXYZ()));
        }
        
        if(format.checkFormat(VertexNormal)) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 
                            format.totalSize(), 
                            BUFFER_OFFSET(format.offsetNormal()));
        }
        
        if(format.checkFormat(VertexColor)) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, 
                           GL_UNSIGNED_BYTE, 
                           format.totalSize(),
                           BUFFER_OFFSET(format.offsetColor()));
        }
        
        if(format.checkFormat(VertexColor2)) {
            glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
            glSecondaryColorPointer(4, 
                                    GL_UNSIGNED_BYTE, 
                                    format.totalSize(),
                                    BUFFER_OFFSET(format.offsetColor2()));
        }
        
        if(format.checkFormat(VertexUV)) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, 
                              GL_FLOAT, 
                              format.totalSize(),
                              BUFFER_OFFSET(format.offsetUV()));
        } else {
            glDisable(GL_TEXTURE_2D);
        }
       
        if(indexBuffer.isValid()) {
            SoraOGLIndexBuffer* ogl_index_buffer = dynamic_cast<SoraOGLIndexBuffer*>(indexBuffer.get());
            if(!ogl_index_buffer) {
                log_error("SoraOGLRenderer: invalid vertex buffer format");
                return;
            }

            indexBuffer->active();
          
            glDrawRangeElements(RenderModeToGLMode(mode), 
                                0,
                                0xffffffff,
                                ogl_index_buffer->count(),
                                GL_UNSIGNED_INT,
                                BUFFER_OFFSET(0));
        } else {
            
            glDrawArrays(RenderModeToGLMode(mode), 0, ogl_vertex_buffer->count());
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_APPLE, 0);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    }
    
    void SoraOGLRenderer::fillDeviceCaps(SoraGraphicDeviceCaps& caps) {
        glGetIntegerv(GL_MAX_ELEMENTS_INDICES, (GLint*)&caps.max_indices);
        glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, (GLint*)&caps.max_vertices);
        
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&caps.max_texture_height);
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&caps.max_texture_width);
        
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, (GLint*)&caps.max_texture_cube_map_size);
        
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*)&caps.max_pixel_texture_units);
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, (GLint*)&caps.max_vertex_texture_units);
    }
    
    void SoraOGLRenderer::showMouse(bool bShow) {
        
    }
    
#ifdef SORA_AUTOMATIC_REGISTER
    SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerRenderSystem(new SoraOGLRenderer()));
#endif


} // namespace sora
