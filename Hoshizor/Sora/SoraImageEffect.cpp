#include "SoraImageEffect.h"
#include "SoraSprite.h"
#include "math/SoraCoreTransformer.h"

namespace sora {
		
	SoraImageEffect::SoraImageEffect(): 
    SoraModifier<SoraSprite>(true),
    etype(ImageEffectNone), 
    states(ImageEffectNotStart), 
    t_transformer(NULL),
    currRepeatTimes(0), 
    repeatTimes(0) {
	}
    
	SoraImageEffect::SoraImageEffect(SoraCoreTransformer<SoraCoreTransform>* transformer): 
    SoraModifier<SoraSprite>(true), 
    currRepeatTimes(0),
    repeatTimes(0) { 
            if(transformer) {
                t_transformer = transformer->clone();
            } else 
                t_transformer = NULL;
	}
    
	SoraImageEffect::SoraImageEffect(ImageEffectMode _mode): 
    SoraModifier<SoraSprite>(true),
    mode(_mode), 
    etype(ImageEffectNone), 
    states(ImageEffectNotStart), 
    t_transformer(NULL), 
    currRepeatTimes(0), 
    repeatTimes(-1) { 
	}
	
	SoraImageEffect::~SoraImageEffect() {
		if(t_transformer) {
            delete t_transformer;
            t_transformer = 0;
        }
	}
	
	void SoraImageEffect::setRepeatTimes(int32 times) {
		repeatTimes = times;
		currRepeatTimes = 0;
	}
	
	uint32 SoraImageEffect::getRepeatTimes() const {
		return repeatTimes;
	}
    
    bool SoraImageEffect::finished() const {
        return (states == ImageEffectEnd || states == ImageEffectNotStart);
    }
	
	void SoraImageEffect::start(ImageEffectMode _mode, float time) {
		effectTime = time;
		if(effectTime == 0.f) {
			states = ImageEffectEnd;
			t_curr = t_dst;
			return;
		}
		started = 1;
		paused = 0;
		mode = _mode;
	
		states = ImageEffectPlaying;
		pingpongState = 0;
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
	}
	
	void SoraImageEffect::restart() {
		started = 1;
		paused = 0;
		states = ImageEffectPlaying;
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
	}
	
	void SoraImageEffect::swap() {
		std::swap(t_src, t_dst);
		t_curr = t_src;
	}
	
	void SoraImageEffect::stop() {
		started = 0;
		states = ImageEffectEnd;
	}
	
	void SoraImageEffect::pause() {
		paused = 1;
		pauseTime = 0.f;
		states = ImageEffectPaused;
	}
	
	void SoraImageEffect::pauseForTime(float t) {
		assert(t != 0.f);
		pause();
		topauseTime = t;
	}
	
	void SoraImageEffect::resume() {
		paused = 0;
		topauseTime = 0.f;
		states = ImageEffectPlaying;
	}
	
	float SoraImageEffect::getTime() {
		if(started == 1) {
			if(paused == 1) {
				return pauseTime;
			}
			return startTime;
		}
		return 0.f;
	}
	
	void SoraImageEffect::setTransformer(SoraCoreTransformer<SoraCoreTransform>* transformer) {
		assert(transformer != 0);
		if(t_transformer) delete t_transformer;
		t_transformer = transformer;
	}
	
	bool SoraImageEffect::update(float delta) {
		if(started == 1) {
			if(paused == 1) {
				pauseTime += delta;
				if(topauseTime != 0.f) 
					if(pauseTime > topauseTime)
						resume();
				
				return false;
			}
			
			startTime += delta;
			if(startTime >= effectTime) {
				t_curr = t_dst;
				switch(mode) {
					case ImageEffectOnce:
						states = ImageEffectEnd;
                        if(mDelegate) {
                            mDelegate(this);
                        }
                        
						t_curr = t_dst;
						break;
						
					case ImageEffectRepeat:
						checkRepeatTimes();
						
						startTime = 0.f;
						t_curr = t_src;
						break;
						
					case ImageEffectPingpong:
						startTime = 0.f;
						std::swap(t_src, t_dst);
						t_curr = t_src;
						++pingpongState;
						
						if(pingpongState == 2) {
							checkRepeatTimes();
							pingpongState = 0;
						}
						break;
				}
			} else {
				if(t_transformer) 
                    t_curr = t_transformer->transform(t_src, t_dst, startTime/effectTime);
                else 
                    t_curr = lerp(t_src, t_dst, startTime/effectTime);
            }
			
		}
		return states==ImageEffectEnd;
	}
	
	bool SoraImageEffect::checkRepeatTimes() {
		if(repeatTimes == 0)
            return false;
        else if(repeatTimes != 0 ) {
			++currRepeatTimes;
			if(currRepeatTimes >= repeatTimes) {
				currRepeatTimes = 0;
				states = ImageEffectEnd;
				started = 0;
                
                if(mDelegate) {
                    mDelegate(this);
                }
                return true;
			}
		}
		return false;
	}
	
	SoraImageEffectList::SoraImageEffectList(ImageEffectMode mode) {
		mListMode = mode;
		mCurrEffect = NULL;
		mReverse = false;
		
		started = 1;
		paused = 0;
		states = ImageEffectPlaying;
	}
        
    SoraImageEffect& SoraImageEffect::operator =(const SoraImageEffect& rhs) {
        if(&rhs != this) {
            etype = rhs.etype;
            states = rhs.states;
            mode = rhs.mode;
            startTime = rhs.startTime;
            pauseTime = rhs.pauseTime;
            topauseTime = rhs.topauseTime;
            effectTime = rhs.effectTime;
            currRepeatTimes = rhs.currRepeatTimes;
            repeatTimes = rhs.repeatTimes;
            pingpongState = rhs.pingpongState;
            started = rhs.started;
            paused = rhs.paused;
            t_src = rhs.t_src;
            t_dst = rhs.t_dst;
            t_curr = rhs.t_curr;
            t_transformer = rhs.t_transformer->clone();
        }
		return *this;
    }
											 
	SoraImageEffectList::SoraImageEffectList() {
		mListMode = ImageEffectOnce;
		mCurrEffect = NULL;
		mReverse = false;
		
		started = 1;
		paused = 0;
		states = ImageEffectPlaying;
	}
	
	SoraImageEffectList::~SoraImageEffectList() {
		ImageEffectList::iterator itEffect = mImageEffects.begin();
		while(itEffect != mImageEffects.end()) {
			delete *itEffect;
			*itEffect = NULL;
			++itEffect;
		}
	}
	
	SoraImageEffectList* SoraImageEffectList::add(SoraImageEffect* effect) {
		mImageEffects.push_back(effect);
		effect->start(ImageEffectOnce, effect->getEffectTime());
		mCurrEffect	= getListHead();
		return this;
	}
	
	SoraImageEffect* SoraImageEffectList::getListHead() const {
		return mImageEffects.front();
	}
	
	SoraImageEffect* SoraImageEffectList::getListTail() const {
		return mImageEffects.back();
	}
	
	void SoraImageEffectList::setListMode(ImageEffectMode mode) {
		mListMode = mode;
	}
	
	ImageEffectMode SoraImageEffectList::getListMode() const {
		return mListMode;
	}
	
	void SoraImageEffectList::start(ImageEffectMode mode, float time) {
		mCurrEffect	= getListHead();
	}
	
	void SoraImageEffectList::modify(SoraSprite* spr) {
		if(mCurrEffect)
			mCurrEffect->modify(spr);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectList::clone() {
        SoraImageEffectList* newlist = new SoraImageEffectList(*this);
        newlist->mImageEffects = this->mImageEffects;
        newlist->mReverse = this->mReverse;
        newlist->mListMode = this->mListMode;
        newlist->mCurrEffect = this->mCurrEffect;
        return newlist;
    }

	bool SoraImageEffectList::update(float delta) {
		if(mCurrEffect) {
			int32 result = mCurrEffect->update(delta);
			if(result == ImageEffectEnd) {
				ImageEffectList::iterator next = std::find(mImageEffects.begin(), mImageEffects.end(), mCurrEffect);
				if(next != mImageEffects.end()) {
					if(!mReverse) {
						++next; 
						if(next != mImageEffects.end()) {
							mCurrEffect = *next;
							mCurrEffect->start(ImageEffectOnce, mCurrEffect->getEffectTime());
						}
						else {
							if(mListMode == ImageEffectPingpong) {
								mReverse = true;
								mCurrEffect->swap();
								mCurrEffect->start(ImageEffectOnce, mCurrEffect->getEffectTime());
								
								return false;
							} else if(mListMode == ImageEffectRepeat) {
								mCurrEffect = getListHead();
								mCurrEffect->start(ImageEffectOnce, mCurrEffect->getEffectTime());
								
								return false;
							} else {
                                if(mDelegate) {
                                    mDelegate(this);
                                }
                                
								return true;
                            }
						}
					} else {
						if(next == mImageEffects.begin()) {
							if(mListMode == ImageEffectPingpong) {
								mReverse = false;
								mCurrEffect->swap();
								mCurrEffect->start(ImageEffectOnce, mCurrEffect->getEffectTime());
								
								return ImageEffectPlaying;
							} else {
                                if(mDelegate) {
                                    mDelegate(this);
                                }
                
								return true;
                            }
						}
						
						--next;
						mCurrEffect->swap();
						mCurrEffect = *next;
						mCurrEffect->swap();
						mCurrEffect->start(ImageEffectOnce, mCurrEffect->getEffectTime());
					}
				}
			}
			return false;
		}
		return true;
	}
	
	SoraImageEffectFade::SoraImageEffectFade(float src, float dst, float _time, ImageEffectMode _mode,
											 SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		src<dst?etype = ImageEffectFadeIn:etype = ImageEffectFadeOut;
		t_src.Set(src);
		t_dst.Set(dst);

		start(_mode, _time);
	}
	
	void SoraImageEffectFade::modify(SoraSprite* spr) {
		spr->setColor(CSETA(spr->getColor(), get1st()*255));
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectFade::clone() {
        return new SoraImageEffectFade(*this);
    }

	SoraImageEffectScale::SoraImageEffectScale(float src, float dst, float _time, ImageEffectMode _mode,
											   SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		src<dst?etype = ImageEffectScaleIn:etype = ImageEffectScaleOut;
		t_src.Set(src, src);
		t_dst.Set(dst, dst);

		start(_mode, _time);
	}
	
	SoraImageEffectScale::SoraImageEffectScale(float srcV, float dstV, float srcH, float dstH, float _time, ImageEffectMode _mode,
											   SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectTensile;
		t_src.Set(srcV, srcH);
		t_dst.Set(dstV, dstH);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectScale::modify(SoraSprite* spr) {
        spr->setScale(get1st(), get2nd());
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectScale::clone() {
        return new SoraImageEffectScale(*this);
    }

	SoraImageEffectTransitions::SoraImageEffectTransitions(float sx, float sy, float sz, float dx, float dy, float dz, float _time, ImageEffectMode _mode,
														   SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectTransitionsZ;
		t_src.Set(sx, sy, sz);
		t_dst.Set(dx, dy, dz);

		start(_mode, _time);
	}

	SoraImageEffectTransitions::SoraImageEffectTransitions(float sx, float sy, float dx, float dy, float _time, ImageEffectMode _mode,
														   SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectTransitions;
		t_src.Set(sx, sy, 0.f);
		t_dst.Set(dx, dy, 0.f);

		start(_mode, _time);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectTransitions::clone() {
        return new SoraImageEffectTransitions(*this);
    }
	
	void SoraImageEffectTransitions::modify(SoraSprite* spr) {
        spr->setPosition(get1st(), get2nd());
	}
	
	SoraImageEffectColorTransitions::SoraImageEffectColorTransitions(const SoraColorRGBA& s, const SoraColorRGBA& end, float _time, ImageEffectMode _mode,
																	 SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectColorTransitions;
		t_src.Set(s.r, s.g, s.b, s.a);
		t_dst.Set(end.r, end.g, end.b, end.a);
		
		start(_mode, _time);
	}
	
	SoraImageEffectColorTransitions::SoraImageEffectColorTransitions(uint32 s, uint32 end, float _time, ImageEffectMode _mode,
																	 SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectColorTransitions;
		t_src.Set((float)CGETR(s)/255.f, (float)CGETG(s)/255.f, (float)CGETB(s)/255.f, (float)CGETA(s)/255.f);
		t_dst.Set((float)CGETR(end)/255.f, (float)CGETG(end)/255.f, (float)CGETB(end)/255.f, (float)CGETA(end)/255.f);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectColorTransitions::modify(SoraSprite* spr) {
		spr->setColor(SoraColorRGBA::GetHWColor(get1st(), get2nd(), get3rd(), get4th()), -1);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectColorTransitions::clone() {
        return new SoraImageEffectColorTransitions(*this);
    }
	
	SoraImageEffectRotation::SoraImageEffectRotation(float s, float e, float _time, ImageEffectMode _mode,
													 SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectRotate;
		t_src.Set(s);
		t_dst.Set(e);
		
		start(_mode, _time);
	}
	
	SoraImageEffectRotation::SoraImageEffectRotation(float s, float sz, float e, float ez, float _time, ImageEffectMode _mode,
													 SoraCoreTransformer<SoraCoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = ImageEffectRotateZ;
		t_src.Set(s, sz);
		t_dst.Set(e, ez);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectRotation::modify(SoraSprite* spr) {
		spr->setRotation(get1st());
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectRotation::clone() {
        return new SoraImageEffectRotation(*this);
    }

} // namespace sora