/*
 *  DivaEvaluateStrategy.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EVALUATE_STRATEGY
#define DIVA_EVALUATE_STRATEGY

#include "DivaCommon.h"
#include "DivaNote.h"
#include "DivaMapInfo.h"
#include "DivaEventManager.h"
#include "DivaGameMode.h"

namespace divacore
{
	class EvalData;
	class EvalResult;

	/*
	EvaluateStrategy
	*/
	class EvaluateStrategy : public EventHandler
	{
	public:
		static const int EVAL_NUM = 5;

		virtual EvalResult& getResult() = 0;
		virtual bool evaluatePress(StateEvent&) = 0;
		virtual void evaluateFaliure(StateEvent&) = 0;
		virtual StateEvent evaluateCombo(StateEvent&, int combo) = 0;
		virtual void update(float dt) = 0;
		virtual float getProtectedTime() = 0;
		virtual void registerState(StateEvent &event) = 0;
		virtual void finalEvaluate() = 0;
		virtual void prepare(std::string configFile) = 0;
	};

	class EvalData
	{
	public:
		std::string uid;
		Base::String nickname;
		int index;
		int score;
		int maxCombo;
		int maxCTLevel;
		std::string status;
		int cntEval[EvaluateStrategy::EVAL_NUM];
		EvalData():uid(""),score(0),nickname("unknown") {memset(cntEval,0,sizeof(cntEval));}
		EvalData(const std::string &uid, int score, int maxCombo, int maxCTLevel, int cntEval[], int index = 0, const std::string nickname = "unknown") {
			this->index = index;
			this->uid = uid;
			this->score = score;
			this->nickname = nickname;
			this->maxCombo = maxCombo;
			this->maxCTLevel = maxCTLevel;
			memcpy(this->cntEval,cntEval,sizeof(int)*EvaluateStrategy::EVAL_NUM);
		}
		EvalData(const std::string &uid, int score, int maxCombo, int maxCTLevel, int index = 0, const std::string nickname = "unknown") {
			this->index = index;
			this->uid = uid;
			this->score = score;
			this->nickname = nickname;
			this->maxCombo = maxCombo;
			this->maxCTLevel = maxCTLevel;
			memset(cntEval,0,sizeof(int)*EvaluateStrategy::EVAL_NUM);
		}
		friend bool operator<(const EvalData &a, const EvalData &b) {
			return a.score>b.score;
		}
	};

	class EvalResult
	{
	public:
		typedef std::vector<EvalData> EVALDATA;

		EVALDATA evalData;
		int myID,myScore;
		int myMaxCombo,myMaxCTLevel;
		int myCntEval[EvaluateStrategy::EVAL_NUM];

		void clear() {
			myID = myScore = 0;
			myMaxCombo = myMaxCTLevel = 0;
			memset(myCntEval,0,sizeof(myCntEval));
			evalData.clear();
		}
	};

	typedef EvaluateStrategy* EvaluateStrategyPtr;
}

#endif