//
//  SoraRenderBuffer.cpp
//  Sora
//
//  Created by Ruiwei Bu on 11/13/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraRenderBuffer.h"

namespace sora {
    
    SoraVertexFormat::SoraVertexFormat(int32 format, uint32 offsetXYZ, uint32 offsetNormal, uint32 offsetUV, uint32 offsetColor, uint32 offsetColor2):
    mFormat(format),
    mOffsetXYZ(offsetXYZ),
    mOffsetNormal(offsetNormal),
    mOffsetUV(offsetUV),
    mOffsetColor(offsetColor),
    mOffsetColor2(offsetColor2),
    mTotalSize(0) {
        if(format & VertexXYZ) {
            mTotalSize += sizeof(float) * 3;
        }
        if(format & VertexNormal) {
            mTotalSize += sizeof(float) * 3;
        }
        if(format & VertexUV) {
            mTotalSize += sizeof(float) * 2;
        }
        if(format & VertexColor) {
            mTotalSize += sizeof(uint32);
        }
        if(format & VertexColor2) {
            mTotalSize += sizeof(uint32);
        }
    }
    
    SoraVertexFormat::SoraVertexFormat(int32 format):
    mFormat(format),
    mTotalSize(0) {
        if(format & VertexXYZ) {
            mTotalSize += sizeof(float) * 3;
        }
        if(format & VertexNormal) {
            mTotalSize += sizeof(float) * 3;
        }
        if(format & VertexUV) {
            mTotalSize += sizeof(float) * 2;
        }
        if(format & VertexColor) {
            mTotalSize += sizeof(uint32);
        }
        if(format & VertexColor2) {
            mTotalSize += sizeof(uint32);
        }
    }
    
    bool SoraVertexFormat::checkFormat(VertexFormat format) const {
        return mFormat & format;
    }
    
    int32 SoraVertexFormat::format() const { 
        return mFormat;
    }
    
    SoraVertexFormat& SoraVertexFormat::offsetXYZ(uint32 offset) {
        mOffsetXYZ = offset;
        return *this;
    }
    
    SoraVertexFormat& SoraVertexFormat::offsetNormal(uint32 offset) {
        mOffsetNormal = offset;
        return *this;
    }
    
    SoraVertexFormat& SoraVertexFormat::offsetUV(uint32 offset) {
        mOffsetUV = offset;
        return *this;
    }
    
    SoraVertexFormat& SoraVertexFormat::offsetColor(uint32 offset) {
        mOffsetColor = offset;
        return *this;
    }
    
    SoraVertexFormat& SoraVertexFormat::offsetColor2(uint32 offset) {
        mOffsetColor2 = offset;
        return *this;
    }
    
    uint32 SoraVertexFormat::offsetXYZ() const { 
        return mOffsetXYZ;
    }
    
    uint32 SoraVertexFormat::offsetNormal() const { 
        return mOffsetNormal;
    }
    
    uint32 SoraVertexFormat::offsetUV() const { 
        return mOffsetUV;
    }
    
    uint32 SoraVertexFormat::offsetColor() const  { 
        return mOffsetColor;
    }
    
    uint32 SoraVertexFormat::offsetColor2() const { 
        return mOffsetColor2; 
    }
    
    uint32 SoraVertexFormat::totalSize() const { 
        return mTotalSize;
    }
    
    SoraVertexFormat SoraVertexFormat::FormatOfSoraVertex() {
        return (SoraVertexFormat(VertexXYZ | VertexUV | VertexColor)
                .offsetUV(sizeof(float) * 3 + sizeof(uint32))
                .offsetXYZ(0)
                .offsetColor(sizeof(float) * 3));
    }
    
    SoraRenderBuffer::Ptr SoraRenderBuffer::NullObject() {
        static SoraAutoPtr<SoraRenderBuffer> ptr;
        return ptr;
    }
    
}
