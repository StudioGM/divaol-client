//
//  SoraRenderParameters.h
//  Sora
//
//  Created by Ruiwei Bu on 10/29/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraRenderParameters_h
#define Sora_SoraRenderParameters_h

namespace sora {
    
    typedef uint32 SoraUniqueId;
    
    typedef SoraHandle SoraSpriteHandle;
    typedef SoraHandle SoraTextureHandle;
    typedef SoraHandle SoraTargetHandle;
    typedef SoraHandle SoraResourceHandle;
    typedef SoraHandle SoraWindowHandle;
    
    enum RenderMode {
        // single points
        Point,              // GL_POINTS, D3DPT_POINTLIST
        
        // line mode
        Line = 1,           // GL_LINES, D3DPT_LINELIST
        LineLoop = 2,       // GL_LINE_LOOP, D3DPT_LINESTRIP (no lineloop in d3d)
        
        // triangle mode
        Triangle,           // GL_TRIANGLE, D3DPT_TRIANGLELIST
        TriangleFan,        // GL_TRIANGLE_FAN, D3DPT_TRIANGLEFSN
        TriangleStrip,      // GL_TRIANGLE_STRIP, D3DPT_TRIANGLESTRIP
        
        // quad may not available in every platform
        // suggest not using it
        Quad,               // GL_QUAD
    };
    
    enum ColorFormat {
        RGBA8,      // GL_RGBA8, D3DFMT_A8R8G8B8
        RGB565,     // GL_RGB565, D3DFMT_RGB6B5
        RGB8,       // GL_RGB8, D3DFMT_X8R8G8B8
    };
    
    enum RenderStateType {
        TextureWrap0 = 0,   // u
        TextureWrap1,       // v
        
        MinFilter,      // GL_TEXTURE_MIN_FILTER, D3DSAMP_MINFILTER
        MagFilter,      
        
        StencilOp,      // glStencilOp
        StencilFunc,
        
        DepthOp,        // glDepthOp, 
        DepthMask,      // glDepthMask, D3DRS_ZWRITTABLE
        
        ColorOp,        // GL_TEXTURE_ENV_MODE, D3DTTS_COLOROP
        
        SrcBlend,       // GL_SRC_BLEND, D3DRS_SRC_BLEND
        DstBlend,
        SrcAlpha,
        DstAlpha,
        
        BlendMode,
    };
    
    enum RenderStateParam {
        TextureWrapClamp = 0,       // D3DTADDRESS_CLAMP, GL_CLAMP
        TextureWrapRepeat,          // D3DTADDRESS_REPEAT, GL_REPEAT
        TextureWrapClampToBoarder,  // D3DTADDRESS_BOARDER, GL_CLAMP_TO_BOARDER
        TextureWrapMirror,          // D3DTADDRESS_MIRROR, GL_MIRRORED_REPEAT
        
        FilterNearest,              // GL_NEAREST, (D3DTEXF_POINT, NONE)
        FilterLinear,               // GL_LINEAR, (D3DTEXF_LINEAR, NONE)
        FilterNearestMipmapNearest, // GL_NEAREST_MIPMAP_NEAREST, (D3DTXEFT_POINT, POINT)
        FilterNearestMipmapLinear,  // GL_NEAREST_MIPMAP_LINEAR, (D3DTXEFT_POINT, LINEAR)
        FilterLinearMipmapNearest,  // GL_LINEAR_MIPMAP_NEAREST, (D3DTXEFT_LINEAR, POINT)
        
        StencilOpZero,          // GL_ZERO, D3DSTENCILOP_ZERO
        StencilOpKeep,          
        StencilOpReplace,
        StencilOpIncr,
        StencilOpDecr,
        StencilOpInvert,
        StencilOpIncrWrap,
        StencilOpDecrWrap,
        
        BlendOpAdd,             // GL_FUNC_ADD, D3DBLENDOP_ADD
        BlendOpSubstract,
        BlendOpReverseSubstract,
        
        BlendFuncZero,              // GL_ZERO, D3DBLEND_ZERO
        BlendFuncOne,
        BlendFuncSrcColor,
        BlendFuncOneMinusSrcColor,
        BlendFuncDstColor,
        BlendFuncOneMinusDstColor,
        BlendFuncSrcAlpha,
        BlendFuncOneMinusSrcAlpha,
        BlendFuncDstAlpha,
        BlendFuncOneMinusDstAlpha,
        
        CompNever,      // GL_NEVER, D3DCMP_NEVER
        CompAlways,
        CompLess,
        CompLEqual,
        CompEqual,
        CompGreater,
        CompGEqual,
        CompNotEqual,
        
        Enable,
        Disable, 
    };
    
    enum MatrixMode {
        ProjectionMatrix,
        TransformMatrix,
    };
    
} // namespace sora


#endif
