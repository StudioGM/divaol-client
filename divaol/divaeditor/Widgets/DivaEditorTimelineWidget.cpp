
#include "divaeditor/Widgets/DivaEditorTimelineWidget.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Core/DivaCore.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

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


	Timeline::Timeline()
		:_selectedBegin(-1),
		isSelecting(false),
		_maxGridHeightFactor(0.5)
		//_selectedChanged(sora::SoraFunction<void(float,float)>())
	{
		setFocusable(true);
		setFrameSize(1);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));
	}


	void Timeline::draw(gcn::Graphics* graphics)
	{

		gcn::Color backGroundColor = getBackgroundColor();
		gcn::Color selectColor = backGroundColor;
		selectColor.r = 255-selectColor.r;
		selectColor.g = 255-selectColor.g;
		selectColor.b = 255-selectColor.b;
		selectColor.a = 100;
		gcn::Color lineColor = getForegroundColor();
		static const gcn::Color stopColor = gcn::Color(255,0,0,100);
		static const gcn::Color bpmLineColor = gcn::Color(255,0,0,255);
		static const gcn::Color beatNumColor = gcn::Color(0,255,0,255);

		float width = getWidth();
		float height = getHeight();

		int GridPerBeat = EDITCONFIG->GridPerBeat;
		int _beatNumberPerScreen = EDITCONFIG->getBeatNumberPerScreen();
		int _gridToShowPerBeat = EDITCONFIG->getGridToShowPerBeat();
		float _showRangeFactor = EDITCONFIG->getShowRangeScale();

		float rangeGridNum = float(_beatNumberPerScreen)*_showRangeFactor * GridPerBeat;
		float mapBeginOffSet = 0;
		float leftPos = CORE_PTR->getRunPosition() - rangeGridNum/2 + mapBeginOffSet;
		float rightPos = leftPos + rangeGridNum;

		//float leftPos = CORE_PTR->getRunPosition();

		//Draw background color
		graphics->setColor(backGroundColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

#pragma region Draw Notes		



#pragma endregion Draw Notes



#pragma region Draw Selected Area

		if(isSelecting)
		{
			//Update selectEndPos
			_selectedEnd = (float(nowMouseXPos>width?width:nowMouseXPos) / float(width) - 0.5) 
				* float(_beatNumberPerScreen)*_showRangeFactor * GridPerBeat + CORE_PTR->getRunPosition();

			float selectBeginPos = (_selectedBegin - leftPos)/rangeGridNum;
			float selectEndPos = (_selectedEnd - leftPos)/rangeGridNum;

			if(selectBeginPos>selectEndPos)
			{float t=selectBeginPos;selectBeginPos=selectEndPos;selectEndPos=t;}

			selectBeginPos = selectBeginPos<0?0:selectBeginPos;
			selectEndPos = selectEndPos>1?1:selectEndPos;

			graphics->setColor(selectColor);
			graphics->fillRectangle(gcn::Rectangle(selectBeginPos*width,0,(selectEndPos-selectBeginPos)*width,height));


			float totalPosition = CORE_FLOW_PTR->getTotalPosition();

			if(isSelecting && nowMouseXPos > getWidth())
			{
				float outFactor = float(nowMouseXPos-getWidth()) / float(getWidth());
				if(CORE_PTR->getRunPosition() + rangeGridNum*outFactor > totalPosition)
					EDITUTILITY.setPosition(totalPosition);
				else
					EDITUTILITY.setPosition(CORE_PTR->getRunPosition() + rangeGridNum*outFactor);
			}
			else if(isSelecting && nowMouseXPos < 0)
			{
				float outFactor = float(nowMouseXPos) / float(getWidth());
				if(CORE_PTR->getRunPosition() + rangeGridNum*outFactor <0)
					EDITUTILITY.setPosition(0);
				else
					EDITUTILITY.setPosition(CORE_PTR->getRunPosition() + rangeGridNum*outFactor);
			}
		}

#pragma endregion Draw Selected Area

#pragma region Draw Lines

		graphics->setColor(lineColor);
		int gridLevel;

		for (float gridToDraw =EDITOR_PTR->mapData->getNowStandardGrid(leftPos, _gridToShowPerBeat);
			gridToDraw <= rightPos; gridToDraw = EDITOR_PTR->mapData->getNextStandardGrid(gridToDraw, _gridToShowPerBeat))
		{
			int nowLevel = EDITOR_PTR->mapData->getGridLevel(gridToDraw);
			
			float tHeightFactor = _maxGridHeightFactor / float(nowLevel);

			graphics->drawLine(width*(gridToDraw-leftPos)/rangeGridNum,
								height*(1- tHeightFactor)-1,
								width*(gridToDraw-leftPos)/rangeGridNum,
								height-1);
		}

#pragma endregion Draw Lines

#pragma region Draw StopLine

		vector<int> stopPos = EDITOR_PTR->mapData->getStopPositionByRange(leftPos,rightPos);
		for(int i=0;i<stopPos.size();i+=2)
		{
			int thisPosLeft = float(stopPos[i]-leftPos)/rangeGridNum * width;
			int thisPosRight = float(stopPos[i+1]-leftPos)/rangeGridNum * width;
			const int dottedHeight = 3;

			graphics->setColor(lineColor);
			if(thisPosLeft>=0)
				for(int i=0;i*dottedHeight<height;i++)
					if(i%2==0)
						graphics->drawLine(thisPosLeft,i*dottedHeight,thisPosLeft,(i+1)*dottedHeight);
			if(thisPosRight<width)
				for(int i=0;i*dottedHeight<height;i++)
					if(i%2==0)
						graphics->drawLine(thisPosRight,i*dottedHeight,thisPosRight,(i+1)*dottedHeight);

			if(thisPosLeft<0)thisPosLeft=0;
			if(thisPosRight>=width)thisPosRight=width-1;

			graphics->setColor(stopColor);
			graphics->fillRectangle(gcn::Rectangle(thisPosLeft,height*0.2,thisPosRight-thisPosLeft,height*0.8));
		}

#pragma endregion Draw StopLine

#pragma region Draw Beat Lines
		std::map<int,int> beatLines = EDITOR_PTR->mapData->getBeatNumByRange(leftPos,rightPos);
		for(std::map<int,int>::iterator i = beatLines.begin();i!=beatLines.end();i++)
		{
			int linePx = (EDITOR_PTR->mapData->getGridFromPeriod(i->first)-leftPos)/rangeGridNum*width;
			graphics->setColor(beatNumColor);
			graphics->drawLine(linePx,0,linePx,height-1);
			graphics->setFont(getFont());
			graphics->drawText("Beats/Period:" + iToS(i->second) ,linePx+2, height*0.20);
		}
#pragma endregion Draw Beat Lines

#pragma region Draw BPM Lines
		for (std::vector<divacore::MapEvent>::iterator i=EDITOR_PTR->mapData->coreInfoPtr->events.begin(); 
					i!=EDITOR_PTR->mapData->coreInfoPtr->events.end();i++)
			if(i->position>=leftPos&&i->position<=rightPos)
			{
				if(i->eventType=="bpm")
				{
					int bpmPx = (i->position-leftPos)/rangeGridNum*width;
					graphics->setColor(bpmLineColor);
					graphics->drawLine(bpmPx,0,bpmPx,height-1);
					graphics->setFont(getFont());
					graphics->drawText("BPM:" + fTos(divacore::Argument::asFloat("value", i->arg),2) ,bpmPx+2, 0);
				}
			}
#pragma endregion Draw BPM Lines
		
#pragma region Draw Music Lines
		if(leftPos<=0 && rightPos>0)
		{
			int beginPx = (0-leftPos)/(rangeGridNum)*width;
			graphics->setColor(lineColor);
			graphics->drawLine(beginPx-1,0,beginPx-1,height-1);
			graphics->drawLine(beginPx,0,beginPx,height-1);
			graphics->drawLine(beginPx-height*0.4,height*0.1,beginPx,height*0.5);
			graphics->drawLine(beginPx,height*0.5,beginPx-height*0.4,height*0.9);
		}
#pragma endregion Draw Music Lines
		
#pragma region Draw Now Line

		graphics->setColor(lineColor);
		graphics->drawLine((int(width)>>1)-1,0,(int(width)>>1)-1,height-1);
		graphics->drawLine((int(width)>>1)+1,0,(int(width)>>1)+1,height-1);
		graphics->drawLine(0,height-1,width,height-1);

#pragma endregion Draw Now Line

	}




	void Timeline::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		isSelecting = false;
		nowMouseXPos = mouseEvent.getX();
	}

	void Timeline::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		if(!isSelecting)
		{
			isSelecting = true;
			_selectedBegin = (float(mouseEvent.getX()) / float(getWidth()) - 0.5) 
								* float(EDITCONFIG->getBeatNumberPerScreen())*EDITCONFIG->getShowRangeScale() * EDITCONFIG->GridPerBeat + CORE_PTR->getRunPosition();
		}
		nowMouseXPos = mouseEvent.getX();
	}

	void Timeline::mouseReleased(gcn::MouseEvent& mouseEvent)
	{
		if(isSelecting)
			isSelecting = false;
		else
		{

		}
		nowMouseXPos = mouseEvent.getX();
	}


	void Timeline::setMaxGridHeightFactor(float factor)
	{
		_maxGridHeightFactor = factor;
	}

}