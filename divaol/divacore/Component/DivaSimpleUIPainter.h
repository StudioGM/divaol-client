/*
 *  DivaSimpleUIPainter.h
 *
 *  Created by Hyf042 on 2/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_SIMPLE_UI_PAINTER
#define DIVA_SIMPLE_UI_PAINTER

#include "Utility/DivaSimpleUI.h"
#include "Core/DivaUIPainter.h"


namespace divacore
{
	class SimpleUIPainter : public UIPainter
	{
		typedef std::map<std::string,SimpleUI::WidgetPtr> UI_MAP;
		
		Config config;
		SimpleUI::WidgetPtr root;
		UI_MAP uiMap;

	public:
		void hide(const std::string &tag = "") {root->hide();}
		void display(const std::string &tag = "") {root->display();}

		void gameLoad(const std::string &configFile)
		{
			root = SimpleUI::Factory::createWithJson(configFile,config,uiMap);
			root->handleInput(true);
			root->init();
		}

		void gameStart()
		{
			root->start();
			root->display();
		}

		void gameReset()
		{
			root = NULL;
			config.clear();
			uiMap.clear();
		}

		void gameStop()
		{
			if(root)
			{
				root->clear();
				SAFE_DELETE(root);
			}
		}

		void update(float dt) 
		{
			if(root)
				root->update(dt);
		}
		void render() 
		{
			if(root)
				root->render();
		}

		bool hasWidget(const std::string &name)
		{
			return uiMap.find(name)!=uiMap.end();
		}

		SimpleUI::WidgetPtr getWidget(const std::string &name)
		{
			if(uiMap.find(name)==uiMap.end())
				DIVA_EXCEPTION_MODULE("Widget "+name+" not found!","SimpleUI");
			return uiMap[name];
		}

		void registerWidget(const std::string &name, SimpleUI::WidgetPtr widget)
		{
			if(uiMap.find(name)!=uiMap.end())
				DIVA_EXCEPTION_MODULE("Widget "+name+" already exists!","SimpleUI");
			uiMap[name] = widget;
		}

		SimpleUI::WidgetPtr createWidget(const std::string &name)
		{
			return SimpleUI::Factory::createWidget(name,config);
		}

		void addWidget(SimpleUI::WidgetPtr widget)
		{
			if(uiMap.find("root")==uiMap.end()) 
				return;
			uiMap["root"]->add(widget);
		}

		void delWidget(SimpleUI::WidgetPtr widget)
		{
			if(uiMap.find("root")==uiMap.end()) 
				return;
			uiMap["root"]->del(widget);
		}

		void addWidget(std::string name)
		{
			SimpleUI::WidgetPtr widget = createWidget(name);
			widget->init();
			addWidget(widget);
		}

		void construct(SimpleUI::WidgetPtr widget, std::string name)
		{
			widget->construct(config,name+"_");
		}

		void onMouseClicked(SoraMouseEvent& event) 
		{
			if(root)
				root->mouseClicked(event);
		}
		void onMouseReleased(SoraMouseEvent& event) 
		{
			if(root)
				root->mouseReleased(event);
		}
		void onMouseMoved(SoraMouseEvent& event)
		{
			if(root)
				root->mouseMoved(event);
		}
	};
}

#endif