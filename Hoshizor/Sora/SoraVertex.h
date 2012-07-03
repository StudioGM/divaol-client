//
//  SoraVertex.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraVertex_h
#define Sora_SoraVertex_h

#include "SoraPlatform.h"

#include "SoraAutoPtr.h"
#include "util/SoraArray.h"

#include "math/SoraMatrix4.h"

namespace sora {
    
	struct SORA_API SoraVertex {
		float			x, y;		// screen position    
		float			z;			// Z-buffer depth
		uint32			col;		// color
		float			tx, ty;		// texture coordinates
		
		SoraVertex(): z(0.0f), col(0xFFFFFFFF) {}
        SoraVertex(float _x, float _y, float _tx, float _ty): x(_x), y(_y), tx(_tx), ty(_ty), col(0xFFFFFFFF), z(0.f) {}
        
        SoraVertex operator*(const SoraMatrix4& rhs) const {
            SoraVector4 vec = rhs * SoraVector4(x, y, z, 1.0);
            
            SoraVertex tmp = *this;
            tmp.x = vec.x;
            tmp.y = vec.y;
            tmp.z = vec.z;
            return tmp;
        }
        SoraVertex& operator*=(const SoraMatrix4& rhs) {
            SoraVector4 vec = rhs * SoraVector4(x, y, z, 1.0);
            x = vec.x;
            y = vec.y;
            z = vec.z;
            
            return *this;
        }
    };
    
#define	BLEND_COLORADD		1
#define	BLEND_COLORMUL		0
#define	BLEND_ALPHABLEND	2
#define	BLEND_ALPHAADD		0
#define	BLEND_ZWRITE		4
#define	BLEND_NOZWRITE		0
#define BLEND_SRCALPHA		8
    
#define BLEND_DEFAULT		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z		(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)
    
    typedef SoraArray<SoraVertex> SoraVertexArray;

    class SoraTexture;
	
	struct SORA_API SoraQuad {
		SoraVertex		v[4];
		SoraTexture*	tex;
		int				blend;
        
        SoraQuad(): tex(0) {}
        
        SoraQuad operator*(const SoraMatrix4& rhs) const {
            SoraQuad tmp = *this;
            for(int i=0; i<4; ++i)
                tmp.v[i] *= rhs;
            return tmp;
        }
        SoraQuad& operator*=(const SoraMatrix4& rhs) {
            for(int i=0; i<4; ++i)
                v[i] *= rhs;
            return *this;
        }
	};
}


#endif
