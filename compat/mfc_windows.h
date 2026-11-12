#pragma once

// ============================================================================
// MFC Message Map System & CCmdTarget / CCmdUI / CMenu
// ============================================================================

class CCmdTarget;
class CWnd;
class CMenu;

struct AFX_CMDHANDLERINFO {
    CCmdTarget* pTarget;
    void (CCmdTarget::*pmf)();
};

#define CN_COMMAND              0
#define CN_UPDATE_COMMAND_UI    ((UINT)(-1))

class CCmdUI {
public:
    UINT m_nID = 0;
    UINT m_nIndex = 0;
    CMenu* m_pMenu = nullptr;
    CMenu* m_pSubMenu = nullptr;
    CWnd* m_pOther = nullptr;
    BOOL m_bEnableChanged = FALSE;
    BOOL m_bContinueRouting = FALSE;
    UINT m_nIndexMax = 0;
    CMenu* m_pParentMenu = nullptr;

    virtual void Enable(BOOL bOn = TRUE) {}
    virtual void SetCheck(int nCheck = 1) {}
    virtual void SetRadio(BOOL bOn = TRUE) {}
    virtual void SetText(LPCTSTR lpszText) {}
    void ContinueRouting() { m_bContinueRouting = TRUE; }
    BOOL DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHndler);
};

enum AfxSig {
    AfxSig_end = 0,
    AfxSig_vv,
    AfxSig_bv,
    AfxSig_vw,
    AfxSig_vwp,
    AfxSig_vwww,
    AfxSig_vwii,
    AfxSig_vwpb,
    AfxSig_vb,
    AfxSig_bD,
    AfxSig_vM,
    AfxSig_iLPCS,
    AfxSig_vW,
    AfxSig_bW,
    AfxSig_vii,
    AfxSig_vS,
    AfxSig_bWuu,
    AfxSig_bwsp,
    AfxSig_vwsCW,
    AfxSig_cmdui,
    AfxSig_cmd_ex,
    AfxSig_lwl,
    AfxSig_u_p,
    AfxSig_v_i_LP,
};

typedef void (CCmdTarget::*AFX_PMSG)();

struct AFX_MSGMAP_ENTRY {
    UINT nMessage;
    UINT nCode;
    UINT nID;
    UINT nLastID;
    UINT nSig;
    AFX_PMSG pfn;
};

struct AFX_MSGMAP {
    const AFX_MSGMAP* (*pfnGetBaseMap)();
    const AFX_MSGMAP_ENTRY* lpEntries;
};

#define DECLARE_MESSAGE_MAP() \
public: \
    static const AFX_MSGMAP* PASCAL GetThisMessageMap(); \
    virtual const AFX_MSGMAP* GetMessageMap() const;

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
        { return GetThisMessageMap(); } \
    const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
    { \
        typedef theClass ThisClass; \
        typedef baseClass TheBaseClass; \
        static const AFX_MSGMAP_ENTRY _messageEntries[] = \
        {

#define END_MESSAGE_MAP() \
            { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
        }; \
        static const AFX_MSGMAP messageMap = \
            { &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
        return &messageMap; \
    }

#define ON_COMMAND(id, memberFxn) \
    { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_COMMAND_EX(id, memberFxn) \
    { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSig_cmd_ex, (AFX_PMSG)(BOOL (CCmdTarget::*)(UINT))&ThisClass::memberFxn },
#define ON_UPDATE_COMMAND_UI(id, memberFxn) \
    { WM_COMMAND, CN_UPDATE_COMMAND_UI, (WORD)id, (WORD)id, AfxSig_cmdui, (AFX_PMSG)(void (CCmdTarget::*)(CCmdUI*))&ThisClass::memberFxn },
#define ON_MESSAGE(message, memberFxn) \
    { message, 0, 0, 0, AfxSig_lwl, (AFX_PMSG)(LRESULT (CCmdTarget::*)(WPARAM, LPARAM))&ThisClass::memberFxn },

#define ON_WM_CREATE()          { WM_CREATE, 0, 0, 0, AfxSig_iLPCS, (AFX_PMSG)(int (CCmdTarget::*)(LPCREATESTRUCT))&ThisClass::OnCreate },
#define ON_WM_DESTROY()         { WM_DESTROY, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnDestroy },
#define ON_WM_NCDESTROY()       { WM_NCDESTROY, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnNcDestroy },
#define ON_WM_CLOSE()           { WM_CLOSE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnClose },
#define ON_WM_PAINT()           { WM_PAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnPaint },
#define ON_WM_NCPAINT()         { WM_NCPAINT, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnNcPaint },
#define ON_WM_ERASEBKGND()      { WM_ERASEBKGND, 0, 0, 0, AfxSig_bD, (AFX_PMSG)(BOOL (CCmdTarget::*)(CDC*))&ThisClass::OnEraseBkgnd },
#define ON_WM_SIZE()            { WM_SIZE, 0, 0, 0, AfxSig_vwii, (AFX_PMSG)(void (CCmdTarget::*)(UINT, int, int))&ThisClass::OnSize },
#define ON_WM_MOVE()            { WM_MOVE, 0, 0, 0, AfxSig_vii, (AFX_PMSG)(void (CCmdTarget::*)(int, int))&ThisClass::OnMove },
#define ON_WM_SETFOCUS()        { WM_SETFOCUS, 0, 0, 0, AfxSig_vW, (AFX_PMSG)(void (CCmdTarget::*)(CWnd*))&ThisClass::OnSetFocus },
#define ON_WM_KILLFOCUS()       { WM_KILLFOCUS, 0, 0, 0, AfxSig_vW, (AFX_PMSG)(void (CCmdTarget::*)(CWnd*))&ThisClass::OnKillFocus },
#define ON_WM_LBUTTONDOWN()     { WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnLButtonDown },
#define ON_WM_LBUTTONUP()       { WM_LBUTTONUP, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnLButtonUp },
#define ON_WM_LBUTTONDBLCLK()   { WM_LBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnLButtonDblClk },
#define ON_WM_RBUTTONDOWN()     { WM_RBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnRButtonDown },
#define ON_WM_RBUTTONUP()       { WM_RBUTTONUP, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnRButtonUp },
#define ON_WM_RBUTTONDBLCLK()   { WM_RBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnRButtonDblClk },
#define ON_WM_MOUSEMOVE()       { WM_MOUSEMOVE, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnMouseMove },
#define ON_WM_MOUSEWHEEL()      { WM_MOUSEWHEEL, 0, 0, 0, AfxSig_bwsp, (AFX_PMSG)(BOOL (CCmdTarget::*)(UINT, short, CPoint))&ThisClass::OnMouseWheel },
#define ON_WM_KEYDOWN()         { WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, (AFX_PMSG)(void (CCmdTarget::*)(UINT, UINT, UINT))&ThisClass::OnKeyDown },
#define ON_WM_KEYUP()           { WM_KEYUP, 0, 0, 0, AfxSig_vwww, (AFX_PMSG)(void (CCmdTarget::*)(UINT, UINT, UINT))&ThisClass::OnKeyUp },
#define ON_WM_CHAR()            { WM_CHAR, 0, 0, 0, AfxSig_vwww, (AFX_PMSG)(void (CCmdTarget::*)(UINT, UINT, UINT))&ThisClass::OnChar },
#define ON_WM_TIMER()           { WM_TIMER, 0, 0, 0, AfxSig_vw, (AFX_PMSG)(void (CCmdTarget::*)(UINT_PTR))&ThisClass::OnTimer },
#define ON_WM_HSCROLL()         { WM_HSCROLL, 0, 0, 0, AfxSig_vwsCW, (AFX_PMSG)(void (CCmdTarget::*)(UINT, UINT, CScrollBar*))&ThisClass::OnHScroll },
#define ON_WM_VSCROLL()         { WM_VSCROLL, 0, 0, 0, AfxSig_vwsCW, (AFX_PMSG)(void (CCmdTarget::*)(UINT, UINT, CScrollBar*))&ThisClass::OnVScroll },
#define ON_WM_SETCURSOR()       { WM_SETCURSOR, 0, 0, 0, AfxSig_bWuu, (AFX_PMSG)(BOOL (CCmdTarget::*)(CWnd*, UINT, UINT))&ThisClass::OnSetCursor },
#define ON_WM_NCHITTEST()       { WM_NCHITTEST, 0, 0, 0, AfxSig_u_p, (AFX_PMSG)(UINT (CCmdTarget::*)(CPoint))&ThisClass::OnNcHitTest },
#define ON_WM_NCLBUTTONDOWN()   { WM_NCLBUTTONDOWN, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, CPoint))&ThisClass::OnNcLButtonDown },
#define ON_WM_NCACTIVATE()      { WM_NCACTIVATE, 0, 0, 0, AfxSig_vb, (AFX_PMSG)(BOOL (CCmdTarget::*)(BOOL))&ThisClass::OnNcActivate },
#define ON_WM_NCCALCSIZE()      { WM_NCCALCSIZE, 0, 0, 0, AfxSig_v_i_LP, (AFX_PMSG)(void (CCmdTarget::*)(BOOL, NCCALCSIZE_PARAMS*))&ThisClass::OnNcCalcSize },
#define ON_WM_SYSCOLORCHANGE()  { WM_SYSCOLORCHANGE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnSysColorChange },
#define ON_WM_PALETTECHANGED()  { WM_PALETTECHANGED, 0, 0, 0, AfxSig_vW, (AFX_PMSG)(void (CCmdTarget::*)(CWnd*))&ThisClass::OnPaletteChanged },
#define ON_WM_QUERYNEWPALETTE() { WM_QUERYNEWPALETTE, 0, 0, 0, AfxSig_bv, (AFX_PMSG)(BOOL (CCmdTarget::*)())&ThisClass::OnQueryNewPalette },
#define ON_WM_GETMINMAXINFO()   { WM_GETMINMAXINFO, 0, 0, 0, AfxSig_vM, (AFX_PMSG)(void (CCmdTarget::*)(MINMAXINFO*))&ThisClass::OnGetMinMaxInfo },
#define ON_WM_WINDOWPOSCHANGING() { WM_WINDOWPOSCHANGING, 0, 0, 0, AfxSig_vS, (AFX_PMSG)(void (CCmdTarget::*)(WINDOWPOS*))&ThisClass::OnWindowPosChanging },
#define ON_WM_CANCELMODE()      { WM_CANCELMODE, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnCancelMode },
#define ON_WM_DESTROYCLIPBOARD() { WM_DESTROYCLIPBOARD, 0, 0, 0, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::OnDestroyClipboard },
#define ON_WM_ACTIVATEAPP()     { WM_ACTIVATEAPP, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(BOOL, DWORD))&ThisClass::OnActivateApp },
#define ON_WM_DEVMODECHANGE()   { WM_DEVMODECHANGE, 0, 0, 0, AfxSig_vS, (AFX_PMSG)(void (CCmdTarget::*)(LPTSTR))&ThisClass::OnDevModeChange },
#define ON_WM_WININICHANGE()    { WM_WININICHANGE, 0, 0, 0, AfxSig_vS, (AFX_PMSG)(void (CCmdTarget::*)(LPCTSTR))&ThisClass::OnWinIniChange },
#define ON_WM_SHOWWINDOW()      { WM_SHOWWINDOW, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(BOOL, UINT))&ThisClass::OnShowWindow },
#define ON_WM_SYSCOMMAND()      { WM_SYSCOMMAND, 0, 0, 0, AfxSig_vwp, (AFX_PMSG)(void (CCmdTarget::*)(UINT, LPARAM))&ThisClass::OnSysCommand },
#define ON_WM_CTLCOLOR()        { WM_CTLCOLOR, 0, 0, 0, AfxSig_bD, (AFX_PMSG)(HBRUSH (CCmdTarget::*)(CDC*, CWnd*, UINT))&ThisClass::OnCtlColor },
#define ON_WM_DRAWITEM()        { WM_DRAWITEM, 0, 0, 0, AfxSig_v_i_LP, (AFX_PMSG)(void (CCmdTarget::*)(int, LPDRAWITEMSTRUCT))&ThisClass::OnDrawItem },
#define ON_WM_MEASUREITEM()     { WM_MEASUREITEM, 0, 0, 0, AfxSig_v_i_LP, (AFX_PMSG)(void (CCmdTarget::*)(int, LPMEASUREITEMSTRUCT))&ThisClass::OnMeasureItem },
#define ON_LBN_DBLCLK(id, memberFxn) { WM_COMMAND, 2, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_BN_CLICKED(id, memberFxn) { WM_COMMAND, 0, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_EN_CHANGE(id, memberFxn)  { WM_COMMAND, 0x0300, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_EN_UPDATE(id, memberFxn)  { WM_COMMAND, 0x0400, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_EN_MAXTEXT(id, memberFxn) { WM_COMMAND, 0x0501, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_CBN_SELCHANGE(id, memberFxn) { WM_COMMAND, 1, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_CBN_EDITCHANGE(id, memberFxn) { WM_COMMAND, 5, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },
#define ON_CBN_KILLFOCUS(id, memberFxn)  { WM_COMMAND, 2, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)(void (CCmdTarget::*)())&ThisClass::memberFxn },

class CCmdTarget : public CObject {
public:
    DECLARE_DYNAMIC(CCmdTarget)
    static const AFX_MSGMAP* PASCAL GetThisMessageMap() {
        static const AFX_MSGMAP messageMap = { nullptr, nullptr };
        return &messageMap;
    }
    virtual const AFX_MSGMAP* GetMessageMap() const { return GetThisMessageMap(); }

    CCmdTarget() {}
    virtual ~CCmdTarget() {}
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    DWORD ExternalAddRef() { return 1; }
    DWORD ExternalRelease() { return 0; }
    HRESULT ExternalQueryInterface(const void* iid, void** ppvObj) { return E_NOINTERFACE; }
    void BeginWaitCursor() {}
    void EndWaitCursor() {}
    void RestoreWaitCursor() {}
};

class CMenu : public CObject {
public:
    DECLARE_DYNCREATE(CMenu)
    HMENU m_hMenu = nullptr;
    CMenu() : m_hMenu((HMENU)1) {}
    virtual ~CMenu() { DestroyMenu(); }
    BOOL CreateMenu() { m_hMenu = (HMENU)1; return TRUE; }
    BOOL CreatePopupMenu() { m_hMenu = (HMENU)2; return TRUE; }
    BOOL LoadMenu(LPCTSTR lpszResourceName) { m_hMenu = (HMENU)1; return TRUE; }
    BOOL LoadMenu(UINT nIDResource) { m_hMenu = (HMENU)(uintptr_t)nIDResource; return TRUE; }
    BOOL DestroyMenu() { m_hMenu = nullptr; return TRUE; }
    CMenu* GetSubMenu(int nPos) const { return const_cast<CMenu*>(this); }
    UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable) { return 0; }
    UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck) { return 0; }
    BOOL RemoveMenu(UINT nPosition, UINT nFlags) { return TRUE; }
    BOOL DeleteMenu(UINT nPosition, UINT nFlags) { return TRUE; }
    BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = nullptr);
};

// ============================================================================
// CWnd & Window Classes
// ============================================================================

class CFrameWnd;
class CScrollBar;

class CWnd : public CCmdTarget {
public:
    DECLARE_DYNCREATE(CWnd)
    DECLARE_MESSAGE_MAP()

    static const CWnd wndTop;
    static const CWnd wndBottom;
    static const CWnd wndTopMost;
    static const CWnd wndNoTopMost;

    HWND m_hWnd = nullptr;
    CWnd* m_pParent = nullptr;
    CWnd* m_pOwner = nullptr;
    CRect m_rectWindow{0, 0, 100, 100};
    DWORD m_dwStyle = WS_VISIBLE;
    DWORD m_dwExStyle = 0;
    UINT m_nID = 0;
    CString m_strWindowText;
    BOOL m_bDirty = TRUE;
    int m_scrollMinH = 0, m_scrollMaxH = 0, m_scrollPosH = 0;
    int m_scrollMinV = 0, m_scrollMaxV = 0, m_scrollPosV = 0;
    BOOL m_bScrollEnableH = FALSE, m_bScrollEnableV = FALSE;
    CFont* m_pFont = nullptr;
    UINT m_nFlags = 0;

    CWnd();
    virtual ~CWnd();

    static CWnd* FromHandle(HWND hWnd);
    static CWnd* FromHandlePermanent(HWND hWnd);
    HWND GetSafeHwnd() const { return this ? m_hWnd : nullptr; }
    operator HWND() const { return GetSafeHwnd(); }
    BOOL Attach(HWND hWndNew) { m_hWnd = hWndNew; return TRUE; }
    HWND Detach() { HWND hWnd = m_hWnd; m_hWnd = nullptr; return hWnd; }

    DWORD GetStyle() const;
    DWORD GetExStyle() const;
    BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
    BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);

    CWnd* GetOwner() const { return m_pOwner ? m_pOwner : m_pParent; }
    void SetOwner(CWnd* pOwnerWnd) { m_pOwner = pOwnerWnd; }

    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
                        const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext = nullptr);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                          DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                          HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = nullptr);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
                          DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = nullptr);
    virtual BOOL DestroyWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs) { return TRUE; }
    virtual void PostNcDestroy() {}

    BOOL IsWindowVisible() const;
    BOOL IsWindowEnabled() const;
    BOOL EnableWindow(BOOL bEnable = TRUE);

    void SetWindowText(LPCTSTR lpszString) { m_strWindowText = lpszString ? lpszString : ""; Invalidate(); }
    int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
    void CenterWindow(CWnd* pAlternateOwner = nullptr) {}
    void GetWindowText(CString& rString) const { rString = m_strWindowText; }
    int GetWindowTextLength() const { return m_strWindowText.GetLength(); }

    void SetFont(CFont* pFont, BOOL bRedraw = TRUE) { m_pFont = pFont; if (bRedraw) Invalidate(); }
    CFont* GetFont() const { return m_pFont; }

    void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
    void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE) {
        if (lpRect) MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
    }
    BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
    BOOL ShowWindow(int nCmdShow);
    CWnd* GetWindow(UINT nCmd) const { return nullptr; }

    void GetWindowRect(LPRECT lpRect) const;
    void GetClientRect(LPRECT lpRect) const;
    void ClientToScreen(LPPOINT lpPoint) const;
    void ClientToScreen(LPRECT lpRect) const;
    void ScreenToClient(LPPOINT lpPoint) const;
    void ScreenToClient(LPRECT lpRect) const;
    void MapWindowPoints(CWnd* pwndTo, LPPOINT lpPoint, UINT nCount) const;
    void MapWindowPoints(CWnd* pwndTo, LPRECT lpRect) const;

    CDC* BeginPaint(LPPAINTSTRUCT lpPaint);
    void EndPaint(LPPAINTSTRUCT lpPaint);
    BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE) const { if (lpRect) *lpRect = CRect(0,0,100,100); return TRUE; }
    CDC* GetDC();
    CDC* GetWindowDC();
    int ReleaseDC(CDC* pDC);

    void UpdateWindow();
    void SetRedraw(BOOL bRedraw = TRUE) {}
    void Invalidate(BOOL bErase = TRUE);
    void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
    void InvalidateRgn(CRgn* pRgn, BOOL bErase = TRUE);
    void ValidateRect(LPCRECT lpRect);
    void ValidateRgn(CRgn* pRgn);
    BOOL RedrawWindow(LPCRECT lpRectUpdate = nullptr, CRgn* prgnUpdate = nullptr, UINT flags = 0);

    UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT nElapse, void (*lpfnTimer)(HWND, UINT, UINT_PTR, DWORD) = nullptr);
    BOOL KillTimer(UINT_PTR nIDEvent);

    static CWnd* GetCapture();
    CWnd* SetCapture();
    static CWnd* GetFocus();
    CWnd* SetFocus();
    static CWnd* GetDesktopWindow();
    static CWnd* GetActiveWindow();
    static CWnd* WindowFromPoint(POINT point) { return nullptr; }
    CWnd* SetActiveWindow() { return this; }
    void BringWindowToTop() {}

    CWnd* GetParent() const { return m_pParent; }
    CWnd* SetParent(CWnd* pWndNewParent) { CWnd* old = m_pParent; m_pParent = pWndNewParent; return old; }
    CFrameWnd* GetParentFrame() const;
    CFrameWnd* GetTopLevelFrame() const { return GetParentFrame(); }
    CWnd* GetTopLevelParent() const;
    CWnd* GetDlgItem(int nID) const;
    void SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE) {}
    UINT GetDlgItemInt(int nID, BOOL* lpTrans = nullptr, BOOL bSigned = TRUE) const { if (lpTrans) *lpTrans = TRUE; return 0; }
    void SetDlgItemText(int nID, LPCTSTR lpszString) {}
    int GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const { return 0; }
    int GetDlgItemText(int nID, CString& rString) const { return 0; }
    void CheckDlgButton(int nIDButton, UINT nCheck) {}
    void CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) {}
    int GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton) { return nIDFirstButton; }
    UINT IsDlgButtonChecked(int nIDButton) const { return 0; }

    int GetScrollPos(int nBar) const;
    int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
    void GetScrollRange(int nBar, int* lpMinPos, int* lpMaxPos) const;
    void SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
    BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE) { return TRUE; }
    BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL) { return TRUE; }
    void ScrollWindow(int xAmount, int yAmount, LPCRECT lpRect = nullptr, LPCRECT lpClipRect = nullptr);
    BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);
    void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
    CScrollBar* GetScrollBarCtrl(int nBar) const { return nullptr; }
    void RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver, UINT nFlag = 0, LPRECT lpRectParam = nullptr, LPCRECT lpRectClient = nullptr, BOOL bStretch = TRUE) {}

    LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
    BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) { return 0; }
    LRESULT Default() { return 0; }
    virtual BOOL PreTranslateMessage(MSG* pMsg) { return FALSE; }
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const { return -1; }
    BOOL EnableToolTips(BOOL bEnable = TRUE) { return TRUE; }

    CMenu* GetMenu() const;
    BOOL SetMenu(CMenu* pMenu) { return TRUE; }
    CMenu* GetSystemMenu(BOOL bRevert) const { return nullptr; }
    void DrawMenuBar() {}

    BOOL OpenClipboard() { return TRUE; }
    void CreateSolidCaret(int nWidth, int nHeight) {}
    void CreateCaret(CBitmap* pBitmap) {}
    static void PASCAL SetCaretPos(POINT point) {}
    static CPoint PASCAL GetCaretPos() { return CPoint(0, 0); }
    void ShowCaret() {}
    void HideCaret() {}
    void DragAcceptFiles(BOOL bAccept = TRUE) {}

    BOOL GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const;
    BOOL SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
    int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = nullptr, UINT nType = MB_OK);

    // Default message handlers
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) { return 0; }
    afx_msg void OnDestroy() {}
    afx_msg void OnNcDestroy() {}
    afx_msg void OnClose() { DestroyWindow(); }
    afx_msg void OnPaint() {}
    afx_msg void OnNcPaint() {}
    afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return TRUE; }
    afx_msg void OnSize(UINT nType, int cx, int cy) {}
    afx_msg void OnMove(int x, int y) {}
    afx_msg void OnSetFocus(CWnd* pOldWnd) {}
    afx_msg void OnKillFocus(CWnd* pNewWnd) {}
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point) {}
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point) {}
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point) {}
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point) {}
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point) {}
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point) {}
    afx_msg void OnMouseMove(UINT nFlags, CPoint point) {}
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) { return FALSE; }
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {}
    afx_msg void OnTimer(UINT_PTR nIDEvent) {}
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {}
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {}
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) { return FALSE; }
    afx_msg UINT OnNcHitTest(CPoint point) { return HTCLIENT; }
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point) {}
    afx_msg BOOL OnNcActivate(BOOL bActive) { return TRUE; }
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {}
    afx_msg void OnSysColorChange() {}
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd) {}
    afx_msg BOOL OnQueryNewPalette() { return FALSE; }
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI) {}
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos) {}
    afx_msg void OnCancelMode() {}
    afx_msg void OnDestroyClipboard() {}
    afx_msg void OnActivateApp(BOOL bActive, HTASK hTask) {}
    afx_msg void OnDevModeChange(LPTSTR lpDeviceName) {}
    afx_msg void OnWinIniChange(LPCTSTR lpszSection) {}
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus) {}
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam) {}
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) { return nullptr; }
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {}
    afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) {}
};

#define wndTop      (CWnd::wndTop)
#define wndBottom   (CWnd::wndBottom)
#define wndTopMost  (CWnd::wndTopMost)
#define wndNoTopMost (CWnd::wndNoTopMost)
#define WF_KEEPMINIACTIVE 0x0001

class CScrollBar : public CWnd {
public:
    DECLARE_DYNAMIC(CScrollBar)
};

class CControlBar : public CWnd {
public:
    DECLARE_DYNAMIC(CControlBar)
    DECLARE_MESSAGE_MAP()
    enum { delayHide = 1, delayShow = 2 };
    UINT m_nStateFlags = 0;
    CFrameWnd* m_pDockSite = nullptr;
    DWORD m_dwStyle = 0;
    BOOL m_bAutoDelete = FALSE;
    int m_cxLeftBorder = 6;
    int m_cxRightBorder = 6;
    int m_cyTopBorder = 1;
    int m_cyBottomBorder = 1;

    CControlBar() {}
    virtual ~CControlBar() {}
    void CalcInsideRect(CRect& rect, BOOL bHorz) const {
        rect.left += m_cxLeftBorder;
        rect.top += m_cyTopBorder;
        rect.right -= m_cxRightBorder;
        rect.bottom -= m_cyBottomBorder;
    }
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz) { return CSize(56, 56); }
    virtual CSize CalcDynamicLayout(int nLength, DWORD nMode) { return CalcFixedLayout(nMode & 1, nMode & 2); }
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) {}
    virtual void DoPaint(CDC* pDC) {}
    virtual BOOL SetStatusText(int nHit) { return TRUE; }
    void SetBarStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }
    DWORD GetBarStyle() const { return m_dwStyle; }
    void EnableDocking(DWORD dwDockStyle) {}
    BOOL IsFloating() const { return FALSE; }
    CFrameWnd* GetDockingFrame() const { return m_pDockSite; }
    BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, void* pContext = nullptr) {
        m_dwStyle = dwStyle;
        return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
    }
    afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam) { return 0; }
};

class CStatusBar : public CControlBar {
public:
    DECLARE_DYNAMIC(CStatusBar)
    DECLARE_MESSAGE_MAP()
    struct Pane {
        UINT nID = 0;
        UINT nStyle = 0;
        int cxText = 100;
        CString strText;
    };
    std::vector<Pane> m_panes;
    int m_nMinHeight = 20;

    CStatusBar() {}
    virtual ~CStatusBar() {}
    BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, UINT nID = AFX_IDW_STATUS_BAR);
    BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);
    void GetItemRect(int nIndex, LPRECT lpRect) const;
    void GetPaneText(int nIndex, CString& rString) const;
    CString GetPaneText(int nIndex) const;
    BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
    void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;
    void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);
    UINT GetPaneStyle(int nIndex) const { return (nIndex >= 0 && nIndex < (int)m_panes.size()) ? m_panes[nIndex].nStyle : 0; }
    void SetPaneStyle(int nIndex, UINT nStyle) { if (nIndex >= 0 && nIndex < (int)m_panes.size()) m_panes[nIndex].nStyle = nStyle; }
    int CommandToIndex(UINT nIDFind) const;
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz) override { return CSize(800, m_nMinHeight); }
};

class CToolBar : public CControlBar {
public:
    DECLARE_DYNAMIC(CToolBar)
    DECLARE_MESSAGE_MAP()
    BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = 0xe801) { return TRUE; }
    BOOL LoadBitmap(LPCTSTR lpszResourceName) { return TRUE; }
    BOOL LoadBitmap(UINT nIDResource) { return TRUE; }
    BOOL SetButtons(const UINT* lpIDArray, int nIDCount) { return TRUE; }
    void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage) {}
    void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const {}
};

class CDialogBar : public CControlBar {
public:
    DECLARE_DYNAMIC(CDialogBar)
    DECLARE_MESSAGE_MAP()
    CSize m_sizeDefault{100, 25};
    BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID) { return TRUE; }
    BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) { return TRUE; }
};

class CDocument;
class CView;
class CDocTemplate;
struct CCreateContext;

#define AFX_WS_DEFAULT_VIEW (WS_CHILD | WS_VISIBLE | WS_BORDER)
#define AFX_IDW_PANE_FIRST  0xE900

struct CCreateContext {
    CRuntimeClass* m_pNewViewClass = nullptr;
    CDocument* m_pCurrentDoc = nullptr;
    CDocTemplate* m_pNewDocTemplate = nullptr;
    CView* m_pLastView = nullptr;
    CFrameWnd* m_pCurrentFrame = nullptr;
};

class CFrameWnd : public CWnd {
public:
    DECLARE_DYNCREATE(CFrameWnd)
    DECLARE_MESSAGE_MAP()

    CView* m_pViewActive = nullptr;
    CDocument* m_pDocument = nullptr;
    DWORD m_dwPromptContext = 0;
    UINT m_nIDTracking = 0;
    UINT m_nIDLastMessage = 0;
    CObList m_listControlBars;

    CFrameWnd();
    virtual ~CFrameWnd();

    virtual BOOL Create(LPCTSTR lpszClassName,
                        LPCTSTR lpszWindowName,
                        DWORD dwStyle = WS_OVERLAPPEDWINDOW,
                        const RECT& rect = CRect(0, 0, 100, 100),
                        CWnd* pParentWnd = nullptr,
                        LPCTSTR lpszMenuName = nullptr,
                        DWORD dwExStyle = 0,
                        void* pContext = nullptr) {
        return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, 0, pContext);
    }

    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW,
                           CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    virtual CWnd* CreateView(CCreateContext* pContext, UINT nID = AFX_IDW_PANE_FIRST);
    virtual void InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible = TRUE);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CView* GetActiveView() const { return m_pViewActive; }
    void SetActiveView(CView* pViewNew, BOOL bNotify = TRUE) { m_pViewActive = pViewNew; }
    virtual CDocument* GetActiveDocument();
    virtual CWnd* GetMessageBar() { return nullptr; }
    virtual void ActivateFrame(int nCmdShow = -1);
    virtual void RecalcLayout(BOOL bNotify = TRUE);
    void DelayRecalcLayout(BOOL bNotify = TRUE) { RecalcLayout(bNotify); }
    virtual BOOL NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder);
    enum BorderCmd { borderGet = 1, borderRequest = 2, borderSet = 3 };

    void EnableDocking(DWORD dwDockStyle) {}
    void DockControlBar(CControlBar* pBar, UINT nDockBarID = 0, LPCRECT lpRect = nullptr);
    void FloatControlBar(CControlBar* pBar, CPoint point, DWORD dwStyle = CBRS_ALIGN_TOP) {}
    void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);
    CControlBar* GetControlBar(UINT nID);
    void LoadBarState(LPCTSTR lpszProfileName) {}
    void SaveBarState(LPCTSTR lpszProfileName) const {}

    afx_msg void OnHelp() {}
    afx_msg void OnHelpIndex() {}
    afx_msg void OnHelpUsing() {}
    afx_msg void OnContextHelp() {}
    afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
    afx_msg BOOL OnBarCheck(UINT nID);
};

class CPrintDialog {
public:
    struct {
        HDC hDC = nullptr;
        WORD nFromPage = 1;
        WORD nToPage = 1;
    } m_pd;
    CPrintDialog(BOOL bPrintSetupOnly, DWORD dwFlags = 0, CWnd* pParentWnd = nullptr) {}
    HDC CreatePrinterDC() { return nullptr; }
};

struct CPrintInfo {
    CPrintDialog* m_pPD = nullptr;
    BOOL m_bPreview = FALSE;
    BOOL m_bDirect = FALSE;
    BOOL m_bDocObject = FALSE;
    UINT m_nCurPage = 1;
    UINT m_nNumPreviewPages = 1;
    CString m_strPageDesc;
    LPVOID m_lpUserData = nullptr;
    CRect m_rectDraw;
    void SetMinPage(UINT nMinPage) {}
    void SetMaxPage(UINT nMaxPage) {}
    UINT GetMinPage() const { return 1; }
    UINT GetMaxPage() const { return 1; }
};

class CView : public CWnd {
public:
    DECLARE_DYNAMIC(CView)
    DECLARE_MESSAGE_MAP()

    CDocument* m_pDocument = nullptr;
    CView() {}
    virtual ~CView() {}
    CDocument* GetDocument() const { return m_pDocument; }
    virtual void OnInitialUpdate() {}
    virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) {}
    virtual void OnDraw(CDC* pDC) = 0;
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = nullptr) {}
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) { return FALSE; }
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {}
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo) {}
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {}
    BOOL DoPreparePrinting(CPrintInfo* pInfo) { return FALSE; }
    afx_msg void OnFilePrint() {}
    afx_msg void OnFilePrintPreview() {}
};

class CDocTemplate : public CCmdTarget {
public:
    enum DocStringIndex {
        windowTitle,
        docName,
        fileNewName,
        filterName,
        filterExt,
        regFileTypeId,
        regFileTypeName
    };
    UINT m_nIDResource = 0;
    CRuntimeClass* m_pDocClass = nullptr;
    CRuntimeClass* m_pFrameClass = nullptr;
    CRuntimeClass* m_pViewClass = nullptr;
    std::vector<CDocument*> m_docList;

    CDocTemplate(UINT nIDResource = 0, CRuntimeClass* pDocClass = nullptr,
                 CRuntimeClass* pFrameClass = nullptr, CRuntimeClass* pViewClass = nullptr)
        : m_nIDResource(nIDResource), m_pDocClass(pDocClass), m_pFrameClass(pFrameClass), m_pViewClass(pViewClass) {}

    virtual BOOL GetDocString(CString& rString, enum DocStringIndex index) const {
        if (index == docName) { rString = "untitled"; return TRUE; }
        if (index == filterExt) { rString = ".bmp"; return TRUE; }
        return FALSE;
    }
    void SetServerInfo(UINT nIDOleEmbedding, UINT nIDOleInPlaceServer = 0, CRuntimeClass* pFrameClass = nullptr, CRuntimeClass* pViewClass = nullptr) {}
    virtual POSITION GetFirstDocPosition() const;
    virtual CDocument* GetNextDoc(POSITION& rPos) const;
    virtual CDocument* CreateNewDocument();
    virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);
    virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, BOOL bMakeVisible = TRUE);
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
};

class CDocManager : public CObject {
public:
    virtual POSITION GetFirstDocTemplatePosition() const { return nullptr; }
    virtual CDocTemplate* GetNextDocTemplate(POSITION& rPosition) const { return nullptr; }
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName) { return nullptr; }
    virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate) { return FALSE; }
};

class CSingleDocTemplate : public CDocTemplate {
public:
    CSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
        : CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass) {}
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) override;
};

class CDocument : public CCmdTarget {
public:
    DECLARE_DYNAMIC(CDocument)
    DECLARE_MESSAGE_MAP()

    CString m_strTitle = "untitled";
    CString m_strPathName;
    BOOL m_bModified = FALSE;
    CDocTemplate* m_pDocTemplate = nullptr;

    CDocument() {}
    virtual ~CDocument() {}
    const CString& GetTitle() const { return m_strTitle; }
    virtual void SetTitle(LPCTSTR lpszTitle) { m_strTitle = lpszTitle ? lpszTitle : ""; }
    virtual POSITION GetFirstViewPosition() const { return nullptr; }
    virtual CView* GetNextView(POSITION& rPosition) const { return nullptr; }
    const CString& GetPathName() const { return m_strPathName; }
    virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE) { m_strPathName = lpszPathName ? lpszPathName : ""; }
    CDocTemplate* GetDocTemplate() const {
        static CDocTemplate s_tmpl;
        return m_pDocTemplate ? m_pDocTemplate : &s_tmpl;
    }
    virtual BOOL IsModified() { return m_bModified; }
    virtual void SetModifiedFlag(BOOL bModified = TRUE) { m_bModified = bModified; }
    virtual void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = nullptr) {}
    virtual BOOL OnNewDocument() { return TRUE; }
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) { return TRUE; }
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName) { return TRUE; }
    virtual void DeleteContents() {}
    afx_msg void OnFileSave() {}
    afx_msg void OnFileSaveCopyAs() {}
    afx_msg void OnFileSendMail() {}
    afx_msg void OnUpdateFileSendMail(CCmdUI* pCmdUI) {}
    virtual void OnCloseDocument() {}
    virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault) {}
    virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError);
    virtual void ReleaseFile(CFile* pFile, BOOL bAbort);
    virtual BOOL CanCloseFrame(CFrameWnd* pFrame) { return TRUE; }
    virtual BOOL SaveModified() { return TRUE; }
    virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE) { return TRUE; }
};

class COleServerDoc;
class COleDataSource;
struct FORMATETC;
typedef FORMATETC* LPFORMATETC;

class COleServerItem : public CCmdTarget {
public:
    DECLARE_DYNAMIC(COleServerItem)
    COleServerDoc* m_pDoc = nullptr;
    COleServerItem(COleServerDoc* pServerDoc = nullptr, BOOL bAutoDelete = FALSE) : m_pDoc(pServerDoc) {}
    COleServerDoc* GetDocument() const { return m_pDoc; }
    void NotifyChanged() {}
    void CopyToClipboard(BOOL bIncludeLink = FALSE) {}
    COleDataSource* GetDataSource();
    virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize) { return TRUE; }
    virtual BOOL OnSetExtent(DVASPECT dwDrawAspect, const CSize& size) { return TRUE; }
    virtual BOOL OnDraw(CDC* pDC, CSize& rSize) { return TRUE; }
    virtual void OnOpen() {}
    virtual void OnShow() {}
    virtual void OnHide() {}
    virtual BOOL OnRenderGlobalData(LPFORMATETC lpFormatEtc, HGLOBAL* phGlobal) { return FALSE; }
    virtual BOOL OnRenderFileData(LPFORMATETC lpFormatEtc, CFile* pFile) { return FALSE; }
    void GetClipboardData(COleDataSource* pDataSource, BOOL bIncludeLink = FALSE, LPPOINT pptOffset = nullptr, LPSIZE pSize = nullptr) {}
    CString GetItemName() const { return "Paint"; }
};

class COleIPFrameWnd;
struct IStorage;
typedef struct IStorage* LPSTORAGE;

class COleServerDoc : public CDocument {
public:
    DECLARE_DYNAMIC(COleServerDoc)
    DECLARE_MESSAGE_MAP()
    LPSTORAGE m_lpRootStg = nullptr;
    COleIPFrameWnd* m_pInPlaceFrame = nullptr;
    virtual void CommitItems(BOOL bCommit = TRUE) {}
    void SaveEmbedding() {}
    afx_msg void OnFileUpdate() {}
    BOOL IsEmbedded() const { return FALSE; }
    BOOL IsInPlaceActive() const { return FALSE; }
    COleServerItem* GetEmbeddedItem() { return nullptr; }
    void NotifyChanged() {}
    void NotifySaved() {}
    void NotifyClosed() {}
    virtual BOOL OnUpdateDocument() { return TRUE; }
    virtual COleServerItem* OnGetEmbeddedItem() { return nullptr; }
    virtual COleServerItem* OnGetLinkedItem(LPCTSTR lpszItemName) { return nullptr; }
    virtual void OnDeactivateUI(BOOL bUndoable) {}
    virtual void OnShowControlBars(CFrameWnd* pFrame, BOOL bShow) {}
    virtual void SaveToStorage(CObject* pObject) {}
    virtual void LoadFromStorage() {}
    struct XPersistStorage {
        HRESULT IsDirty() { return S_FALSE; }
        HRESULT InitNew(LPSTORAGE pstg) { return S_OK; }
        HRESULT Load(LPSTORAGE pstg) { return S_OK; }
        HRESULT Save(LPSTORAGE pstg, BOOL b) { return S_OK; }
        HRESULT SaveCompleted(LPSTORAGE pstg) { return S_OK; }
        HRESULT HandsOffStorage() { return S_OK; }
    } m_xPersistStorage;
};

// OLE / COM Interface Macros (Stubs for pbrusdoc.h)
#define STDMETHOD(method)       virtual HRESULT method
#define STDMETHOD_(type,method) virtual type method
#define DECLARE_INTERFACE_MAP()
#define BEGIN_INTERFACE_PART(localClass, baseClass) \
    class X##localClass { \
    public: \
        STDMETHOD_(ULONG, AddRef)(); \
        STDMETHOD_(ULONG, Release)(); \
        STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj);
#define INIT_INTERFACE_PART(theClass, localClass)
#define END_INTERFACE_PART(localClass) \
    } m_x##localClass; friend class X##localClass;

#define BEGIN_INTERFACE_MAP(theClass, baseClass)
#define INTERFACE_PART(theClass, iid, localClass)
#define END_INTERFACE_MAP()

#define METHOD_PROLOGUE(theClass, localClass) theClass* pThis = nullptr;
#define METHOD_PROLOGUE_EX(theClass, localClass) theClass* pThis = nullptr;

inline HRESULT OleGetClipboard(struct IDataObject** ppDataObj) { return E_FAIL; }

struct IStream;

class COleStreamFile : public CFile {
public:
    COleStreamFile() {}
    BOOL OpenStream(LPSTORAGE lpStorage, LPCTSTR lpszStreamName, DWORD nOpenFlags = modeReadWrite|shareExclusive, CFileException* pError = NULL) { return FALSE; }
    BOOL CreateStream(LPSTORAGE lpStorage, LPCTSTR lpszStreamName, DWORD nOpenFlags = modeReadWrite|shareExclusive|modeCreate, CFileException* pError = NULL) { return FALSE; }
    IStream* m_lpStream = nullptr;
};

class COleTemplateServer {
public:
    static void RegisterAll() {}
    void ConnectTemplate(REFCLSID clsid, CDocTemplate* pDocTemplate, BOOL bMultiInstance) {}
    void UpdateRegistry(WORD bRegister = 0) {}
};

inline BOOL AfxOleInit() { return TRUE; }
class COleMessageFilter : public CCmdTarget {
public:
    void Register() {}
    void Revoke() {}
    void SetMessagePendingDelay(DWORD dwMS = 5000) {}
    void EnableBusyDialog(BOOL bEnable = TRUE) {}
    void EnableNotRespondingDialog(BOOL bEnable = TRUE) {}
};
inline COleMessageFilter* AfxOleGetMessageFilter() { static COleMessageFilter filter; return &filter; }

// ============================================================================
// CDialog, CDataExchange, CEdit, CColorDialog, CFileDialog
// ============================================================================

class CDataExchange {
public:
    BOOL m_bSaveAndValidate = FALSE;
    CWnd* m_pDlgWnd = nullptr;
    CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate) : m_bSaveAndValidate(bSaveAndValidate), m_pDlgWnd(pDlgWnd) {}
};

class CDialog : public CWnd {
public:
    DECLARE_DYNAMIC(CDialog)
    DECLARE_MESSAGE_MAP()
    UINT m_nIDTemplate = 0;
    int m_nModalResult = IDCANCEL;

    CDialog() {}
    CDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr) {}
    CDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr) : m_nIDTemplate(nIDTemplate) { m_pParent = pParentWnd; }
    virtual ~CDialog() {}

    virtual int DoModal();
    void EndDialog(int nResult) { m_nModalResult = nResult; }
    virtual BOOL OnInitDialog() { return TRUE; }
    virtual void DoDataExchange(CDataExchange* pDX) {}
    virtual void OnOK() { EndDialog(IDOK); }
    virtual void OnCancel() { EndDialog(IDCANCEL); }
    BOOL UpdateData(BOOL bSaveAndValidate = TRUE) {
        CDataExchange dx(this, bSaveAndValidate);
        DoDataExchange(&dx);
        return TRUE;
    }
};

class CEdit : public CWnd {
public:
    DECLARE_DYNAMIC(CEdit)
    DECLARE_MESSAGE_MAP()
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
        return CWnd::Create("EDIT", nullptr, dwStyle, rect, pParentWnd, nID);
    }
    void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE) {}
    void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE) {}
    DWORD GetSel() const { return 0; }
    void GetSel(int& nStartChar, int& nEndChar) const { nStartChar = nEndChar = 0; }
    void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE) {}
    void LimitText(int nChars = 0) {}
    void GetRect(LPRECT lpRect) const { GetClientRect(lpRect); }
    void SetRect(LPCRECT lpRect) {}
    void SetRectNP(LPCRECT lpRect) {}
    void SetMargins(UINT nLeft, UINT nRight) {}
    BOOL Undo() { return FALSE; }
    int GetLineCount() const { return 1; }
    int LineIndex(int nLine = -1) const { return 0; }
    int LineLength(int nLine = -1) const { return 0; }
    int LineFromChar(int nIndex = -1) const { return 0; }
    int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const {
        if (lpszBuffer && nMaxLength > 0) lpszBuffer[0] = 0;
        return 0;
    }
    void Clear() {}
    void Copy() {}
    void Cut() {}
    void Paste() {}
};

class CComboBox : public CWnd {
public:
    DECLARE_DYNAMIC(CComboBox)
    DECLARE_MESSAGE_MAP()
    CComboBox() {}
    virtual ~CComboBox() {}
    int GetCount() const { return 0; }
    int GetCurSel() const { return CB_ERR; }
    int SetCurSel(int nSelect) { return CB_ERR; }
    int AddString(LPCTSTR lpszString) { return 0; }
    int DeleteString(UINT nIndex) { return 0; }
    int InsertString(int nIndex, LPCTSTR lpszString) { return 0; }
    void ResetContent() {}
    int GetLBText(int nIndex, LPTSTR lpszText) const { return 0; }
    void GetLBText(int nIndex, CString& rString) const {}
    int GetLBTextLen(int nIndex) const { return 0; }
    void SetItemData(int nIndex, DWORD_PTR dwItemData) {}
    DWORD_PTR GetItemData(int nIndex) const { return 0; }
    int FindString(int nStartAfter, LPCTSTR lpszString) const { return CB_ERR; }
    int FindStringExact(int nStartAfter, LPCTSTR lpszString) const { return CB_ERR; }
    int SelectString(int nStartAfter, LPCTSTR lpszString) { return CB_ERR; }
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) {}
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) {}
    virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) { return 0; }
};

class CListBox : public CWnd {
public:
    DECLARE_DYNAMIC(CListBox)
    DECLARE_MESSAGE_MAP()
    CListBox() {}
    virtual ~CListBox() {}
    int GetCurSel() const { return -1; }
    void SetCurSel(int n) {}
    int GetCount() const { return 0; }
    void ResetContent() {}
    int AddString(LPCTSTR s) { return 0; }
    void SetItemData(int n, DWORD_PTR d) {}
    DWORD_PTR GetItemData(int n) const { return 0; }
};

class CProgressCtrl : public CWnd {
public:
    DECLARE_DYNAMIC(CProgressCtrl)
    DECLARE_MESSAGE_MAP()
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
        return CWnd::Create(nullptr, nullptr, dwStyle, rect, pParentWnd, nID);
    }
    void SetRange(int nLower, int nUpper) {}
    int SetPos(int nPos) { return 0; }
    int OffsetPos(int nPos) { return 0; }
    int SetStep(int nStep) { return 0; }
    int StepIt() { return 0; }
};

typedef UINT_PTR (CALLBACK *LPCCHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

struct CHOOSECOLOR {
    DWORD        lStructSize;
    HWND         hwndOwner;
    HWND         hInstance;
    COLORREF     rgbResult;
    COLORREF*    lpCustColors;
    DWORD        Flags;
    LPARAM       lCustData;
    LPCCHOOKPROC lpfnHook;
    LPCTSTR      lpTemplateName;
};
#define CC_RGBINIT              0x00000001
#define CC_FULLOPEN             0x00000002
#define CC_PREVENTFULLOPEN      0x00000004
#define CC_SHOWHELP             0x00000008

class CColorDialog : public CDialog {
public:
    CHOOSECOLOR m_cc;
    CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, CWnd* pParentWnd = nullptr);
    virtual int DoModal() override;
    COLORREF GetColor() const { return m_cc.rgbResult; }
    static COLORREF* GetSavedCustomColors();
};

class CPageSetupDialog : public CDialog {
public:
    PAGESETUPDLGA m_psd{};
    CPageSetupDialog(DWORD dwFlags = 0, CWnd* pParentWnd = nullptr) {}
    virtual int DoModal() override { return IDCANCEL; }
};

// ============================================================================
// CWinApp & Global Win32/GDI/MFC Functions
// ============================================================================

class CWinApp : public CCmdTarget {
public:
    DECLARE_DYNAMIC(CWinApp)
    DECLARE_MESSAGE_MAP()

    CWnd* m_pMainWnd = nullptr;
    LPCTSTR m_pszAppName = nullptr;
    LPCTSTR m_pszProfileName = nullptr;
    HINSTANCE m_hInstance = (HINSTANCE)1;
    LPTSTR m_lpCmdLine = nullptr;
    int m_nCmdShow = SW_SHOWNORMAL;
    UINT m_nNumPreviewPages = 1;
    HANDLE m_hDevNames = nullptr;
    HANDLE m_hDevMode = nullptr;
    CDocManager* m_pDocManager = nullptr;

    CWinApp();
    virtual ~CWinApp() {}

    virtual BOOL InitInstance() { return TRUE; }
    virtual int ExitInstance() { return 0; }
    virtual BOOL OnIdle(LONG lCount) { return FALSE; }
    virtual void WinHelp(DWORD dwData, UINT nCmd = 0) {}
    void SetRegistryKey(LPCTSTR lpszRegistryKey) {}
    std::vector<CDocTemplate*> m_docTemplates;
    void SetRegistryKey(UINT nIDRegistryKey) {}
    void LoadStdProfileSettings(UINT nMaxMRU = 4) {}
    void AddDocTemplate(CDocTemplate* pTemplate);
    BOOL RunEmbedded() { return FALSE; }
    BOOL RunAutomated() { return FALSE; }
    void SelectPrinter(HANDLE hDevNames, HANDLE hDevMode, BOOL bFreeOld = TRUE) {}
    afx_msg void OnFilePrintSetup() {}
    afx_msg void OnFileNew();
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
    POSITION GetFirstDocTemplatePosition() const {
        return m_pDocManager ? m_pDocManager->GetFirstDocTemplatePosition() : nullptr;
    }
    CDocTemplate* GetNextDocTemplate(POSITION& rPosition) const {
        return m_pDocManager ? m_pDocManager->GetNextDocTemplate(rPosition) : nullptr;
    }
    BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate) {
        return m_pDocManager ? m_pDocManager->DoPromptFileName(fileName, nIDSTitle, lFlags, bOpenFileDialog, pTemplate) : FALSE;
    }

    HCURSOR LoadCursor(LPCTSTR lpszResourceName) const;
    HCURSOR LoadCursor(UINT nIDResource) const;
    HCURSOR LoadStandardCursor(LPCTSTR lpszCursorName) const;
    HCURSOR LoadOEMCursor(UINT nIDCursor) const;
    HICON LoadIcon(LPCTSTR lpszResourceName) const;
    HICON LoadIcon(UINT nIDResource) const;
    HICON LoadStandardIcon(LPCTSTR lpszIconName) const;

    UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault) { return nDefault; }
    BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue) { return TRUE; }
    CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = nullptr) {
        return CString(lpszDefault ? lpszDefault : "");
    }
    BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue) { return TRUE; }

    BOOL GetPrinterDeviceDefaults(void* pPrintDlg) { return FALSE; }
    int DoPrintDialog(CPrintDialog* pPD) { return IDCANCEL; }
};

CWinApp* AfxGetApp();
CWnd* AfxGetMainWnd();
HINSTANCE AfxGetInstanceHandle();
HINSTANCE AfxGetResourceHandle();
void AfxPaintAllWindows(HDC hdcMain);
void AfxDispatchMouse(UINT msg, WPARAM wParam, int screenX, int screenY);
void AfxDumpWindows();
LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor = nullptr, HBRUSH hbrBackground = nullptr, HICON hIcon = nullptr);
BOOL AfxRegisterClass(WNDCLASS* lpWndClass);
int AfxMessageBox(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);
int AfxMessageBox(UINT nIDPrompt, UINT nType = MB_OK, UINT nIDHelp = (UINT)-1);

#define HELP_CONTEXT 0x0001L

// Global Win32 / GDI APIs
BOOL IsWindow(HWND hWnd);
HWND GetDesktopWindow();
HDC GetDC(HWND hWnd);
HDC GetWindowDC(HWND hWnd);
int ReleaseDC(HWND hWnd, HDC hDC);
HDC CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const void* lpInitData);
HDC CreateCompatibleDC(HDC hdc);
BOOL DeleteDC(HDC hdc);
inline HGDIOBJ GetCurrentObject(HDC hdc, UINT type) { return nullptr; }
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ h);
BOOL DeleteObject(HGDIOBJ ho);
HGDIOBJ GetStockObject(int i);
int GetObject(HGDIOBJ h, int c, LPVOID pv);
int GetDeviceCaps(HDC hdc, int index);

HPEN CreatePen(int iStyle, int cWidth, COLORREF color);
HPEN CreatePenIndirect(const LOGPEN* plpen);
HBRUSH CreateSolidBrush(COLORREF color);
HBRUSH CreateBrushIndirect(const LOGBRUSH* plbrush);
HBRUSH CreatePatternBrush(HBITMAP hbm);
HBRUSH CreateDIBPatternBrushPt(const void* lpPackedDIB, UINT iUsage);
HBITMAP CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitCount, const void* lpBits);
HBITMAP CreateCompatibleBitmap(HDC hdc, int cx, int cy);
HBITMAP CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER* pbmih, DWORD flInit, const void* pjBits, const BITMAPINFO* pbmi, UINT iUsage);
HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT usage, void** ppvBits, HANDLE hSection, DWORD offset);
int GetDIBits(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT usage);
int SetDIBits(HDC hdc, HBITMAP hbm, UINT start, UINT cLines, const void* lpBits, const BITMAPINFO* lpbmi, UINT ColorUse);
int SetDIBitsToDevice(HDC hdc, int xDest, int yDest, DWORD w, DWORD h, int xSrc, int ySrc, UINT StartScan, UINT cLines, const void* lpvBits, const BITMAPINFO* lpbmi, UINT ColorUse);
int StretchDIBits(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, const void* lpBits, const BITMAPINFO* lpbmi, UINT iUsage, DWORD rop);

HPALETTE CreatePalette(const LOGPALETTE* plpal);
HPALETTE SelectPalette(HDC hdc, HPALETTE hPal, BOOL bForceBkgd);
UINT RealizePalette(HDC hdc);
UINT GetPaletteEntries(HPALETTE hpal, UINT iStart, UINT cEntries, LPPALETTEENTRY pPalEntries);
UINT SetPaletteEntries(HPALETTE hpal, UINT iStart, UINT cEntries, const PALETTEENTRY* pPalEntries);
UINT GetSystemPaletteEntries(HDC hdc, UINT iStart, UINT cEntries, LPPALETTEENTRY pPalEntries);
UINT GetNearestPaletteIndex(HPALETTE h, COLORREF color);
COLORREF GetNearestColor(HDC hdc, COLORREF color);
UINT GetDIBColorTable(HDC hdc, UINT iStart, UINT cEntries, RGBQUAD* prgbq);
UINT SetDIBColorTable(HDC hdc, UINT iStart, UINT cEntries, const RGBQUAD* prgbq);

HRGN CreateRectRgn(int x1, int y1, int x2, int y2);
HRGN CreatePolygonRgn(const POINT* pptl, int cPoint, int iMode);
int CombineRgn(HRGN hrgnDst, HRGN hrgnSrc1, HRGN hrgnSrc2, int iMode);
int SelectClipRgn(HDC hdc, HRGN hrgn);
int IntersectClipRect(HDC hdc, int left, int top, int right, int bottom);
BOOL FillRgn(HDC hdc, HRGN hrgn, HBRUSH hbr);
BOOL PtInRegion(HRGN hrgn, int x, int y);
BOOL FrameRgn(HDC hdc, HRGN hrgn, HBRUSH hbr, int w, int h);
BOOL InvertRgn(HDC hdc, HRGN hrgn);

COLORREF SetBkColor(HDC hdc, COLORREF color);
COLORREF GetBkColor(HDC hdc);
int SetBkMode(HDC hdc, int mode);
int GetBkMode(HDC hdc);
COLORREF SetTextColor(HDC hdc, COLORREF color);
COLORREF GetTextColor(HDC hdc);
UINT SetTextAlign(HDC hdc, UINT align);
int SetROP2(HDC hdc, int rop2);
int GetROP2(HDC hdc);
int SetPolyFillMode(HDC hdc, int mode);
int GetPolyFillMode(HDC hdc);
int SetStretchBltMode(HDC hdc, int mode);
int GetStretchBltMode(HDC hdc);
BOOL SetBrushOrgEx(HDC hdc, int x, int y, LPPOINT lppt);

COLORREF GetPixel(HDC hdc, int x, int y);
COLORREF SetPixel(HDC hdc, int x, int y, COLORREF color);
BOOL SetPixelV(HDC hdc, int x, int y, COLORREF color);
BOOL MoveToEx(HDC hdc, int x, int y, LPPOINT lppt);
BOOL LineTo(HDC hdc, int x, int y);
BOOL Polyline(HDC hdc, const POINT* apt, int cpt);
BOOL PolyBezier(HDC hdc, const POINT* apt, DWORD cpt);
BOOL Rectangle(HDC hdc, int left, int top, int right, int bottom);
BOOL RoundRect(HDC hdc, int left, int top, int right, int bottom, int width, int height);
BOOL Ellipse(HDC hdc, int left, int top, int right, int bottom);
BOOL Polygon(HDC hdc, const POINT* apt, int cpt);
BOOL FillRect(HDC hdc, const RECT* lprc, HBRUSH hbr);
BOOL FrameRect(HDC hdc, const RECT* lprc, HBRUSH hbr);
BOOL InvertRect(HDC hdc, const RECT* lprc);
BOOL DrawFocusRect(HDC hdc, const RECT* lprc);
BOOL PatBlt(HDC hdc, int x, int y, int w, int h, DWORD rop);
BOOL BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
BOOL StretchBlt(HDC hdcDest, int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop);
BOOL ExtFloodFill(HDC hdc, int x, int y, COLORREF color, UINT type);
BOOL TextOut(HDC hdc, int x, int y, LPCTSTR lpString, int c);
BOOL ExtTextOut(HDC hdc, int x, int y, UINT options, const RECT* lprect, LPCTSTR lpString, UINT c, const int* lpDx);
int DrawText(HDC hdc, LPCTSTR lpchText, int cchText, LPRECT lprc, UINT format);
BOOL GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, int c, LPSIZE psizl);
BOOL GetTextMetrics(HDC hdc, LPTEXTMETRIC lptm);
BOOL DrawEdge(HDC hdc, LPRECT qrc, UINT edge, UINT grfFlags);

#define SEM_FAILCRITICALERRORS 0x0001
#define SEM_NOOPENFILEERRORBOX 0x8000
inline UINT SetErrorMode(UINT uMode) { return 0; }

// System / Memory / Cursor / Clipboard / Resource APIs
inline UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, void (*lpTimerFunc)(HWND, UINT, UINT_PTR, DWORD)) { return nIDEvent; }
inline BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent) { return TRUE; }

COLORREF GetSysColor(int nIndex);
HBRUSH GetSysColorBrush(int nIndex);
int GetSystemMetrics(int nIndex);
DWORD GetTickCount();
void MessageBeep(UINT uType);

HGLOBAL GlobalAlloc(UINT uFlags, SIZE_T dwBytes);
HGLOBAL GlobalReAlloc(HGLOBAL hMem, SIZE_T dwBytes, UINT uFlags);
LPVOID GlobalLock(HGLOBAL hMem);
BOOL GlobalUnlock(HGLOBAL hMem);
HGLOBAL GlobalFree(HGLOBAL hMem);
SIZE_T GlobalSize(HGLOBAL hMem);

#define LMEM_FIXED 0x0000
#define LMEM_MOVEABLE 0x0002
#define LMEM_ZEROINIT 0x0040
inline HGLOBAL LocalAlloc(UINT uFlags, SIZE_T uBytes) { return GlobalAlloc(uFlags, uBytes); }
inline HGLOBAL LocalFree(HGLOBAL hMem) { return GlobalFree(hMem); }
inline HPALETTE CreateHalftonePalette(HDC hdc) { return (HPALETTE)1; }

HCURSOR LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName);
HCURSOR SetCursor(HCURSOR hCursor);
HCURSOR GetCursor();
int ShowCursor(BOOL bShow);
BOOL GetCursorPos(LPPOINT lpPoint);
BOOL SetCursorPos(int X, int Y);
BOOL ClipCursor(const RECT* lpRect);
HWND SetCapture(HWND hWnd);
HWND GetCapture();
BOOL ReleaseCapture();
short GetKeyState(int nVirtKey);
short GetAsyncKeyState(int vKey);

HICON LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName);
HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);
int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int cchBufferMax);

BOOL OpenClipboard(HWND hWndNewOwner);
BOOL CloseClipboard();
BOOL EmptyClipboard();
BOOL IsClipboardFormatAvailable(UINT format);
HANDLE GetClipboardData(UINT uFormat);
HANDLE SetClipboardData(UINT uFormat, HANDLE hMem);
UINT RegisterClipboardFormat(LPCTSTR lpszFormat);

BOOL GetClassInfo(HINSTANCE hInstance, LPCTSTR lpClassName, WNDCLASS* lpWndClass);
LONG GetWindowLong(HWND hWnd, int nIndex);
LONG SetWindowLong(HWND hWnd, int nIndex, LONG dwNewLong);
BOOL GetClientRect(HWND hWnd, LPRECT lpRect);
BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);
BOOL SetWindowText(HWND hWnd, LPCTSTR lpString);
BOOL ClientToScreen(HWND hWnd, LPPOINT lpPoint);
BOOL ScreenToClient(HWND hWnd, LPPOINT lpPoint);
BOOL InvalidateRect(HWND hWnd, const RECT* lpRect, BOOL bErase);
BOOL UpdateWindow(HWND hWnd);
LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL TranslateMessage(const MSG* lpMsg);
LRESULT DispatchMessage(const MSG* lpMsg);
int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);
const MSG* GetCurrentMessage();
#define PM_NOREMOVE 0x0000
#define PM_REMOVE   0x0001

// String & Path helpers
DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer);
UINT GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique, LPTSTR lpTempFileName);
inline DWORD GetCurrentDirectory(DWORD nBufferLength, LPTSTR lpBuffer) {
    if (getcwd(lpBuffer, nBufferLength)) return (DWORD)std::strlen(lpBuffer);
    return 0;
}
#define _tcsnicmp strncasecmp
#define _tcsicmp strcasecmp
#define _tcscmp strcmp
#define _tcslen strlen
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcschr strchr
#define _tcsrchr strrchr
void _tsplitpath(const char* path, char* drive, char* dir, char* fname, char* ext);
char* _itoa(int value, char* str, int radix);
inline char* itoa(int value, char* str, int radix) { return _itoa(value, str, radix); }
inline int lstrlen(LPCTSTR lpString) { return lpString ? (int)std::strlen(lpString) : 0; }
inline int lstrlenA(LPCSTR lpString) { return lpString ? (int)std::strlen(lpString) : 0; }
inline LPTSTR lstrcpy(LPTSTR lpString1, LPCTSTR lpString2) { return std::strcpy(lpString1, lpString2 ? lpString2 : ""); }
inline LPTSTR lstrcat(LPTSTR lpString1, LPCTSTR lpString2) { return std::strcat(lpString1, lpString2 ? lpString2 : ""); }
inline int lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2) { return std::strcmp(lpString1 ? lpString1 : "", lpString2 ? lpString2 : ""); }
inline int lstrcmpi(LPCTSTR lpString1, LPCTSTR lpString2) { return strcasecmp(lpString1 ? lpString1 : "", lpString2 ? lpString2 : ""); }
inline LPTSTR CharNext(LPCTSTR lpsz) { return const_cast<LPTSTR>(lpsz && *lpsz ? lpsz + 1 : lpsz); }
inline BOOL IsDBCSLeadByte(BYTE TestChar) { return FALSE; }
inline int wsprintf(LPTSTR lpOut, LPCTSTR lpFmt, ...) {
    va_list args;
    va_start(args, lpFmt);
    int r = std::vsprintf(lpOut, lpFmt, args);
    va_end(args);
    return r;
}

// Theme & Help stubs
#define BP_PUSHBUTTON 1
#define PBS_NORMAL    1
#define PBS_HOT       2
#define PBS_PRESSED   3
#define PBS_DISABLED  4
inline HWND HtmlHelpA(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData) { return nullptr; }
#define HH_DISPLAY_TOPIC 0x0000

inline LONG_PTR GetWindowLongPtr(HWND hWnd, int nIndex) { return 0; }
inline LONG_PTR SetWindowLongPtr(HWND hWnd, int nIndex, LONG_PTR dwNewLong) { return 0; }
inline ULONG_PTR SetClassLongPtr(HWND hWnd, int nIndex, LONG_PTR dwNewLong) { return 0; }
inline ULONG_PTR GetClassLongPtr(HWND hWnd, int nIndex) { return 0; }

inline HANDLE GetProcessHeap() { return (HANDLE)1; }
inline LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) { return malloc(dwBytes); }
inline BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) { free(lpMem); return TRUE; }
inline LPVOID HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes) { return realloc(lpMem, dwBytes); }

inline void AfxFormatString1(CString& rString, UINT nIDS, LPCTSTR lpsz1) { rString = lpsz1 ? lpsz1 : ""; }
inline void AfxFormatString2(CString& rString, UINT nIDS, LPCTSTR lpsz1, LPCTSTR lpsz2) {
    rString = lpsz1 ? lpsz1 : "";
    rString += " ";
    rString += lpsz2 ? lpsz2 : "";
}

inline BOOL CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck) { return TRUE; }
inline DWORD GetFileAttributes(LPCTSTR lpFileName) { return 0; }
inline int wvsprintf(LPTSTR lpOut, LPCTSTR lpFmt, va_list arglist) { return std::vsprintf(lpOut, lpFmt, arglist); }
inline LRESULT CALLBACK AfxWndProc(HWND, UINT, WPARAM, LPARAM) { return 0; }
inline void DDX_Text(void* pDX, int nIDC, CString& value) {}
inline void DDX_Text(void* pDX, int nIDC, int& value) {}
inline void DDX_Text(void* pDX, int nIDC, UINT& value) {}
inline void DDX_Text(void* pDX, int nIDC, long& value) {}
inline void DDX_Check(void* pDX, int nIDC, int& value) {}
inline void DDX_Radio(void* pDX, int nIDC, int& value) {}
inline BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes) { return mkdir(lpPathName, 0777) == 0; }
inline BOOL SetDlgItemInt(HWND hDlg, int nIDDlgItem, UINT uValue, BOOL bSigned) { return TRUE; }
inline BOOL TranslateCharsetInfo(DWORD* lpSrc, CHARSETINFO* lpCs, DWORD dwFlags) { return TRUE; }
inline short GetFileTitle(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf) { return 0; }
inline void DDV_MaxChars(void* pDX, const CString& value, int nChars) {}
inline void DDV_MinMaxInt(void* pDX, int value, int minVal, int maxVal) {}
inline void DDV_MinMaxUInt(void* pDX, UINT value, UINT minVal, UINT maxVal) {}
inline BOOL LPtoDP(HDC hdc, LPPOINT lppt, int c) { return TRUE; }
inline BOOL PlayMetaFile(HDC hdc, HMETAFILE hmf) { return TRUE; }
inline char* _getdcwd(int drive, char* buffer, int maxlen) { return getcwd(buffer, maxlen); }
inline HWND GetDlgItem(HWND hDlg, int nIDDlgItem) { return nullptr; }
inline HMODULE GetModuleHandle(LPCTSTR lpModuleName) { return (HMODULE)1; }
#define AFX_IDS_UNTITLED        0xF003

inline int GetLocaleInfo(DWORD Locale, DWORD LCType, LPTSTR lpLCData, int cchData) {
    if (lpLCData && cchData > 0) { lpLCData[0] = '.'; if (cchData > 1) lpLCData[1] = 0; }
    return 1;
}
inline LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
inline BOOL IsMenu(HMENU hMenu) { return hMenu != nullptr; }
inline BOOL EnableWindow(HWND hWnd, BOOL bEnable) { return TRUE; }
#ifndef _WINREG_FUNCS_
#define _WINREG_FUNCS_
inline LONG RegOpenKey(HKEY hKey, LPCTSTR lpSubKey, HKEY* phkResult) { return 0; }
inline LONG RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) { return -1; }
inline LONG RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) { return -1; }
inline LONG RegSetValueEx(HKEY hKey, LPCTSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) { return 0; }
inline LONG RegCloseKey(HKEY hKey) { return 0; }
inline LONG RegEnumKey(HKEY hKey, DWORD dwIndex, LPTSTR lpName, DWORD cchName) { return -1; }
inline LONG RegEnumKeyEx(HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPTSTR lpClass, LPDWORD lpcchClass, void* lpftLastWriteTime) { return -1; }
inline LONG RegCreateKey(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult) { return 0; }
inline LONG RegCreateKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD Reserved, LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition) { return 0; }
#endif
inline int SaveDC(HDC hdc) { return 1; }
inline BOOL RestoreDC(HDC hdc, int nSavedDC) { return TRUE; }
inline int SetMapMode(HDC hdc, int iMode) { return 1; }
inline BOOL SetViewportOrgEx(HDC hdc, int x, int y, LPPOINT lppt) { return TRUE; }
inline BOOL SetViewportExtEx(HDC hdc, int x, int y, LPSIZE lpsz) { return TRUE; }
inline BOOL SetWindowOrgEx(HDC hdc, int x, int y, LPPOINT lppt) { return TRUE; }
inline BOOL SetWindowExtEx(HDC hdc, int x, int y, LPSIZE lpsz) { return TRUE; }

inline int EnumFontFamilies(HDC hdc, LPCTSTR lpszFamily, FONTENUMPROC lpEnumFontFamProc, LPARAM lParam) { return 0; }
inline int EnumFontFamiliesEx(HDC hdc, LPLOGFONT lpLogfont, FONTENUMPROC lpProc, LPARAM lParam, DWORD dwFlags) { return 0; }
inline BOOL DestroyWindow(HWND hWnd) { return TRUE; }
inline UINT GetDlgItemInt(HWND hDlg, int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) { if (lpTranslated) *lpTranslated = TRUE; return 0; }
inline BOOL WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, DWORD_PTR dwData) { return TRUE; }
inline HHOOK SetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) { return (HHOOK)1; }
inline BOOL UnhookWindowsHookEx(HHOOK hhk) { return TRUE; }
inline LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam) { return 0; }
#define WH_GETMESSAGE 3
#define HC_ACTION 0

#include "commdlg.h"
