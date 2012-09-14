/*
 *  DivaPingpongNote.cpp
 *
 *  Created by Hyf042 on 2/3/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaPingpongNote.h"
#include "Core/DivaCore.h"
#include "Utility/DivaPath.h"

namespace divacore
{
	const float PingpongNote::SCALE = 1.5;

	void PingpongNote::onInitiate() 
	{
		//get info
		standardNum = Argument::asInt("standard_num",noteInfo.arg);
		//twoType[0] = Argument::asInt("ping_type",noteInfo.arg);
		//twoType[1] = Argument::asInt("pong_type",noteInfo.arg);
		twoType[0] = getReceivePoint().type%8;//Argument::asInt("ping_type",noteInfo.arg);
		twoType[1] = DIVA_KEY_SPACE;
		cntHit = 0;
		midNum = (int)(standardNum*config->getAsDouble("pingpong_score_middleTime"));
		limitNum = (int)(standardNum*config->getAsDouble("pingpong_score_limitTime"));

		periodGrid = noteInfo.aheadBar*GRID_PER_BAR;
		totalGrid = periodGrid+(noteInfo.notePoint[1].position-noteInfo.notePoint[0].position);
		barRate = (totalGrid-periodGrid)/double(periodGrid);
		bPressStart = false;
		bPressOver = false;

		//create sprites
		noteSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("pingpong_note_tex"));
		rhythmSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("pingpong_rhythm_tex"));
		coverSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("pingpong_note_tex"));
		barMetaSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("pingpong_bar_tex"));

		//set note
		notePosition = Point(noteInfo.notePoint[0].x*config->getAsDouble("grid_width")+config->getAsDouble("deltaX"),noteInfo.notePoint[0].y*config->getAsDouble("grid_height")+config->getAsDouble("deltaY"));

		Rect texRect = config->getAsRect("pingpong_note_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		Point centerPoint = config->getAsPoint("pingpong_note_noteCenter");
		noteSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		noteSprite->setCenter(centerPoint.x,centerPoint.y);
		noteSprite->setPosition(notePosition.x,notePosition.y);
		noteSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("pingpong_note_blowup"),SCALE,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("pingpong_note_blowTimeRate")));

		//set base
		coverRect = config->getAsRect("pingpong_note_cover_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		coverSprite->setTextureRect(coverRect.x,coverRect.y,coverRect.w,coverRect.h);
		coverSprite->setCenter(centerPoint.x,coverRect.h);

		coverSprite->setPosition(notePosition.x,notePosition.y+(-centerPoint.y+coverRect.h)*SCALE);
		noteCenterPoint = centerPoint;
		coverSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("pingpong_note_blowup"),SCALE,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("pingpong_note_blowTimeRate")));
	
		//set tail
		tailPosition = notePosition+Point(Argument::asFloat("tailx",noteInfo.arg),Argument::asFloat("taily",noteInfo.arg)).unit()*config->getAsDouble("pingpong_rhythm_distance")*MAP_INFO->header.speedScale;
		texRect = config->getAsRect("pingpong_rhythm_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		centerPoint = config->getAsPoint("pingpong_rhythm_center");
		rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		rhythmSprite->setCenter(centerPoint.x,centerPoint.y);
		rhythmSprite->setPosition(tailPosition.x,tailPosition.y);
		rhythmSprite->setScale(SCALE,SCALE);

		//set bar meta
		texRect = config->getAsRect("pingpong_bar_"+iToS(noteInfo.notePoint[0].type%4));
		centerPoint = config->getAsPoint("pingpong_bar_center");
		Point size = config->getAsPoint("pingpong_bar_size");
		barMetaSprite->setScale(size.x/texRect.w*SCALE,size.y/texRect.h*SCALE);
		barMetaSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		barMetaSprite->setCenter(centerPoint.x,centerPoint.y);

		//set state
		setState(HEAD);

		lastTailPosition = tailPosition, length_drawn = 0;
	}
	void PingpongNote::onEnter() {}
	void PingpongNote::onLeave() 
	{
		EFFECT_SYSTEM_PTR->clearParticle(this);

		SAFE_DELETE_SPRITE(noteSprite);
		SAFE_DELETE_SPRITE(rhythmSprite);
		SAFE_DELETE_SPRITE(barMetaSprite);
		SAFE_DELETE_SPRITE(coverSprite);
	}
	void PingpongNote::onPoint(uint32 cnt, NotePoint point) 
	{
		if(cnt==0)
		{
			bPressStart = true;
			periodGrid = noteInfo.notePoint[1].position-noteInfo.notePoint[0].position;
		}
		else
		{
			bPressOver = true;
			finish(config->getAsDouble("pingpong_note_fadeout"));

			finalCalt();

			setState(END);
		}
		
		//auto click
		StateEvent event(this,0);
		if(GAME_MODE_PTR->checkPoint(event))
			if(getState()==HEAD)
				onPressed(event);
	}
	void PingpongNote::finalCalt()
	{
		StateEvent event(this);
		if(cntHit<standardNum)
		{
			event.rank = 4;
			event.breakCombo = true;
		}
		else if(cntHit<midNum)
			event.rank = 3; //safe不断combo
		else if(cntHit<limitNum)
			event.rank = 2;
		else
			event.rank = 1;
		event.position = noteInfo.notePoint[1].position;
		if(GAME_MODE_PTR->checkExtra(event))
		{
			event.type = StateEvent::PRESS;
			GAME_MODE_PTR->inform(event);
		}
	}
	void PingpongNote::finish(float dt)
	{
		noteSprite->addEffect(sora::CreateEffectFade(1.0,0,dt));
		coverSprite->addEffect(sora::CreateEffectFade(1.0,0,dt));
		mEndTime = dt;
	}
	void PingpongNote::_hitEffect()
	{
		noteSprite->addEffect(sora::CreateEffectList(
			sora::CreateEffectScale(SCALE,SCALE*2,0.2),
			sora::CreateEffectScale(SCALE*2,SCALE,0.2),
			sora::ImageEffectOnce));
		coverSprite->addEffect(sora::CreateEffectList(
			sora::CreateEffectScale(SCALE,SCALE*2,0.2),
			sora::CreateEffectScale(SCALE*2,SCALE,0.2),
			sora::ImageEffectOnce));
	}
	void PingpongNote::onRender()
	{
		//render note and arrow
		Core::Ptr->render(noteSprite,"note+"+getTailTag());
		Core::Ptr->render(coverSprite,"note_cover+"+getTailTag());
		
		//render rhythm
		if(!bPressOver)
		{
			path::Bezier::renderBezier(tailPosition,notePosition,rhythmTail,rhythmHead,barMetaSprite);
		}
		if(!bPressStart)
		{
			Point position = path::Bezier::getBezierPoint(tailPosition,notePosition,rhythmHead);

			/*Rect texRect = config->getAsRect("pingpong_rhythm_"+NOTE_MAP[twoType[0]]);
			rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);*/
			rhythmSprite->setPosition(position.x,position.y);
			Core::Ptr->render(rhythmSprite,"note_rhythm+"+getTailTag());

			nowTailPosition = position;
			{
				length_drawn += (nowTailPosition-lastTailPosition).mod();
				while(length_drawn>0){
					float rand_size = (float)rand()/RAND_MAX;
					// Chance time got 2 times the number of star particles
					length_drawn -= rand_size*16; //原作者犯了一个错误，导致Diva HD总是-2，所以这里为了平均使之*4，再加上屏幕的4倍为16倍

					if(RENDER_SYSTEM_PTR->InsideDrawRange(nowTailPosition))
						EFFECT_SYSTEM_PTR->addParticle("star",
						ParticleData(nowTailPosition.x+rand()*(40.0f/RAND_MAX)-20.0f,
						nowTailPosition.y+rand()*(40.0f/RAND_MAX)-20.0f, 0,0, rand_size*10), this);
				}
			}
			lastTailPosition = nowTailPosition;
		}
		if(!bPressOver)
		{
			Point position = path::Bezier::getBezierPoint(tailPosition,notePosition,rhythmTail);

			/*Rect texRect = config->getAsRect("pingpong_rhythm_"+NOTE_MAP[twoType[0]]);
			rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			rhythmSprite->setPosition(position.x-rhythmSprite->getSpriteWidth()/2,position.y);
			Core::Ptr->render(rhythmSprite,"note_rhythm"+getTailTag());

			texRect = config->getAsRect("pingpong_rhythm_"+NOTE_MAP[twoType[1]]);
			rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);*/
			rhythmSprite->setPosition(position.x,position.y);
			Core::Ptr->render(rhythmSprite,"note_rhythm+"+getTailTag());
		}
	}
	void PingpongNote::onUpdate(double dt, double position) 
	{
		//change the bar color
		float value = (position-int(position/100)*100)/100;
		barMetaSprite->setColor(Color::getHSVColor(value,1,1));

		//calculate arrow angle and rhythm position
		double ratio = position/periodGrid, angle = 0;
		if(!bPressStart)
		{
			rhythmHead = ratio;
			rhythmTail = ratio-barRate;
			angle = position/totalGrid;
		}
		else if(!bPressOver)
		{
			rhythmHead = 1;
			rhythmTail = 1-barRate+ratio*barRate;
			angle = 1+(position-periodGrid)/totalGrid;
		}
		if(getState()==END)
		{
			mEndTime -= dt;
			if(mEndTime < 0)
			{
				mEndTime = 0;
				over();
			}
		}

		Rect tmpRect = coverRect;
		tmpRect.y += tmpRect.h, tmpRect.h = 0;
		tmpRect = Rect::get(tmpRect,coverRect,ratio);
		coverSprite->setTextureRect(tmpRect.x,tmpRect.y,tmpRect.w,tmpRect.h);
		coverSprite->setCenter(coverSprite->getCenterX(),tmpRect.h);
		coverSprite->setPosition(notePosition.x,notePosition.y+(-noteCenterPoint.y+coverRect.h)*coverSprite->getHScale());

		//the first point fail check
		if(getState()==HEAD&&noteInfo.notePoint[0].time+EVALUATE_STRATEGY_PTR->getProtectedTime()<CORE_PTR->getRunTime())
		{
			StateEvent event(this,0);
			if(GAME_MODE_PTR->checkFailure(event))
			{
				GAME_MODE_PTR->inform(event);
				finish(EVALUATE_STRATEGY_PTR->getProtectedTime());
				setState(END);
			}
			else
				setState(TAIL);
		}

		//auto click
		if(getState()==TAIL)
		{
			static float timeDelta = config->getAsDouble("pingpong_auto_delta");
			timeDelta -= dt;

			if(timeDelta<0)
			{
				StateEvent event(this,0);
				if(GAME_MODE_PTR->checkPoint(event))
					onPressed(event);

				timeDelta += config->getAsDouble("pingpong_auto_delta");
			}
		}

		//update sprite
		noteSprite->update(dt);
		rhythmSprite->update(dt);
		coverSprite->update(dt);
	}
	void PingpongNote::onPressed(StateEvent& event)
	{
		if(getState()==HEAD)
		{
			//first note point
			event.position = noteInfo.notePoint[0].position;
			if(GAME_MODE_PTR->checkPress(event))
			{
				cntHit++;
				//Rect texRect = config->getAsRect("pingpong_note_"+NOTE_MAP[twoType[cntHit%2]]);
				//noteSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
				//if(noteInfo.notePoint[0].key!="")
				//	Core::Ptr->getMusicManager()->playDirect(noteInfo.notePoint[0].key,"sound_effect");
				//Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");

				Core::Ptr->getGameMode()->inform(event);
				if(event.breakNote)
				{
					finish(EVALUATE_STRATEGY_PTR->getProtectedTime());
					setState(END);
				}
				else
					setState(TAIL);
			}
		}
		else
		{
			//the pingpong hit
			//if(noteInfo.notePoint[0].key!="")
			//	Core::Ptr->getMusicManager()->playDirect(noteInfo.notePoint[0].key,"sound_effect");
			
			// here playing the key sound should be located in GAME_MODE, fix delayed
			Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");

			cntHit++;

			_hitEffect();
			
			//Rect texRect = config->getAsRect("pingpong_note_"+NOTE_MAP[twoType[cntHit%2]]);
			//noteSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);

			if(cntHit>standardNum)
			{
				StateEvent event(this);
				if(cntHit<midNum)
					event.dtScore = config->getAsInt("pingpong_score_low");
				else if(cntHit<=limitNum)
					event.dtScore = config->getAsInt("pingpong_score_high");
				if(GAME_MODE_PTR->checkExtra(event))
					GAME_MODE_PTR->inform(event);
			}
		}
	}
	void PingpongNote::onReleased(StateEvent& event)
	{
	}
	bool PingpongNote::isReceive(KeyEvent& event, float time)
	{
		if(bOver||getState()<0)
			return false;

		if(getState()==HEAD&&noteInfo.notePoint[0].time+EVALUATE_STRATEGY_PTR->getProtectedTime()>time)
			setReceiveNote(0);
		else if(getState()==TAIL&&noteInfo.notePoint[1].time+EVALUATE_STRATEGY_PTR->getProtectedTime()>time)
			setReceiveNote(1);

		//receive key in turns 
		if(twoType[cntHit%2]!=event.key)
			return false;
		return true;
	}
	void PingpongNote::recover(int point, int position, float time)
	{
		bPressOver = false;
		if(point==0)
		{
			bPressStart = false;
			setState(HEAD);
		}
		else
		{
			bPressStart = true, bPressOver = false;
			setState(TAIL);
			periodGrid = noteInfo.notePoint[1].position-noteInfo.notePoint[0].position;
		}

		onUpdate(0,position-this->getKeyPosition());
		lastTailPosition = nowTailPosition;

		//cancel blow up effect
		noteSprite->clearEffects();
		coverSprite->clearEffects();
		noteSprite->setScale(1,1);
		coverSprite->setScale(1,1);
	}
}
