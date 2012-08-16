#include "divaeditor/DivaEditor.h"
#include "divaeditor/DivaEditorCommon.h"
#include "divaeditor/DivaEditorMapData.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include "stdio.h"

namespace divaeditor
{
	//using namespace divacore;


	DivaEditorMapData::DivaEditorMapData()
	{
		mapOffset = 0;
		beatNumChanged[0]=4;

		//DEBUG
		workingDirectory = L"C:/Users/Administrator/Documents/GitProjects/divaol-core/divaol/song/Ç§±¾—@/";
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
		comboNote.arg["standard_num"] = int((comboNote.notePoint[1].position-comboNote.notePoint[0].position)/(EDITCONFIG->GridPerBeat/4));
	}

	int DivaEditorMapData::addNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key)
	{
		divacore::MapNote mapNote = initNote(pos,keyPress,arrow,x,y,tailX,tailY,"normal",key);

		int checkExist = checkNoteExists(pos, mapNote.notePoint[0].type, mapNote.noteType);
		if(checkExist==-1)
		{
			EDITUTILITY.resetNote(mapNote);

			coreInfoPtr->notes.push_back(mapNote);
			int insertIndex = adjustNoteOrder(coreInfoPtr->notes.size()-1);

			EDITUTILITY.refreshAll();

			return insertIndex;
		}
		else
			return checkExist;
	}
	int DivaEditorMapData::addLongNote(divacore::MapNote longNote)
	{
		int checkExist = checkNoteExists(longNote.notePoint[0].position, longNote.notePoint[0].type, longNote.noteType);
		if(checkExist==-1)
		{
			EDITUTILITY.resetNote(longNote);

			coreInfoPtr->notes.push_back(longNote);
			int insertIndex = adjustNoteOrder(coreInfoPtr->notes.size()-1);

			EDITUTILITY.refreshAll();

			return insertIndex;
		}
		else
			return checkExist;
	}

	void DivaEditorMapData::note_modifyTail(int index,int tailX,int tailY)
	{
		coreInfoPtr->notes[index].arg["tailx"]=tailX;
		coreInfoPtr->notes[index].arg["taily"]=tailY;
	}
	void DivaEditorMapData::note_modifyPos(int index, int x, int y, bool isDelta)
	{
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
		if(!isDelta)
			pos = pos - coreInfoPtr->notes[index].notePoint[0].position;
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			coreInfoPtr->notes[index].notePoint[i].position += pos;
		}
	}
	void DivaEditorMapData::note_modifyType(int index, char keyPress, bool arrow)
	{
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			coreInfoPtr->notes[index].notePoint[i].type = getNoteTypeFromKeyPress(keyPress,arrow);
		}
	}
	void DivaEditorMapData::note_modifyKey(int index, int key)
	{
		for(int i=0;i<coreInfoPtr->notes[index].notePoint.size();i++)
		{
			coreInfoPtr->notes[index].notePoint[i].key = key;
		}
	}
	void DivaEditorMapData::note_delete(int index)
	{
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
				ret.width = ret.x;
			if(notePoint.y<ret.y)
				ret.y = notePoint.y;
			if(notePoint.y>ret.height)
				ret.height = ret.y;
		}
		return ret;
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
			int periodCount = 10000;
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
				int periodCount = 1000;
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
			int periodCount = 10000;
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


	//BeatNum operation
	int DivaEditorMapData::getBeatNum(float pos)
	{
		int nowGrid = decodeOriginalGrid(pos);

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			std::map<int,int>::iterator thisBeatI = beatI;
			beatI++;
			int periodCount = 10000;
			if(beatI!=beatNumChanged.end())
			{
				periodCount = beatI->first-thisBeatI->first;
			}

			if(nowGrid >= periodCount*thisBeatI->second)
				nowGrid -= periodCount*thisBeatI->second;
			else
				return thisBeatI->second;
		}
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

	void DivaEditorMapData::beatNum_change(float pos, int beatNum)
	{
		if(beatNum<=0) return;
		beatNumChanged[getPeriodfromGrid(pos)]=beatNum;
	}
	void DivaEditorMapData::beatNum_delete(float pos)
	{
		int thisPeriod = getPeriodfromGrid(pos);
		int toDel = -1;
		for (std::map<int,int>::iterator i=beatNumChanged.begin();i!=beatNumChanged.end();i++)
			if(i->first<=thisPeriod)
				toDel=i->first;
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

		if(!(movePlacedNote&&moveAll(0,MAXINT32,mapOffset-lastOffset)))
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


	//BPM
	float DivaEditorMapData::getBPM(float pos)
	{
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		return divacore::Argument::asFloat("value",coreInfoPtr->events[nowBPMIndex].arg);
	}
	
	void DivaEditorMapData::bpm_change(float pos, float bpm)
	{
		if(bpm<=0) return;
		int nowBPMIndex=findLastOrEqualEventIndex(pos,"bpm");
		coreInfoPtr->events[nowBPMIndex].arg["value"] = sora::SoraAny((double)bpm);
		adjustEventOrder(nowBPMIndex);
		EDITUTILITY.reCaltTime();
	}
	void DivaEditorMapData::bpm_insert(float pos, float bpm)
	{
		if(bpm<=0) return;
		//Get Nearest Standard Grid
		int toInsertPos = getNearestStandardGrid(pos, EDITCONFIG->getGridToShowPerBeat());

		//Check if already exist or last bpm index is equal with this one
		int findBPMIndex = findLastOrEqualEventIndex(toInsertPos,"bpm");

		float lastBPMValue = divacore::Argument::asFloat("value",coreInfoPtr->events[findBPMIndex].arg);

		if(coreInfoPtr->events[findBPMIndex].position == toInsertPos) // meas change
		{
			bpm_change(pos,bpm);
		}
		else
		{
			divacore::MapEvent eventToPush;
			eventToPush.eventType = "bpm";
			eventToPush.position = toInsertPos;
			eventToPush.arg["value"] = sora::SoraAny((double)bpm);
			coreInfoPtr->events.push_back(eventToPush);
			adjustEventOrder(coreInfoPtr->events.size()-1);
			EDITUTILITY.reCaltTime();
		}
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
		EDITUTILITY.refreshAll();
	}


	//Move All
	bool DivaEditorMapData::moveAll(int left, int right, int delta)
	{
		//Check border	
		int deltaOffset = delta;
		int minPos = MAXINT32,maxPos=-1;
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
			if(i->eventType != "bpm" || (i->eventType == "bpm" && (int)i->position!=0))
			{
				if((int)i->position>maxPos)
					maxPos = (int)i->position;
				if((int)i->position<minPos)
					minPos = (int)i->position;
			}
		}

		//Move All
		if(minPos+deltaOffset>=0 && minPos+deltaOffset<CORE_FLOW_PTR->getTotalPosition()
			&& maxPos+deltaOffset>=0 && maxPos+deltaOffset<CORE_FLOW_PTR->getTotalPosition())
		{
			for(std::vector<divacore::MapNote>::iterator i=coreInfoPtr->notes.begin();i!=coreInfoPtr->notes.end();i++)
				if(i->notePoint.begin()->position>=left && i->notePoint.begin()->position<right)
					for(std::vector<divacore::NotePoint>::iterator j=i->notePoint.begin();j!=i->notePoint.end();j++)
						j->position = (int)j->position + deltaOffset;

			for(std::vector<divacore::MapEvent>::iterator i=coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
				if((i->position>=left && i->position < right) && ((i->eventType != "bpm") || (i->eventType == "bpm" && i->position!=0)))
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

	std::string DivaEditorMapData::resource_add(std::wstring filename)
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

		if(videoExtentions.find(ext)!=std::wstring::npos)
		{
			resourceInfo.type = divacore::MapResourceInfo::VIDEO;
			typeStr = "VIDEO";
		}
		else if(imageExtentions.find(ext)!=std::wstring::npos)
		{
			resourceInfo.type = divacore::MapResourceInfo::IMAGE;
			typeStr = "IMAGE";
		}
		else if(audioExtentions.find(ext)!=std::wstring::npos)
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
		}

		//Copy file
		resourceInfo.filePath = sora::s2ws(typeStr + "/" + resourceInfo.ID) + L'.' + ext;
		CreateDirectoryW((workingDirectory + L"/" + sora::s2ws(typeStr)).c_str(),NULL);
		CopyFileW(filename.c_str(), (workingDirectory + L"/" + resourceInfo.filePath).c_str(), false);
		
		resourceInfo.flag=true;

		//Map ID and filename
		resourceDescription[resourceInfo.ID] = safeFileName;

		EDITUTILITY.loadResource(resourceInfo);

		return resourceInfo.ID;
	}
	void DivaEditorMapData::resource_delete(std::string id)
	{
		if(coreInfoPtr->resources.find(id)==coreInfoPtr->resources.end())
			return;

		divacore::MapResourceInfo &resourceInfo = coreInfoPtr->resources[id];

		//Remove the events using this resource
		for (int i=0;i<coreInfoPtr->events.size();i++)
		{
			if(coreInfoPtr->events[i].eventType == "playVideo" 
				|| coreInfoPtr->events[i].eventType == "playMusic"
				|| coreInfoPtr->events[i].eventType == "displayImage")
			{
				if(divacore::Argument::asString("id",coreInfoPtr->events[i].arg)==resourceInfo.ID
					&&resourceInfo.ID!=coreInfoPtr->header.mainSound)
				{
					EDITUTILITY.delEvent(i);
					i--;
				}
			}
		}

		//remove the safefilename
		resourceDescription.erase(resourceDescription.find(resourceInfo.ID));
		coreInfoPtr->resources.erase(coreInfoPtr->resources.find(id));

		//unload the resource
		EDITUTILITY.unloadResource(resourceInfo);

		//delete the resource file
		DeleteFileW((workingDirectory + L"/" + resourceInfo.filePath).c_str());
	}


	int DivaEditorMapData::resourceEvent_add(int pos, std::string resourceID)
	{
		divacore::MapEvent toAdd;
		toAdd.position = pos;
		toAdd.arg["id"] = resourceID;
		toAdd.eventType = findResourceTypeStrByID(resourceID);
		toAdd.time = EDITUTILITY.posToTime(pos);

		if(toAdd.eventType == "playVideo" || toAdd.eventType== "displayImage")
		{
			toAdd.arg["width"] = 1920;
			toAdd.arg["height"] = 1080;
		}

		coreInfoPtr->events.push_back(toAdd);

		return adjustEventOrder(coreInfoPtr->events.size()-1);
	}
	int DivaEditorMapData::resourceEvent_modifyPos(int index, int pos)
	{
		coreInfoPtr->events[index].position = pos;
		coreInfoPtr->events[index].time = EDITUTILITY.posToTime(pos);

		return adjustEventOrder(index);
	}
	void DivaEditorMapData::resourceEvent_modifyResource(int index, std::string resourceID)
	{
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
		if(index>=0&&index<coreInfoPtr->events.size())
			coreInfoPtr->events.erase(MAP_INFO->events.begin()+index);
	}

}