#ifndef SORA_FT_FONT_MANAGER_H_
#define SORA_FT_FONT_MANAGER_H_

#include <vector>
#include <string>
#include "SoraPlatform.h"
#include "SoraFreetypeFont.h"
#include "SoraFontManager.h"

#ifdef WIN32
#pragma comment(lib, "freetype244ST.lib")
#endif

namespace sora {
	
	/**
	 FTLibrary
	 */
	class FTLibrary {
		friend class SoraSingleton<FTLibrary>;
	public:
		FT_Library library;
		
		FTLibrary() {
			if(FT_Init_FreeType(&library)) {
				err = 1;
			}
			err = 0;
		}
		~FTLibrary() {
			FT_Done_FreeType(library);
		}
		
		int err;
	};
	/*
	class FTFace;
	class SoraFTFont;
	*/
	
	struct SFTFace {
		SoraString fileName;
		FTFace* face;
		
		bool operator < (const SFTFace& other) const {
			return strcmp(fileName.c_str(), other.fileName.c_str()) < 0;
		}
	};

	struct SFTFont {
		SoraString fileName;
		uint32 size;
		SoraFTFont* Font;

		bool operator < (const SFTFont& other) const {
			int r = strcmp(fileName.c_str(), other.fileName.c_str());
			if(r != 0) {
				return r < 0;
			} else {
				return (size < other.size);
			}
		}
	};

	class SoraFTFontManager: public SoraFontManager {
	public:
		SoraFTFontManager() {
			lib = new FTLibrary;
			if(lib->err != 0) {
				delete lib;
				inited = false;
			}
			inited = true;
		}
		~SoraFTFontManager();

		SoraFont* getFont(const char* filename, uint32 fontsize, uint32 bsize=0, const char* pr=0);
		bool fontExists(const char* filename);
		void releaseFont(SoraFont* font);

	private:
		template<typename T1, typename T2>
		int find(T1 &con, T2 &f);

	private:
		std::vector<SFTFace> Faces;
		std::vector<SFTFont> FTFonts;
		FTLibrary* lib;
		
		bool inited;
	};
};

#endif