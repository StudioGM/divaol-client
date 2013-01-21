#ifndef DivaMapManager_H
#define DivaMapManager_H

#include <vector>
#include <map>
#include <list>
#include <string>

#include "Lib/Base/Thread/Queue.h"
#include "Lib/wjson/wjson.h"
#include "Lib/curl/curl.h"
#include "SoraAutoUpdate.h"

namespace divamap
{
	#define LocalSongDirectoryW L"song/"

	class DivaMapEventMessage;
	class DivaMapLevel;
	class DivaMapHeader;
	class DivaMap;
	class DivaMapManager;


	class DivaMapEventMessage
	{
	public:
		char *additionalMessage;
		int additionalMessageLength;
		void *extraPTR;

		enum DIVAMAPMGREVENT {
			PrepareMapList, 
			PrepareThumbFile, 
			PrepareAudioPreviewFile, 
			PrepareMapDataFile, 
			PrepareMapDataFileNoVideo, 
			PrepareRecordByRank,
			PrepareRecordByUser,
			UnpackMapDataFile, 
			PrepareCheckMapDataFile, 
			MapEventERROR
		};
		
		DivaMapEventMessage()
		{
			effectedMapLevel = 0;
			eventType = MapEventERROR;
			error = true;
			finish = false;
			downloadProgress = 0;
			additionalMessage = NULL;
			extraPTR = NULL;
			additionalMessageLength=0;
		}
		DivaMapEventMessage(DIVAMAPMGREVENT eventType, int mapID, bool error, bool finish, float downloadProgess)
		{
			this->eventType = eventType;
			effectedMapID = mapID;
			effectedMapLevel = 0;
			this->error = error;
			this->finish = finish;
			this->downloadProgress = downloadProgess;
			additionalMessage = NULL;
			extraPTR = NULL;
			additionalMessageLength=0;
		}
		DivaMapEventMessage(DIVAMAPMGREVENT eventType, int mapID, int level, bool error, bool finish, float downloadProgess)
		{
			this->eventType = eventType;
			effectedMapID = mapID;
			effectedMapLevel = level;
			this->error = error;
			this->finish = finish;
			this->downloadProgress = downloadProgess;
			additionalMessage = NULL;
			extraPTR = NULL;
			additionalMessageLength=0;
		}


		DIVAMAPMGREVENT eventType;
		int effectedMapID;
		int effectedMapLevel;
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
			levelID = 0;
			this->eventType = eventType;
			this->failed = false;
			curlHandle=NULL;
			extraPTR=NULL;
			
		}

		DivaMapManagerDownloadQuest(Base::String sourceAddress, Base::String localFileAddress, int mapID, int levelID, DivaMapEventMessage::DIVAMAPMGREVENT eventType)
		{
			this->sourceAddress = sourceAddress;
			this->localFileAddress = localFileAddress;
			this->mapID = mapID;
			this->levelID = levelID;
			this->eventType = eventType;
			this->failed = false;
			curlHandle=NULL;
			extraPTR=NULL;
		}

		Base::String sourceAddress;
		Base::String localFileAddress;

		CURL *curlHandle;

		int mapID,levelID;
		DivaMapEventMessage::DIVAMAPMGREVENT eventType;

		bool failed;
		void *extraPTR;
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
		int songLength;
		std::wstring thumb;
		std::wstring audioPreview;
		std::wstring modified;
		std::wstring created;
	};
	class DivaMapLevel
	{
	public:
		std::wstring divaFileName;
		std::wstring FileMD5Value;
		int difficulty;
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
		enum ModeType {
			NormalMode = 0,
			RelayMode = 1,
			PairMode = 2
		};

		int id;
		DivaMapHeader header;

		LevelType getLevel(int index) const;
		int getDifIndex(LevelType l) const;
	public:
		typedef std::map<LevelType, DivaMapLevel> Levels;
		Levels levels;
	};


	class DivaMapSelectedItem
	{
	public:
		DivaMapSelectedItem(int id, DivaMap::LevelType level, DivaMap::ModeType mode) {this->id = id;this->level=level;this->mode=mode;}

		int id;
		DivaMap::LevelType level;
		DivaMap::ModeType mode;
	};

	class DivaMapManager : public sora::SoraAutoUpdate
	{
	public:
		enum GameMode {
			PVMode = 0, 
			CTMode = 1,
			AutoMode = 2,
			FastMode = 3,
			SlowMode = 4,
			DeathMode = 5,
			RandomSwapMode = 6,
			ChaosMode = 7,
			DisappearMode = 8,
			BlackHouseMode = 9,
			NoFailMode = 10
		};

		long long int selectedMode;
		std::map<int, std::string> GameModeStr;
		std::map<int, std::map<int, bool>> ModeConflict;


	private:
		std::wstring downloadCategoryQueryAddress;
		std::wstring gameInfoQueryAddress;
		std::wstring GetQueryAddress_DownloadCategory();
		std::wstring GetQueryAddress_MapListUpdateByTime(std::wstring basedTime);
		std::wstring GetQueryAddress_RecordByRank(int mapID, int difficulty, int startRank, int endRank);
		std::wstring GetQueryAddress_RecordByUser(int mapID, int difficulty, std::wstring userID);

	private:
		DivaMapManager();
		~DivaMapManager();

		void registerConflict();

		std::wstring lastUpdatedDate;
		std::map<int, DivaMap> maps;

		//threadQueue is using for fileDownloadDetection
		std::map<int, std::map<int, std::map<DivaMapEventMessage::DIVAMAPMGREVENT, bool>>> isOperating;
		Base::ThreadSafe::Queue<DivaMapEventMessage> threadQueue;
		
		//listMsgOut is using for output event over
		std::list<DivaMapEventMessage> *listMsgOut;

		std::vector<DivaMapSelectedItem> selectedMaps;
		

		std::map<int,float> mapDownloadPercent;

	public:
		//local song list file management
		bool initMapFromJsonArray(WJson::Value mapsJValue);
		bool initFromLocalFile();
		void saveToLocalFile();

	public:
		static DivaMapManager& instance() {static DivaMapManager instance; return instance;}

		std::list<DivaMapEventMessage>* GetQueue() const {return listMsgOut;}
		void registerMapEventMessageQueue(std::list<DivaMapEventMessage> *listMsg) {listMsgOut = listMsg;}
		Base::ThreadSafe::Queue<DivaMapEventMessage>& GetMessageQueue() {return threadQueue;}

	private:
		bool PrepareDirectFile(int id, DivaMapEventMessage::DIVAMAPMGREVENT eventType);

	public:
		// auto update
		virtual void onUpdate(float dt) {update(dt);}

		//Update function
		void update(float dt);

		//Prepare Functions
		bool PrepareDivaMapListInfo();

		bool PrepareDivaMapThumb(int id);
		bool PrepareDivaMapAudioPreview(int id);

		bool PrepareDivaMapData(int id, bool novideo=false);
		bool PrepareDivaMapDataFromFile(std::wstring divaolpackFile);

		bool PrepareRecordByRank(int mapID, int difficulty, int startRank, int endRank, void* extraPTR=NULL);
		bool PrepareRecordByUser(int mapID, int difficulty, std::wstring userID);


		//Get functions
		std::wstring GetModeDescription(int mode);
		std::wstring GetLevelDescription(int level);
		std::wstring GetMapDescription(int id, int level, int mode);
		std::wstring GetMapName(int id);
		std::wstring GetMapDirectory(int id);
		std::wstring GetThumbFilePath(int id);
		std::wstring GetAudioPreviewFilePath(int id);
		std::wstring GetDivaOLFilePath(int id, DivaMap::LevelType level);
		float GetMapDownloadProgress(int id);
		bool isMapDownloaded(int id);
		bool isMapIdLeagal(int id);
		bool isMapLevelExist(int id, DivaMap::LevelType level);
		bool isMapLeagal(int id, DivaMap::LevelType level);
		std::map<int, DivaMap>& GetMapList(){return maps;}

	public:
		//Select Map functions
		void SelectedMap_Add(int id, DivaMap::LevelType level, DivaMap::ModeType mode);
		void SelectedMap_Remove(int index);
		void SelectedMap_ChangeLevel(int index, DivaMap::LevelType level);
		void SelectedMap_Swap(int indexL,int indexR);
		void SelectedMap_Clear();

		std::vector<DivaMapSelectedItem>& GetSelectedMaps()
		{
			return selectedMaps;
		}


		//Select Mode functions
		void SelectedMode_Set(long long int mode);
		void SelectedMode_ToggleMode(GameMode mode, bool select);

		std::vector<GameMode> GetSelectedMode();
		long long int GetSelectedModeInt();
		std::vector<std::string> GetSelectedModeStr();
		bool IsModeSelected(GameMode mode);
	};

}

typedef std::map<divamap::DivaMap::LevelType, divamap::DivaMapLevel>::iterator MAPLEVELITERATOR;
typedef std::map<int, divamap::DivaMap>::iterator MAPITERATOR;

#define MAPMGR divamap::DivaMapManager::instance()
#define MAPS MAPMGR.GetMapList()
#define SELECTEDMAPS MAPMGR.GetSelectedMaps()
#define MAPQUEUE std::list<divamap::DivaMapEventMessage>

#endif