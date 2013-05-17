
#include "divaeditor/Widgets/DivaEditorNoteArea.h"
#include "divaeditor/Core/DivaEditorMapData.h"
#include "divaeditor/Component/DivaEditorStandardOperation.h"
#include "divaeditor/DivaEditorCommon.h"

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


#define NOTEAREA_DEBUG

	NoteArea::NoteArea()
	{
		nowMousePos.x=-1;
		nowMousePos.y=-1;
		notePlacedPos=-1;
		pressedTimePos=-1;
		isMouseOn=false;
		isSelecting=false;
		moveTail=false;
		placingLong=false;
		placingCombo=false;
		isDraging=false;
		isDragingTime=false;
		
		setFocusable(true);
		setFrameSize(1);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));
	}

	void NoteArea::adjustSize(float fact)
	{
		factor = fact;
		float width = (EDITCONFIG->NoteAreaWidth+EDITCONFIG->NoteAreaTailAreaSize*2+1)*EDITCONFIG->NoteAreaGridSize;
		float height = (EDITCONFIG->NoteAreaHeight+EDITCONFIG->NoteAreaTailAreaSize*2+1)*EDITCONFIG->NoteAreaGridSize;

		setWidth(width*factor);
		setHeight(height*factor);

		setPosition(getX()-(EDITCONFIG->NoteAreaGridSize*factor*0.5)+2,getY()-(EDITCONFIG->NoteAreaGridSize*factor*0.5)+2);
	}

	void NoteArea::updateMousePos()
	{
		float width = getWidth(),height = getHeight();
		if((nowMousePos.x>=0&&nowMousePos.x<width && isMouseOn) || isSelecting)
		{
			nowGridSelectX = nowMousePos.x/(width)*float(EDITCONFIG->NoteAreaWidth+EDITCONFIG->NoteAreaTailAreaSize*2+1);
		}
		else
		{
			nowGridSelectX=-1;
		}

		if((nowMousePos.y>=0&&nowMousePos.y<height && isMouseOn) || isSelecting)
		{
			nowGridSelectY = nowMousePos.y/(height)*float(EDITCONFIG->NoteAreaHeight+EDITCONFIG->NoteAreaTailAreaSize*2+1);
		}
		else
		{
			nowGridSelectY=-1;
		}
	}

	void NoteArea::draw(gcn::Graphics* graphics)
	{

		static const gcn::Color normalLineColor(255,255,255,255);
		static const gcn::Color selectedColor(255,255,0,255);
		static const gcn::Color selectAreaColor(255,255,255,100);

		float width = getWidth(),height = getHeight();


		static const gcn::Color noteColors[4] = {gcn::Color(222,0,255,150),
											gcn::Color(222,0,15,150),
											gcn::Color(11,177,255,150),
											gcn::Color(0,222,15,150)};


#ifdef NOTEAREA_DEBUG

		//Draw Boarder
		graphics->setColor(normalLineColor);
		graphics->drawLine(0,0,0,height-1);
		graphics->drawLine(0,height-1,width-1,height-1);
		graphics->drawLine(0,0,width-1,0);
		graphics->drawLine(width-1,0,width-1,height-1);


		if(nowGridSelectX!=-1 && nowGridSelectY!=-1 && isMouseOn)
		{
			graphics->setColor(normalLineColor);
			graphics->fillRectangle(gcn::Rectangle(float(nowGridSelectX*EDITCONFIG->NoteAreaGridSize)*factor,
													float(nowGridSelectY*EDITCONFIG->NoteAreaGridSize)*factor,
													float(EDITCONFIG->NoteAreaGridSize)*factor,
													float(EDITCONFIG->NoteAreaGridSize)*factor));
		}

#endif

		float nowPosition = CORE_PTR->getRunPosition();

		//Draw selected note's tail

		for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];

			if(thisNote.notePoint[0].position>nowPosition + EDITCONFIG->GridPerBeat*4)
				break;

			//Draw Note need
			float deltaPos;

			if(thisNote.notePoint.size()==1)
			{
				deltaPos = abs((int)thisNote.notePoint[0].position-nowPosition);
			}
			else
			{
				if(nowPosition>=thisNote.notePoint[0].position && nowPosition<=thisNote.notePoint[1].position)
					deltaPos = 0;
				else if(nowPosition<thisNote.notePoint[0].position)
					deltaPos = abs((int)thisNote.notePoint[0].position-nowPosition);
				else
					deltaPos = abs((int)thisNote.notePoint[1].position-nowPosition);
			}


			if(deltaPos<=(EDITCONFIG->GridPerBeat*4))
			{
				int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(thisNote.notePoint[0].type);
				gcn::Color thisNoteColor = noteColors[noteTypeIndex%4];
				thisNoteColor.a = (float)thisNoteColor.a * (1.0f - deltaPos/float(EDITCONFIG->GridPerBeat*4));

				graphics->setColor(thisNoteColor);

				gcn::Rectangle rectangleToDraw(float((thisNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float((thisNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor);

				graphics->fillRectangle(rectangleToDraw);
			}
			
		}
		


		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			graphics->setColor(selectedColor);
			divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];

			for(int j=0;j<selectedNote.notePoint.size();j++)
			{
				graphics->drawRectangle(gcn::Rectangle(float((selectedNote.notePoint[j].x+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float((selectedNote.notePoint[j].y+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor));
			}

			//Draw Tail
			if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
				selectedNote.arg.find("taily")!=selectedNote.arg.end())
			{
				float centerX = (selectedNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize;
				float centerY = (selectedNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize;
				float tailX = centerX + divacore::Argument::asInt("tailx",selectedNote.arg)*2000;
				float tailY = centerY + divacore::Argument::asInt("taily",selectedNote.arg)*2000;

				graphics->setColor(gcn::Color(255,255,255,255));

				//graphics->setFont(getFont());
				//graphics->drawText(iToS(moveTailX)+","+iToS(moveTailY),0,0);

				if(!moveTail)
					graphics->drawLine(centerX*factor,centerY*factor,tailX*factor,tailY*factor);
				else
					graphics->drawLine(centerX*factor,centerY*factor,moveTailX*1000,moveTailY*1000);
			}
			
		}
		
		if(isSelecting) //Draw Selecting Area
		{
			float x1=selectBeginX,y1=selectBeginY,x2=nowMousePos.x,y2=nowMousePos.y;
			if(x1>x2) swap(x1,x2);
			if(y1>y2) swap(y1,y2);
			graphics->setColor(selectAreaColor);
			graphics->fillRectangle(gcn::Rectangle(x1,y1,x2-x1,y2-y1));
		}




	}
	void NoteArea::logic()
	{
		if(moveTail && isMouseOn)
		{
			for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
			{
				divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];
				if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
					selectedNote.arg.find("taily")!=selectedNote.arg.end())
				{
					float centerX = (selectedNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize * factor;
					float centerY = (selectedNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize * factor;

					moveTailX = nowMousePos.x - centerX;
					moveTailY = nowMousePos.y - centerY;

				}
			}
		}
		else if(moveTail)
			changeSelectedNoteTailOver();

		
		if(placingLong && isMouseOn)
		{

		}
		else if(placingLong)
			placingLongNoteOver();

		if(placingCombo && isMouseOn)
		{
		}
		else if(placingCombo)
			placingComboNoteOver();
			
		int nearestStandard = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
	}

	void NoteArea::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		if(mouseEvent.getButton() == MouseEvent::LEFT)
		{
			isSelecting=false;
			isDraging=false;
			selectBeginX = nowMousePos.x;
			selectBeginY = nowMousePos.y;
			selectBeginGridX = nowGridSelectX;
			selectBeginGridY = nowGridSelectY;

			pressedTimePos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

			int nowToSelect = EDITOR_PTR->mapData->findNoteToSelectByPos(ceil(CORE_PTR->getRunPosition()), nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);

			if(EDITCONFIG->isNoteSelected(nowToSelect))
				isDragingTime=true;
		}
		
	}

	void NoteArea::mouseReleased(gcn::MouseEvent& mouseEvent)
	{

		if(mouseEvent.getButton()==MouseEvent::LEFT)
		{
			if(isDragingTime)
			{
				/*
				int thisPos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

				if(thisPos!=pressedTimePos && EDITCONFIG->noteSelected.size()>0)
				{
					DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
					thisModifySet->needToRecalcTime = true;

					for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
						thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],thisPos-pressedTimePos,true));

					EDITCONFIG->addAndDoOperation(thisModifySet);
				}
				*/
				//disable note area draging time function

				isDragingTime=false;
			}

			if(!isSelecting&&!isDraging)
			{
				int nowToSelect = EDITOR_PTR->mapData->findNoteToSelectByPos(ceil(CORE_PTR->getRunPosition()), nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);

				if(!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				if(nowToSelect!=-1)
					EDITCONFIG->addSelectedNote(nowToSelect);
			}

			if(isSelecting)
			{
				if(!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				vector<int> selected = EDITOR_PTR->mapData->findNoteToSelectByRange(ceil(CORE_PTR->getRunPosition()),selectBeginGridX-EDITCONFIG->NoteAreaTailAreaSize
					,selectBeginGridY-EDITCONFIG->NoteAreaTailAreaSize
					,nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize
					,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);
				for(int i=0;i<selected.size();i++)
					EDITCONFIG->addSelectedNote(selected[i]);
				isSelecting=false;
			}
			else if(isDraging)
			{
				EDITCONFIG->UnlockOperation();
				isDraging=false;
			}
		}
		else if(mouseEvent.getButton() == MouseEvent::RIGHT)
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisDeleteSet = new DivaEditorOperationSet();

				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisDeleteSet->addOperation(new DivaEditorOperation_DeleteNote(EDITCONFIG->noteSelected[i]));

				EDITCONFIG->addAndDoOperation(thisDeleteSet);
			}
		}
		
	}

	void NoteArea::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y = mouseEvent.getY();
		updateMousePos();

		if(!isDraging && !isSelecting && mouseEvent.getButton()==MouseEvent::LEFT)
		{

			int nowToSelect = EDITOR_PTR->mapData->findNoteToSelectByPos(ceil(CORE_PTR->getRunPosition()), nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);

			//if(!EDITCONFIG->isctrl)
				//EDITCONFIG->clearSelectedNote();

			if(nowToSelect!=-1 && !EDITCONFIG->isNoteSelected(nowToSelect))
			{
				if(!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();
				EDITCONFIG->addSelectedNote(nowToSelect);
			}


			//check if is clicked on some selected note
			bool clickedOnSelected = false;
			for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
				if(EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]].notePoint[0].x == nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize
					&&EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]].notePoint[0].y == nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize)
				{
					clickedOnSelected = true;
					break;
				}
			

			if(clickedOnSelected)
			{
				dragAreaRect = EDITOR_PTR->mapData->findSelectedAreaRectange();
				dragingPosX = (nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize) - dragAreaRect.x;
				dragingPosY = (nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize) - dragAreaRect.y;
				isDraging=true;
				isFristDrag=true;
			}
			else
				isSelecting=true;
		}	

		if(isDraging)
		{
			int deltaDragX = (nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize)-(dragAreaRect.x+dragingPosX),
				deltaDragY = (nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize)-(dragAreaRect.y+dragingPosY);
			if(deltaDragX!=0 || deltaDragY!=0)
			{
				//Check boarder
				if(dragAreaRect.x+deltaDragX>=0 && dragAreaRect.x+deltaDragX<=EDITCONFIG->NoteAreaWidth &&
					dragAreaRect.width+deltaDragX>=0 && dragAreaRect.width+deltaDragX<=EDITCONFIG->NoteAreaWidth &&
					dragAreaRect.y+deltaDragY>=0 && dragAreaRect.y+deltaDragY<=EDITCONFIG->NoteAreaHeight &&
					dragAreaRect.height+deltaDragY>=0 && dragAreaRect.height+deltaDragY<=EDITCONFIG->NoteAreaHeight)
				{
					//Lock operation
					EDITCONFIG->LockOperation(getId()+"_drag");

					//Operation move set of notes pos
					DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();

					for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
						thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],deltaDragX,deltaDragY,true));
					EDITCONFIG->addAndDoOperation(thisModifySet, getId()+"_drag");

					if(!isFristDrag)
						EDITCONFIG->mergeLastTwoOperation();

					isFristDrag=false;

					dragAreaRect.x += deltaDragX;
					dragAreaRect.width += deltaDragX;
					dragAreaRect.y += deltaDragY;
					dragAreaRect.height += deltaDragY;
				}
			}
		}
	}

	void NoteArea::onKeyPressed(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = false;
		if(thisKey>='A'&&thisKey<='Z') 
		{
			thisKey+='a'-'A';
			caps=true;
		}

		float nowTimePos = CORE_PTR->getRunPosition();

		if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			int noteSelectX = nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize;
			int noteSelectY = nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize;
			if(noteSelectX >= 0 && noteSelectX <= EDITCONFIG->NoteAreaWidth &&
				noteSelectY >= 0 && noteSelectY <= EDITCONFIG->NoteAreaHeight && isMouseOn)
			{
				if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::NORMAL)
				{
					int guessX,guessY,guessTailX,guessTailY;
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(nowTimePos,guessX,guessY,guessTailX,guessTailY);

					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(nowTimePos,EDITCONFIG->getGridToShowPerBeat());

					int toAddFind = EDITOR_PTR->mapData->findNoteIndexByType(pos, EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps));
					if(toAddFind==-1)
					{
						DivaEditorOperation_AddNormalNote *toOp = new DivaEditorOperation_AddNormalNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY);
						if(CORE_FLOW_PTR->getState() == CoreFlow::RUN)
						{
							toOp->needToRefreshAll=false;
							toOp->needToRecalcTime=false;
							EDITCONFIG->needReCalcNextTime=true;
						}
						EDITCONFIG->addAndDoOperation(toOp);
					}
					else
					{
						EDITCONFIG->clearSelectedNote();
						EDITCONFIG->addSelectedNote(toAddFind);
					}
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && !placingLong)
				{
					int guessX,guessY,guessTailX,guessTailY;
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(nowTimePos,guessX,guessY,guessTailX,guessTailY);

					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY,"long");

					placingLong = true;
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && !placingCombo)
				{
					int guessX,guessY,guessTailX,guessTailY;
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(nowTimePos,guessX,guessY,guessTailX,guessTailY);

					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY,"pingpong");

					placingCombo = true;
				}
			}
			
		}
		else if(thisKey=='q')
		{
			moveTail=true;
		}
	}

	void NoteArea::onKeyReleased(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = false;
		if(thisKey>='A'&&thisKey<='Z') 
		{
			thisKey+='a'-'A';
			caps=true;
		}

		if(thisKey=='q')
		{
			changeSelectedNoteTailOver();
		}
		else if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			if(placingLong && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && isMouseOn)
			{
				placingLongNoteOver();
			}
			else if(placingCombo && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && isMouseOn)
			{
				placingComboNoteOver();
			}

			placingLong=false;
			placingCombo=false;
		}
	}

	void NoteArea::mouseEntered(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=true;
	}

	void NoteArea::mouseExited(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=false;
	}

	void NoteArea::mouseMoved(MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y = mouseEvent.getY();
		updateMousePos();
	}

	void NoteArea::changeSelectedNoteTailOver()
	{
		if(EDITCONFIG->noteSelected.size()>0)
		{
			DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();

			for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
			{
				divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];
				if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
					selectedNote.arg.find("taily")!=selectedNote.arg.end())
					thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],moveTailX,moveTailY));
			}

			EDITCONFIG->addAndDoOperation(thisModifySet);
		}

		moveTail=false;
	}


	void NoteArea::placingLongNoteOver()
	{
		int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
		EDITOR_PTR->mapData->finishLongNote(placingNote,pos);


		DivaEditorOperation_AddLongNote *toOp = new DivaEditorOperation_AddLongNote(placingNote);
		if(CORE_FLOW_PTR->getState() == CoreFlow::RUN && (placingNote.notePoint[placingNote.notePoint.size()].position <= CORE_PTR->getRunPosition()))
		{
			toOp->needToRefreshAll=false;
			toOp->needToRecalcTime=false;
			EDITCONFIG->needReCalcNextTime=true;
		}
		EDITCONFIG->addAndDoOperation(toOp);
	}

	void NoteArea::placingComboNoteOver()
	{
		int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
		EDITOR_PTR->mapData->finishComboNote(placingNote,pos);

		DivaEditorOperation_AddLongNote *toOp = new DivaEditorOperation_AddLongNote(placingNote);
		if(CORE_FLOW_PTR->getState() == CoreFlow::RUN && (placingNote.notePoint[placingNote.notePoint.size()].position <= CORE_PTR->getRunPosition()))
		{
			toOp->needToRefreshAll=false;
			toOp->needToRecalcTime=false;
			EDITCONFIG->needReCalcNextTime=true;
		}
		EDITCONFIG->addAndDoOperation(toOp);
	}



#pragma region PICKMEUP_NOTEAREA

	PickMeUpNoteArea::PickMeUpNoteArea()
	{
		nowMousePos.x=-1;
		nowMousePos.y=-1;
		notePlacedPos=-1;
		pressedTimePos=-1;
		isMouseOn=false;
		isSelecting=false;
		moveTail=false;
		placingLong=false;
		placingCombo=false;
		isDraging=false;
		isDragingTime=false;
		
		setFocusable(true);
		setFrameSize(1);

		addMouseListener(this);
		addKeyListener(this);
		addFocusListener(this);

		setBaseColor(gcn::Color(0,0,0,0));
	}


	void PickMeUpNoteArea::updateMousePos()
	{
		float width = getWidth(),height = getHeight();
		if((nowMousePos.x>=0&&nowMousePos.x<width && isMouseOn) || isSelecting)
		{
			nowGridSelectX = nowMousePos.x/(width)*float(EDITCONFIG->NoteAreaWidth+EDITCONFIG->NoteAreaTailAreaSize*2+1);
			if(nowGridSelectX<EDITCONFIG->NoteAreaWidth/2)
				nowGridSelectX = 2;
			else
				nowGridSelectX = EDITCONFIG->NoteAreaWidth-2;
		}
		else
		{
			nowGridSelectX=-1;
		}

		if((nowMousePos.y>=0&&nowMousePos.y<height && isMouseOn) || isSelecting)
		{
			nowGridSelectY = nowMousePos.y/(height)*float(EDITCONFIG->NoteAreaHeight+EDITCONFIG->NoteAreaTailAreaSize*2+1);
			if(nowGridSelectY<EDITCONFIG->NoteAreaHeight/2)
				nowGridSelectY = 2;
			else
				nowGridSelectY = EDITCONFIG->NoteAreaHeight-2;
		}
		else
		{
			nowGridSelectY=-1;
		}
	}

	void PickMeUpNoteArea::draw(gcn::Graphics* graphics)
	{

		static const gcn::Color normalLineColor(255,255,255,255);
		static const gcn::Color blockColor(255,255,255,100);
		static const gcn::Color selectedColor(255,255,0,255);
		static const gcn::Color selectAreaColor(255,255,255,100);

		float width = getWidth(),height = getHeight();


		static const gcn::Color noteColors[4] = {gcn::Color(222,0,255,150),
											gcn::Color(222,0,15,150),
											gcn::Color(11,177,255,150),
											gcn::Color(0,222,15,150)};


#ifdef NOTEAREA_DEBUG

		//Draw Boarder
		graphics->setColor(normalLineColor);
		graphics->drawLine(0,0,0,height-1);
		graphics->drawLine(0,height-1,width-1,height-1);
		graphics->drawLine(0,0,width-1,0);
		graphics->drawLine(width-1,0,width-1,height-1);


		if(nowGridSelectX!=-1 && nowGridSelectY!=-1 && isMouseOn)
		{
			graphics->setColor(blockColor);
			graphics->fillRectangle(gcn::Rectangle((float(nowGridSelectX) - 0.5f) * EDITCONFIG->NoteAreaGridSize * factor,
													(float(nowGridSelectY) - 0.5f) * EDITCONFIG->NoteAreaGridSize * factor,
													float(EDITCONFIG->NoteAreaGridSize*2)*factor,
													float(EDITCONFIG->NoteAreaGridSize*2)*factor));
		}

#endif

		float nowPosition = CORE_PTR->getRunPosition();

		//Draw selected note's tail

		for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];

			if(thisNote.notePoint[0].position>nowPosition + EDITCONFIG->GridPerBeat*4)
				break;

			//Draw Note need
			float deltaPos;

			if(thisNote.notePoint.size()==1)
			{
				deltaPos = abs((int)thisNote.notePoint[0].position-nowPosition);
			}
			else
			{
				if(nowPosition>=thisNote.notePoint[0].position && nowPosition<=thisNote.notePoint[1].position)
					deltaPos = 0;
				else if(nowPosition<thisNote.notePoint[0].position)
					deltaPos = abs((int)thisNote.notePoint[0].position-nowPosition);
				else
					deltaPos = abs((int)thisNote.notePoint[1].position-nowPosition);
			}


			if(deltaPos<=(EDITCONFIG->GridPerBeat*4))
			{
				int noteTypeIndex = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(thisNote.notePoint[0].type);
				gcn::Color thisNoteColor = noteColors[noteTypeIndex%4];
				thisNoteColor.a = (float)thisNoteColor.a * (1.0f - deltaPos/float(EDITCONFIG->GridPerBeat*4));

				graphics->setColor(thisNoteColor);

				gcn::Rectangle rectangleToDraw(float((thisNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float((thisNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor);

				graphics->fillRectangle(rectangleToDraw);
			}
			
		}
		


		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			graphics->setColor(selectedColor);
			divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];

			for(int j=0;j<selectedNote.notePoint.size();j++)
			{
				graphics->drawRectangle(gcn::Rectangle(float((selectedNote.notePoint[j].x+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float((selectedNote.notePoint[j].y+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor,
					float(EDITCONFIG->NoteAreaGridSize*2)*factor));
			}

			//Draw Tail
			if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
				selectedNote.arg.find("taily")!=selectedNote.arg.end())
			{
				float centerX = (selectedNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize;
				float centerY = (selectedNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize;
				float tailX = centerX + divacore::Argument::asInt("tailx",selectedNote.arg)*2000;
				float tailY = centerY + divacore::Argument::asInt("taily",selectedNote.arg)*2000;

				graphics->setColor(gcn::Color(255,255,255,255));

				//graphics->setFont(getFont());
				//graphics->drawText(iToS(moveTailX)+","+iToS(moveTailY),0,0);

				if(!moveTail)
					graphics->drawLine(centerX*factor,centerY*factor,tailX*factor,tailY*factor);
				else
					graphics->drawLine(centerX*factor,centerY*factor,moveTailX*1000,moveTailY*1000);
			}
			
		}
		
		if(isSelecting) //Draw Selecting Area
		{
			float x1=selectBeginX,y1=selectBeginY,x2=nowMousePos.x,y2=nowMousePos.y;
			if(x1>x2) swap(x1,x2);
			if(y1>y2) swap(y1,y2);
			graphics->setColor(selectAreaColor);
			graphics->fillRectangle(gcn::Rectangle(x1,y1,x2-x1,y2-y1));
		}




	}
	void PickMeUpNoteArea::logic()
	{
		if(moveTail && isMouseOn)
		{
			for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
			{
				divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];
				if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
					selectedNote.arg.find("taily")!=selectedNote.arg.end())
				{
					float centerX = (selectedNote.notePoint[0].x+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize * factor;
					float centerY = (selectedNote.notePoint[0].y+EDITCONFIG->NoteAreaTailAreaSize + 0.5)*EDITCONFIG->NoteAreaGridSize * factor;

					moveTailX = nowMousePos.x - centerX;
					moveTailY = nowMousePos.y - centerY;

				}
			}
		}
		else if(moveTail)
			changeSelectedNoteTailOver();

		
		if(placingLong && isMouseOn)
		{

		}
		else if(placingLong)
			placingLongNoteOver();

		if(placingCombo && isMouseOn)
		{
		}
		else if(placingCombo)
			placingComboNoteOver();
			
		int nearestStandard = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
	}

	void PickMeUpNoteArea::mousePressed(gcn::MouseEvent& mouseEvent)
	{
		if(mouseEvent.getButton() == MouseEvent::LEFT)
		{
			isSelecting=false;
			isDraging=false;
			selectBeginX = nowMousePos.x;
			selectBeginY = nowMousePos.y;
			selectBeginGridX = nowGridSelectX;
			selectBeginGridY = nowGridSelectY;

			pressedTimePos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

			int nowToSelect = EDITOR_PTR->mapData->findNoteToSelectByPos(ceil(CORE_PTR->getRunPosition()), nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);

			if(EDITCONFIG->isNoteSelected(nowToSelect))
				isDragingTime=true;
		}
		
	}

	void PickMeUpNoteArea::mouseReleased(gcn::MouseEvent& mouseEvent)
	{

		if(mouseEvent.getButton()==MouseEvent::LEFT)
		{
			if(isDragingTime)
			{
				/*
				int thisPos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

				if(thisPos!=pressedTimePos && EDITCONFIG->noteSelected.size()>0)
				{
					DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
					thisModifySet->needToRecalcTime = true;

					for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
						thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],thisPos-pressedTimePos,true));

					EDITCONFIG->addAndDoOperation(thisModifySet);
				}
				*/
				//disable note area draging time function

				isDragingTime=false;
			}

			if(!isSelecting&&!isDraging)
			{
				int nowToSelect = EDITOR_PTR->mapData->findNoteToSelectByPos(ceil(CORE_PTR->getRunPosition()), nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);

				if(!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				if(nowToSelect!=-1)
					EDITCONFIG->addSelectedNote(nowToSelect);
			}

			if(isSelecting)
			{
				if(!EDITCONFIG->isctrl)
					EDITCONFIG->clearSelectedNote();

				vector<int> selected = EDITOR_PTR->mapData->findNoteToSelectByRange(ceil(CORE_PTR->getRunPosition()),selectBeginGridX-EDITCONFIG->NoteAreaTailAreaSize
					,selectBeginGridY-EDITCONFIG->NoteAreaTailAreaSize
					,nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize
					,nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize);
				for(int i=0;i<selected.size();i++)
					EDITCONFIG->addSelectedNote(selected[i]);
				isSelecting=false;
			}
			else if(isDraging)
			{
				EDITCONFIG->UnlockOperation();
				isDraging=false;
			}
		}
		else if(mouseEvent.getButton() == MouseEvent::RIGHT)
		{
			if(EDITCONFIG->noteSelected.size()>0)
			{
				DivaEditorOperationSet *thisDeleteSet = new DivaEditorOperationSet();

				for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
					thisDeleteSet->addOperation(new DivaEditorOperation_DeleteNote(EDITCONFIG->noteSelected[i]));

				EDITCONFIG->addAndDoOperation(thisDeleteSet);
			}
		}
		
	}

	void PickMeUpNoteArea::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y = mouseEvent.getY();
		updateMousePos();
	}

	void PickMeUpNoteArea::onKeyPressed(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = true;
		if(thisKey>='A'&&thisKey<='Z') 
			thisKey+='a'-'A';

		float nowTimePos = CORE_PTR->getRunPosition();

		if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			int noteSelectX = nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize;
			int noteSelectY = nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize;

			int guessTailX,guessTailY;;
			if(noteSelectX<EDITCONFIG->NoteAreaWidth/2)
			{
				guessTailX = EDITCONFIG->NoteAreaWidth/2-2;
				if(noteSelectY<EDITCONFIG->NoteAreaHeight/2)
				{
					guessTailY = EDITCONFIG->NoteAreaHeight/2-2;
					thisKey='w';
				}
				else
				{
					guessTailY = -(EDITCONFIG->NoteAreaHeight/2-2);
					thisKey='a';
				}
			}
			else
			{
				guessTailX = -(EDITCONFIG->NoteAreaWidth/2-2);
				if(noteSelectY<EDITCONFIG->NoteAreaHeight/2)
				{
					guessTailY = EDITCONFIG->NoteAreaHeight/2-2;
					thisKey='d';
				}
				else
				{
					guessTailY = -(EDITCONFIG->NoteAreaHeight/2-2);
					thisKey='s';
				}
			}

			if(noteSelectX >= 0 && noteSelectX <= EDITCONFIG->NoteAreaWidth &&
				noteSelectY >= 0 && noteSelectY <= EDITCONFIG->NoteAreaHeight && isMouseOn)
			{
				if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::NORMAL)
				{
					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(nowTimePos,EDITCONFIG->getGridToShowPerBeat());

					int toAddFind = EDITOR_PTR->mapData->findNoteIndexByType(pos, EDITOR_PTR->mapData->getNoteTypeFromKeyPress(thisKey,caps));
					if(toAddFind==-1)
					{
						DivaEditorOperation_AddNormalNote *toOp = new DivaEditorOperation_AddNormalNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY);
						if(CORE_FLOW_PTR->getState() == CoreFlow::RUN)
						{
							toOp->needToRefreshAll=false;
							toOp->needToRecalcTime=false;
							EDITCONFIG->needReCalcNextTime=true;
						}
						EDITCONFIG->addAndDoOperation(toOp);
					}
					else
					{
						EDITCONFIG->clearSelectedNote();
						EDITCONFIG->addSelectedNote(toAddFind);
					}
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && !placingLong)
				{
					int guessX,guessY,guessTailX,guessTailY;
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(nowTimePos,guessX,guessY,guessTailX,guessTailY);

					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY,"long");

					placingLong = true;
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && !placingCombo)
				{
					int guessX,guessY,guessTailX,guessTailY;
					EDITOR_PTR->mapData->guessThisNotePositionByLastTwo(nowTimePos,guessX,guessY,guessTailX,guessTailY);

					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,guessTailX,guessTailY,"pingpong");

					placingCombo = true;
				}
			}
			
		}
		else if(thisKey=='q')
		{
			moveTail=true;
		}
	}

	void PickMeUpNoteArea::onKeyReleased(sora::SoraKeyEvent& event)
	{
		int thisKey = event.chr;
		bool caps = false;
		if(thisKey>='A'&&thisKey<='Z') 
		{
			thisKey+='a'-'A';
			caps=true;
		}

		if(thisKey=='q')
		{
			changeSelectedNoteTailOver();
		}
		else if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			if(placingLong && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && isMouseOn)
			{
				placingLongNoteOver();
			}
			else if(placingCombo && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && isMouseOn)
			{
				placingComboNoteOver();
			}

			placingLong=false;
			placingCombo=false;
		}
	}

	void PickMeUpNoteArea::mouseEntered(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=true;
	}

	void PickMeUpNoteArea::mouseExited(gcn::MouseEvent& mouseEvent)
	{
		isMouseOn=false;
	}

	void PickMeUpNoteArea::mouseMoved(MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y = mouseEvent.getY();
		updateMousePos();
	}

#pragma endregion
}