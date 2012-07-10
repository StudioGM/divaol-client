//
//  jsongui.h
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef JSON_GUI_H_
#define JSON_GUI_H_

#include "SoraPlatform.h"
#include "SoraStringId.h"
#include "json/json.h"

#include "SoraGUI.h"

namespace sora {
    /* 
     a json gui widget reader for soraguichan
     support extensions
     */
    
    class JsonGui;
    typedef void (*JsonGuiParseFunc)(const Json::Value& val, gcn::Widget* parent, JsonGui* pCaller);
    
    class JsonGui {
    public:
        typedef Json::Value::Members VALUE_NAMES;
        JsonGui();
        
        static void parseResponser(gcn::Widget* widget, const std::string& responser, const std::string* type);
        static int parseRespondType(const char* respondStr);

        bool parse(const SoraWString& filePath);
        bool parse(void* strData, uint32 size);
        
        void parseDefaults(const Json::Value& val, gcn::Widget* widget);

        void parseWidget(const Json::Value& val, gcn::Widget* parent);
        
        inline void addToParent(gcn::Widget* widget, gcn::Widget* parent);
        
        void parseContainer(const Json::Value& val, gcn::Widget* parent);
        void parseWindow(const Json::Value& val, gcn::Widget* parent);
        void parseButton(const Json::Value& val, gcn::Widget* parent);
        void parseSlider(const Json::Value& val, gcn::Widget* parent);
        void parseLabel(const Json::Value& val, gcn::Widget* parent);
        void parseIcon(const Json::Value& val, gcn::Widget* parent);
        void parseCheckbox(const Json::Value& val, gcn::Widget* parent);
        void parseTextBox(const Json::Value& val, gcn::Widget* parent);
        void parseTextField(const Json::Value& val, gcn::Widget* parent);
        void parseRadioButton(const Json::Value& val, gcn::Widget* parent);
        void parseImageButton(const Json::Value& val, gcn::Widget* parent);
        void parseListBox(const Json::Value& val, gcn::Widget* parent);
		void parseDropDown(const Json::Value& val, gcn::Widget* parent);
		
        void registerParseFunc(const SoraString& name, JsonGuiParseFunc func);
        
        gcn::Widget* getWidget(const SoraString& name);
        
    private:
		inline void _parseWidget(const char* name, const Json::Value& val, gcn::Widget* parent);
		
		void parseLabelModifier(const Json::Value& modifier, gcn::Label* label);
		
        Json::Reader reader;
        Json::Value rootValue;
        
        bool opened;
        
        typedef std::map<SoraStringId, JsonGuiParseFunc> EXTENSION_FUNC_MAP;
        EXTENSION_FUNC_MAP extensionFuncs;
        
        typedef std::map<SoraStringId, gcn::Widget*> WIDGET_MAP;
        WIDGET_MAP widgets;
    };
	
	
	class JsonGuiWritter {
	public:
		typedef Json::Value JValue;
		static Json::Value& writeWidget(Json::Value& inValue, gcn::Widget* widget);
		
		static std::string getHexColor(const gcn::Color& col);
		
		static Json::Value& writeDefault(Json::Value& inValue, gcn::Widget* widget);
		static Json::Value& writeButton(Json::Value& inValue, gcn::Button* widget);
		static Json::Value& writeContainer(Json::Value& inValue, gcn::Container* widget);
		static Json::Value& writeWindow(Json::Value& inValue, gcn::Window* widget);
		static Json::Value& writeSlider(Json::Value& inValue, gcn::Slider* wiget);
		static Json::Value& writeLabel(Json::Value& inValue, gcn::Label* wiget);
		static Json::Value& writeIcon(Json::Value& inValue, gcn::Icon* wiget);
		static Json::Value& writeCheckBox(Json::Value& inValue, gcn::CheckBox* wiget);
		static Json::Value& writeTextBox(Json::Value& inValue, gcn::TextBox* wiget);
		static Json::Value& writeTextField(Json::Value& inValue, gcn::TextField* wiget);
		static Json::Value& writeRadioButton(Json::Value& inValue, gcn::RadioButton* wiget);
		static Json::Value& writeImageButton(Json::Value& inValue, gcn::ImageButton* wiget);
		static Json::Value& writeDropDown(Json::Value& inValue, gcn::DropDown* wiget);
		static Json::Value& writeListBox(Json::Value& inValue, gcn::ListBox* wiget);
	
	};
    
} // namespace sora


#endif