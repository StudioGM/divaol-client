
#include "divaeditor/Core/DivaEditor.h"
#include "divaeditor/Component/DivaEditorStandardOperation.h"

#include "divacore/Component/DivaStandardCoreFlow.h"



namespace divaeditor
{

#pragma region DivaEditorOperationSet

	DivaEditorOperationSet::DivaEditorOperationSet()
	{
		operation_type = "OperationSet";
	}

	DivaEditorOperationSet::~DivaEditorOperationSet()
	{
		clearOperation();
	}

	void DivaEditorOperationSet::merge(DivaEditorOperationSet* tomerge)
	{
		if(operations.size()!=tomerge->operations.size())
			return;

		for (int i=0;i<operations.size();i++)
			((DivaEditorOperation_ModifyNote*)operations[i])->merge((DivaEditorOperation_ModifyNote*)tomerge->operations[i]);
	}

	void DivaEditorOperationSet::doOperation()
	{


		int isPlaying = CORE_FLOW_PTR->getState();
		if(needToPause && isPlaying == CoreFlow::RUN)
			EDITOR_PTR->mapData->PauseAndResume();

		for (int i=0;i<operations.size();i++)
			if(operations[i]!=NULL)
				operations[i]->doOperation();

		EDITOR_PTR->mapData->sortNote();
		EDITOR_PTR->mapData->sortEvent();

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();

		if(needToPause && isPlaying == CoreFlow::RUN)
			EDITOR_PTR->mapData->PauseAndResume();
	}

	void DivaEditorOperationSet::undoOperation()
	{
		int isPlaying = CORE_FLOW_PTR->getState();
		if(needToPause && isPlaying == CoreFlow::RUN)
			EDITOR_PTR->mapData->PauseAndResume();

		for (int i=operations.size()-1;i>=0;i--)
			if(operations[i]!=NULL)
				operations[i]->undoOperation();

		EDITOR_PTR->mapData->sortNote();
		EDITOR_PTR->mapData->sortEvent();

		//if(needToRecalcTime)
		EDITUTILITY.reCaltTime();
		//else if(needToRefreshAll)
		EDITUTILITY.refreshAll();

		if(needToPause && isPlaying == CoreFlow::RUN)
			EDITOR_PTR->mapData->PauseAndResume();

	}

	void DivaEditorOperationSet::clearOperation()
	{
		while(operations.size()>0)
		{
			if(operations[0]!=NULL)
				SAFE_DELETE(operations[0]);
			operations.erase(operations.begin());
		}
	}

	void DivaEditorOperationSet::addOperation(DivaEditorOperation* newOperation)
	{
		newOperation->needToRefreshAll = false;
		newOperation->needToRecalcTime = false;
		operations.push_back(newOperation);
	}

	std::wstring DivaEditorOperationSet::ToString()
	{
		if(operations.size()==0)
			return L"ERROR(Empty operation set)";
		else
			return operations[0]->ToString() + (operations.size()>1?(L" and other " + iToWS(operations.size()-1) + L" operations."):L"");
	}

#pragma endregion DivaEditorOperationSet
#pragma region DivaEditorOperation_BPM

	DivaEditorOperation_BPM::DivaEditorOperation_BPM(double bpmOld, double bpmNew, float position, Type bpmOperation_Type)
	{
		this->bpmOld = bpmOld;
		this->bpmNew = bpmNew;
		this->position = position;
		this->bpmOperation_Type = bpmOperation_Type;

		operation_type = "BPM";
	}

	void DivaEditorOperation_BPM::doOperation()
	{
		if(bpmOperation_Type == CHANGEBPM)
			EDITOR_PTR->mapData->bpm_change(position,bpmNew);
		else if(bpmOperation_Type == INSERTBPM)
			EDITOR_PTR->mapData->bpm_insert(position,bpmNew);
		else if(bpmOperation_Type == DELETEBPM)
			EDITOR_PTR->mapData->bpm_delete(position);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	void DivaEditorOperation_BPM::undoOperation()
	{
		if(bpmOperation_Type == CHANGEBPM)
			EDITOR_PTR->mapData->bpm_change(position,bpmOld);
		else if(bpmOperation_Type == INSERTBPM)
			EDITOR_PTR->mapData->bpm_delete(position);
		else if(bpmOperation_Type == DELETEBPM)
			EDITOR_PTR->mapData->bpm_insert(position,bpmOld);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	std::wstring DivaEditorOperation_BPM::ToString()
	{
		return L"";
	}


#pragma endregion DivaEditorOperation_BPM
#pragma region DivaEditorOperation_GridOffset

	DivaEditorOperation_GridOffset::DivaEditorOperation_GridOffset(int offsetOld, int offsetNew, bool moveAll)
	{
		this->offsetOld = offsetOld;
		this->offsetNew = offsetNew;
		this->moveAll = moveAll;
	}

	std::wstring DivaEditorOperation_GridOffset::ToString()
	{
		return L"";
	}

	void DivaEditorOperation_GridOffset::doOperation()
	{
		EDITOR_PTR->mapData->offset_set(offsetNew, moveAll);
	}

	void DivaEditorOperation_GridOffset::undoOperation()
	{
		EDITOR_PTR->mapData->offset_set(offsetOld, moveAll);
	}

#pragma endregion DivaEditorOperation_GridOffset
#pragma region DivaEditorOperation_STOP

	DivaEditorOperation_STOP::DivaEditorOperation_STOP(int stopOld, int stopNew, float position, Type stopOperation_Type)
	{
		this->stopOld = stopOld;
		this->stopNew = stopNew;
		this->position = position;
		this->stopOperation_Type = stopOperation_Type;

		operation_type = "STOP";
	}

	void DivaEditorOperation_STOP::doOperation()
	{
		if(stopOperation_Type == CHANGESTOP)
			EDITOR_PTR->mapData->stop_change(position, stopNew);
		else if(stopOperation_Type == INSERTSTOP)
			EDITOR_PTR->mapData->stop_insert(position, stopNew);
		else if(stopOperation_Type == DELETESTOP)
			EDITOR_PTR->mapData->stop_delete(position);
	}

	void DivaEditorOperation_STOP::undoOperation()
	{
		if(stopOperation_Type == CHANGESTOP)
			EDITOR_PTR->mapData->stop_change(position, stopOld);
		else if(stopOperation_Type == INSERTSTOP)
			EDITOR_PTR->mapData->stop_delete(position);
		else if(stopOperation_Type == DELETESTOP)
			EDITOR_PTR->mapData->stop_insert(position, stopOld);
	}

	std::wstring DivaEditorOperation_STOP::ToString()
	{
		return L"";
	}


#pragma endregion DivaEditorOperation_STOP
#pragma region DivaEditorOperation_BeatNum

	DivaEditorOperation_BeatNum::DivaEditorOperation_BeatNum(int beatNumOld, int beatNumNew, int period, Type beatNumOperation_Type)
	{
		this->beatNumOld = beatNumOld;
		this->beatNumNew = beatNumNew;
		this->period = period;
		this->beatNumOperation_Type = beatNumOperation_Type;

		operation_type = "BeatNum";
	}

	std::wstring DivaEditorOperation_BeatNum::ToString()
	{
		return L"";
	}

	void DivaEditorOperation_BeatNum::doOperation()
	{
		if(beatNumOperation_Type==CHANGEBEATNUM || beatNumOperation_Type==INSERTBEATNUM)
			EDITOR_PTR->mapData->beatNum_change(period, beatNumNew);
		else if(beatNumOperation_Type==DELETEBEATNUM)
			EDITOR_PTR->mapData->beatNum_delete(period);
	}

	void DivaEditorOperation_BeatNum::undoOperation()
	{
		if(beatNumOperation_Type==CHANGEBEATNUM || beatNumOperation_Type==DELETEBEATNUM)
			EDITOR_PTR->mapData->beatNum_change(period, beatNumOld);
		else if(beatNumOperation_Type==INSERTBEATNUM)
			EDITOR_PTR->mapData->beatNum_delete(period);
	}

#pragma endregion DivaEditorOperation_BeatNum
#pragma region DivaEditorOperation_TailSpeed

	DivaEditorOperation_TailSpeed::DivaEditorOperation_TailSpeed(float tailSpeedOld, float tailSpeedNew)
	{
		this->tailSpeedOld = tailSpeedOld;
		this->tailSpeedNew = tailSpeedNew;
	}

	std::wstring DivaEditorOperation_TailSpeed::ToString()
	{
		return L"";
	}

	void DivaEditorOperation_TailSpeed::doOperation()
	{
		EDITOR_PTR->mapData->tailSpeed_change(tailSpeedNew);
		EDITUTILITY.refreshAll();
	}

	void DivaEditorOperation_TailSpeed::undoOperation()
	{
		EDITOR_PTR->mapData->tailSpeed_change(tailSpeedOld);
		EDITUTILITY.refreshAll();
	}

#pragma endregion DivaEditorOperation_TailSpeed

#pragma region DivaEditorOperation_ModifyEvent

	DivaEditorOperation_ModifyEvent::DivaEditorOperation_ModifyEvent(int eventIndex, int bpmPos, int nextbpmPos, double oldBPM, double newBPM)
	{
		calculated=true;
		eventModifyType=BPMCHANGED;

		double bpmFactor = newBPM/oldBPM;

		oldEvent = EDITOR_PTR->mapData->coreInfoPtr->events[eventIndex];
		newEvent = oldEvent;

		if(newEvent.position>bpmPos && newEvent.position <nextbpmPos)
			newEvent.position = ((double)(newEvent.position-bpmPos)) * bpmFactor + bpmPos;
		else if(newEvent.position>=nextbpmPos)
			newEvent.position = ((double)(nextbpmPos-bpmPos)) * bpmFactor + bpmPos + (newEvent.position-nextbpmPos);
	}

	void DivaEditorOperation_ModifyEvent::doOperation()
	{
		int thisEventIndex = EDITOR_PTR->mapData->findEvent(oldEvent);
		if(calculated && oldEvent.position!=newEvent.position)
		{
			EDITOR_PTR->mapData->event_modifyTimePos(thisEventIndex, newEvent.position);
			thisEventIndex = EDITOR_PTR->mapData->adjustEventOrder(thisEventIndex);
		}
		
		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}
	void DivaEditorOperation_ModifyEvent::undoOperation()
	{
		int thisEventIndex = EDITOR_PTR->mapData->findEvent(newEvent);
		if(newEvent.position!=oldEvent.position)
		{
			EDITOR_PTR->mapData->event_modifyTimePos(thisEventIndex, oldEvent.position);
			thisEventIndex = EDITOR_PTR->mapData->adjustEventOrder(thisEventIndex);
		}

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	std::wstring DivaEditorOperation_ModifyEvent::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_ModifyEvent


#pragma region DivaEditorOperation_AddNormalNote

	DivaEditorOperation_AddNormalNote::DivaEditorOperation_AddNormalNote(divacore::MapNote& addedNote)
	{
		this->addedNote = addedNote;
	}

	DivaEditorOperation_AddNormalNote::DivaEditorOperation_AddNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key/* =-1 */)
	{
		addedNote = EDITOR_PTR->mapData->initNote(pos,keyPress,arrow,x,y,tailX,tailY,"normal",key);
	}

	void DivaEditorOperation_AddNormalNote::doOperation()
	{
		addedIndex = EDITOR_PTR->mapData->addNormalNote(addedNote);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();

		EDITCONFIG->clearSelectedNote();
		EDITCONFIG->addSelectedNote(addedIndex);
	}

	void DivaEditorOperation_AddNormalNote::undoOperation()
	{
		EDITOR_PTR->mapData->note_delete(addedIndex);
		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	std::wstring DivaEditorOperation_AddNormalNote::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_AddNormalNote
#pragma region DivaEditorOperation_AddLongNote

	DivaEditorOperation_AddLongNote::DivaEditorOperation_AddLongNote(divacore::MapNote& addedNote)
	{
		this->addedNote = addedNote;
	}

	void DivaEditorOperation_AddLongNote::doOperation()
	{
		addedIndex = EDITOR_PTR->mapData->addNormalNote(addedNote);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();

		EDITCONFIG->clearSelectedNote();
		EDITCONFIG->addSelectedNote(addedIndex);
	}

	void DivaEditorOperation_AddLongNote::undoOperation()
	{
		EDITOR_PTR->mapData->note_delete(addedIndex);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	std::wstring DivaEditorOperation_AddLongNote::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_AddLongNote
#pragma region DivaEditorOperation_DeleteNote

	DivaEditorOperation_DeleteNote::DivaEditorOperation_DeleteNote(int deletedIndex)
	{
		this->deletedNote = EDITOR_PTR->mapData->coreInfoPtr->notes[deletedIndex];
	}

	void DivaEditorOperation_DeleteNote::doOperation()
	{
		EDITOR_PTR->mapData->note_delete(EDITOR_PTR->mapData->findNoteIndexByType(deletedNote.notePoint[0].position,deletedNote.notePoint[0].type,0,deletedNote.noteType));

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	void DivaEditorOperation_DeleteNote::undoOperation()
	{
		int deletedIndex=-1;
		if(deletedNote.notePoint.size()==1)
			deletedIndex = EDITOR_PTR->mapData->addNormalNote(deletedNote);
		else
			deletedIndex = EDITOR_PTR->mapData->addLongNote(deletedNote);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();

		EDITCONFIG->clearSelectedNote();
		EDITCONFIG->addSelectedNote(deletedIndex);
	}

	std::wstring DivaEditorOperation_DeleteNote::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_DeleteNote
#pragma region DivaEditorOperation_ModifyNote


	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, std::string key) //note_modifyKey
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->key = key;

		noteModifyType = KEY;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int tailX, int tailY) //note_modifyTail
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->tailX = tailX;
		this->tailY = tailY;

		noteModifyType = TAIL;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int x, int y, bool isDelta) //note_modifyPos
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->x = x;
		this->y = y;
		this->isDelta = isDelta;

		noteModifyType = POS;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int posLeft, int posRight, Type noteModifyType)
	{
		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		newNote = oldNote;

		if(posLeft>posRight)
			std::swap(posLeft,posRight);

		newNote.notePoint[0].position = posLeft;
		newNote.notePoint[1].position = posRight;

		calculated=true;
		noteModifyType = LONGNOTETIMEPOS;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int pos, bool isDelta) //note_modifyTimePos
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->pos = pos;
		this->isDelta = isDelta;

		noteModifyType = TIMEPOS;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int type, bool isDelta, bool needDecode) //note_modifyTypeByType
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->type = type;
		this->isDelta = isDelta;
		this->needDecode = needDecode;

		noteModifyType = TYPEBYTYPE;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, char keyPress, bool arrow) //note_modifyType
	{
		calculated=false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		this->keyPress=keyPress;
		this->arrow = arrow;

		noteModifyType = TYPE;
	}

	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, int bpmPos,int nextbpmPos, double oldBPM,	double newBPMValue) //note_bpmChanged
	{
		double bpmFactor = newBPMValue/oldBPM;

		calculated = true;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];

		noteModifyType = BPMCHANGED;

		newNote = oldNote;
		for (int i=0;i<newNote.notePoint.size();i++)
			if(newNote.notePoint[i].position>=bpmPos)
			{
				if(newNote.notePoint[i].position>bpmPos && newNote.notePoint[i].position <nextbpmPos)
					newNote.notePoint[i].position = ((double)(newNote.notePoint[i].position-bpmPos)) * bpmFactor + bpmPos;
				else if(newNote.notePoint[i].position>=nextbpmPos)
					newNote.notePoint[i].position = ((double)(nextbpmPos-bpmPos)) * bpmFactor + bpmPos + (newNote.notePoint[i].position-nextbpmPos);
			}
	}



	DivaEditorOperation_ModifyNote::DivaEditorOperation_ModifyNote(int index, Type noteModifyType)
	{
		calculated = false;

		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];

		this->noteModifyType = noteModifyType;
	}


	void DivaEditorOperation_ModifyNote::doOperation()
	{
		int nowNoteIndex = EDITOR_PTR->mapData->findNoteIndexByType(oldNote.notePoint[0].position,oldNote.notePoint[0].type,0,oldNote.noteType);

		if(calculated && oldNote.notePoint[0].key != newNote.notePoint[0].key)
			EDITOR_PTR->mapData->note_modifyKey(nowNoteIndex,newNote.notePoint[0].key);
		else if(noteModifyType == KEY)
			EDITOR_PTR->mapData->note_modifyKey(nowNoteIndex,key);


		if(calculated && (Argument::asInt("tailx",oldNote.arg) != Argument::asInt("tailx",newNote.arg) || 
							Argument::asInt("taily",oldNote.arg) != Argument::asInt("taily",newNote.arg)))
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex,Argument::asInt("tailx",newNote.arg),Argument::asInt("taily",newNote.arg));
		else if(noteModifyType == TAIL)
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex,tailX,tailY);
		else if(noteModifyType == FLIPHORIZONTAL)
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex, -Argument::asInt("tailx",oldNote.arg), Argument::asInt("taily",oldNote.arg));
		else if(noteModifyType == FLIPVERTICAL)
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex, Argument::asInt("tailx",oldNote.arg), -Argument::asInt("taily",oldNote.arg));
		else if(noteModifyType == SORTFORPICKMEUP)
		{
			int x,y;
			switch(oldNote.notePoint[0].type%4)
			{
			case 3:
				x=EDITCONFIG->NoteAreaWidth/2-2;y=EDITCONFIG->NoteAreaHeight/2-2;break;
			case 1:
				x=EDITCONFIG->NoteAreaWidth/2-2;y=-(EDITCONFIG->NoteAreaHeight/2-2);break;
			case 0:
				x=-(EDITCONFIG->NoteAreaWidth/2-2),y=EDITCONFIG->NoteAreaHeight/2-2;break;
			case 2:
				x=-(EDITCONFIG->NoteAreaWidth/2-2),y=-(EDITCONFIG->NoteAreaHeight/2-2);break;
			}
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex, x, y);
		}


		if(calculated && (oldNote.notePoint[0].x != newNote.notePoint[0].x || oldNote.notePoint[0].y != newNote.notePoint[0].y))
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex,newNote.notePoint[0].x,newNote.notePoint[0].y,false);
		else if(noteModifyType == POS)
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex,x,y,isDelta);
		else if(noteModifyType == FLIPHORIZONTAL)
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex, EDITCONFIG->NoteAreaWidth-oldNote.notePoint[0].x, oldNote.notePoint[0].y, false);
		else if(noteModifyType == FLIPVERTICAL)
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex, oldNote.notePoint[0].x,EDITCONFIG->NoteAreaHeight-oldNote.notePoint[0].y, false);
		else if(noteModifyType == SORTFORPICKMEUP)
		{
			int x,y;
			switch(oldNote.notePoint[0].type%4)
			{
			case 3:
				x=2,y=2;break;
			case 1:
				x=2,y=EDITCONFIG->NoteAreaHeight-2;break;
			case 0:
				x=EDITCONFIG->NoteAreaWidth-2;y=2;break;
			case 2:
				x=EDITCONFIG->NoteAreaWidth-2;y=EDITCONFIG->NoteAreaHeight-2;break;
			}
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex,x,y,false);
		}


		if(calculated && oldNote.notePoint[0].position != newNote.notePoint[0].position)
		{
			EDITOR_PTR->mapData->note_modifyTimePos(nowNoteIndex,newNote.notePoint[0].position,false);
			nowNoteIndex = EDITOR_PTR->mapData->adjustNoteOrder(nowNoteIndex);
		}
		else if(noteModifyType == TIMEPOS)
		{
			EDITOR_PTR->mapData->note_modifyTimePos(nowNoteIndex,pos,isDelta);
			nowNoteIndex = EDITOR_PTR->mapData->adjustNoteOrder(nowNoteIndex);
		}

		if(calculated && oldNote.notePoint.size()>1 && oldNote.notePoint[1].position != newNote.notePoint[1].position)
		{
			EDITOR_PTR->mapData->note_modifySecondTimePos(nowNoteIndex,newNote.notePoint[1].position,false);
		}


		if(calculated && oldNote.notePoint[0].type != newNote.notePoint[0].type)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,newNote.notePoint[0].type,false);
		else if(noteModifyType == TYPEBYTYPE)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,type,isDelta,needDecode);
		else if(noteModifyType == TYPE)
			EDITOR_PTR->mapData->note_modifyType(nowNoteIndex,keyPress,arrow);
		else if(noteModifyType == TOARROW && oldNote.notePoint[0].type<4)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,4,true);
		else if(noteModifyType == TOLETTER && oldNote.notePoint[0].type>=4)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,-4,true);
		else if(noteModifyType == TYPESYMMETRY)
		{
			int toAdd = ((oldNote.notePoint[0].type/4)+1)%2;
			int tType = EDITOR_PTR->mapData->getNoteTypeIndexFromNoteType(oldNote.notePoint[0].type%4);
			if(tType==1) tType=3;
			else if(tType==3) tType=1;
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,tType + toAdd*4,false,true);
		}
		else if(noteModifyType == SIMPLE2KEY)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,oldNote.notePoint[0].type%2 + (oldNote.notePoint[0].type / 4)*4,false);
		else if(noteModifyType == SIMPLE1KEY)
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,(oldNote.notePoint[0].type / 4)*4,false);
		

		if (!calculated)
		{
			newNote = EDITOR_PTR->mapData->coreInfoPtr->notes[nowNoteIndex];
			calculated=true;
		}

		
		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}


	void DivaEditorOperation_ModifyNote::undoOperation()
	{

		int nowNoteIndex = EDITOR_PTR->mapData->findNoteIndexByType(newNote.notePoint[0].position,newNote.notePoint[0].type,0,newNote.noteType);

		if(newNote.notePoint[0].key != oldNote.notePoint[0].key)
		{
			EDITOR_PTR->mapData->note_modifyKey(nowNoteIndex,oldNote.notePoint[0].key);
		}

		if((Argument::asInt("tailx",oldNote.arg) != Argument::asInt("tailx",newNote.arg) || 
			Argument::asInt("taily",oldNote.arg) != Argument::asInt("taily",newNote.arg)))
		{
			EDITOR_PTR->mapData->note_modifyTail(nowNoteIndex,Argument::asInt("tailx",oldNote.arg),Argument::asInt("taily",oldNote.arg));
		} 

		if((oldNote.notePoint[0].x != newNote.notePoint[0].x || oldNote.notePoint[0].y != newNote.notePoint[0].y))
		{
			EDITOR_PTR->mapData->note_modifyPos(nowNoteIndex,oldNote.notePoint[0].x, oldNote.notePoint[0].y,false);
		}

		if(oldNote.notePoint[0].position != newNote.notePoint[0].position)
		{
			EDITOR_PTR->mapData->note_modifyTimePos(nowNoteIndex,oldNote.notePoint[0].position,false);
			nowNoteIndex = EDITOR_PTR->mapData->adjustNoteOrder(nowNoteIndex);
		}

		if(oldNote.notePoint.size()>1 && oldNote.notePoint[1].position != newNote.notePoint[1].position)
		{
			EDITOR_PTR->mapData->note_modifySecondTimePos(nowNoteIndex, oldNote.notePoint[1].position, false);
		}

		if(oldNote.notePoint[0].type != newNote.notePoint[0].type)
		{
			EDITOR_PTR->mapData->note_modifyTypeByType(nowNoteIndex,oldNote.notePoint[0].type,false);
		}

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	void DivaEditorOperation_ModifyNote::merge(DivaEditorOperation_ModifyNote* tomerge)
	{

		newNote = tomerge->newNote;
		calculated=true;
		tomerge->calculated=true;
	}

	std::wstring DivaEditorOperation_ModifyNote::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_ModifyNote
#pragma region DivaEditorOperation_SplitLongNote

	DivaEditorOperation_SplitLongNote::DivaEditorOperation_SplitLongNote(int index)
	{
		oldNote = EDITOR_PTR->mapData->coreInfoPtr->notes[index];
		newNote1 = oldNote;
		newNote2 = oldNote;

		newNote1.notePoint.erase(newNote1.notePoint.begin()+1);
		newNote1.noteType = "normal";
		newNote2.notePoint.erase(newNote2.notePoint.begin());
		newNote2.noteType = "normal";
	}

	void DivaEditorOperation_SplitLongNote::doOperation()
	{
		int nowNoteIndex = EDITOR_PTR->mapData->findNoteIndexByType(oldNote.notePoint[0].position,oldNote.notePoint[0].type,0,oldNote.noteType);
		EDITOR_PTR->mapData->note_delete(nowNoteIndex);

		int newNoteIndex = EDITOR_PTR->mapData->addNormalNote(newNote1);
		if(!EDITCONFIG->isNoteSelected(newNoteIndex))
			EDITCONFIG->addSelectedNote(newNoteIndex);
		newNoteIndex = EDITOR_PTR->mapData->addNormalNote(newNote2);
		if(!EDITCONFIG->isNoteSelected(newNoteIndex))
			EDITCONFIG->addSelectedNote(newNoteIndex);

		

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	void DivaEditorOperation_SplitLongNote::undoOperation()
	{
		int newNoteIndex = EDITOR_PTR->mapData->findNoteIndexByType(newNote1.notePoint[0].position,newNote1.notePoint[0].type,0,newNote1.noteType);
		EDITOR_PTR->mapData->note_delete(newNoteIndex);
		newNoteIndex = EDITOR_PTR->mapData->findNoteIndexByType(newNote2.notePoint[0].position,newNote2.notePoint[0].type,0,newNote1.noteType);
		EDITOR_PTR->mapData->note_delete(newNoteIndex);

		int nowNoteIndex = EDITOR_PTR->mapData->addLongNote(oldNote);
		if(!EDITCONFIG->isNoteSelected(nowNoteIndex))
			EDITCONFIG->addSelectedNote(nowNoteIndex);

		if(needToRecalcTime)
			EDITUTILITY.reCaltTime();
		else if(needToRefreshAll)
			EDITUTILITY.refreshAll();
	}

	std::wstring DivaEditorOperation_SplitLongNote::ToString()
	{
		return L"";
	}

#pragma endregion DivaEditorOperation_SplitLongNote

}