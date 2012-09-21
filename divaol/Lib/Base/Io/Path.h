/****************************************************
 *  Path.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of funs that deal with path
 ****************************************************/

#ifndef _BASE_PATH_H_
#define _BASE_PATH_H_

#include "../Common.h"
#include "Stream.h"

namespace Base
{
	class Path
	{
	public:
		Path() {}
		Path(const String &path) {mPath = FormatPath(path);}
		Path(const char *path) {mPath = FormatPath(path);}
		Path(const wchar *path) {mPath = FormatPath(path);}
		operator String() const {return mPath;}

		String str() {return mPath;}
		String fileName() const {return GetFileName(mPath);}
		Path filePath() const {return GetFilePath(mPath);}
		String extension() const {return GetExtension(mPath);}
		String pathRoot() const {return GetPathRoot(mPath);}
		bool isAbsolute() const {return CheckAbsolutePath(mPath);}
		bool hasExtension() const {return CheckHasEntension(mPath);}
		Path& changeExtension(const String &ext) {
			*this = ChangeExtension(*this,ext); 
			return *this;
		}

		friend Path operator+(const Path &a, const Path &b){
			return CombinePath(a,b);
		}
		Path& operator+=(const Path &other) {
			*this = *this+other;
			return *this;
		}
	public:
		static Path GetEnvironment(const String &env);
		static Path GetApplicationPath();

		static Path GetFontPath();

		static Path GetCurrentPath();
		static Path GetHome();
		static Path GetRoot();
		static Path GetTempPath();
		static Path GetWrittablePath();
		static Path GetResourcePath();

		static bool SetCurrentPath(const String &path);

		static std::vector<String> GetDriveList();
		static Path ExpandPath(const String& path);

		// tools
		static String FormatPath(const String &path);
		static String GetFileName(const String &path, bool hasExt = true);
		static Path GetFilePath(const String &path);
		static Path CombinePath(const Path &a, const Path &b);
		static String GetExtension(const String &path);
		static Path GetFullPath(const String &path);
		static String GetPathRoot(const Path &path);
		static Path ChangeExtension(const Path &path, const String &ext);

		static bool CheckAbsolutePath(const Path &path);
		static bool CheckHasEntension(const Path &path);
	private:
		String mPath;
	};
}

#endif