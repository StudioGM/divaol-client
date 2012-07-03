#ifndef SORA_FONT_MANAGER_H_
#define SORA_FONT_MANAGER_H_

//#include "SoraFont.h"
#include "SoraPlatform.h"

namespace sora {
	/**
		Abstract FontManager class provides font manage APIs
		This would use by SoraCore
	*/
	class SoraFont;

	class SORA_API SoraFontManager {
	public:

		// this API both loads fonts in memory and hard drive
		// when bzsize != 0, then filename = first byte of font data in memory
		// under this occasion, pr is used as real file name because we need to cache the font for efficiency
		virtual SoraFont* getFont(const char* filename, uint32 size, uint32 bsize=0, const char* pr=0) = 0;
		
		// check whether the font already exists in the FontManager
		// if it already exist, then SoraCore won't load resource file again, just give filename and pr = 0;
		virtual bool fontExists(const char* filename) = 0;
		
		virtual void releaseFont(SoraFont* font) = 0;
	};
} // namespace sora

#endif