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

namespace divacore
{
	class RelayRenewData
	{
	public:
		int uid;
		int cnt;
		bool breakCombo;
		bool breakNote;
		RelayRenewData(int uid, int cnt, bool breakCombo, bool breakNote)
			:uid(uid),cnt(cnt),breakCombo(breakCombo),breakNote(breakNote){}
	};
	class RelayPlay : public Multiplay
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
		std::vector<uint32> failure_list;
		RelayPlay_NoteBlender *noteBlender;
		double changePosition;
		std::vector<RelayRenewData> renewData;

		int relayState,nowTurn;
		enum{MYTURN,CHANGE,WAIT,RELAY,APPLY,ACCEPT};
		int getRelayState() {return relayState;}
		void setRelayState(int relayState) {this->relayState=relayState;}
	public:

		void init()
		{
			Multiplay::init();
			RECEIVE_PACKET(network::DIVA_NET_STC_FAILURE,&RelayPlay::getFailure);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_TIME_UP,&RelayPlay::relayTimeUP);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_CHANCE,&RelayPlay::relayChance);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_YOU_CAN_PLAY,&RelayPlay::relayYouCanPlay);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_NEW_PLAYER,&RelayPlay::relayNewPlayer);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_RENEW_FORWARD,&RelayPlay::relayRenew);
			RECEIVE_PACKET(network::DIVA_NET_STC_RELAY_RENEW_BACKWARD,&RelayPlay::relayRenew);
		}
		void gameReset()
		{
			Multiplay::gameReset();
			noteBlender = new RelayPlay_NoteBlender;
			HOOK_MANAGER_PTR->insert(noteBlender);
			nowTurn = 0;
			renewData.clear();
		}
		void gameStop()
		{
			Multiplay::gameStop();
			HOOK_MANAGER_PTR->del(noteBlender);
			SAFE_DELETE(noteBlender);
			noteBlender = NULL;
		}
		void gameStart()
		{
			Multiplay::gameStart();
			if(playerID==0)
				setRelayState(MYTURN);
			else
				setRelayState(WAIT);
		}
		void checkNoteOver(NotePtr note)
		{
			stateList[note->getID()].over();
			LOGGER->log("over %d %d",note->getID(),combo);
			for(int i = 0; i < renewData.size(); i++)
				if(note->getID()==renewData[i].uid)
				{
					relayRenew(renewData[i]);
					renewData.erase(renewData.begin()+i);
					break;
				}
		}
		bool checkNote(NotePtr note) 
		{
			if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
			{				
				note->setOwner(false);
				note->setTailTag("_not_mine");
			}
			Multiplay::checkNote(note);
			if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
			{	
				for(int i = 0; i < failure_list.size(); i++)
					if(note->getID()==failure_list[i])
					{
						StateEvent event;
						event.breakCombo = event.breakNote = true;
						stateList[note->getID()].addKey(event,getScore(),getCombo());
						failure_list.erase(failure_list.begin()+i);
						return false;
					}
			}
			return true;
		}
		bool checkPoint(StateEvent& event) 
		{
			if(!event.note->isOwner())
				return true;
			return SinglePlay::checkPoint(event);
		}
		bool checkFailure(StateEvent& event)
		{
			if(!Multiplay::checkFailure(event))
				return false;
			return true;
		}
		void inform(StateEvent& event)
		{
			if(event.note->isOwner())
			{
				SinglePlay::inform(event);
		
				if(event.breakCombo||event.breakNote)
					sendFailure(event);
				
				sendRenew(event.note->getID(),event.breakCombo,event.breakNote);
			}
			else
			{
				if(event.type==StateEvent::PRESS)
					combo++;

				stateList[event.note->getID()].addKey(event,getScore(),getCombo());
			}
		}

		void sendFailure(StateEvent &event)
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_FAILURE,CORE_PTR->getRunTime(),
				"%d%b%b",event.note->getID(),event.breakCombo,event.breakNote);
		}

		void sendRenew(uint32 uid, bool breakCombo, bool breakNote)
		{
			int cnt = stateList[uid].eventList.size()-1;
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_RENEW,CORE_PTR->getRunTime(),
				"%d%d%d%b%b",nowTurn,uid,cnt,breakCombo,breakNote);
		}

		void onKeyPressed(KeyEvent& event)
		{
			if(event.key==DIVA_KEY_RELAY_WANT_TO_CHANGE)
			{
				if(getRelayState()==MYTURN)
					relayWantToChange();
			}
			else if(event.key==DIVA_KEY_RELAY_WANT_TO_PLAY)
			{
				if(getRelayState()==RELAY)
					relayWantToPlay();
			}
		}

		void getFailure(Packet &packet)
		{
			uint32 uid = packet.readInt32();
			bool breakCombo = packet.readBool();
			bool breakNote = packet.readBool();
			MUSIC_MANAGER_PTR->playDirectWithFile("fail.wav",true);
			if(breakNote)
			{
				if(!CORE_FLOW_PTR->toFail(uid))
					failure_list.push_back(uid);
			}
		}
		void relayWantToChange()
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_WANT_TO_CHANGE,CORE_PTR->getRunTime(),
				"%f",CORE_PTR->getRunPosition()+BUFFER_POSITION);

			changePosition = CORE_PTR->getRunPosition()+BUFFER_POSITION;

			setRelayState(CHANGE);
		}
		void relayWantToPlay()
		{
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_RELAY_WANT_TO_PLAY,CORE_PTR->getRunTime());

			setRelayState(APPLY);
		}

		void relayTimeUP(Packet &packet)
		{
		}
		void relayChance(Packet &packet)
		{
			changePosition = packet.readDouble();
			setRelayState(RELAY);
		}
		void relayYouCanPlay(Packet &packet)
		{
			setRelayState(ACCEPT);
			nowTurn = packet.readInt32();
			if(CORE_PTR->getRunTime()>changePosition)
				changePosition = 0, setRelayState(MYTURN);
		}
		void relayNewPlayer(Packet &packet)
		{
			if(getRelayState()!=CHANGE)
				setRelayState(WAIT);
		}
		void relayRenew(Packet &packet)
		{
			uint32 pID = packet.readInt32();
			uint32 cnt = packet.readInt32();
			bool breakCombo = packet.readBool();
			bool breakNote = packet.readBool();
			RelayRenewData data = RelayRenewData(pID,cnt,breakCombo,breakNote);
			LOGGER->log("renew %d %d %d %d",pID,cnt,breakCombo,breakNote);
			if(stateList.size()<=pID||stateList[pID].eventList.size()<=cnt&&!stateList[pID].isOver())
			{
				renewData.push_back(data);
				return;
			}
			relayRenew(data);
		}
		void relayRenew(RelayRenewData data)
		{
			uint32 pID = data.uid;
			uint32 cnt = data.cnt;
			bool breakCombo = data.breakCombo;
			bool breakNote = data.breakNote;
			LOGGER->log("renew data %d %d %d %d",pID,cnt,breakCombo,breakNote);
			int combo = breakCombo?0:stateList[pID].combo;
			int score = stateList[pID].score;

			if(stateList[pID].eventList.size()>cnt)
			{
				stateList[pID].eventList[cnt].breakCombo = breakCombo;
				stateList[pID].eventList[cnt].breakNote = breakNote;
			}

			if(!breakNote)
			{
				for(int i = cnt+1; i < stateList[pID].eventList.size(); i++)
				{
					StateEvent &event = stateList[pID].eventList[i];
					if(event.type==StateEvent::PRESS)
						if(event.breakCombo)
						{
							combo = 0;
							if(event.breakNote)
								break;
						}
						else
							combo++;
				}
			}
			stateList[pID].score = score;
			stateList[pID].combo = combo;
			++pID;
			while(pID<stateList.size())
			{
				for(int i = 0; i < stateList[pID].eventList.size(); i++)
				{
					StateEvent &event = stateList[pID].eventList[i];
					if(stateList[pID].bOwner)
						modifyScore(event.dtScore,score);
					if(event.type==StateEvent::PRESS)
						if(stateList[pID].eventList[i].breakCombo)
						{
							combo = 0;
							if(stateList[pID].eventList[i].breakNote)
								break;
						}
						else
						{
							combo++;
							StateEvent ret = EVALUATE_STRATEGY_PTR->evaluateCombo(event,combo);
							modifyScore(event.dtScore,score);
						}
				}
				stateList[pID].score = score;
				stateList[pID].combo = combo;
				++pID;
			}
			setScore(score);
			setCombo(combo);
		}

		void update(float dt)
		{
			Multiplay::update(dt);
			if(getRelayState()==CHANGE)
			{
				if(CORE_PTR->getRunPosition()>changePosition)
					changePosition = 0, setRelayState(WAIT);
			}
			else if(getRelayState()==ACCEPT)
			{
				if(CORE_PTR->getRunPosition()>changePosition)
					changePosition = 0, setRelayState(MYTURN);
			}
		}

		void render()
		{
			Multiplay::render();

			if(getState()==PLAY)
			{
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
				output  += format("\n%d",stateList.size());
				if(stateList.size())
				{
					for(STATE_LIST::reverse_iterator ptr = stateList.rbegin(); ptr != stateList.rend(); ptr++)
						if(ptr->eventList.size())
						{
							output += format("\n%d",ptr->score);
							break;
						}
				}

				mText.setText(sora::s2ws(output));
				mText.renderTo(200,0);
			}
		}
	};
}

#endif