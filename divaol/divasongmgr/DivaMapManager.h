#ifndef DivaMapManager_H
#define DivaMapManager_H

#include <vector>
#include <map>
#include <list>
#include <string>

#include "divacore/Lib/Base/Thread/Queue.h"

namespace divamap
{
	class DivaMapEventMessage;
	class DivaMapLevel;
	class DivaMapHeader;
	class DivaMap;
	class DivaMapManager;


	class DivaMapEventMessage
	{
	public:
		enum DIVAMAPMGREVENT {PrepareSongList, PrepareThumbFile, PrepareAudioPreviewFile, PrepareSongDataFile};
		std::vector<int> effectedMapID;
		bool error, finish;
		float downloadProgress;
	};
	class DivaMapHeader
	{
	public:
		//map info
		std::wstring name;
		std::vector<std::wstring> noters;
		std::vector<std::wstring> alias;
		std::vector<std::wstring> composers;
		std::vector<std::wstring> lyricists;
		std::vector<std::wstring> artists;
		std::vector<std::wstring> vocaloids;
		std::wstring additionalMessage;
		int playedCount;
		std::wstring thumb;
		std::wstring audioPreview;
	};
	class DivaMapLevel
	{
	public:
		std::wstring divaFileName;
		std::wstring FileMD5Value;
		int difficualty;
	};
	class DivaMap
	{
	public:
		enum LevelType {
			Easy = 0,
			Normal = 1,
			Hard = 2,
			Extra = 3,
			Die = 4};

		int id;
		DivaMapHeader header;

	public:
		std::map<LevelType, DivaMapLevel> levels;
	};


	class DivaMapSelectedItem
	{
	public:
		DivaMapSelectedItem(int id, DivaMap::LevelType level) {this->id = id;this->level=level;}

		int id;
		DivaMap::LevelType level;
	};

	class DivaMapManager
	{
	private:
		DivaMapManager();

		std::wstring downloadCategoryServerAddress;

		std::wstring lastUpdatedDate;
		std::map<int, DivaMap> maps;

		//threadQueue is using for fileDownloadDetection
		Base::ThreadSafe::Queue<DivaMapEventMessage> threadQueue;
		
		//listMsgOut is using for output event over
		std::list<DivaMapEventMessage> *listMsgOut;

		std::vector<DivaMapSelectedItem> selectedMaps;

	public:
		//local song list file management
		bool initFromLocalFile();
		void saveToLocalFile();

	public:
		static DivaMapManager& instance() {static DivaMapManager instance; return instance;}

		void registerMapEventMessageQueue(std::list<DivaMapEventMessage> *listMsg) {listMsgOut = listMsg;}

	public:
		//Update function
		void update(float dt);

		//Prepare Functions
		void PrepareDivaMapListInfo();

		bool PrepareDivaMapThumb(int id);
		bool PrepareDivaMapAudioPreview(int id);

		bool PrepareDivaMapData(int id);
		bool PrepareDivaMapDataFromFile(std::wstring zippedFile);

		//Check file functions
		bool CheckLocalMapDataFileLeagal(int id);
		bool CheckLocalThumbFile(int id);
		bool CheckLocalAudioPreviewFile(int id);

		//Get functions
		std::wstring GetMapName(int id);
		std::map<int, DivaMap>& GetMapList(){return maps;}

	public:
		//Select Map functions
		void SelectedMap_Add(int id, DivaMap::LevelType level);
		void SelectedMap_Remove(int index);
		void SelectedMap_ChangeLevel(int index, DivaMap::LevelType level);
		void SelectedMap_Swap(int indexL,int indexR);
		void SelectedMap_Clear();

		std::vector<DivaMapSelectedItem>& GetSelectedMaps(){return selectedMaps;}
	};

}
#define MAPMGR divamap::DivaMapManager::instance()
#define MAPS MAPMGR.GetMapList()
#define SELECTEDMAPS MAPMGR.GetSelectedMaps()

#endif