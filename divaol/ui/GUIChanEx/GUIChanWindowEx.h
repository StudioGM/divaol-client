#pragma once

#include "GUIChanContainerEx.h"
#include "GUIChanLabelEx.h"
#include "GUIChanSuperButtonEx.h"
#include "SoraAutoUpdate.h"
#include "Lib/Base/Base.h"
#include "lib/wjson/wjson.h"
#include <deque>
#include <list>
#undef MessageBoxEx

namespace gcn
{
	class WindowMgr;

	class WindowEx : public ContainerEx, public MouseListener
	{
	protected:
		WindowMgr* mgr;
		int mDragOffsetX;
        int mDragOffsetY;
        bool mMoved;
		bool mMovable;

	public:
		WindowEx(WindowMgr* mgr = NULL);
		virtual ~WindowEx();
		
		virtual void FadeOut(int time, bool closeWhenFinished = true);
		virtual void FadeIn(int time);

		void DestroyMySelf(Widget* w);
		virtual void OnDestroy();
		void Destroy();
		void SetMgr(WindowMgr* mgr);

        void SetMovable(bool movable);

        bool IsMovable() const;

		bool isTopWindow() const;

		virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);

        virtual void mouseReleased(MouseEvent& mouseEvent);
	};

	class MessageBoxEx : public WindowEx
	{
	public:
		enum TYPE {TYPE_OK, TYPE_YESNO, TYPE_NONE};
		enum RES {RES_OK, RES_YES, RES_NO, RES_NONE};
		typedef Base::Function<void()> Callback;
	private:
		LabelEx* cap;
		LabelEx* text;
		SuperButtonEx* b[3];
		RES lastRes;
		int fadeOutTime;
		Callback mCallBack;
		
		class ButtonClick : public MouseListener
		{
		public:
			virtual void mouseClicked(MouseEvent& mouseEvent)
			{
				((MessageBoxEx*)mouseEvent.getSource()->getParent())->Clicked(
					(RES)((int)((SuperButtonEx*)mouseEvent.getSource())->userData));
			}
		};

	public:
		MessageBoxEx();
		virtual ~MessageBoxEx();

		void LoadFromJsonFile(WJson::Value conf);
		void Show(const std::wstring& text, const std::wstring& cap = L"", TYPE type = TYPE_OK);
		void RegisterCallback(const Callback &callback = Callback()) {mCallBack = callback;}
		void Clicked(RES btn);

		RES GetResult() const;
	};

	class WindowMgr : public sora::SoraAutoUpdate
	{
	public:
		//enum MB_Type {TYPE_OK, TYPE_YES_NO, TYPE_NONE};
		//enum MB_Result {RES_OK, RES_YES, RES_NO, RES_NONE};
	private:
		
		struct WindowInfo
		{
			WindowEx* window;
			bool lastEnableStauts;
			bool lastVisibleStatus;
			Container* blackBack;
			bool autoDelete;

			WindowInfo(WindowEx* window, bool lastEnableStauts, bool lastVisibleStatus, Container* blackBack, bool autoDelete)
			{
				this->window = window;
				this->lastEnableStauts = lastEnableStauts;
				this->lastVisibleStatus = lastVisibleStatus;
				this->blackBack = blackBack;
				this->autoDelete = autoDelete;
			}
		};

		std::deque<WindowInfo> windows;
		Container* top;
		std::list<WindowEx*> dWindowList;
		std::list<Container*> dContainerList;
		MessageBoxEx* messageBox;
		//MB_Result lastResult;

	public:
		//enum MB_Type {TYPE_OK, TYPE_YES_NO};
		//enum MB_Result {RES_OK, RES_YES, RES_NO};


		virtual void onUpdate(float dt);

		void RegisterMessageBox(MessageBoxEx* box);
		MessageBoxEx* GetMB() const;

		int GetBaseWindowWidth() const {return top->getWidth();}
		int GetBaseWindowHeight() const {return top->getHeight();}

		//void ShowMessageBox(const std::wstring& text, const std::wstring& caption = L"", MB_Type type = TYPE_OK);

		//MB_Result GetMBResult() const;

		// 必须给出使用的gcn的top container
		WindowMgr(Container* top);
		~WindowMgr();
		// 关掉顶层的window
		void CloseTopWindow();
		void OpenWindow(WindowEx* window, int blackAlpha = 0, bool autoDelete = false);
		// 获取顶层的window
		WindowEx* GetTopWindow() const;
	};
}