#ifndef SORA_GUIANIMATION
#define SORA_GUIANIMATION

#include "Hoshizora.h"
#include "guichan.hpp"
#include "guichan/animation.hpp"
#include<algorithm>
#include<cmath>

namespace gcn
{
	using namespace std;
	using namespace sora;


	class Animation;
	/*
	Common
	*/
	const double PI = 3.1415926;
	const double EPS = 1e-10;
	class Point
	{
	public:
		float x,y;
		Point():x(0),y(0) {}
		Point(const float &_x, const float &_y):x(_x),y(_y) {}
		float mod() {return sqrt(x*x+y*y);}
		Point unit() {float tmp = mod(); return Point(x/tmp,y/tmp);}
		Point normal() {return Point(y,-x);}
		friend Point operator+(const Point &a, const Point &b) {return Point(a.x+b.x,a.y+b.y);}
		friend Point operator-(const Point &a, const Point &b) {return Point(a.x-b.x,a.y-b.y);}
		friend Point operator*(const Point &a, const float &b) {return Point(a.x*b,a.y*b);}
		friend float operator*(const Point &a, const Point &b) {return a.x*b.x+a.y*b.y;}
		friend bool operator==(const Point &a, const Point &b) {return abs(a.x-b.x)<EPS&&abs(a.y-b.y)<EPS;}
		friend float cross(const Point &a, const Point &b) {return a.x*b.y-b.x*a.y;}
	};

	class Rect
	{
	public:
		float x,y,w,h;
		Rect():x(0),y(0),w(0),h(0) {}
		Rect(float x, float y, float w, float h):x(x),y(y),w(w),h(h) {}
		Rect(const Point &a, const Point &b):x(a.x),y(a.y),w(b.x),h(b.y) {}
		Point position() const {return Point(x,y);}
		Point size() const {return Point(w,h);}
		Point center() const {return Point(x+w/2,y+h/2);}
		static Rect get(const Rect &a, const Rect &b, float c)
		{
			return Rect(a.position()+(b.position()-a.position())*c,a.size()+(b.size()-a.size())*c);
		}
	};


	#define GUIAnimation_Position_Name "GUIAnimation_Position"
	#define GUIAnimation_Stun_Name "GUIAnimation_Stun"
	#define GUIAnimation_Alpha_Name "GUIAnimation_Alpha"

	bool GUIAnimation_Float_LinearCos(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out);
	bool GUIAnimation_Float_LinearSin(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out);
	bool GUIAnimation_Float_Linear(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out);
	bool GUIAnimation_Float_LinearLog(double ratio, float &beginFloat, float &destFloat, float &nowFloat, float &out);

	/*
	 * GUIAnimation_Position
	 * Use for animations which 
	 */
	class GUIAnimation_Position : public Animation
	{
	private:
		Point _destPoint,_beginPoint,_nowPoint;
		SoraFunction<bool(double,float&,float&,float&,float&)> _algorithmX, _algorithmY;

		virtual void onAdd(Widget* widget);

	public:
		GUIAnimation_Position(Point destPoint, int totalTime, 
							SoraFunction<bool(double,float&,float&,float&,float&)> algorithm,
							GUIAnimation_Mode animationMode = GUIAnimation_Mode::NONE, Animation *nextAnimation = NULL,
							SoraFunction<void(Widget*)> AnimationEnded = SoraFunction<void(Widget*)>());

		GUIAnimation_Position(Point destPoint, int totalTime, 
							SoraFunction<bool(double,float&,float&,float&,float&)> algorithmX, 
							SoraFunction<bool(double,float&,float&,float&,float&)> algorithmY, 
							GUIAnimation_Mode animationMode = GUIAnimation_Mode::NONE, Animation *nextAnimation = NULL, 
							SoraFunction<void(Widget*)> AnimationEnded = SoraFunction<void(Widget*)>());

		virtual void update(Widget *widget);

		virtual string getName() const;
	};


	/*
	 * GUIAnimation_Position
	 * Use for the stun between other animations 
	 */
	class GUIAnimation_Stun : public Animation
	{
	public:
		
		GUIAnimation_Stun(int totalTime, 
							GUIAnimation_Mode animationMode = GUIAnimation_Mode::NONE, Animation *nextAnimation = NULL, 
							SoraFunction<void(Widget*)> AnimationEnded = SoraFunction<void(Widget*)>());

		virtual void update(Widget *widget);

		virtual string getName() const;
	};

	/*
	 * GUIAnimation_Alpha
	 * Use for changing a widget's alpha value
	 */
	class GUIAnimation_Alpha : public Animation
	{
	private:
		float _destAlpha, _beginAlpha, _nowAlpha;
		SoraFunction<bool(double,float&,float&,float&,float&)> _algorithm;

		virtual void onAdd(Widget* widget);
	public:
		GUIAnimation_Alpha(float destFloat, int totalTime, 
							SoraFunction<bool(double,float&,float&,float&,float&)> algorithm,
							GUIAnimation_Mode animationMode = GUIAnimation_Mode::NONE, Animation *nextAnimation = NULL, 
							SoraFunction<void(Widget*)> AnimationEnded = SoraFunction<void(Widget*)>());

		virtual void update(Widget* widget);

		virtual string getName() const;

	};

}
#endif