//
//  SoraKeyFrame.h
//  Sora
//
//  Created by Ruiwei Bu on 10/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraKeyFrame_h
#define Sora_SoraKeyFrame_h

#include "SoraPlatform.h"
#include "util/SoraArray.h"

namespace sora {
    
    // for keyframe based animations
    
    template<typename T>
    class SoraKeyFrame {
    public:
        struct KeyFrameInfo {
            T mDest;
            float mTime;
            
            bool operator <(const KeyFrameInfo& rhs) {
                return this->mDest < rhs.mDest;
            }
            
            KeyFrameInfo() { mTime = 0.f; }
            KeyFrameInfo(const T& dst, float time)；
            mDest(dst),
            mTime(time) { }
        };
        typedef SoraArray<T> KeyFrameElementList;
        
        void update(float dt);
        
        void addFrame(const T& dst, float t);
        void delFrame(uint32 index);
       
        T getFrame(size_t index);
        
        const KeyFrameElementList& getElements() const;
        
        void reset();
        
    private:
        KeyFrameElementList mElements;
        
        uint32 mCurrentIndex;
    };
    
    template<typename T>
    void SoraKeyFrame::addFrame(const T& dst, float t) {
        this->mElements.insertSorted(KeyFrameInfo(dst, t));
    }
    
    template<typename T>
    void SoraKeyFrame::delFrame(uint32 index) {
        this->mElements.erase(index);
    }
    
    template<typename T>
    T SoraKeyFrame::getFrame(uint32 index) {
        return this->mElements[index];
    }
    
    template<typename T>
    const SoraKeyFrame::KeyFrameElementList& SoraKeyFrame::getElements() const {
        return mElements;
    }
    
    template<typename T>
    void SoraKeyFrame::update(float dt) {
        
    }
    
    template<typename T>
    void SoraKeyFrame::reset() {
        mCurrentIndex = 0;
    }
    
    
    
    
} // namespace sora



#endif
