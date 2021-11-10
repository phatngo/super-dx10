#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include "Utils.h"



#define DIRECTINPUT_VERSION 0x0800
#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 256
#include <dinput.h>

#include "Scence.h"


using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256




class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state

	LPD3DX10SPRITE spriteObject;						// Sprite handling object, BIG MYSTERY: it has to be in this place OR will lead to access violation in D3D11.dll ????

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[KEYBOARD_STATE_SIZE];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	HINSTANCE hInstance;

	ID3D10SamplerState* pPointSamplerState;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	int screen_width = 272;
	int screen_height = 256;
	int pre_scene = -1;
	

public:

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd, HINSTANCE hInstance);
	void CGame::Draw(float x, float y, LPTEXTURE tex, int left, int top, int right, int bottom, float alpha = 1.0f)
	{
		RECT r;
		r.left = left;
		r.top = top;
		r.right = right;
		r.bottom = bottom;
		this->Draw(x, y, tex, &r, alpha);

	}
	void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f);
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	ID3D10Device* GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView; }
	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }
	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };
	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }
	void SetPointSamplerState();

	LPTEXTURE LoadTexture(LPCWSTR texturePath);


	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() {
		return scenes[current_scene];
	}
	void SwitchScene(int scene_id);
	void SwitchExtraScene(int scene_id, float start_x, float start_y, bool pipeUp);
	void SwitchBackToOldScene(int scene_id, float start_x, float start_y, bool pipeUp);

	int GetScreenWidth() { return SCREEN_WIDTH; }
	int GetScreenHeight() { return SCREEN_HEIGHT; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	static CGame* GetInstance();

	~CGame();


	//Added By Myself
	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	float GetCamPosX() { return cam_x; }
	float GetCamPosY() { return cam_y; }
};