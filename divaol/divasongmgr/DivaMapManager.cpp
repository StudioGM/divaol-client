#include "DivaMapManager.h"

#include <Process.h>
#include <string>
#include "Lib/wjson/wjson.h"
#include "DivaMapEncryption.h"

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

					if(headerJValue.isMember(L"name") && headerJValue.isMember(L"thumb")  && headerJValue.isMember(L"audioPreview")  && headerJValue.isMember(L"playedCount")  
						&& headerJValue.isMember(L"additionalMessage") && headerJValue.isMember(L"noters")  && headerJValue.isMember(L"alias")  && headerJValue.isMember(L"composers")
						&& headerJValue.isMember(L"lyricists")  && headerJValue.isMember(L"artists") && headerJValue.isMember(L"vocaloids"))
					{
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

	}


#pragma region MultiThread functions

	unsigned __stdcall DownloadDivaMapThumb(void* arg_map)
	{
		int mapID = (*((DivaMap*)arg_map)).id;
		std::wstring thumbAddress = (*((DivaMap*)arg_map)).header.thumb;
		Sleep(2000);
		MessageBoxW(NULL,L"ThreadOver!", L"Thread Test", MB_OK);

		return 0;
	}


#pragma endregion MultiThread functions








	void DivaMapManager::PrepareDivaMapListInfo()
	{

	}
	bool DivaMapManager::PrepareDivaMapThumb(int id)
	{
		//if(maps.find(id)==maps.end())
			//return false;

		//Check if thumb file already exists
		FILE *thumbFile;
		Base::String thumbFilePath = Base::String(LocalSongDirectoryW)+L"MAP_"+Base::String::any2string(id)+L"/"+maps[id].header.thumb;
		if(_wfopen_s(&thumbFile, thumbFilePath.unicode_str(), L"r")!=0)
		{
			//File not exists
			
			unsigned int threadAddress;
			HANDLE hThread = 
				(HANDLE)_beginthreadex(
				NULL,
				0,
				&DownloadDivaMapThumb,
				&id,
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
				listMsgOut->push_back(DivaMapEventMessage(DivaMapEventMessage::PrepareThumbFile, id, false, true, 1));
		}
		return true;
	}

	bool DivaMapManager::PrepareDivaMapAudioPreview(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

		return true;
	}

	bool DivaMapManager::PrepareDivaMapData(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

		return true;
	}

	bool DivaMapManager::PrepareCheckLocalMapDataFileLeagal(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

		return true;
	}

	bool DivaMapManager::PrepareDivaMapDataFromFile(std::wstring zippedFile)
	{
		return false;
	}

	//Check file functions
	bool DivaMapManager::CheckLocalThumbFile(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

		return true;
	}
	bool DivaMapManager::CheckLocalAudioPreviewFile(int id)
	{
		if(maps.find(id)==maps.end())
			return false;

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