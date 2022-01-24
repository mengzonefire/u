#pragma once
#include <Windows.h>

DWORD dwTimeElapsed;

VOID CALLBACK TimerProc(
	_In_ HWND hwndTimer,
	_In_ UINT uMsg,
	_In_ UINT_PTR ulTimerID,
	_In_ DWORD dwTime
);

VOID InitializeTimer(_In_ UINT uDelay);