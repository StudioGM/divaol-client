/*
*  DivaMultiPlay.cpp
*
*  Created by Hyf042 on 8/9/12.
*  Copyright 2012 Hyf042. All rights reserved.
*
*/
#include "divapomelo/diva/Client.h"

#include "DivaMultiPlay.h"
#include "Component/DivaCommonEvaluateStrategy.h"
#include "divanetwork/DivaNetworkManager.h"
#include "Hook/DivaCTMode.h"
#include "Utility/DivaSettings.h"
#include "Utility/DivaRankResult.h"

namespace divacore
{
#if defined(DIVA_GNET_OPEN) && !defined(DIVA_USE_POMELO)
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
		gnet::Item<gnet::Tuple> *tuple = dynamic_cast<gnet::Item<gnet::Tuple>*>(packet->getItem(2));
		gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(tuple->getItem(0));

		for(int i = 0; i < mPlayers.size(); i++)
			if(i!=myPlayerID)
			{
				GPacket *player = dynamic_cast<GPacket*>(list->getItem(i));
				mPlayers[i].score = gnet::ItemUtility::getUInt(player->getItem(1));
				mPlayers[i].combo = gnet::ItemUtility::getUInt(player->getItem(2));
				mPlayers[i].hp = gnet::ItemUtility::getValue(player->getItem(3));
				mPlayers[i].status = gnet::ItemUtility::getString(player->getItem(4));
			}
		list = dynamic_cast<gnet::Item<gnet::List>*>(tuple->getItem(1));

		for(int i = 0; i < mTeams.size(); i++)
		{
			mTeams[i].nowPlayer = list->getItem(i)->getInt()-1;
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
#ifdef _DEBUG
		mText.setColor(CARGB(255,255,0,0));
		mText.setFont(sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 50));
#endif
	}

	void MultiPlay::registerNetworkEvent() {
		//注册接收函数
		GNET_RECEIVE_PACKET("game#membersinfoL",&MultiPlay::gnetMembersInfo);
		GNET_RECEIVE_PACKET("game#playerupdateL",&MultiPlay::gnetPlayerUpdate);
		GNET_RECEIVE_PACKET("game#start_failed",&MultiPlay::gnetStartFailed);
		//GNET_RECEIVE_PACKET("stage#join_failed",&MultiPlay::gnetJoinFailed);
		//GNET_RECEIVE_PACKET("stage#join_ok",&MultiPlay::gnetJoinOK);

		if(mInfo==0) {
			mInfo = new NetGameInfo();
			mInfo->setOwner(this);
		}

		setBaseState(CONNECTING);
	}

	void MultiPlay::gameReset() {

		SinglePlay::gameReset();
	}
	void MultiPlay::gameStop() {
		//NETWORK_SYSTEM_PTR->disconnect();

		SAFE_DELETE(mInfo);

		STAGE_CLIENT.back();
	}
	void MultiPlay::noteOver() {
		if (isOver)
			return;

		SinglePlay::noteOver();

		sendInfo();

		//GNET_UNRECEIVE_PACKET("stage#join_failed");
		//GNET_UNRECEIVE_PACKET("stage#join_ok");
		
		GPacket *packet = new GPacket();
		*packet += (gnet::Atom)"game";
		*packet += (gnet::Atom)"overR";
		*packet += (int8)CORE_FLOW_PTR->isNoteOver();
		GPacket *addData = new GPacket();
		*addData += maxCombo;
		*addData += maxCTLevel;

		*addData += (int)DivaRankResult::GetRankResult(CORE_FLOW_PTR->isNoteOver(), getHP(), maxCTLevel, maxCombo, EVALUATE_STRATEGY_PTR->getResult().myCntEval[0], 
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[1],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[2],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[3],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[4]);
		
		packet->appendItem(addData);

		NETWORK_SYSTEM_PTR->send(packet);
	}
	void MultiPlay::gameOver()
	{
		SinglePlay::gameOver();

		sendInfo();

		GNET_UNRECEIVE_PACKET("game#membersinfoL");
		GNET_UNRECEIVE_PACKET("game#playerupdateL");
		GNET_UNRECEIVE_PACKET("game#start_failed");
		//NETWORK_SYSTEM_PTR->send("game#overR","%b%d",CORE_FLOW_PTR->isSongOver(),maxCombo);
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
				if (getAlive())
					NETWORK_SYSTEM_PTR->send("game#evalR",
						"%d%d",event.note->getID(), event.rank);
			}
		}
	}
	void MultiPlay::gameLoad(const std::string &configFile) {
		if(configFile!="")
			mInfo->setConfig(configFile);

		while(getBaseState() != READY && getBaseState() != FAILED)
		{
			NETWORK_SYSTEM_PTR->waitForNext();
			NETWORK_SYSTEM_PTR->refresh();
		}
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

	void MultiPlay::onHeartbeat(Json::Value &msg) {
		mInfo->updateInfoFromPacket(msg);
		afterUpdateInfo();
	}

	void MultiPlay::sendInfo()
	{
		////与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
		NETWORK_SYSTEM_PTR->send("game#heartbeatR",
			"%f%d%d%f",CORE_PTR->getRunPosition(),getScore(),getCombo(),getHPinRatio());
	}

	/* Pomelo Event&Request */

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
		afterUpdateInfo();
	}

	// abandon
	void MultiPlay::gnetJoinFailed(GPacket *packet)
	{
		setBaseState(FAILURE);
	}

	void MultiPlay::gnetJoinOK(GPacket *packet)
	{
		setBaseState(GET_INFO);
	}

	void MultiPlay::gnetStartFailed(GPacket *packet)
	{
		setBaseState(FAILED);
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
		EVALUATE_STRATEGY_PTR->getResult().myScore = getScore();
		EVALUATE_STRATEGY_PTR->getResult().myID = getPlayerID();
		EVALUATE_STRATEGY_PTR->getResult().myMaxCombo = getMaxCombo();
		EVALUATE_STRATEGY_PTR->getResult().myMaxCTLevel = getMaxCTLevel();
		EVALUATE_STRATEGY_PTR->getResult().myHp = getHPinRatio();
		EVALUATE_STRATEGY_PTR->getResult().myIsOver = (bool)CORE_FLOW_PTR->isNoteOver();

		PLAYERS &players = getPlayerInfo();

		for(int i = 0; i < players.size(); i++)
			EVALUATE_STRATEGY_PTR->getResult().evalData.push_back(EvalData(players[i].uid, players[i].score, 0, 0, 0, false, players[i].teamIndex, STAGE_CLIENT.waiterInfo(players[i].uid).nickname));
		//((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();
	}

	PlayerInfo* MultiPlay::getSpecificPlayerInfo(const std::string uid)
	{
		if(mInfo==NULL)
			return NULL;
		for(int i = 0; i < mInfo->mPlayers.size(); i++)
			if(mInfo->mPlayers[i].uid == uid)
				return &mInfo->mPlayers[i];
		return NULL;
	}
#else
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
	void NetGameInfo::updateInfoFromPacket(Json::Value &msg) {
		Json::Value &list = msg["player"];
		Json::Value &team = msg["team"];

		for(int i = 0; i < mPlayers.size(); i++)
			if(i!=myPlayerID)
			{
				Json::Value &player = list[i];
				mPlayers[i].score = player["score"].asInt();
				mPlayers[i].combo = player["combo"].asInt();
				mPlayers[i].hp = player["hp"].asDouble();
				mPlayers[i].status = "";
			}

		for(int i = 0; i < mTeams.size(); i++)
		{
			mTeams[i].nowPlayer = team[i]["nowPlayer"].asInt();
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

	void NetGameInfo::newGame(Json::Value &msg)
	{
		mPlayers = PLAYERS(msg["players"].size());
		mTeams = TEAMS(msg["teamCnt"].asInt());

		for(int i = 0; i < mPlayers.size(); i++)
		{
			PlayerInfo playerInfo;

			Json::Value &player = msg["players"][i];

			playerInfo.uid = player["info"]["uid"].asString();
			playerInfo.index = player["index"].asInt();
			playerInfo.teamIndex = player["teamId"].asInt();
			playerInfo.indexInTeam = player["idInTeam"].asInt();

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
#ifdef _DEBUG
		mText.setColor(CARGB(255,255,0,0));
		mText.setFont(sora::SoraFont::LoadFromFile(SETTINGS.getGlobalFontName().asUnicode(), 50));
#endif
	}

	void MultiPlay::registerNetworkEvent() {
		//注册接收函数
		POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_HEARTBEAT], [&](divapomelo::MessageReq &req) {
			onHeartbeat(req.msg());
		});
		if(mInfo==0) {
			mInfo = new NetGameInfo();
			mInfo->setOwner(this);
		}

		setBaseState(CONNECTING);
	}

	void MultiPlay::gameReset() {

		SinglePlay::gameReset();
	}
	void MultiPlay::gameStop() {
		//NETWORK_SYSTEM_PTR->disconnect();

		SAFE_DELETE(mInfo);

		POMELO_STAGE_PEER->back();
	}
	void MultiPlay::noteOver() {
		if (isOver)
			return;

		SinglePlay::noteOver();

		sendInfo();

		int rank = (int)DivaRankResult::GetRankResult(CORE_FLOW_PTR->isNoteOver(), getHP(), maxCTLevel, maxCombo, EVALUATE_STRATEGY_PTR->getResult().myCntEval[0], 
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[1],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[2],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[3],
																					   EVALUATE_STRATEGY_PTR->getResult().myCntEval[4]);

		POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_OVER], Json::Object(
			"complete", CORE_FLOW_PTR->isNoteOver(),
			"extra", Json::Object (
				"maxCombo", maxCombo,
				"maxCTLevel", maxCTLevel,
				"rank", rank
			)
		));
	}
	void MultiPlay::gameOver()
	{
		SinglePlay::gameOver();

		sendInfo();

		POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_HEARTBEAT]);
		//NETWORK_SYSTEM_PTR->send("game#overR","%b%d",CORE_FLOW_PTR->isSongOver(),maxCombo);
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
				if (getAlive()) {
					POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_EVAL],
						Json::Object(
							"rank", event.rank
						));
				}
			}
		}
	}
	void MultiPlay::gameLoad(const std::string &configFile) {
		if(configFile!="")
			mInfo->setConfig(configFile);

		//准备完成
		POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_LINK], Json::Object(), 
			[&](divapomelo::RequestReq &req, int status, Json::Value resp) {
			if (status == 0) {
				mInfo->newGame(resp);
				for(int i = 0; i < mInfo->mPlayers.size(); i++)
					mInfo->mPlayers[i].hp = float(ORIGIN_HP)/MAX_HP;

				setBaseState(READY);
			}
			else {
				setBaseState(FAILED);
			}
		});
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

	void MultiPlay::onHeartbeat(Json::Value &msg) {
		mInfo->updateInfoFromPacket(msg);
		afterUpdateInfo();
	}

	void MultiPlay::sendInfo()
	{
		////与server同步score，hp和combo信息，此信息仅为他人显示用，不参与数据处理
		POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_UPDATE], 
			Json::Object(
				"hp", getHPinRatio(),
				"combo", getCombo(),
				"score", getScore(),
				"position", CORE_PTR->getRunPosition()
			));
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
		EVALUATE_STRATEGY_PTR->getResult().myScore = getScore();
		EVALUATE_STRATEGY_PTR->getResult().myID = getPlayerID();
		EVALUATE_STRATEGY_PTR->getResult().myMaxCombo = getMaxCombo();
		EVALUATE_STRATEGY_PTR->getResult().myMaxCTLevel = getMaxCTLevel();
		EVALUATE_STRATEGY_PTR->getResult().myHp = getHPinRatio();
		EVALUATE_STRATEGY_PTR->getResult().myIsOver = (bool)CORE_FLOW_PTR->isNoteOver();

		PLAYERS &players = getPlayerInfo();

		for(int i = 0; i < players.size(); i++)
			EVALUATE_STRATEGY_PTR->getResult().evalData.push_back(EvalData(players[i].uid, players[i].score, 0, 0, 0, false, players[i].teamIndex, POMELO_STAGE_PEER->waiterInfo(players[i].uid).nickname));
		//((CommonEvaluateStrategy*)EVALUATE_STRATEGY_PTR)->addMultiEvalUI();
	}

	PlayerInfo* MultiPlay::getSpecificPlayerInfo(const std::string uid)
	{
		if(mInfo==NULL)
			return NULL;
		for(int i = 0; i < mInfo->mPlayers.size(); i++)
			if(mInfo->mPlayers[i].uid == uid)
				return &mInfo->mPlayers[i];
		return NULL;
	}
#endif
}
