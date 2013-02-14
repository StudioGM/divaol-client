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
	class PairNote
	{
	public:
		int noteID,pointCnt,noteRank;
		float time;
		bool breakCombo,breakNote;
		PairNote(int noteID, int pointCnt, int noteRank, bool breakCombo, bool breakNote, float time)
			:noteID(noteID),pointCnt(pointCnt),breakCombo(breakCombo),breakNote(breakNote),time(time),noteRank(noteRank)
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
				if(tag.find("not_mine+")!=std::string::npos)
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
		std::string getNetGameMode() {return "pair";}
		
		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			MultiPlay::gameReset();
			GNET_RECEIVE_PACKET("game#failureL",&PairPlay::gnetGetFailure);
			
			noteBlender = new PairPlay_NoteBlender;
			HOOK_MANAGER_PTR->insert(noteBlender);

			pairQueue.clear();
		}
		void gameOver()
		{
			MultiPlay::gameOver();
			GNET_UNRECEIVE_PACKET("game#failureL");
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
				note->setOwner(false);
				note->setTailTag("not_mine+");
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
			return true;
		}
		void inform(StateEvent& event)
		{
			if(event.note->isOwner())
			{
				MultiPlay::inform(event);

				//only if the note will break note|combo and the rank is worse than 4, we send failure to partner
				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
					if(event.breakCombo||event.breakNote)
						sendFailure(event);
			}
			else
			{
				//play the key sound automatically
				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				{
					if(event.rank<=5 && event.type!=StateEvent::FAILURE) //!HINT temporary change to 5
						Core::Ptr->getMusicManager()->playDirect("hit","se");
					else
						Core::Ptr->getMusicManager()->playDirect("miss","se");
				}

				//show effect
				if(event.type==StateEvent::PRESS)
					pressEffect(event);
			}
		}

		void sendFailure(StateEvent &event)
		{
			int stateCnt = stateList[event.note->getID()].eventList.size()-1;
			//NETWORK_SYSTEM_PTR->send("game#failureR",
				//"%f%d%d%d%b%b",CORE_PTR->getRunTime(),event.note->getID(),stateCnt, event.rank,event.breakCombo,event.breakNote);
			NETWORK_SYSTEM_PTR->send("game#failureR",gnet::TupleBuilder()
				.AddDouble(CORE_PTR->getRunTime())
				.AddInt(event.note->getID())
				.AddInt(stateCnt)
				.AddInt(event.rank)
				.AddInt(event.breakCombo)
				.AddInt(event.breakNote)
				.Get());
		}

		void gnetGetFailure(GPacket *packet)
		{
			double failedTime;
			int32 noteID, stateCnt, noteRank;
			bool breakCombo, breakNote;
	
			NETWORK_SYSTEM_PTR->read(packet,"%f%d%d%d%b%b",&failedTime,&noteID,&stateCnt,&noteRank,&breakCombo,&breakNote);
			
			PairNote note(noteID,stateCnt,noteRank,breakCombo,breakNote,failedTime);
			
			// if the failed event is in the future, push it into queue, or play miss sound and force it to fail
			if(failedTime>CORE_PTR->getRunTime()) {
				pairQueue.push_back(note);
			} 
			else
			{
				if(noteRank>4)
					MUSIC_MANAGER_PTR->playDirect("miss","se");
				if(breakNote)
					CORE_FLOW_PTR->toFail(noteID);
			}
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
					if(pairQueue.begin()->noteRank>4)
						MUSIC_MANAGER_PTR->playDirect("miss","se");
					if(pairQueue.begin()->breakNote)
						CORE_FLOW_PTR->toFail(pairQueue.begin()->noteID);

					pairQueue.erase(pairQueue.begin());
				}
			}
		}
	};
}

#endif