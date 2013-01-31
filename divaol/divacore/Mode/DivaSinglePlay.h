/*
 *  DivaSinglePlay.h
 *
 *  Created by Hyf042 on 1/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STANDARD_STATE_MANAGER
#define DIVA_STANDARD_STATE_MANAGER

#include "Core/DivaGameMode.h"
#include "Utility/DivaConfigLoader.h"
#include "Core/DivaCore.h"

namespace divacore
{
	class KeyState
	{
	public:
		typedef std::vector<StateEvent> EVENT_LIST;
		EVENT_LIST eventList;
		int noteID;
		bool bOwner,bOver;
		KeyState(NotePtr note) {
			bOwner = note->isOwner();
			noteID = note->getID();
			bOver = false;
		}
		void addKey(StateEvent &event) {
			eventList.push_back(event);
		}
		bool isOver() {return bOver;}
		void over() {bOver = true;}
		size_t size() {return eventList.size();}
		StateEvent& operator[](int index) {
			return eventList[index];
		}
	};

	//SinglePlay, 单人模式的模块控制和数据处理
	class SinglePlay : public GameMode
	{
	protected:
		static const int MAX_HP = 100;
		static const int ORIGIN_HP = 50;
		typedef std::vector<KeyState> STATE_LIST;
		typedef std::vector<StateEvent> STATE_QUEUE;
		STATE_LIST stateList; //记录每个note的进行状况和信息
		STATE_QUEUE stateQueue; //记录state序列
		Config config;
		bool alive;
		bool isOver;

		// statistic information
		int32 nowScore,nowHP;
		float weight; //根据当前的bpm计算的权值
		int combo;
		int maxCombo;
		int maxCTLevel;
	protected:
		virtual void setAlive(bool alive) {this->alive = alive;} 
	public:		
		virtual std::string getName() {return "singlePlay";}
		virtual std::string getNetGameMode() {return "none";}
			
		bool getAlive() {return alive;}
		int32 getScore() {return nowScore;}
		int32 getHP() {return nowHP;}
		float getHPinRatio() {return nowHP/float(MAX_HP);}
		int32 getCombo() {return combo;}
		int32 getMaxCombo() {return maxCombo;}
		int32 getMaxCTLevel() {return maxCTLevel;}
		void setScore(int32 score) {nowScore=score;}
		void setHP(int32 hp) {nowHP=hp;}
		void setCombo(int32 combo) {this->combo = combo;}
		float getProtectedTime(); //保护时间，也就是一个note最后能被接受的时间

		void modifyHP(int dtHP, int &nowHP);
		void modifyScore(int dtScore, int &nowScore);

		void gameReset() ;
		void gameOver();

		bool checkNote(NotePtr note);
		bool checkExtra(StateEvent& event);
		bool checkPress(StateEvent& event);
		bool checkPoint(StateEvent& event);
		bool checkFailure(StateEvent& event);
		void inform(StateEvent& event);
		void update(float dt);

		void pressEffect(StateEvent &event);

		void preStart();

		void preEvaluate();
		void afterEvaluate();
		void noteOver();
	};
}

#endif