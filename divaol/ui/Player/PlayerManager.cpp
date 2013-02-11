#include "ui/Player/PlayerManager.h"
#include "ui/Config/DivaUIConfig.h"

namespace diva
{
	PlayerManager::PlayerManager()
	{
		PlayerInfo t;
		//ReadJsonFile("");
		WJson::Value tv = config[L"stageGuests"];
		for (WJson::Value::iterator i = tv.begin(); i != tv.end(); i++)
		{
			t.id = (*i)[L"id"].asInt();
			t.nickname = (*i)[L"nickname"].asString();
			t.username = (*i)[L"username"].asString();
			stageGuests.push_back(t);
		}
		bOnline = false;
	}

	PlayerManager::~PlayerManager()
	{
		
	}

	const PlayerInfo& PlayerManager::GetHostInfo()
	{
		return host;
	}

	void PlayerManager::SetHostInfo(const PlayerInfo& v)
	{
		host = v;
	}

	std::vector<PlayerInfo>& PlayerManager::GetStageGuests()
	{
		return stageGuests;
	}

	std::vector<PlayerInfo>& PlayerManager::GetHouseGuests()
	{
		return houseGuests;
	}

	bool PlayerManager::IsOnline()
	{
		return bOnline;
	}

	void PlayerManager::SetOnline(bool v)
	{
		bOnline = v;
	}
}