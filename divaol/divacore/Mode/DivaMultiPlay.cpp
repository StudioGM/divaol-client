/*
 *  DivaMultiPlay.cpp
 *
 *  Created by Hyf042 on 8/9/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaMultiPlay.h"
#include "Component/DivaCommonEvaluateStrategy.h"

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

	void MultiGameInfo::newGame(GPacket *packet)
	{
		gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(dynamic_cast<GPacket*>(packet->getItem(3))->getItem(1));
		mPlayers = PLAYERS(list->size());
		mTeams = TEAMS(list->size());

		for(int i = 0; i < mPlayers.size(); i++)
		{
			PlayerInfo playerInfo;

			playerInfo.uid = list->getItem(i)->getString();
			playerInfo.index = i;
			playerInfo.teamIndex = i;
			playerInfo.indexInTeam = 0;

			mPlayers[i] = playerInfo;
			mTeams[playerInfo.teamIndex].nowPlayer = 0;
			mTeams[playerInfo.teamIndex].players.push_back(i);

			if(mConfig.getAsString("uid")==playerInfo.uid)
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
		 void MultiPlay::gameReset() {
			//注册接收函数
			GNET_RECEIVE_PACKET("game#membersinfo",&MultiPlay::gnetMembersInfo);
			GNET_RECEIVE_PACKET("game#playerupdate",&MultiPlay::gnetPlayerUpdate);
			GNET_RECEIVE_PACKET("stage#join_failed",&MultiPlay::gnetJoinFailed);
			GNET_RECEIVE_PACKET("stage#join_ok",&MultiPlay::gnetJoinOK);

			SinglePlay::gameReset();
			
			if(mInfo==0) {
				mInfo = new MultiGameInfo();
				mInfo->setOwner(this);
			}
			
			setBaseState(CONNECT);
		}
		 void MultiPlay::gameStop() {
			GNET_UNRECEIVE_PACKET("game#membersinfo");
			GNET_UNRECEIVE_PACKET("game#playerupdate");
			GNET_UNRECEIVE_PACKET("stage#join_failed");
			GNET_UNRECEIVE_PACKET("stage#join_ok");

			NETWORK_SYSTEM_PTR->disconnect();

			SAFE_DELETE(mInfo);
		}
		 void MultiPlay::gameOver()
		{
			sendInfo();
		}
		 void MultiPlay::setMyInfo(Config &config)
		{
			mInfo->mConfig = config;
		}
		 void MultiPlay::setMyInfo(const std::string &configFile)
		{
			mInfo->setConfig(configFile);
		}
		 void MultiPlay::gameLoad(const std::string &configFile) {
			if(configFile!="")
				mInfo->setConfig(configFile);

			//连接server
			NETWORK_SYSTEM_PTR->connect();

			NETWORK_SYSTEM_PTR->send("auth#setuid","%s",mInfo->mConfig.getAsString("uid").c_str());

			NETWORK_SYSTEM_PTR->send("stage#join","%s","919");

			if(getBaseState()==CONNECT)
			{
				NETWORK_SYSTEM_PTR->waitForNext();

				NETWORK_SYSTEM_PTR->refresh();
			}

			if(getBaseState()!=GET_INFO)
			{
				NETWORK_SYSTEM_PTR->disconnect();
				return;
			}

			while(getBaseState()!=READY)
			{
				NETWORK_SYSTEM_PTR->waitForNext();
				
				NETWORK_SYSTEM_PTR->refresh();
			}

			//准备完成
			NETWORK_SYSTEM_PTR->send("game#systemready");
		}

		 void MultiPlay::gameStart() {
			//游戏开始
			setBaseState(PLAY);
		}

		 void MultiPlay::update(float dt) {
			//receivePacket();
			//updateState(dt);
			if(getBaseState()==PLAY)
			{
				//心跳包，发送当前状态
				DEAL_PER_PERIOD_BEGIN(dt,1.0);
				sendInfo();
				DEAL_PER_PERIOD_END();

				mInfo->update(dt);
			}
		}

		void MultiPlay::sendInfo()
		{
			////与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
			NETWORK_SYSTEM_PTR->send("game#heartbeat",
				"%f%d%d%f",CORE_PTR->getRunPosition(),getScore(),getCombo(),getHPinRatio());
		}

		//获得游戏信息，队伍信息
		void MultiPlay::gnetMembersInfo(GPacket *packet)
		{
			mInfo->newGame(packet);

			for(int i = 0; i < mInfo->mPlayers.size(); i++)
				mInfo->mPlayers[i].hp = float(ORIGIN_HP)/MAX_HP;

			setBaseState(READY);
		}

		//心跳包更新游戏信息
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
				//显示一些调试信息
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
			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();
		}
}
