#include "ui/DivaGUI/DivaPlayerList.h"

#include "ui/Config/DivaUIConfig.h"
#include "ui/roomUI.h"

namespace diva
{
	DivaPlayerList::DivaPlayerList()
	{
		setGap(gcn::Rectangle(0, 0, 462, 116), 0);
		setMaxItem(4);
		setSize(462, 116*4);
		for (int i=0; i<config[L"iconFileNames"].size(); i++)
			iconList.push_back(config[L"iconFileNames"][i].asString());
		info.players.clear();
		info.hostId = 0;
		info.myId = config[L"myId"].asInt();
	}

	DivaPlayerList::~DivaPlayerList()
	{
		
	}

	void DivaPlayerList::setPlayerReady(int id, bool v)
	{
		//((DivaPlayerItem*)items[id])->setReady(v);
		
		// SMDivaNetCode
		// CodeEnd
	}

	void DivaPlayerList::ModifyPlayer(int index, const DivaPlayerInfo& inf)
	{
		info.players[index] = inf;
		delete items[index];
		removeItem(index);

		DivaPlayerItem* p = new DivaPlayerItem();
		p->setHost(inf.isHost);
		p->setInfo(inf.name, iconList[inf.iconIndex], gcn::Rectangle(0,0,0,0), L"UI2.png");
		p->setReady(inf.isReady);
		p->setPlaying(inf.isPlaying);
		p->setVoid(inf.isVoid);
		insertItem(p, index);
	}

	DivaRoomInfo& DivaPlayerList::getRoomInfo()
	{
		return info;
	}

	DivaRoomInfo DivaPlayerList::login()
	{
		DivaRoomInfo ret;
		ret.isSuccessful = true;
		ret.hostId = 0;
		ret.myId = 0;
		ret.players.clear();
		//players
		for (int i = 0; i < 1; i++)
		{
			DivaPlayerInfo player;
			player.iconIndex = i;
			player.id = i;
			player.isHost = (i == 0);
			player.isReady = false;
			player.isPlaying = false;
			player.isVoid = false;
			player.name = config[L"myName"].asString();
			ret.players.push_back(player);
		}
		return ret;
	}

	void DivaPlayerList::init()
	{
		info = login();
		if (!info.isSuccessful)
		{
			throw "login unsuccessful!";
		}

		for (int i = 0; i < info.players.size(); i++)
		{
			DivaPlayerItem* p = new DivaPlayerItem();
			p->setHost(info.players[i].isHost);
			p->setInfo(info.players[i].name, iconList[info.players[i].iconIndex], gcn::Rectangle(0,0,0,0), L"UI2.png");
			p->setReady(info.players[i].isReady);
			p->setVoid(info.players[i].isVoid);
			p->setPlaying(info.players[i].isPlaying);
			pushItem(p);
		}
	}

}