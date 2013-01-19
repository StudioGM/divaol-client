/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core system functions
*/


#include "hge_impl.h"

#pragma comment(lib,"winmm.lib")


#define LOWORDINT(n) ((int)((signed short)(LOWORD(n))))
#define HIWORDINT(n) ((int)((signed short)(HIWORD(n))))


char* wchar2char(const wchar_t* pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen<= 0) return 0;
		char* pszDst = new char[nLen];
		if (NULL == pszDst) return 0;
		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen -1] = 0;
		return pszDst;
	}

	wchar_t* char2wchar(const char* pszSrc, int nLen)
	{
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszSrc, nLen, 0, 0);
		if(nSize <= 0) return NULL;
		WCHAR *pwszDst = new WCHAR[nSize+1];
		if( NULL == pwszDst) return NULL;
		MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize);
		pwszDst[nSize] = 0;
		if( pwszDst[0] == 0xFEFF)                    // skip Oxfeff
			for(int i = 0; i < nSize; i ++)
				pwszDst[i] = pwszDst[i+1];
		return pwszDst;
	}

const wchar_t *WINDOW_CLASS_NAME = L"HGE__WNDCLASS";

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


int			nRef=0;
HGE_Impl*	pHGE=0;



BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}


HGE* CALL hgeCreate(int ver)
{
	if(ver==HGE_VERSION)
		return (HGE*)HGE_Impl::_Interface_Get();
	else
		return 0;
}


HGE_Impl* HGE_Impl::_Interface_Get()
{
	if(!pHGE) pHGE=new HGE_Impl();

	nRef++;

	return pHGE;
}


void CALL HGE_Impl::Release()
{
	nRef--;

	if(!nRef)
	{
		if(pHGE->hwnd) pHGE->System_Shutdown();
		Resource_RemoveAllPacks();
		delete pHGE;
		pHGE=0;
	}
}


bool CALL HGE_Impl::System_Initiate()
{
	OSVERSIONINFO	os_ver;
	SYSTEMTIME		tm;
	MEMORYSTATUS	mem_st;
	WNDCLASSW		winclass;
	int				width, height;

	// Log system info

//	System_Log("HGE Started..\n");

//	System_Log("HGE version: %X.%X", HGE_VERSION>>8, HGE_VERSION & 0xFF);
	GetLocalTime(&tm);
//	System_Log("Date: %02d.%02d.%d, %02d:%02d:%02d\n", tm.wDay, tm.wMonth, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond);

//	System_Log("Application: %s",szWinTitle);
	os_ver.dwOSVersionInfoSize=sizeof(os_ver);
	GetVersionEx(&os_ver);
//	System_Log("OS: Windows %ld.%ld.%ld",os_ver.dwMajorVersion,os_ver.dwMinorVersion,os_ver.dwBuildNumber);

	GlobalMemoryStatus(&mem_st);
//	System_Log("Memory: %ldK total, %ldK free\n",mem_st.dwTotalPhys/1024L,mem_st.dwAvailPhys/1024L);


	// Register window class
	
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	if(!szCursor) {
		winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		hCursor = winclass.hCursor;
	}
	else {
		winclass.hCursor		= LoadCursorFromFileA(szCursor);
		hCursor = winclass.hCursor;
	}
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	
	if(szIcon) {
		winclass.hIcon = (HICON)LoadImageA(NULL, szIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	}
	else winclass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	
	if (!RegisterClassW(&winclass)) {
		_PostError("Can't register window class");
		return false;
	}

	//new add by hyf
	if(bFakeFullScreen)
	{
		// Create window

		width=nScreenWidth;
		height=nScreenHeight;

		rectW.left=0;
		rectW.top=0;
		rectW.right=rectW.left+width;
		rectW.bottom=rectW.top+height;
		styleW=WS_POPUP|WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
	}
	else
	{
		styleW=WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;

		RECT rect = {0,0,nScreenWidth,nScreenHeight};
		AdjustWindowRectEx(&rect, styleW, false, 0);

		// Create window
		width=rect.right-rect.left;
		height=rect.bottom-rect.top;

		rectW.left=(GetSystemMetrics(SM_CXSCREEN)-width)/2;
		rectW.top=(GetSystemMetrics(SM_CYSCREEN)-height)/2;
		rectW.right=rectW.left+width;
		rectW.bottom=rectW.top+height;
	}

	rectFS.left=0;
	rectFS.top=0;
	rectFS.right=nScreenWidth;
	rectFS.bottom=nScreenHeight;
	styleFS=WS_POPUP|WS_VISIBLE; //WS_POPUP

	if(hwndParent)
	{
		rectW.left=0;
		rectW.top=0;
		rectW.right=nScreenWidth;
		rectW.bottom=nScreenHeight;
		styleW=WS_CHILD|WS_VISIBLE; 
		bWindowed=true;
	}
	
	wchar_t* pwstrWinTitle = char2wchar(szWinTitle, -1);
	if(bWindowed)
		hwnd = CreateWindowExW(0, WINDOW_CLASS_NAME, pwstrWinTitle, styleW,
				rectW.left, rectW.top, rectW.right-rectW.left, rectW.bottom-rectW.top,
				hwndParent, NULL, hInstance, NULL);
	else
		hwnd = CreateWindowExW(WS_EX_TOPMOST, WINDOW_CLASS_NAME, pwstrWinTitle, styleFS,
				0, 0, 0, 0,
				NULL, NULL, hInstance, NULL);
	free(pwstrWinTitle);
	if (!hwnd)
	{
		_PostError("Can't create window");
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);

	switch(_DIInit())
	{
	case ERROR_NOKEYBOARD:
	case ERROR_NOKEYBOARD | ERROR_NOJOYSTICK:
		System_Shutdown();
		return false;
	case ERROR_NOJOYSTICK:
		haveJoy = false;
		break;
	}

	// Init subsystems

	//timeBeginPeriod(1);
	_InputInit();
	if(!_GfxInit()) { System_Shutdown(); return false; }

//	System_Log("Init done.\n");

	// Done

	return true;
}

void CALL HGE_Impl::System_Shutdown()
{
//	System_Log("\nFinishing..");

	timeEndPeriod(1);
	if(hSearch) { FindClose(hSearch); hSearch=0; }
	_ClearQueue();

	_GfxDone();
//	_DonePowerStatus();

	

	if(hwnd)
	{
		_DIRelease();
		ShowWindow(hwnd, SW_HIDE);
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
		ShowWindow(hwnd, SW_SHOW);
		DestroyWindow(hwnd);
		hwnd=0;
	}

	if(hInstance) UnregisterClassW(WINDOW_CLASS_NAME, hInstance);

//	System_Log("The End.");
}


bool CALL HGE_Impl::System_Start()
{
	MSG		msg;

	if(!hwnd)
	{
		_PostError("System_Start: System_Initiate wasn't called");
		return false;
	}

	if(!procFrameFunc) {
		_PostError("System_Start: No frame function defined");
		return false;
	}

	bActive=true;

	return true;
}

void CALL HGE_Impl::System_SetStateBool(hgeBoolState state, bool value)
{
	switch(state)
	{
		//new add by hyf
		case HGE_FAKE_FULLSCREEN:  
								if(pD3DDevice && bFakeFullScreen != value)
								{
									bFakeFullScreen = value;
									_Resize(nScreenWidth, nScreenHeight, true);
									_GfxRestore();
									_AdjustWindow();
								}
								else
									bFakeFullScreen = value;
								break;

		case HGE_WINDOWED:		if(VertArray || hwndParent) break;
								if(pD3DDevice && bWindowed != value)
								{
									if(d3dppW.BackBufferFormat==D3DFMT_UNKNOWN || d3dppFS.BackBufferFormat==D3DFMT_UNKNOWN) break;

									if(bWindowed) GetWindowRect(hwnd, &rectW);
									bWindowed=value;
									if(bWindowed) d3dpp=&d3dppW;
									else d3dpp=&d3dppFS;

									if(_format_id(d3dpp->BackBufferFormat) < 4) nScreenBPP=16;
									else nScreenBPP=32;

									_GfxRestore();
									_AdjustWindow();
								}
								else bWindowed=value;
								break;

		case HGE_ZBUFFER:		if(!pD3DDevice)	bZBuffer=value;
								break;

		case HGE_TEXTUREFILTER: bTextureFilter=value;
								if(pD3DDevice)
								{
									_render_batch();
									if(bTextureFilter)//*****
									{
										//pD3DDevice->SetTextureStageState(0, /*D3DTSS_MAGFILTER*/,D3DTEXF_LINEAR);
										//pD3DDevice->SetTextureStageState(0,D3DSAMP_MINFILTER/*D3DTSS_MINFILTER*/,D3DTEXF_LINEAR);//***
										//pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
										//pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

										pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
										pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

									}
									else
									{
										//pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
										//pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

										pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
										pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );

										 
										//pD3DDevice->SetTextureStageState(0, /*D3DTSS_MAGFILTER*/,D3DTEXF_POINT);
										//pD3DDevice->SetTextureStageState(0,D3DSAMP_MINFILTER/*D3DTSS_MINFILTER*/,D3DTEXF_POINT);
									}
								}
								break;

		case HGE_USESOUND:		break;

		case HGE_HIDEMOUSE:		
			bHideMouse=value; 
			if(pHGE->bActive && pHGE->bHideMouse) SetCursor(NULL);
			else {
				SetCursor(pHGE->hCursor);
			}
			break;

		case HGE_DONTSUSPEND:	bDontSuspend=value; break;

		#ifdef DEMO
		case HGE_SHOWSPLASH:	bDMO=value; break;
		#endif
	}
}

void CALL HGE_Impl::System_SetStateFunc(hgeFuncState state, hgeCallback value)
{
	switch(state)
	{
		case HGE_FRAMEFUNC:		 procFrameFunc=value; break;
		case HGE_RENDERFUNC:	 procRenderFunc=value; break;
		case HGE_FOCUSLOSTFUNC:	 procFocusLostFunc=value; break;
		case HGE_FOCUSGAINFUNC:	 procFocusGainFunc=value; break;
		case HGE_GFXRESTOREFUNC: procGfxRestoreFunc=value; break;
		case HGE_EXITFUNC:		 procExitFunc=value; break;
	}
}

void CALL HGE_Impl::System_SetStateHwnd(hgeHwndState state, HWND value)
{
	switch(state)
	{
		case HGE_HWNDPARENT:	if(!hwnd) hwndParent=value; break;
	}
}

void CALL HGE_Impl::System_SetStateInt(hgeIntState state, int value)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	if(!pD3DDevice) nScreenWidth=value; break;

		case HGE_SCREENHEIGHT:	if(!pD3DDevice) nScreenHeight=value; break;

		case HGE_SCREENBPP:		if(!pD3DDevice) nScreenBPP=value; break;

		case HGE_SAMPLERATE:	if(!hBass) nSampleRate=value;
								break;

		case HGE_FSAA:	nFSAA = value; break;

		case HGE_FXVOLUME:	
								break;

		case HGE_MUSVOLUME:		break;

		case HGE_FPS:			if(VertArray) break;

								if(pD3DDevice)
								{
									if((nHGEFPS>=0 && value <0) || (nHGEFPS<0 && value>=0))
									{
										if(value==HGEFPS_VSYNC)
										{
											d3dppW.SwapEffect = D3DSWAPEFFECT_COPY/*_VSYNC*/;//*** because  D3DSWAPEFFECT_COPY_VSYNC have been removed 
											d3dppW.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
											d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_ONE;//***
										}
										else
										{
											d3dppW.SwapEffect = D3DSWAPEFFECT_COPY;
											d3dppW.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
											d3dppFS.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//***
										}
										if(procFocusLostFunc) procFocusLostFunc( );
										_GfxRestore();
										if(procFocusGainFunc) procFocusGainFunc( );
									}
								}
								nHGEFPS=value;
								break;
	}
}

void CALL HGE_Impl::System_SetStateString(hgeStringState state, const char *value)
{
	FILE *hf;
	
	switch(state)
	{
		case HGE_ICON:			szIcon=value;
								if(pHGE->hwnd) SetClassLong(pHGE->hwnd, GCL_HICON, (LONG)(HICON)LoadImageA(NULL, szIcon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE));
								break;
		case HGE_CURSOR:
								szCursor=value;
								if(pHGE->hwnd) {
									hCursor = LoadCursorFromFileA(szCursor);
									SetClassLong(pHGE->hwnd, GCL_HCURSOR, (LONG)hCursor);
								}
								break;

		case HGE_TITLE:			strcpy(szWinTitle,value);
								if(pHGE->hwnd) SetWindowTextA(pHGE->hwnd, szWinTitle);
								break;
		case HGE_INIFILE:		if(value) strcpy(szIniFile,Resource_MakePath(value));
								else szIniFile[0]=0;
								break;
		case HGE_LOGFILE:		if(value)
								{
									strcpy(szLogFile,Resource_MakePath(value));
									hf=fopen(szLogFile, "w");
									if(!hf) szLogFile[0]=0;
									else fclose(hf);
								}
								else szLogFile[0]=0;
								break;
	}
}

bool CALL HGE_Impl::System_GetStateBool(hgeBoolState state)
{
	switch(state)
	{
		case HGE_FAKE_FULLSCREEN: return bFakeFullScreen;
		case HGE_WINDOWED:		return bWindowed;
		case HGE_ZBUFFER:		return bZBuffer;
		case HGE_TEXTUREFILTER:	return bTextureFilter;
		case HGE_USESOUND:		return bUseSound;
		case HGE_DONTSUSPEND:	return bDontSuspend;
		case HGE_HIDEMOUSE:		return bHideMouse;

		case HGE_ACTIVE:		return bActive;

		#ifdef DEMO
		case HGE_SHOWSPLASH:	return bDMO;
		#endif
	}

	return false;
}

hgeCallback CALL HGE_Impl::System_GetStateFunc(hgeFuncState state)
{
	switch(state)
	{
		case HGE_FRAMEFUNC:		return procFrameFunc;
		case HGE_RENDERFUNC:	return procRenderFunc;
		case HGE_FOCUSLOSTFUNC:	return procFocusLostFunc;
		case HGE_FOCUSGAINFUNC:	return procFocusGainFunc;
		case HGE_EXITFUNC:		return procExitFunc;
	}

	return NULL;
}

HWND CALL HGE_Impl::System_GetStateHwnd(hgeHwndState state)
{
	switch(state)
	{
		case HGE_HWND:			return hwnd;
		case HGE_HWNDPARENT:	return hwndParent;
	}

	return 0;
}

int CALL HGE_Impl::System_GetStateInt(hgeIntState state)
{
	switch(state)
	{
		case HGE_SCREENWIDTH:	return nScreenWidth;
		case HGE_SCREENHEIGHT:	return nScreenHeight;
		case HGE_SCREENBPP:		return nScreenBPP;
		case HGE_SAMPLERATE:	return nSampleRate;
		case HGE_FXVOLUME:		return nFXVolume;
		case HGE_MUSVOLUME:		return nMusVolume;
		case HGE_FPS:			return nHGEFPS;
		case HGE_POWERSTATUS:	return nPowerStatus;
	}

	return 0;
}

const char* CALL HGE_Impl::System_GetStateString(hgeStringState state) {
	switch(state) {
		case HGE_ICON:			return szIcon;
		case HGE_TITLE:			return szWinTitle;
		case HGE_INIFILE:		if(szIniFile[0]) return szIniFile;
								else return 0;
		case HGE_LOGFILE:		if(szLogFile[0]) return szLogFile;
								else return 0;
	}

	return NULL;
}

char* CALL HGE_Impl::System_GetErrorMessage()
{
	return szError;
}

void CALL HGE_Impl::System_Log(const char *szFormat, ...)
{
	FILE *hf = NULL;
	va_list ap;
	
	if(!szLogFile[0]) return;

	hf = fopen(szLogFile, "a");
	if(!hf) return;

	va_start(ap, szFormat);
	vfprintf(hf, szFormat, ap);
	va_end(ap);

	fprintf(hf, "\n");

	fclose(hf);
}

bool CALL HGE_Impl::System_Launch(const char *url)
{
	if((DWORD)ShellExecuteA(pHGE->hwnd, NULL, url, NULL, NULL, SW_SHOWMAXIMIZED)>32) return true;
	else return false;
}

void CALL HGE_Impl::System_Snapshot(const char *filename)
{
	LPDIRECT3DSURFACE9 pSurf;
	char *shotname, tempname[_MAX_PATH];
	int i;

	if(!filename)
	{
		i=0;
		shotname=Resource_EnumFiles("shot???.bmp");
		while(shotname)
		{
			i++;
			shotname=Resource_EnumFiles();
		}
		sprintf(tempname, "shot%03d.bmp", i);
		filename=Resource_MakePath(tempname);
	}

	if(pD3DDevice)
	{
		pD3DDevice->GetBackBuffer(0,/* */0, D3DBACKBUFFER_TYPE_MONO, &pSurf);//*** get the first buffer
		D3DXSaveSurfaceToFileA(filename, D3DXIFF_BMP, pSurf, NULL, NULL);
		pSurf->Release();
	}
}

//////// Implementation ////////


HGE_Impl::HGE_Impl()
{
	hInstance=GetModuleHandle(0);
	hwnd=0;
	bActive=false;
	szError[0]=0;

	pD3D=0;
	pD3DDevice=0;
	d3dpp=0;
	pTargets=0;
	pCurTarget=0;
	pScreenSurf=0;
	pScreenDepth=0;
	pVB=0;
	pIB=0;
	VertArray=0;
	textures=0;
	nFSAA = 0;

	hBass=0;
	bSilent=false;
	streams=0;

	hSearch=0;
	res=0;

	queue=0;
	Char=VKey=Zpos=0;
	Xpos=Ypos=0.0f;
	bMouseOver=false;
	bCaptured=false;

	nHGEFPS=HGEFPS_UNLIMITED;
	
	procFrameFunc=0;
	procRenderFunc=0;
	procFocusLostFunc=0;
	procFocusGainFunc=0;
	procGfxRestoreFunc=0;
	procExitFunc=0;
	szIcon=0;
	strcpy(szWinTitle,"HGE");
	nScreenWidth=800;
	nScreenHeight=600;
	nScreenBPP=32;
	bWindowed=false;
	bZBuffer=false;
	bTextureFilter=true;
	szLogFile[0]=0;
	szIniFile[0]=0;
	bUseSound=true;
	nSampleRate=44100;
	nFXVolume=100;
	nMusVolume=100;
	bHideMouse=true;
	bDontSuspend=false;
	hwndParent=0;
	szCursor = NULL;
	hCursor = NULL;

	nPowerStatus=HGEPWR_UNSUPPORTED;
	hKrnl32 = NULL;
	lpfnGetSystemPowerStatus = NULL;

#ifdef DEMO
	bDMO=true;
#endif


	GetModuleFileNameA(GetModuleHandle(NULL), szAppPath, sizeof(szAppPath));
	int i;
	for(i=strlen(szAppPath)-1; i>0; i--) if(szAppPath[i]=='\\') break;
	szAppPath[i+1]=0;
}

void HGE_Impl::_PostError(char *error)
{
	System_Log(error);
	strcpy(szError,error);
}

void HGE_Impl::_FocusChange(bool bAct)
{
	bActive=bAct;

	if(bActive) {
		_GfxRestore();
		if(procFocusGainFunc) procFocusGainFunc( );
		_DIInit();
	}
	else
	{
		if(procFocusLostFunc) procFocusLostFunc( );
		_DIRelease();
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	bool bActivating;

	switch(msg)
	{	
		case WM_CREATE: 
			return FALSE;
		
		case WM_PAINT:
			if(pHGE->pD3D && pHGE->procRenderFunc && pHGE->bWindowed) pHGE->procRenderFunc();
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return FALSE;

/*
		case WM_ACTIVATEAPP:
			bActivating = (wparam == TRUE);
			if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
			return FALSE;
*/
		case WM_ACTIVATE:
			// tricky: we should catch WA_ACTIVE and WA_CLICKACTIVE,
			// but only if HIWORD(wParam) (fMinimized) == FALSE (0)
			bActivating = (LOWORD(wparam) != WA_INACTIVE) && (HIWORD(wparam) == 0);
			if(pHGE->pD3D && pHGE->bActive != bActivating) pHGE->_FocusChange(bActivating);
			return FALSE;


		case WM_SETCURSOR:
			if(pHGE->bActive && LOWORD(lparam)==HTCLIENT && pHGE->bHideMouse) SetCursor(NULL);
			else {
				SetCursor(pHGE->hCursor);
			}
			return FALSE;

		case WM_SYSKEYDOWN:
			if(wparam == VK_F4)
			{
				if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
	//		else if(wparam == VK_RETURN)
	//		{
	//			pHGE->System_SetState(HGE_WINDOWED, !pHGE->System_GetState(HGE_WINDOWED));
	//			return FALSE;
	//		}
			else
			{
				pHGE->_BuildEvent(INPUT_KEYDOWN, wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT:0, -1, -1);
				return FALSE;
			}

		case WM_KEYDOWN:
			pHGE->_BuildEvent(INPUT_KEYDOWN, wparam, HIWORD(lparam) & 0xFF, (lparam & 0x40000000) ? HGEINP_REPEAT:0, -1, -1);
			return FALSE;
		case WM_SYSKEYUP:
			pHGE->_BuildEvent(INPUT_KEYUP, wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
			return FALSE;
		case WM_KEYUP:
			pHGE->_BuildEvent(INPUT_KEYUP, wparam, HIWORD(lparam) & 0xFF, 0, -1, -1);
			return FALSE;

		case WM_LBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONDOWN:
			SetFocus(hwnd);
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_LBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_LBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_MBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONDBLCLK:
			pHGE->_BuildEvent(INPUT_MBUTTONDOWN, HGEK_RBUTTON, 0, HGEINP_REPEAT, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_LBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_LBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_MBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_MBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case WM_RBUTTONUP:
			pHGE->_BuildEvent(INPUT_MBUTTONUP, HGEK_RBUTTON, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_MOUSEMOVE:
			pHGE->_BuildEvent(INPUT_MOUSEMOVE, 0, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;
		case 0x020A: // WM_MOUSEWHEEL, GET_WHEEL_DELTA_WPARAM(wparam);
			pHGE->_BuildEvent(INPUT_MOUSEWHEEL, short(HIWORD(wparam))/120, 0, 0, LOWORDINT(lparam), HIWORDINT(lparam));
			return FALSE;

		case WM_SIZE:
			if(pHGE->pD3D && wparam==SIZE_RESTORED) pHGE->_Resize(LOWORD(lparam), HIWORD(lparam));
			//return FALSE;
			break;

		case WM_SYSCOMMAND:
			if(wparam==SC_CLOSE)
			{
				if(pHGE->procExitFunc && !pHGE->procExitFunc()) return FALSE;
				pHGE->bActive=false;
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

