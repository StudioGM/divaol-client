//
//  SoraVertexList.h
//  Sora
//
//  Created by Ruiwei Bu on 10/28/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraVertexList_h
#define Sora_SoraVertexList_h

#include "SoraPlatform.h"
#include "SoraShaderable.h"
#include "util/SoraArray.h"

namespace sora {
    
    struct SoraVertex;
    
    // simple vertex list works like glVertex*, glTexCoord*, glColor*
    // vertex positions are related positiont
    
    class SORA_API SoraVertexList: public SoraShaderable, public SoraObject {
    public:
        friend class SoraShape;
        
        // Line, Triangle, TriangleStrip, TriangleFan, Quad(not available in opengles renderers such as in ios)
        SoraVertexList(RenderMode vertexMode);
        ~SoraVertexList();
        
        void blendMode(int32 mode);
        void bindTexture(SoraTextureHandle texture);
        
        void clear();
        
        void setRenderMode(RenderMode mode);
        
        void vertex(const SoraVertex& vertex);
        void vertexList(const SoraVertex* vertex, size_t count);
        
        // glVertex2
        void vertex2(float x, float y);
        void vertex2v(size_t count, float* v, size_t stride);
        
        // glVertex3
        void vertex3(float x, float y, float z);
        void vertex3v(size_t count, float* v, size_t stride=0);
        
        // glVertexPointer
        void vertexPointer(size_t size, size_t count, float* v, size_t stride = 0);
                    
        // glTexCoord
        void texCoord(float x, float y);
        void texCoordPointer(size_t count, float* v, size_t stride=0);
        
        // glColor
        void color(uint32 col);
        void color3(float r, float g, float b);
        void color4(float r, float g, float b, float a);
        void colorPointer(size_t count, float* v, size_t stride=0);
        
        void render();
        
        uint32 size() const;
        const SoraArray<SoraVertex>& getVertexList() const;
        SoraArray<SoraVertex>& getVertexList();
        
        SoraVertexList& operator=(const SoraVertexList& rhs);
        
    private:
        RenderMode mVertexMode;
        int32 mBlendMode;
        SoraTextureHandle mTexture;
        SoraArray<SoraVertex> mVertexList;
        
        int mVertexIndex;
        int mTexCoordIndex;
        int mColorIndex;
    };
    
} // namespace sora


#endif
