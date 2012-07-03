#ifndef SORA_GRAPHIC_EFFECT_H
#define SORA_GRAPHIC_EFFECT_H

#include "SoraPlatform.h"

namespace sora {
    
    class SoraSprite;
    
    namespace cpu_graphic_effect {
        
        extern "C" {
            int gaussBlur(uint32 *data, int width, int height, double sigma, int radius);
            int gray(uint32 *data, int width, int height);
            int reverse(uint32* data, int width, int height);
            int alphaMix(uint32* target, uint32* source, int width, int height, int width2, int height2, double a, int posx=0, int posy=0);
            int bresenhamScale(uint32* target, uint32* source, int32 srcWidth, int32 srcHeight, int32 tgtWidth, int32 tgtHeight);
        }
    } // cpu_graphic_effect

	class SoraGraphicAlgorithm {
	public:
		static SoraSprite* gaussBlurNew(SoraSprite* in, double sigma, int radius);
		static void gaussBlur(SoraSprite* in, double sigma, int radius);

		static SoraSprite* grayNew(SoraSprite* in);
        static void gray(SoraSprite* in);
        
        static SoraSprite* reverseNew(SoraSprite* in);
		static void reverse(SoraSprite* in);

		static SoraSprite* alphaMixNew(SoraSprite* in, SoraSprite* src, double a, int posx=0, int posy=0);
		static void alphaMix(SoraSprite* in, SoraSprite* src, double a, int posx=0, int posy=0);
        
        static SoraSprite* scaleNew(SoraSprite* in, float rate);
        static void scale(SoraTextureHandle dst, SoraSprite* src);
	};
    
    

} // namespace sora

#endif