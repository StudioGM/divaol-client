/*
 *  DivaMultiplay.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MULTIPLAY
#define DIVA_MULTIPLAY

#include "DivaStateManager.h"
#include "DivaCore.h"
#include "DivaSinglePlay.h"
#include "thread/SoraMutex.h"
#include "SoraText.h"

namespace divacore
{
	class PlayerInfo
	{
	public:
		std::string name;
		int score,combo,teamID,playerID,id;
		float hp;
		PlayerInfo():name(""),score(0),combo(0),hp(0.5) {}
		friend bool operator<(const PlayerInfo &a, const PlayerInfo &b)
		{
			return a.score>b.score;
		}
	};
	typedef std::vector<PlayerInfo> PLAYERS;
	class TeamInfo
	{
	public:
		PLAYERS players;
		std::string name;
		int combo,score,teamID,nowPlayer;
		TeamInfo():name(""),combo(0),score(0),nowPlayer(-1) {}
	};
	typedef std::vector<TeamInfo> TEAMS;

	class MultiPlay : public SinglePlay
	{
	protected:
		static const int CONNECT_WAIT_TIME = 5000;
		static const int FAILURE_WAIT_TIME = 1000;

		int baseState;
		int teamID, playerID;
		TEAMS teams; //所有队伍的基本信息
		TeamInfo *teamPtr; //本机所属的队伍信息
		PlayerInfo *playerPtr; //本机所属的玩家信息

		sora::SoraMutex mutex;

		sora::SoraText mText;
	public:
		/*CONNECT为等待连接状态
		 GET_INFO为连接成功等待服务器信息状态
		 READY为准备完成等待其他玩家状态
		 PLAY表示游戏进行
		 FAILURE表示连接失败等待中
		 FULL表示服务器已满*/
		enum{CONNECT,GET_INFO,READY,PLAY,FAILURE,FULL,OVER};
		void setBaseState(int state) {sora::SoraMutexGuard lock(mutex);baseState=state;}
		int getBaseState() {sora::SoraMutexGuard lock(mutex);return baseState;}

		virtual int getTeamID() {return teamID;}
		virtual int getPlayerID() {return playerID;}
		virtual TEAMS& getGlobalInfo() {return teams;}
		virtual TeamInfo* getTeamInfo() {return teamPtr;}
		virtual PlayerInfo* getPlayerInfo() {return playerPtr;}

		virtual void init() 
		{
			//注册接收函数
			RECEIVE_PACKET(network::DIVA_NET_STC_PLAYER_INFO,&MultiPlay::getInfo);
			RECEIVE_PACKET(network::DIVA_NET_STC_UPDATE_INFO,&MultiPlay::updateInfo);
			RECEIVE_PACKET(network::DIVA_NET_STC_FULL,&MultiPlay::gameFull);

			//debug info
			mText.setColor(CARGB(255,255,0,0));
			mText.setFont(sora::SoraFont::LoadFromFile("simhei.ttf", 50));
		}
		virtual void gameReset() {
			SinglePlay::gameReset();

			setBaseState(CONNECT);
		}
		virtual void gameStop() {NETWORK_SYSTEM_PTR->disconnect();}
		virtual void gameOver()
		{
			sendInfo();
		}
		virtual void gameLoad(const std::string &configFile) {
			//连接server
			NETWORK_SYSTEM_PTR->connect();

			//等待连接反馈
			while(! NETWORK_SYSTEM_PTR->waitConnection(CONNECT_WAIT_TIME/1000.0))
			{
				//失败，等待一段时间后重连
				setBaseState(FAILURE);
				Sleep(FAILURE_WAIT_TIME);
				setBaseState(CONNECT);
				NETWORK_SYSTEM_PTR->connect();
			}

			//发送登录信息，等待反馈
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_LOGIN);
			setBaseState(GET_INFO);

			//接受信息
			while(getBaseState()!=READY)
			{
				Sleep(1);
				if(getBaseState()==FULL)
				{
					NETWORK_SYSTEM_PTR->disconnect();
					return;
				}
			}

			//准备完成
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_READY);
		}

		virtual void gameStart() {
			//游戏开始
			setBaseState(PLAY);
		}

		virtual void update(float dt) {
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

		void sendInfo()
		{
			//与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_UPDATE_INFO,CORE_PTR->getRunTime(),
				"%f%d%f%d",CORE_PTR->getRunPosition(),getScore(),(double)getHPinRatio(),getCombo());
		}

		//获得游戏信息，队伍信息
		void getInfo(network::Packet &packet)
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
		void updateInfo(Packet &packet)
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

		void render()
		{
			if(getBaseState()==PLAY)
			{
				//显示一些调试信息
				std::string output = format("%d Teams:\nYour team is %d\n",teams.size(),teamID+1);
				for(int i = 0; i < teams.size(); i++)
					output += format("team%d score: %d\n",i+1,teams[i].score);
				output += format("\nThe number of your team member is %d\nYour position is %d and the scores:\n",teams[teamID].players.size(),playerID);
				for(int i = 0; i < teams[teamID].players.size(); i++)
					output += format("Member.%d\t%s : %d\n",i+1,teams[teamID].players[i].name.c_str(),teams[teamID].players[i].score);
	
				mText.setText(sora::s2ws(output));
				mText.renderTo(200,200);
			}
		}

		void gameFull(Packet &packet)
		{
			setBaseState(FULL);
		}

	};
}

#endif