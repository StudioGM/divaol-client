/*
 *  DivaJsonGameModule.cpp
 *
 *  Created by Hyf042 on 2/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaJsonGameModule.h"
#include "SoraResourceFile.h"
#include "Utility/DivaJsonHelper.h"
#include "Core/DivaDefault.h"
#include "Core/DivaCore.h"

namespace divacore
{
	void JsonGameModule::clear()
	{
		noteFile = "__default__";
		uiFile = "__default__";
		renderFile = "__default__";
		effectFile = "__default__";
		resultFile = "__default__";
		hooks.clear();
	}

	void JsonGameModule::load(const std::string &path) 
	{
		clear();
		this->path = path;

		moduleFolder = sora::SoraCore::Ptr->loadResourcePack(path);

		sora::SoraResourceFile data("__init__");

		Json::Reader reader;
		Json::Value root;

		const char* cdata = (const char*)data;
		if(cdata==0) DIVA_EXCEPTION_MODULE("module "+path+" not found!","JsonGameModule");
		if(reader.parse(cdata, cdata+data.size(), root))
		{
			//name
			name = JsonHelper::_loadAsString(root,"name");

			//config files
			if(root.isMember("note"))
				noteFile = JsonHelper::_loadAsString(root,"note");
			if(root.isMember("ui"))
				uiFile = JsonHelper::_loadAsString(root,"ui");
			if(root.isMember("render"))
				renderFile = JsonHelper::_loadAsString(root,"render");
			if(noteFile=="__default__")
				noteFile = DEFAULT_INFO_PTR->getAsString("note");
			if(uiFile=="__default__")
				uiFile = DEFAULT_INFO_PTR->getAsString("ui");
			if(renderFile=="__default__")
				renderFile = DEFAULT_INFO_PTR->getAsString("render");
			if(effectFile=="__default__")
				effectFile = DEFAULT_INFO_PTR->getAsString("effect");
			if(resultFile=="__default__")
				resultFile = DEFAULT_INFO_PTR->getAsString("result");

			//hooks
			if(root.isMember("hook")&&root["hook"].isArray())
			{
				Json::Value &hook = root["hook"];
				for(int i = 0; i < hook.size(); i++)
					if(hook[i].isString())
						hooks.push_back(hook[i].asString());
			}
		}
		else
			DIVA_EXCEPTION_MODULE(reader.getFormatedErrorMessages(),"JsonGameModule");
	}
	void JsonGameModule::distribute() 
	{
		defaultFolder = sora::SoraCore::Ptr->loadResourcePack(DEFAULT_INFO_PTR->getAsString("module"));
		if(MY_PLAYER_INFO.module()!="")
			customFolder = sora::SoraCore::Ptr->loadResourcePack(MY_PLAYER_INFO.module());

		ITEM_FACTORY_PTR->gameLoad(noteFile);
		UI_PAINTER_PTR->gameLoad(uiFile);
		RENDER_SYSTEM_PTR->gameLoad(renderFile);
		EFFECT_SYSTEM_PTR->gameLoad(effectFile);

		//insert hook
		std::vector<std::string> customHooks = MY_PLAYER_INFO.hooks();
		for(int i = 0; i < customHooks.size(); i++)
			HOOK_MANAGER_PTR->insert(HOOK_MANAGER_PTR->createHook(customHooks[i]));
		for(int i = 0; i < hooks.size(); i++)
			HOOK_MANAGER_PTR->insert(HOOK_MANAGER_PTR->createHook(hooks[i]));
	}
	void JsonGameModule::gameStop()
	{
		sora::SoraCore::Ptr->detachResourcePack(moduleFolder);
		if(MY_PLAYER_INFO.module()!="")
			sora::SoraCore::Ptr->detachResourcePack(customFolder);
		sora::SoraCore::Ptr->detachResourcePack(defaultFolder);
	}
}