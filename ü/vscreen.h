#pragma once
#include <Windows.h>

extern RECT GetVirtualScreen(VOID);

extern SIZE GetVirtualScreenSize(VOID);

extern POINT GetVirtualScreenPos(VOID);

extern BOOL CALLBACK MonitorEnumProc(
	_In_ HMONITOR hMonitor,
	_In_ HDC hdcMonitor,
	_In_ PRECT prcBounds,
	_Inout_ LPARAM lParam
);