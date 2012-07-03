/*
 *  SoraiOSResolutionHelper.h
 *  Sora-iPhone
 *
 *  Created by Griffin Bu on 4/20/11.
 *  Copyright 2011 Studio GameMaster. All rights reserved.
 *
 */
#ifndef SORA_IOS_RESOLUTION_HELPER_H_
#define SORA_IOS_RESOLUTION_HELPER_H_

#include "SoraPlatform.h"

// THIS MUST BE INCLUDED BY .MM FILE

// iphone resolution(480*320)
// retina display(960*640) for iPod Touch 4gen or iPhone 4 gen and up
// OS_IPAD for 1024*768
#ifdef OS_IOS

#include "SoraFileUtility.h"
#include "Wrapper.h"

namespace sora {
    
    // ios device type constants
    static const int IOS_NORMAL = 1;
    static const int IOS_RETINA = 2;
    static const int IOS_IPAD = 3;
    
    // ios portraits
    enum iOSOrientation {
        ORIENTATION_PORTRAIT,
        ORIENTATION_PORTRAIT_UPSIDE_DOWN,
        ORIENTATION_LANDSCAPE_LEFT,
        ORIENTATION_LANDSCAPE_RIGHT
    };
    
    int32 getiOSDeviceType();
    std::string iOSDeviceTypeToString(int32 type);
    
    bool _IS_IPAD();
    bool _IS_RETINA_DISPLAY();
    
    int32 iOSGetScreenWidth(bool rot);
    int32 iOSGetScreenHeight(bool rot);
    
    SoraWString iOSGetResourceName(const SoraWString& origName, bool appendRetina=true);
    SoraWString iOSGetDocumentResourceName(const SoraWString& origName, bool appendRetina=true);
	
    float getScaleFactor();
    
    // only works for iPhone
    void vabriteDevice();
    
    /**
     *  Must be called before initialization
     *  If not, glViewport and glortho are still mapped to (320,480) and resources with @2x are ignored for retina displays
     *  Otherwise, will double the scrensize and will deal with resources with @2x and scale resources without @2x
     **/ 
    void setUseRetina(bool flag);
    bool isUseRetina();
	/*
	 Family: Hiragino Kaku Gothic ProN W3 
	 Font: HiraKakuProN-W3 
	 Family: Courier 
	 Font: Courier 
	 Font: Courier-BoldOblique 
	 Font: Courier-Oblique 
	 Font: Courier-Bold 
	 Family: Arial 
	 Font: ArialMT 
	 Font: Arial-BoldMT 
	 Font: Arial-BoldItalicMT 
	 Font: Arial-ItalicMT 
	 Family: STHeiti TC 
	 Font: STHeitiTC-Light 
	 Font: STHeitiTC-Medium 
	 Family: AppleGothic 
	 Font: AppleGothic 
	 Family: Courier New 
	 Font: CourierNewPS-BoldMT 
	 Font: CourierNewPS-ItalicMT 
	 Font: CourierNewPS-BoldItalicMT 
	 Font: CourierNewPSMT 
	 Family: Zapfino 
	 Font: Zapfino 
	 Family: Hiragino Kaku Gothic ProN W6 
	 Font: HiraKakuProN-W6 
	 Family: Arial Unicode MS 
	 Font: ArialUnicodeMS 
	 Family: STHeiti SC 
	 Font: STHeitiSC-Medium 
	 Font: STHeitiSC-Light 
	 Family: American Typewriter 
	 Font: AmericanTypewriter 
	 Font: AmericanTypewriter-Bold 
	 Family: Helvetica 
	 Font: Helvetica-Oblique 
	 Font: Helvetica-BoldOblique 
	 Font: Helvetica 
	 Font: Helvetica-Bold 
	 Family: Marker Felt 
	 Font: MarkerFelt-Thin 
	 Family: Helvetica Neue 
	 Font: HelveticaNeue 
	 Font: HelveticaNeue-Bold 
	 Family: DB LCD Temp 
	 Font: DBLCDTempBlack 
	 Family: Verdana 
	 Font: Verdana-Bold 
	 Font: Verdana-BoldItalic 
	 Font: Verdana 
	 Font: Verdana-Italic 
	 Family: Times New Roman 
	 Font: TimesNewRomanPSMT 
	 Font: TimesNewRomanPS-BoldMT 
	 Font: TimesNewRomanPS-BoldItalicMT 
	 Font: TimesNewRomanPS-ItalicMT 
	 Family: Georgia 
	 Font: Georgia-Bold 
	 Font: Georgia 
	 Font: Georgia-BoldItalic 
	 Font: Georgia-Italic 
	 Family: STHeiti J 
	 Font: STHeitiJ-Medium 
	 Font: STHeitiJ-Light 
	 Family: Arial Rounded MT Bold 
	 Font: ArialRoundedMTBold 
	 Family: Trebuchet MS 
	 Font: TrebuchetMS-Italic 
	 Font: TrebuchetMS 
	 Font: Trebuchet-BoldItalic 
	 Font: TrebuchetMS-Bold 
	 Family: STHeiti K 
	 Font: STHeitiK-Medium 
	 Font: STHeitiK-Light
     */
    
} // namespace sora

#endif // OS_IOS

#endif // SORA_IOS_RESOLUTION_HELPER_H_