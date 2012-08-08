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

		

		int findLastOrEqualEventIndex(int pos, std::string type);
		int findNextEventIndex(int pos, std::string type);
		int findStopIndex(float pos);



	public:
		void addNote(float pos, char keyPress, bool arrow);









	public:
		divacore::MapInfoPtr coreInfoPtr;

		DivaEditorMapData();

		void registerMapInfo(divacore::MapInfoPtr registMapInfo);

		///////////////Grid and period operation
		int getGridInPeriod(float grid);
		int getGridFromPeriod(int period);
		int getPeriodfromGrid(float grid);
		int getGridLevel(float nowGrid);
		int getNextStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getPrevStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getNowStandardGrid(float nowGrid, int gridToShowPerBeat);
		int getNearestStandardGrid(float nowGrid, int gridToShowPerBeat);


		///////////////BeatNum operation
		int getBeatNum(float pos);
		std::map<int,int> getBeatNumByRange(float left, float right);

		void beatNum_change(float pos, int beatNum);
		void beatNum_insert(float pos, int beatNum);
		void beatNum_delete(float pos);


		///////////////offset
		int getOffset();
		void offset_set(int offset, bool movePlacedNote);


		///////////////stop operation
		int getStop(float pos);
		std::vector<int> getStopPositionByRange(float left, float right);

		void stop_change(float pos, int length);
		void stop_insert(float pos, int length);
		void stop_delete(float pos);


		///////////////BPM operation
		float getBPM(float pos);

		void bpm_change(float pos, float bpm);
		void bpm_insert(float pos, float bpm);
		void bpm_delete(float pos);


		///////////////Tail Speed Operation
		float getTailSpeed();
		void tailSpeed_change(float speed);


		///////////////Move All Things
		bool moveAll(int left, int right, int delta);
	};


	//cmp functions
	bool cmp_note(const divacore::MapNote &left,const divacore::MapNote &right);
	bool cmp_event(const divacore::MapEvent &left,const divacore::MapEvent &right);

	
}







#endif