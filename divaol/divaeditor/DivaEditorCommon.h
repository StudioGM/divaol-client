#ifndef DIVAEDITORCOMMON_H
#define DIVAEDITORCOMMON_H

#include <string>

namespace divaeditor
{


	static const int gridToShowPerBeatTable[6] = {1,2,3,4,6,8};
	static const int gridToShowPerBeatTableCount = 6;

	class EditorConfig
	{
	public:

		EditorConfig():beatNumberPerScreen(20),
						gridToShowPerBeat(0),
						showRangeFactor(1)
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

		static const int GridPerBeat = 48;

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



#define EDITCONFIG divaeditor::EditorConfig::instance()

}





#endif