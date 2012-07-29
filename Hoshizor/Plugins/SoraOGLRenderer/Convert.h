//
//  Convert.h
//  Sora
//
//  Created by Robert Bu on 10/29/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_Convert_ogl_renderer_h
#define Sora_Convert_ogl_renderer_h

#include "SoraPlatform.h"

#if defined(OS_LINUX)
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include "glfw/GL/glfw.h"

namespace sora {
    
    GLint RenderParamToGLParam(int32 param) {
        switch (param) {
            case TextureWrapClamp:          return GL_CLAMP;
            case TextureWrapRepeat:         return GL_REPEAT;
            case TextureWrapClampToBoarder: return GL_CLAMP_TO_BORDER;
            case TextureWrapMirror:         return GL_MIRRORED_REPEAT;
                
            case FilterNearest:             return GL_NEAREST;
            case FilterLinear:              return GL_LINEAR;
            case FilterNearestMipmapNearest:return GL_NEAREST_MIPMAP_NEAREST;
            case FilterNearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
            case FilterLinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
                
            case StencilOpZero:             return GL_ZERO;
            case StencilOpKeep:             return GL_KEEP;
            case StencilOpReplace:          return GL_REPLACE;
            case StencilOpIncr:             return GL_INCR;
            case StencilOpDecr:             return GL_DECR;
            case StencilOpInvert:           return GL_INVERT;
            case StencilOpIncrWrap:         return GL_INCR_WRAP;
            case StencilOpDecrWrap:         return GL_DECR_WRAP;
                
            case BlendOpAdd:                return GL_FUNC_ADD;
            case BlendOpSubstract:          return GL_FUNC_SUBTRACT;
            case BlendOpReverseSubstract:   return GL_FUNC_REVERSE_SUBTRACT;
                
            case BlendFuncZero:             return GL_ZERO;
            case BlendFuncOne:              return GL_ONE;
            case BlendFuncSrcColor:         return GL_SRC_COLOR;
            case BlendFuncOneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
            case BlendFuncDstColor:         return GL_DST_COLOR;
            case BlendFuncOneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
            case BlendFuncSrcAlpha:         return GL_SRC_ALPHA;
            case BlendFuncOneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case BlendFuncDstAlpha:         return GL_DST_ALPHA;
            case BlendFuncOneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
                
            case CompNever:                 return GL_NEVER;
            case CompAlways:                return GL_ALWAYS;
            case CompLess:                  return GL_LESS;
            case CompLEqual:                return GL_EQUAL;
            case CompEqual:                 return GL_LEQUAL;
            case CompGreater:               return GL_GREATER;
            case CompGEqual:                return GL_GEQUAL;
            case CompNotEqual:              return GL_NOTEQUAL;
                
        }
        return GL_INVALID_VALUE;
    }
    
    GLenum RenderModeToGLMode(RenderMode mode) {
        switch (mode) {
            case Point:             return GL_POINTS;
			case Line:				return GL_LINES;
            case LineLoop:          return GL_LINE_LOOP;
			case Triangle:          return GL_TRIANGLES;
			case TriangleFan:       return GL_TRIANGLE_FAN;
			case TriangleStrip:     return GL_TRIANGLE_STRIP;
			case Quad:				return GL_QUADS;
		}
		return GL_INVALID_VALUE;
    }
    
} // naemspace sora


#endif
