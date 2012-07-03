/*
 *  SoraMutableSprite.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/10/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MUTABLE_SPRITE_H_
#define SORA_MUTABLE_SPRITE_H_

#include "SoraSprite.h"

namespace sora {
	
	class SORA_API SoraMutableSprite: public SoraSprite {
	public:
		SoraMutableSprite(SoraTextureHandle tex);
		
		uint32 getPixel(int32 x, int32 y);
		void   putPixel(int32 x, int32 y, uint32 col);
		
		void putPixelArray(int32 xstart, int32 ystart, int32 width, int32 height, uint32* col);
		
		virtual void render();
		virtual void render(float x, float y);
		virtual void render4V(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
		virtual void renderWithVertices(SoraVertex* vertices, uint32 size, RenderMode mode);
		
	private:
		inline void applyChange();
		
		uint32* mPixelData;
		bool mChanged;
	};
	
} // namespace sora


#endif // SORA_MUTABLE_SPRITE_H_