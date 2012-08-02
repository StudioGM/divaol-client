#include "divaeditor/DivaEditorMapData.h"

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
	int DivaEditorMapData::getStopLength(float pos)
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
	int DivaEditorMapData::getGridLevel(float nowGrid)
	{
		int nowGridI = nowGrid;
		int nowGrade = GridPerBeat;
		int gridInPeriod = getGridInPeriod(nowGridI);
		if(gridInPeriod==0)return 1;
		int nowLevel = 2;
		while(nowGridI%nowGrade!=0)
		{
			nowLevel++;
			nowGridI %= nowGrade;
			nowGrade>>=1;
		}
		return nowLevel;
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


	void DivaEditorMapData::setStop(float pos, int length)
	{
		stopLength[pos]=length;
	}
	void DivaEditorMapData::setBeatNumChanged(float pos, int beatNum)
	{
		beatNumChanged[getPeriodfromGrid(pos)]=beatNum;
	}
}