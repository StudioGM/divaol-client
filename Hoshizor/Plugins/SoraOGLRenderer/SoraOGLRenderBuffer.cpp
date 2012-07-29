//
//  SoraOGLRenderBuffer.cpp
//  Sora
//
//  Created by Robert Bu on 11/13/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraOGLRenderBuffer.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraOGLVertexBuffer::SoraOGLVertexBuffer(SoraRenderBuffer::Access access, 
                                             SoraRenderBuffer::Usage usage, 
                                             uint32 desired_count,
                                             void* initData,
                                             const SoraVertexFormat& format):
    SoraRenderBuffer(access, usage),
    mFormat(format) {
        
        glGenBuffers(1, &mId);
        glBindBuffer(GL_ARRAY_BUFFER, mId);

        if(initData) {
            glBufferData(GL_ARRAY_BUFFER, 
                         static_cast<GLsizeiptr>(desired_count * format.totalSize()), 
                         static_cast<GLvoid*>(initData),
                         usage == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            
        } else {
            glBufferData(GL_ARRAY_BUFFER, 
                         static_cast<GLsizeiptr>(desired_count * format.totalSize()), 
                         0,
                         usage == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        mCount = desired_count;
    }
    
    SoraOGLVertexBuffer::~SoraOGLVertexBuffer() {
        glDeleteBuffers(1, &mId);
    }
    
    void* SoraOGLVertexBuffer::map() {
        GLenum gl_acess;
        
        switch (access()) {
            case SoraRenderBuffer::ReadOnly:
                gl_acess = GL_READ_ONLY;
                break;
                
            case SoraRenderBuffer::ReadWrite:
                gl_acess = GL_READ_WRITE;
                break;
                
            case SoraRenderBuffer::WriteOnly:
                gl_acess = GL_WRITE_ONLY;
                break;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, mId);
        void* p = glMapBuffer(GL_ARRAY_BUFFER, gl_acess);
        return p;
    }
    
    void SoraOGLVertexBuffer::unmap() {
        glBindBuffer(GL_ARRAY_BUFFER, mId);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void SoraOGLVertexBuffer::active() {
        glBindBuffer(GL_ARRAY_BUFFER, mId);
    }
    
    void SoraOGLVertexBuffer::resize(uint32 desired_count) {
        glBindBuffer(GL_ARRAY_BUFFER, mId);

        glBufferData(GL_ARRAY_BUFFER, 
                     static_cast<GLsizeiptr>(desired_count * mFormat.totalSize()), 
                     0,
                     usage() == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        mCount = desired_count;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    SoraVertexFormat& SoraOGLVertexBuffer::format() {
        return mFormat;
    }
    
    const SoraVertexFormat& SoraOGLVertexBuffer::format() const {
        return mFormat;
    }
    
    uint32 SoraOGLVertexBuffer::count() const {
        return mCount;
    }
    
    SoraOGLIndexBuffer::SoraOGLIndexBuffer(SoraRenderBuffer::Access access, 
                                           SoraRenderBuffer::Usage usage, 
                                           uint32 desired_count,
                                           void* initData):
    SoraRenderBuffer(access, usage)  {
        glGenBuffers(1, &mId);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        if(initData) {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                         static_cast<GLsizeiptr>(desired_count * sizeof(uint32)), 
                         static_cast<GLvoid*>(initData),
                         usage == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        } else {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                         static_cast<GLsizeiptr>(desired_count * sizeof(uint32)), 
                         static_cast<GLvoid*>(initData),
                         usage == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        
        mCount = desired_count;
    }
    
    SoraOGLIndexBuffer::~SoraOGLIndexBuffer() {
        glDeleteBuffers(1, &mId);
    }
    
    uint32 SoraOGLIndexBuffer::count() const {
        return mCount;
    }
    
    void* SoraOGLIndexBuffer::map() {
        GLenum gl_acess;
        
        switch (access()) {
            case SoraRenderBuffer::ReadOnly:
                gl_acess = GL_READ_ONLY;
                break;
                
            case SoraRenderBuffer::ReadWrite:
                gl_acess = GL_READ_WRITE;
                break;
                
            case SoraRenderBuffer::WriteOnly:
                gl_acess = GL_WRITE_ONLY;
                break;
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_acess);
    }
    
    void SoraOGLIndexBuffer::resize(uint32 desired_count) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     static_cast<GLsizeiptr>(desired_count * sizeof(uint32)), 
                     0,   
                     usage() == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        mCount = desired_count;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    
    void SoraOGLIndexBuffer::unmap() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }
    
    void SoraOGLIndexBuffer::active() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
    }
    
} // namespace sora
