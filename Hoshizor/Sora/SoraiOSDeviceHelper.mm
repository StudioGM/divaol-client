//
//  SoraiOSDeviceHelper.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/30/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSDeviceHelper.h"
#include "soraiOSFileUtility.h"

#ifdef OS_IOS

#import <UIKit/UIkit.h>
#import <UIKit/UIDevice.h>
#import <AudioToolbox/AudioToolbox.h>

namespace sora {
    
    int32 getiOSDeviceType() {
        if(_IS_IPAD())
            return IOS_IPAD;
        else if(_IS_RETINA_DISPLAY())
            return IOS_RETINA;
        else 
            return IOS_NORMAL;
    }
    
    std::string iOSDeviceTypeToString(int32 type) {
        switch(type) {
            case IOS_IPAD:
                return std::string("ipad");
            case IOS_NORMAL:
                return std::string("ios device without retina display");
            case IOS_RETINA:
                return std::string("ios device with retina display");
        }
        return std::string("");
    }
    
    bool _IS_IPAD() {
        return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
    }
    
    bool _IS_RETINA_DISPLAY() {
        if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0)) { 
            return true; 
        }	
        return false; 
    }
    
    int32 iOSGetScreenWidth(bool rot) {
        CGRect rect = [[UIScreen mainScreen] bounds];
        if(rot)
            return rect.size.height;
        return rect.size.width;
    }
    
    int32 iOSGetScreenHeight(bool rot) {
        CGRect rect = [[UIScreen mainScreen] bounds];
        if(rot)
            return rect.size.width;
        return rect.size.height ;
    }
    
    SoraWString iOSGetResourceName(const SoraWString& origName, bool appendRetina) {
        if(appendRetina && _IS_RETINA_DISPLAY()) {
            SoraWString fn = origName.substr(0, origName.rfind(L"."));
            SoraWString ext = origName.substr(origName.rfind(L"."), origName.size());
            return sora::SoraFileUtility::getApplicationPath() + fn + L"@2x" + ext;
        }
        return sora::SoraFileUtility::getApplicationPath() + origName;
    }
	
    SoraWString iOSGetDocumentResourceName(const SoraWString& origName, bool appendRetina) {
		if(appendRetina && _IS_RETINA_DISPLAY()) {
			SoraWString fn = origName.substr(0, origName.rfind(L"."));
			SoraWString ext = origName.substr(origName.rfind(L"."), origName.size());
			return appDocumentPath() + fn + L"@2x" + ext;
		}
		return appDocumentPath() + origName;
	}
    
    static bool g_useRetina = true;
    
    void setUseRetina(bool flag) {
        g_useRetina = flag;
    }
    
    bool isUseRetina() {
        return g_useRetina;
    }
    
    float getScaleFactor() {
        return _IS_RETINA_DISPLAY() ? 2.0 : 1.0;
    }
    
    void vabriateDevice() {
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    }
    
} // namespace sora

#endif