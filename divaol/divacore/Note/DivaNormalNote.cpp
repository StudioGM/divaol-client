/*
 *  DivaNormalNote.cpp
 *
 *  Created by Hyf042 on 1/27/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaNormalNote.h"
#include "Core/DivaCore.h"
#include "Utility/DivaPath.h"

namespace divacore
{

	void NormalNote::onInitiate() 
	{
		//create sprite
		noteSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("normal_note_tex"));
		//arrowSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("normal_note_tex"));
		rhythmSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("normal_rhythm_tex"));
		coverSprite = sora::SoraCore::Ptr->createSprite(config->getAsString("normal_note_tex"));

		//set note
		notePosition = Point(noteInfo.notePoint[0].x*config->getAsDouble("grid_width")+config->getAsDouble("deltaX"),noteInfo.notePoint[0].y*config->getAsDouble("grid_height")+config->getAsDouble("deltaY"));

		Rect texRect = config->getAsRect("normal_note_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		Point centerPoint = config->getAsPoint("normal_note_noteCenter");
		noteSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		noteSprite->setCenter(centerPoint.x,centerPoint.y);
		
		noteSprite->setPosition(notePosition.x,notePosition.y);
		noteSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("normal_note_blowup"),1.0,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("normal_note_blowTimeRate")));
		
		//set base
		coverRect = config->getAsRect("normal_note_cover_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		coverSprite->setTextureRect(coverRect.x,coverRect.y,coverRect.w,coverRect.h);
		coverSprite->setCenter(centerPoint.x,coverRect.h);

		coverSprite->setPosition(notePosition.x,notePosition.y-centerPoint.y+coverRect.h);
		coverSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("normal_note_blowup"),1.0,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("normal_note_blowTimeRate")));

		//set arrow
		/*texRect = config->getAsRect("normal_note_arrow");
		centerPoint = config->getAsPoint("normal_note_arrowCenter");
		arrowSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		arrowSprite->setCenter(centerPoint.x,centerPoint.y);
		arrowSprite->setPosition(notePosition.x,notePosition.y);
		arrowSprite->addEffect(sora::CreateEffectScale(config->getAsDouble("normal_note_blowup"),1.0,(noteInfo.notePoint[0].time-noteInfo.aheadTime)*config->getAsDouble("normal_note_blowTimeRate")));*/

		//set tail
		tailPosition = notePosition+Point(Argument::asFloat("tailx",noteInfo.arg),Argument::asFloat("taily",noteInfo.arg)).unit()*config->getAsDouble("normal_rhythm_distance")*MAP_INFO->header.speedScale;
		texRect = config->getAsRect("normal_rhythm_"+NOTE_MAP[noteInfo.notePoint[0].type%8]);
		centerPoint = config->getAsPoint("normal_rhythm_center");
		rhythmSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		rhythmSprite->setCenter(centerPoint.x,centerPoint.y);
		rhythmSprite->setPosition(tailPosition.x,tailPosition.y);
		
		totalPosition = noteInfo.aheadBar*GRID_PER_BAR;

		setState(0);

		lastTailPosition = tailPosition;
	}
	void NormalNote::onEnter() {}
	void NormalNote::onLeave() 
	{
		EFFECT_SYSTEM_PTR->clearParticle(this);

		SAFE_DELETE_SPRITE(noteSprite);
		//SAFE_DELETE_SPRITE(arrowSprite);
		SAFE_DELETE_SPRITE(coverSprite);
		SAFE_DELETE_SPRITE(rhythmSprite);
	}
	void NormalNote::onPoint(uint32 cnt, NotePoint point) 
	{
		//fade out the sprites
		noteSprite->addEffect(sora::CreateEffectFade(1.0,0,config->getAsDouble("normal_note_fadeout")));
		//arrowSprite->addEffect(sora::CreateEffectFade(1.0,0,config->getAsDouble("normal_note_fadeout")));
		coverSprite->addEffect(sora::CreateEffectFade(1.0,0,config->getAsDouble("normal_note_fadeout")));
		rhythmSprite->addEffect(sora::CreateEffectScale(1.0,0,config->getAsDouble("normal_rhythm_lessen")));
		mEndTime = config->getAsDouble("normal_note_fadeout");
		
		//auto click
		StateEvent event(this,0);
		if(GAME_MODE_PTR->checkPoint(event))
			onPressed(event);
	}
	void NormalNote::onRender()
	{
		//render sprites
		Core::Ptr->render(noteSprite,"note+"+getTailTag());
		//Core::Ptr->render(arrowSprite,         "note_arrow"+getTailTag());
		Core::Ptr->render(coverSprite,"note_cover+"+getTailTag());
		Core::Ptr->render(rhythmSprite,"note_rhythm+"+getTailTag());

		path::Line::renderCometLine(lastTailPosition,nowTailPosition,this);
		lastTailPosition = nowTailPosition;
	}
	void NormalNote::onUpdate(double dt, double position) 
	{
		//calculate the rhythm position and arrow angle
		double ratio = position/totalPosition;

		Point rhythmPosition;
		if(getNextPoint()==END)
			rhythmPosition = path::Bezier::getBezierPoint(notePosition,tailPosition,-ratio);
		else
			rhythmPosition = path::Bezier::getBezierPoint(notePosition,tailPosition,1-ratio);
		rhythmSprite->setPosition(rhythmPosition.x,rhythmPosition.y);
		nowTailPosition = rhythmPosition;

		//if(getNextPoint()!=END)
		//	arrowSprite->setRotation(ratio*PI*2);
		//else
		//	arrowSprite->setRotation(0);
		Rect tmpRect = coverRect;
		tmpRect.y += tmpRect.h, tmpRect.h = 0;
		tmpRect = Rect::get(tmpRect,coverRect,(getNextPoint()==END)?1:ratio);
		coverSprite->setTextureRect(tmpRect.x,tmpRect.y,tmpRect.w,tmpRect.h);
		coverSprite->setCenter(coverSprite->getCenterX(),tmpRect.h);

		//update sprites
		noteSprite->update(dt);
		//arrowSprite->update(dt);
		coverSprite->update(dt);
		rhythmSprite->update(dt);

		//check failure
		if(getState()>=0&&noteInfo.notePoint[0].time+EVALUATE_STRATEGY_PTR->getProtectedTime()<CORE_PTR->getRunTime())
		{
			StateEvent event(this,0);
			if(GAME_MODE_PTR->checkFailure(event))
				GAME_MODE_PTR->inform(event);
			setState(END);
		}

		//check over
		if(getState()==END)
		{
			mEndTime -= dt;
			if(mEndTime<0)
			{
				mEndTime = 0;
				over();
			}
		}
	}
	void NormalNote::onPressed(StateEvent& event)
	{
		event.position = noteInfo.notePoint[0].position;

		if(Core::Ptr->getGameMode()->checkPress(event))
		{
			//if(noteInfo.notePoint[0].key!="")
			//	Core::Ptr->getMusicManager()->playDirect(noteInfo.notePoint[0].key,"sound_effect");
			//Core::Ptr->getMusicManager()->playDirect("hit","sound_effect");

			Core::Ptr->getGameMode()->inform(event);

			over();
			//play sound
		}
	}
	//check whether to receive a key and set receive note point
	bool NormalNote::isReceive(KeyEvent& event, float time)
	{
		if(bOver)
			return false;
		if(noteInfo.notePoint[0].time+EVALUATE_STRATEGY_PTR->getProtectedTime()<time)
			return false;
		setReceiveNote(0);
		if(getReceivePoint().type%8!=event.key)
			return false;
		return true;
	}

	void NormalNote::recover(int point, int position, float time)
	{
		onUpdate(0,position-this->getKeyPosition());
		lastTailPosition = nowTailPosition;

		//cancel blow up effect
		noteSprite->clearEffects();
		coverSprite->clearEffects();
		noteSprite->setScale(1,1);
		coverSprite->setScale(1,1);
	}
}
