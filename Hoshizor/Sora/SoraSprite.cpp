#include "SoraSprite.h"
#include "SoraModifierAdapter.h"
#include "SoraTextureMap.h"
#include "SoraCore.h"

#include "physics/SoraPhysicBody.h"

#ifdef OS_IOS
#include "SoraiOSDeviceHelper.h"
#endif

namespace sora {
    
    SoraCore* SoraSprite::mSora = NULL;
    
    SoraSprite::SoraSprite() {
        if(!mSora)
            mSora = SoraCore::Instance();

        _initDefaults();
    }

    SoraSprite::SoraSprite(SoraTextureHandle tex) {
        if(!mSora)
            mSora = SoraCore::Instance();
        
        SoraTexture* ptex = tex==0?NULL:(SoraTexture*)tex;
        
        _init(ptex, 0.f, 0.f, ptex!=NULL?ptex->mTextureWidth:1.f, ptex!=NULL?ptex->mTextureHeight:1.f);
		_initDefaults();
    }
    
    SoraSprite::SoraSprite(SoraTextureHandle tex, float x, float y, float w, float h) {
        if(!mSora)
            mSora = SoraCore::Instance();
        
        if(w == 0.f)
            w = (float)SoraTexture::GetWidth(tex);
        if(h == 0.f)
            h = (float)SoraTexture::GetHeight(tex);
        
        _init((SoraTexture*)tex, x, y, w, h);
		_initDefaults();
		setTextureRect(x, y, w, h);
    }
    
    void SoraSprite::_init(SoraTexture* tex, float x, float y, float width, float height) {
        float texx1, texy1, texx2, texy2;
		
		mTextureRect.x1 = x; mTextureRect.y1 = y;
		if(tex) {
			mTextureRect.x2 = width; mTextureRect.y2 = height;
		} else {
			mTextureRect.x2 = 1.f;
			mTextureRect.y2 = 1.f;
		}
		
		mQuad.tex = tex;
		
		texx1=x/mTextureRect.x2;
		texy1=y/mTextureRect.y2;
		texx2=(x+width)/mTextureRect.x2;
		texy2=(y+height)/mTextureRect.y2;

		mQuad.v[0].tx = texx1; mQuad.v[0].ty = texy1;
		mQuad.v[1].tx = texx2; mQuad.v[1].ty = texy1;
		mQuad.v[2].tx = texx2; mQuad.v[2].ty = texy2;
		mQuad.v[3].tx = texx1; mQuad.v[3].ty = texy2;
		
		if(tex) {
	       mSprWidth = static_cast<int32>(tex->mOriginalWidth);
	       mSprHeight = static_cast<int32>(tex->mOriginalHeight);
		} else {
			mSprWidth = 1;
			mSprHeight = 1;
		}
		if(mQuad.tex && SoraTextureMap::Instance()->exist((SoraTextureHandle)tex)) {
			setName(SoraTextureMap::Instance()->getTextureName((SoraTextureHandle)tex));
			SoraTextureMap::Instance()->addRf((SoraTextureHandle)tex);
		}
    }

	SoraSprite::~SoraSprite() {
		clearEffects();
		//SoraTextureMap::Instance()->decRf((sora::SoraTextureHandle)mQuad.tex);
	}

	void SoraSprite::setTexture(SoraTextureHandle tex) {
        if(mQuad.tex) {
            if(SoraTextureMap::Instance()->exist((SoraTextureHandle)mQuad.tex)) {
                SoraTextureMap::Instance()->decRf((SoraTextureHandle)mQuad.tex);
            }
        }
        
		SoraTexture* ptex = (SoraTexture*)tex;
		if(ptex)
			_init(ptex, 0.f, 0.f, (float)ptex->mTextureWidth, (float)ptex->mTextureHeight);
		else
			_init(NULL, 0.f, 0.f, 1.f, 1.f);
	}

	void SoraSprite::_initDefaults() {
		mVScale = mHScale = 1.f;
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            if(mQuad.tex && !mQuad.tex->mIsRetinaTexture) {
                mVScale = mHScale = getScaleFactor();
            }
        }
#endif
        
		mRotation = 0.f;
		mCenterX = mCenterY = 0.f;

        bVFlip = bHFlip = false;
            
        // force rebuild quad
        bPropChanged = true;
        
        if(hasEffect())
			clearEffects();
		setPosition(0.f, 0.f);
        
		mQuad.blend=BLEND_DEFAULT;
    }

	void SoraSprite::setTextureRect(float x, float y, float width, float height) {
		float tx1, ty1, tx2, ty2;
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            float scale = sora::getScaleFactor();
            x *= scale; y *= scale;
            width *= scale; height *= scale;
        }
#endif
		
		mTextureRect.x1=x;
		mTextureRect.y1=y;
		mTextureRect.x2=width;
		mTextureRect.y2=height;
        
        mSprWidth = (int32)width;
        mSprHeight = (int32)height;
        if(!mQuad.tex)
			return;

		tx1=mTextureRect.x1/mQuad.tex->mTextureWidth; 
        ty1=mTextureRect.y1/mQuad.tex->mTextureHeight;
		
        tx2=(mTextureRect.x1+width)/mQuad.tex->mTextureWidth; 
        ty2=(mTextureRect.y1+height)/mQuad.tex->mTextureHeight;
		
		mQuad.v[0].tx=tx1; mQuad.v[0].ty=ty1; 
		mQuad.v[1].tx=tx2; mQuad.v[1].ty=ty1; 
		mQuad.v[2].tx=tx2; mQuad.v[2].ty=ty2; 
		mQuad.v[3].tx=tx1; mQuad.v[3].ty=ty2; 
        
        bPropChanged = true;
	}
    
    void SoraSprite::onPositionChange(float x, float y, float z) {
        bPropChanged = true;
    }
    
    void SoraSprite::on3DEnabled(bool flag) {
        setZ(0.f);
    }
	
	void SoraSprite::render() {
        if(bPropChanged) {
            buildQuad(getPositionX(), getPositionY());
            bPropChanged = false;
        }
		
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
    }
    
    void SoraSprite::buildQuad(float x, float y) {
        float tx1, ty1, tx2, ty2;
		float sint, cost;
        
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            x *= sora::getScaleFactor();
            y *= sora::getScaleFactor();
        }
#endif
		
        if(!is3DEnabled()) {
            tx1 = -mCenterX*mHScale;
            ty1 = -mCenterY*mVScale;
            tx2 = (mTextureRect.x2-mCenterX)*mHScale;
            ty2 = (mTextureRect.y2-mCenterY)*mVScale;
            
            if(mRotation != 0.0f) {
                cost = cosf(mRotation);
                sint = sinf(mRotation);
                
                mQuad.v[0].x  = tx1*cost - ty1*sint + x;
                mQuad.v[1].x  = tx2*cost - ty1*sint + x;
                mQuad.v[2].x  = tx2*cost - ty2*sint + x;
                mQuad.v[3].x  = tx1*cost - ty2*sint + x;
                
                mQuad.v[0].y  = tx1*sint + ty1*cost + y;	
                mQuad.v[1].y  = tx2*sint + ty1*cost + y;	
                mQuad.v[2].y  = tx2*sint + ty2*cost + y;	
                mQuad.v[3].y  = tx1*sint + ty2*cost + y;
            }
            else {
                mQuad.v[0].x = tx1 + x; 
                mQuad.v[1].x = tx2 + x; 
                mQuad.v[2].x = mQuad.v[1].x; 
                mQuad.v[3].x = mQuad.v[0].x; 
                
                mQuad.v[0].y = ty1 + y;
                mQuad.v[1].y = mQuad.v[0].y;
                mQuad.v[2].y = ty2 + y;
                mQuad.v[3].y = mQuad.v[2].y;
            }
        } else {
            tx1 = -mCenterX*mHScale;
            ty1 = -mCenterY*mVScale;
            tx2 = (mTextureRect.x2-mCenterX)*mHScale;
            ty2 = (mTextureRect.y2-mCenterY)*mVScale;
            
            mQuad.v[0].x = tx1; 
            mQuad.v[1].x = tx2; 
            mQuad.v[2].x = tx2; 
            mQuad.v[3].x = tx1; 
            
            mQuad.v[0].y = ty1;
            mQuad.v[1].y = ty1;
            mQuad.v[2].y = ty2;
            mQuad.v[3].y = ty2;
            
      //      printf("1. %f, %f, %f, %f\n", mQuad.v[0].x, mQuad.v[1].x, mQuad.v[0].y, mQuad.v[2].y);
            
            const SoraTransform& transform = getTransform();
            SoraMatrix4 mat = SoraMatrix4::RotMat(transform.mRotation.x, 
                                                  transform.mRotation.y, 
                                                  transform.mRotation.z).scale(transform.mScale.x,
                                                                               transform.mScale.y,
                                                                               transform.mScale.z);
            mat.translate(x, y, 0.f);
            mQuad *= mat;
            
      //      printf("2. %f, %f, %f, %f\n", mQuad.v[0].x, mQuad.v[1].x, mQuad.v[0].y, mQuad.v[2].y);
        }
    }

	void SoraSprite::render(float x, float y) {
        buildQuad(x, y);
        bPropChanged = true;
		
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
	}
    
    void SoraSprite::renderEx(float x, float y, float rot, float scalex, float scaley) {
        float tx1, ty1, tx2, ty2;
		float sint, cost;
        
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            x *= sora::getScaleFactor();
            y *= sora::getScaleFactor();
        }
#endif
        
		tx1 = -mCenterX*scalex;
		ty1 = -mCenterY*scaley;
		tx2 = (mTextureRect.x2-mCenterX)*scalex;
		ty2 = (mTextureRect.y2-mCenterY)*scaley;
		
		if(mRotation != 0.0f) {
			cost = cosf(rot);
			sint = sinf(rot);
			
			mQuad.v[0].x  = tx1*cost - ty1*sint + x;
			mQuad.v[1].x  = tx2*cost - ty1*sint + x;
			mQuad.v[2].x  = tx2*cost - ty2*sint + x;
			mQuad.v[3].x  = tx1*cost - ty2*sint + x;
            
			mQuad.v[0].y  = tx1*sint + ty1*cost + y;	
            mQuad.v[1].y  = tx2*sint + ty1*cost + y;	
            mQuad.v[2].y  = tx2*sint + ty2*cost + y;	
            mQuad.v[3].y  = tx1*sint + ty2*cost + y;
		}
		else {
			mQuad.v[0].x = tx1 + x; 
			mQuad.v[1].x = tx2 + x; 
			mQuad.v[2].x = mQuad.v[1].x; 
			mQuad.v[3].x = mQuad.v[0].x; 
            
			mQuad.v[0].y = ty1 + y;
            mQuad.v[1].y = mQuad.v[0].y;
            mQuad.v[2].y = ty2 + y;
            mQuad.v[3].y = mQuad.v[2].y;
		}
        
        bPropChanged = true;
        
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
    }
    
    void SoraSprite::renderInBox(float x1, float y1, float x2, float y2) {
        mQuad.v[0].x = x1; mQuad.v[0].y = y1;
        mQuad.v[1].x = x2; mQuad.v[1].y = y1;
        mQuad.v[2].x = x2; mQuad.v[2].y = y2;
        mQuad.v[3].x = x1; mQuad.v[3].y = y2;
        
        bPropChanged = true;
        
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
    }

	void SoraSprite::render4V(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
#ifdef OS_IOS
        float scale = sora::getScaleFactor();
        
		mQuad.v[0].x = x1 * scale; mQuad.v[0].y = y1 * scale;
		mQuad.v[1].x = x2 * scale; mQuad.v[1].y = y2 * scale;
		mQuad.v[2].x = x3 * scale; mQuad.v[2].y = y3 * scale;
		mQuad.v[3].x = x4 * scale; mQuad.v[3].y = y4 * scale;
#else
		mQuad.v[0].x = x1; mQuad.v[0].y = y1;
		mQuad.v[1].x = x2; mQuad.v[1].y = y2;
		mQuad.v[2].x = x3; mQuad.v[2].y = y3;
		mQuad.v[3].x = x4; mQuad.v[3].y = y4;
#endif
        
        bPropChanged = true;
		
        attachShaderToRender();
		mSora->renderQuad(mQuad);
        detachShaderFromRender();
	}

	void SoraSprite::renderWithVertices(SoraVertex* vertices, uint32 size, RenderMode mode) {
		attachShaderToRender();
		mSora->renderWithVertices((SoraTextureHandle)mQuad.tex, mQuad.blend, vertices, size, mode);
		detachShaderFromRender();
	}
	
	void SoraSprite::setColor(uint32 c, int32 i) {
		if(i != -1)
			mQuad.v[i].col = c;
		else
			mQuad.v[0].col = mQuad.v[1].col = mQuad.v[2].col = mQuad.v[3].col = c;
	}

	uint32 SoraSprite::getColor(int32 i)  const{
		return mQuad.v[i].col;
	}
	
	void SoraSprite::setZ(float z, int32 i) {
		if(i != -1)
			mQuad.v[i].z = z;
		else
			mQuad.v[0].z = mQuad.v[1].z = mQuad.v[2].z = mQuad.v[3].z = z;
	}

	float SoraSprite::getZ(int32 i)  const{
		return mQuad.v[i].z;
	}

	void SoraSprite::setBlendMode(int32 mode) {
		mQuad.blend = mode;
	}

	int32 SoraSprite::getBlendMode() const {
		return mQuad.blend;
	}

	uint32* SoraSprite::getPixelData() const {
		return mSora->textureLock((SoraTextureHandle)mQuad.tex);
		return 0;
	}
    
    void SoraSprite::unlockPixelData() {
        mSora->textureUnlock((SoraSpriteHandle)mQuad.tex);
    }

	SoraRect SoraSprite::getTextureRect() const {
		return mTextureRect;
	}

	void SoraSprite::setCenter(float x, float y) { 
		mCenterX = x;
		mCenterY = y;
        
        bPropChanged = true;
	}

	void SoraSprite::getCenter(float& x, float& y) {
		y = mCenterY;
		x = mCenterX;
	}

	void SoraSprite::setFlip(bool hflag, bool vflag, bool bFlipCenter) {
		float tx, ty;
        
        if(bCFlip & bHFlip) mCenterX = getSpriteWidth() - mCenterX;
        if(bCFlip & bVFlip) mCenterY = getSpriteHeight() - mCenterY;
        
        bCFlip = bFlipCenter;
        
        if(bCFlip & bHFlip) mCenterX = getSpriteWidth() - mCenterX;
        if(bCFlip & bVFlip) mCenterY = getSpriteHeight() - mCenterY;
        
        if(hflag != bHFlip) {
            tx=mQuad.v[0].tx; mQuad.v[0].tx=mQuad.v[1].tx; mQuad.v[1].tx=tx;
            ty=mQuad.v[0].ty; mQuad.v[0].ty=mQuad.v[1].ty; mQuad.v[1].ty=ty;
            tx=mQuad.v[3].tx; mQuad.v[3].tx=mQuad.v[2].tx; mQuad.v[2].tx=tx;
            ty=mQuad.v[3].ty; mQuad.v[3].ty=mQuad.v[2].ty; mQuad.v[2].ty=ty;
            
            bHFlip = !bHFlip;
        }
        
        if(vflag != bVFlip) {
            tx=mQuad.v[0].tx; mQuad.v[0].tx=mQuad.v[3].tx; mQuad.v[3].tx=tx;
            ty=mQuad.v[0].ty; mQuad.v[0].ty=mQuad.v[3].ty; mQuad.v[3].ty=ty;
            tx=mQuad.v[1].tx; mQuad.v[1].tx=mQuad.v[2].tx; mQuad.v[2].tx=tx;
            ty=mQuad.v[1].ty; mQuad.v[1].ty=mQuad.v[2].ty; mQuad.v[2].ty=ty;
            
            bVFlip = !bVFlip;
        }
    
        bPropChanged = true;
	}

	int32 SoraSprite::getTextureWidth(bool bOriginal)  const{
		if(mQuad.tex) 
            return bOriginal? mQuad.tex->mOriginalWidth: mQuad.tex->mTextureWidth;
		return 0;
	}

	int32 SoraSprite::getTextureHeight(bool bOriginal) const {
		if(mQuad.tex) 
            return bOriginal? mQuad.tex->mOriginalHeight: mQuad.tex->mTextureHeight;
		return 0;
	}

	int32 SoraSprite::getSpriteWidth()  const{
		return (int32)(mSprWidth);
	}

	int32 SoraSprite::getSpriteHeight()  const{
		return (int32)(mSprHeight);
	}

	void SoraSprite::setScale(float h, float v) {
		mVScale = v;
		mHScale = h;
        
#ifdef OS_IOS
        if(_IS_RETINA_DISPLAY() && isUseRetina()) {
            if(mQuad.tex && !mQuad.tex->mIsRetinaTexture) {
                mVScale *= getScaleFactor();
                mHScale *= getScaleFactor();
            }
        }
#endif
        
        bPropChanged = true;
	}

	float SoraSprite::getVScale()  const{
		return mVScale;
	}

	float SoraSprite::getHScale()  const{
		return mHScale;
	}

	void SoraSprite::setRotation(float r) { 
		mRotation = r;
        
        bPropChanged = true;
	}

	float SoraSprite::getRotation()  const{
		return mRotation;
	}

	bool SoraSprite::getHFlip()  const{ 
		return bHFlip;
	}

	bool SoraSprite::getVFlip()  const{ 
		return bVFlip;
	}

	float SoraSprite::getCenterX()  const{
		return mCenterX;
	}

	float SoraSprite::getCenterY()  const{
		return mCenterY;
	}

	void SoraSprite::addEffect(SoraImageEffect* effect) {
		 vEffects.push_back(effect);
	}
	
	void SoraSprite::stopEffect(SoraImageEffect* _eff) {
		ImageEffectList::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
			if((*eff) == _eff) {
				delete _eff;
				_eff = 0;
				eff = vEffects.erase(eff);
				return;
			} else
				++eff;
		}
	}
	
	void SoraSprite::clearEffects() {
		ImageEffectList::iterator eff = vEffects.begin();
		while(eff != vEffects.end()) {
			if((*eff) != NULL) {
				delete (*eff);
				(*eff) = 0;
			}
			++eff;
		}
		vEffects.clear();
	}
	
	int32 SoraSprite::update(float dt) {
        SoraObject::update(dt);
        
		if(!vEffects.empty()) {
			ImageEffectList::iterator eff = vEffects.begin();
			while(eff != vEffects.end()) {
				bool result = (*eff)->update(dt);
				(*eff)->modify(this);
				
				if(result) {     
					(*eff)->release();
					eff = vEffects.erase(eff);
					continue;
				}
				++eff;
			}
		}
        
        if(mPhysicBody) {
            SoraVector ccenter = mPhysicBody->getLocalCenter();
            setCenter(ccenter.x, ccenter.y);
            
            setRotation(mPhysicBody->getRotation());
        }
		return 0;
	}
    
    SoraTextureHandle SoraSprite::getTexture() const {
        return (SoraTextureHandle)mQuad.tex;
    }
    
    bool SoraSprite::hasEffect() const {
        return !vEffects.empty();
    }
    
    SoraRect SoraSprite::getBoundingBox() const {
        SoraRect rect;
        
        float tx1, ty1, tx2, ty2;
		float sint, cost;
        
		tx1 = -mCenterX*mHScale;
		ty1 = -mCenterY*mVScale;
		tx2 = (mTextureRect.x2-mCenterX)*mHScale;
		ty2 = (mTextureRect.y2-mCenterY)*mVScale;
		
		cost = cosf(mRotation);
        sint = sinf(mRotation);
			
        rect.x1  = tx1*cost - ty1*sint + getPositionX();
        rect.x2  = tx2*cost - ty2*sint + getPositionX();
        rect.y1  = tx1*sint + ty1*cost + getPositionY();	 
        rect.y2  = tx2*sint + ty2*cost + getPositionY();
        
        return rect;
    }
    
    SoraSprite* SoraSprite::LoadFromFile(const StringType& file) {
        SoraTextureHandle tex = SoraTexture::LoadFromFile(file);
        if(tex)
            return new SoraSprite(tex);
        return 0;
    }
    
    SoraSprite* SoraSprite::LoadFromRawData(uint32* data, int32 w, int32 h) {
        SoraTextureHandle tex = SoraTexture::LoadFromRawData(data, w, h);
        if(tex)
            return new SoraSprite(tex);
        return 0;
    }
    
    SoraSprite* SoraSprite::LoadFromMemory(uint32* data, uint32 size) {
        SoraTextureHandle tex = SoraTexture::LoadFromMemory(data, size);
        if(tex)
            return new SoraSprite(tex);
        return 0;
    }
    
    void SoraSprite::Render(const StringType& file, float x, float y, float r, float sh, float sv) {
  //      SoraFastRenderer::Instance()->renderSprite(file, x, y, r, sh, sv);
    }
    
    void SoraSprite::fadeTo(float to, float t) {
        addModifierAdapter(CreateModifierAdapter(this, CreateEffectFade(CGETA(getColor())/255.f,
                                                                        to,
                                                                        t)));
    }
    
    void SoraSprite::rotateTo(float to, float t) {
        addModifierAdapter(CreateModifierAdapter(this, CreateEffectRotation(getRotation(),
                                                                            to,
                                                                            t)));
    }
    
    void SoraSprite::scaleTo(float h, float v, float t) {
        addModifierAdapter(CreateModifierAdapter(this, CreateEffectScale(getHScale(),
                                                                         getVScale(),
                                                                         h,
                                                                         v,
                                                                         t)));
    }
    
    void SoraSprite::fadeToAndNotify(float to, float t, const NotificationFunc& func) {
        addModifierAdapter(CreateModifierAdapterWithNotification(this, CreateEffectFade(CGETA(getColor())/255.f,
                                                                                        to,
                                                                                        t),
                                                                 func));
    }
    
    void SoraSprite::rotateToAndNotify(float to, float t, const NotificationFunc& func) {
        addModifierAdapter(CreateModifierAdapterWithNotification(this, CreateEffectRotation(getRotation(),
                                                                                            to,
                                                                                            t),
                                                                 func));
    }
    
    void SoraSprite::scaleToAndNotify(float h, float v, float t, const NotificationFunc& func) {
        addModifierAdapter(CreateModifierAdapterWithNotification(this, CreateEffectScale(getHScale(),
                                                                                         getVScale(),
                                                                                         h,
                                                                                         v,
                                                                                         t),
                                                                 func));
    }
	
} // namespace sora