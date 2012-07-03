//
//  SoraShape.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShape.h"
#include "SoraCore.h"
#include "SoraRenderSystem.h"
#include "SoraSprite.h"
#include "SoraVertexList.h"

namespace sora {
    
    SoraShape::SoraShape(SoraTextureHandle handle, RenderMode mode):
    mCompiled(false),
    mRenderMode(mode),
    mTexture(handle),
    mOutline(false),
    mOutlineWidth(1.f),
    mOutlineColor(0xFFFFFFFF),
    mOutlineOnly(false),
    mRenderToSprite(false),
    mRenderTarget(0),
    mSprite(0),
    mClosed(false) {
        
    }
    
    SoraShape::~SoraShape() {
        if(mRenderTarget)
            SoraCore::Instance()->freeTarget(mRenderTarget);
        if(mSprite)
            delete mSprite;
    }
    
    void SoraShape::initWithVertexList(const SoraVertexList& list) {
        mCompiled = true;
        mTexture = list.mTexture;
        mRenderMode = list.mVertexMode;
        mVertices = list.mVertexList;
    }
    
    void SoraShape::addPoint(float x, float y, uint32 color, float tx, float ty) {
        if(mTexture && tx == 0.0 && ty == 0.0) {
            SoraTexture* tex = (SoraTexture*)mTexture;
            tx = 1.0f / tex->mTextureWidth;
            ty = 1.0f / tex->mTextureHeight;
        }
        SoraVertex vertex(x, y, tx, ty);
        vertex.col = color;
        mVertices.push_back(vertex);
        mCompiled = false;
    }
    
    void SoraShape::addPoint(const SoraVector& pos, uint32 color, float tx, float ty) {
        if(mTexture && tx == 0.0 && ty == 0.0) {
            SoraTexture* tex = (SoraTexture*)mTexture;
            tx = 1.0f / tex->mTextureWidth;
            ty = 1.0f / tex->mTextureHeight;
        }
        SoraVertex vertex(pos.x, pos.y, tx, ty);
        vertex.col = color;
        mVertices.push_back(vertex);
        mCompiled = false;
    }
    
    void SoraShape::setPoint(uint32 index, float x, float y, uint32 color, float tx, float ty) {
        sora_assert(index < mVertices.size());
        
        if(index < mVertices.size()) {
            SoraVertex& vertex = mVertices[index];
            vertex.x = x;
            vertex.y = y;
            vertex.col = color;
            vertex.tx = tx;
            vertex.ty = ty;
        }
    }
    
    void SoraShape::compile() {
        if(mVertices.size() == 0)
            return;
        
        if(mCompiled)
            return;
        
        if(!mTexture)
            return;
       
        int32 textureWidth = 0;
        int32 textureHeight = 0;
        if(mTexture) {
            textureWidth = SoraTexture::GetOriginalWidth(mTexture);
            textureHeight = SoraTexture::GetOriginalHeight(mTexture);
        }
        
        VertexArray::iterator it = mVertices.begin();
        VertexArray::iterator end = mVertices.end();
        
        for(; it != end; ++it) {            
            if(mTexture && it->tx == 0.f && it->ty == 0.f) {
                it->tx = (float)it->x / textureWidth;
                it->ty = (float)it->y / textureHeight;
            }
        }
    
        mCompiled = true;
        
        if(mRenderToSprite && mSprite && mRenderTarget)
            spriteRender();
    }
    
    void SoraShape::setRenderMode(RenderMode mode) {
        mRenderMode = mode;
    }
    
    RenderMode SoraShape::getRenderMode() const {
        return mRenderMode;
    }
    
    void SoraShape::spriteRender() {
        if(!mCompiled && mTexture)
            compile();
        
        SoraCore* core = SoraCore::Instance();

        core->beginScene(0, mRenderTarget);
        if(!mOutlineOnly) {
            core->renderWithVertices(mTexture, BLEND_DEFAULT, mVertices.begin(), mVertices.size(), mRenderMode);
        }
        
        if(mOutline) {
            VertexArray::iterator it = mVertices.begin();
            VertexArray::iterator end = mVertices.end() - 1;
            
            for(; it != end; ++it) {
                core->renderLine(it->x, it->y, (it+1)->x, (it+1)->y, mOutlineColor, mOutlineWidth);
            }
            if(mClosed) {
                core->renderLine(mVertices.back().x,
                                 mVertices.back().y,
                                 mVertices.front().x,
                                 mVertices.front().y, mOutlineColor, mOutlineWidth);
            }
        }
        core->endScene();
    }
    
    void SoraShape::render() {
        SoraCore* core = SoraCore::Instance();

        if(mRenderToSprite && mSprite && mRenderTarget) {            
            mSprite->render();
            return;
        }
        
        if(!mCompiled && mTexture != 0)
            compile();
        
        if(is3DEnabled()) {
            SoraCore::Ptr->pushTransformMatrix();
            SoraCore::Ptr->getRenderSystem()->multTransformMatrix(getTransform().getTransformMatrix());
        }
                
        if(!mOutlineOnly) {
            core->renderWithVertices(mTexture, BLEND_DEFAULT, mVertices.begin(), mVertices.size(), mRenderMode);
        }
        
        if(mOutline) {
            VertexArray::iterator it = mVertices.begin();
            VertexArray::iterator end = mVertices.end() - 1;
            
            for(; it != end; ++it) {
                core->renderLine(it->x, it->y, (it+1)->x, (it+1)->y, mOutlineColor, mOutlineWidth);
            }
            if(mClosed) {
                core->renderLine(mVertices.back().x,
                                 mVertices.back().y,
                                 mVertices.front().x,
                                 mVertices.front().y, mOutlineColor, mOutlineWidth);
            }
        }
        
        if(is3DEnabled()) {
            SoraCore::Ptr->popTransformMatrix();
        }
    }
    
    size_t SoraShape::size() const {
        return mVertices.size();
    }
    
    void SoraShape::enableOutline(float width, uint32 color) {
        mOutline = true;
        mOutlineWidth = width;
        mOutlineColor = color;
    }
    
    void SoraShape::disableOutline() {
        mOutline = false;
    }
    
    void SoraShape::setOutlineOnly(bool flag) {
        mOutlineOnly = flag;
        if(flag)
            mOutline = true;
    }
    
    void SoraShape::setTexture(SoraTextureHandle texture) {
        mTexture = texture;
        
        mCompiled = false;
    }
    
    SoraTextureHandle SoraShape::getTexture() const {
        return mTexture;
    }
    
    SoraShape& SoraShape::operator=(const SoraShape& rhs) {
        if(this != &rhs) {
            mVertices = rhs.mVertices;
            mCompiled = false;
            
            mRenderMode = rhs.mRenderMode;
            mTexture = rhs.mTexture;
            
            mOutline = rhs.mOutline;
            mOutlineWidth = rhs.mOutlineWidth;
            mOutlineColor = rhs.mOutlineColor;
            mOutlineOnly = rhs.mOutlineOnly;
            
            this->enableRenderToSprite(rhs.mRenderToSprite);
        }
		return *this;
    }

    SoraVector SoraShape::ComputeNormal(const SoraVector& p1, const SoraVector& p2) {
        SoraVector normal;
        
        normal.x = p1.y - p2.y;
        normal.y = p2.x - p1.x;
        float len = sqrt(normal.x * normal.x + normal.y * normal.y);
        if(len == 0.f) {
            return SoraVector();
        }
        
        normal.x /= len;
        normal.y /= len;
        
        return normal;
    }
    
    void SoraShape::enableRenderToSprite(bool flag) {
        mRenderToSprite = flag;
        if(mRenderToSprite) {
            if(!mRenderTarget) {
                float farW = 0.f, farH = 0.f;
                if(!mTexture) {
                    VertexArray::iterator it = mVertices.begin();
                    VertexArray::iterator end = mVertices.end();
                    
                    for(; it != end; ++it) {
                        if(farW < it->x)
                            farW = it->x;
                        if(farH < it->y)
                            farH = it->y;
                    }
                } else {
                    farW = (float)SoraTexture::GetOriginalWidth(mTexture);
                    farH = (float)SoraTexture::GetOriginalHeight(mTexture);
                }
                mRenderTarget = SoraCore::Instance()->createTarget((int32)farW+1, (int32)farH+1);
                
                if(mRenderTarget) {
                    mSprite = new SoraSprite(SoraCore::Instance()->getTargetTexture(mRenderTarget));
                    sora_assert(mSprite);
                }
                
                mCompiled = false;
                spriteRender();
            }
        } else {
            if(mRenderTarget) {
                SoraCore::Instance()->freeTarget(mRenderTarget);
                mRenderTarget = 0;
            }
            if(mSprite) {
                delete mSprite;
                mSprite = 0;
            }
        }
    }
    
    const SoraArray<SoraVertex>& SoraShape::getVertexArray() const {
        return mVertices;
    }
    
    SoraSprite* SoraShape::getSprite() const {
        return mSprite;
    }
    
    void SoraShape::setClosed(bool flag) {
        mClosed = flag;
    }
    
    SoraShape SoraShape::Line(float x1, float y1, float x2, float y2, float width, uint32 col) {
        SoraVector p1(x1, y1);
        SoraVector p2(x2, y2);
        
        // Compute the extrusion direction
        SoraVector normal = ComputeNormal(p1, p2);
        normal *= width / 2;
        
        // Create the shape's points
        SoraShape shape;
        shape.addPoint(p1 - normal, col);
        shape.addPoint(p2 - normal, col);
        shape.addPoint(p2 + normal, col);
        shape.addPoint(p1 + normal, col);
        
        // Compile it
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Circle(float x, float y, float r, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        for(int i=0; i <= maxSegment; ++i) {
            float a = i * 2 * F_PI / maxSegment;
            SoraVector off(cos(a), sin(a));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Ring(float x, float y, float r, float thickness, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        for(int i=0; i <= maxSegment; ++i) {
            float a = i * 2 * F_PI / maxSegment;
            SoraVector off(cos(a), sin(a));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        shape.enableOutline(thickness, col);
        shape.setOutlineOnly(true);
        return shape;
    }
    
    SoraShape SoraShape::Fan(float x, float y, float r, float a, float b, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        
        for(int i=0; i <= maxSegment; ++i) {
            float angle = a + i * (b - a) / maxSegment;
            SoraVector off(cos(angle), sin(angle));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        return shape;
    }
    
    SoraShape SoraShape::Arc(float x, float y, float r, float a, float b, float thickness, uint32 col, int32 maxSegment) {
        sora_assert(maxSegment != 0);
        
        SoraShape shape;
        
        SoraVector center(x, y);
        
        for(int i=0; i <= maxSegment; ++i) {
            float angle = a + i * (b - a) / maxSegment;
            SoraVector off(cos(angle), sin(angle));
            
            shape.addPoint(center + off * r, col);
        }
        
        shape.compile();
        shape.setClosed(false);
        shape.enableOutline(thickness, col);
        shape.setOutlineOnly(true);

        return shape;
    }
    
    SoraShape SoraShape::Box(float x1, float y1, float x2, float y2, uint32 col) {
        SoraShape shape;
        
        shape.addPoint(SoraVector(x1, y1), col);
        shape.addPoint(SoraVector(x2, y1), col);
        shape.addPoint(SoraVector(x2, y2), col);
        shape.addPoint(SoraVector(x1, y2), col);
        
        shape.compile();
        return shape;
    }
    
    
} // namespace sora