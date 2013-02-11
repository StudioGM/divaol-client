#pragma once
#include "guichan.hpp"

namespace diva
{
	using namespace gcn;
	class UIHelper
	{
	private:
		static int globalFinishTime;

	public:
		static void setGlobalFinishTime(int t);
		static void setUIInvisible(gcn::Widget* ui);
		static void SetUIFade(gcn::Widget* ui, int beginAlpha = 255, int endAlpha = 0, int finishTime = -1, bool isVisible = false);
	};
}