#ifndef SORA_IMAGE_EFFECT_H
#define SORA_IMAGE_EFFECT_H

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraColor.h"
#include "SoraModifier.h"
#include "math/SoraCoreTransform.h"
#include "math/SoraCoreTransformer.h"
#include <list>

namespace sora {
	
#define IMAGE_EFFECT_TYPE 0x00001000
    
    class SoraSprite;
	
	enum ImageEffectType {
		ImageEffectNone = 0,

		ImageEffectFadeIn = 1,
		ImageEffectFadeOut  = 2,

		ImageEffectTransitions = 4,
		ImageEffectTransitionsZ = 8,

		ImageEffectScaleIn = 16,
		ImageEffectScaleOut = 32,
		ImageEffectTensile = 64,

		ImageEffectShake = 128,
		
		ImageEffectColorTransitions = 256,
		
		ImageEffectRotate = 512,
		ImageEffectRotateZ = 1024,
	};

	enum ImageEffectState {
		ImageEffectEnd = -1,
		ImageEffectNotStart = 0,
		ImageEffectPlaying = 1,
		ImageEffectPaused = 2,
	};

	enum ImageEffectMode {
		ImageEffectNull = 0,
		ImageEffectOnce = 1,
		ImageEffectPingpong = 2,
		ImageEffectRepeat = 3,
	};
	
	class SORA_API SoraImageEffect: public SoraModifier<SoraSprite> {
	public:
		SoraImageEffect();
		SoraImageEffect(SoraCoreTransformer<SoraCoreTransform>* transformer);
		SoraImageEffect(ImageEffectMode _mode);
		virtual ~SoraImageEffect();

		void stop();

		void pause();
		void pauseForTime(float t);
		void resume();
        
        bool finished() const ;

		float getTime();

		ImageEffectState getState() const { return states; }
		ImageEffectType getType() const  { return etype; }
		ImageEffectMode getMode() const { return mode; }

		virtual void start(ImageEffectMode mode, float time);
		virtual bool update(float delta);
		
		virtual void modify(SoraSprite* sprite) = 0;
		
		void restart();
		
		void setTransformer(SoraCoreTransformer<SoraCoreTransform>* transformer);

		float get1st() const { return t_curr.Get1st(); }
		float get2nd() const { return t_curr.Get2nd(); }
		float get3rd() const { return t_curr.Get3rd(); }
		float get4th() const { return t_curr.Get4th(); }
		
		SoraImageEffect& add(SoraImageEffect* next);
        
        float getEffectTime() const { return effectTime; }
		
		void setRepeatTimes(int32 times);
		uint32 getRepeatTimes() const;
		
		void swap();
        
        virtual SoraModifier<SoraSprite>* clone() = 0;
        
        SoraImageEffect& operator =(const SoraImageEffect& rhs);
        
	protected:
		bool checkRepeatTimes();

		ImageEffectType etype;
		ImageEffectState states;
		ImageEffectMode mode;

		float startTime;
		float pauseTime;
		float topauseTime;
		float effectTime;
		
		int32 currRepeatTimes;
		int32 repeatTimes;
		
		uint8 pingpongState;
		
		uint8 started;
		uint8 paused;

		SoraCoreTransform t_src;
		SoraCoreTransform t_dst;
		SoraCoreTransform t_curr;
		
		SoraCoreTransformer<SoraCoreTransform>* t_transformer;
	};
	
	class SORA_API SoraImageEffectList: public SoraImageEffect {
	public:
		SoraImageEffectList();
		SoraImageEffectList(ImageEffectMode mode);
		virtual ~SoraImageEffectList();
		
		SoraImageEffectList* add(SoraImageEffect* effect);

		SoraImageEffect* getListHead() const;
		SoraImageEffect* getListTail() const;
		
		void setListMode(ImageEffectMode mode);
		ImageEffectMode getListMode() const;
		
		virtual bool update(float delta);
		virtual void start(ImageEffectMode mode, float time);
		
		virtual void modify(SoraSprite* obj);
        
        SoraModifier<SoraSprite>* clone();
        		
	private:
		typedef std::list<SoraImageEffect*> ImageEffectList;
		ImageEffectList mImageEffects;
		
		SoraImageEffect* mCurrEffect;
		
		bool mReverse;
		
		ImageEffectMode mListMode;
	};

	class SORA_API SoraImageEffectFade: public SoraImageEffect {
	public:
		SoraImageEffectFade(float src, float dst, float time, 
							ImageEffectMode mode=ImageEffectOnce,
							SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
        void modify(SoraSprite* obj);
        SoraModifier<SoraSprite>* clone();
	};

	class SORA_API SoraImageEffectShake: public SoraImageEffect {
	public:
		SoraImageEffectShake(float mV, float mH, float time, 
							 ImageEffectMode mode=ImageEffectOnce,
							 SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
		void modify(SoraSprite* obj) {}
		SoraModifier<SoraSprite>* clone() { return NULL; }
	};

	class SORA_API SoraImageEffectScale: public SoraImageEffect {
	public:
		SoraImageEffectScale(float src, float dst, float time, 
							 ImageEffectMode mode=ImageEffectOnce,
							 SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		SoraImageEffectScale(float srcV, float dstV, float srcH, float dstH, float time, 
							 ImageEffectMode mode=ImageEffectOnce,
							 SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
        void modify(SoraSprite* obj);
        SoraModifier<SoraSprite>* clone();
	};

	class SORA_API SoraImageEffectTransitions: public SoraImageEffect {
	public:
		SoraImageEffectTransitions(float sx, float sy, float sz, float dx, float dy, float dz, float time, 
								   ImageEffectMode mode=ImageEffectOnce,
								   SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		SoraImageEffectTransitions(float sx, float sy, float dx, float dy, float time, 
								   ImageEffectMode mode=ImageEffectOnce,
								   SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
        void modify(SoraSprite* obj);
        SoraModifier<SoraSprite>* clone();
	};
	
	class SORA_API SoraImageEffectColorTransitions: public SoraImageEffect {
	public:
		SoraImageEffectColorTransitions(const SoraColorRGBA& start, const SoraColorRGBA& end, float time, 
										ImageEffectMode mode=ImageEffectOnce,
										SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		SoraImageEffectColorTransitions(uint32 start, uint32 end, float time, 
										ImageEffectMode mode=ImageEffectOnce,
										SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
        void modify(SoraSprite* obj);
        SoraModifier<SoraSprite>* clone();
	};
	
	class SORA_API SoraImageEffectRotation: public SoraImageEffect {
	public:
		SoraImageEffectRotation(float start, float end, float time,
								ImageEffectMode mode=ImageEffectOnce,
								SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		SoraImageEffectRotation(float start, float startz, float end, float endz, float time,
								ImageEffectMode mode=ImageEffectOnce,
								SoraCoreTransformer<SoraCoreTransform>* transformer=0);
		
        void modify(SoraSprite* obj);
        SoraModifier<SoraSprite>* clone();
	};
	
	typedef SoraImageEffectFade IEFade;
	typedef SoraImageEffectShake IEShake;
	typedef SoraImageEffectScale IEScale;
	typedef SoraImageEffectTransitions IETransitions;
	typedef SoraImageEffectColorTransitions IEColorTransitions;
	typedef SoraImageEffectRotation IERotation;
	
	typedef SoraCoreTransformer<SoraCoreTransform> ImageEffectTransformer;
    
    static IEFade* CreateEffectFade(float src, float dst, float time, 
                                    ImageEffectMode mode=ImageEffectOnce,
                                    SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IEFade(src, dst, time, mode, transformer);
    }
    
    static IEScale* CreateEffectScale(float src, float dst, float time, 
                                      ImageEffectMode mode=ImageEffectOnce,
                                      SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IEScale(src, dst, time, mode, transformer);
    }
    
    static IEScale* CreateEffectScale(float srcV, float dstV, float srcH, float dstH, float time, 
                                      ImageEffectMode mode=ImageEffectOnce,
                                      SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IEScale(srcV, dstV, srcH, dstH, time, mode, transformer);
    }
    
    static IETransitions* CreateEffectTransitions(float sx, float sy, float sz, float dx, float dy, float dz, float time, 
                                                  ImageEffectMode mode=ImageEffectOnce,
                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IETransitions(sx, sy, sz, dx, dy, dz, time, mode, transformer);
    }
    
    static IETransitions* CreateEffectTransitions(float sx, float sy, float dx, float dy, float time, 
                                                  ImageEffectMode mode=ImageEffectOnce,
                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IETransitions(sx, sy, dx, dy, time, mode, transformer);
    }
    
    static IEColorTransitions* CreateColorTransitions(const SoraColorRGBA& start, const SoraColorRGBA& end, float time, 
                                                      ImageEffectMode mode=ImageEffectOnce,
                                                      SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IEColorTransitions(start, end, time, mode, transformer);
    }
    
    static IEColorTransitions* CreateColorTransitions(uint32 start, uint32 end, float time, 
                                                      ImageEffectMode mode=ImageEffectOnce,
                                                      SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IEColorTransitions(start, end, time, mode, transformer);
    }
    
    static IERotation* CreateEffectRotation(float start, float end, float time,
                                            ImageEffectMode mode=ImageEffectOnce,
                                            SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        return new IERotation(start, end, time, mode, transformer);
    }
    
    static IEFade* CreateEffectFadeWithDelegate(float src, float dst, float time,
                                                const SoraModifier<SoraSprite>::Delegate& del,
                                                ImageEffectMode mode=ImageEffectOnce,
                                                SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IEFade* eff = new IEFade(src, dst, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IEScale* CreateEffectScaleWithDelegate(float src, float dst, float time, 
                                                  const SoraModifier<SoraSprite>::Delegate& del,
                                                  ImageEffectMode mode=ImageEffectOnce,
                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IEScale* eff = new IEScale(src, dst, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IEScale* CreateEffectScaleWithDelegate(float srcV, float dstV, float srcH, float dstH, float time,
                                                  const SoraModifier<SoraSprite>::Delegate& del, 
                                                  ImageEffectMode mode=ImageEffectOnce,
                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IEScale* eff = new IEScale(srcV, dstV, srcH, dstH, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IETransitions* CreateEffectTransitionsWithDelegate(float sx, float sy, float sz, float dx, float dy, float dz, float time, 
                                                              const SoraModifier<SoraSprite>::Delegate& del,
                                                              ImageEffectMode mode=ImageEffectOnce,
                                                              SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IETransitions* eff = new IETransitions(sx, sy, sz, dx, dy, dz, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IETransitions* CreateEffectTransitionsWithDelegate(float sx, float sy, float dx, float dy, float time, 
                                                              const SoraModifier<SoraSprite>::Delegate& del,
                                                              ImageEffectMode mode=ImageEffectOnce,
                                                              SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IETransitions* eff = new IETransitions(sx, sy, dx, dy, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IEColorTransitions* CreateColorTransitionsWithDelegate(const SoraColorRGBA& start, const SoraColorRGBA& end, float time, 
                                                                  const SoraModifier<SoraSprite>::Delegate& del,
                                                                  ImageEffectMode mode=ImageEffectOnce,
                                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IEColorTransitions* eff = new IEColorTransitions(start, end, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IEColorTransitions* CreateColorTransitionsWithDelegate(uint32 start, uint32 end, float time, 
                                                                  const SoraModifier<SoraSprite>::Delegate& del,
                                                                  ImageEffectMode mode=ImageEffectOnce,
                                                                  SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IEColorTransitions* eff = new IEColorTransitions(start, end, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static IERotation* CreateEffectRotationWithDelegate(float start, float end, float time,
                                                        const SoraModifier<SoraSprite>::Delegate& del,
                                                        ImageEffectMode mode=ImageEffectOnce,
                                                        SoraCoreTransformer<SoraCoreTransform>* transformer=0) {
        IERotation* eff = new IERotation(start, end, time, mode, transformer);
        eff->setFinishDelegate(del);
        return eff;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1, SoraImageEffect* eff2, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1, 
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1, 
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 SoraImageEffect* eff4, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1,  
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 SoraImageEffect* eff4, 
                                                 SoraImageEffect* eff5, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1,  
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 SoraImageEffect* eff4, 
                                                 SoraImageEffect* eff5,
                                                 SoraImageEffect* eff6, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1,  
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 SoraImageEffect* eff4, 
                                                 SoraImageEffect* eff5,
                                                 SoraImageEffect* eff6, 
                                                 SoraImageEffect* eff7, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectList(SoraImageEffect* eff1,  
                                                 SoraImageEffect* eff2, 
                                                 SoraImageEffect* eff3, 
                                                 SoraImageEffect* eff4, 
                                                 SoraImageEffect* eff5,
                                                 SoraImageEffect* eff6, 
                                                 SoraImageEffect* eff7,
                                                 SoraImageEffect* eff8, 
                                                 ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->add(eff8);
        return list;
    }
    
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,
                                                             SoraImageEffect* eff2,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1, 
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1, 
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3, 
                                                             SoraImageEffect* eff4,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,  
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3, 
                                                             SoraImageEffect* eff4, 
                                                             SoraImageEffect* eff5,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,  
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3, 
                                                             SoraImageEffect* eff4, 
                                                             SoraImageEffect* eff5,
                                                             SoraImageEffect* eff6,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,  
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3, 
                                                             SoraImageEffect* eff4, 
                                                             SoraImageEffect* eff5,
                                                             SoraImageEffect* eff6, 
                                                             SoraImageEffect* eff7,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->setFinishDelegate(del);
        return list;
    }
    
    static SoraImageEffectList* CreateEffectListWithDelegate(SoraImageEffect* eff1,  
                                                             SoraImageEffect* eff2, 
                                                             SoraImageEffect* eff3, 
                                                             SoraImageEffect* eff4, 
                                                             SoraImageEffect* eff5,
                                                             SoraImageEffect* eff6, 
                                                             SoraImageEffect* eff7,
                                                             SoraImageEffect* eff8,
                                                             const SoraModifier<SoraSprite>::Delegate& del, 
                                                             ImageEffectMode mode=ImageEffectOnce) {
        SoraImageEffectList* list = new SoraImageEffectList(mode);
        list->add(eff1);
        list->add(eff2);
        list->add(eff3);
        list->add(eff4);
        list->add(eff5);
        list->add(eff6);
        list->add(eff7);
        list->add(eff8);
        list->setFinishDelegate(del);
        return list;
    }

} // namespace sora

#endif