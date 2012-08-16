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
	public:
		divacore::MapInfoPtr coreInfoPtr;

		DivaEditorMapData();

		void registerMapInfo(divacore::MapInfoPtr registMapInfo);

	private:
		std::map<int,int> stopLength;
		std::map<int,int> beatNumChanged;

		std::map<std::string,std::wstring> resourceDescription;

		std::wstring workingDirectory;

		std::vector<divacore::MapNote*> selected;
		std::vector<divacore::MapNote*> copyBoard;

		int mapOffset;

		int decodeOriginalGrid(int grid);
		int encodeToOriginalGrid(int grid);

		void qsort_Note(int l,int r);
		void sortNote();
		void sortEvent();
		int adjustNoteOrder(int index);
		int adjustEventOrder(int index);

		

		int findLastOrEqualEventIndex(int pos, std::string type);
		int findNextEventIndex(int pos, std::string type);
		int findStopIndex(float pos);


		

	public:
		//Resource operations

		std::string findResourceTypeStrByID(std::string id);
		std::wstring getResourceDescriptionByIndex(int index);
		std::string findResourceIDByIndex(int index);
		int findResourceIndexByID(std::string id);
		std::string resource_add(std::wstring filename);
		void resource_delete(std::string id);
		


		int resourceEvent_add(int pos, std::string resourceID);
		int resourceEvent_modifyPos(int index, int pos);
		void resourceEvent_modifyResource(int index, std::string resourceID);
		void resourceEvent_delete(int index);


	public:
		//Note Operations
		divacore::MapNote initNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, std::string noteType, int key=-1);
		void finishLongNote(divacore::MapNote &longNote, int pos, int key=-1);
		void finishComboNote(divacore::MapNote &comboNote, int pos, int key=-1);
		void note_modifyTail(int index, int tailX, int tailY);
		void note_modifyPos(int index, int x, int y, bool isDelta);
		void note_modifyTimePos(int index, int pos, bool isDelta);
		void note_modifyType(int index, char keyPress, bool arrow);
		void note_modifyKey(int index, int key);
		void note_delete(int index);

		int addNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key=-1);
		int addLongNote(divacore::MapNote longNote);

		int checkNoteExists(int pos, int pressType, std::string noteType);

		int findNoteToSelectByPos(int position,int x,int y);
		std::vector<int> findNoteToSelectByRange(int position,int leftUpX,int leftUpY,int rightDownX,int rightDownY);

		gcn::Rectangle findSelectedAreaRectange();

		int getNoteTypeFromKeyPress(char keyPress, bool arrow);

	public:

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