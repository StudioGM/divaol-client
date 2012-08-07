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

		int mapOffset;

		int decodeOriginalGrid(int grid);
		int encodeToOriginalGrid(int grid);

		void sortNote();
		void sortEvent();
		void adjustNoteOrder(int index);
		void adjustEventOrder(int index);

	public:
		divacore::MapInfoPtr coreInfoPtr;

		static const int GridPerBeat = 48;

		DivaEditorMapData();

		void registerMapInfo(divacore::MapInfoPtr registMapInfo);

		//Grid and period operation
		int getGridInPeriod(float grid);
		int getPeriodfromGrid(float grid);
		int getGridLevel(float nowGrid);
		int getNextStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getPrevStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getNowStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getNearestStandardGrid(float nowGrid, int gridToShowPerBeat);


		//BeatNum operation
		int getBeatNumFromGrid(float pos);
		void setBeatNumChanged(float pos, int beatNum);
		

		//offset
		int getOffset();
		void offset_set(int offset, bool movePlacedNote);


		//stop operation
		int getStop(float pos);
		std::vector<int> getStopPositionByRange(float left, float right);

		void stop_change(float pos, int length);
		void stop_insert(float pos, int length);
		void stop_delete(float pos, int length);


		//BPM operation
		float getBPM(float pos);

		void bpm_change(float pos, float bpm);
		void bpm_insert(float pos, float bpm);
		void bpm_delete(float pos);
	};


	//cmp functions
	bool cmp_note(const divacore::MapNote &left,const divacore::MapNote &right);
	bool cmp_event(const divacore::MapEvent &left,const divacore::MapEvent &right);

	
}







#endif