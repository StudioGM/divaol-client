//
//  SoraPhysicShape.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraPhysicShape.h"

namespace sora {
    
    SoraPhysicShape::SoraPhysicShape():
    mType(ShapeNone) {
        mVertices.reserve(2);
    }
    
    void SoraPhysicShape::setAsEdge(float x1, float y1, float x2, float y2) {
        mVertices.clear();
        
        mType = ShapeEdge;
        mVertices.push_back(SoraVector(x1, y1));
        mVertices.push_back(SoraVector(x2, y2));
    }
    
    void SoraPhysicShape::setAsBox(float w, float h, float centerX, float centerY, float rotation) {
        mVertices.clear();
        
        mType = ShapeBox;
        mVertices.push_back(SoraVector(w, h));
        mVertices.push_back(SoraVector(centerX, centerY));
        mVertices.push_back(SoraVector(rotation, rotation));
    }
    
    void SoraPhysicShape::setAsCircle(float r) {
        mVertices.clear();
        
        mType = ShapeCircle;
        mVertices.push_back(SoraVector(r, r));
    }
    
    void SoraPhysicShape::setAsPolygon(SoraVector* vertices, uint32 size) {
        mVertices.clear();
        
        mType = ShapePolygon;
        for(int32 i=0; i<size; ++i)
            mVertices.push_back(vertices[i]);
    }
    
    SoraPhysicShape::ShapeType SoraPhysicShape::type() const {
        return mType;
    }
    
    SoraPhysicShape SoraPhysicShape::EdgeAsShape(float x1, float y1, float x2, float y2) {
        SoraPhysicShape shape;
        shape.setAsEdge(x1, y1, x2, y2);
        return shape;
    }
    
    SoraPhysicShape SoraPhysicShape::BoxAsShape(float w, float h, float cx, float cy, float r) {
        SoraPhysicShape shape;
        shape.setAsBox(w, h, cx, cy, r);
        return shape;
    }
    
    SoraPhysicShape SoraPhysicShape::CircleAsShape(float r) {
        SoraPhysicShape shape;
        shape.setAsCircle(r);
        return shape;
    }
    
    SoraPhysicShape SoraPhysicShape::PolygonAsShape(SoraVector* vertices, uint32 size) {
        SoraPhysicShape shape;
        shape.setAsPolygon(vertices, size);
        return shape;
    }
    
    
} // namespace sora
