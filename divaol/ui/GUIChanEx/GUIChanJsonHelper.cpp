#include "GUIChanJsonHelper.h"

#include "GUIChanEx.h"

#undef CreateWindow

namespace gcn
{
	namespace Helper
	{
		SuperButtonEx* CreateButton(const WJson::Value& conf, const std::wstring& normal, const std::wstring& on,
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

		SuperButtonEx* CreateButton(const WJson::Value& conf)
		{
			std::wstring on = L"on", down = L"down", normal = L"normal", disable = L"disable";
			if (!conf.isMember(on))
				on = normal;
			if (!conf.isMember(down))
				down = normal;
			if (!conf.isMember(disable))
				disable = normal;
			SuperButtonEx* b = CreateButton(conf, normal, on, down, disable);
			if (conf.isMember(L"desPos"))
			{
				PointEx p = GetPoint(conf[L"desPos"]);
				b->setPosition(p.x, p.y);
			}
			return b;
		}

		PointEx GetPoint(const WJson::Value& conf)
		{
			if (conf.isArray())
			{
				int z = 0;
				return PointEx(conf[z].asInt(), conf[1].asInt());
			}
			return PointEx(conf[L"x"].asInt(), conf[L"y"].asInt());
		}

		LabelEx* CreateLabel(const WJson::Value& conf)
		{
			LabelEx* label = new LabelEx();
			if (conf.isMember(L"desPos"))
			{
				PointEx p = GetPoint(conf[L"desPos"]);
				label->setPosition(p.x, p.y);
			}
			else
				label->setPosition(conf[L"desX"].asInt(), conf[L"desY"].asInt());
			label->setText(conf[L"text"].asString());
			label->adjustLabelSize();
			if (conf.isMember(L"textColor"))
				label->setForegroundColor(GetColor(conf[L"textColor"]));
			return label;
		}

		Rectangle GetRect(const WJson::Value& v)
		{
			if (v.isArray())
			{
				return gcn::Rectangle(v[WJson::Value::UInt(0)].asInt(), v[1].asInt(), v[2].asInt(), v[3].asInt());
			}
			return gcn::Rectangle(v[L"srcX"].asInt(), v[L"srcY"].asInt(), v[L"width"].asInt(), v[L"height"].asInt());
		}

		void ReadJsonFromFile(const std::wstring& filename, WJson::Value& v)
		{
			std::wstring jsonStrToParse;

			FILE* readFile;
			if(_wfopen_s(&readFile, filename.c_str(),L"rt, ccs=UTF-8")!=0)
				throw "fuck!";
			//return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", path.c_str());

			wchar_t buffer[1000];
			while(fgetws(buffer,sizeof(buffer),readFile))
				jsonStrToParse += std::wstring(buffer);

			fclose(readFile);

			WJson::Reader reader;
			reader.parse(jsonStrToParse, v);

			//return jsonStrToParse;
		}

		Color GetColor(const WJson::Value& v)
		{
			if (v.isArray())
			{
				int zero = 0;
				if (v.size() == 3)
					return Color(v[zero].asInt(), v[1].asInt(), v[2].asInt());
				else
					return Color(v[1].asInt(), v[2].asInt(), v[3].asInt(), v[zero].asInt());
			}
			if (v.isMember(L"a"))
				return Color(v[L"r"].asInt(), v[L"g"].asInt(), v[L"b"].asInt(), v[L"a"].asInt());
			else
				return Color(v[L"r"].asInt(), v[L"g"].asInt(), v[L"b"].asInt());
		}

		ContainerEx* CreateStaticImage(const WJson::Value& conf)
		{
			using namespace gcn;
			ContainerEx* con = new ContainerEx();
			con->load(conf[L"filename"].asString(), 
				gcn::Rectangle(conf[L"srcX"].asInt(), conf[L"srcY"].asInt(), conf[L"width"].asInt(), conf[L"height"].asInt()));
			con->setSize(conf[L"width"].asInt(), conf[L"height"].asInt());
			con->setPosition(conf[L"desX"].asInt(), conf[L"desY"].asInt());
			return con;
		}

		WindowEx* CreateWindow(const WJson::Value& conf)
		{
			using namespace gcn;
			WindowEx* con = new WindowEx();
			con->load(conf[L"filename"].asString(), 
				gcn::Rectangle(conf[L"srcX"].asInt(), conf[L"srcY"].asInt(), conf[L"width"].asInt(), conf[L"height"].asInt()));
			con->setSize(conf[L"width"].asInt(), conf[L"height"].asInt());
			con->setPosition(conf[L"desX"].asInt(), conf[L"desY"].asInt());
			return con;
		}

		ImageTileEx* CreateImageTile(const WJson::Value& conf)
		{
			if (conf.isString())
				return new ImageTileEx(conf.asString());
			if (conf.isArray())
			{
				if (conf.size() < 2)
					throw "bad file!";
				int z = 0;
				return new ImageTileEx(conf[z].asString(), GetRect(conf[1]));
			}
			if (!conf.isMember(L"filename") || !conf.isMember(L"srcRect"))
				throw "bad file!";
			return new ImageTileEx(conf[L"filename"].asString(), GetRect(conf[L"srcRect"]));
		}

		AnimeBoxEx* CreateAnimeBox(const WJson::Value& conf)
		{
			AnimeBoxEx* animeBox = new AnimeBoxEx();

			WJson::Value tv = conf[L"tiles"];
			ImageTiles tiles;
			tiles.clear();
			for (WJson::Value::iterator i = tv.begin(); i != tv.end(); i++)
				tiles.push_back(CreateImageTile(*i));
			int mode = AnimeBoxEx::MODE_ONCE;
			if (conf.isMember(L"mode"))
			{
				if (conf[L"mode"].asString() == L"repeat")
					mode = AnimeBoxEx::MODE_REPEAT;
				else if (conf[L"mode"].asString() == L"once")
					mode = AnimeBoxEx::MODE_ONCE;
			}
			animeBox->loadAnime(tiles, conf[L"fps"].asDouble(), mode);
			Rectangle rect = GetRect(conf[L"desRect"]);
			animeBox->setDimension(rect);
			return animeBox;
		}
	}
}