/*
 *  DivaSimpleUI.h
 *
 *  Created by Hyf042 on 2/5/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_SIMPLE_UI
#define DIVA_SIMPLE_UI

#include "Core/DivaCommon.h"
#include "SoraSprite.h"
#include "SoraText.h"
#include "Core/DivaConfig.h"
#include "Core/DivaHook.h"
#include "Core/DivaEvaluateStrategy.h"
#include "Mode/DivaMultiplay.h"
#include "Hook/DivaCTMode.h"

namespace divacore
{
	namespace SimpleUI
	{
		class Widget
		{
		protected:
			typedef std::vector<Widget*> CHILDREN;

			CHILDREN children;
			float x,y,scale;
			int state;
			bool visible,input;

		protected:
			virtual void onInitialize() {setState(HIDE);}
			virtual void onUpdate(float dt) {}
			virtual void onRender(float x, float y) {}
			virtual void onClear() {}
			virtual void onStart() {}
			virtual void onHide() {setState(HIDE);}
			virtual void onDisplay() {setState(DISPLAY);}
			virtual void onMouseClicked(SoraMouseEvent& event) {}
			virtual void onMouseReleased(SoraMouseEvent& event) {}
			virtual void onMouseMoved(SoraMouseEvent& event) {}

			void setState(float state) {this->state=state;}
		public:
			enum{HIDE,DISPLAY};

			Widget():x(0),y(0),scale(1.0),visible(true),input(false) {}
			virtual ~Widget() {}

			void update(float dt) ;
			virtual void render(float x = 0, float y = 0);
			void init();
			void clear();
			void display();
			void hide();
			void start();
			void setVisible(bool visible) {this->visible = visible;}

			virtual void flush() {update(0);}
			virtual void flushOnly() {onUpdate(0);}
			virtual void setScale(float scale) {}
			virtual void setColor(uint32 color) {}
			virtual uint32 getColor() {return 0;}
			void handleInput(bool input) {this->input = input;}

			int getState() {return state;}

			void add(Widget *widget);
			void del(Widget *widget);
			virtual void setPosition(float x, float y);
			float getPositionX() {return x;}
			float getPositionY() {return y;}

			virtual void construct(Config &config, const std::string &head) {}
			virtual void mouseClicked(SoraMouseEvent& event);
			virtual void mouseReleased(SoraMouseEvent& event);
			virtual void mouseMoved(SoraMouseEvent& event);
		};
		typedef Widget* WidgetPtr;

		class Image : public Widget
		{
		protected:
			sora::SoraSprite *tex;
			Rect position;
			Rect texRect;
		public:
			Image() {}

			void setScale(float scale);
			void setColor(uint32 color) {tex->setColor(color);}
			uint32 getColor() {return tex->getColor();}
			void setTextureRect(Rect texRect);
			void construct(Config &config, const std::string &head);
		protected:
			void onInitialize() {}
			void onClear() {SAFE_DELETE_SPRITE(tex);}
			void onRender(float x, float y);
		};

		class CTLabel : public Image
		{
			static const int CT_LEVEL = 7;
			Rect levelTexRect[CT_LEVEL];
			HookPtr ctMode;
			Rect levelLabelRect[CT_LEVEL];
			int level;
			float changeTime, nowTime, expandScale;
		public:
			enum{BLOWUP=0x80,DOWN,DISAPPER};

			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onStart();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void setTextureRect(Rect texRect);
		};

		class SlideBar : public Image
		{
		protected:
			float hideTime, nowTime;
			Rect hideTexRect;
			Point hidePosition,standardPosition;
		public:
			enum{HIDDING=0x80,DISPLAYING};
			SlideBar() {}

			void construct(Config &config, const std::string &head);
		protected:
			void onInitialize() {nowTime=0;}
			void onHide();
			void onDisplay();
			void onClear() {SAFE_DELETE_SPRITE(tex);}
			void onUpdate(float dt);
			void onRender(float x, float y);
		};

		class Spark : public Widget
		{
			bool spark;
			sora::SoraSprite *tex;
			Rect position;
			Rect texRect;
			bool fix;
			std::string SE;
			int lastPeriod;
		public:
			Spark():fix(false),spark(false) {}
			void setSpark(bool spark);
			void setFix(bool fix) {this->fix = fix;}

			void construct(Config &config, const std::string &head);
			void onInitialize() {spark=fix=false;}
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onClear() {SAFE_DELETE_SPRITE(tex);}
			void setScale(float scale) {tex->setScale(scale,scale);this->scale=scale;}
			
			virtual void setColor(uint32 color) {tex->setColor(color);}
			virtual uint32 getColor() {return tex->getColor();}
		};

		class DangerSpark : public Spark
		{
			float dangerRatio;
		public:
			void construct(Config &config, const std::string &head);
			void onUpdate(float dt);
		};

		class MetaBar : public Widget
		{
		protected:
			sora::SoraSprite *metaSprite;
			Point size;
			Rect position;
			int showBlock,blockNum,hBlock,vBlock;
		public:
			void construct(Config &config, const std::string &head);
			void setRatio(float ratio);
			void onInitialize();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onClear() {SAFE_DELETE_SPRITE(metaSprite);}
		};

		class LifeMetaBar : public MetaBar
		{
		public:
			void onUpdate(float dt);
		};

		class CTMetaBar : public LifeMetaBar
		{
			HookPtr ctMode;
		public:
			void onInitialize();
			void onStart();
			void onUpdate(float dt);
		};

		class ScaleBar : public Widget
		{
		protected:
			sora::SoraSprite *tex,*cursor;
			Rect position;
			Rect texRect,cursorRect;
			Rect targetPosition;
			Rect targetTexRect;
			float ratio,nowRatio;
			std::string direction;
		public:
			ScaleBar() : nowRatio(-1) {}
			void construct(Config &config, const std::string &head);
			virtual void setRatio(float ratio);
			void onInitialize();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onClear() {SAFE_DELETE_SPRITE(tex);SAFE_DELETE_SPRITE(cursor);}
			void setScale(float scale);
			void setTexRect(Rect texRect);
			virtual void setColor(uint32 color) {tex->setColor(color);if(cursor)cursor->setColor(color);}
		};

		class LifeScaleBar : public ScaleBar
		{
		public:
			void onInitialize();
			void onUpdate(float dt);
		};

		class CTScaleBar : public ScaleBar
		{
			static const int CT_LEVEL = 7;
			Rect levelTexRect[CT_LEVEL];
			float startRatio;
			HookPtr ctMode;
			//Image *label;
			//Rect levelLabelRect[CT_LEVEL];
			int level;
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onStart();
			void onUpdate(float dt);
			void setRatio(float ratio);
		};

		class CTSlideBar : public SlideBar
		{
			static const int CT_LEVEL = 7;
			Base::SharedPtr<Spark> light;
			Rect ctLevelColor[CT_LEVEL];
			HookPtr ctMode;
			int level;
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onStart();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onDisplay() {}
			void onHide() {}
			void ctHide() {SlideBar::onHide();}
			void ctDisplay() {SlideBar::onDisplay();}
		};

		class NumberBar : public Widget
		{
		protected:
			static const int DIGIT_NUM = 10;

			SoraSprite *numberTex;
			Point size;
			Rect texRect;
			float gap;
			int digitNum;
			Point startPoint,singleSize;
			int numHash[DIGIT_NUM];
			std::string number;
			int8 showStyle;

		public:
			enum ShowStyle{LEFT,RIGHT,MIDDLE};
			NumberBar():showStyle(LEFT) {}

			void setNumber(int n);
			void setSize(Point size);

			void setShowStyle(int8 style) {this->showStyle=style;}
			int8 getShowStyle() {return showStyle;}

			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onClear() {SAFE_DELETE_SPRITE(numberTex);}
		};

		class ScoreBar : public NumberBar
		{
			int nowScore, realScore;
			float changeTime;
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onUpdate(float dt);
			void onClear() { NumberBar::onClear(); }
		};

		class ComboPrompt : public NumberBar
		{
			Rect position;
			float originGap;
			float lastTime, fadeOutTime, blowupTime, nowTime, blowupRate;
			sora::SoraSprite *backTex;
			int hundredDelta;

			class ComboCatcher : public Hook
			{
				ComboPrompt *prompt;
			public:
				std::string getName() {return "ComboCatcher";}
				ComboCatcher(ComboPrompt *prompt):prompt(prompt) {setPriority(Hook::NORMAL);}
				bool condition() {return true;}
				int getAbility() {return Hook::STATE;}
				bool hook(StateEvent &event);
			};
			ComboCatcher *catcher;
		public:
			enum{BLOWUP=0x80,SHOW,FADEOUT};

			void construct(Config &config, const std::string &head);
			void reset();
			void show(float x, float y, int combo);
			void onInitialize();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void onClear();
		};

		class Prompt : public Widget
		{
		protected:
			Rect texRect;
			float lastTime, fadeOutTime, blowupTime, nowTime, blowupRate;
			Rect position;
	
			sora::SoraSprite *tex;
		public:
			enum{PROMPT=0x80,BLOWUP,FADEOUT};
			
			void onInitialize() {}
			void onClear() {SAFE_DELETE_SPRITE(tex);}
			void onUpdate(float dt);
			void onRender(float x, float y);
			void construct(Config &config, const std::string &head);
			void setLastTime(float time) {lastTime=time;}
			void setImage(const std::string &file);
			void setTextureRect(const Rect &rect);
			void reset();
			void prompt();
		};

		class EvalPrompt : public Prompt
		{
			class EvalCatcher : public Hook
			{
				EvalPrompt *prompt;
			public:
				std::string getName() {return "EvalCatcher";}
				EvalCatcher(EvalPrompt *prompt):prompt(prompt) {setPriority(Hook::NORMAL);}
				bool condition() {return true;}
				int getAbility() {return Hook::STATE;}
				bool hook(StateEvent &event);
			};

			int evalNum;
			std::vector<Rect> texRects;
			EvalCatcher *catcher;
		public:

			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onClear();
			void evaluate(StateEvent &event);
		};

		class Text : public Widget
		{
		protected:
			std::wstring content;
			std::string color;
			Point position;
			sora::SoraText text;
			sora::SoraFont::Alignment style;
		public:
			void setText(const std::wstring &content);
			void setText(const std::string &content) {setText(sora::s2ws(content));}
			void construct(Config &config, const std::string &head);
			void onRender(float x, float y);
			void setScale(float scale);
			void setAlign(sora::SoraFont::Alignment style) {this->style = style;}
		};

		class Title : public Text
		{
		public:
			void onStart();
		};

		/*
		 * PlayerInfo
		 */
		class Player : public Widget
		{
			friend class MultiPlayer;

		public:
			static const int MAX_TEAM = 8;
			static const uint32 TEAM_COLOR[MAX_TEAM];
		protected:
			Point size;
			Text *info, *appendInfo;
			Point position;
			Base::String name,append;
			int score,combo;
			float hp,dangerRatio;
			Image *icon, *background, *focus;
			ScaleBar *hpBar;
			Spark *dangerSpark;
		protected:
			Player():icon(NULL) {}
			void setName(Base::String name) {this->name = name;}
			void setAppend(Base::String append) {this->append = append;}
			void setInfo(int score, int combo, float hp) {this->score=score,this->combo=combo,this->hp=hp;}
			void setFocus(bool bFocus) {if(focus)focus->setVisible(bFocus);}
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onUpdate(float dt);
			void onClear() {}
			void setScale(float scale);
			void setPosition(float x, float y) {position=Point(x,y);Widget::setPosition(x,y);}
		};

		class SinglePlayer : public Player
		{
		public:
			void construct(Config &config, const std::string &head);
			void onUpdate(float dt);
		};

		class MultiPlayer : public Widget
		{
			typedef std::vector<Player*> PANELS;
			typedef std::vector<int> STATE;
			static const int ICON_NUM = 4;
			PANELS panels;
			STATE nowPlayers;
			MultiPlay* state;
			Rect range;
			float gap,main_scale;
			Point size;

			int teamID;
			int playerID;

			ConfigPtr config;
			std::string head;
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onStart();
			void onUpdate(float dt);
		};

		class EvalBar : public Widget
		{
			ConfigPtr config;
			std::string head;
			Point position;
			NumberBar *evalNumber[EvaluateStrategy::EVAL_NUM],*scoreNumber,*goldNumber,*expNumber,*comboNumber;
			Rect levelTexRect[MAX_LEVEL];
			Text *info;
			Image *background, *level, *result;
			std::vector<Rect> resultTexRect;

			int evalCnt[EvaluateStrategy::EVAL_NUM],nowCnt[EvaluateStrategy::EVAL_NUM];
			int score,nowScore;
			int maxCombo, nowCombo;
			int maxCTLevel;
			int gold, nowGold;
			int exp, nowExp;
			bool bNumberUp;
		protected:
			void updateNumber(int &source, int dest);
		public:
			void reset();
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onUpdate(float dt);
			void onRender(float x, float y);
			void setInfo(int score, int maxCombo, int maxCTLevel, int eval[], const Base::String &info);
			void setTeamColor(int teamIndex);
			bool isNumberUp() {return bNumberUp;}
			void evalResult();
		};

		class Button : public Image
		{
			Rect pressed, highlight;
			bool bPress,bInner;
			Rect hotRect;
		public:
			void construct(Config &config, const std::string &head);
			void onInitialize();
			void onUpdate(float dt);
			void mouseClicked(SoraMouseEvent& event);
			void mouseReleased(SoraMouseEvent& event);
			void mouseMoved(SoraMouseEvent& event);
			virtual void press() {}
		};

		class Factory
		{
			typedef std::map<std::string,SimpleUI::WidgetPtr> UI_MAP;
		public:
			static WidgetPtr createWidget(const std::string &name, Config &config);
			static WidgetPtr createWithJson(const std::string &file, Config &config, UI_MAP &uiMap);
		};
	}
}

#endif