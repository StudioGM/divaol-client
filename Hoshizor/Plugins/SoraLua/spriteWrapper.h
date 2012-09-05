//
//  spriteWrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef _SPRITE_WRAPPER_H_
#define _SPRITE_WRAPPER_H_

#include "SoraSprite.h"
#include "SoraCore.h"

namespace spritewrapper {
    
    using namespace sora;
    
    SoraSpriteHandle createSprite(const SoraWString& path);
    SoraSpriteHandle createSpriteWithTex(SoraTextureHandle tex);
    SoraSpriteHandle createSpriteWithTexEx(SoraTextureHandle tex, float x, float y, float w, float h);
	SoraSpriteHandle createSpriteWH(int32 w, int32 h);
	
	SoraTextureHandle createTexture(const SoraWString& path);
	SoraTextureHandle createTextureWH(int32 w, int32 h);
	void releaseTexture(SoraTextureHandle tex);
    
    void setTextureRect	(SoraSpriteHandle h, float x, float y, float w, float fh);
    void setColor		(SoraSpriteHandle h, float r, float g, float b, float a);
	void setVertexColor (SoraSpriteHandle h, float r, float g, float b, float a, int32 i);
	void setAlpha		(SoraSpriteHandle h, float a);
    void setZ			(SoraSpriteHandle h, float z, int32 i=-1);
    uint32 getColor     (SoraSpriteHandle h, int32 i=-1);
    float getZ          (SoraSpriteHandle h, int32 i=-1);
    void setCenter		(SoraSpriteHandle h, float x, float y);
    float getCenterX	(SoraSpriteHandle h);
    float getCenterY	(SoraSpriteHandle h);
    void setFlip		(SoraSpriteHandle h, bool hf, bool vf);
    bool getVFlip		(SoraSpriteHandle h);
    bool getHFlip		(SoraSpriteHandle h);
    
    int32 getTextureWidth		(SoraSpriteHandle h);
    int32 getTextureHeight		(SoraSpriteHandle h);
    int32 getSpriteWidth		(SoraSpriteHandle h);
    int32 getSpriteHeight		(SoraSpriteHandle h);
    void setScale				(SoraSpriteHandle h, float vs, float hs);
    float getVScale             (SoraSpriteHandle h);
    float getHScale             (SoraSpriteHandle h);
    void setRotation			(SoraSpriteHandle h, float r);
    float getRotation			(SoraSpriteHandle h);
	SoraTextureHandle getTexture(SoraSpriteHandle h);

    void addEffect  (SoraSpriteHandle h, SoraHandle eff);
    void stopEffect (SoraSpriteHandle h, SoraHandle eff);
    void clearEffects (SoraSpriteHandle h);
    bool hasEffect  (SoraSpriteHandle h);
    
    void update(SoraSpriteHandle h);
    void render(SoraSpriteHandle h);
    void renderWithPos(SoraSpriteHandle h, float x, float y);
    
    float getPosX(SoraSpriteHandle h);
    float getPosY(SoraSpriteHandle h);
    void setPosition(SoraSpriteHandle h, float x, float y);
    
    void setBlendMode		(SoraSpriteHandle h, int32 mode);
    int32 getBlendMode		(SoraSpriteHandle h);
	
	SoraHandle attachShader(SoraSpriteHandle h, const SoraWString& shader, const SoraString& entry, int32 type);
	
	void setShaderParam1f(SoraHandle shader, const char* name, float param);
	void setShaderParam2f(SoraHandle shader, const char* name, float p1, float p2);
	void setShaderParam3f(SoraHandle shader, const char* name, float p1, float p2, float p3);
	void setShaderParam4f(SoraHandle shader, const char* name, float p1, float p2, float p3, float p4);
	
	void setShaderParam1i(SoraHandle shader, const char* name, int32 param);
	void setShaderParam2i(SoraHandle shader, const char* name, int32 p1, int32 p2);
	void setShaderParam3i(SoraHandle shader, const char* name, int32 p1, int32 p2, int32 p3);
	void setShaderParam4i(SoraHandle shader, const char* name, int32 p1, int32 p2, int32 p3, int32 p4);
	
	void setShaderTextureParam(SoraHandle shader, const char* name, SoraHandle tex);
    
    void release (SoraSpriteHandle h);
} // namespace spritewrapper


namespace imageeffectwrapper {
    
    using namespace sora;

    SoraHandle makeEffect_fade(float src, float dst, float time, int32 mode);
    SoraHandle makeEffect_transitions(float sx, float sy, float dx, float dy, float time, int32 mode);
    SoraHandle makeEffect_scale_tensile(float sv, float dv, float sh, float dh, float time, int32 mode);
    SoraHandle makeEffect_scale(float s, float d, float time, int32 mode);
    SoraHandle makeEffect_colorTransitions(uint32 start, uint32 end, float time, int32 mode);
    SoraHandle makeEffect_rotation(float start, float end, float time, int32 mode);
	SoraHandle makeEffect_rotationZ(float start, float zstart, float end, float zend, float time, int32 mode);
    
} // namespace iamgeeffectwrapper

#endif