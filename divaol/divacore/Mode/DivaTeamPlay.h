/*
 *  DivaTeamPlay.h
 *
 *  Created by Hyf042 on 2/10/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_TEAMPLAY
#define DIVA_TEAMPLAY

#include "DivaMultiPlay.h"

namespace divacore
{
	class PairNote
	{
	public:
		int noteID,pointCnt;
		float time;
		bool breakCombo,breakNote;
		PairNote(int noteID, int pointCnt,bool breakCombo, bool breakNote, float time)
			:noteID(noteID),pointCnt(pointCnt),breakCombo(breakCombo),breakNote(breakNote),time(time)
		{}
	};
	class TeamPlay : public MultiPlay
	{
		//note Blender，把不属于自己的note变淡
		class TeamPlay_NoteBlender : public Hook
		{
		public:
			std::string getName() {return "PVMode";}

			TeamPlay_NoteBlender()
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
		//std::vector<uint32> failure_list;
		TeamPlay_NoteBlender *noteBlender;

		typedef std::vector<PairNote> PAIR_NOTES;
		PAIR_NOTES pairQueue;
	public:
		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			MultiPlay::gameReset();
			RECEIVE_PACKET(network::DIVA_NET_STC_FAILURE,&TeamPlay::getFailure);

			noteBlender = new TeamPlay_NoteBlender;
			HOOK_MANAGER_PTR->insert(noteBlender);

			pairQueue.clear();
		}
		void gameOver()
		{
			MultiPlay::gameOver();
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_FAILURE);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_TIME_UP);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_CHANCE);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_YOU_CAN_PLAY);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_NEW_PLAYER);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_RENEW);
			NETWORK_SYSTEM_PTR->unreceive(network::DIVA_NET_STC_RELAY_VOID_PERIOD);
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
		bool checkNote(NotePtr note) 
		{
			if(note->getID()%2!=playerID)
			{
				//if(failure_list.size()&&note->getID()==failure_list[0])
				//	return false;
				note->setOwner(false);
				note->setTailTag("_not_mine");
			}
			MultiPlay::checkNote(note);
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
			if(!MultiPlay::checkFailure(event))
				return false;
			//sendFailure(event);
			return true;
		}
		void inform(StateEvent& event)
		{
			if(event.note->isOwner())
			{
				SinglePlay::inform(event);

				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
					if(event.breakCombo||event.breakNote)
						sendFailure(event);
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
			}
		}

		void sendFailure(StateEvent &event)
		{
			int cnt = stateList[event.note->getID()].eventList.size()-1;
			NETWORK_SYSTEM_PTR->send(network::DIVA_NET_CTS_FAILURE,CORE_PTR->getRunTime(),
				"%d%d%b%b",event.note->getID(),cnt,event.breakCombo,event.breakNote);
		}

		void getFailure(Packet &packet)
		{
			uint32 pID = packet.readInt32();
			uint32 cnt = packet.readInt32();
			bool breakCombo = packet.readBool();
			bool breakNote = packet.readBool();

			PairNote note(pID,cnt,breakCombo,breakNote,packet.getTime());

			if(packet.getTime()>CORE_PTR->getRunTime())//stateList.size()<=pID||stateList[pID].isOver()&&stateList[pID].eventList.size()<=cnt)
			{
				pairQueue.push_back(note);
				return;
			} 
			else
			{
				MUSIC_MANAGER_PTR->playDirect("miss","sound_effect");
				CORE_FLOW_PTR->toFail(pID);
			}
			/*MUSIC_MANAGER_PTR->playDirectWithFile("fail.wav",true);
			if(breakNote)
			{
				if(!CORE_FLOW_PTR->toFail(uid))
					failure_list.push_back(uid);
			}*/
		}

		void update(float dt)
		{
			MultiPlay::update(dt);
			if(getBaseState()==OVER)
				return;

			while(!pairQueue.empty()&&pairQueue.begin()->time<=CORE_PTR->getRunTime())
			{
				if(stateList.size()>pairQueue.begin()->noteID)
				{
					//if(keyState.eventList.size()>relayQueue.begin()->pointCnt||keyState.isOver())
					//{
					if(pairQueue.begin()->breakNote)
					{
						MUSIC_MANAGER_PTR->playDirect("miss","sound_effect");
						CORE_FLOW_PTR->toFail(pairQueue.begin()->noteID);
					}
					pairQueue.erase(pairQueue.begin());
				}
			}
		}
	};
}

#endif