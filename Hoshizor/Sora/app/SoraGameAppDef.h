//
//  SoraGameAppDef.h
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGameAppDef_h
#define Sora_SoraGameAppDef_h

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
    
    // a simple container class that defines the main window of the game
    
    class SoraWindowInfoBase;
    
    struct SORA_API SoraGameAppDef {
        SoraGameAppDef(SoraWindowInfoBase* window);
        /**
         * Config must contain the following node
         *  /App/Window
         * With following attributes, some are optimal
         *  Width
         *  Height
         *  X
         *  Y
         *  Windowed
         *  HideMouse
         *  Icon
         *  Cursor
         *  Title
         *  Id
         **/
        explicit SoraGameAppDef(const StringType& loadFromConfig /* json or xml or ini */);
        
        SoraGameAppDef();
        
        SoraGameAppDef(int32 wwidth, int32 wheight, int32 wposx, int32 wposy, bool windowedMode, bool hideMouse, const StringType& title);
        
        SoraGameAppDef& width(int32 width);
        SoraGameAppDef& height(int32 height);
        SoraGameAppDef& posx(int32 posx);
        SoraGameAppDef& posy(int32 posy);
        SoraGameAppDef& windowMode(bool mode);
        SoraGameAppDef& hideMouse(bool flag);
        SoraGameAppDef& title(const StringType& title);
        SoraGameAppDef& icon(const StringType& icon);
        SoraGameAppDef& cursor(const StringType& cursor);
        SoraGameAppDef& resource(const StringType& resource);
                                
        // window size
        int32 WindowWidth, WindowHeight;
        // window pos
        int32 WindowPosX, WindowPosY;
        
        // features
        // default: YES
        bool WindowMode;
        // default: NO
        bool HideMouse;
        // default: NONE
        StringType Icon, Cursor;
        
        // default: "Sora App"
        StringType WindowTitle;
        // default: "MainWindow"
        std::string WindowId;
        
        // external window
        SoraWindowInfoBase* Window;
        
        std::vector<StringType> ResourceScripts;
    };
    
} // namespace sora

#endif
