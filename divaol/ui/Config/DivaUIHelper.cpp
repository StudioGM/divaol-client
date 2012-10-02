#include "DivaUIHelper.h"

#include "guichan/widget.hpp"
#include "Animation/SoraGUIAnimation.h"

namespace diva
{
	using namespace gcn;

	int UIHelper::globalFinishTime;

	void UIHelper::setUIInvisible(gcn::Widget* ui)
	{
		ui->setVisible(false);
	}

	void UIHelper::setGlobalFinishTime(int t)
	{
		globalFinishTime = t;
	}

	void UIHelper::SetUIFade(gcn::Widget* ui, int beginAlpha, int endAlpha, int finishTime, bool isVisible /* = false */)
	{
		ui->setAlpha(beginAlpha);
		if (finishTime == -1)
		{
			finishTime = globalFinishTime;
		}
		if (isVisible)
		{
			ui->setVisible(true);
			ui->addModifier(new GUIAnimation_Alpha(endAlpha, finishTime, GUIAnimation_Float_Linear));
		}
		else
			ui->addModifier(new GUIAnimation_Alpha(endAlpha, finishTime, GUIAnimation_Float_Linear, NONE, NULL, &UIHelper::setUIInvisible));
	}

}