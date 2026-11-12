#pragma once
#include "afxwin.h"

// Stubs for Windows OLE & MFC Extensions
class COleClientItem : public CDocument {};
class COleDocument : public CDocument {};
class COleIPFrameWnd : public CFrameWnd {
public:
    DECLARE_DYNCREATE(COleIPFrameWnd)
    DECLARE_MESSAGE_MAP()
    CFrameWnd* m_pMainFrame = nullptr;
    CFrameWnd* m_pDocFrame = nullptr;
    virtual void RepositionFrame(LPCRECT lpPosRect, LPCRECT lpClipRect) {}
};

typedef WORD CLIPFORMAT;
typedef DWORD TYMED;
struct IUnknown;
#define TYMED_HGLOBAL   1
#define TYMED_FILE      2
#define TYMED_ISTREAM   4
#define TYMED_ISTORAGE  8
#define TYMED_GDI       16
#define TYMED_MFPICT    32
#define TYMED_ENHMF     64
#define TYMED_NULL      0
struct FORMATETC {
    CLIPFORMAT cfFormat;
    void* ptd;
    DWORD dwAspect;
    LONG lindex;
    DWORD tymed;
};
typedef FORMATETC* LPFORMATETC;

struct STGMEDIUM {
    DWORD tymed;
    union {
        HBITMAP hBitmap;
        HGLOBAL hGlobal;
        void* pstg;
    };
    IUnknown* pUnkForRelease;
};
typedef STGMEDIUM* LPSTGMEDIUM;

typedef DWORD DVASPECT;
#define DVASPECT_CONTENT    1
#define DVASPECT_THUMBNAIL  2
#define DVASPECT_ICON       4
#define DVASPECT_DOCPRINT   8
class COleResizeBar : public CControlBar {
public:
    BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE, UINT nID = 0) { return TRUE; }
    void SetMinSize(CSize size) {}
};
class COleDropTarget : public CCmdTarget {
public:
    BOOL Register(CWnd* pWnd) { return TRUE; }
    virtual void Revoke() {}
};

typedef DWORD DROPEFFECT;

class COleDataSource : public CCmdTarget {
public:
    COleDataSource() {}
    virtual ~COleDataSource() {}
    void CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal, LPFORMATETC lpFormatEtc = nullptr) {}
    void DelayRenderData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc = nullptr) {}
    void DelayRenderFileData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc = nullptr) {}
    DROPEFFECT DoDragDrop(DWORD dwEffects = 1, LPCRECT lpRectStartDrag = nullptr, void* pDropSource = nullptr) { return 0; }
    void SetClipboard() {}
};

inline void ReleaseStgMedium(LPSTGMEDIUM pMedium) {}

inline COleDataSource* COleServerItem::GetDataSource() {
    static COleDataSource ds;
    return &ds;
}

class CRectTracker : public CObject {
public:
    CRect m_rect;
    UINT m_nStyle;
    CRectTracker() : m_nStyle(0) {}
    BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = nullptr) { return TRUE; }
    void Draw(CDC* pDC) const {}
};

#define OLE_APPTYPE int
#define OLE_APPTYPE_LINK 0
