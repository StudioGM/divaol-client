/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Common core implementation header
*/


#ifndef HGE_IMPL_H
#define HGE_IMPL_H

#include "hge.h"
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
//#include <d3dx8.h>

#define DEMO

#define D3DFVF_HGEVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define VERTEX_BUFFER_SIZE 4000


typedef BOOL (WINAPI *GetSystemPowerStatusFunc)(LPSYSTEM_POWER_STATUS);


struct CRenderTargetList
{
	int					width;
	int					height;
	IDirect3DTexture9*	pTex;
	IDirect3DSurface9*	pDepth;
	CRenderTargetList*	next;
};

struct CTextureList
{
	HTEXTURE			tex;
	int					width;
	int					height;
	CTextureList*		next;
};

struct CResourceList
{
	char				filename[_MAX_PATH];
	char				password[64];
	CResourceList*		next;
};

struct CStreamList
{
	HSTREAM				hstream;
	void*				data;
	CStreamList*		next;
};

struct CInputEventList
{
	hgeInputEvent		event;
	CInputEventList*	next;
};


void DInit();
void DDone();
bool DFrame();


/*
** HGE Interface implementation
*/
class HGE_Impl : public HGE
{
public:
	virtual	void		CALL	Release();

	virtual bool		CALL	System_Initiate();
	virtual void		CALL	System_Shutdown();
	virtual bool		CALL	System_Start();
	virtual void		CALL	System_SetStateBool  (hgeBoolState   state, bool        value);
	virtual void		CALL	System_SetStateFunc  (hgeFuncState   state, hgeCallback value);
	virtual void		CALL	System_SetStateHwnd  (hgeHwndState   state, HWND        value);
	virtual void		CALL	System_SetStateInt   (hgeIntState    state, int         value);
	virtual void		CALL	System_SetStateString(hgeStringState state, const char *value);
	virtual bool		CALL	System_GetStateBool  (hgeBoolState  );
	virtual hgeCallback	CALL	System_GetStateFunc  (hgeFuncState  );
	virtual HWND		CALL	System_GetStateHwnd  (hgeHwndState  );
	virtual int			CALL	System_GetStateInt   (hgeIntState   );
	virtual const char*	CALL	System_GetStateString(hgeStringState);
	virtual char*		CALL	System_GetErrorMessage();
	virtual	void		CALL	System_Log(const char *format, ...);
	virtual bool		CALL	System_Launch(const char *url);
	virtual void		CALL	System_Snapshot(const char *filename=0);

	virtual void		CALL	Input_GetMousePos(float *x, float *y);
	virtual void		CALL	Input_SetMousePos(float x, float y);
	virtual int			CALL	Input_GetMouseWheel();
	virtual bool		CALL	Input_IsMouseOver();
	virtual bool		CALL	Input_KeyDown(int key);
	virtual bool		CALL	Input_KeyUp(int key);
	virtual bool		CALL	Input_GetKeyState(int key);
	virtual char*		CALL	Input_GetKeyName(int key);
	virtual int			CALL	Input_GetKey();
	virtual int			CALL	Input_GetChar();
	virtual bool		CALL	Input_GetEvent(hgeInputEvent *event);

	virtual LPDIRECTINPUT8 CALL Input_GetDevice();
	virtual bool		CALL	Input_GetDIKey(int key, BYTE stateType = DIKEY_PRESSED);
	virtual bool		CALL	Input_SetDIKey(int key, bool set = true);
	virtual bool		CALL	Input_GetDIJoy(int joy, BYTE stateType = DIKEY_PRESSED);
	virtual bool		CALL	Input_HaveJoy();

	virtual bool		CALL	Gfx_BeginScene(HTARGET target=0);
	virtual void		CALL	Gfx_EndScene();
	virtual void		CALL	Gfx_Clear(DWORD color);
	virtual void		CALL	Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color=0xFFFFFFFF, float z=0.5f);
	virtual void		CALL	Gfx_RenderTriple(const hgeTriple *triple);
	virtual void		CALL	Gfx_RenderQuad(const hgeQuad *quad);
	virtual hgeVertex*	CALL	Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim);
	virtual void		CALL	Gfx_FinishBatch(int nprim);
	virtual void		CALL	Gfx_SetClipping(int x=0, int y=0, int w=0, int h=0);
	virtual void		CALL	Gfx_SetTransform(float x=0, float y=0, float dx=0, float dy=0, float rot=0, float hscale=0, float vscale=0); 
	virtual void		CALL	Gfx_SetTransformMatrix(const float* mat);
	virtual float*		CALL	Gfx_GetTransformMatrix() const;
	virtual void		CALL	Gfx_MultTransformMatrix(const float* mat);

	virtual void		CALL	Gfx_SetProjectionMatrix(const float* mat);
	virtual float*		CALL	Gfx_GetProjectionMatrix() const;

	virtual void		CALL	Gfx_SetRenderStateParam(int state, int param);
	virtual int			CALL	Gfx_GetRenderStateParam(int state);
	
	virtual void*		CALL	Resource_Load(const char *filename, DWORD *size=0);
	virtual void		CALL	Resource_Free(void *res);
	virtual bool		CALL	Resource_AttachPack(const char *filename, const char *password=0);
	virtual void		CALL	Resource_RemovePack(const char *filename);
	virtual void		CALL	Resource_RemoveAllPacks();
	virtual char*		CALL	Resource_MakePath(const char *filename=0);
	virtual char*		CALL	Resource_EnumFiles(const char *wildcard=0);
	virtual char*		CALL	Resource_EnumFolders(const char *wildcard=0);

	virtual HSHADER		CALL	Shader_Create(const char *filename);
	virtual void		CALL	Shader_Free(HSHADER shader);
	virtual void		CALL	Gfx_SetShader(HSHADER shader);

	virtual HTARGET		CALL	Target_Create(int width, int height, bool zbuffer);
	virtual void		CALL	Target_Free(HTARGET target);
	virtual HTEXTURE	CALL	Target_GetTexture(HTARGET target);

	virtual HTEXTURE	CALL	Texture_Create(int width, int height);
	virtual HTEXTURE	CALL	Texture_Load(const char *filename, DWORD size=0, bool bMipmap=false);
	virtual void		CALL	Texture_Free(HTEXTURE tex);
	virtual int			CALL	Texture_GetWidth(HTEXTURE tex, bool bOriginal=false);
	virtual int			CALL	Texture_GetHeight(HTEXTURE tex, bool bOriginal=false);
	virtual DWORD*		CALL	Texture_Lock(HTEXTURE tex, bool bReadOnly=true, int left=0, int top=0, int width=0, int height=0);
	virtual void		CALL	Texture_Unlock(HTEXTURE tex);

	virtual void       CALL   Gfx_RenderBuffer(DWORD vertex, DWORD indieces, HTEXTURE tex, int mode, int vsize, int isize);

	virtual unsigned long CALL getVideoDeviceHandle();
	virtual std::string CALL getDeviceInfo();
	virtual void	CALL SetShaderChanged(bool flag) { shaderChanged = flag; }
	//////// Implementation ////////

	static HGE_Impl*	_Interface_Get();
	void				_FocusChange(bool bAct);
	void				_PostError(char *error);

	void CALL enableFSAA();
	void CALL disableFSAA();


	HINSTANCE			hInstance;
	HWND				hwnd;

	bool				bActive;
	char				szError[256];
	char				szAppPath[_MAX_PATH];
	char				szIniString[256];


	// System States
	bool				(*procFrameFunc)();
	bool				(*procRenderFunc)();
	bool				(*procFocusLostFunc)();
	bool				(*procFocusGainFunc)();
	bool				(*procGfxRestoreFunc)();
	bool				(*procExitFunc)();
	
	const char*			szIcon;
	const char*			szCursor;
	char				szWinTitle[256];
	int					nScreenWidth;
	int					nScreenHeight;
	int					nScreenBPP;
	bool				bFakeFullScreen; //new add by hyf
	bool				bWindowed;
	bool				bZBuffer;
	bool				bTextureFilter;
	char				szIniFile[_MAX_PATH];
	char				szLogFile[_MAX_PATH];
	bool				bUseSound;
	int					nSampleRate;
	int					nFXVolume;
	int					nMusVolume;
	int					nHGEFPS;
	bool				bHideMouse;
	bool				bDontSuspend;
	HWND				hwndParent;

	HCURSOR				hCursor;
	int					nFSAA;

	#ifdef DEMO
	bool				bDMO;
	#endif


	// Power
	int							nPowerStatus;
	HMODULE						hKrnl32;
	GetSystemPowerStatusFunc	lpfnGetSystemPowerStatus;


	// Graphics
	D3DPRESENT_PARAMETERS*  d3dpp;

	D3DPRESENT_PARAMETERS   d3dppW;
	RECT					rectW;
	LONG					styleW;

	D3DPRESENT_PARAMETERS   d3dppFS;
	RECT					rectFS;
	LONG					styleFS;

	IDirect3D9*				pD3D;
	
	IDirect3DVertexBuffer9*	pVB;
	IDirect3DIndexBuffer9*	pIB;

	IDirect3DSurface9*	pScreenSurf;
	IDirect3DSurface9*	pScreenDepth;
	CRenderTargetList*	pTargets;
	CRenderTargetList*	pCurTarget;

	D3DXMATRIX			matView;
	D3DXMATRIX			matProj;

	CTextureList*		textures;
	hgeVertex*			VertArray;

	int					nPrim;
	int					CurPrimType;
	int					CurBlendMode;
	HTEXTURE			CurTexture;
	HSHADER				CurShader;



	//LPD3DXSPRITE       pSprite;

	bool				_GfxInit();
	void				_GfxDone();
	bool				_GfxRestore();
	void		CALL		_AdjustWindow();
	void		CALL		_Resize(int width, int height, bool force = false);
	bool				_init_lost();
	void				CALL _render_batch(bool bEndScene=false);
	int					_format_id(D3DFORMAT fmt);
	void				_SetBlendMode(int blend);
	void				_SetProjectionMatrix(int width, int height);
	void		CALL		ResetProjectionMatrix(float minz, float maxz);

	// Audio
	HINSTANCE			hBass;
	bool				bSilent;
	CStreamList*		streams;


	// Input
	int					VKey;
	int					Char;
	int					Zpos;
	float				Xpos;
	float				Ypos;
	bool				bMouseOver;
	bool				bCaptured;
	char				keyz[256];
	CInputEventList*	queue;
	void			CALL	_UpdateMouse();
	void			CALL	_InputInit();
	void			CALL	_ClearQueue();
	void				_BuildEvent(int type, int key, int scan, int flags, int x, int y);

	#define	ERROR_NOKEYBOARD	0x10
	#define ERROR_NOJOYSTICK	0x20

	BYTE				keyState[256];
	BYTE				lastKeyState[256];
	LPDIRECTINPUT8		lpDInput;
	LPDIRECTINPUTDEVICE8 lpDIKDevice;
	GUID				joyGuid; 
	LPDIRECTINPUTDEVICE8 lpDIJDevice;
	DIJOYSTATE          joyState;
	DIJOYSTATE          lastJoyState;
	bool				haveJoy;

	std::string			deviceInfo;

	int					_DIInit();
	void				_DIRelease();
	int					_DIUpdate();


	// Resources
	char				szTmpFilename[_MAX_PATH];
	CResourceList*		res;
	HANDLE				hSearch;
	WIN32_FIND_DATAW	SearchData;


	bool				shaderChanged;
	
	unsigned int get;


private:
	HGE_Impl();
};

extern HGE_Impl*		pHGE;


char* wchar2char(const wchar_t* pwszSrc);

wchar_t* char2wchar(const char* pszSrc, int nLen);

#endif

