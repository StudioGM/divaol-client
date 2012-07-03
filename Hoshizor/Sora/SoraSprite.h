#ifndef SORA_SPRITE_H_
#define SORA_SPRITE_H_

#include "SoraTexture.h"
#include "SoraColor.h"
#include "SoraMath.h"
#include "SoraImageEffect.h"
#include "SoraObject.h"
#include "SoraShaderable.h"
#include "SoraVertex.h"

#include <list>

namespace sora {

	class SoraCore;

	class SORA_API SoraSprite: public SoraShaderable, public SoraObject {
	public:
        SoraSprite();
        SoraSprite(SoraTextureHandle tex);
        SoraSprite(SoraTextureHandle tex, float x, float y, float width, float height);
		virtual ~SoraSprite();

        virtual void render();
        virtual void render(float x, float y);
        virtual void renderInBox(float x1, float y1, float x2, float y2);
        virtual void render4V(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
        virtual void renderEx(float x, float y, float rot, float scalex, float scaley);
		virtual void renderWithVertices(SoraVertex* vertices, uint32 size, RenderMode mode);

		virtual int32 update(float dt);

		void     setTexture(SoraTextureHandle tex);
		void     setTextureRect(float x, float y, float width, float height);
		SoraRect getTextureRect() const;
		
		void    setColor(uint32 c, int32 i=-1);
		uint32  getColor(int32 i=0) const;

		void    setZ(float z, int32 i=-1);
		float   getZ(int32 i=0) const;
        
		void    setCenter(float x, float y);
		void    getCenter(float& x, float& y);
		float   getCenterX() const;
		float   getCenterY() const;

		void    setFlip(bool hflag, bool vflag, bool bFlipCenter=true);
		bool    getHFlip() const;
		bool    getVFlip() const;

		void    setBlendMode(int32 mode);
		int32   getBlendMode() const;

		int32   getTextureWidth(bool bOriginal=true) const;
		int32   getTextureHeight(bool bOriginal=true) const;
		int32   getSpriteWidth() const;
		int32   getSpriteHeight() const;
		int32   getSpritePosX() const;
		int32   getSpritePosY() const;

		void    setScale(float h, float v);
		float   getVScale() const;
		float   getHScale() const;

		void    setRotation(float r);
		float   getRotation() const;
        		
		uint32*              getPixelData() const;
        void                 unlockPixelData();
		SoraTextureHandle    getTexture() const;
        
    public:
        /**
         * inheritated from SoraObject
         **/
        void onPositionChange(float x, float y, float z);
        void on3DEnabled(bool flag);
        
    public:
        /**
         * effects and mofiers
         **/
		
		void addEffect(SoraImageEffect* effect);
		void stopEffect(SoraImageEffect* effect);
		void clearEffects();
		bool hasEffect() const;
        
        void fadeTo(float to, float t);
        void rotateTo(float to, float t);
        void scaleTo(float h, float v, float t);
        
        typedef SoraFunction<void(SoraSprite*)> NotificationFunc;
        
        void fadeToAndNotify(float to, float t, const NotificationFunc& func);
        void rotateToAndNotify(float to, float t, const NotificationFunc& func);
        void scaleToAndNotify(float h, float v, float t, const NotificationFunc& func);
        
        SoraRect getBoundingBox() const;
        
    public:
        /** 
         * static functions
         **/
        
        static SoraSprite* LoadFromFile(const StringType& file);
        static SoraSprite* LoadFromRawData(uint32* data, int32 w, int32 h);
        static SoraSprite* LoadFromMemory(uint32* data, uint32 size);
        static void Render(const StringType& file, float x, float y, float r=0.f, float sh=1.f, float sv=1.f);
		
	protected:        
        inline void _initDefaults();
        inline void _init(SoraTexture* tex, float x, float y, float w, float h);

        inline void buildQuad(float x, float y);
		
		SoraRect mTextureRect;
        int32   mSprWidth, mSprHeight;
        
		float mRotation;
		float mCenterX, mCenterY;
		float mVScale, mHScale;

		bool bVFlip, bHFlip, bCFlip;
        bool bPropChanged;
		
		typedef std::list<SoraImageEffect*> ImageEffectList;
		ImageEffectList vEffects;

		SoraQuad mQuad;

        static SoraCore* mSora;
        
	private:
		SoraSprite(SoraSprite&);
    };

} // namespace sora

#endif