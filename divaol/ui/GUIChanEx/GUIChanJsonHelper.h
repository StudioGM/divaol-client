#pragma once

#include "lib/wjson/wjson.h"

namespace gcn
{
	class SuperButtonEx;
	class ContainerEx;
	class WindowEx;
	class LabelEx;
	class WTextField;
	class Rectangle;

	namespace Helper
	{
		

		SuperButtonEx* CreateButton(const WJson::Value& conf, const std::wstring& normal,
			const std::wstring& on, const std::wstring& down, const std::wstring& disable);

		ContainerEx* CreateStaticImage(const WJson::Value& conf, const std::wstring& name);

		WindowEx* CreateWindowEx(const WJson::Value& conf, const std::wstring& name);

		LabelEx* CreateLabel(const WJson::Value& conf, const std::wstring& name);

		WTextField* CreateInput(const WJson::Value& conf, const std::wstring& name);

		Rectangle GetRect(const WJson::Value& v);
	}
}