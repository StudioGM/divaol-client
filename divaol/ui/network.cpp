#include "network.h"

namespace Network
{

	LoginResult Login(std::string username, std::string password)
	{
		Sleep(2000);
		LoginResult res;
		if (username != "sonicmisora")
		{
			res.result = LOGIN_NONEXITUSERNAME;
			return res;
		}
		if (password != "123456")
		{
			res.result = LOGIN_PSWWRONG;
			return res;
		}
		res.result = LOGIN_SUCCESS;
		res.hostInfo.Gold = 100;
		res.hostInfo.Username = "sonicmisora";
		res.hostInfo.Nickname = "SonicMisora";
		res.hostInfo.PlayerID = 1;
		res.hostInfo.RoomID = 100;
		return res;
	}

	//DateTime DivamapDAO::getLatestMaps(std::vector<Map> &maps, DateTime lastUpdate)
	//{
	//	maps.clear();
	//	Map m1;
	//	m1.artists.push_back("ni");
	//	m1.composers.push_back("ta");
	//	m1.description = "bucuo";
	//	m1.directLinkURLs.push_back("google.cn");
	//	m1.downloadCounts = 2;
	//	m1.lastUpdate.yyyy = 2012;
	//	m1.lastUpdate.mm = 1;
	//	m1.lastUpdate.dd = 1;
	//	m1.lastUpdate.HH = 12;
	//	m1.lastUpdate.MM = 9;
	//	m1.lastUpdate.SS = 1;
	//	m1.lyricAuthors.push_back("wo");
	//	m1.mapAliasNames.push_back("fuckyou");
	//	m1.mapId = 12;
	//	m1.mapThumbURLs.push_back("www.baidu.com");
	//	m1.netdiskLinkURLs.push_back("pruck");
	//	m1.noterId = 12;
	//	m1.origName = "nibucuoa";
	//	m1.vocaloidUsages.push_back("clear");

	//	maps.push_back(m1);

	//	m1.lastUpdate.SS = 5;
	//	return m1.lastUpdate;
	//}
}