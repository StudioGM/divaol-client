/*
 *  DivaRelayPlay.h
 *
 *  Created by Hyf042 on 3/8/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_RELAY_PLAY
#define DIVA_RELAY_PLAY

#include "DivaMultiPlay.h"
#include "DivaRelayUI.h"

namespace divacore
{
	class RelayNote
	{
	public:
		StateEvent event;
		int noteID,pointCnt,turn;
		int combo,score;
		bool bOwner;
		float time;

		RelayNote(StateEvent &event, int noteID, int pointCnt, int turn, int score, int combo, float time):event(event),turn(turn)
		{
			this->noteID = noteID;
			this->pointCnt = pointCnt;
			this->score = score;
			this->combo = combo;
			bOwner = event.note==NULL?false:event.note->isOwner();
			this->time = time;
		}
	};

	typedef std::vector<RelayNote> RELAY_NOTES;

	class RelayPlay : public MultiPlay
	{
		class RelayPlay_NoteBlender : public Hook
		{
		public:
			std::string getName() {return "PVMode";}

			RelayPlay_NoteBlender()
			{
				setPriority(Hook::HIGH);
			}
			bool condition()
			{
				return true;
			}
			int getAbility()
			{
				return Hook::RENDER;
			}

			bool hook(float x, float y, SoraSprite*sprite, const std::string&tag) 
			{
				if(tag.find("not_mine")!=std::string::npos)
				{
					uint32 color = sprite->getColor();
					uint32 alpha = CGETA(color);
					sprite->setColor(CSETA(color,alpha/5));
					sprite->render(x,y);
					sprite->setColor(color);
					setHookInfo(1);
					return true;
				}
				return false;
			}
		};
		static const int BUFFER_POSITION = 192*4;
		static const int MIN_TIME_PERIOD = 192*4;
		static const int MAX_TIME_PERIOD = 192*30;
		int maxLastGrid;

		RelayPlay_NoteBlender *noteBlender;
		double changePosition,voidPosition,endPosition;
		std::vector<RELAY_NOTES> relayInfo;
		RELAY_NOTES relayQueue;

		int relayState;
		uint32 nowTurn,nowPlayer,nextPlayer;
		RelayPrompt *recvPrompt,*changePrompt;
		enum{MYTURN,CHANGE,WAIT,RELAY,APPLY,ACCEPT};
		int getRelayState() {return relayState;}
		void setRelayState(int relayState) {this->relayState=relayState;}
	public:

		//derived control funcs
		int getNowPlayer() {return nowPlayer;}

		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			MultiPlay::gameReset();

			RECEIVE_PACKET(network::DIVA_NET_STC_FAILURE,&RelayPlay::getFailure);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_TIME_UP,&RelayPlay::relayTimeUP);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_CHANCE,&RelayPlay::relayChance);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_YOU_CAN_PLAY,&RelayPlay::relayYouCanPlay);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_NEW_PLAYER,&RelayPlay::relayNewPlayer);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_RENEW,&RelayPlay::relayRenew);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_VOID_PERIOD,&RelayPlay::relayVoidPeriod);

			noteBlender = new RelayPlay_NoteBlender;
			HOOK_MANAGER_PTR->insert(noteBlender);
			nowTurn = 0;
			relayQueue.clear();
			relayInfo.clear();
			recvPrompt = NULL;
		}
		void gameStop()
		{
			MultiPlay::gameStop();
			if(noteBlender)
			{
				HOOK_MANAGER_PTR->del(noteBlender);
				SAFE_DELETE(noteBlender);
				noteBlender = NULL;
			}
		}
		void gameStart()
		{
			MultiPlay::gameStart();
			if(playerID==0)
				setRelayState(MYTURN);
			else
				setRelayState(WAIT);

			relayNewTurn();
		}

		void gameOver()
		{
			MultiPlay::gameOver();
			setRelayState(OVER);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_FAILURE);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_TIME_UP);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_CHANCE);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_YOU_CAN_PLAY);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_NEW_PLAYER);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_RENEW);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_VOID_PERIOD);
		}

		void gameReady()
		{
			maxLastGrid = (int)ceil(MAP_INFO->totalGrid/*header.barNum*GRID_PER_BAR*//teams[teamID].players.size()*1.5);
			endPosition = maxLastGrid;
			registerUI();

			nowPlayer = nextPlayer = 0;
		}

		void registerUI()
		{
			SimpleUIPainter *uiPainter = (SimpleUIPainter*)UI_PAINTER_PTR;
			recvPrompt = new RelayPrompt();
			uiPainter->construct(recvPrompt,"recvPrompt");
			recvPrompt->init();
			uiPainter->addWidget(recvPrompt);

			recvPrompt->setRelayState(RelayPrompt::NORMAL);

			changePrompt = new RelayPrompt();
			uiPainter->construct(changePrompt,"changePrompt");
			changePrompt->init();
			uiPainter->addWidget(changePrompt);

			changePrompt->setRelayState(RelayPrompt::NORMAL);
		}

		void setAlive(bool alive)
		{
			SinglePlay::setAlive(alive);
			if(alive==false)
				relayWantToChange();
		}

		//check funcs
		void checkNoteOver(NotePtr note)
		{
			stateList[note->getID()].over();
			//LOGGER->log("over %d %d",note->getID(),combo);
		}
		bool checkNote(NotePtr note) 
		{
			if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
			{				
				note->setOwner(false);
				note->setTailTag("_not_mine");
			}
			MultiPlay::checkNote(note);
			/*if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
			{	
				for(int i = 0; i < failure_list.size(); i++)
					if(note->getID()==failure_list[i])
					{
						failure_list.erase(failure_list.begin()+i);
						return false;
					}
			}*/
			return true;
		}
		bool checkPoint(StateEvent& event) 
		{
			if(!event.note->isOwner())
			{
				return true;
			}
			return SinglePlay::checkPoint(event);
		}
		bool checkFailure(StateEvent& event)
		{
			if(!MultiPlay::checkFailure(event))
				return false;
			return true;
		}
		void inform(StateEvent& event)
		{
			if(event.note->isOwner())
			{
				SinglePlay::inform(event);
		
				relayInfo[nowTurn].push_back(RelayNote(event,event.note->getID(),stateList[event.note->getID()].size()-1,
					nowTurn,getScore(),getCombo(),CORE_PTR->getRunTime()));

				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				{
					if(event.breakCombo||event.breakNote)
						sendFailure(event);
					sendRenew(event.note->getID(),event.rank,event.breakCombo,event.breakNote,event.getScreenPosition().x,event.getScreenPosition().y);
				}
			}
			else
			{
				//播放hit音
				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				{
					if(event.rank<=4)
						Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");
					else
						Core::Ptr->getMusicManager()->playDirect("miss","sound_effect");
				}

				if(event.type==StateEvent::PRESS)
					//show effect
					pressEffect(event);
				//	combo++;

				stateList[event.note->getID()].addKey(event);
				stateQueue.push_back(event);
			}
		}

		void sendFailure(StateEvent &event)
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_FAILURE,CORE_PTR->getRunTime(),
				"%d%b%b",event.note->getID(),event.breakCombo,event.breakNote);
		}

		void sendRenew(uint32 uid, int rank, bool breakCombo, bool breakNote, float positionX, float positionY)
		{
			int cnt = stateList[uid].eventList.size()-1;
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_RENEW,CORE_PTR->getRunTime(),
				"%d%d%d%d%b%b%f%f",nowTurn,uid,cnt,rank,breakCombo,breakNote,positionX,positionY);
		}

		void iAmPlaying()
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_I_AM_PLAYING,CORE_PTR->getRunTime(),
				"%f",CORE_PTR->getRunTime());
		}

		void onKeyPressed(KeyEvent& event)
		{
			if(event.key==DIVA_KEY_RELAY)
			{
				if(getRelayState()==MYTURN)
					relayWantToChange();
				else if(getRelayState()==RELAY)
						relayWantToPlay();
			}
		}

		void getFailure(Packet &packet)
		{
			uint32 uid = packet.readInt32();
			bool breakCombo = packet.readBool();
			bool breakNote = packet.readBool();
			/*if(breakNote)
			{
				if(!CORE_FLOW_PTR->toFail(uid))
					failure_list.push_back(uid);
				else
					MUSIC_MANAGER_PTR->playDirectWithFile("fail.wav",true);
			}*/
		}

		void relayWantToChange()
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_WANT_TO_CHANGE,CORE_PTR->getRunTime(),
				"%f",CORE_PTR->getRunPosition()+BUFFER_POSITION);

			changePosition = CORE_PTR->getRunPosition()+BUFFER_POSITION;

			setRelayState(CHANGE);
			changePrompt->setRelayState(RelayPrompt::HELP);
			changePrompt->showNumber(true);
		}
		void relayWantToPlay()
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_WANT_TO_PLAY,CORE_PTR->getRunTime());

			setRelayState(APPLY);
		}

		void relayNewTurn()
		{
			relayInfo.push_back(RELAY_NOTES());
		}
		void relayTimeUP(Packet &packet)
		{
		}
		void relayChance(Packet &packet)
		{
			if(!getAlive())
				return;
			changePosition = packet.readDouble();
			setRelayState(RELAY);

			recvPrompt->setRelayState(RelayPrompt::HELP);
		}
		void relayVoidPeriod(Packet &packet)
		{
			double begin = packet.readDouble(), end = packet.readDouble();
			if(combo)
			{
				STATE_QUEUE::reverse_iterator ptr = stateQueue.rbegin();
				while(ptr!=stateQueue.rend()&&!ptr->isOwner())
				{
					if(ptr->position<=end)
					{
						LOGGER->log("void %d %d",ptr->uid,stateList.size());
						sendRenew(ptr->uid,-1,true,true,0,0);

						//renew
						StateEvent event;
						event.breakCombo = true, event.breakNote = true;
						RelayNote data(event,ptr->uid,stateList[ptr->uid].eventList.size()-1,nowTurn,0,0,0);
						relayRenew(data);

						break;
					}
					ptr++;
				}
			}
		}
		void relayYouCanPlay(Packet &packet)
		{
			setRelayState(ACCEPT);
			nowTurn = packet.readInt32();
			relayNewTurn();
			nowPlayer = playerID;

			recvPrompt->setRelayState(RelayPrompt::GET);

			if(CORE_PTR->getRunPosition()>changePosition)
			{
				endPosition = CORE_PTR->getRunPosition()+maxLastGrid;
				//LOGGER->log("%0.3lf %0.3lf",CORE_PTR->getRunPosition(),endPosition);
				changePosition = 0, setRelayState(MYTURN);
				//nowPlayer = nextPlayer;
				iAmPlaying();
			}
		}
		void relayNewPlayer(Packet &packet)
		{
			relayNewTurn();

			nowPlayer = packet.readInt32();

			if(getRelayState()!=CHANGE)
			{
				setRelayState(WAIT);
				recvPrompt->setRelayState(RelayPrompt::REJECT);
			}
			if(changePrompt->getRelayState()==RelayPrompt::HELP)
			{
				changePrompt->setRelayState(RelayPrompt::GET);
			}
		}
		void relayRenew(Packet &packet)
		{
			uint32 turn = packet.readInt32();
			uint32 pID = packet.readInt32();
			uint32 cnt = packet.readInt32();
			int rank = packet.readInt32();
			bool breakCombo = packet.readBool();
			bool breakNote = packet.readBool();
			float positionX = packet.readDouble();
			float positionY = packet.readDouble();
			Point position = Point(positionX,positionY);

			StateEvent event;
			event.type = StateEvent::PRESS, event.tag = "inform", event.bOwner = false, event.rank = rank;
			event.breakCombo = breakCombo, event.breakNote = breakNote, event.setScreenPosition(position);
			RelayNote note(event,pID,cnt,turn,0,0,packet.getTime());

			if(packet.getTime()>CORE_PTR->getRunTime())//stateList.size()<=pID||stateList[pID].isOver()&&stateList[pID].eventList.size()<=cnt)
			{
				relayQueue.push_back(note);
				return;
			} 
			else if(breakNote)
			{
				MUSIC_MANAGER_PTR->playDirect("miss","sound_effect");
				CORE_FLOW_PTR->toFail(pID);
			}

			relayRenew(note);

			//if(!stateList[pID].isOver())
			HOOK_MANAGER_PTR->hook(event);
		}
		void relayRenew(RelayNote &data)
		{
			int score = 0, combo = 0, turn = data.turn;
			while(turn>=0)
			{
				if(relayInfo[turn].size())
				{
					score = relayInfo[turn].rbegin()->score;
					combo = relayInfo[turn].rbegin()->combo;
					break;
				}
				turn--;
			}
			if(data.event.breakCombo)
				combo = 0;
			else
				combo++;

			data.score = score;
			data.combo = combo;

			turn = data.turn;
			relayInfo[turn].push_back(data);
			
			for(int i = turn+1; i <= nowTurn; i++)
				for(int j = 0; j < relayInfo[i].size(); j++)
				{
					StateEvent &event = relayInfo[i][j].event;
					if(relayInfo[i][j].bOwner)
						modifyScore(event.dtScore,score);
					if(event.type!=StateEvent::PRESS&&event.type!=StateEvent::FAILURE)
						continue;
					if(event.breakCombo)
						combo = 0;
					else
						combo++;
					if(relayInfo[i][j].bOwner) {
						StateEvent ret = EVALUATE_STRATEGY_PTR->evaluateCombo(event,combo);
						modifyScore(ret.dtScore,score);
					}
					relayInfo[i][j].score = score;
					relayInfo[i][j].combo = combo;
				}

			setScore(score);
			setCombo(combo);
		}

		void update(float dt)
		{
			MultiPlay::update(dt);
			if(getBaseState()==OVER)
				return;
			else if(getRelayState()==CHANGE)
			{
				if(CORE_PTR->getRunPosition()>changePosition)
				{
					changePosition = 0, setRelayState(WAIT);

					changePrompt->showNumber(false);
				}
				else
				{
					changePrompt->setNumber((int)ceil((changePosition-CORE_PTR->getRunPosition())*4/GRID_PER_BAR));
				}
			}
			else if(getRelayState()==ACCEPT)
			{
				if(CORE_PTR->getRunPosition()>changePosition)
				{
					endPosition = changePosition+maxLastGrid;
					changePosition = 0, setRelayState(MYTURN);

					recvPrompt->setRelayState(RelayPrompt::NORMAL);
					recvPrompt->showNumber(false);

					iAmPlaying();
				}
				else
				{
					if(recvPrompt->getRelayState()==RelayPrompt::NORMAL)
						recvPrompt->showNumber(true);
					recvPrompt->setNumber((int)ceil((changePosition-CORE_PTR->getRunPosition())*4/GRID_PER_BAR));
				}
			}
			else if(getRelayState()==MYTURN)
			{
				if(CORE_PTR->getRunPosition()>endPosition)
				{
					//LOGGER->log("end %0.3lf %0.3lf",CORE_PTR->getRunPosition(),endPosition);
					relayWantToChange();
				}
			}

			while(!relayQueue.empty()&&relayQueue.begin()->time<=CORE_PTR->getRunTime())
			{
				if(stateList.size()>relayQueue.begin()->noteID)
				{
					KeyState &keyState = stateList[relayQueue.begin()->noteID];
					//if(keyState.eventList.size()>relayQueue.begin()->pointCnt||keyState.isOver())
					//{
					if(relayQueue.begin()->event.breakNote&&!keyState.isOver())
					{
						MUSIC_MANAGER_PTR->playDirect("miss","sound_effect");
						CORE_FLOW_PTR->toFail(keyState.noteID);
					}

					relayRenew(*relayQueue.begin());

					//if(!keyState.isOver())
					HOOK_MANAGER_PTR->hook(relayQueue.begin()->event);

					relayQueue.erase(relayQueue.begin());
				}
			}
			
			/*if(relayQueue.size())
				if(stateList.size()>relayQueue.begin()->noteID)
				{
					KeyState &keyState = stateList[relayQueue.begin()->noteID];
					if(keyState.eventList.size()>relayQueue.begin()->pointCnt||keyState.isOver())
					{
						relayRenew(*relayQueue.begin());
						relayQueue.erase(relayQueue.begin());
					}
				}*/
		}

		void render()
		{
			MultiPlay::render();

			if(getBaseState()==PLAY)
			{
#ifdef _DEBUG
				//显示一些调试信息
				std::string output;
				switch(getRelayState())
				{
				case MYTURN:
					output = "This is your turn";
					break;
				case CHANGE:
					output = "Change time";
					break;
				case WAIT:
					output = "Wait";
					break;
				case RELAY:
					output = "Relay time";
					break;
				case APPLY:
					output = "Applying...";
					break;
				case ACCEPT:
					output = "Accpeted, ready...";
					break;
				default:
					output = "";
					break;
				}

				mText.setText(sora::s2ws(output));
				mText.renderTo(200,0);
#endif
			}
		}
	};
}

#endif