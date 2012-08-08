#include "divaeditor/DivaEditor.h"
#include "divaeditor/DivaEditorCommon.h"
#include "divaeditor/DivaEditorMapData.h"
#include "divacore/Component/DivaStandardCoreFlow.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

namespace divaeditor
{
	//using namespace divacore;


	DivaEditorMapData::DivaEditorMapData()
	{
		mapOffset = 0;
		beatNumChanged[0]=4;
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
		std::sort(coreInfoPtr->notes.begin(),coreInfoPtr->notes.end(),cmp_note);
	}
	void DivaEditorMapData::sortEvent()
	{
		std::sort(coreInfoPtr->events.begin(),coreInfoPtr->events.end(),cmp_event);
	}
	void DivaEditorMapData::adjustNoteOrder(int index)
	{
		vector<divacore::MapNote>::iterator i = coreInfoPtr->notes.begin()+index;
		while(true)
		{
			if((i+1)!=coreInfoPtr->notes.end() && 
				((i->notePoint[0].position > (i+1)->notePoint[0].position) || 
					(i->notePoint[0].position == (i+1)->notePoint[0].position && i->notePoint[0].type>(i+1)->notePoint[0].type)))
			{
				std::swap(*i,*(i+1));
				i++;
				if((i+1)==coreInfoPtr->notes.end())
					break;
			}
			else if(i!=coreInfoPtr->notes.begin() && 
				((i->notePoint[0].position < (i-1)->notePoint[0].position) || 
				(i->notePoint[0].position == (i-1)->notePoint[0].position && i->notePoint[0].type<(i-1)->notePoint[0].type)))
			{
				std::swap(*i,*(i-1));
				i--;
				if(i==coreInfoPtr->notes.begin())
					break;
			}
			else
				break;
		}
	}
	void DivaEditorMapData::adjustEventOrder(int index)
	{
		vector<divacore::MapEvent>::iterator i = coreInfoPtr->events.begin()+index;
		while(true)
		{
			if((i+1)!=coreInfoPtr->events.end() && 
				(i->position > (i+1)->position))
			{
				std::swap(*i,*(i+1));
				i++;
				if((i+1)==coreInfoPtr->events.end())
					break;
			}
			else if(i!=coreInfoPtr->events.begin() &&
				(i->position < (i-1)->position))
			{
				std::swap(*i,*(i-1));
				i--;
				if(i==coreInfoPtr->events.begin())
					break;
			}
			else
				break;
		}
	}




	/////////Note Operations
	void DivaEditorMapData::addNote(float pos, char keyPress, bool arrow)
	{
		//divacore::MapNote mapNote;
		//mapNote.
		//EDITUTILITY.addNote();
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


	
}