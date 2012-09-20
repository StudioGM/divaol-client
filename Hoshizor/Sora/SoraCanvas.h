/*
 *  SoraCanvas.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/7/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CANVAS_H_
#define SORA_CANVAS_H_

#include "SoraPlatform.h"
#include "SoraSprite.h"

#include "signal/SoraSignal.h"

namespace sora {
	
	/*
		Base class for canvas which render things onto a texture, using RenderTarget
	 */
	class SORA_API SoraBaseCanvas: public SoraShaderable, public SoraObject {
	public:
        SoraBaseCanvas(int32 width, int32 height, bool bDepthBuffer=true);
        virtual ~SoraBaseCanvas();
        
        void render();
        int32 update(float dt);
        
        void beginRender(uint32 col=0);
        void finishRender();
        
		SoraSprite* getCanvasSprite() const;

		virtual void on3DEnabled(bool flag);

	private:
		SoraHandle canvasTarget;
        SoraSprite* pCanvasSprite;
        
        bool bSceneBegan;
	};
	
} // namespace sora

#endif