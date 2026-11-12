#pragma once
#define NOMINMAX
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "afxwin.h"
#include "mfc_types.h"

// Pixel format is 32-bit ARGB (0xAARRGGBB) matching Wayland WL_SHM_FORMAT_ARGB8888 / XRGB8888
typedef uint32_t Pixel32;

inline Pixel32 ColorRefToPixel32(COLORREF cr) {
    uint8_t r = GetRValue(cr);
    uint8_t g = GetGValue(cr);
    uint8_t b = GetBValue(cr);
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}

inline COLORREF Pixel32ToColorRef(Pixel32 px) {
    uint8_t r = (px >> 16) & 0xFF;
    uint8_t g = (px >> 8) & 0xFF;
    uint8_t b = px & 0xFF;
    return RGB(r, g, b);
}

struct GDISurface {
    int width = 0;
    int height = 0;
    int stride = 0; // in bytes
    std::vector<Pixel32> pixels;
    Pixel32* buffer = nullptr;
    bool isExternal = false;

    GDISurface() = default;
    GDISurface(int w, int h);
    void Resize(int w, int h);
    void Clear(Pixel32 color);
    inline Pixel32 GetPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return buffer[y * (stride / 4) + x];
        }
        return 0;
    }
    inline void SetPixel(int x, int y, Pixel32 c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y * (stride / 4) + x] = c;
        }
    }
};

struct GDIBitmap {
    int width = 0;
    int height = 0;
    int bpp = 32;
    GDISurface surface;
    std::vector<RGBQUAD> palette;
    void* dibSectionBits = nullptr;
};

struct GDIPen {
    int style = PS_SOLID;
    int width = 1;
    COLORREF color = RGB(0, 0, 0);
};

struct GDIBrush {
    int style = BS_SOLID;
    COLORREF color = RGB(255, 255, 255);
    HBITMAP hPatternBitmap = nullptr;
};

struct GDIFont {
    LOGFONT lf;
};

struct GDIPalette {
    std::vector<PALETTEENTRY> entries;
};

struct GDIRgn {
    std::vector<POINT> polyPoints;
    RECT rect = {0, 0, 0, 0};
    int type = 0; // 0: rect, 1: poly
};

struct GDIDC {
    HWND hWnd = nullptr;
    GDISurface* surface = nullptr;
    bool ownsSurface = false;

    HBITMAP hBitmap = nullptr;
    HPEN hPen = nullptr;
    HBRUSH hBrush = nullptr;
    HFONT hFont = nullptr;
    HPALETTE hPalette = nullptr;
    HRGN hClipRgn = nullptr;

    COLORREF textColor = RGB(0, 0, 0);
    COLORREF bkColor = RGB(255, 255, 255);
    int bkMode = OPAQUE;
    int rop2 = R2_COPYPEN;
    int stretchBltMode = COLORONCOLOR;
    POINT curPos = {0, 0};
    POINT viewportOrg = {0, 0};
    POINT windowOrg = {0, 0};
    SIZE viewportExt = {1, 1};
    SIZE windowExt = {1, 1};
    UINT textAlign = 0;

    ~GDIDC();
};

// Internal rasterizer helpers
GDISurface* GetDCSurface(HDC hdc);
GDIPen* GetDCPen(HDC hdc);
GDIBrush* GetDCBrush(HDC hdc);
GDIBitmap* GetBitmapObj(HBITMAP hbmp);

// Bitmap loader from file or memory buffer
HBITMAP LoadBitmapFromFile(const char* filepath);
HBITMAP LoadBitmapFromMemory(const void* data, size_t size);

BOOL ResizePalette(HPALETTE hpal, UINT nEntries);
HRGN CreateRectRgnIndirect(const RECT* lprc);
BOOL OffsetRgn(HRGN hrgn, int nXOffset, int nYOffset);
BOOL TextOutA(HDC hdc, int nXStart, int nYStart, LPCSTR lpString, int cbString);
BOOL ExtTextOutA(HDC hdc, int X, int Y, UINT fuOptions, const RECT* lprc, LPCSTR lpString, UINT cbCount, const int* lpDx);
int DrawTextA(HDC hDC, LPCSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat);
BOOL TabbedTextOutA(HDC hDC, int X, int Y, LPCSTR lpString, int nCount, int nTabPositions, const int* lpnTabStopPositions, int nTabOrigin);
