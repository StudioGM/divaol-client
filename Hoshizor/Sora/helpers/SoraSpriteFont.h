/*
 *  SoraSpriteFont.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/11/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SPRITE_FONT_H_
#define SORA_SPRITE_FONT_H_

#include "SoraFont.h"

namespace sora {
	
	class SORA_API SoraSpriteFont: public SoraFont {
	public:
		SoraSpriteFont(const SoraWString& font, const SoraString& sequance, int32 width, int32 height);
		~SoraSpriteFont();
		
		void	render(float x, float y, const wchar_t* text, bool hcenter = false, bool vcenter = false);
		void	print(float x, float y, int32 align, const wchar_t *format, ...);
		void	render(float x, float y, int32 align, const wchar_t* text);
		
		void	setColor(uint32 dwColor, int32 i = -1);
		uint32	getColor(int32 i);
		
		void	setKerningWidth(float kerning);
		void	setKerningHeight(float kerning);
		
		float	getKerningWidth();
		float	getKerningHeight();
		
		float	getStringWidth(const wchar_t* text);
		float   getStringHeight(const wchar_t* text);
		float	getHeight();
		
		uint32	getFontSize();
		
		float	getWidthFromCharacter(wchar_t c, bool original = false);
		
		void setCharRotation(float rot);
		void setScale(float scale);

	private:
		inline void buildPosMap(const SoraString&);
		
		SoraSprite* mFontSprite;
		
		int32 mCharPos[128];
		
		int32 mWidth, mHeight;
		float mRotation, mScale;
		float mKerningWidth, mKerningHeight;
	}; 
	
	
} // namespace sora


#endif // SORA_SPRITE_FONT_H_