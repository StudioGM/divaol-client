#include "SoraTexture.h"
#include "SoraCore.h"

#include "SoraTextureMap.h"

namespace sora {
    
	SoraTexture::SoraTexture(uint32 texid, int32 width, int32 height, int32 oriwidth, int32 oriheight):
    mTextureID(texid),
    mTextureWidth(width),
    mTextureHeight(height),
    mOriginalWidth(oriwidth),
    mOriginalHeight(oriheight),
    mUserData(0),
    mTexData(0) {
        
#ifdef OS_IOS
        mIsRetinaTexture = false;
#endif
	}
    
	SoraTexture::SoraTexture():
    mTextureID(0),
    mTextureWidth(0),
    mTextureHeight(0),
    mOriginalWidth(0),
    mOriginalHeight(0),
    mUserData(0),
    mTexData(0) {
        
#ifdef OS_IOS
        mIsRetinaTexture = false;
#endif
    }
    
    SoraTextureHandle SoraTexture::LoadFromFile(const StringType& file) {
        return SoraCore::Instance()->createTexture(file);
    }
    
    SoraTextureHandle SoraTexture::LoadFromRawData(uint32* data, int32 w, int32 h) {
        return SoraCore::Instance()->createTextureFromRawData(data, w, h);
    }
    
    SoraTextureHandle SoraTexture::LoadFromMemory(uint32* data, uint32 size) {
        return SoraCore::Instance()->createTextureFromMem(data, size);
    }
    
    int32 SoraTexture::GetWidth(SoraTextureHandle handle) {
        sora_assert(handle);
        return ((SoraTexture*)handle)->mTextureWidth;
    }
    
    int32 SoraTexture::GetHeight(SoraTextureHandle handle) {
        sora_assert(handle);
        return ((SoraTexture*)handle)->mTextureHeight;
    }
    
    int32 SoraTexture::GetOriginalWidth(SoraTextureHandle handle) {
        sora_assert(handle);
        return ((SoraTexture*)handle)->mOriginalWidth;
    }
    
    int32 SoraTexture::GetOriginalHeight(SoraTextureHandle handle) {
        return ((SoraTexture*)handle)->mOriginalHeight;
    }
    
    SoraVector SoraTexture::GetSize(SoraTextureHandle handle) {
        return SoraVector((float)((SoraTexture*)handle)->mTextureWidth,
                          (float)((SoraTexture*)handle)->mTextureHeight);
    }
    
    SoraVector SoraTexture::GetOriginalSize(SoraTextureHandle handle) {
        return SoraVector((float)((SoraTexture*)handle)->mOriginalWidth,
                          (float)((SoraTexture*)handle)->mOriginalHeight);
    }
    
    StringType SoraTexture::GetTextureName(SoraTextureHandle handle) {
        return GetStringByUniqueId(SoraTextureMap::Instance()->getTextureName(handle));
    }
    
    SoraTextureHandle SoraTexture::CreateEmpty(int32 w, int32 h) {
        return SoraCore::Instance()->createTextureWH(w, h);
    }
    
    void SoraTexture::Release(SoraTextureHandle handle) {
        sora_assert(handle);
        SoraCore::Instance()->releaseTexture(handle);
    }
    
    uint32* SoraTexture::GetData(SoraTextureHandle handle) {
        sora_assert(handle);
        return SoraCore::Instance()->textureLock(handle);
    }
    
    void SoraTexture::PutData(SoraTextureHandle handle) {
        sora_assert(handle);
        SoraCore::Instance()->textureUnlock(handle);
    }
    
    SoraTextureHandle SoraTexture::NullObject() {
        static SoraTexture null_tex;
        return (SoraTextureHandle)&null_tex;
    }
    
} // namespace sora