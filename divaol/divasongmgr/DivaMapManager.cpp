#include "DivaMapManager.h"

#include <Process.h>
#include <string>
#include "Lib/Base/Common.h"
#include "Lib/Base/Io/Path.h"
#include "Lib/wjson/wjson.h"
#include "ZLIB/zlib.h"
#include "DivaMapEncryption.h"

#ifdef BASE_OS_WINDOWS
#pragma comment(lib, "libcurl.lib")
#endif

namespace divamap
{

#define LocalSongDirectoryW L"song/"
#define MAXSelectedMapCount 5

	DivaMapManager::DivaMapManager()
	{
		
		listMsgOut = NULL;
		if(!initFromLocalFile())
		{
			lastUpdatedDate = L"1900-09-13 23:59:59";
			maps.clear();
		}

		//Init libcurl
		curl_global_init(CURL_GLOBAL_ALL);

		//For test
		downloadCategoryServerAddress = L"http://divaol.b0.upaiyun.com";
		PrepareDivaMapListInfo();
	}
	DivaMapManager::~DivaMapManager()
	{
		curl_global_cleanup();
	}

	bool DivaMapManager::initFromLocalFile()
	{
		//Read file
		std::wstring jsonStrToParse;

		FILE* readFile;
		if(_wfopen_s(&readFile, (std::wstring(LocalSongDirectoryW)+L"DIVAOLMDB").c_str(),L"r, ccs=UTF-8")!=0)
			return false;
		wchar_t buffer[1000];
		while(fgetws(buffer,sizeof(buffer),readFile))
			jsonStrToParse += std::wstring(buffer);
		fclose(readFile);

		jsonStrToParse = decrypt(jsonStrToParse);

		WJson::Reader reader;
		WJson::Value rootJsonValue;
		if(!reader.parse(jsonStrToParse,rootJsonValue))
			return false;

		if(rootJsonValue.isMember(L"maps"))
		{
			WJson::Value mapsJValue = rootJsonValue[L"maps"];
			if(mapsJValue.isArray() || mapsJValue.isNull())
				for(int mapI=0;mapI<mapsJValue.size();mapI++)
				{
					WJson::Value mapJValue = mapsJValue[mapI];
					DivaMap thisMap;

					if(mapJValue.isMember(L"ID"))
						thisMap.id = mapJValue[L"ID"].asInt();
					else
						return false;

					#pragma region Header
					WJson::Value headerJValue;
					if(mapJValue.isMember(L"header"))
						headerJValue = mapJValue[L"header"];
					else
						return false;

					if(headerJValue.isMember(L"mapType") && headerJValue.isMember(L"name") && headerJValue.isMember(L"thumb")  && headerJValue.isMember(L"audioPreview")  && headerJValue.isMember(L"playedCount")  
						&& headerJValue.isMember(L"additionalMessage") && headerJValue.isMember(L"noters")  && headerJValue.isMember(L"alias")  && headerJValue.isMember(L"composers")
						&& headerJValue.isMember(L"lyricists")  && headerJValue.isMember(L"artists") && headerJValue.isMember(L"vocaloids"))
					{
						thisMap.header.mapType = (DivaMapHeader::MapType)headerJValue[L"mapType"].asInt();
						thisMap.header.name = headerJValue[L"name"].asString();
						thisMap.header.thumb = headerJValue[L"thumb"].asString();
						thisMap.header.audioPreview = headerJValue[L"audioPreview"].asString();
						thisMap.header.playedCount = headerJValue[L"playedCount"].asInt();
						thisMap.header.additionalMessage = headerJValue[L"additionalMessage"].asString();

						WJson::Value notersJValue = headerJValue[L"noters"];
						WJson::Value aliasJValue = headerJValue[L"alias"];
						WJson::Value composersJValue = headerJValue[L"composers"];
						WJson::Value lyricistsJValue = headerJValue[L"lyricists"];
						WJson::Value artistsJValue = headerJValue[L"artists"];
						WJson::Value vocaloidsJValue = headerJValue[L"vocaloids"];

						if((notersJValue.isArray()||notersJValue.isNull())
							&&(aliasJValue.isArray()||aliasJValue.isNull())
							&&(composersJValue.isArray()||composersJValue.isNull())
							&&(lyricistsJValue.isArray()||lyricistsJValue.isNull())
							&&(artistsJValue.isArray()||artistsJValue.isNull())
							&&(vocaloidsJValue.isArray()||vocaloidsJValue.isNull()))
						{
							for (int i=0;i<notersJValue.size();i++)
							{
								WJson::Value noterJValue = notersJValue[i];
								if(noterJValue.isMember(L"noter"))
									thisMap.header.noters.push_back(noterJValue[L"noter"].asString());
								else
									return false;
;							}
							for (int i=0;i<aliasJValue.size();i++)
							{
								WJson::Value aliaJValue = aliasJValue[i];
								if(aliaJValue.isMember(L"alia"))
									thisMap.header.alias.push_back(aliaJValue[L"alia"].asString());
								else
									return false;
;							}
							for (int i=0;i<composersJValue.size();i++)
							{
								WJson::Value composerJValue = composersJValue[i];
								if(composerJValue.isMember(L"composer"))
									thisMap.header.composers.push_back(composerJValue[L"composer"].asString());
								else
									return false;
;							}
							for (int i=0;i<lyricistsJValue.size();i++)
							{
								WJson::Value lyricistJValue = lyricistsJValue[i];
								if(lyricistJValue.isMember(L"lyricist"))
									thisMap.header.lyricists.push_back(lyricistJValue[L"lyricist"].asString());
								else
									return false;
;							}
							for (int i=0;i<artistsJValue.size();i++)
							{
								WJson::Value artistJValue = artistsJValue[i];
								if(artistJValue.isMember(L"artist"))
									thisMap.header.artists.push_back(artistJValue[L"artist"].asString());
								else
									return false;
;							}
							for (int i=0;i<vocaloidsJValue.size();i++)
							{
								WJson::Value vocaloidJValue = vocaloidsJValue[i];
								if(vocaloidJValue.isMember(L"vocaloid"))
									thisMap.header.vocaloids.push_back(vocaloidJValue[L"vocaloid"].asString());
								else
									return false;
;							}
						}
						else
							return false;
					}
					else
						return false;

					#pragma endregion Header

					#pragma region Level
					WJson::Value levelsJValue;
					if(mapJValue.isMember(L"levels"))
						levelsJValue = mapJValue[L"levels"];
					else
						return false;

					if(levelsJValue.isArray() || levelsJValue.isNull())
						for (int levelI = 0;levelI<levelsJValue.size();levelI++)
						{
							WJson::Value levelJValue = levelsJValue[levelI];
							if(levelJValue.isMember(L"divaFileName")&&levelJValue.isMember(L"FileMD5Value")&&levelJValue.isMember(L"level")&&levelJValue.isMember(L"difficualty"))
							{
								DivaMap::LevelType thisLevel = (DivaMap::LevelType)levelJValue[L"level"].asInt();
								thisMap.levels[thisLevel].divaFileName = levelJValue[L"divaFileName"].asString();
								thisMap.levels[thisLevel].FileMD5Value = levelJValue[L"FileMD5Value"].asString();
								thisMap.levels[thisLevel].difficualty = levelJValue[L"difficualty"].asInt();
							}
							else
								return false;
						}
					else
						return false;

					#pragma endregion Level
				}
			else
				return false;
		}
		else
			return false;


		if(rootJsonValue.isMember(L"LastUpdatedDate"))
		{
			lastUpdatedDate = rootJsonValue[L"LastUpdatedDate"].asString();
			return true;
		}
		return false;
	}
	void DivaMapManager::saveToLocalFile()
	{
		WJson::StyledWriter writer;

		WJson::Value rootJValue;

		WJson::Value mapsJValue;

		rootJValue[L"LastUpdatedDate"] = lastUpdatedDate;

		for (std::map<int, DivaMap>::iterator mapI=maps.begin();mapI!=maps.end();mapI++)
		{
			WJson::Value thisMapJValue;
			thisMapJValue[L"ID"] = mapI->second.id;

			#pragma region Header
			DivaMapHeader& header = mapI->second.header;
			WJson::Value headerJValue;
			headerJValue[L"mapType"] = (int)header.mapType;
			headerJValue[L"name"] = header.name;
			headerJValue[L"thumb"] = header.thumb;
			headerJValue[L"audioPreview"] = header.audioPreview;
			headerJValue[L"playedCount"] = header.playedCount;
			headerJValue[L"additionalMessage"] = header.additionalMessage;

			WJson::Value notersJValue;
			for(std::vector<std::wstring>::iterator noterI = header.noters.begin();noterI != header.noters.end();noterI++)
			{
				WJson::Value noterJValue;
				noterJValue[L"noter"] = (*noterI);
				notersJValue.append(noterJValue);
			}
			headerJValue[L"noters"] = notersJValue;

			WJson::Value aliasJValue;
			for(std::vector<std::wstring>::iterator aliaI = header.alias.begin();aliaI != header.alias.end();aliaI++)
			{
				WJson::Value aliaJValue;
				aliaJValue[L"alia"] = (*aliaI);
				aliasJValue.append(aliaJValue);
			}
			headerJValue[L"alias"] = aliasJValue;

			WJson::Value composersJValue;
			for(std::vector<std::wstring>::iterator composerI = header.composers.begin();composerI != header.composers.end();composerI++)
			{
				WJson::Value composerJValue;
				composerJValue[L"composer"] = (*composerI);
				composersJValue.append(composerJValue);
			}
			headerJValue[L"composers"] = composersJValue;

			WJson::Value lyricistsJValue;
			for(std::vector<std::wstring>::iterator lyricistI = header.lyricists.begin();lyricistI != header.lyricists.end();lyricistI++)
			{
				WJson::Value lyricistJValue;
				lyricistJValue[L"lyricist"] = (*lyricistI);
				lyricistsJValue.append(lyricistJValue);
			}
			headerJValue[L"lyricists"] = lyricistsJValue;

			WJson::Value artistsJValue;
			for(std::vector<std::wstring>::iterator artistI = header.artists.begin();artistI != header.artists.end();artistI++)
			{
				WJson::Value artistJValue;
				artistJValue[L"artist"] = (*artistI);
				artistsJValue.append(artistJValue);
			}
			headerJValue[L"artists"] = artistsJValue;

			WJson::Value vocaloidsJValue;
			for(std::vector<std::wstring>::iterator vocaloidI = header.vocaloids.begin();vocaloidI != header.vocaloids.end();vocaloidI++)
			{
				WJson::Value vocaloidJValue;
				vocaloidJValue[L"vocaloid"] = (*vocaloidI);
				vocaloidsJValue.append(vocaloidJValue);
			}
			headerJValue[L"vocaloids"] = vocaloidsJValue;

			thisMapJValue[L"header"] = headerJValue;

			#pragma endregion Header

			#pragma region Levels
			WJson::Value levelsJValue;
			for(std::map<DivaMap::LevelType, DivaMapLevel>::iterator levelI = mapI->second.levels.begin();levelI!=mapI->second.levels.end();levelI++)
			{
				WJson::Value levelJValue;
				levelJValue[L"divaFileName"] = levelI->second.divaFileName;
				levelJValue[L"FileMD5Value"] = levelI->second.FileMD5Value;
				levelJValue[L"level"] = (int)levelI->first;
				levelJValue[L"difficualty"] = levelI->second.difficualty;

				levelsJValue.append(levelJValue);
			}
			thisMapJValue[L"levels"] = levelsJValue;

			#pragma endregion Levels

			mapsJValue.append(thisMapJValue);
		}

		rootJValue[L"maps"] = mapsJValue;


		std::wstring writerData = writer.write(rootJValue);
		std::wstring encryptedData = encrypt(writerData);

		FILE* writeFile;
		if(_wfopen_s(&writeFile, (std::wstring(LocalSongDirectoryW)+L"DIVAOLMDB").c_str(),L"w, ccs=UTF-8")!=0)
			return;
		fwprintf(writeFile,L"%s",encryptedData.c_str());
		fclose(writeFile);
	}

	void DivaMapManager::update(float dt)
	{
		DivaMapEventMessage thisMessage;
		while(threadQueue.take(thisMessage))
		{
			if(thisMessage.finish)
				isOperating[thisMessage.effectedMapID][thisMessage.eventType]=false;

			if(thisMessage.finish && !thisMessage.error && 
				thisMessage.eventType==DivaMapEventMessage::PrepareMapDataFile || thisMessage.eventType==DivaMapEventMessage::PrepareMapDataFileNoVideo)
			{
				Base::String localFile = Base::Path::CombinePath(Base::String(LocalSongDirectoryW),
					L"MAP_"+Base::String::any2string(thisMessage.effectedMapID)+ (thisMessage.eventType==DivaMapEventMessage::PrepareMapDataFile?L"":L"_noVideo") + L".divaolpack").str();
				PrepareDivaMapDataFromFile(localFile);
			}
			
			if(listMsgOut)
				listMsgOut->push_back(thisMessage);
		}
	}


#pragma region MultiThread functions
	

	static int DownloadFileProgressCallback(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
	{
		DivaMapEventMessage *eventMsg = (DivaMapEventMessage*)p;
		if(abs(dltotal)<1e-6)
			eventMsg->downloadProgress=0;
		else
			eventMsg->downloadProgress = dlnow / dltotal;
		if(eventMsg->eventType == DivaMapEventMessage::PrepareMapDataFile)
			MAPMGR.GetMessageQueue().put((*eventMsg));
		return 0;
	}

	static size_t
	WriteFileCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		DivaMapManagerDownloadQuest *quest = (DivaMapManagerDownloadQuest*)userp;

		FILE *saveFile;

		CreateDirectoryW((Base::String(LocalSongDirectoryW)+L"MAP_"+Base::String::any2string(quest->mapID)+L"/").unicode_str(),NULL);
		if(_wfopen_s(&saveFile, quest->localFileAddress.unicode_str(), L"ab+")==0)
		{
			fwrite(contents, size, nmemb, saveFile);
			fclose(saveFile);
		}
		else
		{
			quest->failed=true;
		}

		return size*nmemb;
	}



	unsigned __stdcall DownloadFileAsync(void* arg_quest)
	{
		DivaMapManagerDownloadQuest *thisQuest = (DivaMapManagerDownloadQuest*)arg_quest;

		DivaMapEventMessage thisMessage(thisQuest->eventType, thisQuest->mapID, false, false, 0);

		CURL *curl_handle;
		curl_handle = curl_easy_init();
		thisQuest->curlHandle = curl_handle;
		curl_easy_setopt(curl_handle, CURLOPT_URL, thisQuest->sourceAddress.ansi_str());
		curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, DownloadFileProgressCallback);
		curl_easy_setopt(curl_handle, CURLOPT_PROGRESSDATA, (void *)&thisMessage);
		curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, false);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteFileCallback);
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)thisQuest);
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "DivaOLMapManager-agent/0.1");
		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1);

		
		if(curl_easy_perform(curl_handle)==CURLE_OK && !thisQuest->failed)
		{
			thisMessage.finish=true;
			thisMessage.error=false;
			thisMessage.downloadProgress=1;
		}
		else
		{
			thisMessage.finish=true;
			thisMessage.error=true;
			DeleteFileW(thisQuest->localFileAddress.unicode_str());
		}

		MAPMGR.GetMessageQueue().put(thisMessage);

		curl_easy_cleanup(curl_handle);
		Base::DeletePtr(thisQuest);

		return 0;
	}

	std::wstring readJsonFile(FILE* zippedFile)
	{
		///Get Header Json String
		int strLength;
		fread(&strLength, sizeof(int),1,zippedFile);
		const int bufferLength = 65536;
		std::wstring ret;
		wchar_t strBuffer[bufferLength+1];
		while(strLength>0)
		{
			if(strLength>=bufferLength)
			{
				fread(strBuffer,sizeof(char),bufferLength,zippedFile);
				strBuffer[(bufferLength+1)/2]=0;
				strLength-=bufferLength;
			}
			else
			{
				fread(strBuffer,sizeof(char),strLength,zippedFile);
				strBuffer[(strLength+1)/2]=0;
				strLength-=strLength;
			}
			ret += strBuffer;
		}

		return ret;
	}

	bool DepackDivaOLPack(DivaMapManagerDownloadQuest *quest, DivaMapEventMessage *eventMsg, FILE*& zippedFile, bool& needToClose)
	{
		needToClose=false;
		if(_wfopen_s(&zippedFile, quest->localFileAddress.unicode_str(), L"rb+")==0)
		{
			needToClose=true;
			std::wstring headStr = readJsonFile(zippedFile);

			//Parse header json string
			WJson::Reader reader;
			WJson::Value headRootJsonValue;
			if(!reader.parse(headStr,headRootJsonValue))
				return false;

			if(headRootJsonValue.isMember(L"id") && headRootJsonValue.isMember(L"ver"))
			{
				eventMsg->effectedMapID = headRootJsonValue[L"id"].asInt();
				quest->mapID = eventMsg->effectedMapID;
				std::wstring packVersion = headRootJsonValue[L"ver"].asString();
				while(!feof(zippedFile))
				{
					std::wstring fileJsonStr = readJsonFile(zippedFile);

					WJson::Value fileJsonValue;
					if(!reader.parse(fileJsonStr,fileJsonValue))
						return false;

					if(fileJsonValue.isMember(L"filename") && fileJsonValue.isMember(L"sourcelength") && fileJsonValue.isMember(L"length"))
					{
						std::wstring thisFileName = fileJsonValue[L"filename"].asString();
						uLong sourceLength = (uLong)fileJsonValue[L"sourcelength"].asInt();
						uLong fileLength = (uLong)fileJsonValue[L"length"].asInt();

						Byte *fileByte = (Byte*)malloc(sizeof(char)*fileLength);
						Byte *sourceByte = (Byte*)malloc(sizeof(char)*sourceLength);
						if(fileByte==NULL || sourceByte==NULL)
						{
							if(fileByte)
								free(fileByte);
							if(sourceByte)
								free(sourceByte);
							return false;
						}

						bool someError = false;
						fread(fileByte, sizeof(char), fileLength, zippedFile);
						Base::String workingDirectory = Base::String(LocalSongDirectoryW)+L"MAP_"+Base::String::any2string(quest->mapID)+L"/";
						Base::String unpackedDirectory =  Base::Path::GetFilePath(workingDirectory+thisFileName);
						
						FILE *thisFile=NULL;
						CreateDirectoryW(unpackedDirectory.unicode_str(),NULL);
						if(_wfopen_s(&thisFile,(workingDirectory+thisFileName).unicode_str(),L"wb+")==0)
						{
							uLong uncompressedLength = sourceLength;
							int uncompressResult = uncompress(sourceByte, &uncompressedLength, fileByte, fileLength);
							if(uncompressResult==Z_OK && sourceLength == uncompressedLength)
								fwrite(sourceByte, sizeof(char), sourceLength, thisFile);
							else
								someError=true;
							fclose(thisFile);
						}
						else
							someError=true;

						free(fileByte);
						free(sourceByte);

						if(someError)
							return false;
					}
					else
						return false;
				}
			}
			else
				return false;

			needToClose=false;
			fclose(zippedFile);
		}
		else
			return false;

		return true;
	}

	unsigned __stdcall DepackDivaOLPackAsync(void* arg_quest)
	{
		DivaMapManagerDownloadQuest *thisQuest = (DivaMapManagerDownloadQuest*)arg_quest;
		DivaMapEventMessage thisMessage(thisQuest->eventType, thisQuest->mapID, false, false, 0);

		FILE* zippedFile;
		bool needToClose;
		if(DepackDivaOLPack(thisQuest,&thisMessage,zippedFile,needToClose))
		{
			thisMessage.error=false;
			thisMessage.finish=true;
		}
		else
		{
			thisMessage.error=true;
			thisMessage.finish=true;
		}
		if(needToClose)
			fclose(zippedFile);

		MAPMGR.GetMessageQueue().put(thisMessage);
		Base::DeletePtr(thisQuest);

		return 0;
	}

#pragma endregion MultiThread functions


	void DivaMapManager::PrepareDivaMapListInfo()
	{
		maps[1].id=1;
		maps[1].header.thumb = L"spotlight.png";
		maps[1].header.artists.push_back(L"我");
		maps[1].header.composers.push_back(L"他");
		maps[1].header.mapType = divamap::DivaMapHeader::Couple;
		maps[1].header.bpm = 111;
		maps[1].header.name = L"雪の羽　時の風";
		maps[1].header.playedCount = 12;
		maps[1].header.alias.push_back(L"其实有名字");
		maps[1].header.lyricists.push_back(L"你");
		maps[1].header.noters.push_back(L"彼女");
		maps[1].header.songLength = 450;
		maps[1].levels[divamap::DivaMap::Easy].difficualty = 30;
		maps[1].levels[divamap::DivaMap::Normal].difficualty = 60;
		maps[1].levels[divamap::DivaMap::Hard].difficualty = 80;
	}

	bool DivaMapManager::PrepareDirectFile(int id, DivaMapEventMessage::DIVAMAPMGREVENT eventType)
	{
		if(maps.find(id)==maps.end() || isOperating[id][eventType])
			return false;

		//Check if thumb file already exists
		if(eventType==DivaMapEventMessage::PrepareThumbFile || eventType==DivaMapEventMessage::PrepareAudioPreviewFile)
		{
			FILE *thumbFile;
			Base::String thumbFilePath = Base::String(LocalSongDirectoryW)+L"MAP_"+Base::String::any2string(id)+L"/"+
				(eventType==DivaMapEventMessage::PrepareThumbFile?maps[id].header.thumb:maps[id].header.audioPreview);
			if(_wfopen_s(&thumbFile, thumbFilePath.unicode_str(), L"r")!=0)
			{
				//File not exists
				Base::String thumbAddress = Base::String(downloadCategoryServerAddress) + L"/" + thumbFilePath;
				DivaMapManagerDownloadQuest *thisQuest = new DivaMapManagerDownloadQuest(thumbAddress,thumbFilePath,id,eventType);
				unsigned int threadAddress;
				HANDLE hThread = 
					(HANDLE)_beginthreadex(
					NULL,
					0,
					&DownloadFileAsync,
					thisQuest,
					0,
					&threadAddress
					);

				if(hThread==NULL)
					return false;
			}
			else
			{
				fclose(thumbFile);
				if(listMsgOut!=NULL)
					listMsgOut->push_back(DivaMapEventMessage(eventType, id, false, true, 1));
			}
		}
		else if(eventType==DivaMapEventMessage::PrepareMapDataFile || eventType==DivaMapEventMessage::PrepareMapDataFileNoVideo)
		{
			//Check local map file exists
			bool needToDownload = false;
			for (MAPLEVELITERATOR levelI = maps[id].levels.begin();levelI!=maps[id].levels.end();levelI++)
			{
				FILE *divaFile;
				if(_wfopen_s(&divaFile, GetDivaOLFilePath(id, levelI->first).c_str(), L"r")!=0)
				{
					needToDownload=true;
					break;
				}
				else
					fclose(divaFile);
			}
			if(needToDownload)
			{
				Base::String localFile = Base::Path::CombinePath(Base::String(LocalSongDirectoryW),
					L"MAP_"+Base::String::any2string(id)+ (eventType==DivaMapEventMessage::PrepareMapDataFile?L"":L"_noVideo") + L".divaolpack").str();
				Base::String remoteFile = Base::String(downloadCategoryServerAddress) + L"/" + localFile;
				DivaMapManagerDownloadQuest *thisQuest = new DivaMapManagerDownloadQuest(remoteFile,localFile,id,eventType);
				unsigned int threadAddress;
				HANDLE hThread = 
					(HANDLE)_beginthreadex(
					NULL,
					0,
					&DownloadFileAsync,
					thisQuest,
					0,
					&threadAddress
					);

				if(hThread==NULL)
					return false;
			}
			else
				if(listMsgOut!=NULL)
					listMsgOut->push_back(DivaMapEventMessage(DivaMapEventMessage::UnpackMapDataFile, id, false, true, 1));
		}

		
		return true;
	}

	bool DivaMapManager::PrepareDivaMapThumb(int id)
	{
		return PrepareDirectFile(id, DivaMapEventMessage::PrepareThumbFile);
	}

	bool DivaMapManager::PrepareDivaMapAudioPreview(int id)
	{
		return PrepareDirectFile(id, DivaMapEventMessage::PrepareAudioPreviewFile);
	}

	bool DivaMapManager::PrepareDivaMapData(int id, bool novideo)
	{
		if(!novideo)
			return PrepareDirectFile(id, DivaMapEventMessage::PrepareMapDataFile);
		else
			return PrepareDirectFile(id, DivaMapEventMessage::PrepareMapDataFileNoVideo);
	}

	bool DivaMapManager::PrepareCheckLocalMapDataFileLeagal(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

		return true;
	}

	bool DivaMapManager::PrepareDivaMapDataFromFile(std::wstring divaolpackFile)
	{
		DivaMapManagerDownloadQuest *thisQuest = new DivaMapManagerDownloadQuest(L"",divaolpackFile,-1,DivaMapEventMessage::UnpackMapDataFile);

		unsigned int threadAddress;
		HANDLE hThread = 
			(HANDLE)_beginthreadex(
			NULL,
			0,
			&DepackDivaOLPackAsync,
			thisQuest,
			0,
			&threadAddress
			);

		if(hThread==NULL)
			return false;
		else
			return true;
	}


	//Get functions
	std::wstring DivaMapManager::GetMapName(int id)
	{
		if(id==0)
			return L"Random";
		else if(maps.find(id)!=maps.end())
			return maps.find(id)->second.header.name;
		else
			return L"";
	}
	std::wstring DivaMapManager::GetMapDirectory(int id)
	{
		return Base::String(Base::String(LocalSongDirectoryW)+L"MAP_"+Base::String::any2string(id)+L"/").asUnicode();
	}
	std::wstring DivaMapManager::GetThumbFilePath(int id)
	{
		if(id==0)
			return L"Random";
		if(maps.find(id)!=maps.end())
			return Base::Path::CombinePath(Base::String(GetMapDirectory(id)),Base::String(maps.find(id)->second.header.thumb)).str().asUnicode();
		else
			return L"";
	}
	std::wstring DivaMapManager::GetAudioPreviewFilePath(int id)
	{
		if(id==0)
			return L"Random";
		else if(maps.find(id)!=maps.end())
			return Base::Path::CombinePath(Base::String(GetMapDirectory(id)),Base::String(maps.find(id)->second.header.audioPreview)).str().asUnicode();
		else
			return L"";
	}
	std::wstring DivaMapManager::GetDivaOLFilePath(int id, DivaMap::LevelType level)
	{
		if(id==0)
			return L"Random";
		else if(maps.find(id)!=maps.end())
			return Base::Path::CombinePath(Base::String(GetMapDirectory(id)),Base::String(maps.find(id)->second.levels[level].divaFileName)).str().asUnicode();
		else
			return L"";
	}

	bool DivaMapManager::isMapIdLeagal(int id)
	{
		if(maps.find(id)==maps.end())
			return false;
		return true;
	}

	//Select Map functions
	void DivaMapManager::SelectedMap_Add(int id, DivaMap::LevelType level)
	{
		if(selectedMaps.size()<MAXSelectedMapCount)
			selectedMaps.push_back(DivaMapSelectedItem(id,level));
	}
	void DivaMapManager::SelectedMap_ChangeLevel(int index, DivaMap::LevelType level)
	{
		if(index>=0&&index<selectedMaps.size())
			selectedMaps[index].level = level;
	}
	void DivaMapManager::SelectedMap_Clear()
	{
		selectedMaps.clear();
	}
	void DivaMapManager::SelectedMap_Remove(int index)
	{
		if(index>=0&&index<selectedMaps.size())
			selectedMaps.erase(selectedMaps.begin()+index);
	}
	void DivaMapManager::SelectedMap_Swap(int indexL,int indexR)
	{
		if(indexL!=indexR&&indexL>=0&&indexL<selectedMaps.size() && indexR>=0&&indexR<selectedMaps.size())
			std::swap(selectedMaps[indexL],selectedMaps[indexR]);
	}
}
