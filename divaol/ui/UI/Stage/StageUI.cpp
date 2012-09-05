#include "StageUI.h"

#include <fstream>
#include "ui/Config/DivaUIConfig.h"
#include "luaclass.h"

#include "soraguiimage.hpp"
#include "SoraSprite.h"

namespace diva
{
	namespace StageUI
	{
		StageUI::StageUI()
		{
			top = new gcn::Container();
			top->setSize(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
			top->setOpaque(false);
			Enter();

			// lua init
			//sora::SoraLuaObject lo;
			//gge::LuaClass<StageUI>(lo.getState(), "UI", this).
			//	def("ParseJson", &StageUI::ParseJson).
			//	def("CreateStaticImage", &StageUI::CreateStaticImage).
			//	def("CreateButton", &StageUI::CreateButton).
			//	def("CreateLoginWindow", &StageUI::CreateLoginWindow);
			//lo.doScript(L"HouseUI.lua");
			ParseJson(L"stage.json");
			top->add(_CreateStaticImage(L"RoomList/BackGround"));
		}

		StageUI::~StageUI()
		{

		}

		void StageUI::Enter()
		{
			sora::GCN_GLOBAL->getTop()->add(top, 0, 0);
		}

		void StageUI::Leave()
		{
			sora::GCN_GLOBAL->getTop()->remove(top);
		}

		void StageUI::Render()
		{
		}

		void StageUI::Update(float dt)
		{

		}

		void StageUI::ParseJson(const std::wstring& filename)
		{
			//std::ifstream fs(filename);
			WJson::Reader reader;
			//WJson::Value tv;
			if (!reader.parse(ReadJsonFile(filename), conf))
				throw "failed reading json.";
		}

		gcn::ContainerEx* StageUI::_CreateStaticImage(const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv;
			tv = conf[name];
			ContainerEx* con = new ContainerEx();
			con->load(tv[L"filename"].asString(), 
				gcn::Rectangle(tv[L"srcX"].asInt(), tv[L"srcY"].asInt(), tv[L"width"].asInt(), tv[L"height"].asInt()));
			con->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			con->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			//top->add(con,  tv["desX"].asInt(), tv["desY"].asInt());
			return con;
		}

		gcn::SuperButtonEx* StageUI::CreateButton(const std::wstring& normal, const std::wstring& on,
			const std::wstring& down, const std::wstring& disable)
		{
			using namespace gcn;
			SuperButtonEx* b = _CreateButton(normal, on, down, disable);
			top->add(b);
			return b;
		}

		gcn::SuperButtonEx* StageUI::_CreateButton(const std::wstring& normal, const std::wstring& on,
			const std::wstring& down, const std::wstring& disable)
		{
			using namespace gcn;
			WJson::Value tv1 = conf[normal], tv2 =conf[on], tv3 = conf[down], tv4 = conf[disable];
			SuperButtonEx* b = new SuperButtonEx();
			b->setLook(tv1[L"filename"].asString(),
				gcn::Rectangle(tv1[L"srcX"].asInt(), tv1[L"srcY"].asInt(), tv1[L"width"].asInt(), tv1[L"height"].asInt()),
				gcn::Rectangle(tv2[L"srcX"].asInt(), tv2[L"srcY"].asInt(), tv2[L"width"].asInt(), tv2[L"height"].asInt()),
				gcn::Rectangle(tv3[L"srcX"].asInt(), tv3[L"srcY"].asInt(), tv3[L"width"].asInt(), tv3[L"height"].asInt()),
				gcn::Rectangle(tv4[L"srcX"].asInt(), tv4[L"srcY"].asInt(), tv4[L"width"].asInt(), tv4[L"height"].asInt()),
				0, 0,
				tv2[L"desX"].asInt() - tv1[L"desX"].asInt(), tv2[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv3[L"desX"].asInt() - tv1[L"desX"].asInt(), tv3[L"desY"].asInt() - tv1[L"desY"].asInt(),
				tv4[L"desX"].asInt() - tv1[L"desX"].asInt(), tv4[L"desY"].asInt() - tv1[L"desY"].asInt());
			b->setSize(tv1[L"width"].asInt(), tv1[L"height"].asInt());
			b->setPosition(tv1[L"desX"].asInt(), tv1[L"desY"].asInt());
			return b;
		}

		gcn::TextField* StageUI::_CreateInput(const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::TextField* text = new TextField();
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}

		gcn::LabelEx* StageUI::_CreateLabel(const std::wstring& name)
		{
			using namespace gcn;
			WJson::Value tv = conf[name];
			gcn::LabelEx* text = new LabelEx(tv[L"text"].asString());
			text->setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
			text->setPosition(tv[L"desX"].asInt(), tv[L"desY"].asInt());
			return text;
		}
	}
}