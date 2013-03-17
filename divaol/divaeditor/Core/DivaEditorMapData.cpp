#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include "stdio.h"

#include "divaeditor/Core/DivaEditorMapData.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Component/DivaStandardCoreFlow.h"
#include "divacore/Mode/DivaEditMode.h"

namespace divaeditor
{
	//using namespace divacore;
#define EditUtility divacore::StandardEditUtility::instance()

	DivaEditorMapData::DivaEditorMapData()
	{
	}


	void DivaEditorMapData::copy(bool cut)
	{
		if(EDITCONFIG->noteSelected.size()==0) return;

		copyBoard.clear();
		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
			copyBoard.push_back(coreInfoPtr->notes[EDITCONFIG->noteSelected[i]]);
		std::sort(copyBoard.begin(),copyBoard.end(),cmp_note);

		if(cut)
		{
			DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
			for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
				thisModifySet->addOperation(new DivaEditorOperation_DeleteNote(EDITCONFIG->noteSelected[i]));
			EDITCONFIG->addAndDoOperation(thisModifySet);
		}
	}

	void DivaEditorMapData::paste(float pos)
	{
		if(copyBoard.size()==0) return;

		int toPastePosBegin = getNearestStandardGrid(pos,EDITCONFIG->getGridToShowPerBeat());
		
		int deltaPos = toPastePosBegin - copyBoard[0].notePoint[0].position;
		
		DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
		for(int i=0;i<copyBoard.size();i++)
		{
			divacore::MapNote noteToPaste = copyBoard[i];
			bool isInRange = true;
			for(int notePointI = 0;notePointI<noteToPaste.notePoint.size();notePointI++)
			{
				noteToPaste.notePoint[notePointI].position += deltaPos;
				if(noteToPaste.notePoint[notePointI].position<0||noteToPaste.notePoint[notePointI].position>(int)CORE_FLOW_PTR->getTotalPosition())
					isInRange=false;
			}
			if(isInRange && findNoteIndexByType(noteToPaste.notePoint[0].position,noteToPaste.notePoint[0].type)==-1)
			{
				if(noteToPaste.notePoint.size()==1)
					thisModifySet->addOperation(new DivaEditorOperation_AddNormalNote(noteToPaste));
				else
					thisModifySet->addOperation(new DivaEditorOperation_AddLongNote(noteToPaste));
			}
		}
		if(thisModifySet->operations.size()>0)
			EDITCONFIG->addAndDoOperation(thisModifySet);
	}

	void DivaEditorMapData::setPos(float pos)
	{
		if(EDITCONFIG->needReCalcNextTime)
		{
			EDITUTILITY.reCaltTime();
			EDITCONFIG->needReCalcNextTime=false;
		}
		EditUtility.setPosition(pos);
	}

	void DivaEditorMapData::PauseAndResume()
	{
		if(EDITCONFIG->needReCalcNextTime)
		{
			EDITUTILITY.reCaltTime();
			EDITCONFIG->needReCalcNextTime=false;
		}
		if(CORE_FLOW_PTR->getState() == CoreFlow::RUN)
			CORE_PTR->pause();
		else if(CORE_FLOW_PTR->getState() == CoreFlow::PAUSE)
			CORE_PTR->resume();
	}

	void DivaEditorMapData::stop()
	{
		CORE_PTR->pause();
		if(EDITCONFIG->needReCalcNextTime)
		{
			EDITUTILITY.reCaltTime();
			EDITCONFIG->needReCalcNextTime = false;
		}
		EditUtility.setPosition(0);
	}



	void DivaEditorMapData::ResetEditorMapData()
	{
		stopLength.clear();
		beatNumChanged.clear();
		resourceDescription.clear();
		selected.clear();
		copyBoard.clear();

		beatNumChanged[0]=4;
		mapOffset=0;
		
	}

	void DivaEditorMapData::registerMapInfo(divacore::MapInfoPtr registMapInfo)
	{
		coreInfoPtr = registMapInfo;
	}

	//private search functions
	int DivaEditorMapData::findLastOrEqualEventIndex(int pos, std::string type="")
	{
		int nowIndex=-1;
		for(int i=0;i<coreInfoPtr->events.size();i++)
			if((type==""?true:(coreInfoPtr->events[i].eventType==type)) && pos>=coreInfoPtr->events[i].position)
				nowIndex = i;
			else if(pos<coreInfoPtr->events[i].position)
				break;
		return nowIndex;
	}
	int DivaEditorMapData::findNextEventIndex(int pos, std::string type="")
	{
		int nowIndex=-1;
		for(int i=coreInfoPtr->events.size()-1;i>=0;i--)
			if((type==""?true:(coreInfoPtr->events[i].eventType==type)) && pos<coreInfoPtr->events[i].position)
				nowIndex = i;
			else if(pos>=coreInfoPtr->events[i].position)
				break;
		return nowIndex;
	}
	int DivaEditorMapData::findStopIndex(float pos)
	{
		int decodedPos = decodeOriginalGrid(pos);

		if(stopLength.find(decodedPos)!=stopLength.end())
			return decodedPos;
		
		return -1;
	}

	//Sort functions
	void DivaEditorMapData::qsort_Note(int l,int r)
	{
		int s=l,t=r;
		int pos=coreInfoPtr->notes[(l+r)/2].notePoint[0].position;
		int type=coreInfoPtr->notes[(l+r)/2].notePoint[0].type;
		while(l<=r)
		{
			while(l<=r&&(coreInfoPtr->notes[l].notePoint[0].position<pos ||
				         (coreInfoPtr->notes[l].notePoint[0].position==pos && coreInfoPtr->notes[l].notePoint[0].type<type))) 
						 l++;
			while(l<=r&&(coreInfoPtr->notes[r].notePoint[0].position>pos ||
						 (coreInfoPtr->notes[r].notePoint[0].position==pos && coreInfoPtr->notes[r].notePoint[0].type>type))) 
						 r--;
			if(l<=r)
			{
				std::swap(coreInfoPtr->notes[l],coreInfoPtr->notes[r]);
				EDITCONFIG->noteIndexSwaped(l,r);
				l++;r--;
			}
		}
		if(s<r)qsort_Note(s,r);
		if(l<t)qsort_Note(l,t);
	}

	bool cmp_note(const divacore::MapNote &left, const divacore::MapNote &right)
	{
		return (left.notePoint[0].position==right.notePoint[0].position)?
					(left.notePoint[0].type<right.notePoint[0].type) :
					(left.notePoint[0].position<right.notePoint[0].position);
	}
	bool cmp_event(const divacore::MapEvent &left, const divacore::MapEvent &right)
	{
		return (left.position<right.position);
	}

	void DivaEditorMapData::sortNote()
	{
		if(coreInfoPtr->notes.size()>0)
			qsort_Note(0,coreInfoPtr->notes.size()-1);
		//std::sort(coreInfoPtr->notes.begin(),coreInfoPtr->notes.end(),cmp_note);
	}
	void DivaEditorMapData::sortEvent()
	{
		std::sort(coreInfoPtr->events.begin(),coreInfoPtr->events.end(),cmp_event);
	}
	int DivaEditorMapData::adjustNoteOrder(int index)
	{
		vector<divacore::MapNote>::iterator i = coreInfoPtr->notes.begin()+index;
		while(true)
		{
			bool operated=false;
			if(index<coreInfoPtr->notes.size()-1)
			{
				divacore::MapNote &nowNote = coreInfoPtr->notes[index],
								&nextNote = coreInfoPtr->notes[index+1];

				if((nowNote.notePoint[0].position > nextNote.notePoint[0].position) || 
					(nowNote.notePoint[0].position == nextNote.notePoint[0].position && 
					nowNote.notePoint[0].type>nextNote.notePoint[0].type))
				{
					std::swap(nowNote,nextNote);
					EDITCONFIG->noteIndexSwaped(index,index+1);
					index++;
					operated=true;
					if(index>=coreInfoPtr->notes.size()-1)
						break;
				}
			}
			
			if(index>0)
			{
				divacore::MapNote &nowNote = coreInfoPtr->notes[index],
					&pervNote = coreInfoPtr->notes[index-1];

				if((nowNote.notePoint[0].position < pervNote.notePoint[0].position) || 
					(nowNote.notePoint[0].position == pervNote.notePoint[0].position && 
					nowNote.notePoint[0].type<pervNote.notePoint[0].type))
				{
					std::swap(nowNote,pervNote);
					EDITCONFIG->noteIndexSwaped(index,index-1);
					index--;
					operated=true;
					if(index<=0)
						break;
				}
			}
			if(!operated)
				break;
		}
		return index;
	}
	int DivaEditorMapData::adjustEventOrder(int index)
	{
		vector<divacore::MapEvent>::iterator i = coreInfoPtr->events.begin()+index;
		while(true)
		{
			bool operated=false;
			if(index<coreInfoPtr->events.size()-1)
			{
				divacore::MapEvent &nowEvent = coreInfoPtr->events[index],
					&nextEvent = coreInfoPtr->events[index+1];

				if((nowEvent.position > nextEvent.position) || 
					(nowEvent.position == nextEvent.position && 
					nowEvent.eventType>nextEvent.eventType))
				{
					std::swap(nowEvent,nextEvent);
					index++;
					operated=true;
					if(index>=coreInfoPtr->events.size()-1)
						break;
				}
			}

			if(index>0)
			{
				divacore::MapEvent &nowEvent = coreInfoPtr->events[index],
					&pervEvent = coreInfoPtr->events[index-1];

				if((nowEvent.position < pervEvent.position) || 
					(nowEvent.position == pervEvent.position && 
					nowEvent.eventType<pervEvent.eventType))
				{
					std::swap(nowEvent,pervEvent);
					index--;
					operated=true;
					if(index<=0)
						break;
				}
			}
			if(!operated)
				break;
		}
		return index;

	}


	int DivaEditorMapData::checkNoteExists(int pos, int pressType, std::string noteType)
	{
		for (int i=0;i<coreInfoPtr->notes.size();i++)
			if(coreInfoPtr->notes[i].notePoint[0].position>pos)
				return -1;
			else if(coreInfoPtr->notes[i].notePoint[0].position==pos && coreInfoPtr->notes[i].notePoint[0].type == pressType && coreInfoPtr->notes[i].noteType == noteType)
				return i;
		return -1;
	}

	/////////Note Operations
	int DivaEditorMapData::getNoteTypeFromKeyPress(char keyPress, bool arrow)
	{
		int type = -1;

		if(keyPress=='d')
			type = 0;
		else if(keyPress=='a')
			type = 1;
		else if(keyPress=='s')
			type = 2;
		else if(keyPress=='w')
			type = 3;
		if(arrow)
			type += 4;

		return type;
	}
	int DivaEditorMapData::getNoteTypeIndexFromNoteType(int noteType)
	{
		noteType%=8;

		int toAdd = noteType/4;
		noteType%=4;

		if(noteType==0) //d, return 3
			return 3+toAdd*4;
		else if(noteType==1) //a, return 1
			return 1+toAdd*4;
		else if(noteType==2) //s, return 2
			return 2+toAdd*4;
		else if(noteType==3) //w, return 0
			return toAdd*4;
	}
	int DivaEditorMapData::getNoteTypeFromNoteTypeIndex(int noteTypeIndex)
	{
		noteTypeIndex%=8;

		int toAdd = noteTypeIndex/4;
		noteTypeIndex%=4;

		if(noteTypeIndex==0) //w, return 3
			return 3+toAdd*4;
		else if(noteTypeIndex==1) //a, return 1
			return 1+toAdd*4;
		else if(noteTypeIndex==2) //s, return 2
			return 2+toAdd*4;
		else if(noteTypeIndex==3) //d, return 0
			return toAdd*4;
	}


	divacore::MapNote DivaEditorMapData::initNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, std::string noteType, int key)
	{
		divacore::MapNote mapNote;
		divacore::NotePoint notePoint;
		if(key!=-1)
			notePoint.key = "res_"+iToS(key);
		notePoint.position = pos;
		notePoint.x = x;
		notePoint.y = y;
		notePoint.type = getNoteTypeFromKeyPress(keyPress, arrow);
		

		mapNote.notePoint.push_back(notePoint);

		mapNote.noteType = noteType;
		mapNote.arg["tailx"]=tailX;
		mapNote.arg["taily"]=tailY;

		return mapNote;
	}
	void DivaEditorMapData::finishLongNote(divacore::MapNote &longNote, int pos, int key)
	{
		divacore::NotePoint notePoint;
		if(key!=-1)
			notePoint.key = "res_"+iToS(key);
		notePoint.position = pos;
		notePoint.x = longNote.notePoint[0].x;
		notePoint.y = longNote.notePoint[0].y;
		notePoint.type = longNote.notePoint[0].type;
		longNote.notePoint.push_back(notePoint);

		if(longNote.notePoint[0].position>longNote.notePoint[1].position)
			std::swap(longNote.notePoint[0],longNote.notePoint[1]);
		else if(longNote.notePoint[0].position==longNote.notePoint[1].position)
			longNote.notePoint[1].position = longNote.notePoint[0].position + EDITCONFIG->GridPerBeat/EDITCONFIG->getGridToShowPerBeat();
	}
	void DivaEditorMapData::finishComboNote(divacore::MapNote &comboNote, int pos, int key)
	{
		finishLongNote(comboNote,pos,key);
		comboNote.arg["standard_num"] = int((comboNote.notePoint[1].position-comboNote.notePoint[0].position)/(EDITCONFIG->GridPerBeat/2));
	}


	int DivaEditorMapData::addNormalNote(divacore::MapNote mapNote)
	{
		int checkExist = checkNoteExists(mapNote.notePoint[0].position, mapNote.notePoint[0].type, mapNote.noteType);
		if(checkExist==-1)
		{
			if(EDITCONFIG->map_initialized)
				EDITUTILITY.resetNote(mapNote);

			coreInfoPtr->notes.push_back(mapNote);
			int insertIndex = adjustNoteOrder(coreInfoPtr->notes.size()-1);

			return insertIndex;
		}
		else
			return checkExist;
	}

	int DivaEditorMapData::addNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key)
	{
		return addNormalNote(initNote(pos,keyPress,arrow,x,y,tailX,tailY,"normal",key));	
	}
	int DivaEditorMapData::addLongNote(divacore::MapNote longNote)
	{
		int checkExist = checkNoteExists(longNote.notePoint[0].position, longNote.notePoint[0].type, longNote.noteType);
		if(checkExist==-1)
		{
			if(EDITCONFIG->map_initialized)
				EDITUTILITY.resetNote(longNote);

			coreInfoPtr->notes.push_back(longNote);
			int insertIndex = adjustNoteOrder(coreInfoPtr->notes.size()-1);

			return insertIndex;
		}
		else
			return checkExist;
	}

	void DivaEditorMapData::note_modifyTail(int index,int tailX,int tailY)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		
		coreInfoPtr->notes[index].arg["tailx"]=tailX;
		coreInfoPtr->notes[index].arg["taily"]=tailY;
	}
	void DivaEditorMapData::note_modifyPos(int index, int x, int y, bool isDelta)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			if(isDelta)
			{
				coreInfoPtr->notes[index].notePoint[i].x += x;
				coreInfoPtr->notes[index].notePoint[i].y += y;
			}
			else
			{
				coreInfoPtr->notes[index].notePoint[i].x = x;
				coreInfoPtr->notes[index].notePoint[i].y = y;
			}
		}
	}
	void DivaEditorMapData::note_modifyTimePos(int index, int pos, bool isDelta)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		if(!isDelta)
			pos = pos - coreInfoPtr->notes[index].notePoint[0].position;
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
			coreInfoPtr->notes[index].notePoint[i].position += pos;
	}
	void DivaEditorMapData::note_modifySecondTimePos(int index, int pos, bool isDelta)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		if(coreInfoPtr->notes[index].notePoint.size()<2) return;
		if(!isDelta)
			pos = pos - coreInfoPtr->notes[index].notePoint[1].position;
		coreInfoPtr->notes[index].notePoint[1].position += pos;
	}

	void DivaEditorMapData::note_modifyType(int index, char keyPress, bool arrow)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			coreInfoPtr->notes[index].notePoint[i].type = getNoteTypeFromKeyPress(keyPress,arrow);
		}
	}
	void DivaEditorMapData::note_modifyTypeByType(int index, int type, bool delta, bool needDecode)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			int realType = coreInfoPtr->notes[index].notePoint[i].type;
			if(needDecode)
				realType = getNoteTypeIndexFromNoteType(realType);

			if(delta)
				realType += type;
			else
				realType = type;

			if(realType<0)
				realType = realType%8+8;
			else
				coreInfoPtr->notes[index].notePoint[i].type=realType%8;

			if(needDecode)
				realType = getNoteTypeFromNoteTypeIndex(realType);

			coreInfoPtr->notes[index].notePoint[i].type=realType;
		}
	}
	void DivaEditorMapData::note_modifyKey(int index, std::string key)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			coreInfoPtr->notes[index].notePoint[i].key = key;
		}
	}
	void DivaEditorMapData::note_delete(int index)
	{
		if(index==-1)
		{
			divacore::Logger::instance()->log("Editor: An errror occured when modify notes.");
			return;
		}
		coreInfoPtr->notes.erase(coreInfoPtr->notes.begin()+index);
		EDITCONFIG->deleteSelectedNote(index);
	}

	int DivaEditorMapData::findNoteToSelectByPos(int position,int x,int y)
	{
		if(!(x<0||x>EDITCONFIG->NoteAreaWidth||y<0||y>EDITCONFIG->NoteAreaHeight))
			for (int i=0;i<coreInfoPtr->notes.size();i++)
			{
				divacore::MapNote &nowNote = coreInfoPtr->notes[i];
			
				if(position>int(nowNote.notePoint[0].position)-int(nowNote.aheadBar)*EDITCONFIG->GridPerBeat*4 && position<=nowNote.notePoint[0].position
					&& nowNote.notePoint[0].x == x && nowNote.notePoint[0].y == y)
					return i;
				else
				{
					if(nowNote.notePoint.size()>1)
						if(position >= nowNote.notePoint[0].position && position < nowNote.notePoint[1].position
							&& nowNote.notePoint[0].x == x && nowNote.notePoint[0].y == y)
							return i;
				}
			}
		return -1;
	}
	int DivaEditorMapData::findNoteIndexByType(int position, int type, int singleDeltaNum, std::string specificType)
	{
		if(type<0)
			return -1;

		type%=8;

		for(int i=0;i<coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &nowNote = coreInfoPtr->notes[i];

			if(nowNote.notePoint[0].position>position+singleDeltaNum)
				break;


			if(nowNote.notePoint.size()==1) //check exactly
			{
				if(nowNote.notePoint[0].type%8==type && position>=nowNote.notePoint[0].position-singleDeltaNum && position<=nowNote.notePoint[0].position+singleDeltaNum
					&& (specificType == "" || (specificType!="" && specificType == nowNote.noteType)))
					return i;
			}
			else // in is OK
			{
				if(nowNote.notePoint[0].type%8==type && position>=nowNote.notePoint[0].position && position<=nowNote.notePoint[1].position
					&& (specificType == "" || (specificType!="" && specificType == nowNote.noteType)))
					return i;
			}
		}

		return -1;
	}
	std::vector<int> DivaEditorMapData::findNoteToSelectByRange(int position,int leftUpX,int leftUpY,int rightDownX,int rightDownY)
	{
		if(leftUpX<0) leftUpX=0;
		else if(leftUpX>EDITCONFIG->NoteAreaWidth) leftUpX = EDITCONFIG->NoteAreaWidth;
		if(leftUpY<0) leftUpY=0;
		else if(leftUpY>EDITCONFIG->NoteAreaHeight) leftUpY = EDITCONFIG->NoteAreaHeight;
		if(rightDownX<0) rightDownX=0;
		else if(rightDownX>EDITCONFIG->NoteAreaWidth) rightDownX = EDITCONFIG->NoteAreaWidth;
		if(rightDownY<0) rightDownY=0;
		else if(rightDownY>EDITCONFIG->NoteAreaHeight) rightDownY = EDITCONFIG->NoteAreaHeight;

		if(leftUpX>rightDownX)
			std::swap(leftUpX,rightDownX);
		if(leftUpY>rightDownY)
			std::swap(leftUpY,rightDownY);

		std::vector<int> ret;
		for (int i=0;i<coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &nowNote = coreInfoPtr->notes[i];

			if(position>int(nowNote.notePoint[0].position)-int(nowNote.aheadBar)*EDITCONFIG->GridPerBeat*4 && position<=nowNote.notePoint[0].position
				&& nowNote.notePoint[0].x <= rightDownX && nowNote.notePoint[0].x >= leftUpX && nowNote.notePoint[0].y <= rightDownY && nowNote.notePoint[0].y >= leftUpY)
				ret.push_back(i);
			else
			{
				if(nowNote.notePoint.size()>1)
					if(position >= nowNote.notePoint[0].position && position < nowNote.notePoint[1].position
						&& nowNote.notePoint[0].x <= rightDownX && nowNote.notePoint[0].x >= leftUpX && nowNote.notePoint[0].y <= rightDownY && nowNote.notePoint[0].y >= leftUpY)
						ret.push_back(i);
			}
		}
		return ret;
	}

	gcn::Rectangle DivaEditorMapData::findSelectedAreaRectange()
	{
		if(EDITCONFIG->noteSelected.size()==0)
			return gcn::Rectangle(-1,-1,-1,-1);
		gcn::Rectangle ret(MAXINT32,MAXINT32,-1,-1);

		for(int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			divacore::NotePoint &notePoint = coreInfoPtr->notes[EDITCONFIG->noteSelected[i]].notePoint[0];
			if(notePoint.x<ret.x)
				ret.x = notePoint.x;
			if(notePoint.x>ret.width)
				ret.width = notePoint.x;
			if(notePoint.y<ret.y)
				ret.y = notePoint.y;
			if(notePoint.y>ret.height)
				ret.height = notePoint.y;
		}
		return ret;
	}

	int DivaEditorMapData::findFirstBiggerPositionNoteIndex(int pos)
	{
		int pLeft = 0, pRight = coreInfoPtr->notes.size();
		
		while(pLeft!=pRight)
		{
			int pMid=(pLeft+pRight)>>1;
			divacore::MapNote &pMidNote = coreInfoPtr->notes[pMid];
			if(pMidNote.notePoint[0].position<pos)
				pLeft = pMid;
			else if(pMidNote.notePoint[0].position>=pos)
				pRight = pMid;
		}

		return pLeft;
	}

	void DivaEditorMapData::findNoteIndexInRange(int leftPos, int rightPos, int &beginIndex, int &endIndex)
	{

	}

	void DivaEditorMapData::guessThisNotePositionByLastTwo(int pos, int& out_x,int &out_y,int &tail_x,int &tail_y)
	{

		int prev = -1,prev2 = -1;


		//Find last two notes before pos

		for (int i=0;i<EDITOR_PTR->mapData->coreInfoPtr->notes.size();i++)
		{
			divacore::MapNote &thisNote = EDITOR_PTR->mapData->coreInfoPtr->notes[i];
				
			if(thisNote.notePoint[0].position > pos)
				break;

			bool canCount = true;
			if(thisNote.notePoint.size()==2 && thisNote.notePoint[1].position > pos)
				canCount=false;

			if(canCount)
			{
				if(prev==-1)
					prev = i;
				else
				{
					prev2 = prev;
					prev = i;
				}
			}
		}

		//If not found
		if(prev==-1)
		{
			out_x=EDITCONFIG->NoteAreaWidth/2;
			out_y=EDITCONFIG->NoteAreaHeight/2;
			tail_x=5;
			tail_y=5;
			return;
		}
		else if(prev2==-1) //Only found one, can use tail position
		{
			divacore::MapNote &prevNote = EDITOR_PTR->mapData->coreInfoPtr->notes[prev];
			out_x=EDITCONFIG->NoteAreaWidth/2;
			out_y=EDITCONFIG->NoteAreaHeight/2;
			tail_x = Argument::asInt("tailx",prevNote.arg);
			tail_y = Argument::asInt("taily",prevNote.arg);
			return;
		}
		

		divacore::MapNote &prevNote = EDITOR_PTR->mapData->coreInfoPtr->notes[prev];
		divacore::MapNote &prev2Note = EDITOR_PTR->mapData->coreInfoPtr->notes[prev2];
		
		//Use the same tail direction as last note
		tail_x = Argument::asInt("tailx",prevNote.arg);
		tail_y = Argument::asInt("taily",prevNote.arg);

		//Calculater time delta
		int timeDelta = prevNote.notePoint[0].position - prev2Note.notePoint[prev2Note.notePoint.size()-1].position;

		// if there's no time delta, just return the last one
		if(timeDelta==0)
		{
			out_x = prevNote.notePoint[0].x;
			out_y = prevNote.notePoint[0].y;
			
			return;
		}

		//Calculate note position delta;
		int xDelta = prevNote.notePoint[0].x - prev2Note.notePoint[0].x,
			yDelta = prevNote.notePoint[0].y - prev2Note.notePoint[0].y;

		//Use time delta to calculate note position
		int needToDeltaX = (float)xDelta / (float)timeDelta * float(pos - prevNote.notePoint[prevNote.notePoint.size()-1].position),
			needToDeltaY = (float)yDelta / (float)timeDelta * float(pos - prevNote.notePoint[prevNote.notePoint.size()-1].position);

		int newX = prevNote.notePoint[0].x + needToDeltaX,
			newY = prevNote.notePoint[0].y + needToDeltaY;

		if(newX<0||newX>EDITCONFIG->NoteAreaWidth||newY<0||newY>EDITCONFIG->NoteAreaHeight)
		{
			//if out of range, symmetry it
			out_x = EDITCONFIG->NoteAreaWidth - prevNote.notePoint[0].x;
			out_y = EDITCONFIG->NoteAreaHeight - prevNote.notePoint[0].y;
			tail_x = -Argument::asInt("tailx",prevNote.arg);
			tail_y = -Argument::asInt("taily",prevNote.arg);
		}
		else
		{
			out_x = newX;
			out_y = newY;
		}
	}

	bool DivaEditorMapData::isNoteConflict(divacore::MapNote &note, bool ignoreSelected)
	{
		for(int i=0;i<note.notePoint.size();i++)
			if(note.notePoint[i].position<0 || note.notePoint[i].position>CORE_FLOW_PTR->getTotalPosition())
				return true;

		int deltaAccess = coreInfoPtr->notes.size()/10;

		int nowIndex = 0;
		if(deltaAccess>0)
			while(nowIndex+deltaAccess < coreInfoPtr->notes.size() && 
					coreInfoPtr->notes[nowIndex+deltaAccess].notePoint[coreInfoPtr->notes[nowIndex+deltaAccess].notePoint.size()-1].position 
								< note.notePoint[0].position)
					nowIndex += deltaAccess;

		for (;nowIndex<coreInfoPtr->notes.size();nowIndex++)
		{
			if(ignoreSelected&&EDITCONFIG->isNoteSelected(nowIndex))
				continue;
			if(coreInfoPtr->notes[nowIndex].notePoint[0].position > note.notePoint[note.notePoint.size()-1].position)
				break;

			int note1_left=coreInfoPtr->notes[nowIndex].notePoint[0].position,
				note1_right=coreInfoPtr->notes[nowIndex].notePoint[coreInfoPtr->notes[nowIndex].notePoint.size()-1].position,
				note1_type = coreInfoPtr->notes[nowIndex].notePoint[0].type,
				note2_left=note.notePoint[0].position,
				note2_right=note.notePoint[note.notePoint.size()-1].position,
				note2_type = note.notePoint[0].type;

			if( (note1_type==note2_type) && (!(note1_right<note2_left || note1_left > note2_right)) )
				return true;
		}

		return false;
	}



	//Grid and period operation
	int DivaEditorMapData::decodeOriginalGrid(int grid)
	{
		std::map<int,int>::iterator stopI = stopLength.begin();
		grid-=mapOffset;

		while(stopI!=stopLength.end())
		{
			if(stopI->first < grid && stopI->first + stopI->second <= grid)
				grid -= stopI->second;
			else if(stopI->first < grid && stopI->first+stopI->second > grid)
				grid = stopI->first;
			else
				break;
			stopI++;
		}

		return grid;
	}
	int DivaEditorMapData::encodeToOriginalGrid(int grid)
	{
		std::map<int,int>::iterator stopI = stopLength.begin();
		int ret = grid;
		ret+=mapOffset;
		
		while(stopI!=stopLength.end() && stopI->first < grid)
		{
			ret+=stopI->second;
			stopI++;
		}

		return ret;
	}
	int DivaEditorMapData::getGridInPeriod(float grid)
	{
		int nowGrid = decodeOriginalGrid(grid);
		if(nowGrid<0) return -1;

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			int thisBeatPos = beatI->first;
			int thisBeatNum = beatI->second;
			int periodCount = MAXPERIOD;
			beatI++;
			if(beatI!=beatNumChanged.end())
				periodCount = beatI->first-thisBeatPos;

			if(nowGrid >= periodCount*thisBeatNum*EDITCONFIG->GridPerBeat)
				nowGrid -= periodCount*thisBeatNum* EDITCONFIG->GridPerBeat;
			else
			{
				nowGrid %= thisBeatNum * EDITCONFIG->GridPerBeat;
				break;
			}
		}
		return nowGrid;
	}
	int DivaEditorMapData::getGridFromPeriod(int period)
	{
		if(period<0) return period*EDITCONFIG->GridPerBeat*beatNumChanged[0];
		else
		{
			int ret = 0;
			std::map<int,int>::iterator beatI = beatNumChanged.begin();

			while(true)
			{
				int thisBeaPos = beatI->first;
				int thisBeatNum = beatI->second;
				int periodCount = MAXPERIOD;
				beatI++;
				if(beatI!=beatNumChanged.end())
					periodCount = beatI->first-thisBeaPos;

				if(period > periodCount)
				{
					ret += thisBeatNum*periodCount*EDITCONFIG->GridPerBeat;
					period -= periodCount;
				}
				else
				{
					ret += period*thisBeatNum*EDITCONFIG->GridPerBeat;
					break;
				}
			}
			return encodeToOriginalGrid(ret);
		}
	}
	int DivaEditorMapData::getPeriodfromGrid(float grid)
	{
		int nowGrid = decodeOriginalGrid(grid);
		int nowPeriod = 0;

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			std::map<int,int>::iterator thisBeatI = beatI;
			int periodCount = MAXPERIOD;
			beatI++;
			if(beatI!=beatNumChanged.end())
				periodCount = beatI->first-thisBeatI->first;

			if(nowGrid >= periodCount*(thisBeatI->second*EDITCONFIG->GridPerBeat))
			{
				nowPeriod += periodCount;
				nowGrid -= periodCount*(thisBeatI->second*EDITCONFIG->GridPerBeat);
			}
			else
			{
				nowPeriod += nowGrid/(thisBeatI->second*EDITCONFIG->GridPerBeat);
				nowGrid %= (thisBeatI->second*EDITCONFIG->GridPerBeat);
				break;
			}
		}
		return nowPeriod;
	}
	int DivaEditorMapData::getGridLevel(float nowGrid)
	{
		int nowGridI = nowGrid;
		int nowGrade = EDITCONFIG->GridPerBeat;
		int gridInPeriod = getGridInPeriod(nowGridI);
		if(gridInPeriod==0)return 1;
		int nowLevel = 2;
		while(gridInPeriod%nowGrade!=0)
		{
			nowLevel++;
			gridInPeriod %= nowGrade;
			nowGrade>>=1;
		}
		return nowLevel;
	}
	int DivaEditorMapData::getNextStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = EDITCONFIG->GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid)+1) * deltaGrid);
	}
	int DivaEditorMapData::getPrevStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = EDITCONFIG->GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid) - 1) * deltaGrid);
	}
	int DivaEditorMapData::getNowStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = EDITCONFIG->GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid)) * deltaGrid);
	}
	int DivaEditorMapData::getNearestStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = EDITCONFIG->GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((round(decodeOriginalGrid(round(nowGrid))/deltaGrid)) * deltaGrid);
	}

	int DivaEditorMapData::getCrossAStandardBeatPos(float lastPos, float nowPos, int useOffset)
	{
		if(lastPos>=nowPos) return 0;

		//Use this offset
		int lastOffset = mapOffset;
		if(useOffset!=-65536)
			mapOffset = useOffset;
		int lastPeriod = getPeriodfromGrid(lastPos),nowPeriod = getPeriodfromGrid(nowPos),
			lastGrid = getGridInPeriod(lastPos),nowGrid = getGridInPeriod(nowPos);
		if(useOffset!=-65536)
			mapOffset = lastOffset;

		if(lastPeriod>nowPeriod || (lastPeriod==nowPeriod&&lastGrid>=nowGrid))
			return 0;

		if(lastPeriod<nowPeriod) return 1;

		int lastBeat = lastGrid / EDITCONFIG->GridPerBeat;
		int nowBeat = nowGrid / EDITCONFIG->GridPerBeat;

		if(lastBeat<nowBeat) return 2;
		
		return 0;
	}


	//BeatNum operation
	int DivaEditorMapData::getBeatNum(float pos)
	{
		int nowGrid = decodeOriginalGrid(pos);

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			std::map<int,int>::iterator thisBeatI = beatI;
			beatI++;
			int periodCount = MAXPERIOD;
			if(beatI!=beatNumChanged.end())
			{
				periodCount = beatI->first-thisBeatI->first;
			}

			if(nowGrid >= periodCount*thisBeatI->second*EDITCONFIG->GridPerBeat)
				nowGrid -= periodCount*thisBeatI->second*EDITCONFIG->GridPerBeat;
			else
				return thisBeatI->second;
		}
	}
	int DivaEditorMapData::getBeatNumPeriod(int period)
	{
		int thisPeriod = period;
		int ret = -1;
		for (std::map<int,int>::iterator i=beatNumChanged.begin();i!=beatNumChanged.end();i++)
			if(i->first<=thisPeriod)
				ret=i->first;
		return ret;
	}

	bool DivaEditorMapData::beatNumExist(int period)
	{
		return beatNumChanged.find(period)!=beatNumChanged.end();
	}
	std::map<int,int> DivaEditorMapData::getBeatNumByRange(float left, float right)
	{
		int leftPeriod = getPeriodfromGrid(left);
		int rightPeriod = getPeriodfromGrid(right);

		std::map<int,int> ret;
		for (std::map<int,int>::iterator i = beatNumChanged.begin();i!=beatNumChanged.end();i++)
			if(i->first>=leftPeriod&&i->first<=rightPeriod)
				ret[i->first]=i->second;

		return ret;
	}

	void DivaEditorMapData::beatNum_change(int period, int beatNum)
	{
		if(beatNum<=0) return;
			beatNumChanged[period]=beatNum;
	}
	void DivaEditorMapData::beatNum_delete(int period)
	{
		int toDel = getBeatNumPeriod(period);
		if(toDel!=0)
			beatNumChanged.erase(beatNumChanged.find(toDel));
	}


	//offset operations
	int DivaEditorMapData::getOffset()
	{
		return mapOffset;
	}
	void DivaEditorMapData::offset_set(int offset, bool movePlacedNote)
	{
		//if(offset<0) return;
		int lastOffset = mapOffset;
		mapOffset = offset;

		if(movePlacedNote)
			if(!moveAll(0,MAXINT32,mapOffset-lastOffset))
				mapOffset = lastOffset;
	}


	//stop operations
	int DivaEditorMapData::getStop(float pos)
	{
		int nowStopIndex = findStopIndex(pos);
		if(nowStopIndex!=-1)
			return stopLength[nowStopIndex];
		return 0;
	}
	int DivaEditorMapData::getStopPos(float pos)
	{
		int stopIndex = findStopIndex(pos);
		if(stopIndex==-1)
			return -1;
		else
			return encodeToOriginalGrid(stopIndex);
	}

	vector<int> DivaEditorMapData::getStopPositionByRange(float left, float right)
	{
		vector<int> ret;
		for (std::map<int,int>::iterator i = stopLength.begin();i!=stopLength.end();i++)
		{
			int firstEncode = encodeToOriginalGrid(i->first);
			if((firstEncode>=left && firstEncode<=right) || ((firstEncode+i->second)>=left && (firstEncode+i->second)<=right))
			{
				ret.push_back(firstEncode);
				ret.push_back(firstEncode+i->second);
			}
		}
		return ret;
	}

	void DivaEditorMapData::stop_change(float pos, int length)
	{
		if(length<0) return;
		else if(length==0) stop_delete(pos);
		else
		{
			//check if is in stop
			int nowStopIndex = findStopIndex(pos);
			if(nowStopIndex!=-1)
			{
				int delta = length - stopLength[nowStopIndex];
				if(moveAll(encodeToOriginalGrid(nowStopIndex),MAXINT32,delta))
					stopLength[nowStopIndex]=length;
			}
		}
		
	}
	void DivaEditorMapData::stop_insert(float pos, int length)
	{
		if(length<=0) return;
		int toIns = decodeOriginalGrid(pos);
		if(toIns>=0)
			if(stopLength.find(toIns) == stopLength.end())
			{
				if(moveAll(encodeToOriginalGrid(toIns),MAXINT32,length))
					stopLength[toIns]=length;
			}
	}
	void DivaEditorMapData::stop_delete(float pos)
	{
		int nowStopIndex = findStopIndex(pos);
		if(nowStopIndex!=-1)
		{
			moveAll(encodeToOriginalGrid(nowStopIndex),MAXINT32,-stopLength[nowStopIndex]);
			stopLength.erase(nowStopIndex);
		}
	}


	//Event
	void DivaEditorMapData::event_modifyTimePos(int index, int pos)
	{
		coreInfoPtr->events[index].position = pos;
	}
	int DivaEditorMapData::findEvent(divacore::MapEvent &event)
	{
		for (int i=0;i<coreInfoPtr->events.size();i++)
		{
			divacore::MapEvent &thisEvent = coreInfoPtr->events[i];
			if(thisEvent.position == event.position && thisEvent.eventType == event.eventType)
				return i;
		}
		return -1;
	}

	//BPM
	double DivaEditorMapData::getBPM(float pos)
	{
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		return divacore::Argument::asDouble("value",coreInfoPtr->events[nowBPMIndex].arg);
	}
	int DivaEditorMapData::getBPMPos(float pos)
	{
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		return coreInfoPtr->events[nowBPMIndex].position;
	}
	int DivaEditorMapData::getNextBPMPos(float pos)
	{
		int nextBPMIndex=findNextEventIndex(pos,"bpm");
		if(nextBPMIndex==-1)
			return MAXINT32;
		else
			return coreInfoPtr->events[nextBPMIndex].position;
	}
	int DivaEditorMapData::getPrevBPMIndex(float pos)
	{
		int lastOrEqual = findLastOrEqualEventIndex(pos,"bpm");
		int prev = lastOrEqual-1;
		for(;prev>=0;prev--)
			if(coreInfoPtr->events[prev].eventType == "bpm")
				return prev;
		return -1;
	}
	
	void DivaEditorMapData::bpm_change(float pos, double bpm)
	{
		if(bpm<=0) return;
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		coreInfoPtr->events[nowBPMIndex].arg["value"] = sora::SoraAny(bpm);
		adjustEventOrder(nowBPMIndex);
		EDITUTILITY.reCaltTime();
	}
	void DivaEditorMapData::bpm_insert(float pos, double bpm)
	{
		if(bpm<=0) return;
		//Get Nearest Standard Grid
		int toInsertPos = getNearestStandardGrid(pos, EDITCONFIG->getGridToShowPerBeat());

		//Check if already exist or last bpm index is equal with this one
		int findBPMIndex = findLastOrEqualEventIndex(toInsertPos,"bpm");

		double lastBPMValue = divacore::Argument::asDouble("value",coreInfoPtr->events[findBPMIndex].arg);

		if(coreInfoPtr->events[findBPMIndex].position == toInsertPos) // meas change
		{
			bpm_change(pos,bpm);
		}
		else
		{
			divacore::MapEvent eventToPush;
			eventToPush.eventType = "bpm";
			eventToPush.position = toInsertPos;
			eventToPush.arg["value"] = sora::SoraAny(bpm);
			coreInfoPtr->events.push_back(eventToPush);
			adjustEventOrder(coreInfoPtr->events.size()-1);
			EDITUTILITY.reCaltTime();
		}
	}
	void DivaEditorMapData::bpm_insert(int pos, double bpm)
	{
		if(bpm<=0) return;
		//Get Nearest Standard Grid
		int toInsertPos = pos;

		divacore::MapEvent eventToPush;
		eventToPush.eventType = "bpm";
		eventToPush.position = toInsertPos;
		eventToPush.arg["value"] = sora::SoraAny(bpm);
		coreInfoPtr->events.push_back(eventToPush);
		adjustEventOrder(coreInfoPtr->events.size()-1);
	}
	void DivaEditorMapData::bpm_delete(float pos)
	{
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		if(coreInfoPtr->events[nowBPMIndex].position!=0)
			coreInfoPtr->events.erase(coreInfoPtr->events.begin()+nowBPMIndex);
		EDITUTILITY.reCaltTime();
	}


	//Tail Speed
	float DivaEditorMapData::getTailSpeed()
	{
		return EDITOR_PTR->mapData->coreInfoPtr->header.speedScale;
	}
	void DivaEditorMapData::tailSpeed_change(float speed)
	{
		if(speed<0.1) speed=0.1;
		EDITOR_PTR->mapData->coreInfoPtr->header.speedScale = speed;
	}

	//Move All
	bool DivaEditorMapData::moveAll(int left, int right, int delta)
	{
		//Check border	
		int deltaOffset = delta;
		int minPos = MAXINT32-100000000,maxPos=-1;
		for(std::vector<divacore::MapNote>::iterator i=coreInfoPtr->notes.begin();i!=coreInfoPtr->notes.end();i++)
			for(std::vector<divacore::NotePoint>::iterator j=i->notePoint.begin();j!=i->notePoint.end();j++)
			{
				if((int)j->position>maxPos)
					maxPos = (int)j->position;
				if((int)j->position<minPos)
					minPos = (int)j->position;
			}

		for(std::vector<divacore::MapEvent>::iterator i=coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
		{
			if((i->eventType == "bpm" && (int)i->position!=0))
			{
				if((int)i->position>maxPos)
					maxPos = (int)i->position;
				if((int)i->position<minPos)
					minPos = (int)i->position;
			}
		}

		//Move All
		if(minPos+deltaOffset>=0 && maxPos+deltaOffset<(int)CORE_FLOW_PTR->getTotalPosition())
		{
			for(std::vector<divacore::MapNote>::iterator i=coreInfoPtr->notes.begin();i!=coreInfoPtr->notes.end();i++)
				if(i->notePoint.begin()->position>=left && i->notePoint.begin()->position<right)
					for(std::vector<divacore::NotePoint>::iterator j=i->notePoint.begin();j!=i->notePoint.end();j++)
						j->position = (int)j->position + deltaOffset;

			for(std::vector<divacore::MapEvent>::iterator i=coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
				if((i->position>=left && i->position < right) && (i->eventType == "bpm" && i->position!=0))
					i->position = (int)i->position + deltaOffset;

			sortEvent();
			sortNote();

			EDITUTILITY.reCaltTime();
			return true;
		}
		else
			return false;
	}

	//Resource Function
	std::string DivaEditorMapData::findResourceTypeStrByID(std::string id)
	{
		divacore::MapResourceInfo &resourceInfo = coreInfoPtr->resources[id];

		if(resourceInfo.type == divacore::MapResourceInfo::VIDEO)
			return "playVideo";
		else if (resourceInfo.type == divacore::MapResourceInfo::IMAGE)
			return "displayImage";
		else if(resourceInfo.type == divacore::MapResourceInfo::AUDIO)
			return "playMusic";
		return "ERROR";
	}
	std::wstring DivaEditorMapData::getResourceDescriptionByIndex(int index)
	{
		std::string resourceID = findResourceIDByIndex(index);
		if(resourceDescription.find(resourceID)==resourceDescription.end())
			resourceDescription[resourceID] = sora::s2ws(resourceID);
		return resourceDescription[resourceID];
	}
	std::wstring DivaEditorMapData::getResourceDescription(std::string id)
	{
		if(resourceDescription.find(id)!=resourceDescription.end())
			return resourceDescription[id];
		return L"";
	}
	std::string DivaEditorMapData::findResourceIDByIndex(int index)
	{
		int tIndex=-1;
		for(std::map<std::string,MapResourceInfo>::iterator i=coreInfoPtr->resources.begin();i!=coreInfoPtr->resources.end();i++)
		{
			tIndex++;
			if(tIndex==index)
				return i->second.ID;
		}
		return "ERROR";
	}
	std::string DivaEditorMapData::findResourceIDByTypeAndIndex(divacore::MapResourceInfo::ResourceType type, int index)
	{
		int nowIndex = -1;
		if(type == MapResourceInfo::AUDIO)
			nowIndex = 0; //There's None
		for(divacore::MapInfo::RESOURCES::iterator i=EDITOR_PTR->mapData->coreInfoPtr->resources.begin();i!=EDITOR_PTR->mapData->coreInfoPtr->resources.end();i++)
		{
			divacore::MapResourceInfo &resourceInfo = i->second;
			if(resourceInfo.type == type && resourceInfo.ID!="hit" && resourceInfo.ID!="miss")
			{
				nowIndex++;
				if(index == nowIndex)
					return resourceInfo.ID;
			}
		}
		return "";
	}

	int DivaEditorMapData::findResourceIndexByID(std::string id)
	{
		int ret=-1;
		for(std::map<std::string,MapResourceInfo>::iterator i=coreInfoPtr->resources.begin();i!=coreInfoPtr->resources.end();i++)
		{
			ret++;
			if(i->second.ID==id)
				return ret;
		}
		return -1;
	}

	bool DivaEditorMapData::checkResourceCanModify(std::string resourceID)
	{
		if(resourceID == coreInfoPtr->header.mainSound)
		{
			if(EDITCONFIG->map_initialized)
				MessageBoxW((HWND)sora::SoraCore::Instance()->getMainWindowHandle(),
					LOCALIZATION->getLocalStr(L"Tip_CannotModifyMainSound").c_str(),
					LOCALIZATION->getLocalStr(L"Messagebox_Tips").c_str()
					,MB_OK);
			return false;
		}
		else if(resourceID == "hit" || resourceID == "miss")
		{
			if(EDITCONFIG->map_initialized)
				MessageBoxW((HWND)sora::SoraCore::Instance()->getMainWindowHandle(),
					LOCALIZATION->getLocalStr(L"Tip_CannotModifyHitMiss").c_str(),
					LOCALIZATION->getLocalStr(L"Messagebox_Tips").c_str()
					,MB_OK);
			return false;
		}
		return true;
	}

	std::string DivaEditorMapData::resource_add(std::wstring filename, bool onlyAddInfo)
	{
		divacore::MapResourceInfo resourceInfo;

		//get file type
		std::wstring ext = L"", safeFileName = L"";
		int dotPos;
		for(dotPos=filename.length()-1;dotPos>=0;dotPos--)
		{
			if(filename[dotPos]==L'.')
				break;
			ext = filename[dotPos] + ext;
		}

		for (dotPos--;dotPos>=0;dotPos--)
		{
			if((filename[dotPos]==L'/')||(filename[dotPos]==L'\\'))
				break;
			safeFileName = filename[dotPos] + safeFileName;
		}
		
		tolower(ext);
		std::string typeStr = "";

		if(videoExtensions.find(ext)!=std::wstring::npos)
		{
			resourceInfo.type = divacore::MapResourceInfo::VIDEO;
			typeStr = "VIDEO";
			if (EDITCONFIG->map_initialized)
				MessageBoxW((HWND)sora::SoraCore::Instance()->getMainWindowHandle(),
					LOCALIZATION->getLocalStr(L"Tip_InitializeVideoPlugins").c_str(),
					LOCALIZATION->getLocalStr(L"Messagebox_Tips").c_str()
					,MB_OK);
		}
		else if(imageExtensions.find(ext)!=std::wstring::npos)
		{
			resourceInfo.type = divacore::MapResourceInfo::IMAGE;
			typeStr = "IMAGE";
		}
		else if(audioExtensions.find(ext)!=std::wstring::npos)
		{
			resourceInfo.type = divacore::MapResourceInfo::AUDIO;
			typeStr = "AUDIO";
		}
		else
			return "ERROR";

		//get ID
		int idToIns = 0;
		while(true)
		{
			std::string thisID = typeStr + "_" + divacore::iToS(idToIns);
			std::map<std::string,MapResourceInfo>::iterator i;
			for(i=coreInfoPtr->resources.begin();i!=coreInfoPtr->resources.end();i++)
			{
				if(i->second.ID==thisID)
					break;
			}

			//Found
			if(i==coreInfoPtr->resources.end())
			{
				resourceInfo.ID = thisID;
				break;
			}

			idToIns++;
		}

		if(!onlyAddInfo)
		{
			//Copy file
			resourceInfo.filePath = sora::s2ws(typeStr + "/" + resourceInfo.ID) + L'.' + ext;
			CreateDirectoryW((workingDirectory + L"/" + sora::s2ws(typeStr)).c_str(),NULL);

			std::wstring copyTo = (workingDirectory + L"/" + resourceInfo.filePath);

			bool ret = CopyFileW(filename.c_str(), copyTo.c_str(), false);
		}
		else
		{
			resourceInfo.filePath = filename;
		}
		
		resourceInfo.flag=true;

		resourceDescription[resourceInfo.ID] = safeFileName;

		if(EDITCONFIG->map_initialized)
		{
			//Map ID and filename
			EDITUTILITY.loadResource(resourceInfo);
		}
		else
			coreInfoPtr->resources[resourceInfo.ID] = resourceInfo;

		return resourceInfo.ID;
	}
	void DivaEditorMapData::resource_delete(std::string id)
	{
		if(coreInfoPtr->resources.find(id)==coreInfoPtr->resources.end())
			return;
		
		if(!checkResourceCanModify(id))
			return;
		EDITCONFIG->isMapChanged=true;
			

		divacore::MapResourceInfo &resourceInfo = coreInfoPtr->resources[id];

		//Remove the events using this resource
		for (int i=0;i<coreInfoPtr->events.size();i++)
		{
			if(coreInfoPtr->events[i].eventType == "playVideo" 
				|| coreInfoPtr->events[i].eventType == "playMusic"
				|| coreInfoPtr->events[i].eventType == "displayImage")
			{
				if(divacore::Argument::asString("id",coreInfoPtr->events[i].arg)==resourceInfo.ID)
				{
					EDITUTILITY.delEvent(i);
					i--;
				}
			}
		}

		//unload the resource
		EDITUTILITY.unloadResource(resourceInfo);
		//delete the resource file
		//DeleteFileW((workingDirectory + L"/" + resourceInfo.filePath).c_str());

		//remove the safefilename
		resourceDescription.erase(resourceDescription.find(resourceInfo.ID));
		coreInfoPtr->resources.erase(coreInfoPtr->resources.find(id));
	}

	int DivaEditorMapData::findResourceEventIndexByIndexInResource(int index, std::string resourceID)
	{
		int tCount=-1;
		for (int i=0;i<coreInfoPtr->events.size();i++)
		{
			divacore::MapEvent &thisEvent = coreInfoPtr->events[i];
			if(thisEvent.eventType == "playVideo" || thisEvent.eventType == "displayImage" || thisEvent.eventType == "playMusic")
			{
				if(Argument::asString("id",thisEvent.arg)==resourceID)
				{
					tCount++;
					if(tCount==index)
						return i;
				}
			}
		}
		return -1;
	}
	int DivaEditorMapData::resourceEvent_add(int pos, std::string resourceID)
	{
		if(!checkResourceCanModify(resourceID))
			return -1;
		EDITCONFIG->isMapChanged=true;

		divacore::MapEvent toAdd;
		toAdd.position = pos;
		toAdd.arg["id"] = resourceID;
		toAdd.eventType = findResourceTypeStrByID(resourceID);
		if(EDITCONFIG->map_initialized)
			toAdd.time = EDITUTILITY.posToTime(pos);

		if(toAdd.eventType == "playVideo" || toAdd.eventType== "displayImage")
		{
			toAdd.arg["width"] = 1920;
			toAdd.arg["height"] = 1080;
		}
		else if(toAdd.eventType == "playMusic")
		{
			toAdd.arg["channel"] = resourceID + "_" + iToS(pos);
		}

		coreInfoPtr->events.push_back(toAdd);
		return adjustEventOrder(coreInfoPtr->events.size()-1);
	}
	int DivaEditorMapData::resourceEvent_modifyPos(int index, int pos)
	{
		if(!checkResourceCanModify(Argument::asString("id",(coreInfoPtr->events.begin()+index)->arg)))
			return -1;
		EDITCONFIG->isMapChanged=true;
		coreInfoPtr->events[index].position = pos;
		return adjustEventOrder(index);
	}
	void DivaEditorMapData::resourceEvent_modifyResource(int index, std::string resourceID)
	{
		if(!checkResourceCanModify(Argument::asString("id",(coreInfoPtr->events.begin()+index)->arg)))
			return;
		EDITCONFIG->isMapChanged=true;
		coreInfoPtr->events[index].arg["id"] = resourceID;
		coreInfoPtr->events[index].eventType = findResourceTypeStrByID(resourceID);
		if(coreInfoPtr->events[index].eventType == "playVideo" || coreInfoPtr->events[index].eventType== "displayImage")
		{
			coreInfoPtr->events[index].arg["width"] = 1920;
			coreInfoPtr->events[index].arg["height"] = 1080;
		}
	}
	void DivaEditorMapData::resourceEvent_delete(int index)
	{
		if(!checkResourceCanModify(Argument::asString("id",(coreInfoPtr->events.begin()+index)->arg)))
			return;
		EDITCONFIG->isMapChanged=true;

		if(index>=0&&index<coreInfoPtr->events.size())
			coreInfoPtr->events.erase(coreInfoPtr->events.begin()+index);
	}

	void DivaEditorMapData::resourceDescription_modify(std::string resourceID, std::wstring description)
	{
		resourceDescription[resourceID] = description;
	}

	void DivaEditorMapData::modifyGlobalHitMissSound(std::wstring filename, std::string type)
	{
		divacore::MapResourceInfo resourceInfo;
		if(EDITOR_PTR->mapData->coreInfoPtr->resources.find(type)!=EDITOR_PTR->mapData->coreInfoPtr->resources.end())
		{
			//unload the old resource file
			resourceInfo = EDITOR_PTR->mapData->coreInfoPtr->resources[type];
		}
		else
		{
			resourceInfo.ID = type;
			resourceInfo.filePath = L"";
			resourceInfo.type = divacore::MapResourceInfo::AUDIO;
			resourceInfo.flag = false;
		}

		EDITUTILITY.unloadResource(resourceInfo);

		if(resourceInfo.filePath!=L"")
			DeleteFileW((workingDirectory + L"/" + resourceInfo.filePath).c_str());
		if(resourceDescription.find(type)!=resourceDescription.end())
			resourceDescription.erase(resourceDescription.find(type));
		if(coreInfoPtr->resources.find(type)!=coreInfoPtr->resources.end())
			coreInfoPtr->resources.erase(coreInfoPtr->resources.find(type));
		

		resourceInfo.flag=false;

		//get safe filename
		std::wstring safeFileName = L"";
		for (int dotPos=filename.length()-1;dotPos>=0;dotPos--)
		{
			if((filename[dotPos]==L'/')||(filename[dotPos]==L'\\'))
				break;
			safeFileName = filename[dotPos] + safeFileName;
		}
		resourceInfo.filePath = safeFileName;

		//Copy file

		CopyFileW(filename.c_str(), (workingDirectory + L"/" + resourceInfo.filePath).c_str(), false);
		EDITUTILITY.loadResource(resourceInfo);
		if(safeFileName == L"")
			safeFileName = s2ws(type);
		resourceDescription[type] = safeFileName;
	}

	bool DivaEditorMapData::modifySelectedNotesPos(int posDelta, std::string operationID)
	{
		if(posDelta==0)
			return false;

		//Check conflict here
		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			divacore::MapNote thisNote = coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];
			for(int notePosI = 0;notePosI < thisNote.notePoint.size();notePosI++)
				thisNote.notePoint[notePosI].position += posDelta;
			if(isNoteConflict(thisNote,true))
				return false;
		}


		DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
		for (int i=posDelta<0?0:(EDITCONFIG->noteSelected.size()-1);posDelta<0?(i<EDITCONFIG->noteSelected.size()):(i>=0);posDelta<0?(i++):(i--))
			thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],posDelta,true));
		EDITCONFIG->addAndDoOperation(thisModifySet,operationID);

		return true;
	}

	bool DivaEditorMapData::modifySelectedNotesType(int typeDelta, std::string operationID)
	{
		if(typeDelta==0)
			return false;

		//Check conflict here
		for (int i=0;i<EDITCONFIG->noteSelected.size();i++)
		{
			divacore::MapNote thisNote = coreInfoPtr->notes[EDITCONFIG->noteSelected[i]];

			int thisType = thisNote.notePoint[0].type;
			thisType = getNoteTypeIndexFromNoteType(thisType);

			if(thisType+typeDelta<0||thisType+typeDelta>=8)
				return false;

			for(int notePosI = 0;notePosI < thisNote.notePoint.size();notePosI++)
				thisNote.notePoint[notePosI].type = getNoteTypeFromNoteTypeIndex(thisType+typeDelta);

			if(isNoteConflict(thisNote,true))
				return false;
		}


		DivaEditorOperationSet *thisModifySet = new DivaEditorOperationSet();
		for (int i=typeDelta<0?0:(EDITCONFIG->noteSelected.size()-1);typeDelta<0?(i<EDITCONFIG->noteSelected.size()):(i>=0);typeDelta<0?(i++):(i--))
			thisModifySet->addOperation(new DivaEditorOperation_ModifyNote(EDITCONFIG->noteSelected[i],typeDelta,true,true));
		EDITCONFIG->addAndDoOperation(thisModifySet, operationID);

		return true;
	}

}