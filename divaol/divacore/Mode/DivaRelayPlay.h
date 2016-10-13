/*
 *  DivaRelayPlay.h
 *
 *  Created by Hyf042 on 8/29/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_RELAY_PLAY_H
#define DIVA_RELAY_PLAY_H

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

#if defined(DIVA_USE_GNET)
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
		static const int BUFFER_POSITION = 192*4;
		static const int MIN_TIME_PERIOD = 192*4;
		static const int MAX_TIME_PERIOD = 192*30;
		int maxLastGrid, lastPlayGrid, restGrid;

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
		std::string getNetGameMode() {return "relay";}

		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			MultiPlay::gameReset();

			GNET_RECEIVE_PACKET("game#relayChanceL",&RelayPlay::gnetRelayChance);
			GNET_RECEIVE_PACKET("game#relayYouCanPlayL",&RelayPlay::gnetRelayYouCanPlay);
			GNET_RECEIVE_PACKET("game#relayNewPlayerL",&RelayPlay::gnetRelayNewPlayer);
			GNET_RECEIVE_PACKET("game#renewL",&RelayPlay::gnetRelayRenew);
			GNET_RECEIVE_PACKET("game#relayVoidPeriodL",&RelayPlay::gnetRelayVoidPeriod);

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
			if(getMyPlayerInfo()->indexInTeam==0)
			{
				lastPlayGrid = 0;
				setRelayState(MYTURN);
			}
			else
				setRelayState(WAIT);

			relayNewTurn();
		}

		void gameOver()
		{
			if (getRelayState() == MYTURN)
				relayWantToChange();

			MultiPlay::gameOver();
			setRelayState(OVER);
			GNET_UNRECEIVE_PACKET("game#relayChanceL");
			GNET_UNRECEIVE_PACKET("game#relayYouCanPlayL");
			GNET_UNRECEIVE_PACKET("game#relayNewPlayerL");
			GNET_UNRECEIVE_PACKET("game#renewL");
			GNET_UNRECEIVE_PACKET("game#relayVoidPeriodL");
		}

		void gameReady()
		{
			restGrid = maxLastGrid = (int)ceil(MAP_INFO->totalGrid/*header.barNum*GRID_PER_BAR*//getMyTeamInfo()->players.size()*1.2);
			endPosition = restGrid;

			registerUI();

			nowPlayer = nextPlayer = 0;
		}

		void destroy()
		{
			if (getRelayState() == MYTURN)
				relayWantToChange();
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
		}
		bool checkNote(NotePtr note) 
		{
			if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
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
				MultiPlay::inform(event);
		
				relayInfo[nowTurn].push_back(RelayNote(event,event.note->getID(),stateList[event.note->getID()].size()-1,
					nowTurn,getScore(),getCombo(),CORE_PTR->getRunTime()));

				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				{
					//if(event.breakCombo||event.breakNote)
					//	sendFailure(event);
					sendRenew(event.note->getID(),event.rank,event.breakCombo,event.breakNote,event.getScreenPosition().x,event.getScreenPosition().y);
				}
			}
			else
			{
				//播放hit音
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

				stateList[event.note->getID()].addKey(event);
				stateQueue.push_back(event);
			}
		}

		// force all the players' combo to my combo, to avoid conflict in display
		void afterUpdateInfo()
		{
			for(int i = 0; i < mInfo->mPlayers.size(); i++)
				if(i!=mInfo->myPlayerID && mInfo->mPlayers[i].teamIndex!=mInfo->myTeamID)
				{
					mInfo->mPlayers[i].combo = mInfo->myPlayerPtr->combo;
				}
		}

		void sendRenew(uint32 uid, int rank, bool breakCombo, bool breakNote, double positionX, double positionY)
		{
			int cnt = stateList[uid].eventList.size()-1;
			NETWORK_SYSTEM_PTR->send("game#renewR",
				"%f%d%d%d%d%b%b%f%f",CORE_PTR->getRunTime(),nowTurn,uid,cnt,rank,breakCombo,breakNote,positionX,positionY);
		}

		void iAmPlaying()
		{
			NETWORK_SYSTEM_PTR->send("game#relayIamPlayingR",
				"%f",CORE_PTR->getRunPosition());
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

		void relayWantToChange()
		{
			NETWORK_SYSTEM_PTR->send("game#relayWantToChangeR",
				"%f",CORE_PTR->getRunPosition()+BUFFER_POSITION);

			changePosition = CORE_PTR->getRunPosition()+BUFFER_POSITION;

			setRelayState(CHANGE);
			changePrompt->setRelayState(RelayPrompt::HELP);
			changePrompt->showNumber(true);
		}
		void relayWantToPlay()
		{
			NETWORK_SYSTEM_PTR->send("game#relayWantToPlayR","");

			setRelayState(APPLY);
		}

		void relayNewTurn()
		{
			relayInfo.push_back(RELAY_NOTES());
		}
		void gnetRelayTimeUP(GPacket *packet)
		{
		}
		void gnetRelayChance(GPacket *packet)
		{
			if(!getAlive() || restGrid <= 0)
				return;

			NETWORK_SYSTEM_PTR->read(packet,"%f",&changePosition);
			setRelayState(RELAY);

			recvPrompt->setRelayState(RelayPrompt::HELP);
		}
		void gnetRelayVoidPeriod(GPacket *packet)
		{
			double begin, end;
			NETWORK_SYSTEM_PTR->read(packet,"%f%f",&begin,&end);
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
		void gnetRelayYouCanPlay(GPacket *packet)
		{
			setRelayState(ACCEPT);
			NETWORK_SYSTEM_PTR->read(packet,"%d",&nowTurn);
			relayNewTurn();
			nowPlayer = getMyPlayerInfo()->indexInTeam;

			recvPrompt->setRelayState(RelayPrompt::GET);

			if(CORE_PTR->getRunPosition()>changePosition)
			{
				endPosition = CORE_PTR->getRunPosition()+restGrid;
				lastPlayGrid = CORE_PTR->getRunPosition();
				changePosition = 0, setRelayState(MYTURN);
				
				iAmPlaying();
			}
		}
		void gnetRelayNewPlayer(GPacket *packet)
		{
			relayNewTurn();

			int tmpTurn;
			NETWORK_SYSTEM_PTR->read(packet,"%d%d",&tmpTurn,&nowPlayer);
			nowPlayer--;

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
		void gnetRelayRenew(GPacket *packet)
		{
			double packetTime;
			uint32 turn;
			uint32 pID;
			uint32 cnt;
			int rank;
			bool breakCombo;
			bool breakNote;
			double positionX;
			double positionY;
			NETWORK_SYSTEM_PTR->read(packet,"%f%d%d%d%d%b%b%f%f",&packetTime,&turn,&pID,&cnt,&rank,&breakCombo,&breakNote,&positionX,&positionY);
			Point position = Point(positionX,positionY);

			StateEvent event;
			event.type = StateEvent::PRESS, event.tag = "inform", event.bOwner = false, event.rank = rank;
			event.breakCombo = breakCombo, event.breakNote = breakNote, event.setScreenPosition(position);
			RelayNote note(event,pID,cnt,turn,0,0,packetTime);

			if(packetTime>CORE_PTR->getRunTime())//stateList.size()<=pID||stateList[pID].isOver()&&stateList[pID].eventList.size()<=cnt)
			{
				relayQueue.push_back(note);
				return;
			}
			
			if(rank>4)
				MUSIC_MANAGER_PTR->playDirect("miss","se");
			if(breakNote)
				CORE_FLOW_PTR->toFail(pID);

			relayRenew(note);

			if(event.rank<0)
				event.rank = event.rank;
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
					restGrid -= CORE_PTR->getRunPosition() - lastPlayGrid;
					if (restGrid < 0)
						restGrid = 0;
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
					lastPlayGrid = CORE_PTR->getRunPosition();
					endPosition = changePosition+restGrid;
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
					relayWantToChange();
				}
			}

			while(!relayQueue.empty()&&relayQueue.begin()->time<=CORE_PTR->getRunTime())
			{
				if(stateList.size()>relayQueue.begin()->noteID)
				{
					KeyState &keyState = stateList[relayQueue.begin()->noteID];

					if(relayQueue.begin()->event.rank>4)
						MUSIC_MANAGER_PTR->playDirect("miss","se");

					if(relayQueue.begin()->event.breakNote&&!keyState.isOver())
					{
						CORE_FLOW_PTR->toFail(keyState.noteID);
					}

					relayRenew(*relayQueue.begin());

					HOOK_MANAGER_PTR->hook(relayQueue.begin()->event);

					relayQueue.erase(relayQueue.begin());
				}
			}
		
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
#else
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
		static const int BUFFER_POSITION = 192*4;
		static const int MIN_TIME_PERIOD = 192*4;
		static const int MAX_TIME_PERIOD = 192*30;
		int maxLastGrid, lastPlayGrid, restGrid;

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
		std::string getNetGameMode() {return "relay";}

		void init()
		{
			MultiPlay::init();
		}
		void gameReset()
		{
			MultiPlay::gameReset();

			//register event
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_CHANGE], Base::Bind(this, &RelayPlay::onRelayChance));
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_NEWPLAYER], Base::Bind(this, &RelayPlay::onRelayNewPlayer));
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_RENEW], Base::Bind(this, &RelayPlay::onRelayRenew));
			POMELO_GAME_PEER->on(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_VOIDPERIOD], Base::Bind(this, &RelayPlay::onRelayVoidPeriod));

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
			if(getMyPlayerInfo()->indexInTeam==0)
			{
				lastPlayGrid = 0;
				setRelayState(MYTURN);
			}
			else
				setRelayState(WAIT);

			relayNewTurn();
		}

		void gameOver()
		{
			if (getRelayState() == MYTURN)
				relayWantToChange();

			MultiPlay::gameOver();
			setRelayState(OVER);
			// unregister
			POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_CHANGE]);
			POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_NEWPLAYER]);
			POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_RENEW]);
			POMELO_GAME_PEER->remove(divapomelo::EventCode[divapomelo::ON_GAME_RELAY_VOIDPERIOD]);
		}

		void gameReady()
		{
			restGrid = maxLastGrid = (int)ceil(MAP_INFO->totalGrid/*header.barNum*GRID_PER_BAR*//getMyTeamInfo()->players.size()*1.2);
			endPosition = restGrid;

			registerUI();

			nowPlayer = nextPlayer = 0;
		}

		void destroy()
		{
			if (getRelayState() == MYTURN)
				relayWantToChange();
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
		}
		bool checkNote(NotePtr note) 
		{
			if(getRelayState()!=MYTURN&&getRelayState()!=CHANGE)
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
				MultiPlay::inform(event);
		
				relayInfo[nowTurn].push_back(RelayNote(event,event.note->getID(),stateList[event.note->getID()].size()-1,
					nowTurn,getScore(),getCombo(),CORE_PTR->getRunTime()));

				if(event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)
				{
					//if(event.breakCombo||event.breakNote)
					//	sendFailure(event);
					sendRenew(event.note->getID(),event.rank,event.breakCombo,event.breakNote,event.getScreenPosition().x,event.getScreenPosition().y);
				}
			}
			else
			{
				//播放hit音
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

				stateList[event.note->getID()].addKey(event);
				stateQueue.push_back(event);
			}
		}

		// force all the players' combo to my combo, to avoid conflict in display
		void afterUpdateInfo()
		{
			for(int i = 0; i < mInfo->mPlayers.size(); i++)
				if(i!=mInfo->myPlayerID && mInfo->mPlayers[i].teamIndex!=mInfo->myTeamID)
				{
					mInfo->mPlayers[i].combo = mInfo->myPlayerPtr->combo;
				}
		}

		void sendRenew(uint32 uid, int rank, bool breakCombo, bool breakNote, double positionX, double positionY)
		{
			int cnt = stateList[uid].eventList.size()-1;
			POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_RENEW],
				Json::Object(
					"time", CORE_PTR->getRunTime(),
					"id", uid,
					"turn", nowTurn,
					"cnt", cnt,
					"rank", rank,
					"breakCombo", breakCombo,
					"breakNote", breakNote,
					"positionX", positionX,
					"positionY", positionY
			));
		}

		void iAmPlaying()
		{
			POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_RELAY_IAMPLAYING],
				Json::Object(
					"position", CORE_PTR->getRunPosition()
			));
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

		void onRelayChance(divapomelo::MessageReq &req) {
			if(!getAlive() || restGrid <= 0)
				return;

			changePosition = req.msg()["position"].asDouble();
			setRelayState(RELAY);

			recvPrompt->setRelayState(RelayPrompt::HELP);
		}
		void onRelayYouCan(int turn) {
			setRelayState(ACCEPT);
			nowTurn = turn;
			relayNewTurn();
			nowPlayer = getMyPlayerInfo()->indexInTeam;

			recvPrompt->setRelayState(RelayPrompt::GET);

			if(CORE_PTR->getRunPosition()>changePosition)
			{
				endPosition = CORE_PTR->getRunPosition()+restGrid;
				lastPlayGrid = CORE_PTR->getRunPosition();
				changePosition = 0, setRelayState(MYTURN);
				
				iAmPlaying();
			}
		}
		void onRelayNewPlayer(divapomelo::MessageReq &req) {
			Json::Value &msg = req.msg();

			relayNewTurn();

			int tmpTurn;
			tmpTurn = msg["turn"].asInt();
			nowPlayer = msg["idInTeam"].asInt();

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
		void onRelayRenew(divapomelo::MessageReq &req) {
			Json::Value &msg = req.msg();

			double packetTime;
			uint32 turn;
			uint32 pID;
			uint32 cnt;
			int rank;
			bool breakCombo;
			bool breakNote;
			double positionX;
			double positionY;

			packetTime = msg["time"].asDouble();
			pID = msg["id"].asInt();
			turn = msg["turn"].asInt();
			pID = msg["id"].asInt();
			cnt = msg["cnt"].asInt();
			rank = msg["rank"].asInt();
			breakCombo = msg["breakCombo"].asBool();
			breakNote = msg["breakNote"].asBool();
			positionX = msg["positionX"].asDouble();
			positionY = msg["positionY"].asDouble();
			
			Point position = Point(positionX,positionY);

			StateEvent event;
			event.type = StateEvent::PRESS, event.tag = "inform", event.bOwner = false, event.rank = rank;
			event.breakCombo = breakCombo, event.breakNote = breakNote, event.setScreenPosition(position);
			RelayNote note(event,pID,cnt,turn,0,0,packetTime);

			if(packetTime>CORE_PTR->getRunTime())//stateList.size()<=pID||stateList[pID].isOver()&&stateList[pID].eventList.size()<=cnt)
			{
				relayQueue.push_back(note);
				return;
			}
			
			if(rank>4)
				MUSIC_MANAGER_PTR->playDirect("miss","se");
			if(breakNote)
				CORE_FLOW_PTR->toFail(pID);

			relayRenew(note);

			if(event.rank<0)
				event.rank = event.rank;
			HOOK_MANAGER_PTR->hook(event);
		}
		void onRelayVoidPeriod(divapomelo::MessageReq &req) {
			Json::Value &msg = req.msg();

			double begin, end;
			begin = msg["from"].asDouble();
			end = msg["to"].asDouble();

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

		void relayWantToChange()
		{
			POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_RELAY_WANTTOCHANGE],
				Json::Object(
					"position", CORE_PTR->getRunPosition()+BUFFER_POSITION
			));

			changePosition = CORE_PTR->getRunPosition()+BUFFER_POSITION;

			setRelayState(CHANGE);
			changePrompt->setRelayState(RelayPrompt::HELP);
			changePrompt->showNumber(true);
		}
		void relayWantToPlay()
		{
			POMELO_GAME_PEER->push(divapomelo::EventCode[divapomelo::PUSH_GAME_RELAY_WANTTOPLAY], Json::Object(),
				[&](divapomelo::RequestReq& req, int status, Json::Value resp) {
					if (status == 0)
						onRelayYouCan(resp["turn"].asInt());
			});

			setRelayState(APPLY);
		}

		void relayNewTurn()
		{
			relayInfo.push_back(RELAY_NOTES());
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
					restGrid -= CORE_PTR->getRunPosition() - lastPlayGrid;
					if (restGrid < 0)
						restGrid = 0;
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
					lastPlayGrid = CORE_PTR->getRunPosition();
					endPosition = changePosition+restGrid;
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
					relayWantToChange();
				}
			}

			while(!relayQueue.empty()&&relayQueue.begin()->time<=CORE_PTR->getRunTime())
			{
				if(stateList.size()>relayQueue.begin()->noteID)
				{
					KeyState &keyState = stateList[relayQueue.begin()->noteID];

					if(relayQueue.begin()->event.rank>4)
						MUSIC_MANAGER_PTR->playDirect("miss","se");

					if(relayQueue.begin()->event.breakNote&&!keyState.isOver())
					{
						CORE_FLOW_PTR->toFail(keyState.noteID);
					}

					relayRenew(*relayQueue.begin());

					HOOK_MANAGER_PTR->hook(relayQueue.begin()->event);

					relayQueue.erase(relayQueue.begin());
				}
			}
		
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
#endif
}

#endif