#ifndef DIVAEDITORCOMMON_H
#define DIVAEDITORCOMMON_H


#include "divaeditor/Core/DivaEditorOperate.h"
#include "divaeditor/Component/DivaEditorStandardOperation.h"
#include "divaeditor/Core/DivaEditorLocalization.h"

#include <string>
#include <vector>

#include < windows.h >

namespace divaeditor
{


	static const int gridToShowPerBeatTable[6] = {1,2,3,4,6,8};
	static const int gridToShowPerBeatTableCount = 6;

	static const std::wstring divaprojectDescription = L"Diva Map Project File";
	static const std::wstring divaprojectExtensions = L"*.divaolproject;*.divaproject";

	static const std::wstring divaplayFileDescription = L"Diva Map File";
	static const std::wstring divaplayFileExtensions = L"*.divaol;*.diva";

	static const std::wstring bmsFileDescription = L"Beat Mania File";
	static const std::wstring bmsFileExtensions = L"*.bms";

	static const std::wstring osuFileDescription = L"OSU! File";
	static const std::wstring osuFileExtensions = L"*.osu";

	static const std::wstring midiFileDescription = L"MIDI File";
	static const std::wstring midiFileExtensions = L"*.mid";

	static const std::wstring videoExtensions = L"*.avi;*.mp4;*.flv;*.mkv";
	static const std::wstring imageExtensions = L"*.png;*.jpg";

	static const std::wstring audioDescription = L"Music File";
	static const std::wstring audioExtensions = L"*.wav;*.mp3;*.ogg";

	static const std::string EditorVer = "2.0.0.0";

	class EditorConfig
	{
	public:
		enum NOTESTATE {NORMAL,LONG,COMBO} EDITSTATE_NOTESTATE;

	public:
		


		EditorConfig():beatNumberPerScreen(20),
						gridToShowPerBeat(0),
						showRangeFactor(1),
						EDITSTATE_NOTESTATE(NOTESTATE::NORMAL),
						isctrl(false),
						isshift(false),
						display_note(true),
						display_grid(true),
						display_background(true),
						map_initialized(false),
						operationHistoryIndex(0),
						isMapChanged(false),
						lockOperation(false),
						config_moveNoteAsWell(true),
						lastSavedTimeDelta(0),
						AutoSaveTime(120),
						isAutoSave(true)
						{};

		static EditorConfig* Ptr;
		static EditorConfig* instance() {static EditorConfig config; return (Ptr=&config);}

		//Timeline Showing Settings
		void setBeatNumberPerScreen(int numberPerScreen){beatNumberPerScreen=numberPerScreen;}
		const int getBeatNumberPerScreen() const{return beatNumberPerScreen;}

		void increaseGridToShowPerBeat(){if(gridToShowPerBeat<gridToShowPerBeatTableCount-1)gridToShowPerBeat++;}
		void decreaseGridToShowPerBeat(){if(gridToShowPerBeat>0)gridToShowPerBeat--;}
		void setGridToShowPerBeat(int gridToShowPerBeat){this->gridToShowPerBeat=gridToShowPerBeat;}
		const int getGridToShowPerBeatOrigin() const{return gridToShowPerBeat;}
		const int getGridToShowPerBeat() const{return gridToShowPerBeatTable[gridToShowPerBeat];}

		void increaseShowRangeScale(){if(showRangeFactor<0.1)showRangeFactor*=2.0f;else if(showRangeFactor<2)showRangeFactor+=0.1;else if(showRangeFactor<5)showRangeFactor+=0.25;}
		void decreaseShowRangeScale(){if(showRangeFactor>2)showRangeFactor-=0.25;else if(showRangeFactor>0.1)showRangeFactor-=0.1;else if(showRangeFactor>0.05)showRangeFactor/=2.0f;}
		const float getShowRangeScale() const{return showRangeFactor;}
		void setShowRangeScale(float range) {showRangeFactor = range;}


		//NOTE Edit Settings
		void ChangeEditState();


		void noteIndexChanged(int oldIndex, int newIndex);
		void noteIndexSwaped(int indexL,int indexR);
		void addSelectedNote(int index);
		void deleteSelectedNote(int index);
		void clearSelectedNote();
		bool isNoteSelected(int index);
		std::vector<int> noteSelected;


		static const int GridPerBeat = 48;

		//Use for note area
		static const int NoteAreaX = 174;
		static const int NoteAreaY = 154;
		static const int NoteAreaGridSize = 32;
		static const int NoteAreaWidth = 48;
		static const int NoteAreaHeight = 24;
		static const int NoteAreaTailAreaSize = 0;

		
		
		//Global Key Event
		bool isctrl;
		bool isshift;


		//Display State
		bool display_background;
		bool display_note;
		bool display_grid;
		bool config_moveNoteAsWell;

		//If map is initialized
		bool map_initialized;
		float lastSavedTimeDelta;
		float AutoSaveTime;
		bool isAutoSave;

		//Operation System
		bool lockOperation;
		std::string lockOperationID;

		void LockOperation(std::string operationID);
		void UnlockOperation();

		std::vector<DivaEditorOperation*> operationHistory;

		void undoTo(int toHistoryIndex = -1);
		void redoTo(int toHistoryIndex = -1);

		void addAndDoOperation(DivaEditorOperation* operation, std::string operationID = "");
		void mergeLastTwoOperation();

		bool isMapChanged;

	private:
		//Timeline Showing settings
		int beatNumberPerScreen;
		int gridToShowPerBeat;
		float showRangeFactor;

		int operationHistoryIndex;

		
	};


	//Utility Functions
	std::string fTos(float f,int bit);
	std::wstring iToWS(int n);
	std::wstring fTows(float f,int bit);
	std::wstring dTows(double d,int bit);
	std::wstring secondToTimeWstr(float second);
	float wsTof(std::wstring &s);
	double wsTod(std::wstring &s);
	double round(double r); 
	void tolower(std::wstring &str);
	std::string to_utf8(const wchar_t* buffer, int len);
	std::string to_utf8(const std::wstring& str);



#define EDITCONFIG divaeditor::EditorConfig::instance()

}





#endif