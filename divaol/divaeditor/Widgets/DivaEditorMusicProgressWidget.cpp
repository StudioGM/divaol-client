
#include "divaeditor/Widgets/DivaEditorMusicProgressWidget.h"
#include "divaeditor/DivaEditorMapData.h"

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

namespace divaeditor
{
	using namespace gcn;


	DivaEditorMusicProgressWidget::DivaEditorMusicProgressWidget()
		:mouseDown(false)
		//_selectedChanged(sora::SoraFunction<void(float,float)>())
	{
		setFocusable(true);
		setFrameSize(1);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));
	}


	void DivaEditorMusicProgressWidget::draw(gcn::Graphics* graphics)
	{
		gcn::Color backGroundColor = getBackgroundColor();
		gcn::Color lineColor = getForegroundColor();

		float width = getWidth(),height = getHeight();

		const int timelineOffset = 5;

		graphics->setColor(backGroundColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

		//Draw total timeline
		graphics->setColor(lineColor);
		graphics->drawLine(timelineOffset,height/2,width-timelineOffset-1,height/2);

		//Draw nowLine
		graphics->setColor(lineColor);


		//Update nowPosition
		if(mouseDown)
		{
			float jumpTo = float(nowMouseXPos - timelineOffset) / float(width-timelineOffset*2);
			if(jumpTo<0)jumpTo=0;
			else if(jumpTo>1)jumpTo=1;

			double jumpToTime = jumpTo * float(CORE_FLOW_PTR->getTotalTime());
			if(abs(jumpToTime-CORE_FLOW_PTR->getRealTime())>1e-4)
				EDITUTILITY.setPosition(EDITUTILITY.timeToPos(jumpToTime));
		}
		
		float nowPos = CORE_PTR->getRunTime() / float(CORE_FLOW_PTR->getTotalTime());

		int nowDrawPx = timelineOffset + int(nowPos * float(width-timelineOffset*2));
		graphics->drawLine(nowDrawPx,height*0.2,nowDrawPx,height*0.8);
		graphics->drawLine(nowDrawPx-2,height*0.2,nowDrawPx+2,height*0.2);
		graphics->drawLine(nowDrawPx-2,height*0.8,nowDrawPx+2,height*0.8);
	}




	void DivaEditorMusicProgressWidget::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		mouseDown=true;
		nowMouseXPos = mouseEvent.getX();
	}

	void DivaEditorMusicProgressWidget::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
	}

	void DivaEditorMusicProgressWidget::mouseReleased(gcn::MouseEvent& mouseEvent)
	{
		mouseDown=false;
		nowMouseXPos = mouseEvent.getX();
	}
}