#include "soraguiinput.hpp"
#include "guichan/exception.hpp"
#include "ui/Config/DivaUIConfig.h"

#if defined(DELETE)
#undef DELETE
#endif

namespace gcn
{	
	SoraGUIInput * g_sinput;
#ifdef WIN32
	WNDPROC	g_lpLastHgeWndProc;
#endif
    SoraGUIInput::SoraGUIInput()
    {
        sora = sora::SoraCore::Instance();

        mMouseX = 0;
        mMouseY = 0;

        mLeftMouseButtonDown = false;
        mRightMouseButtonDown = false;
        mMiddleMouseButtonDown = false;
    }

    SoraGUIInput::~SoraGUIInput()
    {
    }

    bool  SoraGUIInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    bool  SoraGUIInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    KeyInput  SoraGUIInput::dequeueKeyInput()
    {
        if (isKeyQueueEmpty())
        {
            throw GCN_EXCEPTION("Key queue is empty.");
        }

        KeyInput keyInput;

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;

    }

    MouseInput SoraGUIInput::dequeueMouseInput()
    {
        if (isMouseQueueEmpty())
        {
            throw GCN_EXCEPTION("Mouse queue is empty.");
        }

        MouseInput mouseInput;

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void  SoraGUIInput::_pollInput()
    {
        sora::SoraKeyEvent ie;
    
        while(sora->getKeyEvent(ie)) 
		  pollKeyInput(ie);
        
        pollMouseInput();
    }


	// ak add ime
	void SoraGUIInput::onIMEKey( int value )
	{
		Key keyObj(value, true);
		KeyInput keyInput(keyObj, KeyInput::PRESSED);
		mKeyInputQueue.push(keyInput);
	}

    void  SoraGUIInput::pollMouseInput()
    {
        float curMouseX, curMouseY;
        int mouseWheel;

        bool leftBtn, rightBtn, centerBtn;

        sora->getMousePos(&curMouseX, &curMouseY);
		// New modified by sonicmisora @ 2012/3/20   BEGIN
		curMouseX *= double(diva::setConfig[L"gameWidth"].asInt()) / diva::setConfig[L"windowWidth"].asInt();
		curMouseY *= double(diva::setConfig[L"gameHeight"].asInt()) / diva::setConfig[L"windowHeight"].asInt();
		// .......................................   END
        mouseWheel = sora->getMouseWheel();

        leftBtn = sora->getKeyState(SORA_KEY_LBUTTON)==SORA_INPUT_KEYDOWN;
        rightBtn = sora->getKeyState(SORA_KEY_RBUTTON)==SORA_INPUT_KEYDOWN;
        centerBtn = sora->getKeyState(SORA_KEY_MBUTTON)==SORA_INPUT_KEYDOWN;
	
        // Check mouse movement
        if (mMouseX != curMouseX 
            || mMouseY != curMouseY )
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::MOVED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
     
            mMouseX = curMouseX;
            mMouseY = curMouseY;
        }

        // Check mouse wheel
        if (mouseWheel > 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::WHEEL_MOVED_UP,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mouseWheel < 0)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::EMPTY,
                                             MouseInput::WHEEL_MOVED_DOWN,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }

        //check mouse buttons
        if (!mLeftMouseButtonDown && leftBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::LEFT,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mLeftMouseButtonDown && !leftBtn )
        {
            mMouseInputQueue.push(MouseInput(MouseInput::LEFT,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (!mRightMouseButtonDown && rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::RIGHT,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mRightMouseButtonDown &&  !rightBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::RIGHT,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (!mMiddleMouseButtonDown && centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE,
                                             MouseInput::PRESSED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }
        else if (mMiddleMouseButtonDown && !centerBtn)
        {
            mMouseInputQueue.push(MouseInput(MouseInput::MIDDLE,
                                             MouseInput::RELEASED,
                                             (int)curMouseX,
                                             (int)curMouseY,
                                             0));
        }

        mLeftMouseButtonDown  = leftBtn;
        mRightMouseButtonDown = rightBtn;
        mMiddleMouseButtonDown  = centerBtn;
    }

    void  SoraGUIInput::pollKeyInput(sora::SoraKeyEvent &ki)
    {
        if (ki.type != SORA_INPUT_KEYDOWN 
            && ki.type != SORA_INPUT_KEYUP )
        {
             return ;
        }
        
        if (ki.type == SORA_INPUT_KEYDOWN)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::PRESSED);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed(ki.flags & SORA_INPUT_FLAG_SHIFT);
            keyInput.setAltPressed(ki.flags & SORA_INPUT_FLAG_ALT);
            keyInput.setControlPressed(ki.flags & SORA_INPUT_FLAG_CTRL);
        
            mKeyInputQueue.push(keyInput);
        }
  
        if (ki.type == SORA_INPUT_KEYUP)
        {
            Key keyObj = convertToKey(ki.key, ki.chr);
  
            KeyInput keyInput(keyObj, KeyInput::RELEASED);
  
            keyInput.setNumericPad(isNumericPad(ki.key));
  
            keyInput.setShiftPressed(ki.flags & SORA_INPUT_FLAG_SHIFT);
            keyInput.setAltPressed(ki.flags & SORA_INPUT_FLAG_ALT);
            keyInput.setControlPressed(ki.flags & SORA_INPUT_FLAG_CTRL);
        
            mKeyInputQueue.push(keyInput);
        }
    }

    Key SoraGUIInput::convertToKey(int key, int chr)
    {
        int keysym;

        switch(key)
        {
            case SORA_KEY_TAB:
              keysym = Key::TAB;  
              break;
            case SORA_KEY_ESCAPE:
              keysym = Key::ESCAPE;
              break;
            case SORA_KEY_ALT:
              keysym = Key::LEFT_ALT;
              break;
            /*
            Unsupported by HGE.
            case SORA_KEY_ALT: 
            keysym = Key::RIGHT_ALT;
            break;
            */
            case SORA_KEY_SHIFT:
              keysym = Key::LEFT_SHIFT;
              break;
            /*
            Unsuppored by HGE.
            case SORA_KEY_SHIFT:
            keysym = Key::RIGHT_SHIFT;
            break;
            */
            case SORA_KEY_CTRL:
              keysym = Key::LEFT_CONTROL;
              break;
            /*
            Unsupported by HGE.
            case SORA_KEY_CTRL:
            keysym = Key::RIGHT_CONTROL;
            break;
            */
            case SORA_KEY_LWIN:
              keysym = Key::LEFT_META;
              break;
            case SORA_KEY_RWIN:
              keysym = Key::RIGHT_META;
              break;   
            case SORA_KEY_INSERT:
              keysym = Key::INSERT;
              break;  
            case SORA_KEY_HOME:
              keysym = Key::HOME;
              break;  
            case SORA_KEY_PGUP:
              keysym = Key::PAGE_UP;
              break;
            case SORA_KEY_PGDN:
              keysym = Key::PAGE_DOWN;
              break;    
            case SORA_KEY_DELETE:
              keysym = Key::DELETE;
              break;
            /*
            Unsupported by HGE.
            case SORA_KEY_BACKSPACE:
            keysym = Key::DELETE;
            pad = true;
            break;
            */
            case SORA_KEY_END:
              keysym = Key::END;
              break;
            case SORA_KEY_CAPSLOCK:
              keysym = Key::CAPS_LOCK;
              break;
            case SORA_KEY_BACKSPACE:
              keysym = Key::BACKSPACE;
              break;
            case SORA_KEY_F1:
              keysym = Key::F1;
              break;
            case SORA_KEY_F2:
              keysym = Key::F2;
              break;
            case SORA_KEY_F3:
              keysym = Key::F3;
              break;
            case SORA_KEY_F4:
              keysym = Key::F4;
              break;
            case SORA_KEY_F5:
              keysym = Key::F5;
              break;
            case SORA_KEY_F6:
              keysym = Key::F6;
              break;
            case SORA_KEY_F7:
              keysym = Key::F7;
              break;
            case SORA_KEY_F8:
              keysym = Key::F8;
              break;
            case SORA_KEY_F9:
              keysym = Key::F9;
              break;
            case SORA_KEY_F10:
              keysym = Key::F10;
              break;
            case SORA_KEY_F11:
              keysym = Key::F11;
              break;
            case SORA_KEY_F12:
              keysym = Key::F12;
              break;
            /* 
            Unsupported by HGE
            case SORA_KEY_PRTSCR:
            keysym = Key::PRINT_SCREEN;
            break;
            */
            case SORA_KEY_PAUSE:
              keysym = Key::PAUSE;
              break;
            case SORA_KEY_SCROLLLOCK:
              keysym = Key::SCROLL_LOCK;
              break;
            case SORA_KEY_NUMLOCK:
              keysym = Key::NUM_LOCK;
              break;
            case SORA_KEY_LEFT:
              keysym = Key::LEFT;
              break;
            case SORA_KEY_RIGHT:
              keysym = Key::RIGHT;
              break;
            case SORA_KEY_UP:
              keysym = Key::UP;
              break;
            case SORA_KEY_DOWN:
              keysym = Key::DOWN;
              break;
            case SORA_KEY_ENTER:
              keysym = Key::ENTER;
              break;
            default:
              keysym = chr;
        }

        Key k = Key(keysym);

        return k;
    }

    bool  SoraGUIInput::isNumericPad(int keyCode)
    {
        switch(keyCode)
        {
          case SORA_KEY_NUMPAD0:
          case SORA_KEY_NUMPAD1:
          case SORA_KEY_NUMPAD2:
          case SORA_KEY_NUMPAD3:
          case SORA_KEY_NUMPAD4:
          case SORA_KEY_NUMPAD5:
          case SORA_KEY_NUMPAD6:
          case SORA_KEY_NUMPAD7:
          case SORA_KEY_NUMPAD8:
          case SORA_KEY_NUMPAD9:
              return true;
          default: 
              return false;
        }
    }

#ifdef WIN32
	// ak add ime
	LRESULT CALLBACK inputWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		if (WM_KEYDOWN == nMsg || WM_KEYUP == nMsg)
		{
			MSG aMsg;
			memset(&aMsg,0,sizeof(MSG));

			aMsg.hwnd	= hWnd;
			aMsg.message= nMsg;
			aMsg.wParam	= wParam;
			aMsg.lParam	= lParam;
			aMsg.time	= GetTickCount();

			TranslateMessage(&aMsg);

			if (WM_KEYDOWN == nMsg)
			{
				switch(wParam)
				{
				case VK_DELETE: //key delete
				case VK_LEFT: //key left
				case VK_RIGHT: //key right
				case VK_HOME: //key home
				case VK_END: //key end
					//g_lpFocusEditPtr->OnChar(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
					break;

				}
			}
		}
		else if (WM_CHAR == nMsg )
		{
			return TRUE;
		}
		else if (WM_IME_CHAR == nMsg)
		{
 			char imeChar[3];
 			imeChar[0] = (char)(wParam>>8);
 			imeChar[1] = (char)wParam;
 			imeChar[2] = '\0';

			g_sinput->onIMEKey(wParam);

			return TRUE;
		}

		return g_lpLastHgeWndProc?g_lpLastHgeWndProc(hWnd,nMsg,wParam,lParam):TRUE;
	}
#endif
	 void SoraGUIInput::setSInput(SoraGUIInput* in) { 
#ifdef WIN32
		 HWND hWnd = (HWND)sora::SoraCore::Instance()->getMainWindowHandle();
		 if (!hWnd) throw NULL;

		 if (g_lpLastHgeWndProc == NULL) {
			 LONG prevWndProc = ::GetWindowLong(hWnd, GWL_WNDPROC);
			 LONG wndProc = ::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)inputWndProc);

			 g_lpLastHgeWndProc = (WNDPROC)(prevWndProc);

			 g_sinput=in;
		 }
#endif
	};
}
