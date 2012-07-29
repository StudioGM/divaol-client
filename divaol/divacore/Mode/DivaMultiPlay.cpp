/*
 *  DivaMultiPlay.cpp
 *
 *  Created by Hyf042 on 2/10/12.
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
			RECEIVE_PACKET(network::DIVA_NET_STC_PLAYER_INFO,&MultiPlay::getInfo);
			RECEIVE_PACKET(network::DIVA_NET_STC_UPDATE_INFO,&MultiPlay::updateInfo);
			RECEIVE_PACKET(network::DIVA_NET_STC_FULL,&MultiPlay::gameFull);


			SinglePlay::gameReset();

			setBaseState(CONNECT);
		}
		 void MultiPlay::gameStop() {
			UNRECEIVE_PACKET(network::DIVA_NET_STC_PLAYER_INFO);
			UNRECEIVE_PACKET(network::DIVA_NET_STC_UPDATE_INFO);
			UNRECEIVE_PACKET(network::DIVA_NET_STC_FULL);
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

			//等待连接反馈
			while(! NETWORK_SYSTEM_PTR->waitConnection(CONNECT_WAIT_TIME/1000.0))
			{
				//失败，等待一段时间后重连
				setBaseState(FAILURE);
				msleep(FAILURE_WAIT_TIME);
				setBaseState(CONNECT);
				NETWORK_SYSTEM_PTR->connect();
			}

			//发送登录信息，等待反馈
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_LOGIN,0,"%d%d%s",myInfo.getAsInt("slot"),myInfo.getAsInt("id"),myInfo.getAsString("name").c_str());
			setBaseState(GET_INFO);

			//接受信息
			while(getBaseState()!=READY)
			{
				msleep(1);
				if(getBaseState()==FULL||NETWORK_SYSTEM_PTR->getState()==NetworkSystem::READY)
				{
					setBaseState(FULL);
					NETWORK_SYSTEM_PTR->disconnect();
					return;
				}
			}

			//准备完成
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_READY);
		}

		 void MultiPlay::gameLoadUnsync(const std::string &configFile) {
			if(configFile!="")
			{
				myInfo.clear();
				configloader::loadWithJson(myInfo,configFile);
			}

			//连接server
			NETWORK_SYSTEM_PTR->connect();

			//等待连接反馈
			while(! NETWORK_SYSTEM_PTR->waitConnection(CONNECT_WAIT_TIME/1000.0))
			{
				//失败，等待一段时间后重连
				setBaseState(FAILURE);
				msleep(FAILURE_WAIT_TIME);
				setBaseState(CONNECT);
				NETWORK_SYSTEM_PTR->connect();
			}

			//发送登录信息，等待反馈
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_LOGIN,0,"%d%d%s",myInfo.getAsInt("slot"),myInfo.getAsInt("id"),myInfo.getAsString("name").c_str());
			setBaseState(GET_INFO);

			//unSyncUpdate(READY);
			TimeCounter timeCounter;
			double nowTime = timeCounter.getTime(),tmpTime;
			while(getBaseState()!=READY)
			{ 
				msleep(100);
				tmpTime = timeCounter.getTime();

				//hold on room connection
				//CORE_PTR->holdOnConnection(tmpTime-nowTime);

				NETWORK_SYSTEM_PTR->update(tmpTime-nowTime);
				nowTime = tmpTime;

				if(getBaseState()==FULL||NETWORK_SYSTEM_PTR->getState()==NetworkSystem::READY)
				{
					setBaseState(FULL);
					NETWORK_SYSTEM_PTR->disconnect();
					return;
				}
			}

			//准备完成
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_READY);
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

				teams[teamID].players[playerID].score = getScore();
				teams[teamID].players[playerID].hp = getHPinRatio();
				teams[teamID].players[playerID].combo = getCombo();
				teams[teamID].combo = getCombo();
				teams[teamID].score = 0;
				for(int i = 0; i < teams[teamID].players.size(); i++)
					teams[teamID].score += teams[teamID].players[i].score;
			}
		}

		void MultiPlay::sendInfo()
		{
			//与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_UPDATE_INFO,CORE_PTR->getRunTime(),
				"%f%d%f%d",CORE_PTR->getRunPosition(),getScore(),(double)getHPinRatio(),getCombo());
		}

		//获得游戏信息，队伍信息
		void MultiPlay::getInfo(network::Packet &packet)
		{
			teamID = packet.readByte();
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
			setBaseState(READY);
		}

		//心跳包更新游戏信息
		void MultiPlay::updateInfo(Packet &packet)
		{
			float hp;
			int score,combo;
			//更新所有人的得分情况，除去自己
			for(int j = 0; j < teams.size(); j++)
			{
				for(int i = 0; i < teams[j].players.size(); i++)
				{
					score = packet.readInt32();
					combo = packet.readInt32();
					hp = packet.readDouble();
					if(j!=teamID||i!=playerID)
						teams[j].players[i].score = score, teams[j].players[i].combo = combo, teams[j].players[i].hp = hp;
				}
			}
			//加入自己的信息，以本机为准
			teamPtr->players[playerID].score = getScore();
			//更新队伍信息，除了本队以外
			for(int i = 0; i < teams.size(); i++)
			{
				score = packet.readInt32();
				combo = packet.readInt32();
				teams[i].nowPlayer = packet.readInt32();
				if(i!=teamID)
				{
					teams[i].score = score;
					teams[i].combo = combo;
				}
			}
			//更新本队总分
			teams[teamID].score = 0;
			for(int i = 0; i < teams[teamID].players.size(); i++)
				teams[teamID].score += teams[teamID].players[i].score;
			teams[teamID].players[playerID].hp = getHPinRatio();
			teams[teamID].combo = getCombo();
		}

		void MultiPlay::render()
		{
			if(getBaseState()==PLAY)
			{
#ifdef _DEBUG
				//显示一些调试信息
				std::string output = format("%d Teams:\nYour team is %d\n",teams.size(),teamID+1);
				for(int i = 0; i < teams.size(); i++)
					output += format("team%d score: %d\n",i+1,teams[i].score);
				output += format("\nThe number of your team member is %d\nYour position is %d and the scores:\n",teams[teamID].players.size(),playerID);
				for(int i = 0; i < teams[teamID].players.size(); i++)
					output += format("Member.%d\t%s : %d\n",i+1,teams[teamID].players[i].name.c_str(),teams[teamID].players[i].score);
	
				mText.setText(sora::s2ws(output));
				mText.renderTo(200,200);
#endif
			}
		}

		void MultiPlay::gameFull(Packet &packet)
		{
			setBaseState(FULL);
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
