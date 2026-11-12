#pragma once
#include "afxwin.h"
#include "TmSchema.h"

typedef void* HTHEME;

inline HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList) { return nullptr; }
inline HRESULT CloseThemeData(HTHEME hTheme) { return 0; }
inline HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect) { return 0; }
inline BOOL IsThemeActive() { return FALSE; }
inline BOOL IsAppThemed() { return FALSE; }
