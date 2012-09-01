/*
 *  DivaRelayUI.h
 *
 *  Created by Hyf042 on 3/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_RELAY_UI
#define DIVA_RELAY_UI

#include "Component/DivaSimpleUIPainter.h"

namespace divacore
{
	using namespace SimpleUI;
		/*
		 * Relay Prompt
		 */
	class RelayPrompt : public Widget
	{
		Spark *help;
		Image *get;
		NumberBar *number;
		float nowTime,blowupTime,blowupRate,fadeOutTime;
		int relayState;
		bool bShowNumber;
	public:
		enum{NORMAL,HELP,GET,CHANGE,REJECT};
		
		int getRelayState() {return relayState;}
		void showNumber(bool bShowNumber) 
		{
			this->bShowNumber=bShowNumber;
			this->number->setVisible(bShowNumber);
		}
		void setNumber(int n)
		{
			this->number->setNumber(n);
		}
		void setRelayState(int state)
		{
			help->setScale(1);
			get->setScale(1);
			help->setSpark(false), help->setFix(false),get->setVisible(false);

			if(state==HELP)
				help->setSpark(true);
			else if(state==GET)
				get->setVisible(true),nowTime = blowupTime*2+fadeOutTime;
			else if(state==REJECT&&relayState==HELP)
				help->setSpark(true), help->setFix(true), nowTime = fadeOutTime;
			
			relayState = state;
		}

		void construct(Config &config, const std::string &head)
		{
			help = NULL;
			if(config.has(head+"help"))
			{
				help = new Spark();
				help->construct(config,head+"help_");
				help->init();
				add(help);
			}
			get = NULL;
			if(config.has(head+"get"))
			{
				get = new Image();
				get->construct(config,head+"get_");
				get->init();
				add(get);
			}
			number = NULL;
			if(config.has(head+"number"))
			{
				number = new NumberBar();
				number->construct(config,head+"number_");
				number->init();
				add(number);
			}
			blowupTime = config.getAsDouble(head+"blowupTime");
			blowupRate = config.getAsDouble(head+"blowupRate");
			fadeOutTime = config.getAsDouble(head+"fadeOutTime");
			setRelayState(NORMAL);
			showNumber(false);
		}
		void onInitialize()
		{
		}
		void onUpdate(float dt)
		{
			//help->setSpark(true);
			if(getRelayState()==GET)
			{
				float scale = 1;
				int alpha = 255;
				if(nowTime>=blowupTime+fadeOutTime)
					scale = 1+(blowupTime*2+fadeOutTime-nowTime)/blowupTime*(blowupRate-1);
				else if(nowTime>=fadeOutTime)
					scale = 1+(nowTime-fadeOutTime)/blowupTime*(blowupRate-1);
				else
					alpha = 255*nowTime/fadeOutTime;
				get->setColor(CSETA(get->getColor(),alpha));
				get->setScale(scale);
				nowTime -= dt;
				if(nowTime<0)
				{
					nowTime = 0;
					setRelayState(NORMAL);
				}
			}
			else if(getRelayState()==REJECT)
			{
				help->setScale(nowTime/fadeOutTime);
				nowTime -= dt;
				if(nowTime<0)
				{
					nowTime = 0;
					setRelayState(NORMAL);
				}
			}
		}
		void onRender(float x, float y)
		{
		}
	};
}

#endif