/*
 *  fontWrapper.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/30/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef FONT_WRAPPER_H_
#define FONT_WRAPPER_H_

#include "SoraPlatform.h"
#include "SoraCore.h"
#include "SoraFont.h"

namespace fontWrapper {
	
	static SoraHandle createFont(const std::wstring& fontName, int32 size) {
		return (SoraHandle)sora::SoraCore::Ptr->createFont(fontName, size);
	}
	
	static void renderString(SoraHandle font, float x, float y, int32 alignment, const std::wstring& str) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->render(x, y, (sora::SoraFont::Alignment)alignment, str.c_str());
		}
	}
	
	static void setKerningWidth(SoraHandle font, float kw) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setKerningWidth(kw);
		}
	}
	
	static void setKerningHeight(SoraHandle font, float kh) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setKerningHeight(kh);
		}
	}
	
	static float getKerningWidth(SoraHandle font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getKerningWidth();
		}
		return 0.f;
	}
	
	static float getKerningHeight(SoraHandle font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getKerningHeight();
		}
		return 0.f;
	}
	
	static int32 getFontSize(SoraHandle font) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getFontSize();
		}
		return 0;
	}
	
	static void setColor(SoraHandle font, SoraHandle color, uint32 i) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			pfont->setColor(color, i);
		}
	}

	static SoraHandle getColor(SoraHandle font, uint32 i) {
		sora::SoraFont* pfont= (sora::SoraFont*)font;
		if(font) {
			return pfont->getColor(i);
		}
		return 0;
	}
}

#endif