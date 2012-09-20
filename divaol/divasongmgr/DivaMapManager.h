#ifndef DivaMapManager_H
#define DivaMapManager_H

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Lib/Base/Thread/Queue.h"
#include "Lib/curl/curl.h"

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
		enum DIVAMAPMGREVENT {
			PrepareMapList, 
			PrepareThumbFile, 
			PrepareAudioPreviewFile, 
			PrepareMapDataFile, 
			PrepareMapDataFileNoVideo, 
			UnpackMapDataFile, 
			PrepareCheckMapDataFile, 
			MapEventERROR
		};
		
		DivaMapEventMessage()
		{
			eventType = MapEventERROR;
			error = true;
			finish = false;
			downloadProgress = 0;
		}
		DivaMapEventMessage(DIVAMAPMGREVENT eventType, int mapID, bool error, bool finish, float downloadProgess)
		{
			this->eventType = eventType;
			effectedMapID = mapID;
			this->error = error;
			this->finish = finish;
			this->downloadProgress = downloadProgess;
		}


		DIVAMAPMGREVENT eventType;
		int effectedMapID;
		bool error, finish;
		float downloadProgress;
	};

	//Use this as param to quest download
	class DivaMapManagerDownloadQuest
	{
	public:
		DivaMapManagerDownloadQuest(Base::String sourceAddress, Base::String localFileAddress, int mapID, DivaMapEventMessage::DIVAMAPMGREVENT eventType)
		{
			this->sourceAddress = sourceAddress;
			this->localFileAddress = localFileAddress;
			this->mapID = mapID;
			this->eventType = eventType;
			this->failed = false;
			curlHandle=NULL;
		}

		Base::String sourceAddress;
		Base::String localFileAddress;

		CURL *curlHandle;

		int mapID;
		DivaMapEventMessage::DIVAMAPMGREVENT eventType;

		bool failed;
	};



	class DivaMapHeader
	{
	public:
		enum MapType {Normal, Couple};
		//map info
		MapType mapType;
		std::wstring name;
		std::vector<std::wstring> noters;
		std::vector<std::wstring> alias;
		std::vector<std::wstring> composers;
		std::vector<std::wstring> lyricists;
		std::vector<std::wstring> artists;
		std::vector<std::wstring> vocaloids;
		int bpm;
		std::wstring additionalMessage;
		int playedCount;
		int songLength;
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
		std::wstring downloadCategoryServerAddress;

	private:
		DivaMapManager();
		~DivaMapManager();

		std::wstring lastUpdatedDate;
		std::map<int, DivaMap> maps;

		//threadQueue is using for fileDownloadDetection
		std::map<int, std::map<DivaMapEventMessage::DIVAMAPMGREVENT, bool>> isOperating;
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
		Base::ThreadSafe::Queue<DivaMapEventMessage>& GetMessageQueue() {return threadQueue;}

	private:
		bool PrepareDirectFile(int id, DivaMapEventMessage::DIVAMAPMGREVENT eventType);

	public:
		//Update function
		void update(float dt);

		//Prepare Functions
		void PrepareDivaMapListInfo();

		bool PrepareDivaMapThumb(int id);
		bool PrepareDivaMapAudioPreview(int id);

		bool PrepareDivaMapData(int id, bool novideo=false);
		bool PrepareCheckLocalMapDataFileLeagal(int id);
		bool PrepareDivaMapDataFromFile(std::wstring divaolpackFile);


		//Get functions
		std::wstring GetMapName(int id);
		std::wstring GetMapDirectory(int id);
		std::wstring GetThumbFilePath(int id);
		std::wstring GetAudioPreviewFilePath(int id);
		std::wstring GetDivaOLFilePath(int id, DivaMap::LevelType level);
		bool isMapIdLeagal(int id);
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

typedef std::map<divamap::DivaMap::LevelType, divamap::DivaMapLevel>::iterator MAPLEVELITERATOR;

#define MAPMGR divamap::DivaMapManager::instance()
#define MAPS MAPMGR.GetMapList()
#define SELECTEDMAPS MAPMGR.GetSelectedMaps()

#endif