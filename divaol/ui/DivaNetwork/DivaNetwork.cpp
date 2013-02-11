#include "ui/DivaNetwork/DivaNetwork.h"

#include <cstring>

namespace diva
{
	namespace Net
	{
		std::vector<std::wstring> Network::msgs;
		std::vector<std::wstring> Network::args;

		void Network::Send(std::wstring msg_name, std::wstring arg)
		{
			if (msg_name == L"LOGIN")
			{
				wchar_t username[100], password[100];
				swscanf(arg.c_str(), L"%s%s", username, password);
				msgs.push_back(L"LOGIN_RET");
				if (wcscmp(username, L"sonicmisora"))
				{
					args.push_back(L"0 No_such_username");
					return;
				}
				if (wcscmp(password, L"123456"))
				{
					args.push_back(L"0 Wrong_Password");
					return;
				}
				args.push_back(L"1 sonicmisora SonicMisora 1");
			}
		}

		void Network::Recv(std::wstring& msg_name, std::wstring& arg)
		{
			if (msgs.empty())
				throw "illegal operation.";
			msg_name = msgs[msgs.size()-1];
			msgs.pop_back();
			arg = args[args.size()-1];
			args.pop_back();
		}

		bool Network::isEmpty()
		{
			return msgs.empty();
		}
	}
}