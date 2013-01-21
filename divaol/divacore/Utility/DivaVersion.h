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

namespace divacore
{
	class Version : public Base::Singleton<Version>
	{
	private:
		friend class Base::Singleton<Version>;

		Version():state(UNINITIALIZED) {RequireVersion();}
		~Version() {}

	public:
		enum State{UNINITIALIZED, REQUIREING, READY};

		void RequireVersion() 
		{
			if(state != UNINITIALIZED)
				return;

			applicationPath = Base::Path::GetApplicationPath(true);

			CheckVersion();
		}
		bool CheckVersion()
		{
			uint64 nowTime = Base::TimeUtil::currentTime();
			MD52 md5(applicationPath);
			Base::String md5str(md5.toString());

			if(md5str == md5Value)
				return true;

			return false;
		}

	protected:
		int state;
		Base::String version;
		Base::String md5Value;
		Base::String applicationPath;
	};

#define VERSION (divacore::Version::instance())
}

#endif