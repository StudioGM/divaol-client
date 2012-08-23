
#include "divaeditor/Widgets/DivaEditorTimelineWidget.h"
#include "divaeditor/Component/DivaEditorStandardOperation.h"
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
		isDraggingNote(false),
		placingLong(false),
		placingCombo(false),
		dragingLongPos(false)
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

		static const gcn::Color noteColorsSelected[4] = {gcn::Color(33,255,0),
													gcn::Color(33,255,240),
													gcn::Color(244,78,0),
													gcn::Color(255,33,240)};

		static const gcn::Color mouseLine = gcn::Color(255,255,0,255);
		static const gcn::Color selectedColor(255,255,0,255);

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

				//if(EDITCONFIG->isNoteSelected(i))
					//graphics->setColor(noteColorsSelected[noteTypeIndex%4]);
				//else
				gcn::Color thisNoteFillColor = noteColors[noteTypeIndex%4];
				
				graphics->setColor(thisNoteFillColor);
				graphics->fillRectangle(toDraw);

				toDraw.width-=1;
				toDraw.height-=1;

				graphics->setColor(gcn::Color(0,0,0,255));
				graphics->drawRectangle(toDraw);

				if(EDITCONFIG->isNoteSelected(i))
				{
					graphics->setColor(selectedColor);
					toDraw.x-=1;
					toDraw.y-=1;
					toDraw.width+=2;
					toDraw.height+=2;
					graphics->drawRectangle(toDraw);
				}

			}
			else if(thisNote.notePoint.size()==2 && ((thisNote.notePoint[0].position >= leftPos && thisNote.notePoint[0].position <= rightPos)
				                                   || (thisNote.notePoint[1].position >= leftPos && thisNote.notePoint[1].position <= rightPos)))
			{
				float leftDrawPos = ((float)thisNote.notePoint[0].position -leftPos)/rangeGridNum * width;
				float rightDrawPos = ((float)thisNote.notePoint[1].position -leftPos)/rangeGridNum * width;
				int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(thisNote.notePoint[0].type);

				if(leftDrawPos<0) leftDrawPos=0;
				if(rightDrawPos>width) rightDrawPos=width;

				gcn::Rectangle toDraw(leftDrawPos,noteBlockSize*noteTypeIndex,(rightDrawPos-leftDrawPos+1),noteBlockSize);

				//if(EDITCONFIG->isNoteSelected(i))
					//graphics->setColor(noteColorsSelected[noteTypeIndex%4]);
				//else
				graphics->setColor(noteColors[noteTypeIndex%4]);
				graphics->fillRectangle(toDraw);


				if(dragingLongPos && i==dragingLongPosIndex)
				{
					float nowDragToPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
					nowDragToPos=(nowDragToPos-leftPos)/rangeGridNum*width;
					if(leftDrawPos>nowDragToPos)
						std::swap(leftDrawPos,nowDragToPos);

					toDraw = gcn::Rectangle(leftDrawPos,noteBlockSize*noteTypeIndex,(nowDragToPos-leftDrawPos+1),noteBlockSize);
				}

				if(EDITCONFIG->isNoteSelected(i))
				{
					graphics->setColor(selectedColor);
					toDraw.x-=1;
					toDraw.y-=1;
					toDraw.width+=2;
					toDraw.height+=2;
					graphics->drawRectangle(toDraw);
				}
			}
		}

		//Draw Note Will be placed
		if(placingLong||placingCombo)
		{
			int mouseXGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
			int tBeginGrid = _placeNoteBeginPos;

			if(mouseXGrid<tBeginGrid)
				std::swap(mouseXGrid,tBeginGrid);

			float leftDrawPos = (tBeginGrid -leftPos)/rangeGridNum * width;
			float rightDrawPos = (mouseXGrid -leftPos)/rangeGridNum * width;
			int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(_placeNoteType);

			if(leftDrawPos<0) leftDrawPos=0;
			if(rightDrawPos>width) rightDrawPos=width;

			gcn::Rectangle toDraw(leftDrawPos,noteBlockSize*noteTypeIndex,(rightDrawPos-leftDrawPos+1),noteBlockSize);

			//if(EDITCONFIG->isNoteSelected(i))
			//graphics->setColor(noteColorsSelected[noteTypeIndex%4]);
			//else
			gcn::Color toDrawNoteColor = noteColors[noteTypeIndex%4];
			toDrawNoteColor.a = 100;
			graphics->setColor(toDrawNoteColor);
			graphics->fillRectangle(toDraw);
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

			if(nowLevel==1)
			{
				graphics->setFont(getFont());
				graphics->drawTextW(L"#" + iToWS(EDITOR_PTR->mapData->getPeriodfromGrid(gridToDraw)),width*(gridToDraw-leftPos)/rangeGridNum+1,height-20);
			}
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

		
		
		if(isMouseOn && isEnabled())
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

		if(mouseEvent.getButton()==gcn::MouseEvent::LEFT)
		{
			isSelecting = false;
			isDraggingNote = false;
		}
		
	}

	void Timeline::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMouseXPos = mouseEvent.getX();
		nowMouseYPos = mouseEvent.getY();

		
		if(mouseEvent.getButton()==gcn::MouseEvent::LEFT)
		{
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

			bool lastIsDraggingNote = isDraggingNote;

			if(!isDraggingNote && !isSelecting && !dragingLongPos)  //Detect to drag note or select note
			{
				//Check if is dragging a note
				int toSelect = findNoteByMousePos(nowMouseXPos,nowMouseYPos);
				if(toSelect!=-1)
				{
					//Add this to select

					if (!EDITCONFIG->isNoteSelected(toSelect)&&!EDITCONFIG->isctrl)
						EDITCONFIG->clearSelectedNote();

					if(!EDITCONFIG->isNoteSelected(toSelect))
						EDITCONFIG->addSelectedNote(toSelect);


					//check if drag note pos and or ,  or modify long note's tail
					divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[toSelect];

					if(thisNote.notePoint.size()>1)
					{
						int mouseXGrid = (float)nowMouseXPos/width * rangeGridNum+leftPos;
						int delta = height*0.1 / width * rangeGridNum;

						if(mouseXGrid>=(int)thisNote.notePoint[1].position-delta && mouseXGrid<=(int)thisNote.notePoint[1].position+delta)
						{
							//drag long note pos detected
							dragingLongPosIndex = toSelect;
							dragingLongPos = true;
						}
					}
					

					//If drag long note pos not detected, then it means drag note
					if(!dragingLongPos)
					{
						lastDragPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
						lastDragType = ((float)nowMouseYPos/(height*0.8))*8;

						isDraggingNote=true;
					}

					firstDrag=true;
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

				int nowDragType = ((float)nowMouseYPos/(height*0.8))*8;

				if(nowDragPos>=0 && nowDragPos<= (int)CORE_FLOW_PTR->getTotalPosition())
				{
					if(nowDragPos!=lastDragPos && EDITCONFIG->noteSelected.size()>0)
					{
						EDITCONFIG->LockOperation(getId()+"_drag");

						DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
						thisModifySet->needToRecalcTime = true;

						for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
							thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],nowDragPos-lastDragPos,true));

						EDITCONFIG->addAndDoOperation(thisModifySet, getId()+"_drag");

						if(!firstDrag)
							EDITCONFIG->mergeLastTwoOperation();

						firstDrag=false;
					}

					if(nowDragType!=lastDragType)
					{
						EDITCONFIG->LockOperation(getId()+"_drag");

						DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();

						for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
							thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],nowDragType-lastDragType,true,true));

						EDITCONFIG->addAndDoOperation(thisModifySet, getId()+"_drag");

						if(!firstDrag)
							EDITCONFIG->mergeLastTwoOperation();

						firstDrag=false;
					}
				}

				lastDragPos=nowDragPos;

				lastDragType = nowDragType;
			}
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

		if(mouseEvent.getButton()==gcn::MouseEvent::LEFT)
		{
			if(isSelecting)
			{
				if (!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				if(_selectedEnd<_selectedBegin)
					std::swap(_selectedBegin,_selectedEnd);

				for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
				{
					divacore::MapNote& thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];

					if(thisNote.notePoint[0].position>_selectedEnd)
						break;

					bool needToSelect = false;

					for(int notePointI=0;notePointI<thisNote.notePoint.size();notePointI++)
						needToSelect = needToSelect || (thisNote.notePoint[notePointI].position>=_selectedBegin && thisNote.notePoint[notePointI].position<=_selectedEnd);

					if(needToSelect && !EDITCONFIG->isNoteSelected(i))
						EDITCONFIG->addSelectedNote(i);
				}

				isSelecting = false;
			}
			else if(dragingLongPos)
			{
				//EDITCONFIG->addAndDoOperation()
				int nowDragToPos = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

				if(nowDragToPos != EDITOR_PTR->mapData->coreInfoPtr->notes[dragingLongPosIndex].notePoint[0].position && 
						nowDragToPos != EDITOR_PTR->mapData->coreInfoPtr->notes[dragingLongPosIndex].notePoint[1].position)
				{
					EDITCONFIG->addAndDoOperation(new DivaEditorOperation_ModifyNote(dragingLongPosIndex,
														EDITOR_PTR->mapData->coreInfoPtr->notes[dragingLongPosIndex].notePoint[0].position,
														nowDragToPos,DivaEditorOperation_ModifyNote::LONGNOTETIMEPOS));
				}

				dragingLongPos=false;
			}
			else if(isDraggingNote)
			{
				EDITCONFIG->UnlockOperation();
				isDraggingNote = false;
			}
			else
			{
				//Check if is select a note
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
		else if(mouseEvent.getButton()==gcn::MouseEvent::RIGHT)
		{

			//if the note is selected, delete it
			//else , select the note, and delete it

			//Check if is dragging a note
			int toSelect = findNoteByMousePos(nowMouseXPos,nowMouseYPos);

			if(toSelect!=-1)
			{
				//Check if it's selected
				if(!EDITCONFIG->isNoteSelected(toSelect))
				{
					if (!EDITCONFIG->isctrl)
						EDITCONFIG->clearSelectedNote();

					EDITCONFIG->addSelectedNote(toSelect);
				}

				//Begin to delete

				DivaEditorOperationSet *thisDeleteSet = new DivaEditorOperationSet();

				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisDeleteSet->addOperation(new DivaEditorOperation_DeleteNote(EDITCONFIG->noteSelected[i]));

				EDITCONFIG->addAndDoOperation(thisDeleteSet);
			}
			else //Clear all selection
			{
				EDITCONFIG->clearSelectedNote();
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


		//int mouseXGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());
		int mouseXGrid = (float)nowMouseXPos/width * rangeGridNum+leftPos;
		
		if( (float)nowMouseYPos < height*0.8)
		{
			int mouseGridYPos = ((float)nowMouseYPos/(height*0.8))*8;
			return EDITOR_PTR->mapData->findNoteIndexByType(mouseXGrid, EDITOR_PTR->mapData->getNoteTypeFromNoteTypeIndex(mouseGridYPos),height*0.1 / width * rangeGridNum );
		}
		else
			return -1;
	}



	void Timeline::onKeyPressed(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = false;
		if(thisKey>='A'&&thisKey<='Z') 
		{
			thisKey+='a'-'A';
			caps=true;
		}
		

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

		

		if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			int mouseXGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

			if(mouseXGrid>=0&&mouseXGrid<=CORE_FLOW_PTR->getTotalPosition() && isMouseOn)
			{
				int guessX,guessY,guessTailX,guessTailY;


				if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::NORMAL)
				{
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(mouseXGrid,guessX,guessY,guessTailX,guessTailY);

					int toAddFind = EDITOR_PTR->mapData->findNoteIndexByType(mouseXGrid, EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps));
					if(toAddFind==-1)
						EDITCONFIG->addAndDoOperation(new DivaEditorOperation_AddNormalNote(mouseXGrid,thisKey,caps,guessX,guessY,guessTailX,guessTailY));
					else
					{
						EDITCONFIG->clearSelectedNote();
						EDITCONFIG->addSelectedNote(toAddFind);
					}
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && !placingLong)
				{
					_placeNoteType = EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps);
					_placeNoteBeginPos = mouseXGrid;

					placingLong = true;
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && !placingCombo)
				{
					_placeNoteType = EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps);
					_placeNoteBeginPos = mouseXGrid;

					placingCombo = true;
				}
			}

		}
	}

	void Timeline::onKeyReleased(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = false;
		if(thisKey>='A'&&thisKey<='Z') 
		{
			thisKey+='a'-'A';
			caps=true;
		}


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


		if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			int mouseXGrid = EDITOR_PTR->mapData->getNearestStandardGrid((float)nowMouseXPos/width * rangeGridNum+leftPos, EDITCONFIG->getGridToShowPerBeat());

			if(mouseXGrid<0||mouseXGrid>CORE_FLOW_PTR->getTotalPosition())
			{
				placingLong=false;
				placingCombo=false;
				return;
			}
			else if(mouseXGrid<_placeNoteBeginPos)
				std::swap(mouseXGrid,_placeNoteBeginPos);

			int guessX,guessY,guessTailX,guessTailY;
			EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(_placeNoteBeginPos,guessX,guessY,guessTailX,guessTailY);

			if(mouseXGrid==_placeNoteBeginPos && isMouseOn) // Place a single note
			{
				int toAddFind = EDITOR_PTR->mapData->findNoteIndexByType(mouseXGrid, EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps));
				if(toAddFind==-1)
					EDITCONFIG->addAndDoOperation(new DivaEditorOperation_AddNormalNote(mouseXGrid,thisKey,caps,guessX,guessY,guessTailX,guessTailY));
				else
				{
					EDITCONFIG->clearSelectedNote();
					EDITCONFIG->addSelectedNote(toAddFind);
				}
			}
			else if(placingLong && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && isMouseOn)
			{
				divacore::MapNote placingNote = EDITOR_PTR->mapData->initNote(_placeNoteBeginPos,thisKey,caps,guessX,guessY,guessTailX,guessTailY,"long");
				EDITOR_PTR->mapData->finishLongNote(placingNote,mouseXGrid);
				
				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_AddLongNote(placingNote));
			}
			else if(placingCombo && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && isMouseOn)
			{
				divacore::MapNote placingNote = EDITOR_PTR->mapData->initNote(_placeNoteBeginPos,thisKey,caps,guessX,guessY,guessTailX,guessTailY,"pingpong");
				EDITOR_PTR->mapData->finishComboNote(placingNote,mouseXGrid);

				EDITCONFIG->addAndDoOperation(new DivaEditorOperation_AddLongNote(placingNote));
			}

			placingLong=false;
			placingCombo=false;
		}
	}




}