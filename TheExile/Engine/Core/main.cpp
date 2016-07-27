#pragma once
//#define NDEBUG
#include "../Renderer/Renderer.h"
#include "../Core/State Machine/StateMachine.h"
#include <stdio.h>
#include <DbgHelp.h>
#include "../Asset Manager/AssetManager.h"
#pragma comment(lib, "dbghelp.lib")


#define NICK_STATE 0
#include "State Machine\States\NickState.h"

#if _DEBUG
//#include "vld.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
// Global Intializations //
HWND hWindow;
bool bInverse = false;
int gCustomWidth;
int gCustomHeight;
int gScreenWidth;
int gScreenHeight;
int gWindowWidth;
int gWindowHeight;

CStateMachine * g_cStateMachine;
LONG WINAPI CrashDumpToFile(_EXCEPTION_POINTERS* pExceptionInfo);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_ESCAPE && NICK_STATE)
		message = WM_DESTROY;

	switch (message)
	{
	case WM_QUIT:
		message = WM_DESTROY;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		#if !NICK_STATE
			g_cStateMachine->Exit();
		#endif

		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int cmdShow)
{
	//Dump File
	#if !NICK_STATE
		SetUnhandledExceptionFilter(CrashDumpToFile);
	#endif

	// Memory Leak Detection //
	#if _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetBreakAlloc(-1);
	#endif

	// Window Initialization //
	MSG message;
	RECT rScreen;
	WNDCLASSEX windowClass;

	// Grab Screen Resolution //
	GetWindowRect(GetDesktopWindow(), &rScreen);
	gScreenWidth = rScreen.right;
	gScreenHeight = rScreen.bottom;

	gWindowWidth = gCustomWidth = DEFAULT_WINDOWED_WIDTH;
	gWindowHeight = gCustomHeight = DEFAULT_WINDOWED_HEIGHT;

	// Fill Window Properties //
	ZeroMemory(&message, sizeof(message));
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(107));
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = MAKEINTRESOURCE(107);
	windowClass.lpszClassName = L"The Exile";

	RegisterClassEx(&windowClass);

	STICKYKEYS wStickyKeys;
	wStickyKeys.cbSize = sizeof(STICKYKEYS);
	wStickyKeys.dwFlags = NULL;
	HRESULT result = SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &wStickyKeys, NULL);

	// Create Window //
	hWindow = CreateWindow(L"The Exile", L"The Exile", WINDOW_STYLE,
						   WINDOW_START_X, WINDOW_START_Y, WINDOW_WIDTH , WINDOW_HEIGHT, 
						   NULL, NULL, hInstance, NULL);

	if (!hWindow) // If creation of window fails
		return false;

	ShowWindow(hWindow, cmdShow);
	UpdateWindow(hWindow);

	// Intialize Global Classes //
	//CAssetManager::GetInstance();
	CTime cTime;
	CRenderer cRenderer(hWindow);

	// Main Loop /
	bool Alive = true;
	float wakeupTime = 0;
	float sleepOffset = 1; // Because Sleep is so inaccurate // Thanks Obama
	srand((unsigned int)(CURRENT_TIME()));

	// Set Startup State //
	#if NICK_STATE
		CNickState cNickState;
		cNickState.Enter();
		GRAPHICS->GetDebugCamera() = true;
	#else
		g_cStateMachine = CStateMachine::GetInstance();
		//g_cStateMachine->GiveAlive(&Alive);
		g_cStateMachine->PushState(eMAINMENU);
		ShowCursor(false);
	#endif

	while (Alive && message.message != WM_QUIT)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			// Game Update //
			TICK();

			#if NICK_STATE
				cNickState.Input();
				cNickState.Update();
				GRAPHICS->Update();
			#else
			if (g_cStateMachine->Update())
				GRAPHICS->Update();
			else if (g_cStateMachine->IsEmpty())
				Alive = false;

			#endif

		}

		// FPS Cap // - Prevents unnecessarily burning up CPU time
		#ifdef FPS
		
		float currentTime = CURRENT_TIME();

		if (currentTime <= wakeupTime) // Are we early for the next frame
			Sleep(DWORD(((wakeupTime - currentTime) * 1000.0f) + sleepOffset)); // Sleep until next frame

		wakeupTime = CURRENT_TIME() + FRAME_TIME;

		#endif
	}

#if NICK_STATE
	cNickState.Exit();
#endif
	#if !NICK_STATE
		g_cStateMachine->DeleteInstance();
	#endif

#if _DEBUG && !NICK_STATE
	//_CrtDumpMemoryLeaks();
		
#endif
	return 0;
}
LONG WINAPI CrashDumpToFile(_EXCEPTION_POINTERS* pExceptionInfo)
{
	HANDLE hFile = ::CreateFile(L"dumpfile.mdmp", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);

		::CloseHandle(hFile);
	}

	return 0;
}