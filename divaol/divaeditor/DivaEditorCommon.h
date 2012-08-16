#ifndef DIVAEDITORCOMMON_H
#define DIVAEDITORCOMMON_H

#include <string>
#include <vector>


namespace divaeditor
{


	static const int gridToShowPerBeatTable[6] = {1,2,3,4,6,8};
	static const int gridToShowPerBeatTableCount = 6;


	static const std::wstring videoExtentions = L"avi,mp4,flv";
	static const std::wstring imageExtentions = L"png,jpg";
	static const std::wstring audioExtentions = L"wav,mp3,ogg";

	class EditorConfig
	{
	public:
		enum NOTESTATE {NORMAL,LONG,COMBO} EDITSTATE_NOTESTATE;

	public:

		EditorConfig():beatNumberPerScreen(20),
						gridToShowPerBeat(0),
						showRangeFactor(1),
						EDITSTATE_NOTESTATE(NOTESTATE::NORMAL),
						isctrl(false)
						{};

		static EditorConfig* Ptr;
		static EditorConfig* instance() {static EditorConfig config; return (Ptr=&config);}

		//Timeline Showing Settings
		void setBeatNumberPerScreen(int numberPerScreen){beatNumberPerScreen=numberPerScreen;}
		const int getBeatNumberPerScreen() const{return beatNumberPerScreen;}

		void increaseGridToShowPerBeat(){if(gridToShowPerBeat<gridToShowPerBeatTableCount-1)gridToShowPerBeat++;}
		void decreaseGridToShowPerBeat(){if(gridToShowPerBeat>0)gridToShowPerBeat--;}
		void setGridToShowPerBeat(int gridToShowPerBeat){gridToShowPerBeat=gridToShowPerBeat;}
		const int getGridToShowPerBeat() const{return gridToShowPerBeatTable[gridToShowPerBeat];}

		void increaseShowRangeScale(){if(showRangeFactor<0.1)showRangeFactor*=2.0f;else if(showRangeFactor<2)showRangeFactor+=0.1;else if(showRangeFactor<5)showRangeFactor+=0.25;}
		void decreaseShowRangeScale(){if(showRangeFactor>2)showRangeFactor-=0.25;else if(showRangeFactor>0.1)showRangeFactor-=0.1;else if(showRangeFactor>0.05)showRangeFactor/=2.0f;}
		const float getShowRangeScale() const{return showRangeFactor;}


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
		//static const double NoteAreaFactor = 0.666666666666666666666667;


		//Global Key Event
		bool isctrl;

	private:
		//Timeline Showing settings
		int beatNumberPerScreen;
		int gridToShowPerBeat;
		float showRangeFactor;

		

	};


	//Utility Functions
	std::string fTos(float f,int bit);
	std::wstring iToWS(int n);
	std::wstring fTows(float f,int bit);
	float wsTof(std::wstring &s);
	double round(double r); 
	void tolower(std::wstring &str);



#define EDITCONFIG divaeditor::EditorConfig::instance()

}





#endif