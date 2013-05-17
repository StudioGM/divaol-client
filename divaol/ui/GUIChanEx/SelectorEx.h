#pragma once

#include "GUIChanSuperButtonEx.h"
#include "GUIChanContainerEx.h"
#include "lib/wjson/wjson.h"

namespace gcn
{
	class SelectorDisplayer
	{
	private:

	public:
		virtual void draw(Graphics* graphics, Font* font, int alpha) = 0;
		virtual void ToNext() = 0;
		virtual void ToLast() = 0;
	};

	class SelectorEx :
		public ContainerEx
	{
	private:
		SuperButtonEx* leftBtn;
		SuperButtonEx* rightBtn;
		SelectorDisplayer* displayer;

	public:
		SelectorEx();
		virtual ~SelectorEx();

		void adjustButtonPos();
		void setDisplayer(SelectorDisplayer* displayer);
		SuperButtonEx* getLeftBtn() const;
		SuperButtonEx* getRightBtn() const;
		SelectorDisplayer* getDisplayer() const;

		static SelectorEx* FromJson(const WJson::Value& conf);
		virtual void draw(Graphics* graphics);
	};

	class Selector_MouseListener
		: public MouseListener
	{
	public:
		void mouseClicked(gcn::MouseEvent& mouseEvent);
	};
}
