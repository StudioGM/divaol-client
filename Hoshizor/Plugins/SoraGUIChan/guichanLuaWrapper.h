//
//  guichanWrapper.h
//  Sora
//
//  Created by Griffin Bu on 3/23/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//
#ifndef SORA_GUICHAN_WRAPPER_H_
#define SORA_GUICHAN_WRAPPER_H_

#include "SoraPlatform.h"
#include "SoraGUIChan/SoraGUI.h"
#include "SoraGUIChan/guichansetup.h"

// requires lua
#include "SoraLua/SoraLuaExporter.h"

#include "SoraLua/SoraLuaStateManager.h"
#include "SoraLua/SoraLuaObject.h"
#include "SoraStringId.h"

namespace sora {

    namespace guiwrapper {
        
        void export_guilib(LuaPlus::LuaState* state);
        
#if	SORA_LUA_CHECK_AUTO_EXPORT_SYMBOL(SORA_LUA_AUTO_EXPORT_GUI)
        SORA_LUA_AUTO_EXPORT_FUNC(export_guilib)
#endif
        
        bool loadXmlGui(const SoraWString& xmlPath, const SoraString& topWindowId);
        bool loadJsonGui(const SoraWString& jsonPath, const SoraString& topWindowId);
        
        SoraHandle createWidget(const SoraString& ident, const SoraString& id);
        
        void setGlobalAlpha(float alpha);
        float getGlobalAlpha();
        
        void setWidgetAlpha(SoraHandle h, int32 alpha);
        int32 getWidgetAlpha(SoraHandle h);
        
        void widgetSendMessage(SoraHandle h, const std::string& message);
        void widgetSendMessageWithReceiver(SoraHandle h, const std::string& message, const std::string& receiver);
        void sendMessageToWidget(const std::string& receiver, const std::string& message);
        
        SoraHandle getWidgetById(const SoraString& id);
        void addWidgetToParent(SoraHandle h, const SoraString& parent);
        void removeWidget(SoraHandle h);
        
        void setWidgetResponser(SoraHandle h, const SoraString& responser, const SoraString& responsetype);
        
        void setWidgetFrameSize(SoraHandle h, uint32 framesize);
        uint32 getWidgetFrameSize(SoraHandle h);
        
        SoraHandle getWidgetParent(SoraHandle h);
        
        void setWidgetWidth(SoraHandle h, int32 w);
        void setWidgetHeight(SoraHandle h, int32 ht);
        void setWidgetSize(SoraHandle h, int32 w, int32 ht);
        int32 getWidgetWidth(SoraHandle h);
        int32 getWidgetHeight(SoraHandle h);
        void adjustWidgetSize(SoraHandle h);
        
        void setWidgetX(SoraHandle h, int32 x);
        void setWidgetY(SoraHandle h, int32 y);
        void setWidgetPos(SoraHandle h, int32 x, int32 y);
        int32 getWidgetX(SoraHandle h);
        int32 getWidgetY(SoraHandle h);
        
        void setWidgetFocusable(SoraHandle h, bool focusable);
        bool isWidgetFocusable(SoraHandle h);
        bool isWidgetFocused(SoraHandle h);
        
        void setWidgetVisible(SoraHandle h, bool visible);
        bool isWidgetVisible(SoraHandle h);
        
        void setWidgetEnabled(SoraHandle h, bool enabled);
        bool isWidgetEnabled(SoraHandle h);
        
        void setWidgetBaseColor(SoraHandle h, uint32 c);
        void setWidgetForegroundColor(SoraHandle h, uint32 c);
        void setWidgetBackgroundColor(SoraHandle h, uint32 c);
        void setWidgetSelectionColor(SoraHandle h, uint32 c);
        
        void setWidgetFont(SoraHandle h, const SoraString& fontName, int32 fontSize);
        
        void requestWidgetFocus(SoraHandle h);
        void requestWidgetModalFocus(SoraHandle h);
        void requestWidgetModalMouseInputFocus(SoraHandle h);
        void releaseWidgetModalFocus(SoraHandle h);
        void releaseWidgetModalMouseInputFocus(SoraHandle h);
        bool isWidgetModalFocused(SoraHandle h);
        bool isWidgetModalMouseInputFocused(SoraHandle h);
        
        SoraHandle getWidgetAt(SoraHandle h, int32 x, int32 y);
        
        void setWidgetTabInEnabled(SoraHandle h, bool flag);
        void setWidgetTabOutEnabled(SoraHandle h, bool flag);
        bool isWidgetTabInEnabled(SoraHandle h);
        bool isWidgetTabOutEnabled(SoraHandle h);
        
        void setWidgetId(SoraHandle h, const SoraString& str);
        SoraString getWidgetId(SoraHandle h);
        void showWidgetPart(SoraHandle h, int32 x, int32 y, int32 wd, int32 ht);
        
        bool canWidgetSetCaption(gcn::Widget* Widget, const SoraString& caption);
        bool canWidgetSetValue(gcn::Widget* Widget, float val);
        bool canWidgetSetOpaque(gcn::Widget* widget, bool flag);
        
        void setWidgetOpaque(SoraHandle h, bool flag);
        bool isWidgetOpaque(SoraHandle h);
        
        void setWidgetCaption(SoraHandle h, const SoraString& caption);
        SoraString getWidgetCaption(SoraHandle h);
        
        void setWidgetValue(SoraHandle h, float val);
    } // namespace guiwrapper
    
    
} // namespace sora



#endif