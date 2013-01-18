#include "ui/UI/TitleScreen/TitleScreenUI.h"

#include <fstream>
#include "ui/Config/DivaUIConfig.h"
#include "luaclass.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"
#include "divasongmgr/DivaMapManager.h"

namespace diva
{

	TitleScreenUI::TitleScreenUI()
	{
		// json init
		//ParseJson("temp_cut.json");

		// create top container

		top = new gcn::Container();
		top->setSize(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());
		top->setOpaque(false);
		//Enter();

		// lua init
		sora::SoraLuaObject lo;
		gge::LuaClass<TitleScreenUI>(lo.getState(), "UI", this).
			def("ParseJson", &TitleScreenUI::ParseJson).
			def("CreateStaticImage", &TitleScreenUI::CreateStaticImage).
			def("CreateWheel", &TitleScreenUI::CreateWheel).
			def("CreateExpander", &TitleScreenUI::CreateExpander).
			def("CreatePressAnyKey", &TitleScreenUI::CreatePressAnyKey).
			def("CreateMovingShader", &TitleScreenUI::CreateMovingShader);
		lo.doScript(L"uiconfig/TitleScreen.lua");

		style = L"Miku";

		MAPMGR.registerMapEventMessageQueue(new std::list<divamap::DivaMapEventMessage>);

		// --------- Inner Line

		/*CreateStaticImage("VOCALOID Charactor/" + style);

		inner = CreateWheel("LeftUpCircle/" + style + "/InnerLine");

		outer = CreateWheel("LeftUpCircle/" + style + "/OuterLine");

		circle01 = CreateStaticImage("OtherCircles/" + style + "/circle01");

		circle04 = CreateStaticImage("OtherCircles/" + style + "/circle04");

		circle02 = CreateStaticImage("OtherCircles/" + style + "/circle02");

		bigCircle = CreateStaticImage("LeftUpCircle/" + style + "/BigCircle");

		CreateStaticImage("OtherCircles/" + style + "/circle03");

		CreateStaticImage("LeftUpCircle/" + style + "/SmallCircle");

		CreateStaticImage("DIVA ONLINE");

		CreateStaticImage("VOCALOID LOGO/" + style);

		CreateStaticImage("PressAnyKeyBack");

		pressAnyKey = CreateStaticImage("PressAnyKey");*/

		//
		//pressAnyKey->setAlpha(100);

		
	}


	TitleScreenUI::~TitleScreenUI()
	{
	}

	void TitleScreenUI::BeginLeave()
	{
		top->setEnabled(false);
	}

	void TitleScreenUI::Enter()
	{
		sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
		top->setVisible(true);
		top->setEnabled(true);
		sora::SoraBGMManager::Instance()->play(config[L"titleMusicFilename"].asString(), false);
	}

	void TitleScreenUI::Leave()
	{
		sora::GCN_GLOBAL->getTop()->remove(top);
		top->setVisible(false);
		sora::SoraBGMManager::Instance()->stop(false);
	}

	TitleScreenWheel* TitleScreenUI::CreateWheel(const std::wstring& name)
	{
		WJson::Value tv;
		tv = conf[name];
		TitleScreenWheel* wheel = new TitleScreenWheel();
		wheel->load(tv[L"filename"].asString(), 
			gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()),
			tv[L"width"].asInt() - std::max(tv[L"width"].asInt(), tv[L"height"].asInt()) / 2,
			tv[L"height"].asInt() - std::max(tv[L"width"].asInt(), tv[L"height"].asInt()) / 2);
		wheel->setRSpeed(tv[L"speed"].asDouble());
		wheel->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		top->add(wheel, tv[L"desX"].asInt(), tv[L"desY"].asInt());

		animes.push_back(wheel);
		return wheel;
	}

	gcn::ContainerEx* TitleScreenUI::CreateStaticImage(const std::wstring& name)
	{
		using namespace gcn;
		WJson::Value tv;
		tv = conf[name];
		ContainerEx* con = new ContainerEx();
		con->load(tv[L"filename"].asString(), 
			gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
		con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		top->add(con,  tv[L"desX"].asInt(), tv[L"desY"].asInt());
		return con;
	}

	gcn::ContainerEx* TitleScreenUI::CreateMovingShader(const std::wstring& name)
	{
		using namespace gcn;
		WJson::Value tv;
		tv = conf[name];
		TitleScreenMovingShader* shader = new TitleScreenMovingShader();
		shader->load(tv[L"filename"].asString(), 
			gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
		shader->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		shader->setPara(tv[L"period"].asDouble());
		top->add(shader,  tv[L"desX"].asInt(), tv[L"desY"].asInt());

		animes.push_back(shader);
		return shader;
	}

	TitleScreenPressAnyKey* TitleScreenUI::CreatePressAnyKey(const std::wstring& name)
	{
		WJson::Value tv;
		tv = conf[name];
		TitleScreenPressAnyKey* key = new TitleScreenPressAnyKey();
		key->load(tv[L"filename"].asString(), 
			gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()),
			tv[L"width"].asInt() / 2,
			tv[L"height"].asInt() / 2);
		key->setPara(tv[L"period"].asDouble());
		key->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		top->add(key, tv[L"desX"].asInt(), tv[L"desY"].asInt());

		animes.push_back(key);
		return key;
	}

	TitleScreenExpander* TitleScreenUI::CreateExpander(const std::wstring& name)
	{
		WJson::Value tv;
		tv = conf[name];
		TitleScreenExpander* expander = new TitleScreenExpander();
		expander->load(tv[L"filename"].asString(), 
			gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()),
			tv[L"width"].asInt() / 2,
			tv[L"height"].asInt() / 2);
		double db = tv[L"fPeriod"].asDouble();
		expander->setPara(tv[L"period"].asDouble(), tv[L"fPeriod"].asDouble(), tv[L"scale"].asDouble());
		expander->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		top->add(expander, tv[L"desX"].asInt(), tv[L"desY"].asInt());
		
		animes.push_back(expander);
		return expander;
	}

	void TitleScreenUI::ParseJson(const std::wstring& filename)
	{
		//std::ifstream fs(filename);
		WJson::Reader reader;
		//WJson::Value tv;
		if (!reader.parse(ReadJsonFile(filename), conf))
			throw "failed reading json.";
	}

	void TitleScreenUI::Render()
	{
	}

	void TitleScreenUI::Update(float dt)
	{
		for (int i=0; i<animes.size(); i++)
			animes[i]->update(dt);
	}

}