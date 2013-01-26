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
#include "divanetwork/DivaNetInfo.h"

namespace divacore
{
	/*
	EvaluateStrategy
	*/
	class CommonEvaluateStrategy : public EvaluateStrategy
	{
		typedef std::vector<SimpleUI::EvalBar*> EVALBAR;
		float weight;
		//PLAYERS players;
		EVALBAR evals;

		//user define args
		float range_rank[EVAL_NUM];
		float protectedTime;

		EvalResult::EVALDATA mRank;
		EvalResult mResult;

		static const int MAX_TEAM = 8;
		int teamScore[MAX_TEAM];
	public:

		void gameReset()
		{
			mResult.clear();
		}

		EvalResult& getResult() {
			return mResult;
		}

		void prepare(std::string configFile)
		{
			if(configFile == "" || Base::FileUtil::FileExist(configFile))
			{
				range_rank[0] = 0.03;
				range_rank[1] = 0.05;
				range_rank[2] = 0.06;
				range_rank[3] = 0.07;
				protectedTime = 0.1;
			}
			else {
				Config config;
				configloader::loadWithJson(config,configFile);
				for(int i = 0; i < EVAL_NUM-1; i++)
					range_rank[i] = config.getAsDouble("range_rank"+iToS(i+1));

				protectedTime = config.getAsDouble("protectedTime");
			}
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
				mResult.myCntEval[event.rank-1]++;
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
			if(CORE_PTR->getState()!=Core::RESULT)
				return;

			// NETWORK CAUTION
			mRank = mResult.evalData;
			_sortRank();
			for(int i = 0; i < mRank.size(); i++)
			{
				evals[i]->setInfo(mRank[i]);
				evals[i]->setTeamColor(mRank[i].index);
			}
		}
		void addSingleEvalUI()
		{
			/*divacore::SimpleUIPainter * uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;
			SimpleUI::EvalBar *eval;

			eval = (SimpleUI::EvalBar *)uiPainter->createWidget("header");
			eval->setInfo(mResult.myScore,mResult.myMaxCombo,mResult.myMaxCTLevel,mResult.myCntEval,NET_INFO.nickname);
			uiPainter->addWidget(eval);*/
		}
		void addMultiEvalUI()
		{
			// NETWORK CAUTION
			divacore::SimpleUIPainter * uiPainter = (divacore::SimpleUIPainter*)UI_PAINTER_PTR;
			
			evals.clear();
			mRank = mResult.evalData;
			_sortRank();
			for(int i = 0; i < mRank.size(); i++) {
				SimpleUI::EvalBar *eval = (SimpleUI::EvalBar *)uiPainter->createWidget(i==0?"header":"bar");
				//eval->addIcon(players[i].netID);
				eval->setPosition(eval->getPositionX(),(i==0)?82:(216+(i-1)*87));
				uiPainter->addWidget(eval);
				evals.push_back(eval);
			}

			updateInfo();
		}
		void finalEvaluate()
		{
			GAME_MODE_PTR->preEvaluate();

			GAME_MODE_PTR->afterEvaluate();
		}

		bool compare(const EvalData &a, const EvalData &b)
		{
			if(a.index!=b.index)
				return teamScore[a.index] > teamScore[b.index];
			return a.score > b.score;
		}

		void _sortRank()
		{
			memset(teamScore,0,sizeof(teamScore));
			for(int i = 0; i < mRank.size(); i++)
				teamScore[mRank[i].index] += mRank[i].score;

			for(int i = 0; i < mRank.size(); i++)
				for(int j = 0; j < mRank.size()-i-1; j++)
					if(compare(mRank[j+1],mRank[j]))
						std::swap(mRank[j],mRank[j+1]);
		}

		bool isNumberUp()
		{
			bool isUp = false;
			for(int i = 0; i < evals.size(); i++)
				isUp |= evals[i]->isNumberUp();
			return isUp;
		}
	};
}

#endif