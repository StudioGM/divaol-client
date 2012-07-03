//
//  SoraPIDFeedbackLoop.h
//  Sora
//
//  Created by Ruiwei Bu on 9/5/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPIDFeedbackLoop_h
#define Sora_SoraPIDFeedbackLoop_h

#include "SoraPlatform.h"
#include <cmath>

namespace sora {
    
    /**
     * A PID feedback loop (proportional integral derivative feedback loop)
     **/
    
    class SoraPIDFeedbackLoop {
    public:
        SoraPIDFeedbackLoop();
        void setValue(double value);
        void setGobal(double goal);

        /// set the propotional, integral and derivative constants, and maximum acceleration (how fast the value kann change, will be disabled if set to 0.0 (default))
        void setConstants(double p, double i, double d, double acceleration);
        const double& getValue() const;
        const double& getGoal() const;
        
        double getLastError() const;
        double getLastDelta() const;
        
        void update(float time);
        void resetError();
        
    private:
        double mValue;
        double mGoal;
        
        double mPConst;
        double mIConst;
        double mDConst;
        double mMaxAcceleration;
        
        double mLastError;
        double mLastDelta;
        double mRunningError;
        bool mValidError;
        
        float mLastDeltaTime;
        float mMaxAllowableDeletaTime;
    };
    
    inline SoraPIDFeedbackLoop::SoraPIDFeedbackLoop():
    mValue(0.0),
    mGoal(0.0),
    mPConst(1.0),
    mIConst(0.0),
    mDConst(0.0),
    mMaxAcceleration(0.0),
    mLastError(0.0),
    mLastDelta(0.0),
    mRunningError(0.0),
    mValidError(false),
    mLastDeltaTime(0.0),
    mMaxAllowableDeletaTime(0.03) {
        
    }
    
    inline void SoraPIDFeedbackLoop::setValue(double value) {
        this->mValue = value;
        this->mLastError = 0.0;
        this->mLastDelta = 0.0;
    }
    
    inline void SoraPIDFeedbackLoop::setConstants(double p, double i, double d, double acceleration) {
        this->mPConst = p;
        this->mIConst = i;
        this->mDConst = d;
        this->mMaxAcceleration = acceleration;
    }
    
    inline const double& SoraPIDFeedbackLoop::getValue() const {
        return this->mValue;
    }
    
    inline double SoraPIDFeedbackLoop::getLastError() const {
        return this->mLastError;
    }
    
    inline double SoraPIDFeedbackLoop::getLastDelta() const {
        return this->mLastDelta;
    }
    
    inline const double& SoraPIDFeedbackLoop::getGoal() const {
        return this->mGoal;
    }
    
    inline void SoraPIDFeedbackLoop::update(float dt) {
        if(dt != 0.0f) {
            if(dt > this->mMaxAllowableDeletaTime)
                dt = this->mMaxAllowableDeletaTime;
            
            double error = (this->mGoal - this->mValue) * dt;
            this->mRunningError += error;
            
            double dP = this->mPConst * error;
            double dI = this->mIConst * this->mRunningError * dt;
            double dD(0.0);
            if(this->mValidError) 
                dD = this->mDConst * (this->mLastError - error) * dt;
            else
                this->mValidError = true;
            
            this->mLastError = error;
            double thisDelta = dP + dI + dD;
            if(this->mMaxAcceleration != 0.0 || false) {
                double timeRatio(1.0);
                if(this->mLastDeltaTime != 0.0) {
                    timeRatio = dt / this->mLastDeltaTime;
                }
                this->mLastDeltaTime = dt;
                
                this->mLastDelta *= timeRatio;
                double difference = (thisDelta - this->mLastDelta);
                double accl = this->mMaxAcceleration * dt * dt;
                if(difference < -accl)
                    thisDelta = this->mLastDelta - accl;
                else if(difference > accl)
                    thisDelta = this->mLastDelta + accl;
            }
            
            this->mValue += thisDelta;
            this->mLastDelta = thisDelta;
        }
    }
    
    inline void SoraPIDFeedbackLoop::resetError() {
        this->mRunningError = 0.0;
        this->mValidError = false;
    }
}


#endif
