#include "divaeditor/DivaEditorScene/DivaEditorTimelineWidget.h"
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


	Timeline::Timeline()
		:_beatNumberPerScreen(20),
		_selectedBegin(-1),
		_gridToShowPerBeat(0),
		_showRangeFactor(1),
		isSelecting(false)
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
		
		static const float BeatLineHeightFactor = 0.6;

		gcn::Color backGroundColor = getBackgroundColor();
		gcn::Color selectColor = backGroundColor;
		selectColor.r = 255-selectColor.r;
		selectColor.g = 255-selectColor.g;
		selectColor.b = 255-selectColor.b;
		selectColor.a = 100;
		gcn::Color lineColor = getForegroundColor();
		gcn::Color stopColor = gcn::Color(255,0,0,100);

		float width = getWidth();
		float height = getHeight();


		float rangeGridNum = float(_beatNumberPerScreen)*_showRangeFactor * GridPerBeat;
		float mapBeginOffSet = 0;
		float leftPos = CORE_PTR->getRunPosition() - rangeGridNum/2 + mapBeginOffSet;
		float rightPos = leftPos + rangeGridNum;

		//float leftPos = CORE_PTR->getRunPosition();

		//Draw background color
		graphics->setColor(backGroundColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

		//Draw Notes



		//Draw Selected Area
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


			float totalPosition = ((divacore::StandardCoreFlow*)CORE_FLOW_PTR)->getTotalPosition();

			if(isSelecting && nowMouseXPos > getWidth())
			{
				float outFactor = float(nowMouseXPos-getWidth()) / float(getWidth());
				if(CORE_PTR->getRunPosition() + rangeGridNum*outFactor > totalPosition)
					divacore::StandardEditUtility::instance().setPosition(totalPosition);
				else
					divacore::StandardEditUtility::instance().setPosition(CORE_PTR->getRunPosition() + rangeGridNum*outFactor);
			}
			else if(isSelecting && nowMouseXPos < 0)
			{
				float outFactor = float(nowMouseXPos) / float(getWidth());
				if(CORE_PTR->getRunPosition() + rangeGridNum*outFactor <0)
					divacore::StandardEditUtility::instance().setPosition(0);
				else
					divacore::StandardEditUtility::instance().setPosition(CORE_PTR->getRunPosition() + rangeGridNum*outFactor);
			}
		}

		graphics->setColor(lineColor);


		//Draw Lines
		float deltaGrid = GridPerBeat/gridToShowPerBeatTable[_gridToShowPerBeat];
		int gridLevel;

		for (float gridToDraw =EDITOR_PTR->mapData->getNowStandardGrid(leftPos, gridToShowPerBeatTable[_gridToShowPerBeat]);
			gridToDraw <= rightPos; gridToDraw = EDITOR_PTR->mapData->getNextStandardGrid(gridToDraw, gridToShowPerBeatTable[_gridToShowPerBeat]))
		{
			int nowLevel = EDITOR_PTR->mapData->getGridLevel(gridToDraw);
			
			float tHeightFactor = BeatLineHeightFactor / float(nowLevel);

			graphics->drawLine(width*(gridToDraw-leftPos)/rangeGridNum,
								height*(1- BeatLineHeightFactor*tHeightFactor)-1,
								width*(gridToDraw-leftPos)/rangeGridNum,
								height-1);
		}

		//Draw StopLine
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

		
		//Draw NowLine

		graphics->setColor(lineColor);

		graphics->drawLine((int(width)>>1)-1,0,(int(width)>>1)-1,height-1);
		graphics->drawLine((int(width)>>1)+1,0,(int(width)>>1)+1,height-1);
		graphics->drawLine(0,height-1,width,height-1);

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
								* float(_beatNumberPerScreen)*_showRangeFactor * GridPerBeat + CORE_PTR->getRunPosition();
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
}