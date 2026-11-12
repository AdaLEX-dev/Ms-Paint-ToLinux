#define NOMINMAX
#include "gdi_rasterizer.h"
#include "afxwin.h"
#include "afxext.h"
#include "afxcmn.h"
#include "afxole.h"
#include "resource.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

// ============================================================================
// Global command-line and WIA format definitions
// ============================================================================

int __argc = 1;
char** __argv = nullptr;

extern const GUID WiaImgFmt_UNDEFINED = {0xb96b3ca9, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_RAWRGB    = {0xb96b3caa, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_MEMORYBMP = {0xb96b3cab, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_BMP       = {0xb96b3cab, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_EMF       = {0xb96b3cac, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_WMF       = {0xb96b3cad, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_JPEG      = {0xb96b3cae, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_PNG       = {0xb96b3caf, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_GIF       = {0xb96b3cb0, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_TIFF      = {0xb96b3cb1, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};
extern const GUID WiaImgFmt_PHOTOCD   = {0xb96b3cb2, 0x0728, 0x11d3, {0x9d, 0x7b, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e}};

BOOL ExtTextOut(HDC hdc, int x, int y, UINT options, const RECT* lprect, LPCTSTR lpString, UINT c, const int* lpDx) {
    return ExtTextOutA(hdc, x, y, options, lprect, lpString, c, lpDx);
}

BOOL TextOut(HDC hdc, int x, int y, LPCTSTR lpString, int c) {
    return TextOutA(hdc, x, y, lpString, c);
}

char* _itoa(int value, char* str, int radix) {
    if (radix == 10) std::snprintf(str, 32, "%d", value);
    else if (radix == 16) std::snprintf(str, 32, "%x", value);
    else std::snprintf(str, 32, "%d", value);
    return str;
}

void _tsplitpath(const char* path, char* drive, char* dir, char* fname, char* ext) {
    if (drive) drive[0] = '\0';
    if (!path) {
        if (dir) dir[0] = '\0';
        if (fname) fname[0] = '\0';
        if (ext) ext[0] = '\0';
        return;
    }
    std::string s(path);
    size_t slash = s.find_last_of("/\\");
    std::string d, f;
    if (slash != std::string::npos) {
        d = s.substr(0, slash + 1);
        f = s.substr(slash + 1);
    } else {
        d = "";
        f = s;
    }
    if (dir) std::strcpy(dir, d.c_str());
    size_t dot = f.find_last_of('.');
    if (dot != std::string::npos) {
        if (fname) {
            std::string fn = f.substr(0, dot);
            std::strcpy(fname, fn.c_str());
        }
        if (ext) {
            std::string ex = f.substr(dot);
            std::strcpy(ext, ex.c_str());
        }
    } else {
        if (fname) std::strcpy(fname, f.c_str());
        if (ext) ext[0] = '\0';
    }
}

// ============================================================================
// Win32 Global Utilities & Memory
// ============================================================================

HGLOBAL GlobalAlloc(UINT uFlags, size_t dwBytes) {
    void* p = std::malloc(dwBytes ? dwBytes : 1);
    if (p && (uFlags & GMEM_ZEROINIT)) {
        std::memset(p, 0, dwBytes);
    }
    return (HGLOBAL)p;
}

HGLOBAL GlobalReAlloc(HGLOBAL hMem, size_t dwBytes, UINT uFlags) {
    return (HGLOBAL)std::realloc(hMem, dwBytes);
}

void* GlobalLock(HGLOBAL hMem) { return (void*)hMem; }
BOOL GlobalUnlock(HGLOBAL hMem) { return TRUE; }
HGLOBAL GlobalFree(HGLOBAL hMem) { if (hMem) std::free((void*)hMem); return nullptr; }

DWORD GetTickCount() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (DWORD)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

short GetKeyState(int nVirtKey) {
    // Basic stub; Wayland key state will update this if needed
    return 0;
}

BOOL IsClipboardFormatAvailable(UINT format) { return FALSE; }
UINT RegisterClipboardFormat(LPCSTR lpszFormat) { return 0xC001; }

DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer) {
    const char* tmp = "/tmp";
    if (lpBuffer && nBufferLength > 4) {
        std::strcpy(lpBuffer, tmp);
        return 4;
    }
    return 0;
}

UINT GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique, LPTSTR lpTempFileName) {
    static int s_counter = 1;
    if (lpTempFileName) {
        std::snprintf(lpTempFileName, 260, "/tmp/mspaint_tmp_%d.tmp", s_counter++);
    }
    return 1;
}

void MessageBeep(UINT uType) {}
int MessageBox(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    std::cout << "[Paint Message] " << (lpCaption ? lpCaption : "Paint") << ": " << (lpText ? lpText : "") << std::endl;
    return IDOK;
}

// ============================================================================
// Resources (Bitmaps, Cursors, Icons, Strings)
// ============================================================================

int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int cchBufferMax) {
    if (!lpBuffer || cchBufferMax <= 0) return 0;
    const char* str = "";
    switch (uID) {
        case 0: str = "untitled"; break;
        case 1: str = "Paint"; break;
        default: str = "Paint"; break;
    }
    std::strncpy(lpBuffer, str, cchBufferMax);
    lpBuffer[cchBufferMax - 1] = '\0';
    return (int)std::strlen(lpBuffer);
}

HCURSOR LoadCursor(HINSTANCE hInstance, LPCSTR lpCursorName) { return (HCURSOR)1; }
HICON LoadIcon(HINSTANCE hInstance, LPCSTR lpIconName) { return (HICON)1; }
HCURSOR SetCursor(HCURSOR hCursor) { return hCursor; }
int ShowCursor(BOOL bShow) { return 0; }

// ============================================================================
// Window Hierarchy & Win32 User Subsystem
// ============================================================================

struct Win32Wnd {
    HWND hWnd = nullptr;
    CWnd* pWnd = nullptr;
    HWND hParent = nullptr;
    std::vector<HWND> children;
    RECT rectClient = {0, 0, 100, 100};
    RECT rectWindow = {0, 0, 100, 100};
    DWORD dwStyle = WS_VISIBLE | WS_CHILD;
    DWORD dwExStyle = 0;
    BOOL bVisible = TRUE;
    BOOL bEnabled = TRUE;
    std::string title;
    HDC hdcClient = nullptr;
};

static std::map<HWND, Win32Wnd> g_windows;
static uintptr_t s_hwndCounter = 100;
static HWND s_hFocusWnd = nullptr;
static HWND s_hCaptureWnd = nullptr;

HWND GetDesktopWindow() { return (HWND)1; }
BOOL IsWindow(HWND hWnd) { return hWnd != nullptr && g_windows.find(hWnd) != g_windows.end(); }

HWND GetCapture() { return s_hCaptureWnd; }
HWND SetCapture(HWND hWnd) {
    HWND old = s_hCaptureWnd;
    s_hCaptureWnd = hWnd;
    return old;
}
BOOL ReleaseCapture() {
    s_hCaptureWnd = nullptr;
    return TRUE;
}

HDC GetDC(HWND hWnd) {
    auto it = g_windows.find(hWnd);
    if (it == g_windows.end()) {
        static HDC s_desktopDC = nullptr;
        if (!s_desktopDC) s_desktopDC = CreateCompatibleDC(nullptr);
        return s_desktopDC;
    }
    int w = it->second.rectClient.right - it->second.rectClient.left;
    int h = it->second.rectClient.bottom - it->second.rectClient.top;
    if (w < 1) w = 800;
    if (h < 1) h = 600;

    if (!it->second.hdcClient) {
        it->second.hdcClient = CreateCompatibleDC(nullptr);
        HBITMAP hbm = CreateCompatibleBitmap(it->second.hdcClient, w, h);
        SelectObject(it->second.hdcClient, hbm);
    } else {
        GDISurface* surf = GetDCSurface(it->second.hdcClient);
        if (surf && (surf->width < w || surf->height < h)) {
            HBITMAP hbm = CreateCompatibleBitmap(it->second.hdcClient, w, h);
            SelectObject(it->second.hdcClient, hbm);
        }
    }
    return it->second.hdcClient;
}

int ReleaseDC(HWND hWnd, HDC hDC) { return 1; }

BOOL InvalidateRect(HWND hWnd, const RECT* lpRect, BOOL bErase) {
    auto it = g_windows.find(hWnd);
    if (it != g_windows.end() && it->second.pWnd) {
        it->second.pWnd->SendMessage(WM_PAINT, 0, 0);
    }
    return TRUE;
}

LONG GetWindowLong(HWND hWnd, int nIndex) { return 0; }
LONG SetWindowLong(HWND hWnd, int nIndex, LONG dwNewLong) { return 0; }
BOOL SetWindowText(HWND hWnd, LPCSTR lpString) {
    auto it = g_windows.find(hWnd);
    if (it != g_windows.end()) it->second.title = lpString ? lpString : "";
    return TRUE;
}

BOOL GetClassInfo(HINSTANCE hInstance, LPCSTR lpClassName, WNDCLASS* lpWndClass) {
    if (lpWndClass) std::memset(lpWndClass, 0, sizeof(*lpWndClass));
    return TRUE;
}

BOOL GetCursorPos(LPPOINT lpPoint) {
    if (lpPoint) { lpPoint->x = 0; lpPoint->y = 0; }
    return TRUE;
}

// System Colors (Windows XP Classic Theme)
DWORD GetSysColor(int nIndex) {
    switch (nIndex) {
        case COLOR_WINDOW: return RGB(255, 255, 255);
        case COLOR_WINDOWTEXT: return RGB(0, 0, 0);
        case COLOR_BTNFACE: return RGB(236, 233, 216); // Authentic XP BTNFACE #ECE9D8
        case COLOR_BTNSHADOW: return RGB(172, 168, 153);
        case COLOR_BTNHIGHLIGHT: return RGB(255, 255, 255);
        case COLOR_BTNTEXT: return RGB(0, 0, 0);
        case COLOR_HIGHLIGHT: return RGB(49, 106, 197);
        case COLOR_HIGHLIGHTTEXT: return RGB(255, 255, 255);
        case COLOR_3DDKSHADOW: return RGB(113, 111, 100);
        case COLOR_3DLIGHT: return RGB(241, 239, 226);
        case COLOR_APPWORKSPACE: return RGB(128, 128, 128);
        default: return RGB(236, 233, 216);
    }
}

int GetSystemMetrics(int nIndex) {
    switch (nIndex) {
        case SM_CXSCREEN: return 1280;
        case SM_CYSCREEN: return 800;
        case SM_CXVSCROLL: return 16;
        case SM_CYHSCROLL: return 16;
        case SM_CXFRAME: return 4;
        case SM_CYFRAME: return 4;
        case SM_CYCAPTION: return 22;
        case SM_CYMENU: return 20;
        default: return 0;
    }
}

// Message pump stubs
BOOL PeekMessage(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) { return FALSE; }
BOOL TranslateMessage(const MSG* lpMsg) { return TRUE; }
LRESULT DispatchMessage(const MSG* lpMsg) { return 0; }

// ============================================================================
// CWnd Implementation
// ============================================================================

#undef wndTop
#undef wndTopMost

const CWnd CWnd::wndTop;
const CWnd CWnd::wndTopMost;
#define SIZE_RESTORED 0

CRuntimeClass CWnd::classCWnd = { "CWnd", sizeof(CWnd), 0xFFFF, nullptr, &CCmdTarget::classCCmdTarget };
CRuntimeClass* CWnd::GetRuntimeClass() const { return &classCWnd; }
const AFX_MSGMAP* CWnd::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CWnd::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CCmdTarget::GetThisMessageMap, entries };
    return &map;
}

CWnd::CWnd() {
    m_hWnd = nullptr;
}

CWnd::~CWnd() {
    if (m_hWnd) {
        g_windows.erase(m_hWnd);
        m_hWnd = nullptr;
    }
}

BOOL CWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                  const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext) {
    return CreateEx(0, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                    DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext) {
    if (!m_hWnd) {
        m_hWnd = (HWND)(s_hwndCounter++);
    }
    m_pParent = pParentWnd;
    std::cout << "[CreateEx] hWnd=" << m_hWnd 
              << " class=" << (GetRuntimeClass() ? GetRuntimeClass()->m_lpszClassName : (lpszClassName ? lpszClassName : "null"))
              << " name=" << (lpszWindowName ? lpszWindowName : "null")
              << " parent=" << (pParentWnd ? pParentWnd->m_hWnd : nullptr) << std::endl;
    m_dwStyle = dwStyle;
    m_dwExStyle = dwExStyle;
    Win32Wnd& w = g_windows[m_hWnd];
    w.hWnd = m_hWnd;
    w.pWnd = this;
    w.hParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    w.rectClient = rect;
    w.rectWindow = rect;
    w.dwStyle = dwStyle;
    w.dwExStyle = dwExStyle;
    w.bVisible = (dwStyle & WS_VISIBLE) != 0;
    w.title = lpszWindowName ? lpszWindowName : "";
    if (pParentWnd) {
        g_windows[pParentWnd->m_hWnd].children.push_back(m_hWnd);
    }

    CREATESTRUCT cs;
    std::memset(&cs, 0, sizeof(cs));
    cs.lpCreateParams = pContext;
    cs.hInstance = (HINSTANCE)1;
    cs.hMenu = (HMENU)(uintptr_t)nID;
    cs.hwndParent = w.hParent;
    cs.cy = rect.bottom - rect.top;
    cs.cx = rect.right - rect.left;
    cs.y = rect.top;
    cs.x = rect.left;
    cs.style = dwStyle;
    cs.lpszName = lpszWindowName;
    cs.lpszClass = lpszClassName;
    cs.dwExStyle = dwExStyle;

    if (SendMessage(WM_CREATE, 0, (LPARAM)&cs) == -1) {
        return FALSE;
    }
    return TRUE;
}

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                    DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                    HWND hWndParent, HMENU nIDorHMenu, void* lpParam) {
    CRect rc(x, y, x + nWidth, y + nHeight);
    CWnd* pParent = FromHandle(hWndParent);
    return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rc, pParent, (UINT)(uintptr_t)nIDorHMenu, lpParam);
}

BOOL CWnd::DestroyWindow() {
    if (m_hWnd) {
        OnDestroy();
        g_windows.erase(m_hWnd);
        m_hWnd = nullptr;
    }
    return TRUE;
}

BOOL CWnd::ShowWindow(int nCmdShow) {
    bool bVis = (nCmdShow != SW_HIDE);
    if (bVis) m_dwStyle |= WS_VISIBLE;
    else m_dwStyle &= ~WS_VISIBLE;

    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) {
        it->second.bVisible = bVis;
        if (bVis) it->second.dwStyle |= WS_VISIBLE;
        else it->second.dwStyle &= ~WS_VISIBLE;
    }
    return TRUE;
}

DWORD CWnd::GetStyle() const {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) return it->second.dwStyle;
    return m_dwStyle;
}

DWORD CWnd::GetExStyle() const {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) return it->second.dwExStyle;
    return m_dwExStyle;
}

BOOL CWnd::IsWindowVisible() const {
    if (!this) return FALSE;
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) return it->second.bVisible;
    return (m_dwStyle & WS_VISIBLE) != 0;
}

BOOL CWnd::IsWindowEnabled() const {
    if (!this) return FALSE;
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) return (it->second.dwStyle & WS_DISABLED) == 0;
    return (m_dwStyle & WS_DISABLED) == 0;
}

void CWnd::UpdateWindow() {
    OnPaint();
}

void CWnd::Invalidate(BOOL bErase) {
    InvalidateRect(nullptr, bErase);
}

void CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase) {
    OnPaint();
}

void CWnd::ValidateRect(LPCRECT lpRect) {}
BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, CRgn* prgnUpdate, UINT flags) {
    OnPaint();
    return TRUE;
}

void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint) {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) {
        it->second.rectWindow = { x, y, x + nWidth, y + nHeight };
        it->second.rectClient = { 0, 0, nWidth, nHeight };
        SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(nWidth, nHeight));
        if (bRepaint) SendMessage(WM_PAINT, 0, 0);
    }
}

BOOL CWnd::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags) {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) {
        if (!(nFlags & SWP_NOMOVE)) {
            it->second.rectWindow.left = x;
            it->second.rectWindow.top = y;
        }
        if (!(nFlags & SWP_NOSIZE)) {
            it->second.rectWindow.right = it->second.rectWindow.left + cx;
            it->second.rectWindow.bottom = it->second.rectWindow.top + cy;
            it->second.rectClient = { 0, 0, cx, cy };
            SendMessage(WM_SIZE, SIZE_RESTORED, MAKELPARAM(cx, cy));
        }
        if (nFlags & SWP_SHOWWINDOW) it->second.bVisible = TRUE;
        if (nFlags & SWP_HIDEWINDOW) it->second.bVisible = FALSE;
    }
    return TRUE;
}

void CWnd::GetClientRect(LPRECT lpRect) const {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end() && lpRect) {
        *lpRect = it->second.rectClient;
    } else if (lpRect) {
        *lpRect = {0, 0, 800, 600};
    }
}

void CWnd::GetWindowRect(LPRECT lpRect) const {
    if (!lpRect) return;
    auto it = g_windows.find(m_hWnd);
    if (it == g_windows.end()) {
        *lpRect = {0, 0, 800, 600};
        return;
    }
    int w = it->second.rectWindow.right - it->second.rectWindow.left;
    int h = it->second.rectWindow.bottom - it->second.rectWindow.top;
    int x = it->second.rectWindow.left;
    int y = it->second.rectWindow.top;
    HWND parent = it->second.hParent;
    while (parent) {
        auto itp = g_windows.find(parent);
        if (itp == g_windows.end()) break;
        x += itp->second.rectWindow.left;
        y += itp->second.rectWindow.top;
        parent = itp->second.hParent;
    }
    lpRect->left = x;
    lpRect->top = y;
    lpRect->right = x + w;
    lpRect->bottom = y + h;
}

void CWnd::ClientToScreen(LPPOINT lpPoint) const {
    if (!lpPoint) return;
    HWND h = m_hWnd;
    while (h) {
        auto it = g_windows.find(h);
        if (it == g_windows.end()) break;
        lpPoint->x += it->second.rectWindow.left;
        lpPoint->y += it->second.rectWindow.top;
        h = it->second.hParent;
    }
}

void CWnd::ClientToScreen(LPRECT lpRect) const {
    if (lpRect) {
        POINT p1 = { lpRect->left, lpRect->top };
        POINT p2 = { lpRect->right, lpRect->bottom };
        ClientToScreen(&p1);
        ClientToScreen(&p2);
        lpRect->left = p1.x; lpRect->top = p1.y;
        lpRect->right = p2.x; lpRect->bottom = p2.y;
    }
}

void CWnd::ScreenToClient(LPPOINT lpPoint) const {
    if (!lpPoint) return;
    HWND h = m_hWnd;
    while (h) {
        auto it = g_windows.find(h);
        if (it == g_windows.end()) break;
        lpPoint->x -= it->second.rectWindow.left;
        lpPoint->y -= it->second.rectWindow.top;
        h = it->second.hParent;
    }
}

void CWnd::ScreenToClient(LPRECT lpRect) const {
    if (lpRect) {
        POINT p1 = { lpRect->left, lpRect->top };
        POINT p2 = { lpRect->right, lpRect->bottom };
        ScreenToClient(&p1);
        ScreenToClient(&p2);
        lpRect->left = p1.x; lpRect->top = p1.y;
        lpRect->right = p2.x; lpRect->bottom = p2.y;
    }
}

BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const {
    if (lpwndpl) {
        lpwndpl->length = sizeof(WINDOWPLACEMENT);
        lpwndpl->showCmd = SW_SHOWNORMAL;
        GetWindowRect(&lpwndpl->rcNormalPosition);
    }
    return TRUE;
}

BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl) {
    if (lpwndpl) {
        MoveWindow(lpwndpl->rcNormalPosition.left, lpwndpl->rcNormalPosition.top,
                   lpwndpl->rcNormalPosition.right - lpwndpl->rcNormalPosition.left,
                   lpwndpl->rcNormalPosition.bottom - lpwndpl->rcNormalPosition.top, TRUE);
    }
    return TRUE;
}

template<typename T>
static inline T MFCCast(AFX_PMSG pfn) {
    union {
        AFX_PMSG src;
        T dst;
    } u;
    u.src = pfn;
    return u.dst;
}

static MSG s_currentMsg = {};
const MSG* GetCurrentMessage() { return &s_currentMsg; }

struct MsgScope {
    MSG old;
    MsgScope(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) : old(s_currentMsg) {
        s_currentMsg.hwnd = hWnd;
        s_currentMsg.message = msg;
        s_currentMsg.wParam = wp;
        s_currentMsg.lParam = lp;
        s_currentMsg.time = 0;
        s_currentMsg.pt = { (LONG)(short)LOWORD(lp), (LONG)(short)HIWORD(lp) };
    }
    ~MsgScope() {
        s_currentMsg = old;
    }
};

LRESULT CWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    MsgScope scope(m_hWnd, message, wParam, lParam);
    const AFX_MSGMAP* pMap = GetMessageMap();
    while (pMap) {
        const AFX_MSGMAP_ENTRY* pEntry = pMap->lpEntries;
        if (pEntry) {
            for (; pEntry->nSig != AfxSig_end; ++pEntry) {
                if (pEntry->nMessage == message) {
                    if (message == WM_COMMAND) {
                        UINT nID = LOWORD(wParam);
                        UINT nCode = HIWORD(wParam);
                        if (pEntry->nCode != nCode || nID < pEntry->nID || nID > pEntry->nLastID) {
                            continue;
                        }
                    }

                    switch (pEntry->nSig) {
                    case AfxSig_vv: {
                        auto pfn = MFCCast<void (CCmdTarget::*)()>(pEntry->pfn);
                        (this->*pfn)();
                        return 0;
                    }
                    case AfxSig_iLPCS: {
                        auto pfn = MFCCast<int (CCmdTarget::*)(LPCREATESTRUCT)>(pEntry->pfn);
                        return (this->*pfn)((LPCREATESTRUCT)lParam);
                    }
                    case AfxSig_vwp: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(UINT, CPoint)>(pEntry->pfn);
                        (this->*pfn)((UINT)wParam, CPoint((short)LOWORD(lParam), (short)HIWORD(lParam)));
                        return 0;
                    }
                    case AfxSig_bD: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)(CDC*)>(pEntry->pfn);
                        return (this->*pfn)((CDC*)wParam);
                    }
                    case AfxSig_vwii: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(UINT, int, int)>(pEntry->pfn);
                        (this->*pfn)((UINT)wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));
                        return 0;
                    }
                    case AfxSig_vii: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(int, int)>(pEntry->pfn);
                        (this->*pfn)((short)LOWORD(lParam), (short)HIWORD(lParam));
                        return 0;
                    }
                    case AfxSig_vW: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(CWnd*)>(pEntry->pfn);
                        (this->*pfn)(CWnd::FromHandle((HWND)wParam));
                        return 0;
                    }
                    case AfxSig_lwl: {
                        auto pfn = MFCCast<LRESULT (CCmdTarget::*)(WPARAM, LPARAM)>(pEntry->pfn);
                        return (this->*pfn)(wParam, lParam);
                    }
                    case AfxSig_vw: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(UINT_PTR)>(pEntry->pfn);
                        (this->*pfn)(wParam);
                        return 0;
                    }
                    case AfxSig_cmd_ex: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)(UINT)>(pEntry->pfn);
                        return (this->*pfn)((UINT)LOWORD(wParam));
                    }
                    case AfxSig_cmdui: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(CCmdUI*)>(pEntry->pfn);
                        (this->*pfn)((CCmdUI*)lParam);
                        return 0;
                    }
                    case AfxSig_bwsp: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)(UINT, short, CPoint)>(pEntry->pfn);
                        return (this->*pfn)(LOWORD(wParam), (short)HIWORD(wParam), CPoint((short)LOWORD(lParam), (short)HIWORD(lParam)));
                    }
                    case AfxSig_vwww: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(UINT, UINT, UINT)>(pEntry->pfn);
                        (this->*pfn)((UINT)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
                        return 0;
                    }
                    case AfxSig_vwsCW: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(UINT, UINT, CScrollBar*)>(pEntry->pfn);
                        (this->*pfn)(LOWORD(wParam), HIWORD(wParam), (CScrollBar*)lParam);
                        return 0;
                    }
                    case AfxSig_bWuu: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)(CWnd*, UINT, UINT)>(pEntry->pfn);
                        return (this->*pfn)(CWnd::FromHandle((HWND)wParam), LOWORD(lParam), HIWORD(lParam));
                    }
                    case AfxSig_u_p: {
                        auto pfn = MFCCast<UINT (CCmdTarget::*)(CPoint)>(pEntry->pfn);
                        return (this->*pfn)(CPoint((short)LOWORD(lParam), (short)HIWORD(lParam)));
                    }
                    case AfxSig_vb: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)(BOOL)>(pEntry->pfn);
                        return (this->*pfn)((BOOL)wParam);
                    }
                    case AfxSig_v_i_LP: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(int, void*)>(pEntry->pfn);
                        (this->*pfn)((int)wParam, (void*)lParam);
                        return 0;
                    }
                    case AfxSig_bv: {
                        auto pfn = MFCCast<BOOL (CCmdTarget::*)()>(pEntry->pfn);
                        return (this->*pfn)();
                    }
                    case AfxSig_vM: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(MINMAXINFO*)>(pEntry->pfn);
                        (this->*pfn)((MINMAXINFO*)lParam);
                        return 0;
                    }
                    case AfxSig_vS: {
                        auto pfn = MFCCast<void (CCmdTarget::*)(LPCTSTR)>(pEntry->pfn);
                        (this->*pfn)((LPCTSTR)lParam);
                        return 0;
                    }
                    default:
                        break;
                    }
                }
            }
        }
        pMap = pMap->pfnGetBaseMap ? pMap->pfnGetBaseMap() : nullptr;
    }
    return DefWindowProc(message, wParam, lParam);
}

LRESULT CWnd::SendMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    return WindowProc(message, wParam, lParam);
}

BOOL CWnd::PostMessage(UINT message, WPARAM wParam, LPARAM lParam) {
    return SendMessage(message, wParam, lParam) != 0;
}

BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam) { return FALSE; }

CWnd* CWnd::SetCapture() {
    HWND old = ::SetCapture(m_hWnd);
    return FromHandle(old);
}

CWnd* CWnd::GetCapture() { return FromHandle(::GetCapture()); }
CWnd* CWnd::SetFocus() {
    HWND old = s_hFocusWnd;
    s_hFocusWnd = m_hWnd;
    return FromHandle(old);
}
CWnd* CWnd::GetFocus() { return FromHandle(s_hFocusWnd); }

UINT_PTR CWnd::SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (*lpfnTimer)(HWND, UINT, UINT_PTR, DWORD)) {
    return nIDEvent;
}

BOOL CWnd::KillTimer(UINT_PTR nIDEvent) { return TRUE; }
BOOL CWnd::EnableWindow(BOOL bEnable) {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end()) it->second.bEnabled = bEnable;
    return TRUE;
}
BOOL CWnd::EnableScrollBar(int nSBFlags, UINT nArrowFlags) { return TRUE; }

CDC* CWnd::GetDC() {
    HDC h = ::GetDC(m_hWnd);
    return CDC::FromHandle(h);
}

int CWnd::ReleaseDC(CDC* pDC) {
    return ::ReleaseDC(m_hWnd, pDC ? pDC->m_hDC : nullptr);
}

CDC* CWnd::GetWindowDC() { return GetDC(); }
CDC* CWnd::BeginPaint(PAINTSTRUCT* lpPaint) {
    if (lpPaint) {
        std::memset(lpPaint, 0, sizeof(*lpPaint));
        lpPaint->hdc = ::GetDC(m_hWnd);
        GetClientRect(&lpPaint->rcPaint);
    }
    return CDC::FromHandle(lpPaint ? lpPaint->hdc : nullptr);
}

void CWnd::EndPaint(PAINTSTRUCT* lpPaint) {}

CFrameWnd* CWnd::GetParentFrame() const {
    auto it = g_windows.find(m_hWnd);
    if (it != g_windows.end() && it->second.hParent) {
        CWnd* p = FromHandle(it->second.hParent);
        if (p) return dynamic_cast<CFrameWnd*>(p);
    }
    return nullptr;
}

CMenu* CWnd::GetMenu() const { return nullptr; }
CWnd* CWnd::GetDlgItem(int nID) const { return nullptr; }
int CWnd::MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType) {
    return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

CWnd* CWnd::FromHandle(HWND hWnd) {
    auto it = g_windows.find(hWnd);
    return (it != g_windows.end()) ? it->second.pWnd : nullptr;
}

CWnd* CWnd::FromHandlePermanent(HWND hWnd) { return FromHandle(hWnd); }

// ============================================================================
// CFrameWnd Implementation
// ============================================================================

CRuntimeClass CFrameWnd::classCFrameWnd = { "CFrameWnd", sizeof(CFrameWnd), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CFrameWnd::GetRuntimeClass() const { return &classCFrameWnd; }
const AFX_MSGMAP* CFrameWnd::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

CFrameWnd::CFrameWnd() {}
CFrameWnd::~CFrameWnd() {}

BOOL CFrameWnd::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) {
    return Create(nullptr, "untitled - Paint", dwDefaultStyle, CRect(0, 0, 1024, 768), pParentWnd, (LPCTSTR)(uintptr_t)nIDResource, 0, pContext);
}

int CFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;
    CCreateContext* pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;
    return OnCreateClient(lpCreateStruct, pContext) ? 0 : -1;
}

BOOL CFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {
    if (pContext && pContext->m_pNewViewClass) {
        if (CreateView(pContext, AFX_IDW_PANE_FIRST) == nullptr)
            return FALSE;
    }
    return TRUE;
}

CWnd* CFrameWnd::CreateView(CCreateContext* pContext, UINT nID) {
    if (!pContext || !pContext->m_pNewViewClass) return nullptr;
    CView* pView = (CView*)pContext->m_pNewViewClass->CreateObject();
    if (!pView) return nullptr;
    pView->m_pDocument = pContext->m_pCurrentDoc;
    pView->Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 1024, 768), this, nID, pContext);
    SetActiveView(pView);
    return pView;
}

void CFrameWnd::InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible) {
    CWinApp* pApp = AfxGetApp();
    if (pApp) pApp->m_pMainWnd = this;
    if (m_pViewActive) {
        m_pViewActive->OnInitialUpdate();
    }
    RecalcLayout();
    ShowWindow(bMakeVisible ? SW_SHOWNORMAL : SW_HIDE);
    UpdateWindow();
}

void CFrameWnd::ActivateFrame(int nCmdShow) {
    ShowWindow(nCmdShow >= 0 ? nCmdShow : SW_SHOWNORMAL);
    UpdateWindow();
}

void CFrameWnd::OnSize(UINT nType, int cx, int cy) {
    CWnd::OnSize(nType, cx, cy);
    if (nType != SIZE_MINIMIZED) {
        RecalcLayout();
    }
}

void CFrameWnd::RecalcLayout(BOOL bNotify) {
    CRect rcClient;
    GetClientRect(&rcClient);
    int top = 0, bottom = rcClient.bottom, left = 0, right = rcClient.right;

    // Layout docked control bars
    for (POSITION pos = m_listControlBars.GetHeadPosition(); pos != nullptr; ) {
        CControlBar* pBar = (CControlBar*)m_listControlBars.GetNext(pos);
        if (!pBar || !(pBar->GetStyle() & WS_VISIBLE)) continue;
        DWORD dwStyle = pBar->GetBarStyle();
        if (dwStyle & CBRS_TOP) {
            CSize sz = pBar->CalcFixedLayout(FALSE, TRUE);
            pBar->MoveWindow(left, top, right - left, sz.cy);
            top += sz.cy;
        } else if (dwStyle & CBRS_BOTTOM) {
            CSize sz = pBar->CalcFixedLayout(FALSE, TRUE);
            bottom -= sz.cy;
            pBar->MoveWindow(left, bottom, right - left, sz.cy);
        } else if (dwStyle & CBRS_LEFT) {
            CSize sz = pBar->CalcFixedLayout(FALSE, FALSE);
            pBar->MoveWindow(left, top, sz.cx, bottom - top);
            left += sz.cx;
        } else if (dwStyle & CBRS_RIGHT) {
            CSize sz = pBar->CalcFixedLayout(FALSE, FALSE);
            right -= sz.cx;
            pBar->MoveWindow(right, top, sz.cx, bottom - top);
        }
    }

    if (m_pViewActive) {
        m_pViewActive->MoveWindow(left, top, right - left, bottom - top);
    }
}

void CFrameWnd::DockControlBar(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect) {
    if (!pBar) return;
    pBar->m_pDockSite = this;
    pBar->ShowWindow(SW_SHOW);
    if (m_listControlBars.Find(pBar) == nullptr) {
        m_listControlBars.AddTail(pBar);
    }
    RecalcLayout();
}

void CFrameWnd::ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay) {
    if (pBar) {
        pBar->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
        RecalcLayout();
    }
}

BOOL CFrameWnd::NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder) { return TRUE; }
CDocument* CFrameWnd::GetActiveDocument() { return m_pDocument; }
BOOL CFrameWnd::OnBarCheck(UINT nID) { return TRUE; }
void CFrameWnd::OnUpdateControlBarMenu(CCmdUI* pCmdUI) {}

// ============================================================================
// CView, CControlBar, CStatusBar, CToolBar, CDialogBar
// ============================================================================

CRuntimeClass CView::classCView = { "CView", sizeof(CView), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CView::GetRuntimeClass() const { return &classCView; }
const AFX_MSGMAP* CView::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

CRuntimeClass CControlBar::classCControlBar = { "CControlBar", sizeof(CControlBar), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CControlBar::GetRuntimeClass() const { return &classCControlBar; }
const AFX_MSGMAP* CControlBar::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CControlBar::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

CRuntimeClass CStatusBar::classCStatusBar = { "CStatusBar", sizeof(CStatusBar), 0xFFFF, nullptr, &CControlBar::classCControlBar };
CRuntimeClass* CStatusBar::GetRuntimeClass() const { return &classCStatusBar; }
const AFX_MSGMAP* CStatusBar::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CControlBar::GetThisMessageMap, entries };
    return &map;
}

BOOL CStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID) {
    m_pDockSite = dynamic_cast<CFrameWnd*>(pParentWnd);
    m_dwStyle = dwStyle | CBRS_BOTTOM;
    BOOL ok = CControlBar::Create(nullptr, "StatusBar", dwStyle | WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 24), pParentWnd, nID);
    ShowWindow(SW_SHOW);
    if (m_pDockSite) {
        if (m_pDockSite->m_listControlBars.Find(this) == nullptr) {
            m_pDockSite->m_listControlBars.AddTail(this);
        }
        m_pDockSite->RecalcLayout();
    }
    return ok;
}

BOOL CStatusBar::SetIndicators(const UINT* lpIDArray, int nIDCount) {
    m_panes.clear();
    if (lpIDArray) {
        for (int i = 0; i < nIDCount; ++i) {
            Pane p;
            p.nID = lpIDArray[i];
            p.cxText = (i == 0) ? 200 : 100;
            m_panes.push_back(p);
        }
    }
    return TRUE;
}

void CStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const {
    if (!lpRect) return;
    int x = 0;
    for (int i = 0; i < nIndex && i < (int)m_panes.size(); ++i) {
        x += m_panes[i].cxText + 4;
    }
    int w = (nIndex < (int)m_panes.size()) ? m_panes[nIndex].cxText : 100;
    *lpRect = { x, 2, x + w, 22 };
}

void CStatusBar::GetPaneText(int nIndex, CString& rString) const {
    if (nIndex >= 0 && nIndex < (int)m_panes.size()) rString = m_panes[nIndex].strText;
}

BOOL CStatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate) {
    if (nIndex >= 0 && nIndex < (int)m_panes.size()) {
        m_panes[nIndex].strText = lpszNewText ? lpszNewText : "";
        if (bUpdate) Invalidate();
        return TRUE;
    }
    return FALSE;
}

void CStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const {
    if (nIndex >= 0 && nIndex < (int)m_panes.size()) {
        nID = m_panes[nIndex].nID;
        nStyle = m_panes[nIndex].nStyle;
        cxWidth = m_panes[nIndex].cxText;
    }
}

void CStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth) {
    if (nIndex >= 0 && nIndex < (int)m_panes.size()) {
        m_panes[nIndex].nID = nID;
        m_panes[nIndex].nStyle = nStyle;
        m_panes[nIndex].cxText = cxWidth;
    }
}

CRuntimeClass CToolBar::classCToolBar = { "CToolBar", sizeof(CToolBar), 0xFFFF, nullptr, &CControlBar::classCControlBar };
const AFX_MSGMAP* CToolBar::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CControlBar::GetThisMessageMap, entries };
    return &map;
}

CRuntimeClass CDialogBar::classCDialogBar = { "CDialogBar", sizeof(CDialogBar), 0xFFFF, nullptr, &CControlBar::classCControlBar };
const AFX_MSGMAP* CDialogBar::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CControlBar::GetThisMessageMap, entries };
    return &map;
}

CRuntimeClass CDialog::classCDialog = { "CDialog", sizeof(CDialog), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CDialog::GetRuntimeClass() const { return &classCDialog; }
const AFX_MSGMAP* CDialog::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CDialog::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

int CDialog::DoModal() { return IDOK; }

CColorDialog::CColorDialog(COLORREF clrInit, DWORD dwFlags, CWnd* pParentWnd) {}
int CColorDialog::DoModal() { return IDOK; }

CRuntimeClass CEdit::classCEdit = { "CEdit", sizeof(CEdit), 0xFFFF, nullptr, &CWnd::classCWnd };
const AFX_MSGMAP* CEdit::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

BOOL CMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect) { return TRUE; }

// ============================================================================
// OLE Document & Server
// ============================================================================

void CDocument::ReleaseFile(CFile* pFile, BOOL bAbort) { if (pFile) pFile->Close(); }

const AFX_MSGMAP* CFrameWnd::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CView::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CStatusBar::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CToolBar::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CDialogBar::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CWinApp::GetMessageMap() const { return GetThisMessageMap(); }

const AFX_MSGMAP* CDocument::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CDocument::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CCmdTarget::GetThisMessageMap, entries };
    return &map;
}
CRuntimeClass CDocument::classCDocument = { "CDocument", sizeof(CDocument), 0xFFFF, nullptr, &CCmdTarget::classCCmdTarget };
CRuntimeClass* CDocument::GetRuntimeClass() const { return &classCDocument; }

const AFX_MSGMAP* COleServerDoc::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* COleIPFrameWnd::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CEdit::GetMessageMap() const { return GetThisMessageMap(); }

const AFX_MSGMAP* CComboBox::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CComboBox::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

const AFX_MSGMAP* CProgressCtrl::GetMessageMap() const { return GetThisMessageMap(); }
const AFX_MSGMAP* CProgressCtrl::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CWnd::GetThisMessageMap, entries };
    return &map;
}

CRuntimeClass* CToolBar::GetRuntimeClass() const { return &classCToolBar; }
CRuntimeClass* CDialogBar::GetRuntimeClass() const { return &classCDialogBar; }
CRuntimeClass* CEdit::GetRuntimeClass() const { return &classCEdit; }

CRuntimeClass CComboBox::classCComboBox = { "CComboBox", sizeof(CComboBox), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CComboBox::GetRuntimeClass() const { return &classCComboBox; }

CRuntimeClass CProgressCtrl::classCProgressCtrl = { "CProgressCtrl", sizeof(CProgressCtrl), 0xFFFF, nullptr, &CWnd::classCWnd };
CRuntimeClass* CProgressCtrl::GetRuntimeClass() const { return &classCProgressCtrl; }

CRuntimeClass CMenu::classCMenu = { "CMenu", sizeof(CMenu), 0xFFFF, nullptr, &CObject::classCObject };
CRuntimeClass* CMenu::GetRuntimeClass() const { return &classCMenu; }

CRuntimeClass CBrush::classCBrush = { "CBrush", sizeof(CBrush), 0xFFFF, nullptr, &CGdiObject::classCGdiObject };
CRuntimeClass* CBrush::GetRuntimeClass() const { return &classCBrush; }

CRuntimeClass CFont::classCFont = { "CFont", sizeof(CFont), 0xFFFF, nullptr, &CGdiObject::classCGdiObject };
CRuntimeClass* CFont::GetRuntimeClass() const { return &classCFont; }

CRuntimeClass CPalette::classCPalette = { "CPalette", sizeof(CPalette), 0xFFFF, nullptr, &CGdiObject::classCGdiObject };
CRuntimeClass* CPalette::GetRuntimeClass() const { return &classCPalette; }

CRuntimeClass CRgn::classCRgn = { "CRgn", sizeof(CRgn), 0xFFFF, nullptr, &CGdiObject::classCGdiObject };
CRuntimeClass* CRgn::GetRuntimeClass() const { return &classCRgn; }

CRuntimeClass* CDC::GetRuntimeClass() const { return &classCDC; }
CObject* CDC::CreateObject() { return new CDC(); }

CRuntimeClass* CGdiObject::GetRuntimeClass() const { return &classCGdiObject; }
CObject* CGdiObject::CreateObject() { return new CGdiObject(); }

CRuntimeClass CClientDC::classCClientDC = { "CClientDC", sizeof(CClientDC), 0xFFFF, nullptr, &CDC::classCDC };
CRuntimeClass* CClientDC::GetRuntimeClass() const { return &classCClientDC; }

CRuntimeClass* CPaintDC::GetRuntimeClass() const { return &classCPaintDC; }

CRuntimeClass CWindowDC::classCWindowDC = { "CWindowDC", sizeof(CWindowDC), 0xFFFF, nullptr, &CDC::classCDC };
CRuntimeClass* CWindowDC::GetRuntimeClass() const { return &classCWindowDC; }

CRuntimeClass COleServerDoc::classCOleServerDoc = { "COleServerDoc", sizeof(COleServerDoc), 0xFFFF, nullptr, &CDocument::classCDocument };
const AFX_MSGMAP* COleServerDoc::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CDocument::GetThisMessageMap, entries };
    return &map;
}
CRuntimeClass* COleServerDoc::GetRuntimeClass() const { return &classCOleServerDoc; }

CRuntimeClass COleServerItem::classCOleServerItem = { "COleServerItem", sizeof(COleServerItem), 0xFFFF, nullptr, &CCmdTarget::classCCmdTarget };
CRuntimeClass* COleServerItem::GetRuntimeClass() const { return &classCOleServerItem; }

CRuntimeClass COleIPFrameWnd::classCOleIPFrameWnd = { "COleIPFrameWnd", sizeof(COleIPFrameWnd), 0xFFFF, nullptr, &CFrameWnd::classCFrameWnd };
const AFX_MSGMAP* COleIPFrameWnd::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CFrameWnd::GetThisMessageMap, entries };
    return &map;
}
CRuntimeClass* COleIPFrameWnd::GetRuntimeClass() const { return &classCOleIPFrameWnd; }
CObject* COleIPFrameWnd::CreateObject() { return new COleIPFrameWnd(); }

CFile* CDocument::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError) {
    CFile* f = new CFile();
    if (f->Open(lpszFileName, nOpenFlags, pError)) return f;
    delete f;
    return nullptr;
}

// ============================================================================
// CDC & GDI Object Classes
// ============================================================================

CRuntimeClass CDC::classCDC = { "CDC", sizeof(CDC), 0xFFFF, nullptr, &CObject::classCObject };
CRuntimeClass CGdiObject::classCGdiObject = { "CGdiObject", sizeof(CGdiObject), 0xFFFF, nullptr, &CObject::classCObject };
CRuntimeClass CBitmap::classCBitmap = { "CBitmap", sizeof(CBitmap), 0xFFFF, nullptr, &CGdiObject::classCGdiObject };
CRuntimeClass* CBitmap::GetRuntimeClass() const { return &classCBitmap; }
CRuntimeClass CPaintDC::classCPaintDC = { "CPaintDC", sizeof(CPaintDC), 0xFFFF, nullptr, &CDC::classCDC };

BOOL CGdiObject::Attach(HGDIOBJ hObject) { m_hObject = hObject; return TRUE; }
HGDIOBJ CGdiObject::Detach() { HGDIOBJ h = m_hObject; m_hObject = nullptr; return h; }
BOOL CGdiObject::DeleteObject() { BOOL r = ::DeleteObject(m_hObject); m_hObject = nullptr; return r; }
int CGdiObject::GetObject(int nCount, LPVOID lpObject) const { return ::GetObject(m_hObject, nCount, lpObject); }

CDC::CDC() {
    m_hDC = ::CreateCompatibleDC(nullptr);
    m_bAutoDelete = TRUE;
}

CDC::~CDC() {
    if (m_bAutoDelete && m_hDC) {
        ::DeleteDC(m_hDC);
        m_hDC = nullptr;
    }
}

BOOL CDC::CreateCompatibleDC(CDC* pDC) {
    if (m_hDC && m_bAutoDelete) ::DeleteDC(m_hDC);
    m_hDC = ::CreateCompatibleDC(pDC ? pDC->m_hDC : nullptr);
    m_bAutoDelete = TRUE;
    return m_hDC != nullptr;
}

BOOL CDC::CreateIC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData) {
    return CreateCompatibleDC(nullptr);
}

BOOL CDC::DeleteDC() {
    if (m_hDC) {
        BOOL r = ::DeleteDC(m_hDC);
        m_hDC = nullptr;
        return r;
    }
    return FALSE;
}

BOOL CDC::Attach(HDC hDC) { m_hDC = hDC; m_bAutoDelete = FALSE; return TRUE; }
HDC CDC::Detach() { HDC h = m_hDC; m_hDC = nullptr; return h; }

CBitmap* CDC::SelectObject(CBitmap* pBitmap) {
    HBITMAP old = (HBITMAP)::SelectObject(m_hDC, pBitmap ? pBitmap->m_hObject : nullptr);
    return CBitmap::FromHandle(old);
}
CPen* CDC::SelectObject(CPen* pPen) {
    HPEN old = (HPEN)::SelectObject(m_hDC, pPen ? pPen->m_hObject : nullptr);
    return (CPen*)CGdiObject::FromHandle(old);
}
CBrush* CDC::SelectObject(CBrush* pBrush) {
    HBRUSH old = (HBRUSH)::SelectObject(m_hDC, pBrush ? pBrush->m_hObject : nullptr);
    return CBrush::FromHandle(old);
}
CFont* CDC::SelectObject(CFont* pFont) {
    HFONT old = (HFONT)::SelectObject(m_hDC, pFont ? pFont->m_hObject : nullptr);
    return CFont::FromHandle(old);
}
CGdiObject* CDC::SelectObject(CGdiObject* pObject) {
    HGDIOBJ old = ::SelectObject(m_hDC, pObject ? pObject->m_hObject : nullptr);
    return CGdiObject::FromHandle(old);
}
CPalette* CDC::SelectPalette(CPalette* pPalette, BOOL bForceBackground) {
    HPALETTE old = ::SelectPalette(m_hDC, pPalette ? (HPALETTE)pPalette->m_hObject : nullptr, bForceBackground);
    return CPalette::FromHandle(old);
}
CGdiObject* CDC::SelectStockObject(int nIndex) {
    HGDIOBJ h = ::GetStockObject(nIndex);
    return SelectObject(CGdiObject::FromHandle(h));
}

BOOL CDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop) {
    return ::BitBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC ? pSrcDC->m_hDC : nullptr, xSrc, ySrc, dwRop);
}
BOOL CDC::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop) {
    return ::PatBlt(m_hDC, x, y, nWidth, nHeight, dwRop);
}
BOOL CDC::StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop) {
    return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, pSrcDC ? pSrcDC->m_hDC : nullptr, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}
BOOL CDC::LineTo(int x, int y) { return ::LineTo(m_hDC, x, y); }
CPoint CDC::MoveTo(int x, int y) {
    POINT old;
    ::MoveToEx(m_hDC, x, y, &old);
    return CPoint(old.x, old.y);
}
BOOL CDC::Rectangle(int x1, int y1, int x2, int y2) { return ::Rectangle(m_hDC, x1, y1, x2, y2); }
BOOL CDC::Rectangle(LPCRECT lpRect) { return lpRect ? ::Rectangle(m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom) : FALSE; }
BOOL CDC::Polygon(LPPOINT lpPoints, int nCount) { return ::Polygon(m_hDC, lpPoints, nCount); }
BOOL CDC::Polyline(LPPOINT lpPoints, int nCount) { return ::Polyline(m_hDC, lpPoints, nCount); }
void CDC::FillRect(LPCRECT lpRect, CBrush* pBrush) { ::FillRect(m_hDC, lpRect, pBrush ? (HBRUSH)pBrush->m_hObject : nullptr); }
void CDC::FrameRect(LPCRECT lpRect, CBrush* pBrush) { ::FrameRect(m_hDC, lpRect, pBrush ? (HBRUSH)pBrush->m_hObject : nullptr); }
BOOL CDC::FillRgn(CRgn* pRgn, CBrush* pBrush) { return ::FillRgn(m_hDC, pRgn ? (HRGN)pRgn->m_hObject : nullptr, pBrush ? (HBRUSH)pBrush->m_hObject : nullptr); }
BOOL CDC::FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight) { return TRUE; }
BOOL CDC::DrawFocusRect(LPCRECT lpRect) { return ::DrawFocusRect(m_hDC, lpRect); }
BOOL CDC::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType) { return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType); }
COLORREF CDC::GetPixel(int x, int y) const { return ::GetPixel(m_hDC, x, y); }
COLORREF CDC::SetPixel(int x, int y, COLORREF crColor) { return ::SetPixel(m_hDC, x, y, crColor); }
BOOL CDC::TextOut(int x, int y, LPCTSTR lpszString, int nCount) { return ::TextOut(m_hDC, x, y, lpszString, nCount >= 0 ? nCount : (int)std::strlen(lpszString)); }
BOOL CDC::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths) {
    return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths);
}
int CDC::DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat) {
    return ::DrawTextA(m_hDC, lpszString, nCount, lpRect, nFormat);
}
CSize CDC::TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) {
    ::TabbedTextOutA(m_hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin);
    return CSize(nCount * 8, 16);
}
CSize CDC::GetTextExtent(LPCTSTR lpszString, int nCount) const {
    if (nCount < 0) nCount = lpszString ? (int)std::strlen(lpszString) : 0;
    return CSize(nCount * 8, 16);
}
BOOL CDC::GetTextMetrics(LPTEXTMETRIC lpMetrics) const {
    if (lpMetrics) {
        std::memset(lpMetrics, 0, sizeof(*lpMetrics));
        lpMetrics->tmHeight = 16;
        lpMetrics->tmAveCharWidth = 8;
    }
    return TRUE;
}
int CDC::GetDeviceCaps(int nIndex) const { return ::GetDeviceCaps(m_hDC, nIndex); }
int CDC::GetClipBox(LPRECT lpRect) const { if (lpRect) *lpRect = {0, 0, 10000, 10000}; return SIMPLEREGION; }
int CDC::IntersectClipRect(LPCRECT lpRect) { return SIMPLEREGION; }
int CDC::ExcludeClipRect(LPCRECT lpRect) { return SIMPLEREGION; }
int CDC::ExcludeUpdateRgn(CWnd* pWnd) { return SIMPLEREGION; }
int CDC::SelectClipRgn(CRgn* pRgn) { return ::SelectObject(m_hDC, pRgn ? pRgn->m_hObject : nullptr) ? SIMPLEREGION : ERROR; }
BOOL CDC::PtVisible(int x, int y) const { return TRUE; }
BOOL CDC::RectVisible(LPCRECT lpRect) const { return TRUE; }
COLORREF CDC::SetBkColor(COLORREF crColor) { return ::SetBkColor(m_hDC, crColor); }
COLORREF CDC::SetTextColor(COLORREF crColor) { return ::SetTextColor(m_hDC, crColor); }
int CDC::SetBkMode(int nBkMode) { return ::SetBkMode(m_hDC, nBkMode); }
int CDC::GetBkMode() const { return OPAQUE; }
COLORREF CDC::GetTextColor() const { return RGB(0, 0, 0); }
int CDC::SetROP2(int nDrawMode) { return ::SetROP2(m_hDC, nDrawMode); }
int CDC::SetStretchBltMode(int nStretchMode) { return ::SetStretchBltMode(m_hDC, nStretchMode); }
CPoint CDC::SetViewportOrg(int x, int y) { return CPoint(0, 0); }
CPoint CDC::OffsetViewportOrg(int nWidth, int nHeight) { return CPoint(0, 0); }
CPoint CDC::SetWindowOrg(int x, int y) { return CPoint(0, 0); }
CSize CDC::SetViewportExt(int cx, int cy) { return CSize(1, 1); }
CSize CDC::SetWindowExt(int cx, int cy) { return CSize(1, 1); }
CPoint CDC::SetBrushOrg(int x, int y) { return CPoint(0, 0); }
int CDC::SaveDC() { return 1; }
BOOL CDC::RestoreDC(int nSavedDC) { return TRUE; }
UINT CDC::RealizePalette() { return ::RealizePalette(m_hDC); }

static std::map<HDC, CDC*> s_mapCDC;
CDC* CDC::FromHandle(HDC hDC) {
    if (!hDC) return nullptr;
    auto it = s_mapCDC.find(hDC);
    if (it != s_mapCDC.end()) return it->second;
    CDC* dc = new CDC();
    dc->Attach(hDC);
    s_mapCDC[hDC] = dc;
    return dc;
}

CClientDC::CClientDC(CWnd* pWnd) {
    m_hWnd = pWnd ? pWnd->m_hWnd : nullptr;
    m_hDC = ::GetDC(m_hWnd);
    m_bAutoDelete = FALSE;
}
CClientDC::~CClientDC() {
    ::ReleaseDC(m_hWnd, m_hDC);
}

CPaintDC::CPaintDC(CWnd* pWnd) {
    m_hWnd = pWnd ? pWnd->m_hWnd : nullptr;
    if (pWnd) {
        pWnd->BeginPaint(&m_ps);
        m_hDC = m_ps.hdc;
    } else {
        m_hDC = ::GetDC(nullptr);
    }
    m_bAutoDelete = FALSE;
}
CPaintDC::~CPaintDC() {
    CWnd* pWnd = CWnd::FromHandle(m_hWnd);
    if (pWnd) {
        pWnd->EndPaint(&m_ps);
    }
    ::ReleaseDC(m_hWnd, m_hDC);
}

CWindowDC::CWindowDC(CWnd* pWnd) {
    m_hWnd = pWnd ? pWnd->m_hWnd : nullptr;
    m_hDC = ::GetDC(m_hWnd);
    m_bAutoDelete = FALSE;
}
CWindowDC::~CWindowDC() {
    ::ReleaseDC(m_hWnd, m_hDC);
}

// ============================================================================
// GDI Object Class Implementations
// ============================================================================

static std::map<HGDIOBJ, CGdiObject*> s_mapGdi;
CGdiObject* CGdiObject::FromHandle(HGDIOBJ h) {
    if (!h) return nullptr;
    auto it = s_mapGdi.find(h);
    if (it != s_mapGdi.end()) return it->second;
    CGdiObject* obj = new CGdiObject();
    obj->Attach(h);
    s_mapGdi[h] = obj;
    return obj;
}

CBitmap* CBitmap::FromHandle(HBITMAP hBitmap) {
    return (CBitmap*)CGdiObject::FromHandle((HGDIOBJ)hBitmap);
}
BOOL CBitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits) {
    m_hObject = ::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits);
    return m_hObject != nullptr;
}
BOOL CBitmap::CreateCompatibleBitmap(CDC* pDC, int nWidth, int nHeight) {
    m_hObject = ::CreateCompatibleBitmap(pDC ? pDC->m_hDC : nullptr, nWidth, nHeight);
    return m_hObject != nullptr;
}
BOOL CBitmap::LoadBitmap(LPCTSTR lpszResourceName) {
    // Check if authentic bitmap exists in xp-mspaint/res/bitmaps/
    std::string path = "xp-mspaint/res/bitmaps/";
    std::string res(lpszResourceName ? lpszResourceName : "");
    std::string file = path + res + ".bmp";
    m_hObject = LoadBitmapFromFile(file.c_str());
    return m_hObject != nullptr;
}
BOOL CBitmap::LoadBitmap(UINT nIDResource) {
    std::string path = "xp-mspaint/res/bitmaps/";
    switch (nIDResource) {
        case IDB_IMGTOOLS: path += "imgtools.bmp"; break;
        case IDB_MAINTOOLS: path += "maintool.bmp"; break;
        case IDBM_COMPWELL: path += "compwell.bmp"; break;
        case IDB_ZOOMOPT: path += "zoomopt.bmp"; break;
        case IDB_SELOPT: path += "selopt.bmp"; break;
        case IDB_AIROPT: path += "airopt.bmp"; break;
        case IDB_SBSIZE: path += "sbsize.bmp"; break;
        case IDB_SBPOS: path += "sborigin.bmp"; break;
        case IDBM_DRAGHANDLE: path += "handle.bmp"; break;
        case IDBM_DRAGHANDLE2: path += "handle2.bmp"; break;
        case IDB_ELLIPSE1: path += "ellipse1.bmp"; break;
        case IDB_ELLIPSE2: path += "ellipse2.bmp"; break;
        case IDB_ELLIPSE3: path += "ellipse3.bmp"; break;
        case IDB_ELLIPSE4: path += "ellipse4.bmp"; break;
        default: path += "imgtools.bmp"; break;
    }
    m_hObject = LoadBitmapFromFile(path.c_str());
    return m_hObject != nullptr;
}

CBrush::CBrush(COLORREF crColor) {
    m_hObject = ::CreateSolidBrush(crColor);
}
CBrush* CBrush::FromHandle(HBRUSH hBrush) {
    return (CBrush*)CGdiObject::FromHandle((HGDIOBJ)hBrush);
}
BOOL CBrush::CreateSolidBrush(COLORREF crColor) {
    m_hObject = ::CreateSolidBrush(crColor);
    return m_hObject != nullptr;
}
BOOL CBrush::CreatePatternBrush(CBitmap* pBitmap) {
    m_hObject = ::CreateSolidBrush(RGB(255, 255, 255));
    return TRUE;
}

CFont* CFont::FromHandle(HFONT hFont) {
    return (CFont*)CGdiObject::FromHandle((HGDIOBJ)hFont);
}
BOOL CFont::CreateFontIndirect(const LOGFONT* lpLogFont) {
    m_hObject = (HFONT)1;
    return TRUE;
}

CPalette* CPalette::FromHandle(HPALETTE hPalette) {
    return (CPalette*)CGdiObject::FromHandle((HGDIOBJ)hPalette);
}
BOOL CPalette::CreatePalette(LPLOGPALETTE lpLogPalette) {
    m_hObject = ::CreatePalette(lpLogPalette);
    return m_hObject != nullptr;
}
UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const {
    return ::GetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}
UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) {
    return ::SetPaletteEntries((HPALETTE)m_hObject, nStartIndex, nNumEntries, lpPaletteColors);
}
BOOL CPalette::ResizePalette(UINT nNumEntries) {
    return ::ResizePalette((HPALETTE)m_hObject, nNumEntries);
}
UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const {
    return ::GetNearestPaletteIndex((HPALETTE)m_hObject, crColor);
}

BOOL CRgn::CreateRectRgnIndirect(LPCRECT lpRect) {
    m_hObject = ::CreateRectRgnIndirect(lpRect);
    return m_hObject != nullptr;
}
BOOL CRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode) {
    m_hObject = ::CreatePolygonRgn(lpPoints, nCount, nMode);
    return m_hObject != nullptr;
}
int CRgn::OffsetRgn(int x, int y) {
    return ::OffsetRgn((HRGN)m_hObject, x, y);
}

// ============================================================================
// CFile Implementation
// ============================================================================

CFile::CFile() {}
CFile::~CFile() { Close(); }

BOOL CFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError) {
    Close();
    m_strFileName = lpszFileName ? lpszFileName : "";
    const char* mode = "rb";
    if (nOpenFlags & modeCreate) mode = (nOpenFlags & modeReadWrite) ? "w+b" : "wb";
    else if (nOpenFlags & modeWrite) mode = "wb";
    else if (nOpenFlags & modeReadWrite) mode = "r+b";
    m_fp = std::fopen(lpszFileName, mode);
    return m_fp != nullptr;
}

void CFile::Close() {
    if (m_fp) {
        std::fclose((FILE*)m_fp);
        m_fp = nullptr;
    }
}

UINT CFile::Read(void* lpBuf, UINT nCount) {
    return m_fp ? (UINT)std::fread(lpBuf, 1, nCount, (FILE*)m_fp) : 0;
}
void CFile::Write(const void* lpBuf, UINT nCount) {
    if (m_fp) std::fwrite(lpBuf, 1, nCount, (FILE*)m_fp);
}
LONG CFile::Seek(LONG lOff, UINT nFrom) {
    if (!m_fp) return 0;
    int origin = SEEK_SET;
    if (nFrom == current) origin = SEEK_CUR;
    else if (nFrom == end) origin = SEEK_END;
    std::fseek((FILE*)m_fp, lOff, origin);
    return std::ftell((FILE*)m_fp);
}
DWORD CFile::GetLength() const {
    if (!m_fp) return 0;
    long cur = std::ftell((FILE*)m_fp);
    std::fseek((FILE*)m_fp, 0, SEEK_END);
    long len = std::ftell((FILE*)m_fp);
    std::fseek((FILE*)m_fp, cur, SEEK_SET);
    return (DWORD)len;
}
DWORD CFile::GetPosition() const {
    return m_fp ? (DWORD)std::ftell((FILE*)m_fp) : 0;
}
void CFile::SetLength(DWORD dwNewLen) {
    if (m_fp) ftruncate(fileno((FILE*)m_fp), dwNewLen);
}
void CFile::Flush() {
    if (m_fp) std::fflush((FILE*)m_fp);
}
void CFile::Abort() { Close(); }
BOOL CFile::GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus) {
    struct stat st;
    if (stat(lpszFileName, &st) == 0) {
        rStatus.m_size = st.st_size;
        rStatus.m_attribute = 0;
        return TRUE;
    }
    return FALSE;
}
void CFile::Remove(LPCTSTR lpszFileName) { unlink(lpszFileName); }
void CFile::Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName) { rename(lpszOldName, lpszNewName); }

// ============================================================================
// Application & Framework Core
// ============================================================================

static CWinApp* s_pApp = nullptr;

CWinApp::CWinApp() {
    s_pApp = this;
}

POSITION CDocTemplate::GetFirstDocPosition() const {
    return m_docList.empty() ? nullptr : (POSITION)1;
}

CDocument* CDocTemplate::GetNextDoc(POSITION& rPos) const {
    if (!rPos || m_docList.empty()) return nullptr;
    CDocument* pDoc = m_docList[0];
    rPos = nullptr;
    return pDoc;
}

CDocument* CDocTemplate::CreateNewDocument() {
    if (!m_pDocClass) return nullptr;
    CDocument* pDoc = (CDocument*)m_pDocClass->CreateObject();
    if (pDoc) {
        pDoc->m_pDocTemplate = this;
        m_docList.push_back(pDoc);
    }
    return pDoc;
}

CFrameWnd* CDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther) {
    if (!m_pFrameClass) return nullptr;
    CCreateContext context;
    context.m_pCurrentDoc = pDoc;
    context.m_pNewViewClass = m_pViewClass;
    context.m_pNewDocTemplate = this;

    CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
    if (!pFrame) return nullptr;
    if (!pFrame->LoadFrame(m_nIDResource, WS_OVERLAPPEDWINDOW, nullptr, &context)) {
        return nullptr;
    }
    return pFrame;
}

void CDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible) {
    if (pFrame) {
        pFrame->InitialUpdateFrame(pDoc, bMakeVisible);
    }
}

CDocument* CDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible) {
    return nullptr;
}

CDocument* CSingleDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible) {
    CDocument* pDoc = CreateNewDocument();
    if (!pDoc) return nullptr;
    CFrameWnd* pFrame = CreateNewFrame(pDoc, nullptr);
    if (!pFrame) {
        delete pDoc;
        return nullptr;
    }
    if (lpszPathName == nullptr) {
        if (!pDoc->OnNewDocument()) {
            return nullptr;
        }
    } else {
        if (!pDoc->OnOpenDocument(lpszPathName)) {
            return nullptr;
        }
    }
    InitialUpdateFrame(pFrame, pDoc, bMakeVisible);
    return pDoc;
}

void CWinApp::AddDocTemplate(CDocTemplate* pTemplate) {
    if (pTemplate) m_docTemplates.push_back(pTemplate);
}

void CWinApp::OnFileNew() {
    if (!m_docTemplates.empty()) {
        m_docTemplates[0]->OpenDocumentFile(nullptr, TRUE);
    }
}

CDocument* CWinApp::OpenDocumentFile(LPCTSTR lpszFileName) {
    if (!m_docTemplates.empty()) {
        return m_docTemplates[0]->OpenDocumentFile(lpszFileName, TRUE);
    }
    return nullptr;
}

CWinApp* AfxGetApp() { return s_pApp; }
CWnd* AfxGetMainWnd() { return s_pApp ? s_pApp->m_pMainWnd : nullptr; }
HINSTANCE AfxGetInstanceHandle() { return (HINSTANCE)1; }
HINSTANCE AfxGetResourceHandle() { return (HINSTANCE)1; }

void AfxDumpWindows() {
    std::cout << "--- Dump All Windows (" << g_windows.size() << ") ---" << std::endl;
    for (auto& pair : g_windows) {
        const auto& w = pair.second;
        const char* className = (w.pWnd && w.pWnd->GetRuntimeClass()) ? w.pWnd->GetRuntimeClass()->m_lpszClassName : "Unknown";
        CRect rc;
        if (w.pWnd) w.pWnd->GetWindowRect(&rc);
        std::cout << "HWND=" << pair.first
                  << " Class=" << className
                  << " Title=" << w.title
                  << " Parent=" << w.hParent
                  << " Vis=" << w.bVisible
                  << " rectWindow=(" << w.rectWindow.left << "," << w.rectWindow.top
                  << " - " << w.rectWindow.right << "," << w.rectWindow.bottom << ")"
                  << " ScreenRect=(" << rc.left << "," << rc.top
                  << " - " << rc.right << "," << rc.bottom << ")"
                  << std::endl;
    }
}

void AfxPaintAllWindows(HDC hdcMain) {
    CWnd* pMain = AfxGetMainWnd();
    if (!pMain) return;

    // 1. Fill background of main frame with standard buttonface/ltgray
    CRect rcMain;
    pMain->GetClientRect(&rcMain);
    FillRect(hdcMain, &rcMain, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

    // 2. Collect all active visible windows and sort by depth (parents first)
    struct WinOrder {
        HWND hWnd;
        CWnd* pWnd;
        int depth;
        CRect rc;
    };
    std::vector<WinOrder> wins;
    for (auto& pair : g_windows) {
        if (!pair.second.pWnd || !pair.second.bVisible) continue;
        int d = 0;
        HWND p = pair.second.hParent;
        while (p) {
            d++;
            auto itp = g_windows.find(p);
            if (itp == g_windows.end()) break;
            p = itp->second.hParent;
        }
        CRect rc;
        pair.second.pWnd->GetWindowRect(&rc);
        wins.push_back({ pair.first, pair.second.pWnd, d, rc });
    }

    std::sort(wins.begin(), wins.end(), [](const WinOrder& a, const WinOrder& b) {
        return a.depth < b.depth;
    });

    for (const auto& w : wins) {
        if (w.pWnd == pMain) continue;
        HDC hdcChild = w.pWnd->GetDC()->m_hDC;
        w.pWnd->SendMessage(WM_PAINT, 0, 0);
        GDISurface* pChildSurf = GetDCSurface(hdcChild);
        if (pChildSurf) {
            BitBlt(hdcMain, w.rc.left, w.rc.top, w.rc.Width(), w.rc.Height(), hdcChild, 0, 0, SRCCOPY);
        }
    }
}

void AfxDispatchMouse(UINT msg, WPARAM wParam, int screenX, int screenY) {
    CWnd* pCapture = CWnd::GetCapture();
    if (pCapture) {
        CPoint pt(screenX, screenY);
        pCapture->ScreenToClient(&pt);
        LPARAM lParam = MAKELPARAM(pt.x, pt.y);
        pCapture->SendMessage(msg, wParam, lParam);
        return;
    }

    CWnd* pMain = AfxGetMainWnd();
    if (!pMain) return;

    // Collect all windows and sort by depth (deepest first for hit testing)
    struct WinOrder {
        HWND hWnd;
        CWnd* pWnd;
        int depth;
        CRect rc;
    };
    std::vector<WinOrder> wins;
    for (auto& pair : g_windows) {
        if (!pair.second.pWnd || !pair.second.bVisible) continue;
        int d = 0;
        HWND p = pair.second.hParent;
        while (p) {
            d++;
            auto itp = g_windows.find(p);
            if (itp == g_windows.end()) break;
            p = itp->second.hParent;
        }
        CRect rc;
        pair.second.pWnd->GetWindowRect(&rc);
        wins.push_back({ pair.first, pair.second.pWnd, d, rc });
    }

    std::sort(wins.begin(), wins.end(), [](const WinOrder& a, const WinOrder& b) {
        return a.depth > b.depth;
    });

    CWnd* pTarget = pMain;
    for (const auto& w : wins) {
        if (w.rc.PtInRect(CPoint(screenX, screenY))) {
            pTarget = w.pWnd;
            break;
        }
    }

    CPoint pt(screenX, screenY);
    pTarget->ScreenToClient(&pt);
    LPARAM lParam = MAKELPARAM(pt.x, pt.y);
    pTarget->SendMessage(msg, wParam, lParam);
}

CRuntimeClass CWinApp::classCWinApp = { "CWinApp", sizeof(CWinApp), 0xFFFF, nullptr, &CCmdTarget::classCCmdTarget };
CRuntimeClass* CWinApp::GetRuntimeClass() const { return &classCWinApp; }
const AFX_MSGMAP* CWinApp::GetThisMessageMap() {
    static const AFX_MSGMAP_ENTRY entries[] = { { 0, 0, 0, 0, AfxSig_end, 0 } };
    static const AFX_MSGMAP map = { &CCmdTarget::GetThisMessageMap, entries };
    return &map;
}

HCURSOR CWinApp::LoadCursor(UINT nIDResource) const { return (HCURSOR)1; }
HCURSOR CWinApp::LoadStandardCursor(LPCTSTR lpszCursorName) const { return (HCURSOR)1; }
HICON CWinApp::LoadIcon(UINT nIDResource) const { return (HICON)1; }

BOOL AfxRegisterClass(WNDCLASS* lpWndClass) { return TRUE; }
LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon) {
    return "PaintWndClass";
}

int AfxMessageBox(LPCTSTR lpszText, UINT nType, UINT nIDHelp) {
    std::cout << "[Paint MsgBox] " << (lpszText ? lpszText : "") << std::endl;
    return IDOK;
}
int AfxMessageBox(UINT nIDPrompt, UINT nType, UINT nIDHelp) {
    std::cout << "[Paint MsgBox ID] " << nIDPrompt << std::endl;
    return IDOK;
}

BOOL AfxLoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf) {
    return LoadString((HINSTANCE)1, nID, lpszBuf, nMaxBuf) > 0;
}

void AfxThrowMemoryException() { std::cerr << "Memory Exception!" << std::endl; }
void AfxThrowResourceException() { std::cerr << "Resource Exception!" << std::endl; }

INT_PTR AfxDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return 0; }

BOOL CCmdTarget::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    return FALSE;
}
CRuntimeClass CCmdTarget::classCCmdTarget = { "CCmdTarget", sizeof(CCmdTarget), 0xFFFF, nullptr, &CObject::classCObject };
CRuntimeClass* CCmdTarget::GetRuntimeClass() const { return &classCCmdTarget; }

CRuntimeClass CObject::classCObject = { "CObject", sizeof(CObject), 0xFFFF, nullptr, nullptr };
