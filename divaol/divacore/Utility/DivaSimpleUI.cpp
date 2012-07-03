/*
 *  DivaSimpleUI.cpp
 *
 *  Created by Hyf042 on 2/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */
#include "DivaSimpleUI.h"
#include "Utility/DivaConfigLoader.h"
#include "json/json.h"
#include "SoraResourceFile.h"
#include "Core/DivaCore.h"
#include "Hook/DivaCTMode.h"

namespace divacore
{
	namespace SimpleUI
	{
		/*
		 * Widget
		 */
		void Widget::update(float dt) 
		{
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				(*ptr)->update(dt);
			onUpdate(dt);
		}
		void Widget::render(float x, float y) 
		{
			if(getState()!=HIDE&&visible)
				onRender(this->x+x,this->y+y);
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
					(*ptr)->render(this->x+x,this->y+y);
		}
		void Widget::init() 
		{
			onInitialize();
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				(*ptr)->init();
		}
		void Widget::clear()
		{
			onClear();
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
			{
				(*ptr)->clear();
				SAFE_DELETE((*ptr));
			}
			children.clear();
		}
		void Widget::hide()
		{
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				(*ptr)->hide();
			onHide();
		}
		void Widget::start()
		{
			onStart();
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				(*ptr)->start();
		}
		void Widget::display()
		{
			onDisplay();
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				(*ptr)->display();
		}

		void Widget::add(Widget *widget) 
		{
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				if((*ptr)==widget)
					return;
			children.push_back(widget);
		}
		void Widget::del(Widget *widget)
		{
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
				if((*ptr)==widget)
				{
					SAFE_DELETE((*ptr));
					children.erase(ptr);
					break;
				}
		}
		void Widget::mouseClicked(SoraMouseEvent& event) 
		{
			if(!input)
				return;
			onMouseClicked(event);
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
			{
				event.x -= getPositionX(), event.y -= getPositionY();
				(*ptr)->mouseClicked(event);
				event.x += getPositionX(), event.y += getPositionY();
			}
		}
		void Widget::mouseReleased(SoraMouseEvent& event)
		{
			if(!input)
				return;
			onMouseReleased(event);
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
			{
				event.x -= getPositionX(), event.y -= getPositionY();
				(*ptr)->mouseReleased(event);
				event.x += getPositionX(), event.y += getPositionY();
			}
		}
		void Widget::mouseMoved(SoraMouseEvent& event)
		{
			if(!input)
				return;
			onMouseMoved(event);
			for(CHILDREN::iterator ptr = children.begin(); ptr != children.end(); ptr++)
			{
				event.x -= getPositionX(), event.y -= getPositionY();
				(*ptr)->mouseMoved(event);
				event.x += getPositionX(), event.y += getPositionY();
			}
		}
		void Widget::setPosition(float x, float y)
		{
			this->x = x, this->y = y;
		}

		/*
		 * Image
		 */
		void Image::setTextureRect(Rect texRect)
		{
			this->texRect = texRect; 
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			tex->setScale(position.w/texRect.w,position.h/texRect.h);
		}
		void Image::construct(Config &config, const std::string &head)
		{
			tex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			texRect = config.getAsRect(head+"texRect");
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			position = config.getAsRect(head+"position");
			tex->setScale(position.w/texRect.w,position.h/texRect.h);

			std::string way = "normal";
			if(config.has(head+"showStyle"))
				way = config.getAsString(head+"showStyle");
			if(way=="middle")
			{
				tex->setCenter(texRect.w/2,texRect.h/2);
				setPosition(position.x+position.w/2,position.y+position.h/2);
			}
			else
				setPosition(position.x,position.y);
		}
		void Image::onRender(float x, float y)
		{
			tex->render(x+tex->getPositionX(),y+tex->getPositionY());
		}
		void Image::setScale(float scale)
		{
			tex->setScale(scale,scale);
			this->scale = scale;
		}

		void CTLabel::setTextureRect(Rect texRect)
		{
			this->texRect = texRect; 
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			tex->setScale(position.w/texRect.w,position.h/texRect.h);
			tex->setCenter(texRect.w/2,texRect.h/2);
			setPosition(position.x+position.w/2,position.y+position.h/2);
		}
		void CTLabel::construct(Config &config, const std::string &head)
		{
			Image::construct(config,head);
			changeTime = config.getAsDouble(head+"changeTime");
			expandScale = config.getAsDouble(head+"expandScale");

			for(int i = 1; i  < CT_LEVEL; i++)
				levelLabelRect[i] = config.getAsRect(head+iToS(i));

			level = 0;
		}
		void CTLabel::onInitialize()
		{
			setVisible(false);
			ctMode = NULL;
			flush();
		}
		void CTLabel::onStart()
		{
			ctMode = HOOK_MANAGER_PTR->getHook("CTMode");
			if(ctMode)
				flush();
		}
		void CTLabel::onUpdate(float dt)
		{
			Image::onUpdate(dt);
			if(ctMode)
			{
				float scale = 1;
				if(getState()==BLOWUP)
				{
					scale = 1+(changeTime-nowTime)/changeTime*expandScale;
					nowTime -= dt;
					if(nowTime<0)
					{
						nowTime = changeTime, setState(DOWN);
						setTextureRect(levelLabelRect[level]);
					}
				}
				else if(getState()==DOWN)
				{
					scale = 1+nowTime/changeTime*expandScale;
					nowTime -= dt;
					if(nowTime<0)
						nowTime = 0, setState(DISPLAY);
				}
				else if(getState()==DISAPPER)
				{
					scale = 1*nowTime/changeTime;
					nowTime -= dt;
					if(nowTime<0)
						nowTime = 0, setState(DISPLAY), setVisible(false);
				}
				tex->setScale(scale,scale);

				if(level!=((CTMode*)ctMode)->getLevel())
				{
					level = ((CTMode*)ctMode)->getLevel();
					if(level==0)
						nowTime = changeTime, setState(DISAPPER);
					else
					{
						nowTime = changeTime, setState(BLOWUP);
						if(!visible)
							setTextureRect(levelLabelRect[level]);
						setVisible(true);
					}
				}
			}
		}

		void CTLabel::onRender(float x, float y)
		{
			Image::onRender(x,y);
		}

		void SlideBar::construct(Config &config, const std::string &head)
		{
			Image::construct(config,head);

			standardPosition = Point(0,0);
			tex->setScale(float(position.w)/tex->getSpriteWidth(),float(position.h)/tex->getSpriteHeight());

			if(config.has(head+"hideTime"))
				hideTime = config.getAsDouble(head+"hideTime");
			else
				hideTime = 0;
			std::string hideWay;
			if(config.has(head+"hideWay"))
				hideWay = config.getAsString(head+"hideWay");
			else
				hideWay = "top";

			hideTexRect = texRect, hidePosition = standardPosition;
			if(hideWay=="top")
				hideTexRect.y += position.h, hideTexRect.h = 0;
			else if(hideWay=="bottom")
				hideTexRect.h = 0, hidePosition.y += position.h;
			else if(hideWay=="left")
				hideTexRect.x += position.w, hideTexRect.w = 0;
			else if(hideWay=="right")
				hideTexRect.w = 0, hidePosition.x += position.w;
		}

		void SlideBar::onHide()
		{
			if(getState()!=HIDE)
			{
				if(getState()==DISPLAYING)
					nowTime = hideTime-nowTime;
				setState(HIDDING);
				flush();
			}
		}

		void SlideBar::onDisplay()
		{
			if(getState()!=DISPLAY)
			{
				if(getState()==HIDDING)
					nowTime = hideTime-nowTime;
				setState(DISPLAYING);
				flush();
			}
		}

		void SlideBar::onUpdate(float dt)
		{
			if(getState()==HIDDING||getState()==DISPLAYING)
			{
				Rect &sourceRect = (getState()==HIDDING)?texRect:hideTexRect,
					&targetRect = (getState()==HIDDING)?hideTexRect:texRect;
				Point &sourcePoint = (getState()==HIDDING)?standardPosition:hidePosition,
					&targetPoint = (getState()==HIDDING)?hidePosition:standardPosition;

				nowTime += dt;
				if(nowTime>hideTime)
				{
					nowTime = 0;
					tex->setTextureRect(targetRect.x,targetRect.y,targetRect.w,targetRect.h);
					setPosition(position.x+targetPoint.x,position.y+targetPoint.y);
					setState(getState()==HIDDING?HIDE:DISPLAY);
				}
				else
				{
					Rect tmpRect = Rect::get(sourceRect,targetRect,nowTime/hideTime);
					Point tmpPoint = (targetPoint-sourcePoint)*(nowTime/hideTime)+sourcePoint;
					tex->setTextureRect(tmpRect.x,tmpRect.y,tmpRect.w,tmpRect.h);
					setPosition(position.x+tmpPoint.x,position.y+tmpPoint.y);
				}
			}
		}

		void SlideBar::onRender(float x, float y)
		{
			if(getState()==HIDDING)
				setState(HIDDING);
			if(getState()!=HIDE)
				Image::render(x,y);
		}
		/*
		 *StateBar
		 */
		void Spark::setSpark(bool spark) 
		{
			this->spark=spark;
		}
		void Spark::construct(Config &config, const std::string &head)
		{
			SE = "";
			if(config.has(head+"SE"))
				SE = config.getAsString(head+"SE");

			tex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			texRect = config.getAsRect(head+"texRect");
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			position = config.getAsRect(head+"position");
			
			tex->setScale(float(position.w)/tex->getSpriteWidth(),float(position.h)/tex->getSpriteHeight());

			std::string way = "normal";
			if(config.has(head+"showStyle"))
				way = config.getAsString(head+"showStyle");
			if(way=="middle")
			{
				tex->setCenter(texRect.w/2,texRect.h/2);
				setPosition(position.x+position.w/2,position.y+position.h/2);
			}
			else
				setPosition(position.x,position.y);

			lastPeriod = 0;
		}
		void Spark::onUpdate(float dt)
		{
			if(!spark)
				return;
			if(fix)
			{
				tex->setColor(CSETA(tex->getColor(),255));
				return;
			}
			double position = CORE_PTR->getRunPosition()/48;
			int p = (int)floor(position);
			if((p&1)&&p!=lastPeriod)
			{
				if(SE!="")
					MUSIC_MANAGER_PTR->playDirectWithFile(SE,true);
				lastPeriod = p;
			}
			float ratio = position-p;
			int alpha = ((p&1)?(1-ratio):ratio)*255;
			tex->setColor(CSETA(tex->getColor(),alpha));
		}
		void Spark::onRender(float x, float y)
		{
			if(spark)
				tex->render(x+tex->getPositionX(),y+tex->getPositionY());
		}
		void DangerSpark::construct(Config &config, const std::string &head)
		{
			Spark::construct(config,head);
			dangerRatio = config.getAsDouble(head+"dangerRatio");
		}
		void DangerSpark::onUpdate(float dt)
		{
			Spark::onUpdate(dt);
			setFix(!GAME_MODE_PTR->getAlive());
			setSpark((GAME_MODE_PTR->getHPinRatio()<dangerRatio));
		}
		/*
		 * LifeBar
		 */
		void MetaBar::construct(Config &config, const std::string &head)
		{
			metaSprite = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"meta_tex"));
			Rect texRect = config.getAsRect(head+"meta_texRect");
			metaSprite->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			
			position = config.getAsRect(head+"position");
			setPosition(position.x,position.y);

			size = config.getAsPoint(head+"blockArrange");
			hBlock = (int)floor(size.x), vBlock = (int)floor(size.y);
			size = Point(position.w/hBlock,position.h/vBlock);

			blockNum = hBlock*vBlock, showBlock = 0;
		}
		void MetaBar::onInitialize()
		{
			flush();
		}
		void MetaBar::setRatio(float ratio) 
		{
			showBlock = (int)ceil(ratio*blockNum);
		}
		void MetaBar::onUpdate(float dt)
		{
		}
		void MetaBar::onRender(float x, float y)
		{
			for(int i = 0; i < showBlock; i++)
			{
				int dx = i%hBlock*size.x;
				int dy = i/hBlock*size.y;
				metaSprite->render(x+dx,y+dy);
			}
		}
		void LifeMetaBar::onUpdate(float dt)
		{
			setRatio(GAME_MODE_PTR->getHPinRatio());
		}
		void CTMetaBar::onInitialize()
		{
			ctMode = NULL;
			metaSprite->setColor(CARGB(255,0,0,255));
			flush();
		}
		void CTMetaBar::onStart()
		{
			ctMode = HOOK_MANAGER_PTR->getHook("CTMode");
		}
		void CTMetaBar::onUpdate(float dt)
		{
			if(ctMode)
				setRatio(((CTMode*)ctMode)->getCTRatio());
		}
		/*
		 * ScaleBar
		 */
		void ScaleBar::construct(Config &config, const std::string &head)
		{
			tex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			texRect = config.getAsRect(head+"texRect");
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			position = config.getAsRect(head+"position");

			direction = config.getAsString(head+"direction");
			targetPosition = position, targetTexRect = texRect;
			if(direction=="left")
				targetPosition.w = targetTexRect.w = 0;
			else if(direction=="right")
				targetPosition.x += targetPosition.w, targetTexRect.x += targetTexRect.w, targetPosition.w = targetTexRect.w = 0;
			else if(direction=="top")
				targetPosition.h = targetTexRect.h = 0;
			else
				targetPosition.y += targetPosition.h, targetTexRect.y += targetTexRect.h, targetPosition.h = targetTexRect.h = 0;
		
			cursor = NULL;
			if(config.has(head+"cursor_tex"))
			{
				cursor =  sora::SoraCore::Ptr->createSprite(config.getAsString(head+"cursor_tex"));
				Rect rect = config.getAsRect(head+"cursor_rect");
				cursor->setTextureRect(rect.x,rect.y,rect.w,rect.h);
				Point point = config.getAsPoint(head+"cursor_size");
				cursor->setScale(point.x/cursor->getSpriteWidth(),point.y/cursor->getSpriteHeight());
				point = config.getAsPoint(head+"cursor_center");
				cursor->setCenter(point.x,point.y);
			}

			nowRatio = -1;
		}
		void ScaleBar::setTexRect(Rect texRect)
		{
			this->texRect = texRect;
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);

			targetTexRect = texRect;
			if(direction=="left")
				targetPosition.w = targetTexRect.w = 0;
			else if(direction=="right")
				targetPosition.x += targetPosition.w, targetTexRect.x += targetTexRect.w, targetPosition.w = targetTexRect.w = 0;
			else if(direction=="top")
				targetPosition.h = targetTexRect.h = 0;
			else
				targetPosition.y += targetPosition.h, targetTexRect.y += targetTexRect.h, targetPosition.h = targetTexRect.h = 0;

		}
		void ScaleBar::setRatio(float ratio)
		{
			this->ratio = ratio;
			if(nowRatio<0)
				nowRatio = ratio;
		}
		void ScaleBar::onInitialize()
		{
			//setRatio(1);
			flush();
		}
		void ScaleBar::onUpdate(float dt)
		{
			if(nowRatio<0)
				return;
			if(nowRatio<ratio)
			{
				nowRatio += max<float>((ratio-nowRatio)/5,0.001);
				if(nowRatio>ratio) nowRatio = ratio;
			}
			else if(nowRatio>ratio)
			{
				nowRatio -= max<float>((nowRatio-ratio)/5,0.001);
				if(nowRatio<ratio) nowRatio = ratio;
			}
		}
		void ScaleBar::onRender(float x, float y)
		{
			if(nowRatio<0)
				return;
			Rect tmpRect = Rect::get(targetTexRect,texRect,nowRatio);
			tex->setTextureRect(tmpRect.x,tmpRect.y,tmpRect.w,tmpRect.h);
			tmpRect = Rect::get(targetPosition,position,nowRatio);
			tex->setPosition(tmpRect.x,tmpRect.y);
			tex->render(x+tex->getPositionX(),y+tex->getPositionY());
			if(cursor)
				cursor->render(x+tmpRect.x+tmpRect.w,y+tmpRect.y);
		}
		void ScaleBar::setScale(float scale) 
		{
			tex->setScale(scale,scale);
			if(cursor)cursor->setScale(scale,scale);
			targetPosition.x = position.x+(targetPosition.x-position.x)/this->scale*scale;
			targetPosition.y = position.y+(targetPosition.y-position.y)/this->scale*scale;
			this->scale = scale;
		}

		void LifeScaleBar::onInitialize()
		{
			flush();
		}
		void LifeScaleBar::onUpdate(float dt)
		{
			ScaleBar::onUpdate(dt);
			setRatio(GAME_MODE_PTR->getHPinRatio());
		}

		void CTScaleBar::construct(Config &config, const std::string &head)
		{
			ScaleBar::construct(config,head);
		    startRatio = config.getAsDouble(head+"startRatio");
			for(int i = 0; i < CT_LEVEL; i++)
				levelTexRect[i] = config.getAsRect(head+iToS(i));

			//label = new Image();
			//label->construct(config,head+"label_");
			//for(int i = 1; i  < CT_LEVEL; i++)
			//	levelLabelRect[i] = config.getAsRect(head+"label_"+iToS(i));
			//add(label);

			level = 0;
		}
		void CTScaleBar::onInitialize()
		{
			ctMode = NULL;
			flush();
		}
		void CTScaleBar::onStart()
		{
			ctMode = HOOK_MANAGER_PTR->getHook("CTMode");
			if(!ctMode)
				setRatio(0);
			else
				flush();
		}
		void CTScaleBar::onUpdate(float dt)
		{
			ScaleBar::onUpdate(dt);
			if(ctMode)
			{
				setRatio(((CTMode*)ctMode)->getCTRatio());
				if(level!=((CTMode*)ctMode)->getLevel())
				{
					level = ((CTMode*)ctMode)->getLevel();
					setTexRect(levelTexRect[level]);
					/*if(level==0)
						label->setVisible(false);
					else
					{
						label->setVisible(true);
						label->setTextureRect(levelLabelRect[level]);
					}*/
				}
			}
		}
		void CTScaleBar::setRatio(float ratio)
		{
			ScaleBar::setRatio((1-startRatio)*ratio+startRatio);
		}
		/*
		 * NumberBar
		 */
		void NumberBar::setSize(Point size)
		{
			this->size = size;
			numberTex->setScale(size.x/singleSize.x,size.y/singleSize.y);
		}
		void NumberBar::construct(Config &config, const std::string &head)
		{
			numberTex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			texRect = config.getAsRect(head+"texRect");
		
			digitNum = config.getAsInt(head+"digitNum");

			startPoint = config.getAsPoint(head+"startPoint");
			setPosition(startPoint.x,startPoint.y);

			//calculate the single size and scale
			singleSize = Point(texRect.w/10,texRect.h);

			if(config.has(head+"size"))
				size = config.getAsPoint(head+"size");
			else
				size = singleSize;
			if(config.has(head+"gap"))
				gap = config.getAsDouble(head+"gap");
			else
				gap = size.x;

			numberTex->setScale(size.x/singleSize.x,size.y/singleSize.y);

			std::string format = config.getAsString(head+"format");
			for(int i = 0; i < format.size(); i++)
				numHash[format[i]-'0'] = i;

			if(config.has(head+"showStyle"))
			{
				std::string style = config.getAsString(head+"showStyle");
				if(style=="left")
					setShowStyle(LEFT);
				else if(style=="right")
					setShowStyle(RIGHT);
				else if(style=="middle")
					setShowStyle(MIDDLE);
			}
		}
		void NumberBar::onInitialize()
		{
			setNumber(0);
		}
		void NumberBar::setNumber(int n)
		{
			number = iToS(n);
			for(int i = number.size(); i < digitNum; i++)
				number = "0"+number;
		}
		void NumberBar::onUpdate(float dt)
		{
		}
		void NumberBar::onRender(float x, float y)
		{
			for(int i = 0; i < number.size(); i++)
			{
				int rank = numHash[number[i]-'0'];
				numberTex->setTextureRect(texRect.x+singleSize.x*rank,texRect.y,singleSize.x,singleSize.y);
				if(showStyle==LEFT)
					numberTex->render(x+gap*i,y);
				else if(showStyle==RIGHT)
					numberTex->render(x-singleSize.x+gap*(i-(int)number.size()+1),y);
				else if(showStyle==MIDDLE)
					numberTex->render(x+gap*(i-(int)number.size()/2),y);
			}
		}
		void ScoreBar::construct(Config &config, const std::string &head)
		{
			NumberBar::construct(config,head);

			changeTime = config.getAsDouble(head+"changeTime");
		}
		void ScoreBar::onInitialize()
		{
			nowScore = 0;
		}
		void ScoreBar::onUpdate(float dt)
		{
			realScore = GAME_MODE_PTR->getScore();
			setNumber(nowScore);
			if(nowScore<realScore)
			{
				if((realScore-nowScore)<5)
					nowScore = realScore;
				int scoreDt = int((realScore-nowScore)/3);
				nowScore += scoreDt;
				if(nowScore>realScore)
					nowScore = realScore;
			}
		}
		bool ComboPrompt::ComboCatcher::hook(StateEvent &event)
		{
			if(GAME_MODE_PTR->getAlive()&&event.type==StateEvent::PRESS&&event.tag=="inform")
				if(!event.breakCombo)
				{
					Point position = event.getScreenPosition();
					prompt->show(position.x,position.y,GAME_MODE_PTR->getCombo()+event.isOwner());
				}
			return false;
		}
		void ComboPrompt::construct(Config &config, const std::string &head)
		{
			NumberBar::construct(config,head+"numberBar_");

			numberTex->setCenter(singleSize.x/2,singleSize.y/2);

			position = config.getAsRect(head+"position");
			backTex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			Rect texRect = config.getAsRect(head+"texRect");

			backTex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			backTex->setScale(position.w/texRect.w,position.h/texRect.h);
			backTex->setPosition(position.x,position.y);

			lastTime = config.getAsDouble(head+"time_last");
			fadeOutTime = config.getAsDouble(head+"time_fadeOut");
			blowupTime = config.getAsDouble(head+"time_blowup");
			blowupRate = config.getAsDouble(head+"time_blowupRate");
			hundredDelta = config.getAsInt(head+"hundredDelta");

			originGap = gap;
		}
		void ComboPrompt::reset()
		{
			backTex->setColor(CSETA(backTex->getColor(),255*0.6));
			backTex->setPosition(position.x,position.y);
			numberTex->setColor(CSETA(numberTex->getColor(),255*0.6));
			numberTex->setScale(size.x/singleSize.x,size.y/singleSize.y);
			gap = originGap;
		}
		void ComboPrompt::show(float x, float y, int combo)
		{
			if(combo<=4)
				return;
			setPosition(x,y);
			setState(BLOWUP);
			setNumber(combo);
			reset();
			if(combo>=100)
				backTex->setPosition(backTex->getPositionX()+hundredDelta,backTex->getPositionY());
			nowTime = lastTime;
		}
		void ComboPrompt::onInitialize()
		{
			catcher = new ComboCatcher(this);
			HOOK_MANAGER_PTR->insert(catcher);
		}
		void ComboPrompt::onUpdate(float dt)
		{
			if(nowTime>0)
			{
				nowTime -= dt;
				if(getState()==BLOWUP)
					if(lastTime-nowTime<blowupTime)
					{
						float rate = 1+(blowupRate-1)*(1-(lastTime-nowTime)/blowupTime);
						numberTex->setScale(size.x/singleSize.x*rate,size.y/singleSize.y*rate);
						gap = originGap*rate;
					}
					else
					{
						numberTex->setScale(size.x/singleSize.x,size.y/singleSize.y);
						gap = originGap;
						setState(SHOW);
					}
				if(nowTime<fadeOutTime)
				{
					setState(FADEOUT);
					float rate = nowTime/fadeOutTime;
					numberTex->setColor(CSETA(numberTex->getColor(),rate*255*0.6));
					backTex->setColor(CSETA(backTex->getColor(),rate*255*0.6));
				}
				if(nowTime<0)
				{
					nowTime = 0;
					setState(DISPLAY);
				}
			}
		}
		void ComboPrompt::onRender(float x, float y)
		{
			if(getState()!=DISPLAY)
			{
				backTex->render(x+backTex->getPositionX(),y+backTex->getPositionY());
				NumberBar::onRender(x+startPoint.x,y+startPoint.y);
			}
		}
		void ComboPrompt::onClear()
		{
			HOOK_MANAGER_PTR->del(catcher);
			SAFE_DELETE(catcher);
			NumberBar::onClear(); 
			SAFE_DELETE_SPRITE(backTex);
		}
		/*
		 * Prompt
		 */
		void Prompt::construct(Config &config, const std::string &head)
		{
			position = config.getAsRect(head+"position");
			tex = sora::SoraCore::Ptr->createSprite(config.getAsString(head+"tex"));
			
			setPosition(position.x+position.w/2,position.y+position.h/2);
			if(config.has(head+"texRect"))
			{
				setTextureRect(config.getAsRect(head+"texRect"));
				reset();
			}

			lastTime = config.getAsDouble(head+"time_last");
			fadeOutTime = config.getAsDouble(head+"time_fadeOut");
			blowupTime = config.getAsDouble(head+"time_blowup");
			blowupRate = config.getAsDouble(head+"time_blowupRate");
		}
		void Prompt::onUpdate(float dt)
		{
			if(getState()>=PROMPT&&nowTime>0)
			{
				nowTime -= dt;
				if(getState()==BLOWUP)
					if(lastTime-nowTime<blowupTime)
					{
						float rate = 1+(blowupRate-1)*(1-(lastTime-nowTime)/blowupTime);
						tex->setScale(position.w/texRect.w*rate,position.h/texRect.h*rate);
					}
					else
					{
						tex->setScale(position.w/texRect.w,position.h/texRect.h);
						setState(PROMPT);
					}
				if(nowTime<fadeOutTime)
				{
					setState(FADEOUT);
					float rate = nowTime/fadeOutTime;
					tex->setColor(CSETA(tex->getColor(),rate*255*0.6));
				}
				if(nowTime<0)
				{
					nowTime = 0;
					setState(DISPLAY);
				}
			}
		}
		void Prompt::onRender(float x, float y)
		{
			if(getState()>=PROMPT)
				tex->render(x,y);
		}
		void Prompt::setImage(const std::string &file)
		{
			tex->setTexture(sora::SoraCore::Ptr->createTexture(file));
		}
		void Prompt::setTextureRect(const Rect &rect)
		{
			texRect = rect;
		}
		void Prompt::prompt()
		{
			setState(BLOWUP);
			reset();
			nowTime = lastTime;
		}
		void Prompt::reset()
		{
			tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
			tex->setScale(position.w/texRect.w,position.h/texRect.h);
			tex->setCenter(tex->getSpriteWidth()/2,tex->getSpriteHeight()/2);
			tex->setColor(CSETA(tex->getColor(),255*0.6));
		}

		bool EvalPrompt::EvalCatcher::hook(StateEvent &event)
		{
			if((event.type==StateEvent::PRESS||event.type==StateEvent::FAILURE)&&event.tag=="inform"&&event.rank>=0)
				prompt->evaluate(event);
			return false;
		}
		void EvalPrompt::construct(Config &config, const std::string &head)
		{
			Prompt::construct(config,head);
			evalNum = config.getAsInt(head+"evalNum");
			for(int i = 0; i < evalNum; i++)
				texRects.push_back(config.getAsRect(head+iToS(i)));
		}
		void EvalPrompt::onInitialize()
		{
			catcher = new EvalCatcher(this);
			HOOK_MANAGER_PTR->insert(catcher);
		}
		void EvalPrompt::onClear()
		{
			HOOK_MANAGER_PTR->del(catcher);
			SAFE_DELETE(catcher);
			Prompt::onClear();
		}
		void EvalPrompt::evaluate(StateEvent &event)
		{
			setPosition(event.screenPosition.x+position.x+position.w/2,event.screenPosition.y+position.y+position.h/2);
			setTextureRect(texRects[event.rank-1]);
			prompt();
		}
		/*
		 * Text
		 */
		void Text::setText(const std::string &content)
		{
			this->content = content;
			text.setText(sora::s2ws(color+content));
		}
		void Text::construct(Config &config, const std::string &head)
		{
			if(config.has(head+"text"))
				content = config.getAsString(head+"text");
			else
				content = "";
			position = config.getAsPoint(head+"position");
			setPosition(position.x,position.y);

			std::string font = config.getAsString(head+"font_file");
			int size = config.getAsInt(head+"font_size");

			text.setFont(sora::SoraFont::LoadFromFile(font, size));
			if(config.has(head+"font_color"))
				color = "|"+config.getAsString(head+"font_color")+"|";
			else
				color = "";

			setText(content);
		}
		void Text::onRender(float x, float y)
		{
			text.renderTo(x,y);
		}
		void Text::setScale(float scale)
		{
			text.getFont()->setScale(scale);
			this->scale = scale;
		}

		void Title::onStart()
		{
			setText(CORE_PTR->getMapInfo()->header.mapName);
		}

		/*
		 * Player
		 */
		const uint32 Player::TEAM_COLOR[MAX_TEAM] = {CARGB(255,245,23,12),
			CARGB(255,252,180,46),
			CARGB(255,159,218,89),
			CARGB(255,255,229,16),
			CARGB(255,82,233,224),
			CARGB(255,73,196,253),
			CARGB(255,185,85,245),
			CARGB(255,241,71,139)};

		void Player::construct(Config &config, const std::string &head)
		{
			hpBar = NULL;
			if(config.has(head+"life"))
			{
				hpBar = new ScaleBar();
				hpBar->construct(config,head+"life_");
				hpBar->init();
				add(hpBar);
			}
			background = NULL;
			if(config.has(head+"background"))
			{
				background = new Image();
				background->construct(config,head+"background_");
				add(background);
			}
			focus = NULL;
			if(config.has(head+"position"))
				position = config.getAsPoint(head+"position");
			else
				position = Point();
			setPosition(position.x,position.y);
			size = config.getAsPoint(head+"size");

			dangerSpark = NULL;
			if(config.has(head+"danger"))
			{
				dangerSpark = new Spark();
				dangerSpark->construct(config,head+"danger_");
				dangerSpark->init();
				dangerRatio = config.getAsDouble(head+"danger_dangerRatio");
				add(dangerSpark);
			}

			//icon = NULL;
			if(icon)
				add(icon);
			else if(config.has(head+"icon"))
			{
				icon = new Image();
				icon->construct(config,head+"icon_");
				add(icon);
			}
			info = NULL;
			if(config.has(head+"info"))
			{
				info = new Text();
				info->construct(config,head+"info_");
				info->init();
				add(info);
			}
			focus = NULL;
			if(config.has(head+"focus"))
			{
				focus = NULL;
				focus = new Image();
				focus->construct(config,head+"focus_");
				focus->setVisible(false);
				add(focus);
			}
			if(config.has(head+"name"))
				name = config.getAsString(head+"name");
		}
		void Player::onInitialize()
		{
			score = combo = 0, hp = 0;
		}
		void Player::onUpdate(float dt)
		{
			info->setText(format("%d\nX %d\n%s",score,combo,name.c_str()));
			hpBar->setRatio(hp);

			dangerSpark->setFix(hp==0);
			dangerSpark->setSpark(hp<dangerRatio);
		}
		void Player::setScale(float scale)
		{
			position.y = position.y+size.y*this->scale-size.y*scale;
			this->scale = scale;

			setPosition(position.x,position.y);
			background->setScale(scale);
			icon->setScale(scale);
			hpBar->setScale(scale);
			dangerSpark->setScale(scale);
			//info->setScale(scale);
			info->setPosition(info->getPositionX()*scale,info->getPositionY());
		}

		void SinglePlayer::construct(Config &config, const std::string &head)
		{
			Player::construct(config,head);
			setInfo(0,0,0.5);
			Image *highLight = new Image();
			highLight->construct(config,head+"highlight_");
			this->add(highLight);
			
			hpBar->setColor(Player::TEAM_COLOR[0]);
			//setFocus(true);
		}
		void SinglePlayer::onUpdate(float dt)
		{
			setInfo(GAME_MODE_PTR->getScore(),GAME_MODE_PTR->getCombo(),GAME_MODE_PTR->getHPinRatio());
			Player::onUpdate(dt);
		}

		void MultiPlayer::construct(Config &config, const std::string &head)
		{
			this->config = &config;
			this->head = head;
			range = config.getAsRect(head+"range");
			gap = config.getAsDouble(head+"gap");
			size = config.getAsPoint(head+"size");
			main_scale = config.getAsDouble(head+"main_scale");

			setPosition(range.x,range.y);
		}; 
		void MultiPlayer::onInitialize()
		{
			players.clear();
			state = (MultiPlay*)GAME_MODE_PTR;
		}
		void MultiPlayer::onStart()
		{
			TEAMS& teams = state->getGlobalInfo();
			teamID = state->getTeamID();
			playerID = state->getPlayerID();

			int width = 0;

			for(int i = 0; i < teams.size(); i++)
			{
				nowPlayers.push_back(-1);
				for(int j = 0; j < teams[i].players.size(); j++)
				{
					Player *player = new Player();
					//if(0<=teams[i].players[j].netID&&teams[i].players[j].netID<ICON_NUM)
					//{
					//	//player->icon = new Image();
					//	//player->icon->construct(*config,head+"icon"+iToS(teams[i].players[j].netID)+"_");
					//}
					player->construct(*config,head);
					player->onInitialize();

					player->hpBar->setColor(Player::TEAM_COLOR[i]);
					player->setPosition(width,0);
					player->setName(teams[i].players[j].name);
					player->setInfo(0,0,0.5);
					player->flushOnly();
					player->flush();

					add(player);
					players.push_back(player);

					if(i==teamID&&j==playerID)
					{
						Image *highLight = new Image();
						highLight->construct(*config,head+"highlight_");
						player->add(highLight);
					}
					width += gap+size.x;
				}
			}
		}
		void MultiPlayer::onUpdate(float dt)
		{
			TEAMS& teams = state->getGlobalInfo();
			int cnt = 0;
			for(int i = 0; i < teams.size(); i++)
			{
				if(teams[i].nowPlayer!=nowPlayers[i])
				{
					if(nowPlayers[i]>=0)
						players[teams[i].players[nowPlayers[i]].id]->setFocus(false);
					if(teams[i].nowPlayer>=0)
						players[teams[i].players[teams[i].nowPlayer].id]->setFocus(true);
					nowPlayers[i] = teams[i].nowPlayer;
				}
				for(int j = 0; j < teams[i].players.size(); j++)
					players[cnt++]->setInfo(teams[i].players[j].score,teams[i].players[j].combo,teams[i].players[j].hp);
			}
		}

		/*
		 *Eval Result UI
		 */
		void EvalBar::reset()
		{
			nowScore = score = nowGold = gold = nowExp = exp = 0;
			memset(evalCnt,0,sizeof(evalCnt));
			memset(nowCnt,0,sizeof(nowCnt));
		}
		void EvalBar::updateNumber(int &source, int dest)
		{
			if(source<dest)
			{
				if((dest-source)<100)
					source++;
				else
				{
					int scoreDt = int((dest-source)/6);
					source += scoreDt;
				}
				if(source>dest)
					source = dest;
			}
			else if(dest<source)
			{
				if((source-dest)<100)
					source--;
				else
				{
					int scoreDt = int((source-dest)/6);
					source -= scoreDt;
				}
				if(source<dest)
					source = dest;
			}
		}
		void EvalBar::setInfo(int score, int eval[])
		{
			this->score = score;
			memcpy(evalCnt,eval,sizeof(evalCnt));
		}
		void EvalBar::construct(Config &config, const std::string &head)
		{
			this->head = head;
			this->config = &config;

			if(config.has(head+"background"))
			{
				Image *image = new Image();
				image->construct(config,head+"background_");
				add(image);
			}
			if(config.has(head+"gold"))
			{
				Image *image = new Image();
				image->construct(config,head+"gold_");
				add(image);
			}
			if(config.has(head+"exp"))
			{
				Image *image = new Image();
				image->construct(config,head+"exp_");
				add(image);
			}

			for(int i = 0; i < EvaluateStrategy::EVAL_NUM; i++)
			{
				evalNumber[i] = new NumberBar();
				evalNumber[i]->construct(config,head+"evalNumber_");
				evalNumber[i]->init();
				evalNumber[i]->setShowStyle(NumberBar::MIDDLE);
				add(evalNumber[i]);
			}
			scoreNumber = new NumberBar();
			scoreNumber->construct(config,head+"scoreNumber_");
			scoreNumber->init();
			add(scoreNumber);
			goldNumber = new NumberBar();
			goldNumber->construct(config,head+"littleNumber_");
			goldNumber->setSize(Point(13,15));
			goldNumber->init();
			add(goldNumber);
			expNumber = new NumberBar();
			expNumber->construct(config,head+"littleNumber_");
			expNumber->setSize(Point(13,15));
			expNumber->init();
			add(expNumber);

			if(config.has(head+"scorePosition"))
			{
				Point tmp = config.getAsPoint(head+"scorePosition");
				scoreNumber->setPosition(tmp.x,tmp.y);
			}
			if(config.has(head+"goldPosition"))
			{
				Point tmp = config.getAsPoint(head+"goldPosition");
				goldNumber->setPosition(tmp.x,tmp.y);
			}
			if(config.has(head+"expPosition"))
			{
				Point tmp = config.getAsPoint(head+"expPosition");
				expNumber->setPosition(tmp.x,tmp.y);
			}
			for(int i = 0; i < EvaluateStrategy::EVAL_NUM; i++)
			{
				Point tmp = config.getAsPoint(head+iToS(i));
				evalNumber[i]->setPosition(tmp.x,tmp.y);
			}
			
			position = config.getAsPoint(head+"position");
			setPosition(position.x,position.y);

			reset();
		}
		void EvalBar::onInitialize()
		{

		}
		void EvalBar::onUpdate(float dt)
		{
			for(int i = 0; i < EvaluateStrategy::EVAL_NUM; i++)
			{
				updateNumber(nowCnt[i],evalCnt[i]);
				evalNumber[i]->setNumber(nowCnt[i]);
			}
			updateNumber(nowScore,score);
			scoreNumber->setNumber(nowScore);
			updateNumber(nowGold,gold);
			goldNumber->setNumber(nowGold);
			updateNumber(nowExp,exp);
			expNumber->setNumber(nowExp);
		}
		void EvalBar::onRender(float x, float y)
		{
		}

		/*
		 * Button
		 */
		void Button::construct(Config &config, const std::string &head)
		{
			Image::construct(config,head);
			pressed = config.getAsRect(head+"pressedRect");
			highlight = config.getAsRect(head+"highlightRect");
			if(config.has(head+"hotRect"))
				hotRect = config.getAsRect(head+"hotRect");
			else
				hotRect = position;
		}
		void Button::onInitialize()
		{
			handleInput(true);
			bPress = bInner = false;
		}
		void Button::onUpdate(float dt)
		{
			if(bPress)
				tex->setTextureRect(pressed.x,pressed.y,pressed.w,pressed.h);
			else if(bInner)
				tex->setTextureRect(highlight.x,highlight.y,highlight.w,highlight.h);
			else
				tex->setTextureRect(texRect.x,texRect.y,texRect.w,texRect.h);
		}
		void Button::mouseClicked(SoraMouseEvent& event)
		{
			if(bInner)
				bPress = true;
		}
		void Button::mouseReleased(SoraMouseEvent& event)
		{
			if(bPress)
			{
				if(event.getX()>=hotRect.x&&event.getX()<=hotRect.x+hotRect.w
					&&event.getY()>=hotRect.y&&event.getY()<=hotRect.y+hotRect.h)
				{
					bPress = false;
					press();
				}
				else
					bPress = bInner = false;
			}
		}
		void Button::mouseMoved(SoraMouseEvent& event)
		{
			if(event.getX()>=hotRect.x&&event.getX()<=hotRect.x+hotRect.w
				&&event.getY()>=hotRect.y&&event.getY()<=hotRect.y+hotRect.h)
				bInner = true;
			else
				bInner = bPress = false;
		}
		
		/*
		 * Factory
		 */
		WidgetPtr Factory::createWidget(const std::string &name, Config &config)
		{
			WidgetPtr widget = NULL;
			std::string type = config.getAsString(name+"_type");
			if(type=="root")
				widget = new Widget();
			else if(type=="image")
				widget = new Image();
			else if(type=="lifeMetaBar")
				widget = new LifeMetaBar();
			else if(type=="ctMetaBar")
				widget = new CTMetaBar();
			else if(type=="scoreBar")
				widget = new ScoreBar();
			else if(type=="evalPrompt")
				widget = new EvalPrompt();
			else if(type=="title")
				widget = new Title();
			else if(type=="comboPrompt")
				widget = new ComboPrompt();
			else if(type=="lifeScaleBar")
				widget = new LifeScaleBar();
			else if(type=="ctScaleBar")
				widget = new CTScaleBar();
			else if(type=="dangerSpark")
				widget = new DangerSpark();
			else if(type=="singlePlayer")
				widget = new SinglePlayer();
			else if(type=="multiPlayer")
				widget = new MultiPlayer();
			else if(type=="evalBar")
				widget = new EvalBar();
			else if(type=="button")
				widget = new Button();
			else if(type=="ctLabel")
				widget = new CTLabel();

			if(widget)
			{
				widget->construct(config,name+"_");
				if(config.has(name+"_input"))
					widget->handleInput(config.getAsBool(name+"_input"));
			}
			else
				DIVA_EXCEPTION_MODULE("Widget "+name+" not found!","SimpleUI");

			return widget;
		}

		WidgetPtr Factory::createWithJson(const std::string &file, Config &config, UI_MAP &uiMap)
		{
			configloader::loadWithJson(config,file);

			WidgetPtr root = new SimpleUI::Widget();
			uiMap["root"] = root;

			sora::SoraResourceFile data(file);

			Json::Reader reader;
			Json::Value value;

			const char* cdata = (const char*)data;

			if(reader.parse(cdata, cdata+data.size(), value))
			{	
				Json::Value::Members members = value.getMemberNames();
				for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
				{
					if(!config.has(*ptr+"_type")||config.has(*ptr+"_reserve"))
						continue;
					uiMap[*ptr] = SimpleUI::Factory::createWidget((*ptr),config);
				}
				for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
				{
					if(!config.has(*ptr+"_type")||config.has(*ptr+"_reserve"))
						continue;
					if(config.has(*ptr+"_parent"))
					{	
						if(uiMap.find(config.getAsString(*ptr+"_parent"))==uiMap.end())
							DIVA_EXCEPTION_MODULE("Widget "+config.getAsString(*ptr+"_parent")+" not found!","SimpleUI");
						uiMap[config.getAsString(*ptr+"_parent")]->add(uiMap[*ptr]);
					}
					else
						uiMap["root"]->add(uiMap[*ptr]);
				}
			}
			else
				DIVA_EXCEPTION_MODULE(reader.getFormatedErrorMessages(),"SimpleUIPainter");

			return root;
		}
	}
}
