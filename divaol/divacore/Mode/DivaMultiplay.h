/*
 *  DivaMultiplay.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_MULTIPLAY
#define DIVA_MULTIPLAY

#include "Core/DivaGameMode.h"
#include "Core/DivaCore.h"
#include "DivaSinglePlay.h"
#include "thread/SoraMutex.h"
#include "SoraText.h"
#include "Utility/DivaConfigLoader.h"
#include "Core/DivaTimeCounter.h"

namespace divacore
{
	class PlayerInfo
	{
	public:
		std::string name;
		int score,combo,teamID,playerID,id,netID;
		float hp;
		PlayerInfo():name(""),score(0),combo(0),hp(0.5),netID(0) {}
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
		TEAMS teams; //���ж���Ļ�����Ϣ
		TeamInfo *teamPtr; //���������Ķ�����Ϣ
		PlayerInfo *playerPtr; //���������������Ϣ

		sora::SoraMutex mutex;

		sora::SoraText mText;

		Config myInfo;
	public:
		/*CONNECTΪ�ȴ�����״̬
		 GET_INFOΪ���ӳɹ��ȴ���������Ϣ״̬
		 READYΪ׼����ɵȴ��������״̬
		 PLAY��ʾ��Ϸ����
		 FAILURE��ʾ����ʧ�ܵȴ���
		 FULL��ʾ����������*/
		enum{CONNECT,GET_INFO,READY,PLAY,FAILURE,FULL,OVER};
		void setBaseState(int state) {sora::SoraMutexGuard lock(mutex);baseState=state;}
		int getBaseState() {sora::SoraMutexGuard lock(mutex);return baseState;}

		virtual int getTeamID() {return teamID;}
		virtual int getPlayerID() {return playerID;}
		virtual TEAMS& getGlobalInfo() {return teams;}
		virtual TeamInfo* getTeamInfo() {return teamPtr;}
		virtual PlayerInfo* getPlayerInfo() {return playerPtr;}

		virtual std::string getName() {return "multiPlay";}

		virtual void init() ;

		virtual void gameReset();

		virtual void gameStop();

		virtual void gameOver();

		virtual void setMyInfo(Config &config);

		virtual void setMyInfo(const std::string &configFile);

		virtual void gameLoad(const std::string &configFile);

		virtual void gameLoadUnsync(const std::string &configFile);

		virtual void gameStart();

		virtual void update(float dt);

		void sendInfo();

		//�����Ϸ��Ϣ��������Ϣ
		void getInfo(network::Packet &packet);

		//������������Ϸ��Ϣ
		void updateInfo(Packet &packet);

		void render();

		void gameFull(Packet &packet);

		void preStart();

		void preEvaluate();
	};
}

#endif