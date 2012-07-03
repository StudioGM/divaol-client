/*
 *  SoraCamera.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CAMERA_H_
#define SORA_CAMERA_H_

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraModifierAdapter.h"

namespace sora {
    
    class SORA_API SoraCamera: public SoraObject {
    public:
        virtual void apply() = 0;
    };
	
	class SORA_API Sora2DCamera: public SoraCamera {
	public:
        Sora2DCamera();
		Sora2DCamera(float x, float y, float viewWidth, float viewHeight);
		~Sora2DCamera();
		
        void    setZoom(float h, float v);
        void    setRotation(float rot);
        
        void    setViewRect(float width, float height);
        float   getViewWidth() const;
        float   getViewHeight() const;
            
        void    getZoom(float* h, float* v);
        float   getRotation() const;
        float   getHZoom() const;
        float   getVZoom() const;
        
        void    setPosition(float x, float y);
        /**
         *  Modifier helper functions
         *  Use SoraModifierAdapter as applier
         **/
        void moveTo(float x, float y, float inTime);
        void rotateTo(float newRot, float inTime);
        void zoomTo(float hz, float vz, float inTime);
        
        /**
         *  Transform to another camera
         *  Position, rotation and zoom applies
         **/
        void transformTo(Sora2DCamera* r, float inTime);
        		
        virtual int32 update(float dt);
        virtual void render();
        
        void apply();
        
	private:
        float mHZoom;
        float mVZoom;
        float mRotation;
        
        SoraRect mViewRect;
        
        bool mPropChanged;
        
        typedef SoraModifierAdapter<Sora2DCamera> CameraModifier;
	};
  
	
} // namespace sora

#endif // SORA_CAMERA_H_