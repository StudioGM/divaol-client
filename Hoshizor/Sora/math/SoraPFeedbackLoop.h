//
//  SoraFeedbackLoop.h
//  Sora
//
//  Created by Ruiwei Bu on 9/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFeedbackLoop_h
#define Sora_SoraFeedbackLoop_h

#include <cmath>
#include "SoraTimestamp.h"

namespace sora {
    
    /**
     A P feedback loop (proportional feedback loop) is a simple object which 
     moves a system's current state towards a goal, using the resulting error 
     (difference between goal and state as feedback on the next run.
     
     If you need to implement motion controllers, camera controllers, etc...
     then the feedback loop is your friend.
     **/
    
    template<typename T>
    class SoraPFeedbackLoop {
    public:
        SoraPFeedbackLoop();
        void reset(float time, float stepSize, float gain, const T& curState);
        void setGain(float g);
        float getGain() const;
        void setGoal(const T& c);
        const T& getGoal() const;
        void setState(const T& s);
        const T& getState() const;
        void update(float time);
        
    private:
        float mTime;
        float mStepSize;
        float mGain;
        T mGoal;
        T mState;
    };
    
    template<typename T>
    SoraPFeedbackLoop<T>::SoraPFeedbackLoop():
    mTime(0.f),
    mStepSize(0.001f),
    mGain(-1.0f) {
        
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::reset(float time, float stepSize, float gain, const T& curState) {
        this->mTime = time;
        this->mStepSize = stepSize;
        this->mGain = gain;
        this->mState = curState;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setGain(float g) {
        this->mGain = g;
    }
    
    template<typename T>
    float SoraPFeedbackLoop<T>::getGain() const {
        return this->mGain;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setGoal(const T& g) {
        this->mGoal = g;
    }
    
    template<typename T>
    const T& SoraPFeedbackLoop<T>::getGoal() const {
        return this->mGoal;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::setState(const T& s) {
        this->mState = s;
    }
    
    template<typename T>
    const T& SoraPFeedbackLoop<T>::getState() const {
        return this->mState;
    }
    
    template<typename T>
    void SoraPFeedbackLoop<T>::update(float dt) {
        float curTime = this->mTime + dt;
        while(mTime < curTime) {
            this->mState = this->mState + (this->mState - this->mGoal) * this->mGain * this->mStepSize;
            this->mTime += this->mStepSize;
        }
    }
    
} // namespace sora

#endif
