//
//  SoraGameAppDef.cpp
//  Sora
//
//  Created by Ruiwei Bu on 9/17/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraGameAppDef.h"
#include "SoraCore.h"
#include "SoraLogger.h"

#include "config/SoraConfigParser.h"
#include "helpers/SoraBooter.h"

namespace sora {
    
    SoraGameAppDef::SoraGameAppDef():
    WindowWidth(800),
    WindowHeight(600),
    WindowMode(true),
    HideMouse(false),
    Icon(StringType()),
    Cursor(StringType()),
    WindowTitle("Sora App"),
    WindowId("MainWindow"),
    Window(0) {
        
    }
    
    SoraGameAppDef::SoraGameAppDef(SoraWindowInfoBase* window):
    Window(window) {
        
    }
    
    SoraGameAppDef::SoraGameAppDef(int32 wwidth, int32 wheight, int32 wposx, int32 wposy, bool windowedMode, bool hideMouse, const StringType& title):
    WindowWidth(wwidth),
    WindowHeight(wheight),
    WindowPosX(wposx),
    WindowPosY(wposy),
    WindowMode(windowedMode),
    HideMouse(hideMouse),
    WindowTitle(title) {
    }
    
    SoraGameAppDef& SoraGameAppDef::width(int32 width) {
        this->WindowWidth = width;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::height(int32 height) {
        this->WindowHeight = height;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::posx(int32 posx) {
        this->WindowPosX = posx;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::posy(int32 posy) {
        this->WindowPosY = posy;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::windowMode(bool mode) {
        this->WindowMode = mode;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::hideMouse(bool flag) {
        this->HideMouse = flag;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::title(const StringType& title) {
        this->WindowTitle = title;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::icon(const StringType& icon) {
        this->Icon = icon;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::cursor(const StringType& cursor) {
        this->Cursor = cursor;
        return *this;
    }
    
    SoraGameAppDef& SoraGameAppDef::resource(const StringType& resource) {
        this->ResourceScripts.push_back(resource);
        return *this;
    }
    
    SoraGameAppDef::SoraGameAppDef(const StringType& loadFromConfig):
    WindowWidth(800),
    WindowHeight(600),
    WindowMode(true),
    HideMouse(false),
    Icon(std::string()),
    Cursor(std::string()),
    WindowTitle("Sora App"),
    WindowId("MainWindow"),
    Window(0) {
        SoraConfigParser parser;
        if(parser.open(loadFromConfig, CONFIG_AUTO_DETECT)) {
            if(parser.toNode("/app/window")) {
                WindowWidth = parser.getInt("width", 800);
                WindowHeight = parser.getInt("height", 600);
                WindowPosX = parser.getInt("x", 0);
                WindowPosY = parser.getInt("y", 0);
                WindowMode = parser.getBool("windowed", true);
                HideMouse = parser.getBool("hidemouse", false);
                Icon = parser.getString("icon", "");
                Cursor = parser.getString("cursor", "");
                WindowTitle = parser.getString("title", "Sora App");
                WindowId = parser.getString("id", "MainWindow");
            } else {
                log_error("no /app/window node exists in window config, default used");
            }
            if(parser.toNode("/app/feature")) {
                SoraCoreFeature feature(parser.getBool("load_plugin", false),
                                        parser.getBool("fsb", false),
                                        parser.getBool("msgbox_show_error", false),
                                        parser.getBool("seperate_sound_thread", false),
                                        parser.getBool("debug_render", false));
                SoraCore::Instance()->init(feature);
                
                if(parser.getBool("load_plugin", false)) {
                    std::string plugin_path = parser.getString("plugin_path");
                    if(!plugin_path.empty()) {
                        SoraBooter::loadExPlugins(plugin_path);
                    }
                }
            }
            if(parser.toNode("/app/resource")) {
                parser.toFirstChild();
                do {
                    ResourceScripts.push_back(parser.getString(std::string()));
                } while(parser.toNextChild());
            }
        }
    }
    
} // namespace sora