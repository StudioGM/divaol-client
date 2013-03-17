#ifndef DivaEditorMapData_H
#define DivaEditorMapData_H

#include "divaeditor/Core/DivaEditor.h"

#include "divacore/core/DivaMapInfo.h"

#include "guichan.hpp"

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

		std::wstring workingDirectory, workingDivaOLFile, workingDivaOLProFile;

		std::vector<divacore::MapNote*> selected;

		int mapOffset;

		static const int MAXPERIOD = 1000000;

	public:
		std::vector<divacore::MapNote> copyBoard;

		void copy(bool cut);
		void paste(float pos);

		void setPos(float pos);
		void PauseAndResume();
		void stop();

	public:

		void ResetEditorMapData();

		int decodeOriginalGrid(int grid);
		int encodeToOriginalGrid(int grid);

		void qsort_Note(int l,int r);
		void sortEvent();
		int adjustNoteOrder(int index);
		int adjustEventOrder(int index);
		
		

		int findLastOrEqualEventIndex(int pos, std::string type);
		int findNextEventIndex(int pos, std::string type);
		int findStopIndex(float pos);

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

		int getCrossAStandardBeatPos(float lastPos, float nowPos, int useOffset=-65535);

	public:
		//File functions
		enum InitSourceFileType {
			DivaPCFile, 
			DivaPCProFile,
			DivaOLFile, 
			DivaOLProFile, 
			MusicFile, 
			BMSFile,
			OSUFile,
			MIDIFile,
			Unknown
								};


		InitSourceFileType parseInitFileType(std::wstring path);
		std::wstring InitFromFile(std::wstring path);
		std::wstring InitFromDivaOLFile(std::wstring path);
		std::wstring InitFromDivaOLProFile(std::wstring path);
		std::wstring InitFromMusicFile(std::wstring path);
		std::wstring InitFromDivaPCFile(std::wstring path);
		std::wstring InitFromDivaPCProFile(std::wstring path);
		std::wstring InitFromBMSFile(std::wstring path){return L"Function not ready.";}
		std::wstring InitFromOSUFile(std::wstring path){return L"Function not ready.";}
		std::wstring InitFromMIDIFile(std::wstring path){return L"Function not ready.";}

		std::wstring ChooseWorkingFile(bool needToCreateNewDirectory = true);
		bool SaveFile(bool olFile=true, bool proFile=true, divacore::MapInfo *infoToSave=NULL);




	public:
		//Resource operations

		std::string findResourceTypeStrByID(std::string id);
		std::wstring getResourceDescriptionByIndex(int index);
		std::wstring getResourceDescription(std::string id);
		std::string findResourceIDByIndex(int index);
		std::string findResourceIDByTypeAndIndex(divacore::MapResourceInfo::ResourceType type, int index);
		int findResourceIndexByID(std::string id);
		std::string resource_add(std::wstring filename, bool onlyAddInfo = false);
		void resource_delete(std::string id);
		
		void modifyGlobalHitMissSound(std::wstring filename, std::string type);

		int findResourceEventIndexByIndexInResource(int index, std::string resourceID);
		int resourceEvent_add(int pos, std::string resourceID);
		int resourceEvent_modifyPos(int index, int pos);
		void resourceEvent_modifyResource(int index, std::string resourceID);
		void resourceEvent_delete(int index);

		void resourceDescription_modify(std::string resourceID, std::wstring description);

		bool checkResourceCanModify(std::string resourceID);

	public:
		//Note Operations
		divacore::MapNote initNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, std::string noteType, int key=-1);
		void finishLongNote(divacore::MapNote &longNote, int pos, int key=-1);
		void finishComboNote(divacore::MapNote &comboNote, int pos, int key=-1);
		void note_modifyTail(int index, int tailX, int tailY);
		void note_modifyPos(int index, int x, int y, bool isDelta);
		void note_modifyTimePos(int index, int pos, bool isDelta);
		void note_modifySecondTimePos(int index, int pos, bool isDelta);
		void note_modifyType(int index, char keyPress, bool arrow);
		void note_modifyTypeByType(int index, int type, bool delta, bool needDecode=false);
		void note_modifyKey(int index, std::string key);
		void note_delete(int index);

		int addNormalNote(divacore::MapNote mapNote);
		int addNormalNote(int pos, char keyPress, bool arrow, int x, int y, int tailX, int tailY, int key=-1);
		int addLongNote(divacore::MapNote longNote);

		int checkNoteExists(int pos, int pressType, std::string noteType);

		int findFirstBiggerPositionNoteIndex(int pos);
		void findNoteIndexInRange(int leftPos, int rightPos, int &beginIndex, int &endIndex);
		int findNoteToSelectByPos(int position,int x,int y);
		int findNoteIndexByType(int position, int type, int singleDeltaNum=0,std::string specificType = "");
		std::vector<int> findNoteToSelectByRange(int position,int leftUpX,int leftUpY,int rightDownX,int rightDownY);

		gcn::Rectangle findSelectedAreaRectange();

		int getNoteTypeFromKeyPress(char keyPress, bool arrow);
		int getNoteTypeIndexFromNoteType(int noteType);
		int getNoteTypeFromNoteTypeIndex(int noteTypeIndex);

		bool isNoteConflict(divacore::MapNote &note, bool ignoreSelected);

		void sortNote();

		void guessThisNotePositionByLastTwo(int pos, int& out_x,int &out_y,int &tail_x,int &tail_y);

	public:
		/////////Event modify
		void event_modifyTimePos(int index, int pos);
		int findEvent(divacore::MapEvent &event);

		///////////////BeatNum operation
		int getBeatNum(float pos);
		int getBeatNumPeriod(int period);
		bool beatNumExist(int period);
		std::map<int,int> getBeatNumByRange(float left, float right);

		void beatNum_change(int period, int beatNum);
		void beatNum_delete(int period);


		///////////////offset
		int getOffset();
		void offset_set(int offset, bool movePlacedNote);


		///////////////stop operation
		int getStop(float pos);
		int getStopPos(float pos);
		std::vector<int> getStopPositionByRange(float left, float right);

		void stop_change(float pos, int length);
		void stop_insert(float pos, int length);
		void stop_delete(float pos);


		///////////////BPM operation
		double getBPM(float pos);
		int getBPMPos(float pos);
		int getNextBPMPos(float pos);
		int getPrevBPMIndex(float pos);

		void bpm_change(float pos, double bpm);
		void bpm_insert(float pos, double bpm);
		void bpm_insert(int pos, double bpm);
		void bpm_delete(float pos);


		///////////////Tail Speed Operation
		float getTailSpeed();
		void tailSpeed_change(float speed);


		///////////////Move All Things
		bool moveAll(int left, int right, int delta);


		bool modifySelectedNotesPos(int posDelta, std::string operationID = "");
		bool modifySelectedNotesType(int typeDelta, std::string operationID = "");
	};


	//cmp functions
	bool cmp_note(const divacore::MapNote &left,const divacore::MapNote &right);
	bool cmp_event(const divacore::MapEvent &left,const divacore::MapEvent &right);

	
}







#endif