//
//  jsongui.cpp
//  Sora
//
//  Created by Griffin Bu on 3/28/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "jsongui.h"

#include "SoraGUIResponser.h"
#include "SoraGUIResponserMap.h"
#include "guichansora.hpp"
#include "guichansetup.h"

#include "SoraLogger.h"
#include "Modifiers/LabelSliderLinker.h"

#include "SoraCore.h"

namespace sora {
    
    JsonGui::JsonGui() {
        opened = false;
    }
    
    bool JsonGui::parse(const SoraWString& filePath) {
        uint32 size;
        void* pdata = SoraCore::Ptr->getResourceFile(filePath, size);
        if(pdata) {
            bool result = parse(pdata, size);
            SoraCore::Ptr->freeResourceFile(pdata);
            return result;
        }
        return false;
    }
    
    bool JsonGui::parse(void* data, uint32 size) {
        const char* pstrdata = (const char*)data;
        if(reader.parse(pstrdata, pstrdata+size, rootValue)) {
            parseWidget(rootValue, NULL);
            return true;
        }
		log_error(reader.getFormatedErrorMessages());
        return false;
    }
    
    void JsonGui::addToParent(gcn::Widget* widget, gcn::Widget* parent) {
		if(parent == NULL) return;
		
        gcn::Container* pcont = dynamic_cast<gcn::Container*>(parent);
        if(pcont) {
            pcont->add(widget);
        } else {
            gcn::Window* pwnd = dynamic_cast<gcn::Window*>(parent);
            if(pwnd) {
                pwnd->add(widget);
            } else {
                gcn::ScrollArea* pscroll = dynamic_cast<gcn::ScrollArea*>(parent);
                if(pscroll) {
                    pscroll->setContent(widget);
                }
            }
        }
    }
    
    gcn::Widget* JsonGui::getWidget(const SoraString& name) {
        WIDGET_MAP::iterator itWidget = widgets.find(GetUniqueStringId(name));
        if(itWidget != widgets.end()) {
            return itWidget->second;
        }
        return NULL;
    }
    
    void JsonGui::registerParseFunc(const SoraString& name, JsonGuiParseFunc func) {
        extensionFuncs[GetUniqueStringId(name)] = func;
    }
	
	void JsonGui::_parseWidget(const char* name, const Json::Value& val, gcn::Widget* parent) {
		if(strcmpnocase(name, "container") == 0) {
			parseContainer(val, parent);
		} else if(strcmpnocase(name, "window") == 0) {
			parseWindow(val, parent);
		} else if(strcmpnocase(name, "button") == 0) {
			parseButton(val, parent);
		} else if(strcmpnocase(name, "slider") == 0) {
			parseSlider(val, parent);
		} else if(strcmpnocase(name, "textbox") == 0) {
			parseTextBox(val, parent);
		} else if(strcmpnocase(name, "textfield") == 0) {
			parseTextField(val, parent);
		} else if(strcmpnocase(name, "label") == 0) {
			parseLabel(val, parent);
		} else if(strcmpnocase(name, "checkbox") == 0) {
			parseCheckbox(val, parent);
		} else if(strcmpnocase(name, "radiobutton") == 0) {
			parseRadioButton(val, parent);
		} else if(strcmpnocase(name, "imagebutton") == 0) {
			parseImageButton(val, parent);
		} else if(strcmpnocase(name, "icon") == 0) {
			parseIcon(val, parent);
		} else if(strcmpnocase(name, "listbox") == 0) {
			parseListBox(val, parent);
		} else if(strcmpnocase(name, "dropdown") == 0) {
			parseDropDown(val, parent);
		}
		else {
			EXTENSION_FUNC_MAP::iterator itFunc = extensionFuncs.find(GetUniqueStringId(name));
			if(itFunc != extensionFuncs.end()) {
				itFunc->second(val, parent, this);
			}
		}
	}
    
    void JsonGui::parseWidget(const Json::Value& val, gcn::Widget* parent) {
        VALUE_NAMES members = val.getMemberNames();
        VALUE_NAMES::iterator itMember = members.begin();
        while(itMember != members.end()) {
            const char* type = itMember->c_str();

            bool isObjectValue = val[type].isObject();
            if(isObjectValue) {
				_parseWidget(type, val[type], parent);
			} else if(val[type].isArray()) {
				if(strcmpnocase(type, "widgets") == 0) {
					for(int i=0; i<val[type].size(); ++i) {
						if(val[type][i].isObject()) {
							parseWidget(val[type][i], parent);
						}
					}
				}
			}
            ++itMember;
        }

    }
    
    void JsonGui::parseDefaults(const Json::Value& val, gcn::Widget* widget) {
        if(val.isMember("x")) {
            widget->setX(val["x"].asInt());
        }
        if(val.isMember("y")) {
            widget->setY(val["y"].asInt());
        }
        if(val.isMember("width")) {
            widget->setWidth(val["width"].asInt());
        }
        if(val.isMember("height")) {
            widget->setHeight(val["height"].asInt());
        }
        if(val.isMember("basecolor")) {
            unsigned int color;
            sscanf(val["basecolor"].asCString(), "%x", &color);
            widget->setBaseColor(gcn::Color(color));
        }
        if(val.isMember("foregroundcolor")) {
            unsigned int color;
            sscanf(val["foregroundcolor"].asCString(), "%x", &color);
            widget->setForegroundColor(gcn::Color(color));
        }
        if(val.isMember("backgroundcolor")) {
            unsigned int color;
            sscanf(val["backgroundcolor"].asCString(), "%x", &color);
            widget->setBackgroundColor(gcn::Color(color));
        }
        
        if(val.isMember("framesize")) {
            widget->setFrameSize(val["framesize"].asInt());
        }
        
        if(val.isMember("font")) {
            int fontsize;
            if(val.isMember("fontsize")) 
                fontsize = val["fontsize"].asInt();
            else fontsize = 16;	
            gcn::SoraGUIFont* pfont = new gcn::SoraGUIFont(s2ws(val["font"].asCString()), fontsize);
            if(pfont) {
                widget->setFont(pfont);
            }
        }
        
        if(val.isMember("visible"))
            widget->setVisible(val["visible"].asBool());
        
        if(val.isMember("focusable"))
            widget->setFocusable(val["focusable"].asBool());
        
        if(val.isMember("enabled"))
            widget->setEnabled(val["enabled"].asBool());
        
        if(val.isMember("tabin"))
            widget->setTabInEnabled(val["tabin"].asBool());
        
        if(val.isMember("tabout"))
            widget->setTabOutEnabled(val["tabout"].asBool());
        
        if(val.isMember("eventId"))
            widget->setActionEventId(val["eventId"].asString());
        
		if(val.isMember("name")) {
            widget->setId(val["name"].asString());
		}
		if(val.isMember("id")) {
            widget->setId(val["id"].asString());
		}
        
		if(val.isMember("responser")) {
			SoraString arg = val["responser"].asString();
			SoraString type = val["responsetype"].asString();
			parseResponser(widget, arg, type.size()==0?NULL:&type);
		}
        
        if(val.isMember("parent")) {
            if(widget->getParent() == NULL)
                GCN_GLOBAL->addWidget(widget, val["parent"].asString().c_str());
        }
    }
    
    void JsonGui::parseContainer(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Container *c = new gcn::Container();
        
        if(val.isMember("opaque"))
            c->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, c);
        
        //parsing child elements
		if(val.isMember("widgets")) {
			Json::Value widgets = val["widgets"];
			if(widgets.isArray()) {
				for(int i=0; i<widgets.size(); ++i) {
					if(widgets[i].isObject()) {
						parseWidget(widgets[i], c);
					}
				}
			}
        }
		
        addToParent(c, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = c;
    }
    
    void JsonGui::parseWindow(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Window *window = new gcn::Window;
        if(val.isMember("caption")) {
            window->setCaption(val["caption"].asString());
        }
        if(val.isMember("tabbing")) {
            window->setPadding(val["tabbing"].asBool());
        }
        if(val.isMember("movable")) {
            window->setMovable(val["movable"].asBool());
        }
        if(val.isMember("titleBarHeight")) {
            window->setTitleBarHeight(val["titleBarHeight"].asInt());
        }
        
        if(val.isMember("opaque"))
            window->setOpaque(val["opaque"].asBool());
        
        
        parseDefaults(val, window);
        
        //parsing child elements
		if(val.isMember("widgets")) {
			Json::Value widgets = val["widgets"];
			if(widgets.isArray()) {
				for(int i=0; i<widgets.size(); ++i) {
					if(widgets[i].isObject()) {
						parseWidget(widgets[i], window);
					}
				}
			}
        }
                
        addToParent(window, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = window;
    }
    
    void JsonGui::parseButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Button *button = new gcn::Button;
        if(val.isMember("caption")) {
            button->setCaption(val["caption"].asString());
        }
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmpnocase(align, "center")) {
                button->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmpnocase(align, "left")) {
                button->setAlignment(gcn::Graphics::LEFT);
            } else {
                button->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        button->adjustSize();
        parseDefaults(val, button);
		
        addToParent(button, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = button;
    }
    
    void JsonGui::parseSlider(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Slider *slider = new gcn::Slider;
        if(val.isMember("start")) {            
            slider->setScaleStart(val["start"].asDouble());
        }
        if(val.isMember("end")) {
            slider->setScaleEnd(val["end"].asDouble());
        }
        if(val.isMember("value")) {
            slider->setValue(val["value"].asDouble());
        }
        if(val.isMember("markerLength")) {
            slider->setMarkerLength(val["markerLength"].asInt());
        }
        if(val.isMember("stepLength")) {
            slider->setStepLength(val["stepLength"].asDouble());
        }
        if(val.isMember("orientation")) {
            if(strcmpnocase(val["orientation"].asCString(), "horizontal"))
                slider->setOrientation(gcn::Slider::HORIZONTAL);
            else slider->setOrientation(gcn::Slider::VERTICAL);
        }
        
        parseDefaults(val, slider);
        
        addToParent(slider, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = slider;
    }
	
	void JsonGui::parseLabelModifier(const Json::Value& modifier, gcn::Label* label) {
		if(modifier.isMember("name")) {
			std::string name = modifier["name"].asString();
			if(strcmpnocase("SliderLinker", name.c_str()) == 0) {
				if(modifier.isMember("object")) { 
					std::string obj = modifier["object"].asString();
					gcn::Slider* slider = dynamic_cast<gcn::Slider*> (getWidget(obj));
					if(slider) {
						label->addModifier(new gcn::LabelSliderLinker(slider, label->getCaption()));
					}
				}
			}
		}
	}
    
    void JsonGui::parseLabel(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Label *label = new gcn::Label;
        if(val.isMember("caption")) {
            label->setCaption(val["caption"].asString());
        }
		
		Json::Value modifiers = val["modifiers"];
		if(modifiers.isArray()) {
			for(int i=0; i<modifiers.size(); ++i) {
				Json::Value modifier = modifiers[i];
				if(modifier.isObject()) {
					parseLabelModifier(modifier, label);
				}
			}
		}
        
        label->adjustSize();
        if(val.isMember("align")) {
            const char* align = val["align"].asCString();
            if(strcmpnocase(align, "center")) {
                label->setAlignment(gcn::Graphics::CENTER);
            } else if(strcmpnocase(align, "left")) {
                label->setAlignment(gcn::Graphics::LEFT);
            } else {
                label->setAlignment(gcn::Graphics::RIGHT);
            }
        }
        
        parseDefaults(val, label);
        
        addToParent(label, parent);
		if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = label;
    }
    
    void JsonGui::parseImageButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::ImageButton* pButton = 0;
        gcn::Image* image = NULL;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        if(image) {
            pButton = new gcn::ImageButton(image);
            parseDefaults(val, pButton);
            
            addToParent(pButton, parent);
            if(name.size() != 0)
				widgets[GetUniqueStringId(name)] = pButton;
        }
    }
    
    void JsonGui::parseIcon(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::Icon *icon;
        gcn::Image *image;
        
        if(val.isMember("image")) {
            image = gcn::Image::load(val["image"].asString());
        }
        
        if(image) {
            icon = new gcn::Icon(image);
            
            parseDefaults(val, icon);
            
            addToParent(icon, parent);
            if(name.size() != 0)
				widgets[GetUniqueStringId(name)] = icon;
        }
    }
    
    void JsonGui::parseCheckbox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::CheckBox *checkbox = new gcn::CheckBox;
        
        if(val.isMember("caption")) {
            checkbox->setCaption(val["caption"].asString());
        }
        
        checkbox->adjustSize();
    
        parseDefaults(val, checkbox);

        addToParent(checkbox, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = checkbox;
    }
    
    void JsonGui::parseTextBox(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::TextBox *textbox = new gcn::TextBox;
        
        if(val.isMember("editable"))
            textbox->setEditable(val["editable"].asBool());
        
        if(val.isMember("text")) {
            textbox->setText(val["text"].asString().c_str());
        }
        
        if(val.isMember("opaque"))
            textbox->setOpaque(val["opaque"].asBool());
        
        parseDefaults(val, textbox);
        
        addToParent(textbox, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = textbox;
    }
    
    void JsonGui::parseTextField(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::TextField *textField = new gcn::TextField;
    
        if(val.isMember("text")) {
            textField->setText(val["text"].asString().c_str());
        }
		
		if(val.isMember("numeric")) 
			textField->enableNumericMode(val["numeric"].asBool());
    
        parseDefaults(val, textField);
        
        addToParent(textField, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = textField;
    }
    
    void JsonGui::parseRadioButton(const Json::Value& val, gcn::Widget* parent) {
        std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
        
        gcn::RadioButton *radio = new gcn::RadioButton;
        
        if(val.isMember("caption")) {
            radio->setCaption(val["caption"].asString());
        }
        radio->adjustSize();
        
        if(val.isMember("group"))
            radio->setGroup(val["group"].asString());		
        
        parseDefaults(val, radio);
        
        addToParent(radio, parent);
        if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = radio;
    }
	
	void JsonGui::parseListBox(const Json::Value& val, gcn::Widget* parent) {
		std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
		
		gcn::ListBox* listbox = new gcn::ListBox;
		if(val.isMember("selectioncolor")) {
			int color;
            sscanf(val["selectioncolor"].asCString(), "%x", &color);
            listbox->setSelectionColor(gcn::Color(color));
		}
		
		if(val.isMember("items")) {
			Json::Value items = val["items"];
			if(items.isArray()) {
                gcn::ListModel* listModel = new gcn::ListModel;

				for(int i=0; i<items.size(); ++i) {
					listModel->pushElement(items[i].asString());
				}
				listbox->setListModel(listModel);
			}
		}
		
		parseDefaults(val, listbox);
		addToParent(listbox, parent);
		if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = listbox;
	}
	
	void JsonGui::parseDropDown(const Json::Value& val, gcn::Widget* parent) {
		std::string name;
        if(val.isMember("name")) {
            name = val["name"].asString();
        }
		
		gcn::ListBox* dropdown = new gcn::ListBox;
		if(val.isMember("selectioncolor")) {
			int color;
            sscanf(val["selectioncolor"].asCString(), "%x", &color);
            dropdown->setSelectionColor(gcn::Color(color));
		}
		
		if(val.isMember("items")) {
			Json::Value items = val["items"];
			if(items.isArray()) {
				gcn::ListModel* listModel = new gcn::ListModel;
				
				for(int i=0; i<items.size(); ++i) {
					listModel->pushElement(items[i].asString());
				}
				dropdown->setListModel(listModel);
			} 
		}
		
		parseDefaults(val, dropdown);
		addToParent(dropdown, parent);
		if(name.size() != 0)
			widgets[GetUniqueStringId(name)] = dropdown;
	}
    
    int JsonGui::parseRespondType(const char* respondStr) {
        std::vector<std::string> rtypes;
        sora::deliStr(rtypes, respondStr, ',');
        int irtype = 0;
        for(size_t i=0; i<rtypes.size(); ++i) {
            if(rtypes[i].compare("action") == 0) irtype |= sora::RESPONSEACTION;
            else if(rtypes[i].compare("death") == 0) irtype |= sora::RESPONSEDEATH;
            else if(rtypes[i].compare("focus") == 0) irtype |= sora::RESPONSEFOCUS;
            else if(rtypes[i].compare("mouse") == 0) irtype |= sora::RESPONSEMOUSE;
            else if(rtypes[i].compare("key") == 0) irtype |= sora::RESPONSEKEY;
			else if(rtypes[i].compare("selection") == 0) irtype |= sora::RESPONSESELECTION;
        }
        return irtype;
    }
    
    void JsonGui::parseResponser(gcn::Widget* widget, const std::string& arg, const std::string* type) {
        if(arg[0] == '@') {
            size_t ap = arg.find("=");
            if(ap != std::string::npos) {
                SoraString srcType = arg.substr(1, ap);
                
                sora::SoraGUIResponserExtern* pResponser = sora::SoraGUIResponserMap::Instance()->getExternalResponser( srcType );
                if(pResponser) {
                    const std::string* respondtypestr = type;
                    if(respondtypestr) {
                        int irtype = parseRespondType(respondtypestr->c_str());
                        sora::SoraGUI::Instance()->registerExternalGUIResponser(widget, 
                                                                                pResponser, 
                                                                                widget->getId(),
                                                                                arg.substr(ap+1, arg.size()),
                                                                                irtype);
                    } else {
                        sora::SoraGUI::Instance()->registerExternalGUIResponser(widget, 
                                                                                pResponser, 
                                                                                widget->getId(),
                                                                                arg.substr(ap+1, arg.size()),
                                                                                sora::RESPONSEACTION);
                    }
                }
            }
        } else {
            sora::SoraGUIResponser* pResponser = sora::SoraGUIResponserMap::Instance()->getResponser( arg );
            if(pResponser) {
                const std::string* respondtypestr = type;
                if(respondtypestr) {
                    int irtype = parseRespondType(respondtypestr->c_str());
                    sora::SoraGUI::Instance()->registerGUIResponser(widget, 
                                                                    pResponser, 
                                                                    widget->getId(),
                                                                    irtype);
                } else {
                    sora::SoraGUI::Instance()->registerGUIResponser(widget, 
                                                                    pResponser, 
                                                                    widget->getId(),
                                                                    sora::RESPONSEACTION);
                }
            } 
        }
    }
    
	
	Json::Value& JsonGuiWritter::writeWidget(Json::Value& inValue, gcn::Widget* widget) {
		gcn::Button* pb = dynamic_cast<gcn::Button*> (widget);
		if(pb) {
			JValue jv;
			writeButton(jv, pb);
			writeDefault(jv, widget);
			inValue["button"] = jv;
		}
		gcn::Container* pc = dynamic_cast<gcn::Container*> (widget);
		if(pc) {
			JValue jv;
			writeContainer(jv, pc);
			writeDefault(jv, widget);
			inValue["container"] = jv;
		}
		gcn::Window* pw = dynamic_cast<gcn::Window*> (widget);
		if(pw) {
			JValue jv;
			writeWindow(jv, pw);
			writeDefault(jv, widget);
			inValue["window"] = jv;
		}
		gcn::Slider* ps = dynamic_cast<gcn::Slider*> (widget);
		if(ps) {
			JValue jv;
			writeSlider(jv, ps);
			writeDefault(jv, widget);
			inValue["slider"] = jv;
		}
		gcn::Label* pl = dynamic_cast<gcn::Label*> (widget);
		if(pl) {
			JValue jv;
			writeLabel(jv, pl);
			writeDefault(jv, widget);
			inValue["label"] = jv;
		}
		gcn::Icon* pi = dynamic_cast<gcn::Icon*> (widget);
		if(pi) {
			JValue jv;
			writeIcon(jv, pi);
			writeDefault(jv, widget);
			inValue["icon"] = jv;
		}
		gcn::CheckBox* pcb = dynamic_cast<gcn::CheckBox*> (widget);
		if(pcb) {
			JValue jv;
			writeCheckBox(jv, pcb);
			writeDefault(jv, widget);
			inValue["checkbox"] = jv;
		}
		gcn::TextBox* ptb = dynamic_cast<gcn::TextBox*> (widget);
		if(ptb) {
			JValue jv;
			writeTextBox(jv, ptb);
			writeDefault(jv, widget);
			inValue["textbox"] = jv;
		}
		gcn::TextField* ptf = dynamic_cast<gcn::TextField*> (widget);
		if(ptf) {
			JValue jv;
			writeContainer(jv, pc);
			writeDefault(jv, widget);
			inValue["textfield"] = jv;
		}
		gcn::RadioButton* prb = dynamic_cast<gcn::RadioButton*> (widget);
		if(prb) {
			JValue jv;
			writeRadioButton(jv, prb);
			writeDefault(jv, widget);
			inValue["radiobutton"] = jv;
		}
		gcn::ImageButton* pib = dynamic_cast<gcn::ImageButton*> (widget);
		if(pib) {
			JValue jv;
			writeImageButton(jv, pib);
			writeDefault(jv, widget);
			inValue["imagebutton"] = jv;
		}
		gcn::DropDown* pdd = dynamic_cast<gcn::DropDown*> (widget);
		if(pdd) {
			JValue jv;
			writeDropDown(jv, pdd);
			writeDefault(jv, widget);
			inValue["dropdown"] = jv;
		}
		gcn::ListBox* plb = dynamic_cast<gcn::ListBox*> (widget);
		if(plb) {
			JValue jv;
			writeListBox(jv, plb);
			writeDefault(jv, widget);
			inValue["listbox"] = jv;
		}
		return inValue;
	}
	
	std::string JsonGuiWritter::getHexColor(const gcn::Color& col) {
		char colout[16];
		unsigned int uiCol = CARGB(col.a, col.r, col.b, col.b);
		sprintf(colout, "%X", uiCol);
		
		return std::string("0x")+colout;
	}
	
	Json::Value& JsonGuiWritter::writeDefault(Json::Value& inValue, gcn::Widget* widget) {
		inValue["x"] = JValue(widget->getX());
		inValue["y"] = JValue(widget->getY());
		inValue["width"] = JValue(widget->getWidth());
		inValue["height"] = JValue(widget->getHeight());
	
		inValue["basecolor"] = JValue(getHexColor(widget->getBaseColor()));
		inValue["foregroundcolor"] = JValue(getHexColor(widget->getForegroundColor()));
		inValue["backgroundcolor"] = JValue(getHexColor(widget->getBackgroundColor()));
		
		inValue["framesize"] = JValue(widget->getFrameSize());
		inValue["visible"] = JValue(widget->isVisible());
		inValue["focusable"] = JValue(widget->isFocusable());
		inValue["enabled"] = JValue(widget->isEnabled());
		inValue["tabout"] = JValue(widget->isTabOutEnabled());
		inValue["id"] = JValue(widget->getId());
		
		return inValue;
	}
	
	Json::Value& JsonGuiWritter::writeButton(Json::Value& inValue, gcn::Button* widget) {
		inValue["caption"] = JValue(widget->getCaption());
		switch (widget->getAlignment()) {
			case gcn::Graphics::CENTER: inValue["align"] = JValue("center"); break;
			case gcn::Graphics::LEFT: inValue["align"] = JValue("left"); break;
			case gcn::Graphics::RIGHT: inValue["align"] = JValue("right"); break;
		}
		
		return inValue;
	}
	
	Json::Value& JsonGuiWritter::writeContainer(Json::Value& inValue, gcn::Container* widget) {
		inValue["opaque"] = JValue(widget->isOpaque());
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeWindow(Json::Value& inValue, gcn::Window* widget) {
		inValue["opaque"] = JValue(widget->isOpaque());
		inValue["caption"] = JValue(widget->getCaption());
		inValue["movable"] = JValue(widget->isMovable());
		inValue["titleBarHeight"] = JValue(widget->getTitleBarHeight());
		inValue["tabbing"] = JValue(widget->getPadding());
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeSlider(Json::Value& inValue, gcn::Slider* widget) {
		inValue["start"] = JValue(widget->getScaleStart());
		inValue["end"] = JValue(widget->getScaleEnd());
		inValue["value"] = JValue(widget->getValue());
		inValue["markerLength"] = JValue(widget->getMarkerLength());
		inValue["stepLength"] = JValue(widget->getStepLength());
		if(widget->getOrientation() == gcn::Slider::HORIZONTAL)
			inValue["orientation"] = JValue("horizontal");
		else
			inValue["orientation"] = JValue("vertical");
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeLabel(Json::Value& inValue, gcn::Label* widget) {
		inValue["caption"] = JValue(widget->getCaption());
		switch (widget->getAlignment()) {
			case gcn::Graphics::CENTER: inValue["align"] = JValue("center"); break;
			case gcn::Graphics::LEFT: inValue["align"] = JValue("left"); break;
			case gcn::Graphics::RIGHT: inValue["align"] = JValue("right"); break;
		}
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeIcon(Json::Value& inValue, gcn::Icon* widget) {
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeCheckBox(Json::Value& inValue, gcn::CheckBox* widget) {
		inValue["caption"] = JValue(widget->getCaption());

		return inValue;
	}

	Json::Value& JsonGuiWritter::writeTextBox(Json::Value& inValue, gcn::TextBox* widget) {
		inValue["editable"] = JValue(widget->isEditable());
		inValue["text"] = JValue((widget->getText()));
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeTextField(Json::Value& inValue, gcn::TextField* widget) {
		inValue["text"] = JValue((widget->getText()));

		return inValue;
	}

	Json::Value& JsonGuiWritter::writeRadioButton(Json::Value& inValue, gcn::RadioButton* widget) {
		inValue["caption"] = JValue(widget->getCaption());
		inValue["group"] = JValue(widget->getGroup());
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeImageButton(Json::Value& inValue, gcn::ImageButton* widget) {
		
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeDropDown(Json::Value& inValue, gcn::DropDown* widget) {
		inValue["selectioncolor"] = getHexColor(widget->getSelectionColor());
		JValue items;
		gcn::ListModel* lm = widget->getListModel();
		if(lm) {
			for(int i=0; i<lm->getNumberOfElements(); ++i)
				items.append(JValue(lm->getElementAt(i)));
		}
		inValue["items"] = items;
		return inValue;
	}

	Json::Value& JsonGuiWritter::writeListBox(Json::Value& inValue, gcn::ListBox* widget) {
		inValue["selectioncolor"] = getHexColor(widget->getSelectionColor());
		JValue items;
		gcn::ListModel* lm = widget->getListModel();
		if(lm) {
			for(int i=0; i<lm->getNumberOfElements(); ++i)
				items.append(JValue(lm->getElementAt(i)));
		}
		inValue["items"] = items;
		
		return inValue;
	}

} // namespace sora