//
//  SoraVertexList.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/28/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraVertexList.h"
#include "SoraSprite.h"
#include "SoraLogger.h"
#include "SoraRenderSystem.h"
#include "SoraCore.h"

namespace sora {
    
    SoraVertexList::SoraVertexList(RenderMode vertexMode):
    mVertexMode(vertexMode),
    mBlendMode(BLEND_DEFAULT),
    mTexture(0),
    mVertexIndex(0),
    mTexCoordIndex(0),
    mColorIndex(0) {
    
    }
    
    SoraVertexList::~SoraVertexList() {
        
    }
    
    void SoraVertexList::blendMode(int32 mode) {
        mBlendMode = mode;
    }
    
    void SoraVertexList::bindTexture(SoraTextureHandle texture) {
        mTexture = texture;
    }
    
    void SoraVertexList::clear() {
        mColorIndex = 0;
        mTexCoordIndex = 0;
        mVertexIndex = 0;
        
        mVertexList.clear();
    }
    
    void SoraVertexList::vertex(const SoraVertex& vertex) {
        mVertexList.push_back(vertex);
        
        mVertexIndex++;
    }
    
    void SoraVertexList::vertexList(const SoraVertex* vertex, size_t count) {
        mVertexList.append(vertex, count);
    }
    
    void SoraVertexList::vertex2(float x, float y) {
        mVertexList.push_back(SoraVertex(x, y, 0.f, 0.f));
        
        mVertexIndex++;
    }
    
    void SoraVertexList::vertex2v(size_t count, float* v, size_t stride) {
        float* p = v;
        for(size_t i=0; i<count; ++i) {
            mVertexList.push_back(SoraVertex(*p++, *p++, 0.f, 0.f));
            
            if(stride != 0)
                p += stride;
            
            mVertexIndex++;
        }
    }
    
    void SoraVertexList::vertex3(float x, float y, float z) {
        SoraVertex vertex(x, y, 0.f, 0.f);
        vertex.z = z;
        
        mVertexList.push_back(vertex);
        mVertexIndex++;
    }
    
    uint32 SoraVertexList::size() const {
        return mVertexList.size();
    }
    
    const SoraArray<SoraVertex>& SoraVertexList::getVertexList() const {
        return mVertexList;
    }
    
    SoraArray<SoraVertex>& SoraVertexList::getVertexList() {
        return mVertexList;
    }
    
    void SoraVertexList::vertex3v(size_t count, float* v, size_t stride) {
        float* p = v;
        for(size_t i=0; i<count; ++i) {
            SoraVertex vertex(*p++, *p++, 0.f, 0.f);
            vertex.z = *p++;
            
            if(stride != 0)
                *p += stride;
            
            mVertexList.push_back(vertex);
            mVertexIndex++;
        }
    }
    
    void SoraVertexList::vertexPointer(size_t size, size_t count, float* v, size_t stride) {
        switch(size) {
            case 2: vertex2v(count, v, stride); break;
            case 3: vertex3v(count, v, stride); break;
                
            default:
                break;
        }
    }
    
    void SoraVertexList::texCoord(float x, float y) {
        if(mTexCoordIndex >= mVertexList.size()) {
            log_error("SoraVertexList: tex index invalid");
        } else {
            SoraVertex& vet = mVertexList[mTexCoordIndex];
            vet.tx = x;
            vet.ty = y;
            
            mTexCoordIndex++;
        }
    }
    
    void SoraVertexList::texCoordPointer(size_t count, float* v, size_t stride) {
        float* p = v;
        for(size_t i=0; i<count; ++i) {
            if(mTexCoordIndex >= mVertexList.size()) {
                log_error("SoraVertexList: tex index invalid");
                break;
            } else {
                SoraVertex& vet = mVertexList[mTexCoordIndex];
                vet.tx = *p++;
                vet.ty = *p++;
                
                p += stride;
                
                mTexCoordIndex++;
            }
        }
    }
    
    void SoraVertexList::color(uint32 col) {
        if(mColorIndex >= mVertexList.size()) {
            log_error("SoraVertexList: color index invalid");
        } else {
            SoraVertex& vet = mVertexList[mTexCoordIndex];
            vet.col = col;
        
            mColorIndex++;
        }
    }
    
    void SoraVertexList::color3(float r, float g, float b) {
        if(mColorIndex >= mVertexList.size()) {
            log_error("SoraVertexList: color index invalid");
        } else {
            SoraVertex& vet = mVertexList[mTexCoordIndex];
            vet.col = SoraColorRGBA::GetHWColor(r, g, b, 1.0f);
            
            mColorIndex++;
        }
    }
    
    void SoraVertexList::color4(float r, float g, float b, float a) {
        if(mColorIndex >= mVertexList.size()) {
            log_error("SoraVertexList: color index invalid");
        } else {
            SoraVertex& vet = mVertexList[mTexCoordIndex];
            vet.col = SoraColorRGBA::GetHWColor(r, g, b, a);
            
            mColorIndex++;
        }
    }
    
    void SoraVertexList::setRenderMode(RenderMode mode) {
        mVertexMode = mode;
    }
    
    void SoraVertexList::colorPointer(size_t count, float* v, size_t stride) {
        float* p = v;
        for(size_t i=0; i<count; ++i) {
            if(mColorIndex >= mVertexList.size()) {
                log_error("SoraVertexList: color index invalid");
            } else {
                SoraVertex& vet = mVertexList[mTexCoordIndex];
                vet.col = SoraColorRGBA::GetHWColor(*p++, *p++, *p++, *p++);
                
                p += stride;
                
                mColorIndex++;
            }
        }
    }
    
    void SoraVertexList::render() {
        SoraCore::Ptr->pushTransformMatrix();
        SoraCore::Ptr->getRenderSystem()->multTransformMatrix(getTransform().getTransformMatrix());
        
        this->attachShaderToRender();
        SoraCore::Ptr->renderWithVertices(mTexture, mBlendMode, mVertexList.begin(), mVertexList.size(), mVertexMode);
        this->detachShaderFromRender();
        
        SoraCore::Ptr->popTransformMatrix();
    }
    
    SoraVertexList& SoraVertexList::operator=(const SoraVertexList& rhs) {
        if(this != &rhs) {
            mVertexMode = rhs.mVertexMode;
            mBlendMode = rhs.mBlendMode;
            mTexture = rhs.mTexture;
            mVertexList = rhs.mVertexList;
            
            mVertexIndex = rhs.mVertexIndex;
            mTexCoordIndex = rhs.mTexCoordIndex;
            mColorIndex = rhs.mColorIndex;
        }
        
        return *this;
    }
    
} // namespace sora
