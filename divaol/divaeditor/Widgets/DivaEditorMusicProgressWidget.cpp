
#include "divaeditor/Widgets/DivaEditorMusicProgressWidget.h"
#include "divaeditor/Core/DivaEditorMapData.h"

#include "divacore/Core/DivaCore.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include "guichan/widgets/button.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/graphics.hpp"
#include "guichan/key.hpp"
#include "guichan/mouseevent.hpp"
#include "guichan/mouseinput.hpp"

#include<cmath>

#define MainSceneImageFile L"standard/pic/editor_001.png"

namespace divaeditor
{
	using namespace gcn;

	DivaEditorMusicProgressWidget::DivaEditorMusicProgressWidget()
		:mouseDown(false)
		//_selectedChanged(sora::SoraFunction<void(float,float)>())
	{
		setFocusable(true);
		setFrameSize(0);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));

		image = Image::load(MainSceneImageFile);
	}

	void DivaEditorMusicProgressWidget::draw(gcn::Graphics* graphics)
	{
		gcn::Color backGroundColor = getBackgroundColor();
		gcn::Color lineColor = getForegroundColor();

		float width = getWidth(),height = getHeight();

		//graphics->setColor(backGroundColor);
		//graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

		//Draw total timeline
		//graphics->setColor(lineColor);
		//graphics->drawLine(timelineOffset,height/2,width-timelineOffset-1,height/2);
		graphics->setColor(gcn::Color(255,255,255,255));
		graphics->drawImage(image,214,403,0,13,277,9);

		//Draw nowLine
		/*
		graphics->setColor(lineColor);
		graphics->drawLine(nowDrawPx,height*0.2,nowDrawPx,height*0.8);
		graphics->drawLine(nowDrawPx-2,height*0.2,nowDrawPx+2,height*0.2);
		graphics->drawLine(nowDrawPx-2,height*0.8,nowDrawPx+2,height*0.8);
		*/
		float nowPos = CORE_PTR->getRunTime() / float(CORE_FLOW_PTR->getTotalTime());
		int nowDrawPx = nowPos * float(width-7);
		graphics->drawImage(image,125,363,nowDrawPx,0,7,35);
	}

	void DivaEditorMusicProgressWidget::jumpMusicByMousePos()
	{
		float jumpTo = float(nowMouseXPos - timelineOffset) / float(getWidth()-timelineOffset*2);
		if(jumpTo<0)jumpTo=0;
		else if(jumpTo>1)jumpTo=1;

		double jumpToTime = jumpTo * float(CORE_FLOW_PTR->getTotalTime());
		if(abs(jumpToTime-CORE_FLOW_PTR->getRealTime())>1e-4)
			EDITOR_PTR->mapData->setPos(EDITUTILITY.timeToPos(jumpToTime));
	}

	void DivaEditorMusicProgressWidget::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		mouseDown=true;
		nowMouseXPos = mouseEvent.getX();
		jumpMusicByMousePos();
	}

	void DivaEditorMusicProgressWidget::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		jumpMusicByMousePos();
	}

	void DivaEditorMusicProgressWidget::mouseReleased(gcn::MouseEvent& mouseEvent)
	{
		mouseDown=false;
		nowMouseXPos = mouseEvent.getX();
	}
}