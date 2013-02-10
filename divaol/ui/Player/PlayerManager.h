#pragma once

#include "ui/DivaNetwork/DivaNetwork.h"


namespace diva
{
	class PlayerManager
	{
	private:
		PlayerInfo host;
		std::vector<PlayerInfo> stageGuests;
		std::vector<PlayerInfo> houseGuests;
		bool bOnline;

		PlayerManager();
	public:
		~PlayerManager();
		static PlayerManager* Instance()
		{
			static PlayerManager p;
			return &p;
		}

		const PlayerInfo& GetHostInfo();
		void SetHostInfo(const PlayerInfo& v);
		void ClearHostInfo() {host.id = 0, host.nickname = host.username = L"";}

		std::vector<PlayerInfo>& GetStageGuests();
		std::vector<PlayerInfo>& GetHouseGuests();

		bool IsOnline();
		
		// test
		void SetOnline(bool v);
	};
}