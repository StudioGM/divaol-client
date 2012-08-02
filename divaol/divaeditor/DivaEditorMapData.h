#ifndef DivaEditorMapData_H
#define DivaEditorMapData_H

#include "divaeditor/DivaEditor.h"
#include "divacore/core/DivaMapInfo.h "
#include <map>
#include <vector>

namespace divaeditor
{


	class DivaEditorMapData
	{
	private:
		std::map<int,int> stopLength;
		std::map<int,int> beatNumChanged;

		std::vector<divacore::MapNote*> selected;
		std::vector<divacore::MapNote*> copyBoard;

		divacore::MapInfoPtr coreInfoPtr;
		int mapOffset;

		int decodeOriginalGrid(int grid);
		int encodeToOriginalGrid(int grid);

	public:

		static const int GridPerBeat = 48;

		DivaEditorMapData();

		void registerMapInfo(divacore::MapInfoPtr registMapInfo);

		//Get Properties
		inline int getMapOffset() {return mapOffset;}
		int getStopLength(float pos);
		int getBeatNumFromGrid(float pos);


		//Get Calculate Data
		int getGridInPeriod(float grid);
		int getPeriodfromGrid(float grid);
		int getNextStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getPrevStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getNowStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getGridLevel(float nowGrid);
		std::vector<int> getStopPositionByRange(float left, float right);

		//Sets
		inline void setMapOffset(int offset) {mapOffset = offset;}
		void setStop(float pos, int length);
		void setBeatNumChanged(float pos, int beatNum);
	};



}







#endif