
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
		_maxGridHeightFactor(0.5),
		isMouseOn(false),
		isDraggingNote(false)
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

		static const gcn::Color noteColors[4] = {gcn::Color(222,0,255),
													gcn::Color(222,0,15),
													gcn::Color(11,177,255),
													gcn::Color(0,222,15)};

		static const gcn::Color mouseLine = gcn::Color(255,255,0,255);

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

		const float noteBlockSize = height*0.1;

		//float leftPos = CORE_PTR->getRunPosition();

		//Draw background color
		graphics->setColor(backGroundColor);
		graphics->fillRectangle(gcn::Rectangle(0,0,width,height));

#pragma region Draw Notes		

		//int noteBeginToDraw,noteEndToDraw;
		//EDITOR_PTR->mapData->findNoteIndexInRange(leftPos, rightPos, noteBeginToDraw, noteEndToDraw);
		for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];
			if(thisNote.notePoint[0].position > rightPos) break;

			if(thisNote.notePoint.size()==1 && thisNote.notePoint[0].position >= leftPos && thisNote.notePoint[0].position <= rightPos)
			{
				float drawPos =  ((float)thisNote.notePoint[0].position -leftPos)/rangeGridNum * width;
				int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(thisNote.notePoint[0].type);

				gcn::Rectangle toDraw(drawPos-noteBlockSize/2,noteBlockSize*noteTypeIndex,noteBlockSize,noteBlockSize);

				graphics->setColor(noteColors[noteTypeIndex%4]);
				graphics->fillRectangle(toDraw);

				toDraw.width-=1;
				toDraw.height-=1;

				graphics->setColor(lineColor);
				graphics->drawRectangle(toDraw);
			}
			else if(thisNote.notePoint.size()==2 && ((thisNote.notePoint[0].position >= leftPos && thisNote.notePoint[0].position <= rightPos)
				                                   || (thisNote.notePoint[1].position >= leftPos && thisNote.notePoint[1].position <= rightPos)))
			{
				float leftDrawPos = ((float)thisNote.notePoint[0].position -leftPos)/rangeGridNum * width;
				float rightDrawPos = ((float)thisNote.notePoint[1].position -leftPos)/rangeGridNum * width;
				int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(thisNote.notePoint[0].type);

				if(leftDrawPos<0) leftDrawPos=0;
				if(rightDrawPos>width) rightDrawPos=width;

				graphics->setColor(noteColors[noteTypeIndex%4]);
				graphics->fillRectangle(gcn::Rectangle(leftDrawPos,noteBlockSize*noteTypeIndex,(rightDrawPos-leftDrawPos+1),noteBlockSize));
			}
		}


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

		
		
		if(isMouseOn)
		{
			float mouseGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
			int mouseGridXPos = (mouseGrid-leftPos)/rangeGridNum*width;

			graphics->setColor(mouseLine);

			if( (float)nowMouseYPos < height*0.8)
			{
				int mouseGridYPos = ((float)nowMouseYPos/(height*0.8))*8;

				int mouseYPosToDraw = ((float)mouseGridYPos*0.1+0.05)*height;
				graphics->drawLine(0,mouseYPosToDraw,width-1,mouseYPosToDraw);
			}
			
			graphics->drawLine(mouseGridXPos,0,mouseGridXPos,height-1);
		}
		

#pragma endregion Draw Now Line

	}




	void Timeline::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		nowMouseYPos = mouseEvent.getY();

		isSelecting = false;
		isDraggingNote = false;
	}

	void Timeline::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		nowMouseYPos = mouseEvent.getY();


		float width = getWidth();
		float height = getHeight();

		int GridPerBeat = EDITCONFIG->GridPerBeat;
		int _beatNumberPerScreen = EDITCONFIG->getBeatNumberPerScreen();
		int _gridToShowPerBeat = EDITCONFIG->getGridToShowPerBeat();
		float _showRangeFactor = EDITCONFIG->getShowRangeScale();

		float rangeGridNum = float(_beatNumberPerScreen)*_showRangeFactor * GridPerBeat;
		float mapBeginOffSet = 0;
		float leftPos = (float)CORE_PTR->getRunPosition() - rangeGridNum/2 + mapBeginOffSet;
		float rightPos = leftPos + rangeGridNum;


		if(!isDraggingNote && !isSelecting)  //Detect to drag note or select note
		{
			//Check if is dragging a note
			int toSelect = findNoteByMousePos(nowMouseXPos,nowMouseYPos);
			if(toSelect!=-1)
			{
				//Add this to select

				if (!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();
				
				if(!EDITCONFIG->isNoteSelected(toSelect))
					EDITCONFIG->addSelectedNote(toSelect);

				lastDragPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

				isDraggingNote=true;
			}
			else
			{
				_selectedBegin = (float(mouseEvent.getX()) / float(getWidth()) - 0.5) 
					* float(EDITCONFIG->getBeatNumberPerScreen())*EDITCONFIG->getShowRangeScale() * EDITCONFIG->GridPerBeat + CORE_PTR->getRunPosition();
				isSelecting=true;
			}
		}


		if(isDraggingNote)
		{
			int nowDragPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

			if(nowDragPos>=0 && nowDragPos<= (int)CORE_FLOW_PTR->getTotalPosition())
				if(nowDragPos!=lastDragPos)
				{
					for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
						EDITOR_PTR->mapData->note_modifyTimePos(EDITCONFIG->noteSelected[i],nowDragPos-lastDragPos,true);
					EDITOR_PTR->mapData->sortNote();
					EDITUTILITY.reCaltTime();
				}

			lastDragPos=nowDragPos;
		}


	}

	void Timeline::mouseMoved(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		nowMouseYPos = mouseEvent.getY();
	}

	void Timeline::mouseReleased(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		nowMouseYPos = mouseEvent.getY();


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


		if(isSelecting)
			isSelecting = false;
		else if(isDraggingNote)
			isDraggingNote = false;
		else
		{
			//Check if is dragging a note
			int toSelect = findNoteByMousePos(nowMouseXPos,nowMouseYPos);
			if(toSelect!=-1)
			{
				if (!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				//Add this to select
				EDITCONFIG->addSelectedNote(toSelect);
			}
			else //Set Position
			{
				int nowSetPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

				if(nowSetPos>=0&&nowSetPos<=(int)CORE_FLOW_PTR->getTotalPosition())
					EDITUTILITY.setPosition(nowSetPos);
			}

		}
		
	}


	void Timeline::setMaxGridHeightFactor(float factor)
	{
		_maxGridHeightFactor = factor;
	}


	int Timeline::findNoteByMousePos(int x,int y)
	{
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

		const float noteBlockSize = height*0.1;


		int mouseXGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
		
		if( (float)nowMouseYPos < height*0.8)
		{
			int mouseGridYPos = ((float)nowMouseYPos/(height*0.8))*8;
			return EDITOR_PTR->mapData->findNoteIndexByType(mouseXGrid, EDITOR_PTR->mapData->getNoteTypeFromNoteTypeIndex(mouseGridYPos));
		}
		else
			return -1;
	}








}