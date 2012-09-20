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

		folders.clear();
		folderPaths.clear();
	}

	void JsonGameModule::loadConfig(const std::string &path)
	{
		if(path=="")
			return;

		sora::SoraResourceHandle handle = sora::SoraCore::Ptr->loadResourcePack(path);
		sora::SoraResourceFile data("__init__");

		Json::Reader reader;
		Json::Value root;

		const char* cdata = (const char*)data;
		if(cdata==0) LOGGER->notice("module "+path+" not found!");

		if(reader.parse(cdata, cdata+data.size(), root))
		{
			if(root.isMember("base"))
			{
				sora::SoraCore::Ptr->detachResourcePack(handle);
				loadConfig( JsonHelper::_loadAsString(root,"base"));
				sora::SoraCore::Ptr->loadResourcePack(path);
			}

			//config files
			if(root.isMember("note"))
			{
				noteFile = JsonHelper::_loadAsString(root,"note");
				if(noteFile=="__default__")
					noteFile = DEFAULT_INFO_PTR->getAsString("note");
				configloader::loadWithJson(noteConfig,noteFile);
			}
			
			if(root.isMember("render"))
			{
				renderFile = JsonHelper::_loadAsString(root,"render");
				if(renderFile=="__default__")
					renderFile = DEFAULT_INFO_PTR->getAsString("render");
				configloader::loadWithJson(renderConfig,renderFile);
			}

			if(root.isMember("effect"))
			{
				effectFile = JsonHelper::_loadAsString(root,"effect");
				if(effectFile=="__default__")
					effectFile = DEFAULT_INFO_PTR->getAsString("effect");
				configloader::loadWithJson(effectConfig,effectFile);
			}

			if(root.isMember("ui"))
				uiFile = JsonHelper::_loadAsString(root,"ui");

			if(root.isMember("result"))
				resultFile = JsonHelper::_loadAsString(root,"result");

			//hooks
			if(root.isMember("hook")&&root["hook"].isArray())
			{
				if(!root.isMember("hook_reserve"))
					hooks.clear();
				Json::Value &hook = root["hook"];
				for(int i = 0; i < hook.size(); i++)
					if(hook[i].isString())
						hooks.push_back(hook[i].asString());
			}
		}
		else
			LOGGER->notice(reader.getFormatedErrorMessages());

		sora::SoraCore::Ptr->detachResourcePack(handle);
		folderPaths.push_back(path);
	}

	void JsonGameModule::load(const std::string &path) 
	{
		clear();
		this->path = path;

		sora::SoraResourceHandle handle = sora::SoraCore::Ptr->loadResourcePack(path);
		sora::SoraResourceFile data("__init__");
		sora::SoraCore::Ptr->detachResourcePack(handle);

		Json::Reader reader;
		Json::Value root;

		const char* cdata = (const char*)data;
		if(cdata==0) DIVA_EXCEPTION_MODULE("module "+path+" not found!","JsonGameModule");
		if(reader.parse(cdata, cdata+data.size(), root))
		{
			//name
			if(root.isMember("name"))
				name = JsonHelper::_loadAsString(root,"name");
			else
				name = "anonymous";

			//merge note/render/effect configs so that player can custom specfic parts of them
			loadConfig(path);
			loadConfig(MY_PLAYER_INFO.module());

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
		}
		else
			DIVA_EXCEPTION_MODULE(reader.getFormatedErrorMessages(),"JsonGameModule");
	}
	void JsonGameModule::distribute() 
	{
		for(int i = folderPaths.size()-1; i >= 0; i--)
			folders.push_back(sora::SoraCore::Ptr->loadResourcePack(folderPaths[i]));

		ITEM_FACTORY_PTR->gameLoadFromConfig(noteConfig);
		UI_PAINTER_PTR->gameLoad(uiFile);
		RENDER_SYSTEM_PTR->gameLoadFromConfig(renderConfig);
		EFFECT_SYSTEM_PTR->gameLoadFromConfig(effectConfig);
		HOOK_MANAGER_PTR->gameLoad(DEFAULT_INFO_PTR->getAsString("hookConfig"));

		//insert hook
		std::vector<std::string> customHooks = MY_PLAYER_INFO.hooks();
		for(int i = 0; i < customHooks.size(); i++)
			HOOK_MANAGER_PTR->insert(HOOK_MANAGER_PTR->createHook(customHooks[i]));
		for(int i = 0; i < hooks.size(); i++)
			HOOK_MANAGER_PTR->insert(HOOK_MANAGER_PTR->createHook(hooks[i]));
	}
	void JsonGameModule::gameStop()
	{
		for(int i = 0; i < folderPaths.size(); i++)
			sora::SoraCore::Ptr->detachResourcePack(folders[i]);
	}
}