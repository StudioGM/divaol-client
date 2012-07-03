#ifndef SORA_FREETYPE_FONT_H_
#define SORA_FREETYPE_FONT_H_

#include "SoraCore.h"
#include "SoraFont.h"

#include <ft2build.h>
#include <freetype/freetype.h>

#include <vector>

namespace sora {

	/**
		FTFace
	*/
	class FTFace {
	public:
		FTFace(): loaded(false) {}
		~FTFace();
		
		bool loaded;
		FT_Library	library;
		FT_Face		face;
		
		void* fontDataBuffer;

		bool load(const char* filename, uint32 size=0);
	};

	/**
		FTGlyph
	*/
	class FTGlyph {
	public:		
		FTGlyph();
		virtual ~FTGlyph();

		bool cached;
		SoraCore  *sora;

		void init() {}
		SoraTextureHandle createTexture( unsigned int *data, int w, int h );

		void cache(unsigned int idx);
		FT_Face *face;
		unsigned int size;
		unsigned int top;
		unsigned int left;
		unsigned int texw;
		unsigned int texh;
		unsigned int imgw;
		unsigned int imgh;
		SoraTextureHandle tex;
		int offset;
		unsigned char *image;
	};

	/** 
		Font Class using freetype to generate font glyphs and render to screen
	*/

	class SoraFTFont: public SoraFont {
	protected:
		friend class SoraFTFontManager;

	public:
		/**
		 * Render
		 *
		 * renders the text string to the screen
		 * @param x Screen-x coordinate
		 * @param y Screen-y coordinate
		 * @param align Alignment specificator. May be one of these: HGETEXT_LEFT, HGETEXT_RIGHT, HGETEXT_CENTER. 
		 * @param string the string to be rendered
		*/
		void	render(float x, float y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		/**
			render with alignments
		*/
		void	print(float x, float y, Alignment align, const wchar_t *format, ...);
		void	render(float x, float y, Alignment align, const wchar_t* text);

        void    renderIn3D(float x, float y, Alignment align, const wchar_t* text, const SoraMatrix4& transformMat);
        
		void	setColor(uint32 dwColor, int32 i = -1);
		uint32	getColor(int32 i = 0);

		void	setKerningWidth(float kerning);
		void	setKerningHeight(float kerning);

		float	getKerningWidth() const;
		float	getKerningHeight() const;

		float	getStringWidth(const wchar_t* text);
		float   getStringHeight(const wchar_t* pwstr);
        SoraVector getStringDimensions(const wchar_t* text);
		float	getHeight() const;

		uint32	getFontSize() const;
		void	setFontSize(uint32 size);
	
		float	getWidthFromCharacter(wchar_t c, bool original = false);
		
		// font effects
		
		void setScale(float scale);
		void setCharRotation(float rot);
        
        void setLineWidth(float width);
        float getLineWidth() const;
        
        float getCharRotation() const;
        float getScale() const;
        
        void setLineRotation(float rot, bool rotateChar=false);
        float getLineRotation() const;
		
	private:
		SoraFTFont();
		~SoraFTFont();

		SoraCore* sora;
		SoraSprite* sprite;
				
		float kerningWidth;
		float kerningHeight;
        float lineWidth;

		bool attached;
		uint32 size;		
		// not recommended methods
		bool attach(FTFace *Face, uint32 size); 
		uint32 getGlyphByChar(wchar_t c);

		FTFace* ft_face;
		std::vector<FTGlyph> ft_glyphs;
		
		float scale;
		float charRotation;
        float lineRotation;
	};
} // namespace sora


#endif