#ifndef SORA_FONT_H_
#define SORA_FONT_H_

#include "SoraPlatform.h"
#include "SoraImageEffect.h"
#include "SoraString.h"
#include "math/SoraMatrix4.h"
#include "math/SoraVector.h"

namespace sora {

	/*
		Abstract Font Class provides Font related APIs
	*/

	class SORA_API SoraFont {
	public:
        // Font Render Alignments
        enum Alignment {
            AlignmentLeft     =   1,
            AlignmentRight    =   2,
            AlignmentCenter   =   3,
        };
        
		virtual void	render(float x, float y, const wchar_t* text, bool hcenter = false, bool vcenter = false) = 0;
		// render with alignments
		virtual void	print(float x, float y, Alignment align, const wchar_t *format, ...) = 0;
		virtual void	render(float x, float y, Alignment align, const wchar_t* text) = 0;
        
        virtual void    renderIn3D(float x, float y, Alignment align, const wchar_t* text, const SoraMatrix4& transformMat) = 0;

		virtual void	setColor(uint32 dwColor, int32 i = -1) = 0;
		virtual uint32	getColor(int32 i = 0) = 0;

		// set kernings
		virtual void	setKerningWidth(float kerning) = 0;
		virtual void	setKerningHeight(float kerning) = 0;

		// get kernings
		virtual float	getKerningWidth() const = 0;
		virtual float	getKerningHeight() const = 0;

		// get font dimensions
		virtual float       getStringWidth(const wchar_t* text) = 0;
		virtual float       getStringHeight(const wchar_t* text) = 0;
        virtual SoraVector  getStringDimensions(const wchar_t* text) = 0;
		virtual float       getHeight() const = 0;

		// get/set font size
		virtual uint32	getFontSize() const = 0;
		virtual void	setFontSize(uint32 size) = 0;

		// get width of a char
		virtual float	getWidthFromCharacter(wchar_t c, bool original = false) = 0;

		// font effects
		virtual void setCharRotation(float rot) = 0;
		virtual void setScale(float scale) = 0;
        
        // get/set line width
        virtual void  setLineWidth(float width) = 0;
        virtual float getLineWidth() const= 0;
        
        virtual float getCharRotation() const = 0;
        virtual float getScale() const = 0;
        
        // set/get line rotation
        virtual void    setLineRotation(float rot, bool rotateChar=false) = 0;
        virtual float   getLineRotation() const = 0;
        
        static SoraFont* LoadFromFile(const StringType& font, uint32 size);
        static SoraFont* LoadFromMemory(void* data, uint32 size, uint32 fontSize, const StringType& name);
        static void Release(SoraFont* font);
	};

} // namespace sora


#endif
