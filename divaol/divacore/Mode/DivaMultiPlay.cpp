/*
*  DivaMultiPlay.cpp
*
*  Created by Hyf042 on 8/9/12.
*  Copyright 2012 Hyf042. All rights reserved.
*
*/

#include "DivaMultiPlay.h"
#include "Component/DivaCommonEvaluateStrategy.h"
#include "divanetwork/DivaNetworkManager.h"

namespace divacore
{
	void NetGameInfo::setConfig(const std::string &configFile)
	{
		mConfig.clear();
		configloader::loadWithJson(mConfig,configFile);
	}
	void NetGameInfo::update(float dt)
	{
		mPlayers[myPlayerID].score = mOwner->getScore();
		mPlayers[myPlayerID].hp = mOwner->getHPinRatio();
		mPlayers[myPlayerID].combo = mOwner->getCombo();
		mTeams[myTeamID].combo = mOwner->getCombo();
		mTeams[myTeamID].score = 0;
		for(int i = 0; i < mTeams[myTeamID].players.size(); i++)
			mTeams[myTeamID].score += mPlayers[mTeams[myTeamID].players[i]].score;
	}
	void NetGameInfo::updateInfoFromPacket(GPacket *packet)
	{
		gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(packet->getItem(2));

		for(int i = 0; i < mPlayers.size(); i++)
			if(i!=myPlayerID)
			{
				GPacket *player = dynamic_cast<GPacket*>(list->getItem(i));
				mPlayers[i].score = gnet::ItemUtility::getUInt(player->getItem(1));
				mPlayers[i].combo = gnet::ItemUtility::getUInt(player->getItem(2));
				mPlayers[i].hp = gnet::ItemUtility::getValue(player->getItem(3));
			}
			updateTeamInfo();
	}
	void NetGameInfo::updateTeamInfo()
	{
		for(int i = 0; i < mTeams.size(); i++)
		{
			mTeams[i].score = 0;
			mTeams[i].combo = mPlayers[mTeams[i].nowPlayer].combo;
			for(int j = 0; j < mTeams[i].players.size(); j++)
				mTeams[i].score += mPlayers[mTeams[i].players[j]].score;
		}
	}

	void NetGameInfo::newGame(GPacket *packet)
	{
		gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(dynamic_cast<GPacket*>(packet->getItem(3))->getItem(1));
		mPlayers = PLAYERS(list->size());
		mTeams = TEAMS(packet->getItem(4)->getInt());

		for(int i = 0; i < mPlayers.size(); i++)
		{
			PlayerInfo playerInfo;

			gnet::Item<gnet::List> *playerList = dynamic_cast<gnet::Item<gnet::List>*>(list->getItem(i));

			playerInfo.uid = playerList->getItem(0)->getString();
			playerInfo.index = playerList->getItem(1)->getInt()-1;
			playerInfo.teamIndex = playerList->getItem(2)->getInt()-1;
			playerInfo.indexInTeam = playerList->getItem(3)->getInt()-1;

			mPlayers[i] = playerInfo;
			mTeams[playerInfo.teamIndex].nowPlayer = 0;
			mTeams[playerInfo.teamIndex].players.push_back(i);

			if(MY_PLAYER_INFO.uid()==playerInfo.uid)
			{
				myPlayerID = playerInfo.index;
				myTeamID = playerInfo.teamIndex;
				myPlayerPtr = &mPlayers[myPlayerID];
				myTeamPtr = &mTeams[myTeamID];
			}
		}
	}

	void MultiPlay::init() 
	{
		//debug info
		mText.setColor(CARGB(255,255,0,0));
		mText.setFont(sora::SoraFont::LoadFromFile("simhei.ttf", 50));
	}

	void MultiPlay::registerNetworkEvent() {
		//ע����պ���
		GNET_RECEIVE_PACKET("game#membersinfoL",&MultiPlay::gnetMembersInfo);
		GNET_RECEIVE_PACKET("game#playerupdateL",&MultiPlay::gnetPlayerUpdate);
		//GNET_RECEIVE_PACKET("stage#join_failed",&MultiPlay::gnetJoinFailed);
		//GNET_RECEIVE_PACKET("stage#join_ok",&MultiPlay::gnetJoinOK);

		if(mInfo==0) {
			mInfo = new NetGameInfo();
			mInfo->setOwner(this);
		}

		setBaseState(CONNECT);
	}

	void MultiPlay::gameReset() {

		SinglePlay::gameReset();
	}
	void MultiPlay::gameStop() {
		//NETWORK_SYSTEM_PTR->disconnect();

		SAFE_DELETE(mInfo);

		STAGE_CLIENT.back();
	}
	void MultiPlay::gameOver()
	{
		sendInfo();

		GNET_UNRECEIVE_PACKET("game#membersinfoL");
		GNET_UNRECEIVE_PACKET("game#playerupdateL");
		//GNET_UNRECEIVE_PACKET("stage#join_failed");
		//GNET_UNRECEIVE_PACKET("stage#join_ok");

		NETWORK_SYSTEM_PTR->send("game#overR");
	}
	void MultiPlay::setMyInfo(Config &config)
	{
		mInfo->mConfig = config;
	}
	void MultiPlay::setMyInfo(const std::string &configFile)
	{
		mInfo->setConfig(configFile);
	}
	void MultiPlay::inform(StateEvent& event)
	{
		if(event.note->isOwner())
		{
			SinglePlay::inform(event);

			if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
			{
				//if(event.breakCombo||event.breakNote)
				//	sendFailure(event);
				NETWORK_SYSTEM_PTR->send("game#evalR",
					"%d%d",event.note->getID(), event.rank);
			}
		}
	}
	void MultiPlay::gameLoad(const std::string &configFile) {
		if(configFile!="")
			mInfo->setConfig(configFile);

		//����server
		/*NETWORK_SYSTEM_PTR->connect();

		NETWORK_SYSTEM_PTR->send("auth#setuid","%s",MY_PLAYER_INFO.uid().c_str());

		NETWORK_SYSTEM_PTR->send("stage#join","%s","919");*/

		//NETWORK_SYSTEM_PTR->send("stage#ready");

		while(getBaseState()==CONNECT)
		{
			NETWORK_SYSTEM_PTR->waitForNext();

			NETWORK_SYSTEM_PTR->refresh();
		}

		if(getBaseState()!=GET_INFO&&getBaseState()!=READY)
		{
			NETWORK_SYSTEM_PTR->disconnect();
			return;
		}

		while(getBaseState()!=READY)
		{
			NETWORK_SYSTEM_PTR->waitForNext();

			NETWORK_SYSTEM_PTR->refresh();
		}

		//׼�����
		NETWORK_SYSTEM_PTR->send("game#systemready");
	}

	void MultiPlay::gameStart() {
		//��Ϸ��ʼ
		setBaseState(PLAY);
	}

	void MultiPlay::update(float dt) {
		//receivePacket();
		//updateState(dt);
		if(getBaseState()==PLAY)
		{
			//�����������͵�ǰ״̬
			DEAL_PER_PERIOD_BEGIN(dt,1.0);
			sendInfo();
			DEAL_PER_PERIOD_END();

			mInfo->update(dt);
		}
	}

	void MultiPlay::sendInfo()
	{
		////��serverͬ��score��hp��combo��Ϣ������Ϣ��Ϊ������ʾ�ã����������ݴ���
		NETWORK_SYSTEM_PTR->send("game#heartbeatR",
			"%f%d%d%f",CORE_PTR->getRunPosition(),getScore(),getCombo(),getHPinRatio());
	}

	//�����Ϸ��Ϣ��������Ϣ
	void MultiPlay::gnetMembersInfo(GPacket *packet)
	{
		mInfo->newGame(packet);

		for(int i = 0; i < mInfo->mPlayers.size(); i++)
			mInfo->mPlayers[i].hp = float(ORIGIN_HP)/MAX_HP;

		setBaseState(READY);
	}

	//������������Ϸ��Ϣ
	void MultiPlay::gnetPlayerUpdate(GPacket *packet)
	{
		mInfo->updateInfoFromPacket(packet);
	}

	void MultiPlay::gnetJoinFailed(GPacket *packet)
	{
		setBaseState(FAILURE);
	}

	void MultiPlay::gnetJoinOK(GPacket *packet)
	{
		setBaseState(GET_INFO);
	}

	void MultiPlay::render()
	{
		if(getBaseState()==PLAY)
		{
#ifdef _DEBUG
			//��ʾһЩ������Ϣ
			std::string output = format("%d Players",mInfo->mPlayers.size());

			for(int i = 0; i < mInfo->mPlayers.size(); i++)
				output += format("player%d score:%d\tcombo:%d\n",i+1,mInfo->mPlayers[i].score,mInfo->mPlayers[i].combo);

			mText.setText(sora::s2ws(output));
			mText.renderTo(200,200);
#endif
		}
	}

	void MultiPlay::preStart()
	{
		if(CORE_PTR->getState()!=Core::RESULT)
			UI_PAINTER_PTR->addWidget("multiPlayer");
	}

	void MultiPlay::preEvaluate()
	{
		EVALUATE_STRATEGY_PTR->getResult().myScore = getScore();
		EVALUATE_STRATEGY_PTR->getResult().myID = getPlayerID();

		PLAYERS &players = getPlayerInfo();

		for(int i = 0; i < players.size(); i++)
			EVALUATE_STRATEGY_PTR->getResult().evalData.push_back(EvalData(players[i].uid, players[i].score, i));
		//((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();
	}
}
