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
		beatNumChanged[-10000]=4;
	}
	void DivaEditorMapData::registerMapInfo(divacore::MapInfoPtr registMapInfo)
	{
		coreInfoPtr = registMapInfo;
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
				(i->position < (i+1)->position))
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


	//Grid and period operation
	int DivaEditorMapData::decodeOriginalGrid(int grid)
	{
		std::map<int,int>::iterator stopI = stopLength.begin();
		int stopGridCount=0;

		while(stopI!=stopLength.end() && stopI->first <= grid)
		{
			if(stopI->first+stopI->second > grid)
				stopGridCount += grid-stopI->first;
			else
				stopGridCount += stopI->second;
			stopI++;
		}

		return grid-stopGridCount-mapOffset;
	}
	int DivaEditorMapData::encodeToOriginalGrid(int grid)
	{
		std::map<int,int>::iterator stopI = stopLength.begin();
		grid+=mapOffset;
		
		while(stopI!=stopLength.end() && stopI->first <= grid)
		{
			grid+=stopI->second;
			stopI++;
		}

		return grid;
	}
	int DivaEditorMapData::getGridInPeriod(float grid)
	{
		int nowGrid = decodeOriginalGrid(grid);

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			int thisBeatPos = beatI->first;
			int thisBeatNum = beatI->second;
			//std::map<int,int>::iterator thisBeatI = beatI;
			int periodCount = 10000;
			beatI++;
			if(beatI!=beatNumChanged.end())
				periodCount = beatI->first-thisBeatPos;

			if(nowGrid >= periodCount*thisBeatNum* GridPerBeat)
				nowGrid -= periodCount*thisBeatNum* GridPerBeat;
			else
			{
				nowGrid %= thisBeatNum * GridPerBeat;
				break;
			}
		}
		return nowGrid;
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
			if(beatI!=beatNumChanged.end())
			{
				beatI++;
				periodCount = beatI->first-thisBeatI->first;
			}

			if(nowGrid >= periodCount*thisBeatI->second)
			{
				nowPeriod += periodCount;
				nowGrid -= periodCount*thisBeatI->second;
			}
			else
			{
				nowPeriod += nowGrid/thisBeatI->second;
				nowGrid %= thisBeatI->second;
				break;
			}
		}
		return nowPeriod;
	}
	int DivaEditorMapData::getGridLevel(float nowGrid)
	{
		int nowGridI = nowGrid;
		int nowGrade = GridPerBeat;
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
		float deltaGrid = GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid)+1) * deltaGrid);
	}
	int DivaEditorMapData::getPrevStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid) - 1) * deltaGrid);
	}
	int DivaEditorMapData::getNowStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((ceil(decodeOriginalGrid(ceil(nowGrid))/deltaGrid)) * deltaGrid);
	}
	int DivaEditorMapData::getNearestStandardGrid(float nowGrid, int gridToShowPerBeat)
	{
		float deltaGrid = GridPerBeat/gridToShowPerBeat;
		return encodeToOriginalGrid((round(decodeOriginalGrid(round(nowGrid))/deltaGrid)) * deltaGrid);
	}


	//BeatNum operation
	int DivaEditorMapData::getBeatNumFromGrid(float pos)
	{
		int nowGrid = decodeOriginalGrid(pos);

		std::map<int,int>::iterator beatI = beatNumChanged.begin();

		while(true)
		{
			std::map<int,int>::iterator thisBeatI = beatI;
			int periodCount = 10000;
			if(beatI!=beatNumChanged.end())
			{
				beatI++;
				periodCount = beatI->first-thisBeatI->first;
			}

			if(nowGrid >= periodCount*thisBeatI->second)
				nowGrid -= periodCount*thisBeatI->second;
			else
				return thisBeatI->second;
		}
	}
	void DivaEditorMapData::setBeatNumChanged(float pos, int beatNum)
	{
		beatNumChanged[getPeriodfromGrid(pos)]=beatNum;
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
		{
			//Check border	
			int deltaOffset = mapOffset - lastOffset;
			int minPos = MAXINT32,maxPos=-1;
			for(std::vector<divacore::MapNote>::iterator i=coreInfoPtr->notes.begin();i!=coreInfoPtr->notes.end();i++)
				for(std::vector<divacore::NotePoint>::iterator j=i->notePoint.begin();j!=i->notePoint.end();j++)
				{
					if((int)j->position>maxPos)
						maxPos = j->position;
					if((int)j->position<minPos)
						minPos = j->position;
				}

			for(std::vector<divacore::MapEvent>::iterator i=coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
			{
				if(i->eventType != "bpm" || (i->eventType == "bpm" && i->position!=0))
				{
					if((int)i->position>maxPos)
						maxPos = i->position;
					if((int)i->position<minPos)
						minPos = i->position;
				}
			}

			//Move All
			if(minPos+deltaOffset>=0 && minPos+deltaOffset<CORE_FLOW_PTR->getTotalPosition()
				&& maxPos+deltaOffset>=0 && maxPos+deltaOffset<CORE_FLOW_PTR->getTotalPosition())
			{
				for(std::vector<divacore::MapNote>::iterator i=coreInfoPtr->notes.begin();i!=coreInfoPtr->notes.end();i++)
					for(std::vector<divacore::NotePoint>::iterator j=i->notePoint.begin();j!=i->notePoint.end();j++)
						j->position += deltaOffset;

				for(std::vector<divacore::MapEvent>::iterator i=coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
					if(i->eventType != "bpm" || (i->eventType == "bpm" && i->position!=0))
						i->position += deltaOffset;

				EDITUTILITY.reCaltTime();
			}
			else
				mapOffset = lastOffset;
		}
	}


	//stop operations
	int DivaEditorMapData::getStop(float pos)
	{
		map<int,int>::iterator stopI = stopLength.begin();
		while(stopI!=stopLength.end() && pos>stopI->first+stopI->second)
			stopI++;
		if(stopI!=stopLength.end())
			if(pos>=stopI->first && pos<stopI->first+stopI->second)
				return stopI->second;
			else
				return 0;
		else
			return 0;
	}
	vector<int> DivaEditorMapData::getStopPositionByRange(float left, float right)
	{
		vector<int> ret;
		for (std::map<int,int>::iterator i = stopLength.begin();i!=stopLength.end();i++)
		{
			if((i->first>=left && i->first<=right) || ((i->first+i->second)>=left && (i->first+i->second)<=right))
			{
				ret.push_back(i->first);
				ret.push_back(i->first+i->second);
			}
		}
		return ret;
	}

	void DivaEditorMapData::stop_change(float pos, int length)
	{

	}
	void DivaEditorMapData::stop_insert(float pos, int length)
	{

	}
	void DivaEditorMapData::stop_delete(float pos, int length)
	{

	}


	//BPM
	float DivaEditorMapData::getBPM(float pos)
	{
		float ret = -1;
		for (std::vector<divacore::MapEvent>::iterator i = coreInfoPtr->events.begin();i!=coreInfoPtr->events.end();i++)
			if(i->eventType=="bpm" && pos>=i->position)
				ret = divacore::Argument::asFloat("value",i->arg);
			else if(pos<i->position)
				break;
		return ret;
	}
	
	void DivaEditorMapData::bpm_change(float pos, float bpm)
	{
		int nowBPMIndex=-1;
		std::vector<divacore::MapEvent>::iterator it = coreInfoPtr->events.begin();
		for(int i=0;i<coreInfoPtr->events.size();i++,it++)
			if(it->eventType=="bpm" && pos>=it->position)
				nowBPMIndex = i;
			else if(pos<it->position)
				break;
		coreInfoPtr->events[nowBPMIndex].arg["value"] = sora::SoraAny((double)bpm);
		adjustEventOrder(nowBPMIndex);
		EDITUTILITY.reCaltTime();
	}

	void DivaEditorMapData::bpm_insert(float pos, float bpm)
	{
		//Get Nearest Standard Grid
		getNearestStandardGrid(pos, EDITCONFIG->getGridToShowPerBeat());
	}

	void DivaEditorMapData::bpm_delete(float pos)
	{

	}





	
}