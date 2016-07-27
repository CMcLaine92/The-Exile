#pragma once
#ifdef AK_WIN
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#endif
#define WIN32_LEAN_AND_MEAN

#include <array>
#include <string>
#include <vector>
#include <d3d11.h>
#include <Windows.h>
#include <typeindex>
#include <directxmath.h>

#include "Time.h"
#include "../Audio/Source/AudioSystemWwise.h"
#include "../Audio/Source/Wwise_IDs.h"
#
using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

typedef XMINT2 int2;
typedef XMINT3 int3;
typedef XMINT4 int4;

typedef XMFLOAT2 float2;
typedef XMFLOAT3 float3;
typedef XMFLOAT4 float4;

typedef XMUINT2 uint2;
typedef XMUINT3 uint3;
typedef XMUINT4 uint4;

#define OUTPUT(x) OutputDebugStringA(std::string(x).c_str()) // Remember to use newline
#define SAFE_DELETE(x) { if((x)) { delete (x); (x) = nullptr; } }
#define SAFE_RELEASE(x) { if((x)) { (x)->Release(); (x) = nullptr; } }

#define DEGREES(radians) ((180.0f / 3.14f) * radians)
#define RADIANS(degrees) ((3.14f / 180.0f) * degrees)

//#define FPS 60
#define FRAME_TIME (1.0f / FPS)

#define FOV 75
#define SENSITIVITY 0.35f

#define DEFAULT_WINDOWED_WIDTH (SCREEN_WIDTH / 1.5f)
#define DEFAULT_WINDOWED_HEIGHT (SCREEN_HEIGHT / 1.5f)

#define CUSTOM_RES_WIDTH  gCustomWidth
#define CUSTOM_RES_HEIGHT gCustomHeight

#define SCREEN_WIDTH gScreenWidth
#define SCREEN_HEIGHT gScreenHeight

#define WINDOW_WIDTH gWindowWidth
#define WINDOW_HEIGHT gWindowHeight
#define RATIO ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

#define WINDOW_START_X ((SCREEN_WIDTH - WINDOW_WIDTH) / 2)
#define WINDOW_START_Y ((SCREEN_HEIGHT - WINDOW_HEIGHT) / 2)

#define WINDOW_STYLE WS_POPUP

#pragma region Array Cleanup Macros

#define ARRAY_DELETE(arr, size) { for (unsigned int i = 0; i < (size); i++) { delete (arr)[i]; } }
#define ARRAY_RELEASE(arr, size) { for (unsigned int i = 0; i < (size); i++) { SAFE_RELEASE((arr)[i]); } }

#define VECTOR_REMOVE(vector, x) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { if (*iter == (x)) { (vector).erase(iter); break; } iter++; } }
#define VECTOR_DELETE(vector, x) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { if (*iter == (x)) { SAFE_DELETE(*iter); (x) = nullptr; (vector).erase(iter); break; } iter++; } }
#define VECTOR_RELEASE(vector, x) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { if (*iter == (x)) { SAFE_RELEASE(*iter); (x) = nullptr; (vector).erase(iter); break; } iter++; } }
#define VECTOR_DELETE_ALL(vector) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { SAFE_DELETE(*iter); iter++; } (vector).clear(); }
#define VECTOR_RELEASE_ALL(vector) { auto iter = (vector).begin(); auto end = (vector).end(); while (iter != end) { SAFE_RELEASE(*iter); iter++; } (vector).clear(); }

#define MAP_REMOVE(map, x) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { if (iter->second == (x)) { (map).erase(iter); break; } iter++; } }
#define MAP_DELETE(map, x) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { if (iter->second == (x)) { SAFE_DELETE(iter->second); (x) = nullptr; (map).erase(iter); break; } iter++; } }
#define MAP_RELEASE(map, x) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { if (iter->second == (x)) { SAFE_RELEASE(iter->second); (x) = nullptr; (map).erase(iter); break; } iter++; } }
#define MAP_DELETE_ALL(map) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { SAFE_DELETE(iter->second); iter++; } (map).clear(); }
#define MAP_RELEASE_ALL(map) { auto iter = (map).begin(); auto end = (map).end(); while (iter != end) { SAFE_RELEASE(iter->second); iter++; } (map).clear(); }

#pragma endregion // Because I hate how ugly iteration loops look

// Global Declarations //
extern HWND hWindow;
extern bool bInverse;

extern int gCustomWidth;
extern int gCustomHeight;

extern int gScreenWidth;
extern int gScreenHeight;

extern int gWindowWidth;
extern int gWindowHeight;

extern const void* gGlobalTime;
extern const void* gGlobalGraphics;

// Global Accessors //
#define TICK ((CTime*)gGlobalTime)->Tick
#define DELTA_TIME ((CTime*)gGlobalTime)->Delta
#define CURRENT_TIME ((CTime*)gGlobalTime)->Current

#define GRAPHICS ((CRenderer*)gGlobalGraphics)
#include "../Core/DebugUtil.h"
#if _DEBUG
#endif

//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////




// TODO: reference additional headers your program requires here
