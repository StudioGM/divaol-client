/*
 *  SoraOGLRenderTarget.cpp
 *  SoraPureCore
 *
 *  Created by Robert Bu on 11/19/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */


#include "SoraOGLRenderTarget.h"
#include "SoraTexture.h"
#include "SoraCore.h"
#include "SoraLogger.h"

#ifdef WIN32
#include <GL/GL.h>
#include <GL/GLU.h>
#elif defined(OS_OSX)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>
#endif

namespace sora {

SoraRenderTargetOG::SoraRenderTargetOG(int32 _w, int32 _h, bool _zbuffer):
    w(_w), h(_h), zbuffer(_zbuffer), err(0), frameBuffer(0), depthBuffer(0) {
	GLuint gl_error;

#ifndef WIN32
		SoraTexture* objtex = (SoraTexture*)SoraCore::Instance()->createTextureWH(_w, _h);
        glTex = objtex->mTextureID;
        tex = (SoraTextureHandle)objtex;
		
		if((gl_error = glGetError()) != GL_NO_ERROR) {
			err = 1;
		}
		// create framebuffer
		glGenFramebuffersEXT(1, &frameBuffer);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
		
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, glTex, NULL);

		//create depth buffer
		if (zbuffer) {
			glGenRenderbuffersEXT(1, &depthBuffer);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
			
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
			
			glClear(GL_DEPTH_BUFFER_BIT);
		}
			
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
			err = 1;
		}
		
		
		if(!glTex || !frameBuffer)
			err = 2;
		else err = 0;
		
		glDisable(GL_TEXTURE_2D);
		
		if(err != 0)
			log_error("SoraOGLRenderTarget: Error creating Render Target");
#endif
}

SoraRenderTargetOG::~SoraRenderTargetOG() {
#ifndef WIN32
	if (tex) {
		glDeleteTextures(1, (GLuint*)(&((SoraTexture*)tex)->mTextureID));
        delete (SoraTexture*)tex;
    }

	if (frameBuffer)
		glDeleteFramebuffersEXT(1, (uint32*)(&frameBuffer));

	if (depthBuffer)
		glDeleteRenderbuffersEXT(1, (uint32*)(&depthBuffer));
#endif
}

void SoraRenderTargetOG::attachToRender() {
#ifndef WIN32

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
	GLuint gl_error;
	if((gl_error = glGetError()) != GL_NO_ERROR) {
	    log_error("SoraOGLRenderTarget: error detach rendtarget");
	}
#endif
}

void SoraRenderTargetOG::detachFromRender() {
#ifndef WIN32
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    GLuint gl_error;
    if((gl_error = glGetError()) != GL_NO_ERROR) {
	    log_error("SoraOGLRenderTarget: error detach rendtarget");
	}
#endif
}

} // namespace sora
