//
//  SoraPhysicShape.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPhysicShape_h
#define Sora_SoraPhysicShape_h

#include "SoraPlatform.h"
#include "SoraMath.h"
#include "util/SoraArray.h"

namespace sora {
    
    class SoraPhysicShape {
    public:
        SoraPhysicShape();
        
        enum ShapeType {
            ShapeNone,
            ShapeEdge,
            ShapeBox,
            ShapeCircle,
            ShapePolygon, 
        };
        
        void setAsEdge(float x1, float y1, float x2, float y2);
        void setAsBox(float w, float h, float centerX, float centerY, float rotation);
        void setAsCircle(float r);
        void setAsPolygon(SoraVector* vertices, uint32 size);
        
        ShapeType type() const;
        
        static SoraPhysicShape EdgeAsShape(float x1, float y1, float x2, float y2);
        static SoraPhysicShape BoxAsShape(float w, float h, float centerx, float centery, float rotation);
        static SoraPhysicShape CircleAsShape(float r);
        static SoraPhysicShape PolygonAsShape(SoraVector* vertices, uint32 size);
        
        typedef SoraArray<SoraVector> VertexArray;
        
        /**
         *  for edge
                mVertices[0] = p1; mVertices[1] = p2;
         *  for box
                mVertices[0] = w/h; mVertices[1] = center;
         *  for circle
                mVertices[0] = r/r;
         *  for polygon
                mVertices = vertice list;
         **/
        VertexArray mVertices;
        
        ShapeType mType;
    };
    
} // namespace sora



#endif
