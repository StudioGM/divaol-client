#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "SoraPlatform.h"
#include "SoraString.h"
#include "math/SoraVector.h"

namespace sora {
    
	class SORA_API SoraTexture {
    public:
		SoraTexture();
		SoraTexture(uint32 tid, int32 width, int32 height, int32 oriwidth, int32 oriheight);

		uint32 mTextureID;
		int32 mTextureWidth, mTextureHeight;
		int32 mOriginalWidth, mOriginalHeight;
        
        /**
         * For wrapper convinience
         **/
        void* mUserData;
        /*
         * Use to store texture color data temporary by Renderers
         */
		uint32* mTexData;
        
#ifdef OS_IOS
        // if not retina texture and the device supports retina display, we need to scale the texture
        bool mIsRetinaTexture;
#endif
    public:
        /** 
         * static functions
         **/
        
        static SoraTextureHandle LoadFromFile(const StringType& file);
        static SoraTextureHandle LoadFromRawData(uint32* data, int32 w, int32 h);
        static SoraTextureHandle LoadFromMemory(uint32* data, uint32 size);
        static SoraTextureHandle CreateEmpty(int32 w, int32 h);
        
        static SoraTextureHandle NullObject();
        
        static uint32*  GetData(SoraTextureHandle handle);
        static void     PutData(SoraTextureHandle data);
        
        static int32        GetWidth(SoraTextureHandle handle);
        static int32        GetHeight(SoraTextureHandle handle);
        static int32        GetOriginalWidth(SoraTextureHandle handle);
        static int32        GetOriginalHeight(SoraTextureHandle handle);
        static SoraVector   GetSize(SoraTextureHandle handle);
        static SoraVector   GetOriginalSize(SoraTextureHandle handle);
        static StringType   GetTextureName(SoraTextureHandle handle);
        
        static void Release(SoraTextureHandle h);
	};

} // namespace sora

#endif