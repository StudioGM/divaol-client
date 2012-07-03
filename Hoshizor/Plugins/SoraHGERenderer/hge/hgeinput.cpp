/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: input
*/

/************************************************************************/
/* This define is added by h5nc (h5nc@yahoo.com.cn)                     */
/************************************************************************/
#define INITGUID

#include "hge_impl.h"
/************************************************************************/
/* These header files are added by h5nc (h5nc@yahoo.com.cn)             */
/************************************************************************/
#include <dinput.h>
#include <objbase.h>

#include "SoraInputQueue.h"

char *KeyNames[] =
{
 "?",
 "Left Mouse Button", "Right Mouse Button", "?", "Middle Mouse Button",
 "?", "?", "?", "Backspace", "Tab", "?", "?", "?", "Enter", "?", "?",
 "Shift", "Ctrl", "Alt", "Pause", "Caps Lock", "?", "?", "?", "?", "?", "?",
 "Escape", "?", "?", "?", "?",
 "Space", "Page Up", "Page Down", "End", "Home",
 "Left Arrow", "Up Arrow", "Right Arrow", "Down Arrow",
 "?", "?", "?", "?", "Insert", "Delete", "?",
 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
 "?", "?", "?", "?", "?", "?", "?",
 "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
 "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
 "Left Win", "Right Win", "Application", "?", "?",
 "NumPad 0", "NumPad 1", "NumPad 2", "NumPad 3", "NumPad 4",
 "NumPad 5", "NumPad 6", "NumPad 7", "NumPad 8", "NumPad 9",
 "Multiply", "Add", "?", "Subtract", "Decimal", "Divide",
 "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Num Lock", "Scroll Lock",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?",
 "Left bracket", "Backslash", "Right bracket", "Apostrophe",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
 "?", "?", "?"
};


bool CALL HGE_Impl::Input_GetEvent(hgeInputEvent *event)
{
	CInputEventList *eptr;

	if(queue)
	{
		eptr=queue;
		memcpy(event, &eptr->event, sizeof(hgeInputEvent));
		queue=eptr->next;
		delete eptr;
		return true;
	}
	
	return false;
}

void CALL HGE_Impl::Input_GetMousePos(float *x, float *y)
{
	*x=Xpos; *y=Ypos;
}


void CALL HGE_Impl::Input_SetMousePos(float x, float y)
{
	POINT pt;
	pt.x=(long)x; pt.y=(long)y;
	ClientToScreen(hwnd, &pt);
	SetCursorPos(pt.x,pt.y);
}

int CALL HGE_Impl::Input_GetMouseWheel()
{
	return Zpos;
}

bool CALL HGE_Impl::Input_IsMouseOver()
{
	return bMouseOver;
}

bool CALL HGE_Impl::Input_GetKeyState(int key)
{
	/************************************************************************/
	/* This condition is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	if(bActive)
		return ((GetKeyState(key) & 0x8000) != 0);
	return false;
}

bool CALL HGE_Impl::Input_KeyDown(int key)
{
	/************************************************************************/
	/* This condition is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	if(bActive)
		return (keyz[key] & 1) != 0;
	return false;
}

bool CALL HGE_Impl::Input_KeyUp(int key)
{
	/************************************************************************/
	/* This condition is added by h5nc (h5nc@yahoo.com.cn)                  */
	/************************************************************************/
	if(bActive)
		return (keyz[key] & 2) != 0;
	return false;
}

char* CALL HGE_Impl::Input_GetKeyName(int key)
{
	return KeyNames[key];
}

int CALL HGE_Impl::Input_GetKey()
{
	return VKey;
}

int CALL HGE_Impl::Input_GetChar()
{
	return Char;
}


//////// Implementation ////////


void HGE_Impl::_InputInit()
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	Xpos = (float)pt.x;
	Ypos = (float)pt.y;

	memset(&keyz, 0, sizeof(keyz));
}

/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
bool CALL HGE_Impl::Input_GetDIKey(int key, BYTE stateType)
{
	if(key >= 0 && key < 256)
	{
		switch(stateType)
		{
		case DIKEY_PRESSED:
			if(keyState[key])
				return true;
			break;
		case DIKEY_DOWN:
			if(keyState[key] && !lastKeyState[key])
				return true;
			break;
		case DIKEY_UP:
			if(!keyState[key] && lastKeyState[key])
				return true;
			break;
		default:
			return false;
		}
	}
	return false;
}

bool CALL HGE_Impl::Input_SetDIKey(int key, bool set)
{
	if(key >=0 && key < 256)
	{
		if(set)
			keyState[key] |= 1<<7;
		else
			keyState[key] = 0;
	}
	return true;
}

bool CALL HGE_Impl::Input_GetDIJoy(int joy, BYTE stateType)
{
	if(joy >=0 && joy < 32)
	{
		switch(stateType)
		{
		case DIKEY_PRESSED:
			if(joyState.rgbButtons[joy])
				return true;
			break;
		case DIKEY_DOWN:
			if(joyState.rgbButtons[joy] && !lastJoyState.rgbButtons[joy])
				return true;
			break;
		case DIKEY_UP:
			if(!joyState.rgbButtons[joy] && lastJoyState.rgbButtons[joy])
				return true;
			break;
		default:
			return false;
		}
	}
	else
	{
		switch(joy)
		{
		case JOY_LEFT:
			switch(stateType)
			{
			case DIKEY_PRESSED:
				if(joyState.lX < 0)
					return true;
				break;
			case DIKEY_DOWN:
				if(joyState.lX < 0 && !(lastJoyState.lX < 0))
					return true;
				break;
			case DIKEY_UP:
				if(!(joyState.lX < 0) && lastJoyState.lX < 0)
					return true;
				break;
			default:
				return false;
			}
			break;
		case JOY_RIGHT:
			switch(stateType)
			{
			case DIKEY_PRESSED:
				if(joyState.lX > 0)
					return true;
				break;
			case DIKEY_DOWN:
				if(joyState.lX > 0 && !(lastJoyState.lX > 0))
					return true;
				break;
			case DIKEY_UP:
				if(!(joyState.lX > 0) && lastJoyState.lX > 0)
					return true;
				break;
			default:
				return false;
			}
			break;
		case JOY_UP:
			switch(stateType)
			{
			case DIKEY_PRESSED:
				if(joyState.lY < 0)
					return true;
				break;
			case DIKEY_DOWN:
				if(joyState.lY < 0 && !(lastJoyState.lY < 0))
					return true;
				break;
			case DIKEY_UP:
				if(!(joyState.lY < 0) && lastJoyState.lY < 0)
					return true;
				break;
			default:
				return false;
			}
			break;
		case JOY_DOWN:
			switch(stateType)
			{
			case DIKEY_PRESSED:
				if(joyState.lY > 0)
					return true;
				break;
			case DIKEY_DOWN:
				if(joyState.lY > 0 && !(lastJoyState.lY > 0))
					return true;
				break;
			case DIKEY_UP:
				if(!(joyState.lY > 0) && lastJoyState.lY > 0)
					return true;
				break;
			default:
				return false;
			}
			break;
		default:
			return false;
		}
	}
	return false;
}
// end

void HGE_Impl::_UpdateMouse()
{
	POINT	pt;
	RECT	rc;

	GetCursorPos(&pt);
	GetClientRect(hwnd, &rc);
	MapWindowPoints(hwnd, NULL, (LPPOINT)&rc, 2);

	if(bCaptured || (PtInRect(&rc, pt) && WindowFromPoint(pt)==hwnd))
		bMouseOver=true;
	else
		bMouseOver=false;
}

void HGE_Impl::_BuildEvent(int type, int key, int scan, int flags, int x, int y)
{
	CInputEventList *last, *eptr=new CInputEventList;
	unsigned char kbstate[256];
	POINT pt;

	eptr->event.type=type;
	eptr->event.chr=0;
	pt.x=x; pt.y=y;

	GetKeyboardState(kbstate);
	if(type==INPUT_KEYDOWN)
	{
		if((flags & HGEINP_REPEAT) == 0) keyz[key] |= 1;
		ToAscii(key, scan, kbstate, (unsigned short *)&eptr->event.chr, 0);
	}
	if(type==INPUT_KEYUP)
	{
		keyz[key] |= 2;
		ToAscii(key, scan, kbstate, (unsigned short *)&eptr->event.chr, 0);
	}
	if(type==INPUT_MOUSEWHEEL)
	{
		eptr->event.key=0; eptr->event.wheel=key;
		ScreenToClient(hwnd,&pt);
	}
	else { eptr->event.key=key; eptr->event.wheel=0; }

	if(type==INPUT_MBUTTONDOWN)
	{
		keyz[key] |= 1;
		SetCapture(hwnd);
		bCaptured=true;
	}
	if(type==INPUT_MBUTTONUP)
	{
		keyz[key] |= 2;
		ReleaseCapture();
		Input_SetMousePos(Xpos, Ypos);
		pt.x=(int)Xpos; pt.y=(int)Ypos;
		bCaptured=false;
	}
	
	if(kbstate[VK_SHIFT] & 0x80) flags|=HGEINP_SHIFT;
	if(kbstate[VK_CONTROL] & 0x80) flags|=HGEINP_CTRL;
	if(kbstate[VK_MENU] & 0x80) flags|=HGEINP_ALT;
	if(kbstate[VK_CAPITAL] & 0x1) flags|=HGEINP_CAPSLOCK;
	if(kbstate[VK_SCROLL] & 0x1) flags|=HGEINP_SCROLLLOCK;
	if(kbstate[VK_NUMLOCK] & 0x1) flags|=HGEINP_NUMLOCK;
	eptr->event.flags=flags;

	if(pt.x==-1) { eptr->event.x=Xpos;eptr->event.y=Ypos; }
	else
	{
		if(pt.x<0) pt.x=0;
		if(pt.y<0) pt.y=0;
		if(pt.x>=nScreenWidth) pt.x=nScreenWidth-1;
		if(pt.y>=nScreenHeight) pt.y=nScreenHeight-1;

		eptr->event.x=(float)pt.x;
		eptr->event.y=(float)pt.y;
	}

	
	sora::SoraKeyPoll::publishInputedKey(eptr->event.key, eptr->event.type, eptr->event.chr);
	
	/*sora::SoraKeyEvent ev;
	if(eptr->event.key != 0) {
		ev.chr = eptr->event.chr;
		ev.flags = eptr->event.flags;
		ev.key = eptr->event.key;
		ev.type = eptr->event.type;
		ev.wheel = eptr->event.wheel;
		ev.x = eptr->event.x;
		ev.y = eptr->event.y;
		sora::SORA_EVENT_MANAGER->publishInputEvent(&ev);
	}

	if(!ev.isConsumed()) {
		eptr->next=0; 

		if(!queue) queue=eptr;
		else
		{
			last=queue;
			while(last->next) last=last->next;
			last->next=eptr;
		}
	}*/

	if(eptr->event.type==INPUT_KEYDOWN || eptr->event.type==INPUT_MBUTTONDOWN)
	{
		VKey=eptr->event.key;Char=eptr->event.chr;
	}
	else if(eptr->event.type==INPUT_MOUSEMOVE)
	{
		Xpos=eptr->event.x;Ypos=eptr->event.y;
	}
	else if(eptr->event.type==INPUT_MOUSEWHEEL)
	{
		Zpos+=eptr->event.wheel;
	}

	
}

void HGE_Impl::_ClearQueue()
{
	CInputEventList *nexteptr, *eptr=queue;
	
	memset(&keyz, 0, sizeof(keyz));
	
	while(eptr)
	{
		nexteptr=eptr->next;
		delete eptr;
		eptr=nexteptr;
	}

	queue=0; VKey=0; Char=0; Zpos=0;
}

/************************************************************************/
/* These functions are added by h5nc (h5nc@yahoo.com.cn)                */
/************************************************************************/
// begin
BOOL CALLBACK EnumJoysticksCallback (const DIDEVICEINSTANCE * pdidInstance, VOID* pContext)
{
 	*(GUID*)pContext = pdidInstance->guidInstance; 

	return (DIENUM_STOP);
}

LPDIRECTINPUT8 CALL HGE_Impl::Input_GetDevice()
{
	return lpDInput;
}

int HGE_Impl::_DIInit()
{
	lpDIKDevice = NULL;
	lpDIJDevice = NULL;

	bool keyable = true;
	bool joyable = true;

	if (FAILED (DirectInput8Create (hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**) &lpDInput, NULL)))
	{
		keyable = false;
//		return ERROR_NOKEYBOARD;
	}

	if (keyable && FAILED(lpDInput->CreateDevice(GUID_SysKeyboard, &lpDIKDevice, NULL)))
	{
		keyable = false;
//		return ERROR_NOKEYBOARD;
	}
	if (keyable && FAILED (lpDIKDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		keyable = false;
//		return ERROR_NOKEYBOARD;
	}
	if (keyable && FAILED (lpDIKDevice->SetCooperativeLevel (hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		keyable = false;
//		return ERROR_NOKEYBOARD;
	}
	if(keyable && FAILED(lpDIKDevice->Acquire ()))
	{
		keyable = false;
//		return ERROR_NOKEYBOARD;
	}

	if (FAILED (lpDInput->EnumDevices (DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, &joyGuid, DIEDFL_ATTACHEDONLY)))
	{
		joyable = false;
//		return ERROR_NOJOYSTICK;
	}
	if (joyable && FAILED (lpDInput->CreateDevice(joyGuid, &lpDIJDevice, NULL)))
	{
		joyable = false;
//		return ERROR_NOJOYSTICK;
	}
	if (joyable && FAILED (lpDIJDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		joyable = false;
//		return ERROR_NOJOYSTICK;
	}
	if (joyable && FAILED (lpDIJDevice->SetCooperativeLevel (hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		joyable = false;
//		return ERROR_NOJOYSTICK;
	}

	if(joyable)
	{
		DIPROPRANGE joyRange;

		joyRange.lMin = -24;
		joyRange.lMax = 24;

		joyRange.diph.dwSize       = sizeof(DIPROPRANGE); 
		joyRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		joyRange.diph.dwObj        = DIJOFS_X;
		joyRange.diph.dwHow        = DIPH_BYOFFSET;

		lpDIJDevice->SetProperty(DIPROP_RANGE, &joyRange.diph);

		joyRange.lMin = -24;
		joyRange.lMax = 24;

		joyRange.diph.dwSize       = sizeof(DIPROPRANGE); 
		joyRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		joyRange.diph.dwObj        = DIJOFS_Y;
		joyRange.diph.dwHow        = DIPH_BYOFFSET;

		lpDIJDevice->SetProperty(DIPROP_RANGE,&joyRange.diph);

		DIPROPDWORD deadZone;

		deadZone.diph.dwSize       = sizeof(deadZone);
		deadZone.diph.dwHeaderSize = sizeof(deadZone.diph);
		deadZone.diph.dwObj        = DIJOFS_X;
		deadZone.diph.dwHow        = DIPH_BYOFFSET;
		deadZone.dwData            = 1000;

		lpDIJDevice->SetProperty(DIPROP_DEADZONE, &deadZone.diph);

		deadZone.diph.dwSize       = sizeof(deadZone);
		deadZone.diph.dwHeaderSize = sizeof(deadZone.diph);
		deadZone.diph.dwObj        = DIJOFS_Y;
		deadZone.diph.dwHow        = DIPH_BYOFFSET;
		deadZone.dwData            = 1000;

		lpDIJDevice->SetProperty(DIPROP_DEADZONE,&deadZone.diph);

		if (FAILED (lpDIJDevice->Acquire()))
		{
			joyable = false;
//			return ERROR_NOJOYSTICK;
		}
	}

	return (joyable?0:ERROR_NOJOYSTICK) | (keyable?0:ERROR_NOKEYBOARD);
}

bool HGE_Impl::Input_HaveJoy()
{
	return haveJoy;
}

void HGE_Impl::_DIRelease()
{
	if(lpDIKDevice != NULL)
	{
		lpDIKDevice->Unacquire();
		lpDIKDevice->Release();
		lpDIKDevice = NULL;
	}
	if(lpDIJDevice != NULL)
	{
		lpDIJDevice->Unacquire();
		lpDIJDevice->Release();
		lpDIJDevice = NULL;
	}
}

int HGE_Impl::_DIUpdate()
{
	bool keyable = true;
	bool joyable = true;

	memcpy(lastKeyState, keyState, sizeof(keyState));

	if(lpDIKDevice == NULL)
	{
		_DIRelease();
		_DIInit();
		if(lpDIKDevice == NULL)
		{
			keyable = false;
			return ERROR_NOKEYBOARD;
		}
	}
	if(keyable)
	{
		HRESULT hRet = lpDIKDevice->GetDeviceState(256, keyState);
		if(FAILED(hRet)/* && hRet == DIERR_INPUTLOST*/)
		{
			if(FAILED(lpDIKDevice->Acquire()))
			{
				_DIRelease();
				_DIInit();
				if(FAILED(lpDIKDevice->Acquire()))
					keyable = false;
					return ERROR_NOKEYBOARD;
			}
			if(keyable && FAILED(lpDIKDevice->GetDeviceState(256, keyState)))
				keyable = false;
				return ERROR_NOKEYBOARD;
		}
		if(hwnd != GetActiveWindow())
			ZeroMemory(&keyState, sizeof(keyState));
	}

	lastJoyState = joyState;

	if(haveJoy)
	{
		if(lpDIJDevice == NULL)
		{
			_DIRelease();
			_DIInit();
			if(lpDIJDevice == NULL)
			{
				joyable = false;
				return ERROR_NOJOYSTICK;
			}
		}
		if(joyable && FAILED(lpDIJDevice->Poll()))
		{
			if(FAILED(lpDIJDevice->Acquire()))
			{
				_DIRelease();
				_DIInit();
				if(FAILED(lpDIKDevice->Acquire()))
				{
					joyable = false;
					return ERROR_NOJOYSTICK;
				}
			}
		}
		if(joyable && FAILED(lpDIJDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
		{
			joyable = false;
			return ERROR_NOJOYSTICK;
		}
	}
	else
		joyable = false;

	return (joyable?0:ERROR_NOJOYSTICK) | (keyable?0:ERROR_NOKEYBOARD);
}
