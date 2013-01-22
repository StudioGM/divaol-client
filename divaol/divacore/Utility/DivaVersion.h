/*
 *  DivaVersion.h
 *
 *  Created by Hyf042 on 1/21/13.
 *  Copyright 2013 Hyf042. All rights reserved.
 *
 *  Store the version of game and do version check
 */

#ifndef DIVA_VERSION
#define DIVA_VERSION

#include "Core/DivaCommon.h"
#include "Core/DivaCore.h"
#include "Lib/Base/Base.h"
#include "Lib/MD5/md52.h"
#include "divasongmgr/divamapmanager.h"
#include "SoraAutoUpdate.h"

namespace divacore
{
	class Version : public Base::Singleton<Version>, public sora::SoraAutoUpdate
	{
	private:
		friend class Base::Singleton<Version>;

		Version():state(UNINITIALIZED) {
		}
		~Version() {}

	public:
		enum State{UNINITIALIZED, REQUIREING, UNREADY, READY};

		int getState() {return state;}

		void RequireVersion() 
		{
			if(state != UNINITIALIZED && state != UNREADY)
				return;

			applicationPath = Base::Path::GetApplicationPath(true);

			MAPMGR.PrepareDownloadFile(L"http://divaol.com/update/divaol.json");

			state = REQUIREING;
		}
		bool CheckVersion()
		{
			std::wstring path = applicationPath;
			MD52 md5(path);
			Base::String md5str(md5.toString());

			if(md5str == md5Value)
				return true;

			state = UNREADY;
			return false;
		}
		void onUpdate(float dt) {
			if(state == REQUIREING) {
				MAPQUEUE* q = MAPMGR.GetQueue();
				if(q == NULL)
				{
					state = UNREADY;
					return;
				}
				while (!q->empty())
				{
					const divamap::DivaMapEventMessage &t = (*((*q).begin()));
					switch (t.eventType)
					{
					case divamap::DivaMapEventMessage::DownloadFile:
						if(t.finish)
						{
							if(!t.error)
							{
								Json::Reader reader;
								Json::Value value;
								if(reader.parse(t.additionalMessage, value))
								{
									if(value.isMember("version"))
									{
										version = value["version"].asString();
										if(value.isMember("files")&&value["files"].isMember("divaol.exe_zlib")) {
											md5Value = value["files"]["divaol.exe_zlib"].asString();
											q->pop_front();
											state = READY;
											return;
										}
									}
								}
							}
							q->pop_front();
							state = UNREADY;
							return;
						}
						break;
					}
					q->pop_front();	
				}
			}
		}

	protected:
		int state;
		Base::String version;
		Base::String md5Value;
		Base::String errorCode;
		Base::String applicationPath;
	};

#define VERSION (divacore::Version::instance())
}

#endif