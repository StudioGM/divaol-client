#include "divaeditor/DivaEditorScene/DivaEditorNoteArea.h"
#include "divaeditor/DivaEditorMapData.h"
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
		static const gcn::Color selectedColor(255,255,0,130);
		static const gcn::Color selectAreaColor(255,255,255,100);

		float width = getWidth(),height = getHeight();

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


		//Draw selected note's tail

		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			graphics->setColor(selectedColor);
			divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];

			for(int j=0;j<selectedNote.notePoint.size();j++)
			{
				graphics->fillRectangle(gcn::Rectangle(float((selectedNote.notePoint[j].x+EDITCONFIG->NoteAreaTailAreaSize-0.5)*EDITCONFIG->NoteAreaGridSize)*factor,
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
					graphics->drawLine(centerX*factor,centerY*factor,moveTailX,moveTailY);
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

					moveTailX = (nowMousePos.x - centerX)*1000;
					moveTailY = (nowMousePos.y - centerY)*1000;

					//EDITOR_PTR->mapData->note_modifyTail(EDITCONFIG->noteSelected[i],nowMousePos.x - centerX,nowMousePos.y - centerY);
				}
			}
			//if(EDITCONFIG->noteSelected.size()>0)
				//EDITUTILITY.refreshAll();
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
				int thisPos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

				if(thisPos!=pressedTimePos)
				{
					for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
						EDITOR_PTR->mapData->note_modifyTimePos(EDITCONFIG->noteSelected[i],thisPos-pressedTimePos,true);
					EDITUTILITY.reCaltTime();
				}

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
				isDraging=false;
		}
		else if(mouseEvent.getButton() == MouseEvent::RIGHT)
		{
			int thisPos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());

			for (int i=0;i<EDITCONFIG->noteSelected.size();)
				EDITOR_PTR->mapData->note_delete(EDITCONFIG->noteSelected[i]);
				
			EDITUTILITY.refreshAll();
		}
		
	}

	void NoteArea::mouseDragged(gcn::MouseEvent& mouseEvent)
	{
		nowMousePos.x = mouseEvent.getX();
		nowMousePos.y = mouseEvent.getY();
		updateMousePos();

		if(!isDraging && !isSelecting)
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


			if(EDITCONFIG->isNoteSelected(nowToSelect))
			{
				dragAreaRect = EDITOR_PTR->mapData->findSelectedAreaRectange();
				dragingPosX = (nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize) - dragAreaRect.x;
				dragingPosY = (nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize) - dragAreaRect.y;
				isDraging=true;
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
					//Operation move set of notes pos
					for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
						EDITOR_PTR->mapData->note_modifyPos(EDITCONFIG->noteSelected[i],deltaDragX,deltaDragY,true);
					dragAreaRect.x += deltaDragX;
					dragAreaRect.width += deltaDragX;
					dragAreaRect.y += deltaDragY;
					dragAreaRect.height += deltaDragY;
					EDITUTILITY.refreshAll();
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

		else if(thisKey=='	' && !placingLong && !placingCombo && !moveTail)//Tab
		{
			EDITCONFIG->ChangeEditState();
		}
		else if(thisKey=='a'||thisKey=='w'||thisKey=='s'||thisKey=='d' && !event.isAltFlag() && !event.isCtrlFlag())
		{
			int noteSelectX = nowGridSelectX-EDITCONFIG->NoteAreaTailAreaSize;
			int noteSelectY = nowGridSelectY-EDITCONFIG->NoteAreaTailAreaSize;
			if(noteSelectX > 0 && noteSelectX <= EDITCONFIG->NoteAreaWidth &&
				noteSelectY > 0 && noteSelectY <= EDITCONFIG->NoteAreaHeight)
			{
				if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::NORMAL)
				{
					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					int addedIndex = EDITOR_PTR->mapData->addNormalNote(pos,thisKey,caps,noteSelectX,noteSelectY,5,5);
					EDITUTILITY.setPosition(pos);
					EDITCONFIG->clearSelectedNote();
					EDITCONFIG->addSelectedNote(addedIndex);
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG && !placingLong)
				{
					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,5,5,"long");
					//EDITUTILITY.setPosition(pos);

					placingLong = true;
				}
				else if(EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO && !placingCombo)
				{
					int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
					placingNote = EDITOR_PTR->mapData->initNote(pos,thisKey,caps,noteSelectX,noteSelectY,5,5,"pingpong");
					//EDITUTILITY.setPosition(pos);

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
			if(placingLong && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::LONG)
			{
				placingLongNoteOver();
			}
			else if(placingCombo && EDITCONFIG->EDITSTATE_NOTESTATE == EditorConfig::NOTESTATE::COMBO)
			{
				placingComboNoteOver();
			}
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
		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			divaeditor::MapNote &selectedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];
			if(selectedNote.arg.find("tailx")!=selectedNote.arg.end() && 
				selectedNote.arg.find("taily")!=selectedNote.arg.end())
				EDITOR_PTR->mapData->note_modifyTail(EDITCONFIG->noteSelected[i],moveTailX,moveTailY);
		}
		if(EDITCONFIG->noteSelected.size()>0)
			EDITUTILITY.refreshAll();
		moveTail=false;
	}


	void NoteArea::placingLongNoteOver()
	{
		int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
		EDITOR_PTR->mapData->finishLongNote(placingNote,pos);

		int addedIndex = EDITOR_PTR->mapData->addLongNote(placingNote);

		EDITCONFIG->clearSelectedNote();
		EDITCONFIG->addSelectedNote(addedIndex);

		placingLong=false;
	}

	void NoteArea::placingComboNoteOver()
	{
		int pos = EDITOR_PTR->mapData->getNearestStandardGrid(CORE_PTR->getRunPosition(),EDITCONFIG->getGridToShowPerBeat());
		EDITOR_PTR->mapData->finishComboNote(placingNote,pos);

		int addedIndex = EDITOR_PTR->mapData->addLongNote(placingNote);

		EDITCONFIG->clearSelectedNote();
		EDITCONFIG->addSelectedNote(addedIndex);

		placingCombo=false;
	}
}