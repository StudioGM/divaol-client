#include "GUIChanWindowEx.h"
#include "Animation/SoraGUIAnimation.h"
#include "GUIChanJsonHelper.h"
#include <cassert>

namespace gcn
{
	WindowEx::WindowEx(WindowMgr* mgr)
	{
		addMouseListener(this);
		SetMgr(mgr);
		SetMovable(false);
		//this->mgr = mgr;
	}

	WindowEx::~WindowEx()
	{

	}

	void WindowEx::OnDestroy()
	{
		
	}

	void WindowEx::FadeOut(int time, bool closeWhenFinished)
	{
		setAlpha(255);
		if (!closeWhenFinished)
			addModifier(new GUIAnimation_Alpha(0, time, GUIAnimation_Float_Linear));
		else
			addModifier(new GUIAnimation_Alpha(0, time, GUIAnimation_Float_Linear, NONE, NULL, sora::Bind(this, &WindowEx::DestroyMySelf)));
	}

	void WindowEx::FadeIn(int time)
	{
		setVisible(true);
		setAlpha(0);
		addModifier(new GUIAnimation_Alpha(255, time, GUIAnimation_Float_Linear));
	}

	void WindowEx::DestroyMySelf(Widget* w)
	{
		Destroy();
	}

	void WindowEx::Destroy()
	{
		assert(mgr->GetTopWindow() == this);
		OnDestroy();
		mgr->CloseTopWindow();
	}
	
	void WindowEx::SetMgr(WindowMgr* mgr)
	{
		this->mgr = mgr;
	}

	void WindowEx::SetMovable(bool movable)
	{
		mMovable = movable;
	}

	bool WindowEx::IsMovable() const
	{
		return mMovable;
	}

	void WindowEx::mousePressed(MouseEvent& mouseEvent)
	{
		if (mouseEvent.getSource() != this)
		{
			return;
		}

		//if (getParent() != NULL)
		//{
		//	getParent()->moveToTop(this);
		//}

		mDragOffsetX = mouseEvent.getX();
		mDragOffsetY = mouseEvent.getY();

		//mMoved = mouseEvent.getY() <= (int)mTitleBarHeight;
		mMoved = true;
	}

	void WindowEx::mouseReleased(MouseEvent& mouseEvent)
	{
		mMoved = false;
	}

	void WindowEx::mouseDragged(MouseEvent& mouseEvent)
	{
		if (mouseEvent.isConsumed() || mouseEvent.getSource() != this)
		{
			return;
		}

		if (IsMovable() && mMoved)
		{
			setPosition(mouseEvent.getX() - mDragOffsetX + getX(),
				mouseEvent.getY() - mDragOffsetY + getY());
		}

		mouseEvent.consume();
	}

	//////////////////////////////////////////////////////////////////////////

	MessageBoxEx::MessageBoxEx()
	{
		cap = NULL;
		text = NULL;
		for (int i=0; i<3; i++)
			b[i] = NULL;
	}

	MessageBoxEx::~MessageBoxEx()
	{

	}

	void MessageBoxEx::LoadFromJsonFile(WJson::Value conf)
	{
		// my self
		WJson::Value tv = conf[L"background"];
		load(tv[L"filename"].asString(), Helper::GetRect(tv));
		setSize(tv[L"width"].asInt(), tv[L"height"].asInt());
		setOpaque(false);

		
		tv = conf[L"config"];
		fadeOutTime = tv[L"fadeOutTime"].asInt();
		// my caption
		if (cap)
			remove(cap);
	
		cap = new LabelEx();
		cap->setPosition(tv[L"cap_x"].asInt(), tv[L"cap_y"].asInt());
		cap->setSize(10, cap->getFont()->getHeight() + 2);
		cap->setForegroundColor(Color(tv[L"cap_color"].asUInt()));
		add(cap);
		
		// my text
		if (text)
			delete text;
		text = new LabelEx();
		text->setY(tv[L"text_y"].asInt());
		text->setSize(10, cap->getFont()->getHeight() + 2);
		text->setForegroundColor(Color(tv[L"text_color"].asUInt()));
		add(text);
		
		// my buttons
		if (b[0])
		{
			for (int i=0; i<3; i++)
				delete b[i];
		}
		for (int i=0; i<3; i++)
			b[i] = Helper::CreateButton(conf, L"btn/normal", L"btn/mouseon", L"btn/mousedown", L"btn/normal");
		b[0]->setPosition(tv[L"1_1_x"].asInt(), tv[L"1_1_y"].asInt());
		b[1]->setPosition(tv[L"2_1_x"].asInt(), tv[L"2_1_y"].asInt());
		b[2]->setPosition(tv[L"2_2_x"].asInt(), tv[L"2_2_y"].asInt());
		b[0]->setText(L"ºÃµÄ");
		b[1]->setText(L"ÊÇ");
		b[2]->setText(L"·ñ");
		b[0]->userData = (void*) RES_OK;
		b[1]->userData = (void*) RES_YES;
		b[2]->userData = (void*) RES_NO;
		for (int i=0; i<3; i++)
		{
			b[i]->addMouseListener(new ButtonClick());
			add(b[i]);
		}
	}

	void MessageBoxEx::Show(const std::wstring& text_t, const std::wstring& cap_t, MessageBoxEx::TYPE type)
	{
		SetMovable(true);
		setVisible(true);
		setPosition((mgr->GetBaseWindowWidth() - getWidth()) / 2, 
			(mgr->GetBaseWindowHeight() - getHeight()) / 2 );

		cap->setText(cap_t);
		cap->setWidth(cap->getFont()->getWidthW(cap_t) + 3);
		text->setText(text_t);
		text->setX((getWidth() - text->getFont()->getWidthW(text_t)) / 2);
		text->setWidth(text->getFont()->getWidthW(text_t) + 3);

		if (type == TYPE_OK)
		{
			b[0]->setVisible(true);
			b[1]->setVisible(false);
			b[2]->setVisible(false);
		}
		else if (type == TYPE_YESNO)
		{
			b[0]->setVisible(false);
			b[1]->setVisible(true);
			b[2]->setVisible(true);
		}
		else if (type == TYPE_NONE)
		{
			b[0]->setVisible(false);
			b[1]->setVisible(false);
			b[2]->setVisible(false);
		}

		assert(mgr);
		
		if (mgr->GetTopWindow() != (WindowEx*)this)
		{
			FadeIn(10);
			mgr->OpenWindow(this);
		}
	}

	void MessageBoxEx::Clicked(MessageBoxEx::RES btn)
	{
		lastRes = btn;
		if (fadeOutTime != 0)
			FadeOut(fadeOutTime);
		else
			Destroy();
	}

	MessageBoxEx::RES MessageBoxEx::GetResult() const
	{
		return lastRes;
	}

	//////////////////////////////////////////////////////////////////////////


	WindowMgr::WindowMgr(Container* top)
	{
		this->top = top;
		windows.clear();
		windows.push_back(WindowInfo(new WindowEx(this), true, true, NULL, false));
		dWindowList.clear();
		dContainerList.clear();
		messageBox = NULL;
	}

	WindowMgr::~WindowMgr()
	{
		while (windows.size() > 1)
			CloseTopWindow();
		delete GetTopWindow();
	}

	WindowEx* WindowMgr::GetTopWindow() const
	{
		return windows.back().window;
	}

	void WindowMgr::RegisterMessageBox(MessageBoxEx* box)
	{
		messageBox = box;
		messageBox->SetMgr(this);
	}

	MessageBoxEx* WindowMgr::GetMB() const
	{
		return messageBox;
	}

	void WindowMgr::CloseTopWindow()
	{
		WindowInfo info = windows.back();
		windows.pop_back();
		assert(windows.size() >= 1);
		//top->remove(info.window);
		dWindowList.push_back(info.window);
		GetTopWindow()->setEnabled(info.lastEnableStauts);
		GetTopWindow()->setVisible(info.lastVisibleStatus);
		if (info.blackBack)
		{
			dContainerList.push_back(info.blackBack);
			//top->remove(info.blackBack);
			//delete info.blackBack;
		}
		if (info.autoDelete)
		{
			//dWindowList.push_back(info.window);
			//delete info.window;
		}
	}

	void WindowMgr::onUpdate(float dt)
	{
		for (std::list<WindowEx*>::iterator i = dWindowList.begin(); i != dWindowList.end(); i++)
			//delete (*i);
			top->remove(*i);
		for (std::list<Container*>::iterator i = dContainerList.begin(); i != dContainerList.end(); i++)
		{
			top->remove(*i);
			delete (*i);
		}
		dWindowList.clear();
		dContainerList.clear();
	}

	void WindowMgr::OpenWindow(WindowEx* window, int blackAlpha, bool autoDelete)
	{
		window->SetMgr(this);
		Container* con = NULL;
		if (blackAlpha)
		{
			con = new Container();
			con->setSize(top->getWidth(), top->getHeight());
			con->setPosition(0, 0);
			con->setBaseColor(gcn::Color(0));
			//con->seColor(gcn::Color(0));
			con->setAlpha(blackAlpha);	
			top->add(con);
		}
		bool lastEnable = GetTopWindow()->isEnabled(), lastVisible = GetTopWindow()->isVisible();
		GetTopWindow()->setEnabled(false);
		top->add(window);
		windows.push_back(WindowInfo(window, lastEnable, lastVisible, con, autoDelete));
	}
}