//
//  SoraHotKey.cpp
//  Sora
//
//  Created by Robert Bu on 7/1/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraHotkey.h"

#include "SoraCore.h"

namespace sora {

    SoraHotkey::SoraHotkey():
    mKey(0),
    mCtrlFlag(false),
    mAltFlag(false),
    mShiftFlag(false),
    mRequirePress(true){
    }
    
    SoraHotkey::SoraHotkey(int32 key, bool ctrlFlag, bool altFlag, bool shiftFlag):
    mKey(key),
    mCtrlFlag(ctrlFlag),
    mAltFlag(altFlag),
    mShiftFlag(shiftFlag),
    mRequirePress(true){
        
    }
    
    bool SoraHotkey::test() {
        bool keyFlag;
        if(mRequirePress) {
            keyFlag = SoraCore::Ptr->keyDown(mKey);
        } else 
            keyFlag = SoraCore::Ptr->keyDown(mKey) || SoraCore::Ptr->keyUp(mKey);
        
        if(keyFlag) {
            bool flags = true;
            if(mCtrlFlag && !SoraCore::Ptr->keyDown(SORA_KEY_CTRL))
                flags = false;
            if(mAltFlag && !SoraCore::Ptr->keyDown(SORA_KEY_ALT))
                flags = false;
            if(mShiftFlag && !SoraCore::Ptr->keyDown(SORA_KEY_SHIFT))
                flags = false;
            return flags;
        }
        return false;
    }
    
    bool SoraHotkey::test(SoraKeyEvent* kev) {
        bool keyFlag;
        if(mRequirePress)
            keyFlag = kev->isKeyPressed(mKey);
        else
            keyFlag = kev->isKeyPressed(mKey) || kev->isKeyReleased(mKey);
        
        if(keyFlag) {
            if(mCtrlFlag == kev->isCtrlFlag() &&
               mAltFlag == kev->isAltFlag() &&
               mShiftFlag == kev->isShiftFlag())
                return true;
        }
        return false;
    }
    
    void SoraHotkey::set(int32 key, bool ctrlFlag, bool altFlag, bool shiftFlag) {
        mKey = key;
        mCtrlFlag = ctrlFlag;
        mAltFlag = altFlag;
        mShiftFlag = shiftFlag;
    }
    
    bool SoraHotkey::isCtrlFlag() const {
        return mCtrlFlag;
    }
    
    bool SoraHotkey::isAltFlag() const {
        return mAltFlag;
    }
    
    bool SoraHotkey::isShiftFlag() const {
        return mShiftFlag;
    }
    
    int32 SoraHotkey::getKey() const {
        return mKey;
    }
    
    SoraHotkey& SoraHotkey::operator=(const SoraHotkey& rhs) {
        mKey = rhs.mKey;
        mShiftFlag = rhs.mShiftFlag;
        mAltFlag = rhs.mAltFlag;
        mCtrlFlag = rhs.mCtrlFlag;
        mRequirePress = rhs.mRequirePress;
		return *this;
    }
    
} // namespace sora