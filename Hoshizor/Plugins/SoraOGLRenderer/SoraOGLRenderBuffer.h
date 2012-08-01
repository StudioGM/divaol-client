//
//  SoraOGLRenderBuffer.h
//  Sora
//
//  Created by Robert Bu on 11/13/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraOGLRenderBuffer_h
#define Sora_SoraOGLRenderBuffer_h

#include "SoraPlatform.h"
#include "SoraRenderBuffer.h"

#include "glfw/GL/glfw.h"

namespace sora {
    
    class SoraOGLVertexBuffer: public SoraRenderBuffer {
    public:
        SoraOGLVertexBuffer(SoraRenderBuffer::Access access, 
                            SoraRenderBuffer::Usage usage, 
                            uint32 desired_count,
                            void* initData,
                            const SoraVertexFormat& format);
        
        virtual ~SoraOGLVertexBuffer();
        
        virtual void* map();
        virtual void unmap();
        
        virtual void active();
        
        SoraVertexFormat& format();
        const SoraVertexFormat& format() const;
        
        uint32 count() const;
        void resize(uint32 desired_count);
        
    private:
        GLuint mId;
        
        uint32 mCount;
        SoraVertexFormat mFormat;
    };
    
    class SoraOGLIndexBuffer: public SoraRenderBuffer {
    public:
        SoraOGLIndexBuffer(SoraRenderBuffer::Access acess, 
                           SoraRenderBuffer::Usage usage, 
                           uint32 desired_count,
                           void* initData);
        
        virtual ~SoraOGLIndexBuffer();
        
        virtual void* map();
        virtual void unmap();
        
        virtual void active();
        
        uint32 count() const;
        void resize(uint32 desired_count);
        
    private:
        GLuint mId;
        
        uint32 mCount;
    };
}

#endif
