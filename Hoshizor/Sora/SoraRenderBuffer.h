//
//  SoraVertexBuffer.h
//  Sora
//
//  Created by Ruiwei Bu on 11/12/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraVertexBuffer_h
#define Sora_SoraVertexBuffer_h

#include "SoraPlatform.h"
#include "SoraPreDeclare.h"

#include "SoraAutoPtr.h"

namespace sora {
    
    /**
     * Vertex Format
     * Format of SoraVertex = VertexXYZ | VertexUV | VertexColor
            & OffsetXYZ = 0, OffsetUV = 16, OffsetColor = 12
            & TotalSize = 24
     **/
    enum VertexFormat {
        VertexXYZ    = 1UL << 0,
        VertexNormal = 1UL << 1,
        VertexUV     = 1UL << 2,
        VertexColor  = 1UL << 3,
        VertexColor2 = 1UL << 4,
    };

    struct SORA_API SoraVertexFormat {
        SoraVertexFormat(int32 format);
        SoraVertexFormat(int32 format, uint32 offsetXYZ, uint32 offsetNormal, uint32 offsetUV, uint32 offsetColor, uint32 offsetColor2);
        
        bool checkFormat(VertexFormat format) const;
        
        int32 format() const;
        
        SoraVertexFormat& offsetXYZ(uint32 offset);
        SoraVertexFormat& offsetNormal(uint32 offset);
        SoraVertexFormat& offsetUV(uint32 offset);
        SoraVertexFormat& offsetColor(uint32 offset);
        SoraVertexFormat& offsetColor2(uint32 offset);
        
        uint32 offsetXYZ() const;
        uint32 offsetNormal() const;
        uint32 offsetUV() const;

        uint32 offsetColor() const;
        uint32 offsetColor2() const;
        
        uint32 totalSize() const;
        
        static SoraVertexFormat FormatOfSoraVertex();
        
    private:
        uint32 mOffsetXYZ;
        uint32 mOffsetNormal;
        uint32 mOffsetColor;
        uint32 mOffsetColor2;
        uint32 mOffsetUV;
        
        uint32 mTotalSize;
        
        int32 mFormat;
    };
    
    /**
     * Abstract base classes from direct render buffer access
     **/
    class SORA_API SoraRenderBuffer {
    public:
        typedef SoraAutoPtr<SoraRenderBuffer> Ptr;
        
        enum Access {
            ReadOnly,
            WriteOnly,
            ReadWrite,
        };
        
        enum Usage {
            Static,     // GL_STATIC_DRAW etc
            Dynamic,
        };
        
        static SoraRenderBuffer::Ptr NullObject();
        
        SoraRenderBuffer(Access a, Usage u):
        mAccessHint(a),
        mUsage(u) { }
        
        virtual ~SoraRenderBuffer() { }
        
        // null
        virtual void* map() { return 0; }
        virtual void unmap() { }
        
        virtual void active() { }

		virtual uint32 count() const { return 0; }
        
        virtual void resize(uint32 desired_count) { }
        
        Access access() const { return mAccessHint; }
        Usage usage() const { return mUsage; }
                
    private:
        Access mAccessHint;
        Usage mUsage;
    };
        
} // namespace sora

#endif
