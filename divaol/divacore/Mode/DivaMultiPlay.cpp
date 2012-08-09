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
		 void MultiPlay::init() 
		{
			//debug info
			mText.setColor(CARGB(255,255,0,0));
			mText.setFont(sora::SoraFont::LoadFromFile("simhei.ttf", 50));
		}
		 void MultiPlay::gameReset() {
			//注册接收函数
			GNET_RECEIVE_PACKET("game#membersinfo",&MultiPlay::getInfo);
			GNET_RECEIVE_PACKET("game#playerupdate",&MultiPlay::updateInfo);
			GNET_RECEIVE_PACKET("stage#join_failed",&MultiPlay::joinFailed);
			GNET_RECEIVE_PACKET("stage#join_ok",&MultiPlay::join);

			SinglePlay::gameReset();

			setBaseState(CONNECT);
		}
		 void MultiPlay::gameStop() {
			GNET_UNRECEIVE_PACKET("game#membersinfo");
			GNET_UNRECEIVE_PACKET("game#playerupdate");
			GNET_UNRECEIVE_PACKET("stage#join_failed");
			GNET_UNRECEIVE_PACKET("stage#join_ok");

			NETWORK_SYSTEM_PTR->disconnect();
		}
		 void MultiPlay::gameOver()
		{
			sendInfo();
		}
		 void MultiPlay::setMyInfo(Config &config)
		{
			myInfo = config;
		}
		 void MultiPlay::setMyInfo(const std::string &configFile)
		{
			myInfo.clear();
			configloader::loadWithJson(myInfo,configFile);
		}
		 void MultiPlay::gameLoad(const std::string &configFile) {
			if(configFile!="")
			{
				myInfo.clear();
				configloader::loadWithJson(myInfo,configFile);
			}

				//连接server
				NETWORK_SYSTEM_PTR->connect();

				NETWORK_SYSTEM_PTR->send("auth#setuid","%s","1");

				NETWORK_SYSTEM_PTR->send("stage#join","%s","919");

				NETWORK_SYSTEM_PTR->waitForNext();

				NETWORK_SYSTEM_PTR->refresh();

				NETWORK_SYSTEM_PTR->waitForNext();

				NETWORK_SYSTEM_PTR->refresh();


				if(getBaseState()!=READY)
				{
					NETWORK_SYSTEM_PTR->disconnect();
					return;
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

				/*teams[teamID].players[playerID].score = getScore();
				teams[teamID].players[playerID].hp = getHPinRatio();
				teams[teamID].players[playerID].combo = getCombo();
				teams[teamID].combo = getCombo();
				teams[teamID].score = 0;
				for(int i = 0; i < teams[teamID].players.size(); i++)
					teams[teamID].score += teams[teamID].players[i].score;*/
			}
		}

		void MultiPlay::sendInfo()
		{
			////与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
			NETWORK_SYSTEM_PTR->send("game#heartbeat",
				"%d%d",getScore(),getCombo());
		}

		//获得游戏信息，队伍信息
		void MultiPlay::getInfo(GPacket *packet)
		{
			/*teamID = packet.readByte();
			playerID = packet.readByte();

			teams.clear();
			int teamNum = packet.readByte(),cnt = 0;
			for(int i = 0; i < teamNum; i++)
			{
			TeamInfo teamInfo;
			teamInfo.teamID = i;
			teamInfo.name = packet.readString();
			int playerNum = packet.readByte();
			for(int j = 0; j < playerNum; j++)
			{
			teamInfo.players.push_back(PlayerInfo());
			teamInfo.players[j].teamID = i;
			teamInfo.players[j].playerID = j;
			teamInfo.players[j].id = cnt++;
			teamInfo.players[j].name = packet.readString();
			teamInfo.players[j].netID = packet.readInt32();
			teamInfo.players[j].score = teamInfo.players[j].combo = 0;
			teamInfo.players[j].hp = float(ORIGIN_HP)/MAX_HP;
			}
			teams.push_back(teamInfo);
			}
			teamPtr = &teams[teamID];
			playerPtr = &teams[teamID].players[playerID];
			setBaseState(READY);*/
			gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(dynamic_cast<GPacket*>(packet->getItem(3))->getItem(1));
			mPlayers = PLAYERS(list->size());

			for(int i = 0; i < mPlayers.size(); i++)
				mPlayers[i].uid = dynamic_cast<gnet::Item<gnet::Binary>*>(list->getItem(i))->getData();

			setBaseState(READY);
		}

		//心跳包更新游戏信息
		void MultiPlay::updateInfo(GPacket *packet)
		{
			//float hp;
			//int score,combo;
			////更新所有人的得分情况，除去自己
			//for(int j = 0; j < teams.size(); j++)
			//{
			//	for(int i = 0; i < teams[j].players.size(); i++)
			//	{
			//		score = packet.readInt32();
			//		combo = packet.readInt32();
			//		hp = packet.readDouble();
			//		if(j!=teamID||i!=playerID)
			//			teams[j].players[i].score = score, teams[j].players[i].combo = combo, teams[j].players[i].hp = hp;
			//	}
			//}
			////加入自己的信息，以本机为准
			//teamPtr->players[playerID].score = getScore();
			////更新队伍信息，除了本队以外
			//for(int i = 0; i < teams.size(); i++)
			//{
			//	score = packet.readInt32();
			//	combo = packet.readInt32();
			//	teams[i].nowPlayer = packet.readInt32();
			//	if(i!=teamID)
			//	{
			//		teams[i].score = score;
			//		teams[i].combo = combo;
			//	}
			//}
			////更新本队总分
			//teams[teamID].score = 0;
			//for(int i = 0; i < teams[teamID].players.size(); i++)
			//	teams[teamID].score += teams[teamID].players[i].score;
			//teams[teamID].players[playerID].hp = getHPinRatio();
			//teams[teamID].combo = getCombo();
			gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(packet->getItem(2));

			for(int i = 0; i < mPlayers.size(); i++)
			{
				GPacket *player = dynamic_cast<GPacket*>(list->getItem(i));
				mPlayers[i].score = gnet::ItemUtility::getUInt(player->getItem(1));
				mPlayers[i].combo = gnet::ItemUtility::getUInt(player->getItem(2));
			}
		}

		void MultiPlay::render()
		{
			if(getBaseState()==PLAY)
			{
#ifdef _DEBUG
				//显示一些调试信息
				std::string output = format("%d Players",mPlayers.size());
				//for(int i = 0; i < mTeams.size(); i++)
				//	output += format("team%d score: %d\n",i+1,mTeams[i].score);
				//output += format("\nThe number of your team member is %d\nYour position is %d and the scores:\n",mTeams[myTeamID].players.size(),myPlayerID);
				//for(int i = 0; i < mTeams[myTeamID].players.size(); i++)
				//	output += format("Member.%d : %d\n",i+1,mTeams[myTeamID].players[i]);
	
				for(int i = 0; i < mPlayers.size(); i++)
					output += format("player%d score:%d\tcombo:%d\n",i+1,mPlayers[i].score,mPlayers[i].combo);

				mText.setText(sora::s2ws(output));
				mText.renderTo(200,200);
#endif
			}
		}

		void MultiPlay::joinFailed(GPacket *packet)
		{
			setBaseState(FAILURE);
		}

		void MultiPlay::join(GPacket *packet)
		{
			setBaseState(GET_INFO);
		}

		void MultiPlay::preStart()
		{
			//if(CORE_PTR->getState()!=Core::RESULT)
			//	UI_PAINTER_PTR->addWidget("multiPlayer");
		}

		void MultiPlay::preEvaluate()
		{
			((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();
		}
}
