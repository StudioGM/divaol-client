#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <direct.h>
#include <locale>
#include "stdio.h"

#include "divaeditor/Core/DivaEditorMapData.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Component/DivaStandardCoreFlow.h"

#include "divaeditor/WJson/wjson.h"

namespace divaeditor
{
	std::wstring checkTailEnter(std::wstring str)
	{
		std::wstring ret = str;
		if(ret.length()==0) return L"";

		int rightTrim=ret.length()-1,leftTrim=0;
		while(rightTrim>=0&&(ret[rightTrim]=='\n'||ret[rightTrim]=='\r'||ret[rightTrim]==' '))
			rightTrim--;
		while(leftTrim<ret.length()&&(ret[leftTrim]=='\n'||ret[leftTrim]=='\r'||ret[leftTrim]==' '))
			leftTrim++;

		if(rightTrim<0 || leftTrim==ret.length()) return L"";
		return ret.substr(leftTrim,(rightTrim-leftTrim)+1);
	}

	DivaEditorMapData::InitSourceFileType DivaEditorMapData::parseInitFileType(std::wstring path)
	{
		std::wstring ext=L"";
		for(int i=path.length()-1;i>=0;i--)
		{
			if(path[i]==L'.')
				break;
			ext = path[i] + ext;
		}

		tolower(ext);

		const int fileTypeCount = 8;
		std::wstring fileTypes[fileTypeCount] = {
			L"diva",
			L"divaproject",
			L"divaol",
			L"divaolproject" , 
			L"mp3,wav,ogg", 
			L"bms",
			L"osu",
			L"mid"};

		for(int i=0;i<fileTypeCount;i++)
		{
			if(fileTypes[i].find(ext)!=std::wstring::npos)
				return (InitSourceFileType)i;
		}
		return InitSourceFileType::Unknown;
	}

	std::wstring DivaEditorMapData::InitFromFile(std::wstring path)
	{
		InitSourceFileType fileType = parseInitFileType(path);

		if(fileType==InitSourceFileType::DivaPCFile)
			return InitFromDivaPCFile(path);
		else if(fileType==InitSourceFileType::DivaPCProFile)
			return InitFromDivaPCProFile(path);
		else if(fileType==InitSourceFileType::DivaOLFile)
			return InitFromDivaOLFile(path);
		else if(fileType==InitSourceFileType::DivaOLProFile)
			return InitFromDivaOLProFile(path);
		else if(fileType==InitSourceFileType::MusicFile)
			return InitFromMusicFile(path);
		else if(fileType==InitSourceFileType::BMSFile)
			return InitFromBMSFile(path);
		else if(fileType==InitSourceFileType::OSUFile)
			return InitFromOSUFile(path);
		else if(fileType==InitSourceFileType::MIDIFile)
			return InitFromMIDIFile(path);
		else
			return LOCALIZATION->getLocalStr(L"ReadFile_FileNotSupported", path.c_str());
	}
	std::wstring DivaEditorMapData::ChooseWorkingFile(bool needToCreateNewDirectory)
	{
		wchar_t cwd[_MAX_PATH];
		_wgetcwd(cwd,_MAX_PATH);

		wstring saveFile = sora::SoraCore::Instance()->fileSaveDialogW(L"Diva Online Map Project(*.divaolproject)\0*.divaolproject\0",NULL,L".divaproject");

		_wchdir(cwd);

		if(saveFile!=L"")
		{
			int pathPos,dotPos;
			for(pathPos=saveFile.length()-1;pathPos>=0;pathPos--)
				if(saveFile[pathPos]==L'\\'||saveFile[pathPos]==L'/')
					break;

			for(dotPos=saveFile.length()-1;dotPos>=0;dotPos--)
				if(saveFile[dotPos]==L'.')
					break;

			if(pathPos==-1) return LOCALIZATION->getLocalStr(L"ReadFile_PathError", saveFile.c_str());

			if(needToCreateNewDirectory)
			{
				workingDirectory = saveFile.substr(0, dotPos) + L"/";
				CreateDirectoryW((workingDirectory).c_str(),NULL);
			}
			else
			{
				workingDirectory = saveFile.substr(0, pathPos) + L"/";
			}
			

			workingDivaOLFile = saveFile.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaol";
			workingDivaOLProFile = saveFile.substr(pathPos+1, saveFile.length()-(pathPos+1));

			return L"OK";
		}
		else
			return LOCALIZATION->getLocalStr(L"ReadFile_CancelChooseWorkingFile");
	}
	bool DivaEditorMapData::SaveFile(bool olFile, bool proFile, divacore::MapInfo *infoToSave)
	{
		if(workingDirectory==L"" || workingDivaOLFile==L"" || workingDivaOLProFile==L"")
			return false;

		CreateDirectoryW(workingDirectory.c_str(),NULL);

#pragma region Save DivaOL Play File
		if(olFile)
		{
			if(infoToSave==NULL)
				infoToSave=coreInfoPtr;

			WJson::StyledWriter writer;
			
			WJson::Value rootJsonValue;

			WJson::Value eventJsonValue;
			WJson::Value headerJsonValue;
			WJson::Value noteListJsonValue;
			WJson::Value resourceListJsonValue;

#pragma region Convert Event to json
			for(int i=0;i<infoToSave->events.size();i++)
			{
				WJson::Value thisEventJsonValue;
				divacore::MapEvent &thisEvent = infoToSave->events[i];

				thisEventJsonValue[L"type"] = s2ws(thisEvent.eventType);
				thisEventJsonValue[L"position"] = thisEvent.position;

				for (map<std::string,sora::SoraAny>::iterator argI=thisEvent.arg.begin();argI!=thisEvent.arg.end();argI++)
				{
					if(argI->first=="value")
						thisEventJsonValue[s2ws(argI->first)] = Argument::asDouble(argI->first, thisEvent.arg);

					else if(argI->first=="width"
							|| argI->first=="height"
							|| argI->first=="tex_x"
							|| argI->first=="tex_y"
							|| argI->first=="tex_width"
							|| argI->first=="tex_height"
							)
						thisEventJsonValue[s2ws(argI->first)] = Argument::asInt(argI->first, thisEvent.arg);

					else if(argI->first=="id"
							|| argI->first=="tag"
							|| argI->first=="channel"
							|| argI->first=="label"
							)
						thisEventJsonValue[s2ws(argI->first)] = s2ws(Argument::asString(argI->first, thisEvent.arg));
				}
				eventJsonValue.append(thisEventJsonValue);
			}
			rootJsonValue[L"event"] = eventJsonValue;
#pragma endregion Convert Event to json

#pragma region Convert header to json

			headerJsonValue[L"bpm"] = infoToSave->header.BPM;
			headerJsonValue[L"difficulty"] = (int)infoToSave->header.difficulty;
			headerJsonValue[L"hard"] = (int)infoToSave->header.hardLevel;
			headerJsonValue[L"mainSound"] = s2ws(infoToSave->header.mainSound);
			headerJsonValue[L"name"] = infoToSave->header.mapName;
			
			WJson::Value noterListJsonValue;
			for (int i=0;i<infoToSave->header.noter.size();i++)
			{
				WJson::Value noterNameJsonValue;
				noterNameJsonValue[L"name"] = infoToSave->header.noter[i];
				noterListJsonValue.append(noterNameJsonValue);
			}
			headerJsonValue[L"noter"] = noterListJsonValue;
			headerJsonValue[L"speedScale"] = (double)infoToSave->header.speedScale;
			headerJsonValue[L"version"] = s2ws(infoToSave->header.versionStr);

			rootJsonValue[L"header"] = headerJsonValue;
#pragma endregion Convert header to json

			rootJsonValue[L"module"] = s2ws(infoToSave->module);

#pragma region notes
			for (int i=0;i<infoToSave->notes.size();i++)
			{
				WJson::Value noteJsonValue;
				divacore::MapNote &thisNote = infoToSave->notes[i];

				noteJsonValue[L"type"] = s2ws(thisNote.noteType);
				
				WJson::Value notePointListJsonValue;
				for (int notePointI=0;notePointI<thisNote.notePoint.size();notePointI++)
				{
					WJson::Value notePointJsonValue;
					divacore::NotePoint &thisNotePoint = thisNote.notePoint[notePointI];

					notePointJsonValue[L"type"] = thisNotePoint.type;
					notePointJsonValue[L"position"] = thisNotePoint.position;
					notePointJsonValue[L"x"] = thisNotePoint.x;
					notePointJsonValue[L"y"] = thisNotePoint.y;
					if(thisNotePoint.key!="")
						notePointJsonValue[L"key"] = s2ws(thisNotePoint.key);

					notePointListJsonValue.append(notePointJsonValue);
				}

				noteJsonValue[L"notePoint"] = notePointListJsonValue;

				for (map<std::string, sora::SoraAny>::iterator argI = thisNote.arg.begin();argI!=thisNote.arg.end();argI++)
				{
					if(argI->first == "tailx"
						|| argI->first == "taily"
						|| argI->first == "standard_num"
						)
						noteJsonValue[s2ws(argI->first)] = Argument::asInt(argI->first, thisNote.arg);
				}
				noteListJsonValue.append(noteJsonValue);
			}

			rootJsonValue[L"note"] = noteListJsonValue;
#pragma endregion notes

#pragma region resource
			for(map<std::string, divacore::MapResourceInfo>::iterator resourceI=infoToSave->resources.begin();resourceI!=infoToSave->resources.end();resourceI++)
			{
				WJson::Value resourceJsonValue;
				divacore::MapResourceInfo &resourceInfo = resourceI->second;

				resourceJsonValue[L"file"] = resourceInfo.filePath;
				resourceJsonValue[L"id"] = s2ws(resourceInfo.ID);

				if(resourceInfo.type==divacore::MapResourceInfo::VIDEO)
					resourceJsonValue[L"type"] = L"video";
				if(resourceInfo.type==divacore::MapResourceInfo::AUDIO)
				{
					resourceJsonValue[L"type"] = L"audio";
					resourceJsonValue[L"stream"] = resourceInfo.flag;
				}
				if(resourceInfo.type==divacore::MapResourceInfo::IMAGE)
					resourceJsonValue[L"type"] = L"image";

				resourceListJsonValue.append(resourceJsonValue);
			}

			rootJsonValue[L"resource"] = resourceListJsonValue;
#pragma endregion resource

			
			FILE* writeFile;
			
			if(_wfopen_s(&writeFile, (workingDirectory+L"/"+workingDivaOLFile).c_str(),L"w, ccs=UTF-8")!=0)
				return false;
			fwprintf(writeFile,L"%s",writer.write(rootJsonValue).c_str());
			fclose(writeFile);
			

			/*
			std::string jsonStrToSave = to_utf8(writer.write(rootJsonValue));
			std::ofstream outFile(workingDirectory+L"/"+workingDivaOLFile);

			outFile << jsonStrToSave;
			outFile.close();
			*/
			
		}
#pragma endregion Save DivaOL Play File
		
#pragma region Save Diva Online Project File

		if(proFile)
		{
			WJson::StyledWriter writer;

			WJson::Value rootJsonValue;

			WJson::Value headerJsonValue;
			WJson::Value stopListJsonValue;
			WJson::Value beatNumListJsonValue;
			WJson::Value descriptionListJsonValue;

			//header
			headerJsonValue[L"divaol"] = workingDivaOLFile;
			headerJsonValue[L"offset"] = mapOffset;
			headerJsonValue[L"version"] = s2ws(EditorVer);
			headerJsonValue[L"gridToShowPerBeat"] = EDITCONFIG->getGridToShowPerBeatOrigin();
			headerJsonValue[L"showRangeScale"] = EDITCONFIG->getShowRangeScale();

			rootJsonValue[L"header"] = headerJsonValue;

#pragma region editor info

			//Stops
			for (map<int,int>::iterator stopI=stopLength.begin();stopI!=stopLength.end();stopI++)
			{
				WJson::Value stopJsonValue;
				stopJsonValue[L"position"] = stopI->first;
				stopJsonValue[L"value"] = stopI->second;

				stopListJsonValue.append(stopJsonValue);
			}

			rootJsonValue[L"stopblock"] = stopListJsonValue;


			//Beat Num
			for (map<int,int>::iterator beatNumI = beatNumChanged.begin();beatNumI!=beatNumChanged.end();beatNumI++)
			{
				WJson::Value beatJsonValue;
				beatJsonValue[L"period"] = beatNumI->first;
				beatJsonValue[L"beat"] = beatNumI->second;

				beatNumListJsonValue.append(beatJsonValue);
			}

			rootJsonValue[L"beat"] = beatNumListJsonValue;

			//Resource Description
			for(map<std::string,std::wstring>::iterator descriptionI = resourceDescription.begin();descriptionI!=resourceDescription.end();descriptionI++)
			{
				WJson::Value descriptionJsonValue;
				descriptionJsonValue[L"id"] = s2ws(descriptionI->first);
				descriptionJsonValue[L"label"] = descriptionI->second;

				descriptionListJsonValue.append(descriptionJsonValue);
			}

			rootJsonValue[L"label"] = descriptionListJsonValue;

#pragma endregion editor info

			
			FILE* writeFile;

			if(_wfopen_s(&writeFile, (workingDirectory+L"/"+workingDivaOLProFile).c_str(),L"wt, ccs=UTF-8")!=0)
				return false;
			fwprintf(writeFile,L"%s",writer.write(rootJsonValue).c_str());

			fclose(writeFile);
			
			
			/*
			std::string jsonStrToSave = to_utf8(writer.write(rootJsonValue));
			std::ofstream outFile(workingDirectory+L"/"+workingDivaOLProFile);

			outFile << jsonStrToSave;
			outFile.close();
			*/
		}

#pragma endregion Save Diva Online Project File

		EDITCONFIG->isMapChanged=false;
		return true;
	}

	std::wstring DivaEditorMapData::InitFromDivaOLProFile(std::wstring path)
	{
		int pathPos,dotPos;
		for(pathPos=path.length()-1;pathPos>=0;pathPos--)
			if(path[pathPos]==L'\\'||path[pathPos]==L'/')
				break;

		for(dotPos=path.length()-1;dotPos>=0;dotPos--)
			if(path[dotPos]==L'.')
				break;

		if(pathPos==-1) return LOCALIZATION->getLocalStr(L"ReadFile_PathError", path.c_str());

		workingDirectory = path.substr(0,pathPos);
		workingDivaOLProFile = path.substr(pathPos+1, path.length()-(pathPos+1));


		//Read file

		std::wstring jsonStrToParse;

		FILE* readFile;
		if(_wfopen_s(&readFile, (workingDirectory+L"/"+workingDivaOLProFile).c_str(),L"rt, ccs=UTF-8")!=0)
			return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", path.c_str());

		wchar_t buffer[1000];
		while(fgetws(buffer,sizeof(buffer),readFile))
			jsonStrToParse += std::wstring(buffer);

		fclose(readFile);
		

		WJson::Reader reader;

		WJson::Value rootJsonValue;
		if(!reader.parse(jsonStrToParse,rootJsonValue))
			return LOCALIZATION->getLocalStr(L"ReadFile_ParseFileError", path.c_str());

		//Parse header
		if(rootJsonValue.isMember(L"header"))
		{
			WJson::Value header = rootJsonValue[L"header"];
			if(header.isMember(L"divaol"))
				workingDivaOLFile = header[L"divaol"].asString();
			else
				return LOCALIZATION->getLocalStr(L"ReadOLFile_divaolTagError");

			if(header.isMember(L"offset"))
				mapOffset = header[L"offset"].asInt();
			else
				return LOCALIZATION->getLocalStr(L"ReadOLFile_offsetTagError");


			if(header.isMember(L"gridToShowPerBeat"))
				EDITCONFIG->setGridToShowPerBeat(header[L"gridToShowPerBeat"].asInt());
			else
				return LOCALIZATION->getLocalStr(L"ReadOLFile_gridToShowPerBeatTagError");

			if(header.isMember(L"showRangeScale"))
				EDITCONFIG->setShowRangeScale(header[L"showRangeScale"].asDouble());
			else
				return LOCALIZATION->getLocalStr(L"ReadOLFile_showRangeScaleTagError");
		}
		else
			return LOCALIZATION->getLocalStr(L"ReadOLFile_headError");

		//Parse stop
		stopLength.clear();
		if(rootJsonValue.isMember(L"stopblock"))
		{
			WJson::Value stopList = rootJsonValue[L"stopblock"];
			if(!stopList.isNull() && stopList.isArray())
				for (int i=0;i<stopList.size();i++)
				{
					WJson::Value stopJsonValue = stopList[i];
					stopLength[stopJsonValue[L"position"].asInt()] = stopJsonValue[L"value"].asInt();
				}
		}
		else
			return LOCALIZATION->getLocalStr(L"ReadOLFile_stopBlockError");

		//Parse beat num
		beatNumChanged.clear();
		if(rootJsonValue.isMember(L"beat"))
		{
			WJson::Value beatNumList = rootJsonValue[L"beat"];
			if(!beatNumList.isNull() && beatNumList.isArray())
				for (int i=0;i<beatNumList.size();i++)
				{
					WJson::Value beatNumValue = beatNumList[i];
					beatNumChanged[beatNumValue[L"period"].asInt()] = beatNumValue[L"beat"].asInt();
				}
		}
		else
			return LOCALIZATION->getLocalStr(L"ReadOLFile_beatNumError");

		//Parse label
		resourceDescription.clear();
		if(rootJsonValue.isMember(L"label"))
		{
			WJson::Value labelList = rootJsonValue[L"label"];
			if(!labelList.isNull() && labelList.isArray())
				for (int i=0;i<labelList.size();i++)
				{
					WJson::Value labelValue = labelList[i];
					resourceDescription[ws2s(labelValue[L"id"].asString())] = labelValue[L"label"].asString();
				}
		}
		else
			return LOCALIZATION->getLocalStr(L"ReadOLFile_labelError");


		//Check if the .divaol exists
		if(_wfopen_s(&readFile, (workingDirectory+L"/"+workingDivaOLFile).c_str(),L"rt, ccs=UTF-8")!=0)
			return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", workingDivaOLFile.c_str());
		fclose(readFile);


		CORE_PTR->setSong(workingDirectory, workingDivaOLFile);

		return L"OK";
	}
	std::wstring DivaEditorMapData::InitFromDivaOLFile(std::wstring path)
	{
		int pathPos,dotPos;
		for(pathPos=path.length()-1;pathPos>=0;pathPos--)
			if(path[pathPos]==L'\\'||path[pathPos]==L'/')
				break;

		for(dotPos=path.length()-1;dotPos>=0;dotPos--)
			if(path[dotPos]==L'.')
				break;

		if(pathPos==-1) return LOCALIZATION->getLocalStr(L"ReadFile_PathError", path.c_str());

		workingDirectory = path.substr(0,pathPos);
		workingDivaOLFile = path.substr(pathPos+1, path.length()-(pathPos+1));
		workingDivaOLProFile = path.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaolproject";

		//Init editor default value
		ResetEditorMapData();

		SaveFile(false);

		CORE_PTR->setSong(workingDirectory,workingDivaOLFile);

		return L"OK";
	}

	std::wstring DivaEditorMapData::InitFromDivaPCFile(std::wstring path)
	{
		int pathPos,dotPos;
		for(pathPos=path.length()-1;pathPos>=0;pathPos--)
			if(path[pathPos]==L'\\'||path[pathPos]==L'/')
				break;

		for(dotPos=path.length()-1;dotPos>=0;dotPos--)
			if(path[dotPos]==L'.')
				break;

		if(pathPos==-1) return LOCALIZATION->getLocalStr(L"ReadFile_PathError", path.c_str());

		workingDirectory = path.substr(0,pathPos);
		workingDivaOLFile = path.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaol";
		workingDivaOLProFile = path.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaolproject";

		//Init editor default value
		ResetEditorMapData();

		divacore::MapInfo contructMapInfo;
		registerMapInfo(&contructMapInfo);

		coreInfoPtr->header.speedScale = 1;
		coreInfoPtr->header.versionStr = EditorVer;

		//Init module
		coreInfoPtr->module = "standard";

#pragma region Read DivaOL File

		char tempbuffer[1000];

		FILE* readFile;
		if(_wfopen_s(&readFile, path.c_str(),L"rt")!=0)
			return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", path.c_str());

		wstring thisVer;
		//Version
		thisVer = checkTailEnter(s2ws(fgets(tempbuffer,sizeof(tempbuffer),readFile)));
		//MapName
		coreInfoPtr->header.mapName = checkTailEnter(s2ws(fgets(tempbuffer,sizeof(tempbuffer),readFile)));
		//NoterName
		fgets(tempbuffer,sizeof(tempbuffer),readFile);
		std::wstring noterName = s2ws(tempbuffer);
		noterName = checkTailEnter(noterName);
		coreInfoPtr->header.noter.push_back(noterName);
		//AuthorName
		fgets(tempbuffer,sizeof(tempbuffer),readFile);
		//MusicStyle
		fgets(tempbuffer,sizeof(tempbuffer),readFile);

		//Preview Image path
		fgets(tempbuffer,sizeof(tempbuffer),readFile);
		//HardLevel
		int hardLevel;
		fwscanf(readFile, L"%d\n", &hardLevel);
		coreInfoPtr->header.hardLevel = hardLevel;
		//Difficulty
		int difficualty;
		fwscanf(readFile, L"%d\n", &difficualty);
		coreInfoPtr->header.difficulty=difficualty;
		//BPM Label
		fwscanf(readFile, L"%lf\n", &coreInfoPtr->header.BPM);
		//Period Number
		fgets(tempbuffer,sizeof(tempbuffer),readFile);


		//BPM Info
		std::map<int,double> bpmEvent;
		while(true)
		{
			int pos;double bpmValue;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%lf",&bpmValue);
			bpmEvent[pos]=bpmValue;
		}

		//Resource Event
		std::map<int,vector<int>> resourceEvent;
		while(true)
		{
			int pos,resId;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%d",&resId);
			resourceEvent[pos].push_back(resId);
		}

		//BGM
		int beginPos,mainSoundResId;

		std::map<int,vector<int>> bgmEvent;
		while(true)
		{
			int pos,bgmTrack,resId;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%d\n%d",&bgmTrack,&resId);
			if(bgmEvent.size()==0)
			{
				beginPos = pos;
				mainSoundResId = resId;
				mapOffset = -beginPos%192;
			}
			bgmEvent[pos].push_back(resId);
		}

		if(bgmEvent.size()==0)
			return LOCALIZATION->getLocalStr(L"ReadFile_NoMainSoundError", path.c_str());

		//Note
		while(true)
		{
			int pos;
			divacore::MapNote thisNote;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;

			divacore::NotePoint thisNotePoint;
			thisNotePoint.position = pos-beginPos;
			fwscanf(readFile,L"%d%d%d",&thisNotePoint.type,&thisNotePoint.x,&thisNotePoint.y);

			if(thisNotePoint.type<8)
				thisNote.noteType="normal";
			else
				thisNote.noteType="long";
			thisNotePoint.type%=8;

			int tailX,tailY;
			fwscanf(readFile,L"%d%d",&tailX,&tailY);
			thisNote.arg["tailx"]=tailX-(36+thisNotePoint.x*12);
			thisNote.arg["taily"]=tailY-(63+thisNotePoint.y*12);

			int keyNum;
			fwscanf(readFile,L"%d",&keyNum);
			thisNotePoint.key = divacore::iToS(keyNum);
			thisNotePoint.x += 1+(EDITCONFIG->NoteAreaWidth-36)/2;
			thisNotePoint.y += 1+(EDITCONFIG->NoteAreaHeight-14)/2;

			thisNote.notePoint.push_back(thisNotePoint);

			if(thisNote.noteType=="long")
			{
				int noteLength;
				fwscanf(readFile,L"%d",&noteLength);
				divacore::NotePoint tailNotePoint = thisNotePoint;
				tailNotePoint.position += noteLength;
				thisNote.notePoint.push_back(tailNotePoint);
				addLongNote(thisNote);
			}
			else
			{
				addNormalNote(thisNote);
			}
		}

		//AUDIO Resource
		std::map<int,string> audioResIDMap;
		while(true)
		{
			int wavResId;
			fwscanf(readFile,L"%d",&wavResId);
			if(wavResId==-1)
				break;

			std::string addedID = resource_add(checkTailEnter(s2ws(fgets(tempbuffer,sizeof(tempbuffer),readFile))),true);
			audioResIDMap[wavResId] = addedID;
			if(wavResId == mainSoundResId)
				contructMapInfo.header.mainSound = addedID;
		}

		//VIDEO IMAGE Resource
		std::map<int,string> videoResIDMap;
		while(true)
		{
			int resId;
			fwscanf(readFile,L"%d",&resId);
			if(resId==-1)
				break;

			std::string addedID = resource_add(checkTailEnter(s2ws(fgets(tempbuffer,sizeof(tempbuffer),readFile))),true);
			videoResIDMap[resId] = addedID;
		}

		//ChanceTime
		if(thisVer >= L"1.0.1.0")
		{
			int ct_begin,ct_end;
			fwscanf(readFile,L"%d%d",&ct_begin,&ct_end);
		}

		//No stop in .diva file

		fclose(readFile);


		//Deal with event
		for (std::map<int,double>::iterator i=bpmEvent.begin();i!=bpmEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			bpm_insert(thisPos,i->second);
		}

		for (std::map<int,vector<int>>::iterator i=resourceEvent.begin();i!=resourceEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			for (vector<int>::iterator eventI = i->second.begin();eventI!=i->second.end();eventI++)
			{
				if(videoResIDMap.find(*eventI)==videoResIDMap.end())
					continue;
				std::string resourceID = videoResIDMap[(*eventI)];
				int eventNo = resourceEvent_add(thisPos, resourceID);
				if(coreInfoPtr->resources[resourceID].type == divacore::MapResourceInfo::IMAGE)
				{
					coreInfoPtr->events[eventNo].arg["tex_x"] = 0;
					coreInfoPtr->events[eventNo].arg["tex_y"] = 0;
					coreInfoPtr->events[eventNo].arg["tex_width"] = 480;
					coreInfoPtr->events[eventNo].arg["tex_height"] = 272;
					coreInfoPtr->events[eventNo].arg["label"] = std::string("bg_image");
				}
			}
		}

		for (std::map<int,vector<int>>::iterator i=bgmEvent.begin();i!=bgmEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			for (vector<int>::iterator eventI = i->second.begin();eventI!=i->second.end();eventI++)
			{
				if(audioResIDMap.find(*eventI)!=audioResIDMap.end())
					resourceEvent_add(thisPos, audioResIDMap[(*eventI)]);
			}
		}

		for(int i=0;i<coreInfoPtr->notes.size();i++)
		{
			for (int j=0;j<coreInfoPtr->notes[i].notePoint.size();j++)
			{
				int keyNum = sToI(coreInfoPtr->notes[i].notePoint[j].key);
				if(audioResIDMap.find(keyNum)!=audioResIDMap.end())
					coreInfoPtr->notes[i].notePoint[j].key = audioResIDMap[keyNum];
				else
					coreInfoPtr->notes[i].notePoint[j].key = "";
			}
		}

#pragma endregion Read Diva File

		//coreInfoPtr->header.noter.clear();

		if(!SaveFile())
			return LOCALIZATION->getLocalStr(L"ReadFile_SaveFileError", (workingDirectory + L"/" + workingDivaOLProFile).c_str());

		ResetEditorMapData();
		registerMapInfo(NULL);

		return InitFromDivaOLProFile(workingDirectory + L"/" + workingDivaOLProFile);


	}
	std::wstring DivaEditorMapData::InitFromDivaPCProFile(std::wstring path)
	{
		std::wstring chooseWorkingFileResult = ChooseWorkingFile();
		if(chooseWorkingFileResult!=L"OK")
			return chooseWorkingFileResult;

		//Init editor default value
		ResetEditorMapData();

		divacore::MapInfo contructMapInfo;
		registerMapInfo(&contructMapInfo);

		coreInfoPtr->header.speedScale = 1;
		coreInfoPtr->header.versionStr = EditorVer;

		//Init module
		coreInfoPtr->module = "standard";

#pragma region Read DivaPro File

		wchar_t tempbuffer[1000];

		FILE* readFile;
		if(_wfopen_s(&readFile, path.c_str(),L"rt, ccs=UTF-8")!=0)
			return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", path.c_str());

		wstring thisVer;
		//Version
		thisVer = checkTailEnter(fgetws(tempbuffer,sizeof(tempbuffer),readFile));
		//MapName
		coreInfoPtr->header.mapName = checkTailEnter(fgetws(tempbuffer,sizeof(tempbuffer),readFile));
		//NoterName
		fgetws(tempbuffer,sizeof(tempbuffer),readFile);
		std::wstring noterName = tempbuffer;
		noterName = checkTailEnter(noterName);
		coreInfoPtr->header.noter.push_back(noterName);
		//AuthorName
		fgetws(tempbuffer,sizeof(tempbuffer),readFile);
		//MusicStyle
		fgetws(tempbuffer,sizeof(tempbuffer),readFile);

		//Preview Image number
		fgetws(tempbuffer,sizeof(tempbuffer),readFile);
		//HardLevel
		int hardLevel;
		fwscanf(readFile, L"%d\n", &hardLevel);
		coreInfoPtr->header.hardLevel = hardLevel;
		//Difficulty
		int difficualty;
		fwscanf(readFile, L"%d\n", &difficualty);
		coreInfoPtr->header.difficulty=difficualty;
		//BPM Label
		fwscanf(readFile, L"%lf\n", &coreInfoPtr->header.BPM);
		//Period Number
		fgetws(tempbuffer,sizeof(tempbuffer),readFile);


		//BPM Info
		std::map<int,double> bpmEvent;
		while(true)
		{
			int pos;double bpmValue;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%lf",&bpmValue);
			bpmEvent[pos]=bpmValue;
		}

		//Resource Event
		std::map<int,vector<int>> resourceEvent;
		while(true)
		{
			int pos,resId;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%d",&resId);
			resourceEvent[pos].push_back(resId);
		}

		//BGM
		int beginPos,mainSoundResId;

		std::map<int,vector<int>> bgmEvent;
		while(true)
		{
			int pos,bgmTrack,resId;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;
			fwscanf(readFile,L"%d\n%d",&bgmTrack,&resId);
			if(bgmEvent.size()==0)
			{
				beginPos = pos;
				mainSoundResId = resId;
				mapOffset = -beginPos%192;
			}
			bgmEvent[pos].push_back(resId);
		}

		if(bgmEvent.size()==0)
			return LOCALIZATION->getLocalStr(L"ReadFile_NoMainSoundError", path.c_str());

		//Note
		while(true)
		{
			int pos;
			divacore::MapNote thisNote;
			fwscanf(readFile,L"%d",&pos);
			if(pos==-1)
				break;

			divacore::NotePoint thisNotePoint;
			thisNotePoint.position = pos-beginPos;
			fwscanf(readFile,L"%d%d%d",&thisNotePoint.type,&thisNotePoint.x,&thisNotePoint.y);

			if(thisNotePoint.type<8)
				thisNote.noteType="normal";
			else
				thisNote.noteType="long";
			thisNotePoint.type%=8;

			int tailX,tailY;
			fwscanf(readFile,L"%d%d",&tailX,&tailY);
			thisNote.arg["tailx"]=tailX-(36+thisNotePoint.x*12);
			thisNote.arg["taily"]=tailY-(63+thisNotePoint.y*12);

			int keyNum;
			fwscanf(readFile,L"%d",&keyNum);
			thisNotePoint.key = divacore::iToS(keyNum);
			thisNotePoint.x += 1+(EDITCONFIG->NoteAreaWidth-36)/2;
			thisNotePoint.y += 1+(EDITCONFIG->NoteAreaHeight-14)/2;

			thisNote.notePoint.push_back(thisNotePoint);

			if(thisNote.noteType=="long")
			{
				int noteLength;
				fwscanf(readFile,L"%d",&noteLength);
				divacore::NotePoint tailNotePoint = thisNotePoint;
				tailNotePoint.position += noteLength;
				thisNote.notePoint.push_back(tailNotePoint);
				addLongNote(thisNote);
			}
			else
			{
				addNormalNote(thisNote);
			}
		}

		//AUDIO Resource
		std::map<int,string> audioResIDMap;
		while(true)
		{
			int wavResId;
			fwscanf(readFile,L"%d",&wavResId);
			if(wavResId==-1)
				break;
			
			std::string addedID = resource_add(checkTailEnter(fgetws(tempbuffer,sizeof(tempbuffer),readFile)));
			audioResIDMap[wavResId] = addedID;
			if(wavResId == mainSoundResId)
				contructMapInfo.header.mainSound = addedID;
		}

		//VIDEO IMAGE Resource
		std::map<int,string> videoResIDMap;
		while(true)
		{
			int resId;
			fwscanf(readFile,L"%d",&resId);
			if(resId==-1)
				break;

			std::string addedID = resource_add(checkTailEnter(fgetws(tempbuffer,sizeof(tempbuffer),readFile)));
			videoResIDMap[resId] = addedID;
		}

		//ChanceTime
		if(thisVer >= L"1.0.1.0")
		{
			int ct_begin,ct_end;
			fwscanf(readFile,L"%d%d",&ct_begin,&ct_end);
		}

		//Stop
		if(thisVer >= L"1.0.4.2")
		{
			while(true)
			{
				int pos;
				fwscanf(readFile,L"%d",&pos);
				if(pos==-1)
					break;
				int stopLength;
				fwscanf(readFile,L"%d",&stopLength);
				stop_insert(pos-beginPos,stopLength);
			}
		}

		fclose(readFile);


		//Deal with event
		for (std::map<int,double>::iterator i=bpmEvent.begin();i!=bpmEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			bpm_insert(thisPos,i->second);
		}

		for (std::map<int,vector<int>>::iterator i=resourceEvent.begin();i!=resourceEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			for (vector<int>::iterator eventI = i->second.begin();eventI!=i->second.end();eventI++)
			{
				if(videoResIDMap.find(*eventI)==videoResIDMap.end())
					continue;
				std::string resourceID = videoResIDMap[(*eventI)];
				int eventNo = resourceEvent_add(thisPos, resourceID);
				if(coreInfoPtr->resources[resourceID].type == divacore::MapResourceInfo::IMAGE)
				{
					coreInfoPtr->events[eventNo].arg["tex_x"] = 0;
					coreInfoPtr->events[eventNo].arg["tex_y"] = 0;
					coreInfoPtr->events[eventNo].arg["tex_width"] = 480;
					coreInfoPtr->events[eventNo].arg["tex_height"] = 272;
					coreInfoPtr->events[eventNo].arg["label"] = std::string("bg_image");
				}
			}
		}

		for (std::map<int,vector<int>>::iterator i=bgmEvent.begin();i!=bgmEvent.end();i++)
		{
			int thisPos = i->first-beginPos;
			if(thisPos<0) thisPos=0;
			for (vector<int>::iterator eventI = i->second.begin();eventI!=i->second.end();eventI++)
			{
				if(audioResIDMap.find(*eventI)!=audioResIDMap.end())
					resourceEvent_add(thisPos, audioResIDMap[(*eventI)]);
			}
		}

		for(int i=0;i<coreInfoPtr->notes.size();i++)
		{
			for (int j=0;j<coreInfoPtr->notes[i].notePoint.size();j++)
			{
				int keyNum = sToI(coreInfoPtr->notes[i].notePoint[j].key);
				if(audioResIDMap.find(keyNum)!=audioResIDMap.end())
					coreInfoPtr->notes[i].notePoint[j].key = audioResIDMap[keyNum];
				else
					coreInfoPtr->notes[i].notePoint[j].key = "";
			}
		}

#pragma endregion Read DivaPro File

		//coreInfoPtr->header.noter.clear();

		if(!SaveFile())
			return LOCALIZATION->getLocalStr(L"ReadFile_SaveFileError", (workingDirectory + L"/" + workingDivaOLProFile).c_str());

		ResetEditorMapData();
		registerMapInfo(NULL);

		return InitFromDivaOLProFile(workingDirectory + L"/" + workingDivaOLProFile);
	}


	std::wstring DivaEditorMapData::InitFromMusicFile(std::wstring path)
	{
		std::wstring chooseWorkingFileResult = ChooseWorkingFile();
		if(chooseWorkingFileResult==L"OK")
		{
			divacore::MapInfo contructMapInfo;
			registerMapInfo(&contructMapInfo);

			//Need to copy file now
			contructMapInfo.header.mainSound = resource_add(path);

			divacore::MapEvent initBPM;
			initBPM.position = 0;
			initBPM.eventType = "bpm";
			initBPM.arg["value"] = 120.0;

			contructMapInfo.events.push_back(initBPM);
			
			//Init header
			contructMapInfo.header.BPM = 120.0;
			contructMapInfo.header.difficulty = 0;
			contructMapInfo.header.hardLevel = 0;
			contructMapInfo.header.mapName = L"no name";
			contructMapInfo.header.speedScale = 1;
			contructMapInfo.header.versionStr = EditorVer;

			//Init module
			contructMapInfo.module = "standard";

			
			if(!SaveFile())
			{
				std::string soundID = coreInfoPtr->header.mainSound = "";
				resource_delete(soundID);
				bpm_delete(0);

				return LOCALIZATION->getLocalStr(L"ReadFile_SaveFileError", (workingDirectory + L"/" + workingDivaOLProFile).c_str());
			}

			//Init editor default value
			ResetEditorMapData();

			registerMapInfo(NULL);
			CORE_PTR->setSong(workingDirectory,workingDivaOLFile);
			return L"OK";
		}
		else
			return chooseWorkingFileResult;
		
	}
	

}

