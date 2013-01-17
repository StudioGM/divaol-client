#pragma once

#include "ui/GUIChanEx/SelectorEx.h"
#include <vector>

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;
		typedef std::vector<std::wstring> StringItems;

		class SpecialItemDisplayer
			: public SelectorDisplayer
		{
		private:
			int selectedIndex;
			StringItems items;
			bool repeat;

		public:
			SpecialItemDisplayer();
			~SpecialItemDisplayer();

			void pushItem(const std::wstring& str);
			int getSelectedIndex() const;
			void setRepeat(bool v);
			const std::wstring& getSelectedItem() const;

			virtual void draw(Graphics* graphics, Font* font, int alpha);
			virtual void ToNext();
			virtual void ToLast();
		};
	}
}