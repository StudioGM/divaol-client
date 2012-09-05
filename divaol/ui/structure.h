#pragma once

#include <string>
#include <vector>

namespace Network
{
	struct HostInfo
	{
		std::string Nickname;
		std::string Username;
		int PlayerID;
		int Gold;
		int RoomID;
	};

	enum {LOGIN_SUCCESS, LOGIN_OVERTIME, LOGIN_NONEXITUSERNAME, LOGIN_PSWWRONG};

	struct LoginResult
	{
		int result;
		HostInfo hostInfo;
	};

	struct DateTime {
		short int yyyy, mm, dd, HH, MM, SS;
	};

	struct Map {
		int mapId;
		int noterId;
		std::wstring noterName;
		std::wstring origName;
		std::wstring description;
		int downloadCounts;
		// Newly Modified by SonicMisora @ 2012/3/24
		int length;
		int players;
		int bpm;
		std::vector<std::wstring> difNames;
		std::vector<int> difPercents;
		std::wstring mapThumbFileName;
		std::wstring listenFileName;
		std::wstring songPath;
		std::wstring songFileName;
		// End
		DateTime lastUpdate;
		std::vector<std::wstring> directLinkURLs;
		std::vector<std::wstring> netdiskLinkURLs;
		std::vector<std::wstring> mapAliasNames;
		std::vector<std::wstring> mapThumbURLs;
		std::vector<std::wstring> composers;
		std::vector<std::wstring> artists;
		std::vector<std::wstring> lyricAuthors;
		std::vector<std::wstring> vocaloidUsages;
	};

}
