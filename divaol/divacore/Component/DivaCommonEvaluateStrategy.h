/*
 *  DivaCommonEvaluateStrategy.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_COMMON_EVALUATE_STRATEGY
#define DIVA_COMMON_EVALUATE_STRATEGY

#include "Core/DivaEvaluateStrategy.h"
#include "Component/DivaSimpleUIPainter.h"
#include "Utility/DivaConfigLoader.h"

namespace divacore
{
	class EvalData
	{
	public:
		int score;
		int cntEval[EvaluateStrategy::EVAL_NUM];
		EvalData():score(0) {memset(cntEval,0,sizeof(cntEval));}
	};
	/*
	EvaluateStrategy
	*/
	class CommonEvaluateStrategy : public EvaluateStrategy
	{
		typedef std::vector<EvalData> EVALDATA;
		typedef std::vector<SimpleUI::EvalBar*> EVALBAR;
		float weight;
		int cntEval[EVAL_NUM];
		int score,playerID;
		//PLAYERS players;
		EVALDATA evalData;
		EVALBAR evals;

		//user define args
		float range_rank[EVAL_NUM];
		float protectedTime;

	public:

		void gameReset()
		{
			memset(cntEval,0,sizeof(cntEval));
		}

		void prepare(std::string configFile)
		{
			Config config;
			configloader::loadWithJson(config,configFile);
			for(int i = 0; i < EVAL_NUM-1; i++)
				range_rank[i] = config.getAsDouble("range_rank"+iToS(i+1));

			protectedTime = config.getAsDouble("protectedTime");
		}

		bool evaluatePress(StateEvent& event)
		{
			if(event.dtTime>0.1)
				return false;
			if(event.dtTime<=range_rank[0]+weight)
			{
				event.rank = 1;

				event.dtScore = 400;
				event.dtHP = 1; 
			}
			else if(event.dtTime<=range_rank[1]+weight)
			{
				event.rank = 2;

				event.dtScore = 200;
				event.dtHP = 0; 
			}
			else if(event.dtTime<=range_rank[2]+weight)
			{
				event.rank = 3;
				event.breakCombo = true;

				event.dtScore = 50;
				event.dtHP = 0;
			}
			else if(event.dtTime<=range_rank[3]+weight)
			{
				event.rank = 4;
				event.breakCombo = true;
				event.breakNote = true;

				event.dtScore = 0;
				event.dtHP = -2;
			}
			else
			{
				event.rank = 5;
				event.breakCombo = true;
				event.breakNote = true;

				event.dtScore = 0;
				event.dtHP = -4;
			}
			return true;
		}
		virtual void evaluateFaliure(StateEvent& event)
		{
			event.rank = 5;
			event.dtHP = -4;
			event.dtScore = 0;
			event.breakCombo = true;
			event.breakNote = true;
		}
		virtual StateEvent evaluateCombo(StateEvent&event, int combo)
		{
			StateEvent ret;
			if(event.rank==1)
			{
				ret.dtHP = min(100,combo)/50;
				ret.dtScore = min(50,combo)/10*10;
			}
			else if(event.rank==2)
			{
				ret.dtHP = min(100,combo)/50;
				ret.dtScore = min(50,combo)/10*5;
			}
			return ret;
		}
		virtual void update(float dt)
		{
			weight = min<int>(200,Core::Ptr->getCoreFlow()->getBPM())/20*0.002;
		}
		float getProtectedTime() {return protectedTime;}
		void registerState(StateEvent &event) 
		{
			if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				cntEval[event.rank-1]++;
		}
		//void updateEval(Packet &packet)
		//{
		//	for(int i = 0; i < evals.size(); i++)
		//	{
		//		EvalData data;
		//		data.score = packet.readInt32();
		//		for(int j = 0; j < EVAL_NUM; j++)
		//			data.cntEval[j] = packet.readInt32();
		//		if(i!=playerID)
		//			evalData[i] = data;
		//	}
		//	updateInfo();
		//	//for(int i = 0; i < players.size(); i++)
		//}
		void updateInfo()
		{
			// NETWORK CAUTION
			//for(int i = 0; i < players.size(); i++)
			//	evals[i]->setInfo(evalData[players[i].id].score,evalData[players[i].id].cntEval);
		}
		void addSingleEvalUI()
		{
			score = GAME_MODE_PTR->getScore();

			divacore::SimpleUIPainter * uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;
			SimpleUI::EvalBar *eval;

			eval = (SimpleUI::EvalBar *)uiPainter->createWidget("header");
			eval->setInfo(score,cntEval);
			uiPainter->addWidget(eval);
		}
		void addMultiEvalUI()
		{
			// NETWORK CAUTION

			//score = GAME_MODE_PTR->getScore();

			//divacore::SimpleUIPainter * uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;
			//SimpleUI::EvalBar *eval;

			//evals.clear();
			//evalData.clear();
			//players.clear();

			//MultiPlay *multiplay = (MultiPlay*)GAME_MODE_PTR;
			//TEAMS &teams = multiplay->getGlobalInfo();
			//for(int i = 0; i < teams.size(); i++)
			//	for(int j = 0; j < teams[i].players.size(); j++)
			//	{
			//		players.push_back(teams[i].players[j]);
			//		evalData.push_back(EvalData());
			//	}//here

			//	sort(players.begin(),players.end());

			//	for(int i = 0; i < players.size(); i++)
			//	{
			//		eval = (SimpleUI::EvalBar *)uiPainter->createWidget(i==0?"header":"bar");
			//		//eval->addIcon(players[i].netID);
			//		eval->setPosition(eval->getPositionX(),(i==0)?82:(216+(i-1)*87));
			//		uiPainter->addWidget(eval);
			//		evals.push_back(eval);
			//	}

			//playerID = multiplay->getPlayerInfo()->id;
			//evalData[playerID].score = score;
			//for(int i = 0; i < EVAL_NUM; i++)
			//	evalData[playerID].cntEval[i] = cntEval[i];

			//updateInfo();

			//Packet packet(network::DIVA_NET_CTS_EVAL_INFO,CORE_PTR->getRunTime());
			//packet.write<int32>(score);
			//for(int i = 0; i < EVAL_NUM; i++)
			//	packet.write<int32>(cntEval[i]);
			//NETWORK_SYSTEM_PTR->send(packet);
		}
		void finalEvaluate()
		{
			GAME_MODE_PTR->preEvaluate();
			GAME_MODE_PTR->afterEvaluate();
		}
	};
}

#endif