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
	/*
	EvaluateStrategy
	*/
	class EvaluateStrategy : public EventHandler
	{
	public:
		static const int EVAL_NUM = 5;

		virtual bool evaluatePress(StateEvent&) = 0;
		virtual void evaluateFaliure(StateEvent&) = 0;
		virtual StateEvent evaluateCombo(StateEvent&, int combo) = 0;
		virtual void update(float dt) = 0;
		virtual float getProtectedTime() = 0;
		virtual void registerState(StateEvent &event) = 0;
		virtual void finalEvaluate() = 0;
		virtual void prepare(std::string configFile) = 0;
	};

	typedef EvaluateStrategy* EvaluateStrategyPtr;
}

#endif