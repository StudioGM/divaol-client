/*
 *  SoraOGLRenderTarget.h
 *  SoraPureCore
 *
 *  Created by Robert Bu on 11/19/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_OGL_RENDER_TARGET_H_
#define SORA_OGL_RENDER_TARGET_H_

#if defined(OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include "glfw/GL/glfw.h"

#include "SoraPlatform.h"

namespace sora {

    class SoraRenderTargetOG {
        GLuint frameBuffer;
        GLuint depthBuffer;
        GLuint glTex;
        int32 w, h;
        SoraHandle tex;
        bool zbuffer;
        int32 err;
        
    public:
        SoraRenderTargetOG(int32 _w, int32 _h, bool _zbuffer);
        ~SoraRenderTargetOG();
        
        virtual void attachToRender();
        virtual void detachFromRender();
        
        int32 getWidth() const { return w; }
        int32 getHeight() const { return h; }
        
        SoraHandle getTexture() const { return tex; }
    };

} // namespace sora

#endif
