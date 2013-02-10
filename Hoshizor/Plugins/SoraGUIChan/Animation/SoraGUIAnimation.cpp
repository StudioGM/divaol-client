#include "guichan.hpp"
#include "guichan/animation.hpp"

#include "Animation/SoraGUIAnimation.h"

#include<algorithm>
#include<cmath>

namespace gcn
{
	using namespace sora;
	using namespace std;

	/*
	 * GUIAnimation_Float_Algorithms
	 */
	bool GUIAnimation_Float_LinearCos(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out)
	{
		out = beginFloat + double(destFloat-beginFloat) * (1-cos(ratio*PI/2));

		if(out == destFloat)
			return true;
		return false;
	}
	bool GUIAnimation_Float_LinearSin(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out)
	{
		out = beginFloat + double(destFloat-beginFloat) * (sin(ratio*PI/2));

		if(out == destFloat)
			return true;
		return false;
	}
	bool GUIAnimation_Float_Linear(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out)
	{
		out = beginFloat + double(destFloat-beginFloat) * ratio;

		if(out == destFloat)
			return true;
		return false;
	}
	bool GUIAnimation_Float_LinearLog(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out)
	{
		static const double factor = 20.0;
		out = nowFloat;
		double delta = double(destFloat-nowFloat) / factor;
		bool cannotDelta = (abs(delta) <= EPS);
		bool NeedPlus = beginFloat < destFloat;
			
		if(nowFloat != destFloat)
			out += cannotDelta ? (nowFloat<destFloat?1:-1) : delta;

		if((NeedPlus && out > destFloat) || (!NeedPlus && out < destFloat)) out = nowFloat = destFloat;

		if(out == destFloat)
			return true;
		return false;
	}

	/*
	 * GUIAnimation_Position
	 */
	GUIAnimation_Position::GUIAnimation_Position(Point destPoint, int totalTime, SoraFunction<bool(double,float&,float&,float&,float&)> algorithm, 
												GUIAnimation_Mode animationMode, Animation *nextAnimation, SoraFunction<void(Widget*)> AnimationEnded) 
												: Animation(totalTime, animationMode, nextAnimation, AnimationEnded)
	{
		_destPoint = destPoint;
		_algorithmX = algorithm;
		_algorithmY = algorithm;
	}
	GUIAnimation_Position::GUIAnimation_Position(Point destPoint, int totalTime,
												SoraFunction<bool(double,float&,float&,float&,float&)> algorithmX,
												SoraFunction<bool(double,float&,float&,float&,float&)> algorithmY, 
												GUIAnimation_Mode animationMode, Animation *nextAnimation, SoraFunction<void(Widget*)> AnimationEnded) 
												: Animation(totalTime, animationMode, nextAnimation, AnimationEnded)
	{
		_destPoint = destPoint;
		_algorithmX = algorithmX;
		_algorithmY = algorithmY;
	}
	void GUIAnimation_Position::onAdd(Widget* widget)
	{
		Animation::onAdd(widget);
		int nowX,nowY;
		nowX = widget->getX();
		//widget->getPosition(nowX,nowY);
		nowY = widget->getY();
		_beginPoint = Point(nowX,nowY);
		_nowPoint = _beginPoint;
	}
	void GUIAnimation_Position::update(Widget *widget)
	{
		Animation::update(widget);

		int nowX,nowY;
		widget->getAbsolutePosition(nowX,nowY);

		Point newPoint;
		//bool ended = _algorithm(getCurrTimeRatio(), _beginPoint, _destPoint, _nowPoint, newPoint);

		bool endedX = _algorithmX(getCurrTimeRatio(), _beginPoint.x, _destPoint.x, _nowPoint.x, newPoint.x);
		bool endedY = _algorithmY(getCurrTimeRatio(), _beginPoint.y, _destPoint.y, _nowPoint.y, newPoint.y);

		widget->setPosition(newPoint.x,newPoint.y);
		_nowPoint = newPoint;

		if(getCurrTimeRatio() == 1 || (endedX && endedY))
		{
			if(_AnimationEnded)
				_AnimationEnded(widget);

			widget->setPosition(_destPoint.x,_destPoint.y);

			if(_animationMode == GUIAnimation_Mode::NONE)
			{
				setFinish(true);
				if(_nextAnimation)
					widget->addModifier(_nextAnimation);
			}
			else if(_animationMode == GUIAnimation_Mode::PINGPONG)
			{
				reset(this->mTotalTime);
				_destPoint = _beginPoint;
				_beginPoint = _nowPoint;
			}
			else if(_animationMode == GUIAnimation_Mode::REPEAT)
			{
				reset(this->mTotalTime);
				_nowPoint = _beginPoint;
			}
		}
	}
	string GUIAnimation_Position::getName() const
	{
		return GUIAnimation_Position_Name;
	}


	/*
	 * GUIAnimation_Stun
	 */
	GUIAnimation_Stun::GUIAnimation_Stun(int totalTime, 
										GUIAnimation_Mode animationMode, Animation *nextAnimation, SoraFunction<void(Widget*)> AnimationEnded) 
										: Animation(totalTime, animationMode, nextAnimation, AnimationEnded){}
	void GUIAnimation_Stun::update(Widget *widget)
	{
		Animation::update(widget);
		if(getCurrTimeRatio()==1)
		{
			if(_AnimationEnded)
				_AnimationEnded(widget);
			setFinish(true);
			if(_nextAnimation)
				widget->addModifier(_nextAnimation);
		}
	}
	string GUIAnimation_Stun::getName() const
	{
		return GUIAnimation_Stun_Name;
	}


	/*
	 * GUIAnimation_Alpha
	 */
	GUIAnimation_Alpha::GUIAnimation_Alpha(float destFloat, int totalTime, 
							SoraFunction<bool(double,float&,float&,float&,float&)> algorithm,
							GUIAnimation_Mode animationMode, Animation *nextAnimation, SoraFunction<void(Widget*)> AnimationEnded)
							: Animation(totalTime, animationMode, nextAnimation, AnimationEnded)
	{
		_destAlpha = destFloat;
		_algorithm = algorithm;
	}
	void GUIAnimation_Alpha::onAdd(Widget* widget)
	{
		Animation::onAdd(widget);
		_beginAlpha = widget->getAlpha();
		_nowAlpha = _beginAlpha;
	}
	void GUIAnimation_Alpha::update(Widget* widget)
	{
		Animation::update(widget);

		float nowAlpha = widget->getAlpha();
		float newAlpha;

		bool ended = _algorithm(getCurrTimeRatio(), _beginAlpha, _destAlpha, _nowAlpha, newAlpha);

		widget->setAlpha(newAlpha);
		_nowAlpha = newAlpha;

		if(getCurrTimeRatio() == 1 || ended)
		{
			if(_AnimationEnded)
				_AnimationEnded(widget);
			widget->setAlpha(_destAlpha);
			if(_animationMode == GUIAnimation_Mode::NONE)
			{
				setFinish(true);
				if(_nextAnimation)
					widget->addModifier(_nextAnimation);
			}
			else if(_animationMode == GUIAnimation_Mode::PINGPONG)
			{
				reset(this->mTotalTime);
				_destAlpha = _beginAlpha;
				_beginAlpha = _nowAlpha;
			}
			else if(_animationMode == GUIAnimation_Mode::REPEAT)
			{
				reset(this->mTotalTime);
				_nowAlpha = _beginAlpha;
			}
		}
	}
	string GUIAnimation_Alpha::getName() const
	{
		return GUIAnimation_Alpha_Name;
	}
}
