/*
 *  DivaStandardCoreFlow.cpp
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaStandardCoreFlow.h"
//#include "SoraSoundManager/SoraSoundEffectManager.h"
//#include "Plugins/SoraSoundManager/SoraBGMManager.h"

namespace divacore
{
	void StandardCoreFlow::changeBPM(ARGUMENTS &arg)
	{
		nowBPM = Argument::asDouble("value",arg);
	}

	void StandardCoreFlow::registerEvents()
	{
		registerEvent("bpm",&StandardCoreFlow::changeBPM);
	}

	void StandardCoreFlow::gameReset()
	{
		LOGGER->msg("Set","StandardCoreFlow");
		clear();
	}

	void StandardCoreFlow::gameStop()
	{
		destroy();
	}

	void StandardCoreFlow::clear() 
	{
		nowTime = 0;
		totalTime = 0;
		totalGrid = 0;
		nowPosition = 0;
		lastStamp = SCF_TimeStamp();
		timeQueue = TIME_QUEUE();
		notesPtr = NULL, eventsPtr = NULL;
		noteList.clear();
		timeCounter.reset();
		mainSound = "";
		mIsFinish = true;
		mIsSongOver = false;

		state = INIT;
	}
	void StandardCoreFlow::ready()
	{
		clear();

		this->totalTime = core->getMapInfo()->totalTime;
		this->totalGrid = core->getMapInfo()->totalGrid;
		nowBPM = core->getMapInfo()->header.BPM;

		notesPtr = &core->getMapInfo()->notes;
		eventsPtr = &core->getMapInfo()->events;
		for(int i = 0; i < notesPtr->size(); i++)
		{
			MapNote &note = (*notesPtr)[i];
			timeQueue.push(SCF_TimeStamp(note.aheadTime,note.notePoint[0].position-note.aheadBar*GRID_PER_BAR,SCF_TimeStamp::NOTE_START,i));
		}
		for(int i = 0; i < eventsPtr->size(); i++)
		{
			MapEvent &event = (*eventsPtr)[i];
			timeQueue.push(SCF_TimeStamp(event.time,event.position,SCF_TimeStamp::EVENT,i));
		}
		timeQueue.push(SCF_TimeStamp(totalTime,totalGrid,SCF_TimeStamp::SYSTEM,0));
		
		//get main sound
		mainSound = MAP_INFO->header.mainSound;

		state = READY;

		mIsFinish = false;
	}
	void StandardCoreFlow::over()
	{
		EVALUATE_STRATEGY_PTR->finalEvaluate();
		core->getMusicManager()->destroy();
		state = END;
	}
	void StandardCoreFlow::destroy()
	{
		LOGGER->msg("Game Finish","CoreFlow");
		for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
		{
			(*ptr)->onLeave();
			delete (*ptr);
		}
		noteList.clear();
	}
	void StandardCoreFlow::start() 
	{
		if(state==READY)
			state = RUN;
		else
			DIVA_EXCEPTION_MODULE("not ready yet!","StandardCoreFlow");

		LOGGER->msg("Game Start","CoreFlow");

		CORE_PTR->onStart();

		/*MUSIC_MANAGER_PTR->play(mainSound,MAIN_SOUND_CHANNEL);*/
		MUSIC_MANAGER_PTR->resume(MAIN_SOUND_CHANNEL);

		timeCounter.start();

		//beginTask
		beginTask.start();

		MUSIC_MANAGER_PTR->setPosition(MAIN_SOUND_CHANNEL,0);
	}
	void StandardCoreFlow::update(float dt)
	{
		if(state==RUN)
		{
			nowTime = MUSIC_MANAGER_PTR->getPosition(MAIN_SOUND_CHANNEL);/*timeCounter.getTime()*/;
			bool actived = MUSIC_MANAGER_PTR->isPlaying(MAIN_SOUND_CHANNEL);

			if(!actived/*nowTime>=totalTime*//*||!GAME_MODE_PTR->getAlive()*/)
			{
				// flow back 0.5s to have a buffer, otherwise it will cause a thread confliction so that the music will back to 0
				//MUSIC_MANAGER_PTR->setPosition(MAIN_SOUND_CHANNEL,totalTime-5);
				//float tmp = MUSIC_MANAGER_PTR->getPosition(MAIN_SOUND_CHANNEL);
				//endTask

				mIsFinish = true;
				mIsSongOver = nowTime>=totalTime;

				endTask.start();

				if(state!=PAUSE)
				{
					EVALUATE_STRATEGY_PTR->finalEvaluate();
					core->getMusicManager()->destroy();

					state = END;
				}
				
				return;
			}

			while(!timeQueue.empty()&&nowTime>=timeQueue.top().time)
			{
				lastStamp = timeQueue.top();
				SCF_TimeStamp &t = lastStamp;
				timeQueue.pop();

				if(t.type==SCF_TimeStamp::NOTE_START)
				{
					//info
					MapNote &noteInfo = (*notesPtr)[t.uid];
					char msg[255];
					sprintf(msg,"note_start time:%0.6lf uid:%d",nowTime,t.uid);
					sora::log_mssg(msg);
					
					//create note
					NotePtr note = core->getItemFactory()->createNote(noteInfo);
					note->setKeyPosition(t.position);
					note->setID(t.uid);
					note->setNextPoint(0);
					note->onEnter();
					if(GAME_MODE_PTR->checkNote(note))
					{
						noteInfo.pointer = (void*)note;
						noteList.push_back(note);
					}
					else
					{
						GAME_MODE_PTR->checkNoteOver(note);
						SAFE_DELETE(note);
					}

					//add note points
					for(int i = 0; i < noteInfo.notePoint.size(); i++)
						timeQueue.push(SCF_TimeStamp(noteInfo.notePoint[i].time,noteInfo.notePoint[i].position,SCF_TimeStamp::NOTE_POINT,t.uid,i));
				}
				else if(t.type==SCF_TimeStamp::NOTE_POINT)
				{
					//info
					char msg[255];
					sprintf(msg,"note_point time:%0.6lf uid1:%d uid2:%d",nowTime,t.uid,t.uid2);
					sora::log_mssg(msg);

					//inform note
					MapNote &noteInfo = (*notesPtr)[t.uid];
					NotePtr note = NotePtr(noteInfo.pointer);
					if(note)
					{
						note->setKeyPosition(t.position);
						note->onPoint(t.uid2,noteInfo.notePoint[t.uid2]);
						note->setNextPoint(t.uid2+1);
					}
				}
				else if(t.type==SCF_TimeStamp::EVENT)
				{
					//这里把时钟暂停以降低延迟，也就是忽略事件的耗时，原因是播放bgm一类的事件需要花不少时间
					timeCounter.pause();
					call((*eventsPtr)[t.uid]);
					timeCounter.resume();
				}
			}
			if(!timeQueue.empty())
			{
				SCF_TimeStamp &t = const_cast<SCF_TimeStamp&>(timeQueue.top());
				nowPosition = lastStamp.position;
				if(lastStamp.position<t.position)
					nowPosition += (nowTime-lastStamp.time)/(t.time-lastStamp.time)*(t.position-lastStamp.position);
			}
			for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
				if(!(*ptr)->isOver())
					(*ptr)->onUpdate(dt,nowPosition-(*ptr)->getKeyPosition());

			for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
				if((*ptr)->isOver())
				{
					GAME_MODE_PTR->checkNoteOver(*ptr);
					(*ptr)->onLeave();
					(*notesPtr)[(*ptr)->getID()].pointer = NULL;
					SAFE_DELETE((*ptr));
					noteList.erase(ptr);
					break;
				}
		}
	}
	void StandardCoreFlow::render() 
	{
		for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
			(*ptr)->onRender();
	}
	void StandardCoreFlow::pause() 
	{
		if(state==RUN)
		{
			state = PAUSE;
			timeCounter.pause();
			MUSIC_MANAGER_PTR->pause();
		}
	}
	void StandardCoreFlow::resume()
	{
		if(state==PAUSE&&!mIsFinish)
		{
			state = RUN;
			timeCounter.resume();
			MUSIC_MANAGER_PTR->resume();
		}
	}
	bool StandardCoreFlow::toFail(uint32 uid)
	{
		if(noteList.empty())
			return false;
		NotePtr note = (NotePtr)((*notesPtr)[uid].pointer);
		if(note==NULL)
			return false;
		else
		{
			note->toFail();
			return true;
		}
	}
	double StandardCoreFlow::getRunTime() {return nowTime; }
	double StandardCoreFlow::getRealTime() {return MUSIC_MANAGER_PTR->getPosition(MAIN_SOUND_CHANNEL);/*timeCounter.getTime();*/ }
	double StandardCoreFlow::getTotalTime() {return totalTime;}

	void StandardCoreFlow::onKeyPressed(KeyEvent& event)
	{
		int minPosition = totalGrid;
		NotePtr note = NULL;
		double nowTime = getRealTime();
		for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
			if((*ptr)->isOwner()&&(*ptr)->isReceive(event,nowTime))
			{
				const NotePoint &notePoint = (*ptr)->getReceivePoint();
				if(notePoint.position<minPosition)
				{
					note = *ptr;
					minPosition = notePoint.position;
				}
			}
		if(note)
		{
			StateEvent _event(note,abs(nowTime-note->getReceivePoint().time));
			_event.key = event.key;
			note->onPressed(_event);
		}
	}
	void StandardCoreFlow::onKeyReleased(KeyEvent& event)
	{
		int minPosition = totalGrid;
		NotePtr note = NULL;
		double nowTime = getRealTime();
		for(NOTELIST::iterator ptr = noteList.begin(); ptr != noteList.end(); ptr++)
			if((*ptr)->isOwner()&&(*ptr)->isReceive(event,nowTime))
			{
				const NotePoint &notePoint = (*ptr)->getReceivePoint();
				if(notePoint.position<minPosition)
				{
					note = *ptr;
					minPosition = notePoint.position;
				}
			}
		if(note)
		{
			StateEvent _event(note,abs(nowTime-note->getReceivePoint().time));
			_event.key = event.key;
			note->onReleased(_event);
		}
	}


	//-------------------------------------------------
	//edit functions
	//-------------------------------------------------

	/*void StandardCoreFlow::refresh()
	{
	if(CORE_PTR->getMode()!="editMode")
	return;

	_locate(nowPosition,timeCounter.getTime());
	}

	void StandardCoreFlow::setPosition(int position)
	{
	if(CORE_PTR->getMode()!="editMode")
	return;

	_locate(position,_posToTime(position));
	}

	void StandardCoreFlow::setTime(double time)
	{
	if(CORE_PTR->getMode()!="editMode")
	return;

	_locate(time,_timeToPos(time));
	}*/

	void StandardCoreFlow::_locate(double position, double time)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;
		if(time>totalTime) 
			time = totalTime;
		if(position>totalGrid) 
			position = totalGrid;

		//because it will cause some strange bug when relocate at final point, so refuse it
		double _totalTime = MUSIC_MANAGER_PTR->getLength(MAIN_SOUND_CHANNEL);
		if(_totalTime!=totalTime)
		{
			MUSIC_MANAGER_PTR->reload(MAP_INFO->header.mainSound);
			MUSIC_MANAGER_PTR->play(MAP_INFO->header.mainSound,CORE_FLOW_PTR->MAIN_SOUND_CHANNEL);
		}
		_totalTime = MUSIC_MANAGER_PTR->getLength(MAIN_SOUND_CHANNEL);

		if(time>=_totalTime)
			if(mIsFinish)
				return;
			else
			{
				mIsFinish = true;
				CORE_PTR->pause();
			}
		else
			mIsFinish = false;

		timeQueue = TIME_QUEUE();

		nowPosition = position;
		timeCounter.reset();
		timeCounter.set(time);
		nowTime = time;

		_reloadNotes();
		_reloadEvents();

		//final point
		timeQueue.push(SCF_TimeStamp(totalTime,totalGrid,SCF_TimeStamp::SYSTEM,0));
		
		//由于音频多线程缓冲，所以setPosition之后的pause会出现时间混乱的情况，故缓冲一段时间，实属下策
		//static const float bufferTime = 0.01;
		//msleep(bufferTime*1000);
		MUSIC_MANAGER_PTR->setPosition(MAIN_SOUND_CHANNEL,time);
		float tmp = MUSIC_MANAGER_PTR->getPosition(MAIN_SOUND_CHANNEL);
		//LOGGER->log("%.3f",CORE_PTR->getRealTime());
		//msleep(bufferTime*1000);

		if(state==PAUSE)
		{
			//CORE_PTR->resume();
			//MUSIC_MANAGER_PTR->update();
			state = RUN;
			CORE_PTR->pause();
			state = PAUSE;
			//LOGGER->log("%.3f",CORE_PTR->getRealTime());
		}

		else
		{
			//MUSIC_MANAGER_PTR->resume();
			timeCounter.resume();
		}
	}

	void StandardCoreFlow::_reloadNotes()
	{
		float time = nowTime;

		EFFECT_SYSTEM_PTR->clear();
		noteList.clear();

		for(int i = 0; i < notesPtr->size(); i++)
		{
			MapNote &note = (*notesPtr)[i];

			double start_time = note.aheadTime;
			double end_time = note.notePoint.rbegin()->time;

			if(start_time<=time)
			{
				for(int index = 0; index < note.notePoint.size(); index++)
					if(note.notePoint[index].time>=time)
					{
						NotePtr _note = core->getItemFactory()->createNote(note);
						if(index==0)
							_note->setKeyPosition(note.notePoint[0].position-note.aheadBar*GRID_PER_BAR);
						else
							_note->setKeyPosition(note.notePoint[index-1].position);
						_note->setID(i);
						_note->setNextPoint(index);
						_note->recover(index,nowPosition,time);
						_note->onEnter();

						note.pointer = (void*)_note;
						noteList.push_back(_note);
						//add note points
						for(int j = index; j < note.notePoint.size(); j++)
							timeQueue.push(SCF_TimeStamp(note.notePoint[j].time,note.notePoint[j].position,SCF_TimeStamp::NOTE_POINT,i,j));
						break;
					}
			}
			else
				timeQueue.push(SCF_TimeStamp(note.aheadTime,note.notePoint[0].position-note.aheadBar*GRID_PER_BAR,SCF_TimeStamp::NOTE_START,i));
		}
		timeQueue.push(SCF_TimeStamp(totalTime,totalGrid,SCF_TimeStamp::EVENT,0));
	}
	
	void StandardCoreFlow::_reloadEvents()
	{
		float time = nowTime;

		DISPLAY_PTR->clearImage();
		DISPLAY_PTR->clearVideo();
		MUSIC_MANAGER_PTR->stop();

		lastStamp = SCF_TimeStamp(0,0,0,0);
		for(int i = 0; i < eventsPtr->size(); i++)
		{
			MapEvent &event = (*eventsPtr)[i];

			if(event.time<=time)
			{
				event.arg["__location__"] = time-event.time;
				call(event);

				if(event.time>lastStamp.time)
					lastStamp = SCF_TimeStamp(event.time,event.position,SCF_TimeStamp::EVENT,i);
			}
			else
			{
				ARGUMENTS::iterator locPtr = event.arg.find("__location__");
				if(locPtr!=event.arg.end())
					event.arg.erase(locPtr);
				timeQueue.push(SCF_TimeStamp(event.time,event.position,SCF_TimeStamp::EVENT,i));
			}
		}

		MUSIC_MANAGER_PTR->play(mainSound,MAIN_SOUND_CHANNEL);
	}

    double StandardCoreFlow::_posToTime(double position)
	{
		SCF_TimeStamp left = SCF_TimeStamp(0,0,0,0),right = SCF_TimeStamp(totalTime,totalGrid,0,0);

		for(int i = 0; i < eventsPtr->size(); i++)
		{
			MapEvent &event = (*eventsPtr)[i];

			if(event.position<=position&&event.position>left.position)
				left = SCF_TimeStamp(event.time,event.position,0,0);
			else if(event.position>position&&event.position<right.position)
				right = SCF_TimeStamp(event.time,event.position,0,0);
		}

		if(right.position==left.position)
			return left.time;
		else
			return left.time+(right.time-left.time)*(position-left.position)/(right.position-left.position);
	}
	double StandardCoreFlow::_timeToPos(double time)
	{
		SCF_TimeStamp left = SCF_TimeStamp(0,0,0,0),right = SCF_TimeStamp(totalTime,totalGrid,0,0);

		for(int i = 0; i < eventsPtr->size(); i++)
		{
			MapEvent &event = (*eventsPtr)[i];

			if(event.time<=time&&event.time>left.time)
				left = SCF_TimeStamp(event.time,event.position,0,0);
			else if(event.time>time&&event.time<right.time)
				right = SCF_TimeStamp(event.time,event.position,0,0);
		}

		if(right.position==left.position)
			return left.position;
		else
			return left.position+(right.position-left.position)*(time-left.time)/(right.time-left.time);
	}


	//EditUtility
	void StandardEditUtility::init()
	{
		coreFlow = dynamic_cast<StandardCoreFlow*>(CORE_FLOW_PTR);
	}

	void StandardEditUtility::refreshAll()
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		double caltTime = coreFlow->_posToTime(coreFlow->nowPosition);
		
		if(abs(caltTime-coreFlow->timeCounter.getTime())<EPS)
			caltTime = coreFlow->timeCounter.getTime();
		coreFlow->_locate(coreFlow->nowPosition,caltTime);
	}

	void StandardEditUtility::setPosition(double position)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		coreFlow->_locate(position,coreFlow->_posToTime(position));
	}

	void StandardEditUtility::setTime(double time)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		coreFlow->_locate(time,coreFlow->_timeToPos(time));
	}

	void StandardEditUtility::loadResource(MapResourceInfo info)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		MAP_PARSER_PTR->loadResource(info);
	}

	void StandardEditUtility::unloadResource(MapResourceInfo info)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		MAP_PARSER_PTR->unloadResource(info);
	}

	void StandardEditUtility::addEvent(MapEvent event)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		//calt time
		event.time = posToTime(event.position);
		
		MAP_INFO->events.push_back(event);
		refreshAll();
	}
	void StandardEditUtility::delEvent(int index)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		if(index<0||index>=MAP_INFO->events.size())
			DIVA_EXCEPTION_MESSAGE("index out of range!");

		MAP_INFO->events.erase(MAP_INFO->events.begin()+index);
		refreshAll();
	}
	void StandardEditUtility::insert(MapNote note, size_t index, bool isRefresh)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;
		
		if(index<0||index>=MAP_INFO->notes.size())
			DIVA_EXCEPTION_MESSAGE("index out of range!");

		//calt time
		resetNote(note);

		MAP_INFO->notes.insert(MAP_INFO->notes.begin()+index,note);
		
		if(isRefresh)
			refreshAll();
	}
	void StandardEditUtility::append(MapNote note, bool isRefresh)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		resetNote(note);

		MAP_INFO->notes.push_back(note);
		
		if(isRefresh)
			refreshAll();
	}
	void StandardEditUtility::delNote(int index)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		if(index<0||index>=MAP_INFO->notes.size())
			DIVA_EXCEPTION_MESSAGE("index out of range!");

		MAP_INFO->notes.erase(MAP_INFO->notes.begin()+index);
		refreshAll();
	}
	void StandardEditUtility::reCaltTime()
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		MAP_PARSER_PTR->reParser(MapParser::PARSE_TIME);

		// set new totalGrid
		coreFlow->totalTime = MAP_INFO->totalTime;
		coreFlow->totalGrid = MAP_INFO->totalGrid;
		
		refreshAll();
	}
	void StandardEditUtility::resetNote(MapNote &note)
	{
		if(CORE_PTR->getMode()!="editMode")
			return;

		//calt time
		uint32 pos = 0;
		if(note.notePoint[0].position>=note.aheadBar*GRID_PER_BAR)
			pos = note.notePoint[0].position-note.aheadBar*GRID_PER_BAR;

		note.aheadTime = posToTime(pos);
		for(int i = 0; i < note.notePoint.size(); i++)
			note.notePoint[i].time = posToTime(note.notePoint[i].position);
	}
}
