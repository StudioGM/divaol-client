/*
 *  DivaLongNote.cpp
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaLongNote.h"
#include "Core/DivaCore.h"
#include "Utility/DivaPath.h"

namespace divacore
{

	void LongNote::onInitiate() 
	{
		//create sprites
		noteSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("long_note_tex"));
		rhythmSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("long_rhythm_tex"));
		coverSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("long_note_tex"));
		barMetaSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("long_bar_tex"));

		//set note
		notePosition = Point(noteInfo.notePoint[0].x*config->getAsDouble("grid_width")+config->getAsDouble("deltaX"),noteInfo.notePoint[0].y*config->getAsDouble("grid_height")+config->getAsDouble("deltaY"));

		Rect texRect = config->getAsRect("long_note_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		Point centerPoint;
		if(config->has("long_note_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center"))
			centerPoint = config->getAsPoint("long_note_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center");
		else
			centerPoint = config->getAsPoint("long_note_noteCenter");

		noteSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		noteSprite->setCenter(centerPoint.x,centerPoint.y);
		noteSprite->setPosition(notePosition.x,notePosition.y);
		noteSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("long_note_blowup"),1.0,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("long_note_blowTimeRate")));

		//set base
		coverRect = config->getAsRect("long_note_cover_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		coverSprite->setTextureRect(coverRect.x,coverRect.y,coverRect.w,coverRect.h);

		if(config->has("long_note_cover_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center"))
			centerPoint = config->getAsPoint("long_note_cover_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center");
		else
			centerPoint = config->getAsPoint("long_note_cover_center");
		coverSprite->setCenter(centerPoint.x,coverRect.h);

		coverSprite->setPosition(notePosition.x,notePosition.y-centerPoint.y+coverRect.h);
		coverSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("long_note_blowup"),1.0,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("long_note_blowTimeRate")));

		//set tail
		tailPosition = notePosition+Point(Argument::asFloat("tailx",noteInfo.arg),Argument::asFloat("taily",noteInfo.arg)).unit()*config->getAsDouble("long_rhythm_distance")*MAP_INFO->header.speedScale;
		texRect = config->getAsRect("long_rhythm_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);

		if(config->has("long_rhythm_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center"))
			centerPoint = config->getAsPoint("long_rhythm_"+NOTE_MAP[noteInfo.notePoint[0].type%8]+"Center");
		else
			centerPoint = config->getAsPoint("long_rhythm_center");

		rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		rhythmSprite->setCenter(centerPoint.x,centerPoint.y);
		rhythmSprite->setPosition(tailPosition.x,tailPosition.y);

		//set bar meta
		texRect = config->getAsRect("long_bar_"+iToS(noteInfo.notePoint[0].type%4));
		centerPoint = config->getAsPoint("long_bar_center");
		Point size = config->getAsPoint("long_bar_size");
		barMetaSprite->setScale(size.x/texRect.w,size.y/texRect.h);
		barMetaSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		barMetaSprite->setCenter(centerPoint.x,centerPoint.y);

		//get info
		periodGrid = noteInfo.aheadBar*GRID_PER_BAR;
		totalGrid = periodGrid+(noteInfo.notePoint[1].position-noteInfo.notePoint[0].position);
		barRate = (totalGrid-periodGrid)/double(periodGrid);
		bPressStart = false;
		bPressOver = false;

		//set state
		setState(HEAD);

		lastTailPosition = tailPosition, length_drawn = 0;
	}
	void LongNote::onEnter() {}
	void LongNote::onLeave() 
	{
		EFFECT_SYSTEM_PTR->clearParticle(this);

		SAFE_DELETE_SPRITE(noteSprite);
		SAFE_DELETE_SPRITE(rhythmSprite);
		SAFE_DELETE_SPRITE(barMetaSprite);
		SAFE_DELETE_SPRITE(coverSprite);
	}
	void LongNote::onPoint(uint32 cnt, NotePoint point) 
	{
		if(cnt==0)
		{
			bPressStart = true;
			periodGrid = noteInfo.notePoint[1].position-noteInfo.notePoint[0].position;
		}
		else
		{
			bPressOver = true;
			finish(config->getAsDouble("long_note_fadeout"));
		}
		
		//auto click
		StateEvent event(this,0);
		if(GAME_MODE_PTR->checkPoint(event))
			if(getState()==HEAD)
				onPressed(event);
			else if(getState()==TAIL)
				onReleased(event);
	}
	//note finish effect
	void LongNote::finish(float dt)
	{
		noteSprite->addEffect(sora::CreateEffectFade(1.0,0,dt));
		coverSprite->addEffect(sora::CreateEffectFade(1.0,0,dt));
		mEndTime = dt;
	}
	void LongNote::onRender()
	{
		//render note
		Core::Ptr->render(noteSprite,"note+"+getTailTag());
		Core::Ptr->render(coverSprite,"note_cover+"+getTailTag());

		//render bar
		if(!bPressOver)
		{
			path::Bezier::renderBezier(tailPosition,notePosition,rhythmTail,rhythmHead,barMetaSprite);
		}
		if(!bPressStart)
		{
			Point position = path::Bezier::getBezierPoint(tailPosition,notePosition,rhythmHead);
			rhythmSprite->setPosition(position.x,position.y);
			Core::Ptr->render(rhythmSprite,"note_rhythm+"+getTailTag());

			nowTailPosition = position;
			{
				length_drawn += (nowTailPosition-lastTailPosition).mod();
				while(length_drawn>0){
					float rand_size = (float)rand()/RAND_MAX;
					// Chance time got 2 times the number of star particles
					length_drawn -= rand_size*32;
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
			rhythmSprite->setPosition(position.x,position.y);
			Core::Ptr->render(rhythmSprite,"note_rhythm+"+getTailTag());
		}
	}
	void LongNote::onUpdate(double dt, double position) 
	{
		//calculate arrow angle and rhythm ratio
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
			if(mEndTime< 0)
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

		//failed time
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
		else if(getState()==TAIL&&noteInfo.notePoint[1].time+EVALUATE_STRATEGY_PTR->getProtectedTime()<CORE_PTR->getRunTime())
		{
			StateEvent event(this,0);
			if(GAME_MODE_PTR->checkFailure(event))
			{
				setState(FAILED);
				GAME_MODE_PTR->inform(event);
			}
			else
				setState(END);
		}

		//extra addition
		if(getState()==TAIL)
		{
			static float time_delta = config->getAsDouble("long_extra_auto_delta");
			time_delta -= dt;

			if(time_delta<0)
			{
				StateEvent event(this);
				event.dtScore += config->getAsInt("long_extra_single_score");
				if(GAME_MODE_PTR->checkExtra(event))
					GAME_MODE_PTR->inform(event);

				time_delta += config->getAsDouble("long_extra_auto_delta");
			}
		}

		//update
		noteSprite->update(dt);
		rhythmSprite->update(dt);
		coverSprite->update(dt);
	}
	void LongNote::onPressed(StateEvent& event)
	{
		if(getState()==HEAD)
		{
			//check press
			event.position = noteInfo.notePoint[0].position;
			if(GAME_MODE_PTR->checkPress(event))
			{
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
	}
	void LongNote::onReleased(StateEvent& event)
	{
		if(getState()==TAIL)
		{
			event.position = noteInfo.notePoint[1].position;
			if(GAME_MODE_PTR->checkPress(event))
			{
				//if(noteInfo.notePoint[1].key!="")
				//	Core::Ptr->getMusicManager()->playDirect(noteInfo.notePoint[1].key,"sound_effect");
				//Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");

				Core::Ptr->getGameMode()->inform(event);
				setState(END);
				over();
			}
			else
			{
				if(GAME_MODE_PTR->checkFailure(event))
					GAME_MODE_PTR->inform(event);
				setState(END);
			}
		}
	}
	bool LongNote::isReceive(KeyEvent& event, float time)
	{
		if(bOver||getState()<0)
			return false;

		//check received point
		if(getState()==HEAD&&noteInfo.notePoint[0].time+EVALUATE_STRATEGY_PTR->getProtectedTime()>time)
			setReceiveNote(0);
		else if(getState()==TAIL&&noteInfo.notePoint[1].time+EVALUATE_STRATEGY_PTR->getProtectedTime()>time)
			setReceiveNote(1);

		if(getReceivePoint().type%8!=event.key)
			return false;
		return true;
	}

	void LongNote::recover(int point, int position, float time)
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
