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
#include "FileUtil.h"

namespace Base
{
	class DirectoryIterator;

	class Path
	{
	public:
		Path() {}
		Path(const String &path) {mPath = FormatPath(path);}
		Path(const char *path) {mPath = FormatPath(path);}
		Path(const wchar *path) {mPath = FormatPath(path);}
		operator String() const {return mPath;}

		const String& str() const {return mPath;}
		String fileName(bool hasExt = true) const {return GetFileName(mPath, hasExt);}
		Path filePath() const {return GetFilePath(mPath);}
		Path folderPath() const {return GetFolderPath(mPath);}
		String extension() const {return GetExtension(mPath);}
		String pathRoot() const {return GetPathRoot(mPath);}
		Path fullPath() const {return GetFullPath(mPath);}
		bool isAbsolute() const {return CheckAbsolutePath(mPath);}
		bool hasExtension() const {return CheckHasEntension(mPath);}
		Path firstComponent() const {return mPath(0, mPath("/"));}
		Path removeFirstComponent() const {return mPath(mPath("/")+1,-1);}
		Path& changeExtension(const String &ext) {
			*this = ChangeExtension(*this,ext); 
			return *this;
		}
		DirectoryIterator beginEnum();
		std::vector<Path> enumFolder(bool recursion = false) {
			return EnumFolder(mPath, recursion);
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
		static Path GetApplicationPath(bool full = false);

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
		static Path GetFolderPath(const String &path);
		static Path CombinePath(const Path &a, const Path &b);
		static String GetExtension(const String &path);
		static Path GetFullPath(const String &path);
		static String GetPathRoot(const Path &path);
		static Path ChangeExtension(const Path &path, const String &ext);
		static Path PickoutFinalSlash(const String &path);

		std::vector<Path> EnumFolder(const String &folderPath, bool recursion = false);

		static bool CheckAbsolutePath(const Path &path);
		static bool CheckHasEntension(const Path &path);
		static bool CheckIsFolder(const Path &path);
	protected:
		static std::vector<Path> EnumFolder(const String &folderPath, std::vector<Path> &files, bool recursion);
	private:
		String mPath;
	};

	class DirectoryIterator {
	public:
		DirectoryIterator();
		DirectoryIterator(const DirectoryIterator &iter);
		explicit DirectoryIterator(const String &path);
		~DirectoryIterator();

		const String& file() const {return mFile;}
		const String& folderPath() const {return mFolderPath.str();}
		String fullPath() const {return Path::CombinePath(mFolderPath,mFile).fullPath();}
		String path() const {return Path::CombinePath(mFolderPath,mFile);}

		bool isEnd() const {return mIsEnd;}
		bool isFolder() const {return mImpl->isFolder();}
		void reset() {mImpl = new DirectoryIteratorImpl(mFolderPath), mFile = "", mIsEnd = false;}

		DirectoryIterator& operator=(const DirectoryIterator &it);
		DirectoryIterator& operator++();

		bool operator==(const DirectoryIterator &rhs) const {return file() == rhs.file();}
		bool operator!=(const DirectoryIterator &rhs) const {return file() != rhs.file();}

	private:
		Path mFolderPath;
		String mFile;
		bool mIsEnd;
		SharedPtr<DirectoryIteratorImpl> mImpl;
	};
}

#endif