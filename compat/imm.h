#pragma once
#include "afxwin.h"

#define CFS_RECT 0x0001
struct COMPOSITIONFORM {
    DWORD dwStyle;
    POINT ptCurrentPos;
    RECT  rcArea;
};

// Input Method Manager (IMM32) stubs
typedef void* HIMC;
inline HIMC ImmGetContext(HWND hWnd) { return nullptr; }
inline BOOL ImmReleaseContext(HWND hWnd, HIMC hIMC) { return TRUE; }
inline BOOL ImmSetCompositionWindow(HIMC hIMC, void* lpCompForm) { return TRUE; }
inline BOOL ImmGetCompositionWindow(HIMC hIMC, COMPOSITIONFORM* lpCompForm) { return TRUE; }
inline BOOL ImmSetCompositionFont(HIMC hIMC, void* lplf) { return TRUE; }
inline BOOL ImmNotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue) { return TRUE; }
inline HIMC ImmAssociateContext(HWND hWnd, HIMC hIMC) { return nullptr; }
inline LONG ImmGetCompositionString(HIMC hIMC, DWORD dwIndex, LPVOID lpBuf, DWORD dwBufLen) { return 0; }
