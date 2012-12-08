#pragma once

#include "lib/wjson/wjson.h"

#undef CreateWindow

namespace gcn
{
	class SuperButtonEx;
	class ContainerEx;
	class WindowEx;
	class LabelEx;
	class WTextField;
	class Rectangle;
	class Color;
	class PointEx;

	namespace Helper
	{
		void ReadJsonFromFile(const std::wstring& filename, WJson::Value& v);

		SuperButtonEx* CreateButton(const WJson::Value& conf, const std::wstring& normal,
			const std::wstring& on, const std::wstring& down, const std::wstring& disable);

		SuperButtonEx* CreateButton(const WJson::Value& conf);

		template<class T>
		T* CreateList(const WJson::Value& conf)
		{
			T* list = new T();
			
			list->setMaxItem(conf[L"maxItem"].asInt());
			list->setGap(Helper::GetRect(conf[L"firstRect"]), conf[L"itemGap"].asInt());
			if (conf.isMember(L"backImage"))
				list->loadImage(conf[L"backImage"][L"filename"].asString(), Helper::GetRect(conf[L"backImage"][L"srcRect"]));
			if (conf.isMember(L"desRect"))
			{
				list->setPosition(conf[L"desRect"][(WJson::Value::UInt)0].asInt(), conf[L"desRect"][1].asInt());
				list->setSize(conf[L"desRect"][2].asInt(), conf[L"desRect"][3].asInt());
			}
			else
			{
				list->setPosition(conf[L"desPos"][(WJson::Value::UInt)0].asInt(), conf[L"desPos"][1].asInt());
				list->adjustMyWidth();
				list->adjustMyHeight();
			}
			
			return list;
		}

		PointEx GetPoint(const WJson::Value& conf);

		ContainerEx* CreateStaticImage(const WJson::Value& conf);

		WindowEx* CreateWindow(const WJson::Value& conf);

		LabelEx* CreateLabel(const WJson::Value& conf);

		WTextField* CreateInput(const WJson::Value& conf, const std::wstring& name);

		Rectangle GetRect(const WJson::Value& v);

		Color GetColor(const WJson::Value& v);
	}
}