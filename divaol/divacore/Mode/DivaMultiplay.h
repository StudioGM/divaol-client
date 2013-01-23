/*
 *  DivaMultiplay.h
 *
 *  Created by Hyf042 on 8/9/12.
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
	class MultiPlay;

	struct PlayerInfo
	{
	public:
		//identity
		std::string name,uid;
		//slot
		int index;
		int teamIndex;
		int indexInTeam;
		//game info
		int score;
		int combo;
		float hp;
		int maxCombo;
		int maxCTLevel;
		std::string status;

		PlayerInfo()
			:name(""),score(0),combo(0),hp(1.0),index(0),teamIndex(0),indexInTeam(0),maxCombo(0),maxCTLevel(0),status("systemready")
		{}
		friend bool operator<(const PlayerInfo &a, const PlayerInfo &b)
		{
			return a.score>b.score;
		}
	};
	typedef std::vector<PlayerInfo> PLAYERS;
	class TeamInfo
	{
	public:
		std::vector<int> players;
		int combo,score,teamID,nowPlayer;
		TeamInfo():combo(0),score(0),nowPlayer(-1) {}
	};
	typedef std::vector<TeamInfo> TEAMS;

	class NetGameInfo
	{
	public:
		typedef std::map<std::string,int32> UID_MAP;

		Config mConfig;
		int myTeamID, myPlayerID;
		TEAMS mTeams; //���ж���Ļ�����Ϣ
		PLAYERS mPlayers; // ������ҵ���Ϣ
		TeamInfo *myTeamPtr; //���������Ķ�����Ϣ
		PlayerInfo *myPlayerPtr; //���������������Ϣ
		UID_MAP mUidMap;

		MultiPlay *mOwner;
			
		void setOwner(MultiPlay *owner) {mOwner=owner;}
		virtual void newGame(GPacket *packet);
		virtual void updateInfoFromPacket(GPacket *packet);
		virtual void update(float dt);
		virtual void updateTeamInfo();
		void setConfig(const std::string &configFile);
	};

	class MultiGameInfo : public NetGameInfo
	{
	public:
		void newGame(GPacket *packet);
	};

	class MultiPlay : public SinglePlay
	{
	protected:
		static const int CONNECT_WAIT_TIME = 5000;
		static const int FAILURE_WAIT_TIME = 1000;

		int mBaseState;

		NetGameInfo *mInfo;

		sora::SoraText mText;
		sora::SoraMutex mutex;
	public:
		/*CONNECTΪ�ȴ�����״̬
		 GET_INFOΪ���ӳɹ��ȴ���������Ϣ״̬
		 READYΪ׼����ɵȴ��������״̬
		 PLAY��ʾ��Ϸ����
		 FAILURE��ʾ����ʧ�ܵȴ���
		 FULL��ʾ����������*/
		enum{CONNECTING,GET_INFO,READY,PLAY,FAILED,FAILURE,FULL,OVER};

		MultiPlay():mInfo(0) {}

		void setBaseState(int state) {sora::SoraMutexGuard lock(mutex);mBaseState=state;}
		int getBaseState() {sora::SoraMutexGuard lock(mutex);return mBaseState;}
		
		virtual void registerNetworkEvent();
		NetGameInfo* getGlobalInfo() {return mInfo;}
		virtual int getTeamID() {return mInfo->myTeamID;}
		virtual int getPlayerID() {return mInfo->myPlayerID;}
		virtual TEAMS& getTeamInfo() {return mInfo->mTeams;}
		virtual PLAYERS& getPlayerInfo() {return mInfo->mPlayers;}
		virtual TeamInfo* getMyTeamInfo() {return mInfo->myTeamPtr;}
		virtual PlayerInfo* getMyPlayerInfo() {return mInfo->myPlayerPtr;}
		PlayerInfo* getSpecificPlayerInfo(const std::string uid);

		virtual std::string getName() {return "multiPlay";}
		virtual std::string getNetGameMode() {return "normal";}

		virtual void inform(StateEvent& event);
		
		virtual void init() ;

		virtual void gameReset();

		virtual void gameStop();

		virtual void gameOver();

		virtual void setMyInfo(Config &config);

		virtual void setMyInfo(const std::string &configFile);

		virtual void gameLoad(const std::string &configFile);

		//virtual void gameLoadUnsync(const std::string &configFile);

		virtual void gameStart();

		virtual void update(float dt);

		void sendInfo();

		//�����Ϸ��Ϣ��������Ϣ
		void gnetMembersInfo(GPacket *packet);
		//������������Ϸ��Ϣ
		void gnetPlayerUpdate(GPacket *packet);
		void gnetGameStart(GPacket *packet);
		void gnetJoinFailed(GPacket *packet);
		void gnetJoinOK(GPacket *packet);
		void gnetStartFailed(GPacket *packet);
		void render();

		void preStart();

		void preEvaluate();
	};
}

#endif