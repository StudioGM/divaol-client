#pragma once

#include <vector>
#include <string>

namespace diva
{
	struct DivaPlayerInfo
	{
		int id;
		bool isHost;
		bool isReady;
		bool isVoid;
		bool isPlaying;
		int slot;
		std::wstring name;
		int iconIndex;
	};

	struct DivaRoomInfo
	{
		std::vector<DivaPlayerInfo> players;
		int hostId;
		int myId;
		bool isSuccessful;
	};

	struct PlayerInfo
	{
		int id;
		std::wstring username;
		std::wstring nickname;
	};

	namespace Net
	{
		class Network
		{
		private:
			static std::vector<std::wstring> msgs;
			static std::vector<std::wstring> args;

		public:
			static bool isEmpty();

			static void Send(std::wstring msg_name, std::wstring arg);
			static void Recv(std::wstring& msg_name, std::wstring& arg);

		};


	}
}