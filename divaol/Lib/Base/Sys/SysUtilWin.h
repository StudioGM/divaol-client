/****************************************************
 *  SysUtilWin.h
 *
 *  Created by tempbuffer on 9/16/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A bunch of things that are related to windows 
 ****************************************************/

#if !defined(_BASE_SYS_UTIL_) && defined(BASE_OS_WINDOWS)
#define _BASE_SYS_UTIL_

#include "../Common.h"
#include "../Uncopyable.h"

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

namespace Base
{
	class FileSystemUtilImpl
	{
	public:
		static String GetEnvironment(const String &env) {
			DWORD len = GetEnvironmentVariableW(env.unicode_str(), 0, 0);
			if (len != 0) {
				wchar* buffer = new wchar[len];
				GetEnvironmentVariableW(env.unicode_str(), buffer, len);
				String result(buffer);
				delete [] buffer;
				return result;
			}
			return "";
		}
		static bool FileExist(const String& path) {
			return PathFileExistsW(path.unicode_str())?true:false;
		}
		static bool PathExist(const String& path) {
			return PathFileExistsW(path.unicode_str())?true:false;
		}
		static String GetApplicationPath() {
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectoryW(MAX_PATH, buffer);

			return String(buffer)+L'\\';
		}
		static String GetWindowsDirectory() {
			wchar_t buffer[_MAX_PATH];
			::GetWindowsDirectoryW(buffer, _MAX_PATH-1);
			return buffer;
		}
		static String GetFontPath() {
			return GetWindowsDirectory() + "\\Fonts\\";
		}
		static String GetCurrentPath() {
			wchar_t buffer[_MAX_PATH];
			DWORD n = GetCurrentDirectoryW(sizeof(buffer), buffer);
			if(n > 0 && n < sizeof(buffer)) {
				String result(buffer, n);
				if(result[n-1] != L'\\')
					result += L'\\';
				return result;
			}
			return "";
		}
		static String GetHome() {
			String result = GetEnvironment("HOMEDRIVE");
			result += GetEnvironment("HOMEPATH");

			size_t n = result.size();
			if(n > 0 && result[n-1] != L'\\')
				result += L'\\';
			return result;
		}
		static String GetRoot() {
			return ".\\";
		}
		static String GetTempPath() {
			wchar_t buffer[_MAX_PATH];
			DWORD n = ::GetTempPathW(sizeof(buffer), buffer);
			if(n > 0 && n < sizeof(buffer)) {
				String result(buffer, n);
				if(result[n-1] != L'\\')
					result += L"\\";
				return result;
			} 
			return ".\\";
		}
		static String GetWrittablePath() {
			return ".\\";
		}
		static String GetResourcePath() {
			return ".\\";
		} 
		static std::vector<String> GetDriveList()
		{
			std::vector<String> list;

			char buffer[128];
			DWORD n = GetLogicalDriveStringsA(sizeof(buffer)-1, buffer);
			char* it = buffer;
			char* end = buffer + (n > sizeof(buffer) ? sizeof(buffer): n);
			while(it < end) {
				std::string dev;
				while(it < end && *it)
					dev += *it++;
				list.push_back(dev);
				++it;
			}

			return list;
		}
		static String ExpandPath(const String& path) {
			wchar_t buffer[_MAX_PATH];
			DWORD n = ExpandEnvironmentStringsW(path.unicode_str(), buffer, sizeof(buffer));
			if(n > 0 && n < sizeof(buffer)) {
				return String(buffer, n-1);
			} else
				return path;
		}
		static bool SetCurrentPath(const String &path) {
			return SetCurrentDirectoryW(path.unicode_str())?true:false;
		}
	};

	// win mutex
	class MutexImpl : public Uncopyable
	{
	protected:
		MutexImpl() {
			mutex = CreateMutexW(NULL,FALSE,NULL);
		}
		~MutexImpl() {
			CloseHandle(mutex);
		}
		inline void lockImpl() {
			WaitForSingleObject(mutex,INFINITE);
		}
		inline void unlockImpl() {
			ReleaseMutex(mutex);
		}

	private:
		HANDLE mutex;
	};
}

#endif