#include "gdi_rasterizer.h"
#include <cmath>

GDISurface::GDISurface(int w, int h) {
    Resize(w, h);
}

void GDISurface::Resize(int w, int h) {
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    width = w;
    height = h;
    stride = w * 4;
    pixels.resize(width * height, 0xFFFFFFFF);
    buffer = pixels.data();
    isExternal = false;
}

void GDISurface::Clear(Pixel32 color) {
    if (buffer) {
        std::fill(pixels.begin(), pixels.end(), color);
    }
}

// ============================================================================
// Object Pools & Handles
// ============================================================================

static std::vector<std::unique_ptr<GDIBitmap>> g_bitmaps;
static std::vector<std::unique_ptr<GDIPen>> g_pens;
static std::vector<std::unique_ptr<GDIBrush>> g_brushes;
static std::vector<std::unique_ptr<GDIFont>> g_fonts;
static std::vector<std::unique_ptr<GDIPalette>> g_palettes;
static std::vector<std::unique_ptr<GDIRgn>> g_rgns;
static std::vector<std::unique_ptr<GDIDC>> g_dcs;

GDIDC::~GDIDC() {
    if (ownsSurface && surface) {
        delete surface;
        surface = nullptr;
    }
}

static inline uintptr_t MakeHandle(uint32_t type, uint32_t idx) {
    return ((uintptr_t)type << 24) | (idx + 1);
}

static inline uint32_t HandleType(void* h) {
    return (uint32_t)((uintptr_t)h >> 24);
}

static inline uint32_t HandleIndex(void* h) {
    return (uint32_t)((uintptr_t)h & 0xFFFFFF) - 1;
}

#define GDI_OBJ_DC 12

GDIBitmap* GetBitmapObj(HBITMAP hbmp) {
    if (!hbmp || HandleType(hbmp) != OBJ_BITMAP) return nullptr;
    uint32_t idx = HandleIndex(hbmp);
    return (idx < g_bitmaps.size()) ? g_bitmaps[idx].get() : nullptr;
}

GDIPen* GetDCPen(HDC hdc) {
    if (!hdc || HandleType(hdc) != GDI_OBJ_DC) return nullptr;
    uint32_t idx = HandleIndex(hdc);
    if (idx >= g_dcs.size() || !g_dcs[idx]) return nullptr;
    HPEN hp = g_dcs[idx]->hPen;
    if (!hp || HandleType(hp) != OBJ_PEN) return nullptr;
    uint32_t pidx = HandleIndex(hp);
    return (pidx < g_pens.size()) ? g_pens[pidx].get() : nullptr;
}

GDIBrush* GetDCBrush(HDC hdc) {
    if (!hdc || HandleType(hdc) != GDI_OBJ_DC) return nullptr;
    uint32_t idx = HandleIndex(hdc);
    if (idx >= g_dcs.size() || !g_dcs[idx]) return nullptr;
    HBRUSH hb = g_dcs[idx]->hBrush;
    if (!hb || HandleType(hb) != OBJ_BRUSH) return nullptr;
    uint32_t bidx = HandleIndex(hb);
    return (bidx < g_brushes.size()) ? g_brushes[bidx].get() : nullptr;
}

GDISurface* GetDCSurface(HDC hdc) {
    if (!hdc || HandleType(hdc) != GDI_OBJ_DC) return nullptr;
    uint32_t idx = HandleIndex(hdc);
    if (idx >= g_dcs.size() || !g_dcs[idx]) return nullptr;
    return g_dcs[idx]->surface;
}

static GDIDC* GetDC(HDC hdc) {
    if (!hdc || HandleType(hdc) != GDI_OBJ_DC) return nullptr;
    uint32_t idx = HandleIndex(hdc);
    return (idx < g_dcs.size()) ? g_dcs[idx].get() : nullptr;
}

// ============================================================================
// Stock Objects
// ============================================================================

static HBRUSH s_hWhiteBrush = nullptr;
static HBRUSH s_hBlackBrush = nullptr;
static HBRUSH s_hGrayBrush = nullptr;
static HBRUSH s_hNullBrush = nullptr;
static HPEN s_hWhitePen = nullptr;
static HPEN s_hBlackPen = nullptr;
static HPEN s_hNullPen = nullptr;

static bool s_stockInit = false;

static void InitStockObjects() {
    if (s_stockInit) return;
    s_stockInit = true;

    auto wb = std::make_unique<GDIBrush>();
    wb->style = BS_SOLID;
    wb->color = RGB(255, 255, 255);
    g_brushes.push_back(std::move(wb));
    s_hWhiteBrush = (HBRUSH)MakeHandle(OBJ_BRUSH, g_brushes.size() - 1);

    auto bb = std::make_unique<GDIBrush>();
    bb->style = BS_SOLID;
    bb->color = RGB(0, 0, 0);
    g_brushes.push_back(std::move(bb));
    s_hBlackBrush = (HBRUSH)MakeHandle(OBJ_BRUSH, g_brushes.size() - 1);

    auto gb = std::make_unique<GDIBrush>();
    gb->style = BS_SOLID;
    gb->color = RGB(192, 192, 192);
    g_brushes.push_back(std::move(gb));
    s_hGrayBrush = (HBRUSH)MakeHandle(OBJ_BRUSH, g_brushes.size() - 1);

    auto nb = std::make_unique<GDIBrush>();
    nb->style = BS_NULL;
    g_brushes.push_back(std::move(nb));
    s_hNullBrush = (HBRUSH)MakeHandle(OBJ_BRUSH, g_brushes.size() - 1);

    auto wp = std::make_unique<GDIPen>();
    wp->style = PS_SOLID;
    wp->width = 1;
    wp->color = RGB(255, 255, 255);
    g_pens.push_back(std::move(wp));
    s_hWhitePen = (HPEN)MakeHandle(OBJ_PEN, g_pens.size() - 1);

    auto bp = std::make_unique<GDIPen>();
    bp->style = PS_SOLID;
    bp->width = 1;
    bp->color = RGB(0, 0, 0);
    g_pens.push_back(std::move(bp));
    s_hBlackPen = (HPEN)MakeHandle(OBJ_PEN, g_pens.size() - 1);

    auto np = std::make_unique<GDIPen>();
    np->style = PS_NULL;
    g_pens.push_back(std::move(np));
    s_hNullPen = (HPEN)MakeHandle(OBJ_PEN, g_pens.size() - 1);
}

HGDIOBJ GetStockObject(int fnObject) {
    InitStockObjects();
    switch (fnObject) {
        case WHITE_BRUSH: return (HGDIOBJ)s_hWhiteBrush;
        case BLACK_BRUSH: return (HGDIOBJ)s_hBlackBrush;
        case LTGRAY_BRUSH:
        case GRAY_BRUSH:
        case DKGRAY_BRUSH: return (HGDIOBJ)s_hGrayBrush;
        case NULL_BRUSH: return (HGDIOBJ)s_hNullBrush;
        case WHITE_PEN: return (HGDIOBJ)s_hWhitePen;
        case BLACK_PEN: return (HGDIOBJ)s_hBlackPen;
        case NULL_PEN: return (HGDIOBJ)s_hNullPen;
        default: return (HGDIOBJ)s_hWhiteBrush;
    }
}

// ============================================================================
// GDI Object Creation / Destruction
// ============================================================================

HBITMAP CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, const void* lpBits) {
    InitStockObjects();
    auto bmp = std::make_unique<GDIBitmap>();
    bmp->width = nWidth;
    bmp->height = nHeight;
    bmp->bpp = nBitCount;
    bmp->surface.Resize(nWidth, nHeight);
    if (lpBits && nBitCount == 32) {
        std::memcpy(bmp->surface.buffer, lpBits, nWidth * nHeight * 4);
    } else if (lpBits && nBitCount == 1) {
        // Monochrome bitmap
        const uint8_t* src = (const uint8_t*)lpBits;
        int rowBytes = ((nWidth + 15) / 16) * 2;
        for (int y = 0; y < nHeight; ++y) {
            for (int x = 0; x < nWidth; ++x) {
                int byteIdx = y * rowBytes + (x / 8);
                int bit = 7 - (x % 8);
                bool on = (src[byteIdx] >> bit) & 1;
                bmp->surface.SetPixel(x, y, on ? 0xFFFFFFFF : 0xFF000000);
            }
        }
    }
    g_bitmaps.push_back(std::move(bmp));
    return (HBITMAP)MakeHandle(OBJ_BITMAP, g_bitmaps.size() - 1);
}

HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight) {
    return CreateBitmap(nWidth, nHeight, 1, 32, nullptr);
}

HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT iUsage, void** ppvBits, HANDLE hSection, DWORD dwOffset) {
    InitStockObjects();
    if (!pbmi) return nullptr;
    int w = pbmi->bmiHeader.biWidth;
    int h = std::abs(pbmi->bmiHeader.biHeight);
    int bpp = pbmi->bmiHeader.biBitCount;

    auto bmp = std::make_unique<GDIBitmap>();
    bmp->width = w;
    bmp->height = h;
    bmp->bpp = bpp;
    bmp->surface.Resize(w, h);

    // If 8-bpp or less, copy palette
    if (bpp <= 8 && pbmi->bmiHeader.biClrUsed > 0) {
        bmp->palette.assign(pbmi->bmiColors, pbmi->bmiColors + pbmi->bmiHeader.biClrUsed);
    }

    if (ppvBits) {
        *ppvBits = bmp->surface.buffer;
        bmp->dibSectionBits = bmp->surface.buffer;
    }
    g_bitmaps.push_back(std::move(bmp));
    return (HBITMAP)MakeHandle(OBJ_BITMAP, g_bitmaps.size() - 1);
}

HBITMAP CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER* lpbmih, DWORD fdwInit, const void* lpbInit, const BITMAPINFO* lpbmi, UINT fuUsage) {
    if (!lpbmih) return nullptr;
    int w = lpbmih->biWidth;
    int h = std::abs(lpbmih->biHeight);
    HBITMAP hbmp = CreateBitmap(w, h, 1, 32, nullptr);
    if (lpbInit && lpbmi) {
        SetDIBits(hdc, hbmp, 0, h, lpbInit, lpbmi, fuUsage);
    }
    return hbmp;
}

HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor) {
    InitStockObjects();
    auto pen = std::make_unique<GDIPen>();
    pen->style = fnPenStyle;
    pen->width = nWidth > 0 ? nWidth : 1;
    pen->color = crColor;
    g_pens.push_back(std::move(pen));
    return (HPEN)MakeHandle(OBJ_PEN, g_pens.size() - 1);
}

HBRUSH CreateSolidBrush(COLORREF crColor) {
    InitStockObjects();
    auto br = std::make_unique<GDIBrush>();
    br->style = BS_SOLID;
    br->color = crColor;
    g_brushes.push_back(std::move(br));
    return (HBRUSH)MakeHandle(OBJ_BRUSH, g_brushes.size() - 1);
}

HRGN CreateRectRgnIndirect(const RECT* lprc) {
    auto rgn = std::make_unique<GDIRgn>();
    if (lprc) rgn->rect = *lprc;
    rgn->type = 0;
    g_rgns.push_back(std::move(rgn));
    return (HRGN)MakeHandle(OBJ_REGION, g_rgns.size() - 1);
}

HRGN CreatePolygonRgn(const POINT* pptl, int cPoints, int fnPolyFillMode) {
    auto rgn = std::make_unique<GDIRgn>();
    rgn->type = 1;
    if (pptl && cPoints > 0) {
        rgn->polyPoints.assign(pptl, pptl + cPoints);
    }
    g_rgns.push_back(std::move(rgn));
    return (HRGN)MakeHandle(OBJ_REGION, g_rgns.size() - 1);
}

BOOL OffsetRgn(HRGN hrgn, int nXOffset, int nYOffset) {
    if (!hrgn || HandleType(hrgn) != OBJ_REGION) return FALSE;
    uint32_t idx = HandleIndex(hrgn);
    if (idx < g_rgns.size() && g_rgns[idx]) {
        g_rgns[idx]->rect.left += nXOffset;
        g_rgns[idx]->rect.right += nXOffset;
        g_rgns[idx]->rect.top += nYOffset;
        g_rgns[idx]->rect.bottom += nYOffset;
        for (auto& pt : g_rgns[idx]->polyPoints) {
            pt.x += nXOffset;
            pt.y += nYOffset;
        }
        return TRUE;
    }
    return FALSE;
}

HPALETTE CreatePalette(const LOGPALETTE* lplgpl) {
    auto pal = std::make_unique<GDIPalette>();
    if (lplgpl && lplgpl->palNumEntries > 0) {
        pal->entries.assign(lplgpl->palPalEntry, lplgpl->palPalEntry + lplgpl->palNumEntries);
    }
    g_palettes.push_back(std::move(pal));
    return (HPALETTE)MakeHandle(OBJ_PAL, g_palettes.size() - 1);
}

UINT GetPaletteEntries(HPALETTE hpal, UINT iStartIndex, UINT nEntries, LPPALETTEENTRY lppe) {
    if (!hpal || HandleType(hpal) != OBJ_PAL) return 0;
    uint32_t idx = HandleIndex(hpal);
    if (idx >= g_palettes.size() || !g_palettes[idx]) return 0;
    const auto& pal = g_palettes[idx]->entries;
    UINT count = 0;
    for (UINT i = 0; i < nEntries && (iStartIndex + i) < pal.size(); ++i) {
        if (lppe) lppe[i] = pal[iStartIndex + i];
        count++;
    }
    return count;
}

UINT SetPaletteEntries(HPALETTE hpal, UINT iStartIndex, UINT nEntries, const PALETTEENTRY* lppe) {
    if (!hpal || HandleType(hpal) != OBJ_PAL || !lppe) return 0;
    uint32_t idx = HandleIndex(hpal);
    if (idx >= g_palettes.size() || !g_palettes[idx]) return 0;
    auto& pal = g_palettes[idx]->entries;
    if (iStartIndex + nEntries > pal.size()) pal.resize(iStartIndex + nEntries);
    for (UINT i = 0; i < nEntries; ++i) {
        pal[iStartIndex + i] = lppe[i];
    }
    return nEntries;
}

BOOL ResizePalette(HPALETTE hpal, UINT nEntries) {
    if (!hpal || HandleType(hpal) != OBJ_PAL) return FALSE;
    uint32_t idx = HandleIndex(hpal);
    if (idx >= g_palettes.size() || !g_palettes[idx]) return FALSE;
    g_palettes[idx]->entries.resize(nEntries);
    return TRUE;
}

UINT GetNearestPaletteIndex(HPALETTE hpal, COLORREF crColor) {
    if (!hpal || HandleType(hpal) != OBJ_PAL) return 0;
    uint32_t idx = HandleIndex(hpal);
    if (idx >= g_palettes.size() || !g_palettes[idx]) return 0;
    const auto& pal = g_palettes[idx]->entries;
    int bestDist = 0x7FFFFFFF;
    UINT bestIdx = 0;
    int r = GetRValue(crColor), g = GetGValue(crColor), b = GetBValue(crColor);
    for (UINT i = 0; i < pal.size(); ++i) {
        int dr = (int)pal[i].peRed - r;
        int dg = (int)pal[i].peGreen - g;
        int db = (int)pal[i].peBlue - b;
        int dist = dr * dr + dg * dg + db * db;
        if (dist < bestDist) {
            bestDist = dist;
            bestIdx = i;
        }
    }
    return bestIdx;
}

BOOL DeleteObject(HGDIOBJ hObject) {
    if (!hObject) return FALSE;
    uint32_t type = HandleType(hObject);
    uint32_t idx = HandleIndex(hObject);
    switch (type) {
        case OBJ_BITMAP:
            if (idx < g_bitmaps.size()) g_bitmaps[idx].reset();
            return TRUE;
        case OBJ_PEN:
            if (idx < g_pens.size()) g_pens[idx].reset();
            return TRUE;
        case OBJ_BRUSH:
            if (idx < g_brushes.size()) g_brushes[idx].reset();
            return TRUE;
        case OBJ_FONT:
            if (idx < g_fonts.size()) g_fonts[idx].reset();
            return TRUE;
        case OBJ_PAL:
            if (idx < g_palettes.size()) g_palettes[idx].reset();
            return TRUE;
        case OBJ_REGION:
            if (idx < g_rgns.size()) g_rgns[idx].reset();
            return TRUE;
    }
    return FALSE;
}

int GetObject(HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject) {
    if (!hgdiobj || !lpvObject) return 0;
    uint32_t type = HandleType(hgdiobj);
    if (type == OBJ_BITMAP) {
        GDIBitmap* bmp = GetBitmapObj((HBITMAP)hgdiobj);
        if (!bmp) return 0;
        if (cbBuffer >= (int)sizeof(BITMAP)) {
            BITMAP* b = (BITMAP*)lpvObject;
            b->bmType = 0;
            b->bmWidth = bmp->width;
            b->bmHeight = bmp->height;
            b->bmWidthBytes = bmp->surface.stride;
            b->bmPlanes = 1;
            b->bmBitsPixel = bmp->bpp;
            b->bmBits = bmp->surface.buffer;
            return sizeof(BITMAP);
        }
    } else if (type == OBJ_PEN) {
        uint32_t idx = HandleIndex(hgdiobj);
        if (idx < g_pens.size() && g_pens[idx]) {
            if (cbBuffer >= (int)sizeof(LOGPEN)) {
                LOGPEN* lp = (LOGPEN*)lpvObject;
                lp->lopnStyle = g_pens[idx]->style;
                lp->lopnWidth.x = g_pens[idx]->width;
                lp->lopnWidth.y = 0;
                lp->lopnColor = g_pens[idx]->color;
                return sizeof(LOGPEN);
            }
        }
    } else if (type == OBJ_BRUSH) {
        uint32_t idx = HandleIndex(hgdiobj);
        if (idx < g_brushes.size() && g_brushes[idx]) {
            if (cbBuffer >= (int)sizeof(LOGBRUSH)) {
                LOGBRUSH* lb = (LOGBRUSH*)lpvObject;
                lb->lbStyle = g_brushes[idx]->style;
                lb->lbColor = g_brushes[idx]->color;
                lb->lbHatch = 0;
                return sizeof(LOGBRUSH);
            }
        }
    }
    return 0;
}

// ============================================================================
// Device Context (HDC)
// ============================================================================

HDC CreateCompatibleDC(HDC hdc) {
    InitStockObjects();
    auto dc = std::make_unique<GDIDC>();
    dc->surface = new GDISurface(1, 1);
    dc->ownsSurface = true;
    dc->hPen = (HPEN)GetStockObject(BLACK_PEN);
    dc->hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    g_dcs.push_back(std::move(dc));
    return (HDC)MakeHandle(GDI_OBJ_DC, g_dcs.size() - 1);
}

HDC CreateDC(LPCSTR lpszDriver, LPCSTR lpszDevice, LPCSTR lpszOutput, const void* lpInitData) {
    return CreateCompatibleDC(nullptr);
}

BOOL DeleteDC(HDC hdc) {
    if (!hdc || HandleType(hdc) != GDI_OBJ_DC) return FALSE;
    uint32_t idx = HandleIndex(hdc);
    if (idx < g_dcs.size()) {
        g_dcs[idx].reset();
        return TRUE;
    }
    return FALSE;
}

HGDIOBJ SelectObject(HDC hdc, HGDIOBJ h) {
    GDIDC* dc = GetDC(hdc);
    if (!dc || !h) return nullptr;
    uint32_t type = HandleType(h);
    HGDIOBJ old = nullptr;
    switch (type) {
        case OBJ_BITMAP: {
            old = (HGDIOBJ)dc->hBitmap;
            dc->hBitmap = (HBITMAP)h;
            GDIBitmap* bmp = GetBitmapObj((HBITMAP)h);
            if (bmp) {
                if (dc->ownsSurface && dc->surface) delete dc->surface;
                dc->surface = &bmp->surface;
                dc->ownsSurface = false;
            }
            break;
        }
        case OBJ_PEN:
            old = (HGDIOBJ)dc->hPen;
            dc->hPen = (HPEN)h;
            break;
        case OBJ_BRUSH:
            old = (HGDIOBJ)dc->hBrush;
            dc->hBrush = (HBRUSH)h;
            break;
        case OBJ_FONT:
            old = (HGDIOBJ)dc->hFont;
            dc->hFont = (HFONT)h;
            break;
        case OBJ_REGION:
            old = (HGDIOBJ)dc->hClipRgn;
            dc->hClipRgn = (HRGN)h;
            break;
    }
    return old;
}

HPALETTE SelectPalette(HDC hdc, HPALETTE hPal, BOOL bForceBackground) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return nullptr;
    HPALETTE old = dc->hPalette;
    dc->hPalette = hPal;
    return old;
}

UINT RealizePalette(HDC hdc) { return 0; }

COLORREF SetTextColor(HDC hdc, COLORREF crColor) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return RGB(0, 0, 0);
    COLORREF old = dc->textColor;
    dc->textColor = crColor;
    return old;
}

COLORREF SetBkColor(HDC hdc, COLORREF crColor) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return RGB(255, 255, 255);
    COLORREF old = dc->bkColor;
    dc->bkColor = crColor;
    return old;
}

int SetBkMode(HDC hdc, int iBkMode) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return OPAQUE;
    int old = dc->bkMode;
    dc->bkMode = iBkMode;
    return old;
}

int SetROP2(HDC hdc, int fnDrawMode) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return R2_COPYPEN;
    int old = dc->rop2;
    dc->rop2 = fnDrawMode;
    return old;
}

int SetStretchBltMode(HDC hdc, int iStretchMode) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return COLORONCOLOR;
    int old = dc->stretchBltMode;
    dc->stretchBltMode = iStretchMode;
    return old;
}

UINT SetTextAlign(HDC hdc, UINT fMode) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return 0;
    UINT old = dc->textAlign;
    dc->textAlign = fMode;
    return old;
}

BOOL MoveToEx(HDC hdc, int X, int Y, LPPOINT lpPoint) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return FALSE;
    if (lpPoint) *lpPoint = dc->curPos;
    dc->curPos.x = X;
    dc->curPos.y = Y;
    return TRUE;
}

COLORREF GetPixel(HDC hdc, int nXPos, int nYPos) {
    GDISurface* s = GetDCSurface(hdc);
    if (!s) return RGB(0, 0, 0);
    return Pixel32ToColorRef(s->GetPixel(nXPos, nYPos));
}

COLORREF SetPixel(HDC hdc, int X, int Y, COLORREF crColor) {
    GDISurface* s = GetDCSurface(hdc);
    if (!s) return crColor;
    s->SetPixel(X, Y, ColorRefToPixel32(crColor));
    return crColor;
}

int GetDeviceCaps(HDC hdc, int nIndex) {
    switch (nIndex) {
        case HORZRES: return 1024;
        case VERTRES: return 768;
        case HORZSIZE: return 320;
        case VERTSIZE: return 240;
        case BITSPIXEL: return 32;
        case PLANES: return 1;
        case NUMCOLORS: return -1;
        case RASTERCAPS: return 0xFFFF;
        case LOGPIXELSX: return 96;
        case LOGPIXELSY: return 96;
        default: return 0;
    }
}

COLORREF GetNearestColor(HDC hdc, COLORREF crColor) { return crColor; }

// ============================================================================
// BitBlt, PatBlt, StretchBlt Rasterization
// ============================================================================

static inline Pixel32 ApplyROP(Pixel32 dst, Pixel32 src, Pixel32 pat, DWORD rop) {
    switch (rop) {
        case SRCCOPY: return src;
        case SRCINVERT: return dst ^ src;
        case DSTINVERT: return ~dst;
        case SRCPAINT: return dst | src;
        case SRCAND: return dst & src;
        case PATCOPY: return pat;
        case PATINVERT: return dst ^ pat;
        case BLACKNESS: return 0xFF000000;
        case WHITENESS: return 0xFFFFFFFF;
        case 0x00E20746: // DSPDxax: (src ^ pat) & dst ^ src
            return ((src ^ pat) & dst) ^ src;
        case 0x00B8074A: // PSDPxax
            return ((dst ^ pat) & src) ^ pat;
        case 0x00220326: // DSna: dst & ~src
            return dst & ~src;
        default:
            return src;
    }
}

BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, DWORD dwRop) {
    GDISurface* sDest = GetDCSurface(hdcDest);
    if (!sDest) return FALSE;
    GDISurface* sSrc = hdcSrc ? GetDCSurface(hdcSrc) : nullptr;
    GDIBrush* brush = GetDCBrush(hdcDest);
    Pixel32 patColor = brush ? ColorRefToPixel32(brush->color) : 0xFFFFFFFF;

    for (int y = 0; y < nHeight; ++y) {
        int dy = nYDest + y;
        int sy = nYSrc + y;
        if (dy < 0 || dy >= sDest->height) continue;

        for (int x = 0; x < nWidth; ++x) {
            int dx = nXDest + x;
            int sx = nXSrc + x;
            if (dx < 0 || dx >= sDest->width) continue;

            Pixel32 srcPix = sSrc ? sSrc->GetPixel(sx, sy) : 0;
            Pixel32 dstPix = sDest->GetPixel(dx, dy);
            sDest->SetPixel(dx, dy, ApplyROP(dstPix, srcPix, patColor, dwRop));
        }
    }
    return TRUE;
}

BOOL PatBlt(HDC hdc, int nXLeft, int nYLeft, int nWidth, int nHeight, DWORD dwRop) {
    GDISurface* sDest = GetDCSurface(hdc);
    if (!sDest) return FALSE;
    GDIBrush* brush = GetDCBrush(hdc);
    Pixel32 patColor = brush ? ColorRefToPixel32(brush->color) : 0xFFFFFFFF;

    for (int y = 0; y < nHeight; ++y) {
        int dy = nYLeft + y;
        if (dy < 0 || dy >= sDest->height) continue;
        for (int x = 0; x < nWidth; ++x) {
            int dx = nXLeft + x;
            if (dx < 0 || dx >= sDest->width) continue;
            Pixel32 dstPix = sDest->GetPixel(dx, dy);
            sDest->SetPixel(dx, dy, ApplyROP(dstPix, patColor, patColor, dwRop));
        }
    }
    return TRUE;
}

BOOL StretchBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
                HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, DWORD dwRop) {
    GDISurface* sDest = GetDCSurface(hdcDest);
    GDISurface* sSrc = GetDCSurface(hdcSrc);
    if (!sDest || !sSrc || nWidthDest == 0 || nHeightDest == 0 || nWidthSrc == 0 || nHeightSrc == 0) return FALSE;

    for (int y = 0; y < nHeightDest; ++y) {
        int dy = nYOriginDest + y;
        if (dy < 0 || dy >= sDest->height) continue;
        int sy = nYOriginSrc + (y * nHeightSrc) / nHeightDest;

        for (int x = 0; x < nWidthDest; ++x) {
            int dx = nXOriginDest + x;
            if (dx < 0 || dx >= sDest->width) continue;
            int sx = nXOriginSrc + (x * nWidthSrc) / nWidthDest;

            Pixel32 srcPix = sSrc->GetPixel(sx, sy);
            sDest->SetPixel(dx, dy, srcPix);
        }
    }
    return TRUE;
}

// ============================================================================
// Vector Primitives: Lines, Rectangles, Ellipses, Polygons
// ============================================================================

static void DrawPixelROP2(GDISurface* s, int x, int y, Pixel32 penColor, int rop2) {
    if (x < 0 || x >= s->width || y < 0 || y >= s->height) return;
    Pixel32 dst = s->GetPixel(x, y);
    Pixel32 res = penColor;
    switch (rop2) {
        case R2_NOT: res = ~dst; break;
        case R2_XORPEN: res = dst ^ penColor; break;
        case R2_NOTXORPEN: res = ~(dst ^ penColor); break;
        case R2_NOP: res = dst; break;
        case R2_COPYPEN:
        default: res = penColor; break;
    }
    s->SetPixel(x, y, res);
}

BOOL LineTo(HDC hdc, int nXEnd, int nYEnd) {
    GDIDC* dc = GetDC(hdc);
    if (!dc || !dc->surface) return FALSE;
    GDIPen* pen = GetDCPen(hdc);
    if (pen && pen->style == PS_NULL) {
        dc->curPos.x = nXEnd;
        dc->curPos.y = nYEnd;
        return TRUE;
    }
    Pixel32 penColor = pen ? ColorRefToPixel32(pen->color) : 0xFF000000;
    int penW = pen ? pen->width : 1;

    int x0 = dc->curPos.x;
    int y0 = dc->curPos.y;
    int x1 = nXEnd;
    int y1 = nYEnd;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        for (int wy = -penW / 2; wy <= penW / 2; ++wy) {
            for (int wx = -penW / 2; wx <= penW / 2; ++wx) {
                DrawPixelROP2(dc->surface, x0 + wx, y0 + wy, penColor, dc->rop2);
            }
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }

    dc->curPos.x = nXEnd;
    dc->curPos.y = nYEnd;
    return TRUE;
}

BOOL Rectangle(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect) {
    GDIDC* dc = GetDC(hdc);
    if (!dc || !dc->surface) return FALSE;
    GDIBrush* brush = GetDCBrush(hdc);
    GDIPen* pen = GetDCPen(hdc);

    int l = std::min(nLeftRect, nRightRect);
    int r = std::max(nLeftRect, nRightRect);
    int t = std::min(nTopRect, nBottomRect);
    int b = std::max(nTopRect, nBottomRect);

    // Fill
    if (brush && brush->style != BS_NULL) {
        Pixel32 brushColor = ColorRefToPixel32(brush->color);
        for (int y = t + 1; y < b - 1; ++y) {
            for (int x = l + 1; x < r - 1; ++x) {
                dc->surface->SetPixel(x, y, brushColor);
            }
        }
    }
    // Border
    if (pen && pen->style != PS_NULL) {
        Pixel32 penColor = ColorRefToPixel32(pen->color);
        for (int x = l; x < r; ++x) {
            DrawPixelROP2(dc->surface, x, t, penColor, dc->rop2);
            DrawPixelROP2(dc->surface, x, b - 1, penColor, dc->rop2);
        }
        for (int y = t; y < b; ++y) {
            DrawPixelROP2(dc->surface, l, y, penColor, dc->rop2);
            DrawPixelROP2(dc->surface, r - 1, y, penColor, dc->rop2);
        }
    }
    return TRUE;
}

BOOL RoundRect(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, int nWidth, int nHeight) {
    return Rectangle(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
}

BOOL Ellipse(HDC hdc, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect) {
    GDIDC* dc = GetDC(hdc);
    if (!dc || !dc->surface) return FALSE;
    GDIBrush* brush = GetDCBrush(hdc);
    GDIPen* pen = GetDCPen(hdc);

    int l = std::min(nLeftRect, nRightRect);
    int r = std::max(nLeftRect, nRightRect);
    int t = std::min(nTopRect, nBottomRect);
    int b = std::max(nTopRect, nBottomRect);

    double rx = (r - l) / 2.0;
    double ry = (b - t) / 2.0;
    double cx = l + rx;
    double cy = t + ry;
    if (rx <= 0 || ry <= 0) return TRUE;

    Pixel32 brushColor = brush ? ColorRefToPixel32(brush->color) : 0xFFFFFFFF;
    Pixel32 penColor = pen ? ColorRefToPixel32(pen->color) : 0xFF000000;

    for (int y = t; y < b; ++y) {
        double dy = (y + 0.5 - cy) / ry;
        if (dy * dy > 1.0) continue;
        double dxMax = rx * std::sqrt(1.0 - dy * dy);
        int xMin = (int)std::ceil(cx - dxMax);
        int xMax = (int)std::floor(cx + dxMax);

        if (brush && brush->style != BS_NULL) {
            for (int x = xMin; x <= xMax; ++x) {
                dc->surface->SetPixel(x, y, brushColor);
            }
        }
        if (pen && pen->style != PS_NULL) {
            DrawPixelROP2(dc->surface, xMin, y, penColor, dc->rop2);
            DrawPixelROP2(dc->surface, xMax, y, penColor, dc->rop2);
        }
    }
    return TRUE;
}

BOOL Polyline(HDC hdc, const POINT* lppt, int cPoints) {
    if (!lppt || cPoints < 2) return FALSE;
    MoveToEx(hdc, lppt[0].x, lppt[0].y, nullptr);
    for (int i = 1; i < cPoints; ++i) {
        LineTo(hdc, lppt[i].x, lppt[i].y);
    }
    return TRUE;
}

BOOL Polygon(HDC hdc, const POINT* lpPoints, int nCount) {
    if (!lpPoints || nCount < 3) return FALSE;
    GDIDC* dc = GetDC(hdc);
    if (!dc || !dc->surface) return FALSE;
    GDIBrush* brush = GetDCBrush(hdc);
    GDIPen* pen = GetDCPen(hdc);

    int minY = lpPoints[0].y, maxY = lpPoints[0].y;
    for (int i = 1; i < nCount; ++i) {
        if (lpPoints[i].y < minY) minY = lpPoints[i].y;
        if (lpPoints[i].y > maxY) maxY = lpPoints[i].y;
    }

    if (brush && brush->style != BS_NULL) {
        Pixel32 brushColor = ColorRefToPixel32(brush->color);
        for (int y = minY; y <= maxY; ++y) {
            std::vector<int> nodes;
            for (int i = 0, j = nCount - 1; i < nCount; j = i++) {
                if ((lpPoints[i].y < y && lpPoints[j].y >= y) ||
                    (lpPoints[j].y < y && lpPoints[i].y >= y)) {
                    int x = lpPoints[i].x + (y - lpPoints[i].y) * (lpPoints[j].x - lpPoints[i].x) / (lpPoints[j].y - lpPoints[i].y);
                    nodes.push_back(x);
                }
            }
            std::sort(nodes.begin(), nodes.end());
            for (size_t i = 0; i + 1 < nodes.size(); i += 2) {
                for (int x = nodes[i]; x <= nodes[i + 1]; ++x) {
                    dc->surface->SetPixel(x, y, brushColor);
                }
            }
        }
    }

    // Outline
    Polyline(hdc, lpPoints, nCount);
    LineTo(hdc, lpPoints[0].x, lpPoints[0].y);
    return TRUE;
}

BOOL PolyBezier(HDC hdc, const POINT* lppt, DWORD cPoints) {
    if (!lppt || cPoints < 4) return FALSE;
    // Cubic bezier subdivided
    for (DWORD i = 0; i + 3 < cPoints; i += 3) {
        POINT p0 = lppt[i], p1 = lppt[i + 1], p2 = lppt[i + 2], p3 = lppt[i + 3];
        POINT prev = p0;
        MoveToEx(hdc, prev.x, prev.y, nullptr);
        const int steps = 30;
        for (int s = 1; s <= steps; ++s) {
            double t = (double)s / steps;
            double u = 1.0 - t;
            double x = u * u * u * p0.x + 3 * u * u * t * p1.x + 3 * u * t * t * p2.x + t * t * t * p3.x;
            double y = u * u * u * p0.y + 3 * u * u * t * p1.y + 3 * u * t * t * p2.y + t * t * t * p3.y;
            LineTo(hdc, (int)std::round(x), (int)std::round(y));
        }
    }
    return TRUE;
}

// ============================================================================
// FloodFill & ExtFloodFill
// ============================================================================

BOOL ExtFloodFill(HDC hdc, int nXStart, int nYStart, COLORREF crColor, UINT fuFillType) {
    GDISurface* s = GetDCSurface(hdc);
    if (!s) return FALSE;
    if (nXStart < 0 || nXStart >= s->width || nYStart < 0 || nYStart >= s->height) return FALSE;

    GDIBrush* brush = GetDCBrush(hdc);
    Pixel32 fillColor = brush ? ColorRefToPixel32(brush->color) : 0xFFFFFFFF;
    Pixel32 targetPx = ColorRefToPixel32(crColor);
    Pixel32 startPx = s->GetPixel(nXStart, nYStart);

    if (fuFillType == FLOODFILLSURFACE) {
        if (startPx != targetPx || startPx == fillColor) return TRUE;
    } else { // FLOODFILLBORDER
        if (startPx == targetPx || startPx == fillColor) return TRUE;
    }

    std::queue<std::pair<int, int>> q;
    q.push({nXStart, nYStart});
    s->SetPixel(nXStart, nYStart, fillColor);

    const int dx[] = {1, -1, 0, 0};
    const int dy[] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= s->width || ny < 0 || ny >= s->height) continue;

            Pixel32 cur = s->GetPixel(nx, ny);
            bool shouldFill = false;
            if (fuFillType == FLOODFILLSURFACE) {
                shouldFill = (cur == startPx && cur != fillColor);
            } else {
                shouldFill = (cur != targetPx && cur != fillColor);
            }

            if (shouldFill) {
                s->SetPixel(nx, ny, fillColor);
                q.push({nx, ny});
            }
        }
    }
    return TRUE;
}

// ============================================================================
// Rect Helpers: FillRect, FrameRect, InvertRect, DrawFocusRect
// ============================================================================

int FillRect(HDC hDC, const RECT* lprc, HBRUSH hbr) {
    if (!lprc) return 0;
    GDISurface* s = GetDCSurface(hDC);
    if (!s) return 0;
    HBRUSH old = (HBRUSH)SelectObject(hDC, hbr);
    GDIBrush* b = GetDCBrush(hDC);
    Pixel32 c = b ? ColorRefToPixel32(b->color) : 0xFFFFFFFF;
    for (int y = lprc->top; y < lprc->bottom; ++y) {
        for (int x = lprc->left; x < lprc->right; ++x) {
            s->SetPixel(x, y, c);
        }
    }
    SelectObject(hDC, old);
    return 1;
}

int FrameRect(HDC hDC, const RECT* lprc, HBRUSH hbr) {
    if (!lprc) return 0;
    GDISurface* s = GetDCSurface(hDC);
    if (!s) return 0;
    HBRUSH old = (HBRUSH)SelectObject(hDC, hbr);
    GDIBrush* b = GetDCBrush(hDC);
    Pixel32 c = b ? ColorRefToPixel32(b->color) : 0xFF000000;
    for (int x = lprc->left; x < lprc->right; ++x) {
        s->SetPixel(x, lprc->top, c);
        s->SetPixel(x, lprc->bottom - 1, c);
    }
    for (int y = lprc->top; y < lprc->bottom; ++y) {
        s->SetPixel(lprc->left, y, c);
        s->SetPixel(lprc->right - 1, y, c);
    }
    SelectObject(hDC, old);
    return 1;
}

BOOL InvertRect(HDC hDC, const RECT* lprc) {
    return PatBlt(hDC, lprc->left, lprc->top, lprc->right - lprc->left, lprc->bottom - lprc->top, DSTINVERT);
}

BOOL DrawFocusRect(HDC hDC, const RECT* lprc) {
    if (!lprc) return FALSE;
    GDISurface* s = GetDCSurface(hDC);
    if (!s) return FALSE;
    // Dotted XOR border
    for (int x = lprc->left; x < lprc->right; x += 2) {
        DrawPixelROP2(s, x, lprc->top, 0xFFFFFFFF, R2_XORPEN);
        DrawPixelROP2(s, x, lprc->bottom - 1, 0xFFFFFFFF, R2_XORPEN);
    }
    for (int y = lprc->top; y < lprc->bottom; y += 2) {
        DrawPixelROP2(s, lprc->left, y, 0xFFFFFFFF, R2_XORPEN);
        DrawPixelROP2(s, lprc->right - 1, y, 0xFFFFFFFF, R2_XORPEN);
    }
    return TRUE;
}

int FillRgn(HDC hdc, HRGN hrgn, HBRUSH hbr) {
    if (!hrgn || HandleType(hrgn) != OBJ_REGION) return 0;
    uint32_t idx = HandleIndex(hrgn);
    if (idx >= g_rgns.size() || !g_rgns[idx]) return 0;
    if (g_rgns[idx]->type == 0) {
        return FillRect(hdc, &g_rgns[idx]->rect, hbr);
    } else {
        HBRUSH old = (HBRUSH)SelectObject(hdc, hbr);
        HPEN oldP = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
        Polygon(hdc, g_rgns[idx]->polyPoints.data(), g_rgns[idx]->polyPoints.size());
        SelectObject(hdc, old);
        SelectObject(hdc, oldP);
        return 1;
    }
}

// ============================================================================
// Bitmap Decoding & Loading (BMP 1, 4, 8, 24, 32 bpp)
// ============================================================================

HBITMAP LoadBitmapFromMemory(const void* data, size_t size) {
    if (!data || size < 54) return nullptr;
    const uint8_t* p = (const uint8_t*)data;
    if (p[0] != 'B' || p[1] != 'M') return nullptr;

    uint32_t offset = *(const uint32_t*)(p + 10);
    int32_t width = *(const int32_t*)(p + 18);
    int32_t height = *(const int32_t*)(p + 22);
    uint16_t bpp = *(const uint16_t*)(p + 28);
    uint32_t compression = *(const uint32_t*)(p + 30);

    bool topDown = (height < 0);
    height = std::abs(height);

    auto bmp = std::make_unique<GDIBitmap>();
    bmp->width = width;
    bmp->height = height;
    bmp->bpp = bpp;
    bmp->surface.Resize(width, height);

    // Read palette if <= 8 bpp
    std::vector<Pixel32> pal;
    if (bpp <= 8) {
        int colors = 1 << bpp;
        const uint8_t* palData = p + 14 + *(const uint32_t*)(p + 14);
        for (int i = 0; i < colors; ++i) {
            uint8_t b = palData[i * 4 + 0];
            uint8_t g = palData[i * 4 + 1];
            uint8_t r = palData[i * 4 + 2];
            pal.push_back(0xFF000000 | (r << 16) | (g << 8) | b);
        }
    }

    int rowStride = ((width * bpp + 31) / 32) * 4;
    const uint8_t* pixelsData = p + offset;

    for (int y = 0; y < height; ++y) {
        int srcY = topDown ? y : (height - 1 - y);
        const uint8_t* row = pixelsData + srcY * rowStride;

        if (bpp == 4) {
            for (int x = 0; x < width; ++x) {
                uint8_t byte = row[x / 2];
                uint8_t cidx = (x % 2 == 0) ? (byte >> 4) : (byte & 0x0F);
                bmp->surface.SetPixel(x, y, (cidx < pal.size()) ? pal[cidx] : 0xFF000000);
            }
        } else if (bpp == 8) {
            for (int x = 0; x < width; ++x) {
                uint8_t cidx = row[x];
                bmp->surface.SetPixel(x, y, (cidx < pal.size()) ? pal[cidx] : 0xFF000000);
            }
        } else if (bpp == 24) {
            for (int x = 0; x < width; ++x) {
                uint8_t b = row[x * 3 + 0];
                uint8_t g = row[x * 3 + 1];
                uint8_t r = row[x * 3 + 2];
                bmp->surface.SetPixel(x, y, 0xFF000000 | (r << 16) | (g << 8) | b);
            }
        } else if (bpp == 32) {
            for (int x = 0; x < width; ++x) {
                uint8_t b = row[x * 4 + 0];
                uint8_t g = row[x * 4 + 1];
                uint8_t r = row[x * 4 + 2];
                bmp->surface.SetPixel(x, y, 0xFF000000 | (r << 16) | (g << 8) | b);
            }
        } else if (bpp == 1) {
            for (int x = 0; x < width; ++x) {
                uint8_t bit = (row[x / 8] >> (7 - (x % 8))) & 1;
                bmp->surface.SetPixel(x, y, (bit < pal.size()) ? pal[bit] : (bit ? 0xFFFFFFFF : 0xFF000000));
            }
        }
    }

    g_bitmaps.push_back(std::move(bmp));
    return (HBITMAP)MakeHandle(OBJ_BITMAP, g_bitmaps.size() - 1);
}

HBITMAP LoadBitmapFromFile(const char* filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) return nullptr;
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return LoadBitmapFromMemory(buffer.data(), buffer.size());
}

// ============================================================================
// DIB Functions (GetDIBits, SetDIBits, SetDIBitsToDevice, StretchDIBits)
// ============================================================================

int GetDIBits(HDC hdc, HBITMAP hbm, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uUsage) {
    GDIBitmap* bmp = GetBitmapObj(hbm);
    if (!bmp || !lpbmi) return 0;

    lpbmi->bmiHeader.biWidth = bmp->width;
    lpbmi->bmiHeader.biHeight = bmp->height;
    lpbmi->bmiHeader.biPlanes = 1;
    lpbmi->bmiHeader.biBitCount = bmp->bpp;
    lpbmi->bmiHeader.biCompression = BI_RGB;
    lpbmi->bmiHeader.biSizeImage = bmp->surface.stride * bmp->height;

    if (!lpvBits) return cScanLines;

    int rowBytes = bmp->surface.stride;
    for (UINT i = 0; i < cScanLines; ++i) {
        int y = bmp->height - 1 - (uStartScan + i);
        if (y < 0 || y >= bmp->height) continue;
        uint8_t* dst = (uint8_t*)lpvBits + i * rowBytes;
        const uint8_t* src = (const uint8_t*)bmp->surface.buffer + y * rowBytes;
        std::memcpy(dst, src, rowBytes);
    }
    return cScanLines;
}

int SetDIBits(HDC hdc, HBITMAP hbm, UINT uStartScan, UINT cScanLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT fuColorUse) {
    GDIBitmap* bmp = GetBitmapObj(hbm);
    if (!bmp || !lpbmi || !lpvBits) return 0;
    int bpp = lpbmi->bmiHeader.biBitCount;
    int w = lpbmi->bmiHeader.biWidth;
    int rowStride = ((w * bpp + 31) / 32) * 4;

    for (UINT i = 0; i < cScanLines; ++i) {
        int y = bmp->height - 1 - (uStartScan + i);
        if (y < 0 || y >= bmp->height) continue;
        const uint8_t* row = (const uint8_t*)lpvBits + i * rowStride;
        for (int x = 0; x < w && x < bmp->width; ++x) {
            if (bpp == 32) {
                Pixel32 p = *(const Pixel32*)(row + x * 4);
                bmp->surface.SetPixel(x, y, p);
            } else if (bpp == 24) {
                uint8_t b = row[x * 3 + 0];
                uint8_t g = row[x * 3 + 1];
                uint8_t r = row[x * 3 + 2];
                bmp->surface.SetPixel(x, y, 0xFF000000 | (r << 16) | (g << 8) | b);
            }
        }
    }
    return cScanLines;
}

int SetDIBitsToDevice(HDC hdc, int xDest, int yDest, DWORD w, DWORD h, int xSrc, int ySrc, UINT StartScan, UINT cLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT ColorUse) {
    GDISurface* s = GetDCSurface(hdc);
    if (!s || !lpvBits || !lpbmi) return 0;
    int bpp = lpbmi->bmiHeader.biBitCount;
    int rowStride = ((w * bpp + 31) / 32) * 4;
    for (UINT i = 0; i < cLines; ++i) {
        int dy = yDest + i;
        if (dy < 0 || dy >= s->height) continue;
        int sy = cLines - 1 - i;
        const uint8_t* row = (const uint8_t*)lpvBits + sy * rowStride;
        for (DWORD x = 0; x < w; ++x) {
            int dx = xDest + x;
            if (dx < 0 || dx >= s->width) continue;
            if (bpp == 32) {
                s->SetPixel(dx, dy, *(const Pixel32*)(row + x * 4));
            } else if (bpp == 24) {
                s->SetPixel(dx, dy, 0xFF000000 | (row[x * 3 + 2] << 16) | (row[x * 3 + 1] << 8) | row[x * 3]);
            }
        }
    }
    return cLines;
}

int StretchDIBits(HDC hdc, int XDest, int YDest, int nDestWidth, int nDestHeight,
                  int XSrc, int YSrc, int nSrcWidth, int nSrcHeight,
                  const void* lpBits, const BITMAPINFO* lpBitsInfo, UINT iUsage, DWORD dwRop) {
    if (!lpBits || !lpBitsInfo) return 0;
    HBITMAP hbmp = CreateDIBitmap(hdc, &lpBitsInfo->bmiHeader, CBM_INIT, lpBits, lpBitsInfo, iUsage);
    HDC hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hbmp);
    StretchBlt(hdc, XDest, YDest, nDestWidth, nDestHeight, hdcMem, XSrc, YSrc, nSrcWidth, nSrcHeight, dwRop);
    DeleteDC(hdcMem);
    DeleteObject(hbmp);
    return nDestHeight;
}

UINT GetDIBColorTable(HDC hdc, UINT iStartMap, UINT cEntries, RGBQUAD* prgbq) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return 0;
    GDIBitmap* bmp = GetBitmapObj(dc->hBitmap);
    if (!bmp || !prgbq) return 0;
    UINT count = 0;
    for (UINT i = 0; i < cEntries && (iStartMap + i) < bmp->palette.size(); ++i) {
        prgbq[i] = bmp->palette[iStartMap + i];
        count++;
    }
    return count;
}

UINT SetDIBColorTable(HDC hdc, UINT iStartMap, UINT cEntries, const RGBQUAD* prgbq) {
    GDIDC* dc = GetDC(hdc);
    if (!dc) return 0;
    GDIBitmap* bmp = GetBitmapObj(dc->hBitmap);
    if (!bmp || !prgbq) return 0;
    if (iStartMap + cEntries > bmp->palette.size()) bmp->palette.resize(iStartMap + cEntries);
    for (UINT i = 0; i < cEntries; ++i) {
        bmp->palette[iStartMap + i] = prgbq[i];
    }
    return cEntries;
}

// ============================================================================
// Built-in 8x16 Bitmap Font Renderer for TextOut, DrawText
// ============================================================================

// Standard VGA 8x16 font glyph table for ASCII 32..126
#include "font8x16.inl"

BOOL TextOutA(HDC hdc, int nXStart, int nYStart, LPCSTR lpString, int cbString) {
    if (!lpString || cbString <= 0) return FALSE;
    GDISurface* s = GetDCSurface(hdc);
    GDIDC* dc = GetDC(hdc);
    if (!s || !dc) return FALSE;

    Pixel32 textC = ColorRefToPixel32(dc->textColor);
    Pixel32 bkC = ColorRefToPixel32(dc->bkColor);

    int curX = nXStart;
    for (int i = 0; i < cbString; ++i) {
        unsigned char ch = (unsigned char)lpString[i];
        if (ch < 32 || ch > 126) ch = '?';
        const uint8_t* glyph = &font8x16[(ch - 32) * 16];

        for (int row = 0; row < 16; ++row) {
            int py = nYStart + row;
            if (py < 0 || py >= s->height) continue;
            uint8_t bits = glyph[row];
            for (int col = 0; col < 8; ++col) {
                int px = curX + col;
                if (px < 0 || px >= s->width) continue;
                if ((bits >> (7 - col)) & 1) {
                    s->SetPixel(px, py, textC);
                } else if (dc->bkMode == OPAQUE) {
                    s->SetPixel(px, py, bkC);
                }
            }
        }
        curX += 8;
    }
    return TRUE;
}

BOOL ExtTextOutA(HDC hdc, int X, int Y, UINT fuOptions, const RECT* lprc, LPCSTR lpString, UINT cbCount, const int* lpDx) {
    if ((fuOptions & ETO_OPAQUE) && lprc) {
        GDIDC* dc = GetDC(hdc);
        if (dc) {
            HBRUSH br = CreateSolidBrush(dc->bkColor);
            FillRect(hdc, lprc, br);
            DeleteObject(br);
        }
    }
    return TextOutA(hdc, X, Y, lpString, cbCount);
}

int DrawTextA(HDC hDC, LPCSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat) {
    if (!lpchText || !lpRect) return 0;
    if (nCount < 0) nCount = (int)std::strlen(lpchText);

    int textW = nCount * 8;
    int textH = 16;
    int x = lpRect->left;
    int y = lpRect->top;

    if (uFormat & DT_CENTER) {
        x = lpRect->left + (lpRect->right - lpRect->left - textW) / 2;
    } else if (uFormat & DT_RIGHT) {
        x = lpRect->right - textW;
    }
    if (uFormat & DT_VCENTER) {
        y = lpRect->top + (lpRect->bottom - lpRect->top - textH) / 2;
    }

    TextOutA(hDC, x, y, lpchText, nCount);
    return textH;
}

BOOL TabbedTextOutA(HDC hDC, int X, int Y, LPCSTR lpString, int nCount, int nTabPositions, const int* lpnTabStopPositions, int nTabOrigin) {
    return TextOutA(hDC, X, Y, lpString, nCount);
}
