#include "timer.h"

DWORD dwTimeElapsed = 0;

VOID CALLBACK TimerProc(
	_In_ HWND hwndTimer,
	_In_ UINT uMsg,
	_In_ UINT_PTR ulTimerID,
	_In_ DWORD dwTime
) {
	dwTimeElapsed++;
}

VOID WINAPI InitializeTimer(_In_ UINT uDelay) {
	SetTimer(NULL, 0, uDelay, TimerProc);
	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}