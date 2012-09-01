/*
 *  DivaPairPlay.h
 *
 *  Created by Hyf042 on 8/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_PAIR_PLAY_H
#define DIVA_PAIR_PLAY_H

#include "DivaMultiPlay.h"

namespace divacore
{
	class PairGameInfo : public NetGameInfo
	{
	public:
		void newGame(GPacket *packet)
		{
			gnet::Item<gnet::List> *list = dynamic_cast<gnet::Item<gnet::List>*>(dynamic_cast<GPacket*>(packet->getItem(3))->getItem(1));
			mPlayers = PLAYERS(list->size());
			mTeams = TEAMS((list->size()+1)>>1);

			for(int i = 0; i < mPlayers.size(); i++)
			{
				PlayerInfo playerInfo;

				playerInfo.uid = list->getItem(i)->getString();
				playerInfo.index = i;
				playerInfo.teamIndex = (i+1)/2;
				playerInfo.indexInTeam = (i+1)%2;

				mPlayers[i] = playerInfo;
				mTeams[playerInfo.teamIndex].nowPlayer = 0;
				mTeams[playerInfo.teamIndex].players.push_back(i);

				if(mConfig.getAsString("uid")==playerInfo.uid)
				{
					myPlayerID = playerInfo.index;
					myTeamID = playerInfo.teamIndex;
					myPlayerPtr = &mPlayers[myPlayerID];
					myTeamPtr = &mTeams[myTeamID];
				}
			}
		}
	};

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
	class PairPlay : public MultiPlay
	{
		//note Blender，把不属于自己的note变淡
		class PairPlay_NoteBlender : public Hook
		{
		public:
			std::string getName() {return "PVMode";}

			PairPlay_NoteBlender()
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
		PairPlay_NoteBlender *noteBlender;

		typedef std::vector<PairNote> PAIR_NOTES;
		PAIR_NOTES pairQueue;
	public:
		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			mInfo = new PairGameInfo();
			mInfo->setOwner(this);

			MultiPlay::gameReset();
			GNET_RECEIVE_PACKET("game#failure",&PairPlay::gnetGetFailure);
			
			noteBlender = new PairPlay_NoteBlender;
			HOOK_MANAGER_PTR->insert(noteBlender);

			pairQueue.clear();
		}
		void gameOver()
		{
			MultiPlay::gameOver();
			GNET_UNRECEIVE_PACKET("game#failure");
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
			if(note->getID()%2!=getMyPlayerInfo()->indexInTeam)
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
			NETWORK_SYSTEM_PTR->send("game#failure",
				"%f%d%d%b%b",CORE_PTR->getRunTime(),event.note->getID(),cnt,1,1);
		}

		void gnetGetFailure(GPacket *packet)
		{
			double failedTime;
			uint32 pID, cnt;
			bool breakCombo, breakNote;
	
			NETWORK_SYSTEM_PTR->read(packet,"%f%d%d%b%b",&failedTime,&pID,&cnt,&breakCombo,&breakNote);
			
			PairNote note(pID,cnt,breakCombo,breakNote,failedTime);

			if(failedTime>CORE_PTR->getRunTime())//stateList.size()<=pID||stateList[pID].isOver()&&stateList[pID].eventList.size()<=cnt)
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