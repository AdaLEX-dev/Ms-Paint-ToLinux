#pragma once

// Standard C/C++ headers
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <cctype>
#include <climits>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>

#ifndef __AFXWIN_H__
#define __AFXWIN_H__
#endif

// MSVC / Win16 / Win32 calling convention & storage modifiers
#ifndef NEAR
#define NEAR
#endif
#ifndef near
#define near
#endif
#ifndef FAR
#define FAR
#endif
#ifndef far
#define far
#endif
#ifndef PASCAL
#define PASCAL
#endif
#ifndef pascal
#define pascal
#endif
#ifndef CDECL
#define CDECL
#endif
#ifndef _cdecl
#define _cdecl
#endif
#ifndef __cdecl
#define __cdecl
#endif
#ifndef WINAPI
#define WINAPI
#endif
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef CALLBACK
#define CALLBACK
#endif
#ifndef AFX_CDECL
#define AFX_CDECL
#endif
#ifndef EXPORT
#define EXPORT
#endif
#ifndef HILAPI
#define HILAPI
#endif
#ifndef BASED_CODE
#define BASED_CODE
#endif
#ifndef AFX_DATA
#define AFX_DATA
#endif
#ifndef AFXAPI
#define AFXAPI
#endif
#ifndef afx_msg
#define afx_msg
#endif
#ifndef __try
#define __try if (true)
#define __except(x) else if (false)
#define EXCEPTION_EXECUTE_HANDLER 1
#endif

// Basic Win32 types
typedef void                VOID;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef uint32_t            DWORD;
typedef int32_t             LONG;
typedef uint32_t            ULONG;
typedef unsigned int        UINT;
typedef intptr_t            INT_PTR;
typedef intptr_t            LONG_PTR;
typedef uintptr_t           ULONG_PTR;
typedef uintptr_t           UINT_PTR;
typedef uintptr_t           DWORD_PTR;
typedef size_t              SIZE_T;
typedef void*               PVOID;
typedef char                CHAR;
typedef char                TCHAR;
typedef wchar_t             WCHAR;
typedef char*               LPSTR;
typedef const char*         LPCSTR;
typedef char*               LPTSTR;
typedef const char*         LPCTSTR;
typedef wchar_t*            LPWSTR;
typedef const wchar_t*      LPCWSTR;
typedef const char*         PCTSTR;
typedef BYTE*               LPBYTE;
typedef BYTE*               PBYTE;
typedef WORD*               LPWORD;
typedef DWORD*              LPDWORD;
typedef DWORD*              PDWORD;
typedef void*               LPVOID;
typedef const void*         LPCVOID;
typedef void*               HANDLE;
typedef void*               HGLOBAL;
typedef void*               HINSTANCE;
typedef void*               HMODULE;
typedef void*               HTASK;
typedef void*               HKL;
typedef void*               HIMC;
typedef void*               HDROP;
typedef void*               HMENU;
typedef void*               HACCEL;
typedef void*               HDWP;
typedef void*               HTHEME;
typedef void*               HHOOK;
typedef void*               HKEY;
typedef HKEY*               PHKEY;
struct HWND__;      typedef HWND__*     HWND;
struct HDC__;       typedef HDC__*      HDC;
struct HGDIOBJ__;   typedef void*       HGDIOBJ;
struct HBITMAP__;   typedef HBITMAP__*  HBITMAP;
struct HPEN__;      typedef HPEN__*     HPEN;
struct HBRUSH__;    typedef HBRUSH__*   HBRUSH;
struct HPALETTE__;  typedef HPALETTE__* HPALETTE;
struct HRGN__;      typedef HRGN__*     HRGN;
struct HFONT__;     typedef HFONT__*    HFONT;
struct HICON__;     typedef HICON__*    HICON;
struct HCURSOR__;   typedef HCURSOR__*  HCURSOR;
typedef void*               LPSECURITY_ATTRIBUTES;
typedef intptr_t            LRESULT;
typedef uintptr_t           WPARAM;
typedef intptr_t            LPARAM;
typedef LRESULT (CALLBACK* HOOKPROC)(int code, WPARAM wParam, LPARAM lParam);
typedef int32_t             HRESULT;
typedef LONG                SCODE;
typedef DWORD               LCID;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr)    (((HRESULT)(hr)) < 0)
#endif
#define HRESULT_FROM_WIN32(x) ((HRESULT)(x) <= 0 ? ((HRESULT)(x)) : ((HRESULT) (((x) & 0x0000FFFF) | (7 << 16) | 0x80000000)))

inline DWORD GetLastError() { return 0; }
inline void SetLastError(DWORD) {}
inline DWORD GetCurrentThreadId() { return 1; }
inline DWORD GetCurrentProcessId() { return 1; }
inline LCID GetThreadLocale() { return 1033; }
inline LONG InterlockedIncrement(LONG volatile *Addend) { return ++(*Addend); }
inline LONG InterlockedDecrement(LONG volatile *Addend) { return --(*Addend); }

#define LOCALE_USER_DEFAULT 0x0400
#define LOCALE_ILZERO       0x00000012
#define LOCALE_SDECIMAL     0x0000000E
#define LOCALE_STHOUSAND    0x0000000F
#define LOCALE_INEGNUMBER   0x00000010
#define LOCALE_IMEASURE     0x0000000D
#define HELP_CONTEXTMENU    0x000a
#define WH_CALLWNDPROC      4
#define EM_LIMITTEXT        0x00C5
#define EM_SETLIMITTEXT     EM_LIMITTEXT

#define _ttol(s)            atol(s)
#define _ltot(v,b,r)        snprintf(b, 32, "%ld", (long)(v))
#define _tcstok             strtok
#define _tcscat             strcat
#define _tcschr             strchr
#define _tcsrchr            strrchr
#define _tcscpy             strcpy
#define _tcsncpy            strncpy
#define _tcslen             strlen
#define _tcscmp             strcmp
#define _tcsicmp            strcasecmp
#define _tcsncmp            strncmp
#define _tcsnicmp           strncasecmp

typedef TCHAR*              PTSTR;
typedef const TCHAR*        PCTSTR;

#define AFX_IDS_OPENFILE    0xF006
#define AFX_IDP_FAILED_TO_UPDATE 0xF007

#define QS_ALLINPUT         0x04FF
#define WAIT_OBJECT_0       0
#define INFINITE            0xFFFFFFFF
#define STILL_ACTIVE        0x00000103

inline DWORD MsgWaitForMultipleObjects(DWORD nCount, const HANDLE* pHandles, BOOL bWaitAll, DWORD dwMilliseconds, DWORD dwWakeMask) {
    return WAIT_OBJECT_0;
}
inline uintptr_t _beginthreadex(void* security, unsigned stack_size, unsigned (*start_address)(void*), void* arglist, unsigned initflag, unsigned* thrdaddr) {
    return 1;
}
inline BOOL GetExitCodeThread(HANDLE hThread, LPDWORD lpExitCode) {
    if (lpExitCode) *lpExitCode = 0;
    return TRUE;
}
inline BOOL TerminateThread(HANDLE hThread, DWORD dwExitCode) { return TRUE; }

typedef int                 INT;
typedef int*                LPINT;

#define GENERIC_READ        0x80000000
#define GENERIC_WRITE       0x40000000
#define FILE_SHARE_READ     0x00000001
#define FILE_SHARE_WRITE    0x00000002
#define OPEN_EXISTING       3
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)

#define SHCNE_UPDATEITEM    0x00002000L
#define SHCNE_CREATE        0x00000002L

#define SPI_SETDESKWALLPAPER 0x0014
#define SPIF_UPDATEINIFILE   0x0001
#define SPIF_SENDWININICHANGE 0x0002

#define BN_CLICKED          0
#define EM_POSFROMCHAR      0x00D6
#define EN_CHANGE           0x0300
#define EN_KILLFOCUS        0x0200

#define DATE_SHORTDATE      0x00000001
#define TIME_NOSECONDS      0x00000002

#define RASTER_FONTTYPE     0x0001
#define DEVICE_FONTTYPE     0x0002
#define TRUETYPE_FONTTYPE   0x0004

#define PRIMARYLANGID(lgid) ((WORD)(lgid) & 0x3ff)
#define _tcsdup             strdup

#define SHCNF_PATH          0x0005
inline void SHChangeNotify(LONG wEventId, UINT uFlags, LPCVOID dwItem1, LPCVOID dwItem2) {}

#define LANG_JAPANESE           0x11
#define LANG_KOREAN             0x12
#define NI_COMPOSITIONSTR       0x0015
#define CPS_COMPLETE            0x0001
#define GCS_RESULTSTR           0x0800
#define GCS_RESULTREADSTR       0x0200
#define GCS_RESULTREADCLAUSE    0x0400
#define GCS_RESULTCLAUSE        0x1000

#define VK_HANJA                0x19
#define TMPF_FIXED_PITCH        0x01

#define ES_LEFT         0x0000L
#define ES_CENTER       0x0001L
#define ES_RIGHT        0x0002L
#define ES_MULTILINE    0x0004L
#define ES_UPPERCASE    0x0008L
#define ES_LOWERCASE    0x0010L
#define ES_PASSWORD     0x0020L
#define ES_AUTOVSCROLL  0x0040L
#define ES_AUTOHSCROLL  0x0080L
#define ES_NOHIDESEL    0x0100L
#define ES_OEMCONVERT   0x0400L
#define ES_READONLY     0x0800L
#define ES_WANTRETURN   0x1000L
#define ES_NUMBER       0x2000L

inline BOOL DestroyCaret() { return TRUE; }
inline BOOL CreateCaret(HWND hWnd, HBITMAP hBitmap, int nWidth, int nHeight) { return TRUE; }
inline BOOL ShowCaret(HWND hWnd) { return TRUE; }
inline BOOL HideCaret(HWND hWnd) { return TRUE; }
inline BOOL SetCaretPos(int X, int Y) { return TRUE; }
inline BOOL DestroyIcon(HICON hIcon) { return TRUE; }
inline int ShellAbout(HWND hWnd, LPCTSTR szApp, LPCTSTR szOtherStuff, HICON hIcon) { return 1; }
inline BOOL AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn) {
    if (lpszPathOut && lpszFileIn) { strncpy(lpszPathOut, lpszFileIn, 260); lpszPathOut[259] = 0; }
    return TRUE;
}

#define RDW_INVALIDATE          0x0001
#define RDW_INTERNALPAINT       0x0002
#define RDW_ERASE               0x0004
#define RDW_VALIDATE            0x0008
#define RDW_NOINTERNALPAINT     0x0010
#define RDW_NOERASE             0x0020
#define RDW_NOCHILDREN          0x0040
#define RDW_ALLCHILDREN         0x0080
#define RDW_UPDATENOW           0x0100
#define RDW_ERASENOW            0x0200
#define RDW_FRAME               0x0400
#define RDW_NOFRAME             0x0800

inline HMODULE LoadLibrary(LPCTSTR lpLibFileName) { return (HMODULE)1; }
inline void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName) { return nullptr; }
inline BOOL FreeLibrary(HMODULE hLibModule) { return TRUE; }

typedef ULONG               PROPID;
typedef void*               HMETAFILE;

struct METAFILEPICT {
    LONG      mm;
    LONG      xExt;
    LONG      yExt;
    HMETAFILE hMF;
};
typedef METAFILEPICT* LPMETAFILEPICT;

#define REG_OPTION_RESERVED     0x00000000L
#define REG_OPTION_NON_VOLATILE 0x00000000L
#define REG_OPTION_VOLATILE     0x00000001L

#ifndef _FILETIME_
#define _FILETIME_
struct FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};
#endif

struct SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

struct WIN32_FIND_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    TCHAR cFileName[260];
    TCHAR cAlternateFileName[14];
};

inline HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) { return INVALID_HANDLE_VALUE; }
inline BOOL CloseHandle(HANDLE hObject) { return TRUE; }
inline DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) { if (lpFileSizeHigh) *lpFileSizeHigh = 0; return 0; }
inline BOOL FileTimeToSystemTime(const FILETIME* lpFileTime, SYSTEMTIME* lpSystemTime) { return TRUE; }
inline BOOL GetFileTime(HANDLE hFile, FILETIME* lpCreationTime, FILETIME* lpLastAccessTime, FILETIME* lpLastWriteTime) { return TRUE; }
inline BOOL FileTimeToLocalFileTime(const FILETIME* lpFileTime, FILETIME* lpLocalFileTime) { return TRUE; }
inline HANDLE FindFirstFile(LPCTSTR lpFileName, WIN32_FIND_DATA* lpFindFileData) { return INVALID_HANDLE_VALUE; }
inline BOOL FindNextFile(HANDLE hFindFile, WIN32_FIND_DATA* lpFindFileData) { return FALSE; }
inline BOOL FindClose(HANDLE hFindFile) { return TRUE; }
inline DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize) {
    if (lpFilename && nSize > 0) { strncpy(lpFilename, "mspaint", nSize); lpFilename[nSize-1] = 0; return strlen(lpFilename); }
    return 0;
}
inline LPTSTR lstrcpyn(LPTSTR lpString1, LPCTSTR lpString2, int iMaxLength) {
    if (lpString1 && iMaxLength > 0) {
        strncpy(lpString1, lpString2 ? lpString2 : "", iMaxLength);
        lpString1[iMaxLength-1] = 0;
    }
    return lpString1;
}
inline DWORD GetShortPathName(LPCTSTR lpszLongPath, LPTSTR lpszShortPath, DWORD cchBuffer) {
    if (lpszShortPath && cchBuffer > 0) {
        strncpy(lpszShortPath, lpszLongPath ? lpszLongPath : "", cchBuffer);
        return strlen(lpszShortPath);
    }
    return 0;
}
inline BOOL SystemParametersInfo(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni) { return TRUE; }
inline LRESULT SendDlgItemMessage(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
inline BOOL SetEndOfFile(HANDLE hFile) { return TRUE; }
inline HKL GetKeyboardLayout(DWORD idThread) { return (HKL)0; }
inline int GetDateFormat(LCID Locale, DWORD dwFlags, const SYSTEMTIME* lpDate, LPCTSTR lpFormat, LPTSTR lpDateStr, int cchDate) {
    if (lpDateStr && cchDate > 0) {
        strncpy(lpDateStr, "1/1/2000", cchDate);
        lpDateStr[cchDate-1] = 0;
        return strlen(lpDateStr);
    }
    return 9;
}
inline int GetTimeFormat(LCID Locale, DWORD dwFlags, const SYSTEMTIME* lpTime, LPCTSTR lpFormat, LPTSTR lpTimeStr, int cchTime) {
    if (lpTimeStr && cchTime > 0) {
        strncpy(lpTimeStr, "12:00 PM", cchTime);
        lpTimeStr[cchTime-1] = 0;
        return strlen(lpTimeStr);
    }
    return 9;
}

struct NUMBERFMT {
    UINT      NumDigits;
    UINT      LeadingZero;
    UINT      Grouping;
    LPSTR     lpDecimalSep;
    LPSTR     lpThousandSep;
    UINT      NegativeOrder;
};

inline int GetNumberFormat(LCID Locale, DWORD dwFlags, LPCTSTR lpValue, const NUMBERFMT *lpFormat, LPTSTR lpNumberStr, int cchNumber) {
    if (lpNumberStr && cchNumber > 0) {
        strncpy(lpNumberStr, lpValue ? lpValue : "", cchNumber);
        lpNumberStr[cchNumber - 1] = 0;
        return strlen(lpNumberStr);
    }
    return lpValue ? strlen(lpValue) + 1 : 1;
}

extern int __argc;
extern char** __argv;

#define TEXT(x) x
#define _T(x)   x
#define _MAX_PATH   260
#define _MAX_DRIVE  3
#define _MAX_DIR    256
#define _MAX_FNAME  256
#define _MAX_EXT    256
#define MAX_PATH    260

#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
#ifndef CopyMemory
#define CopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#endif

inline UINT GetACP() { return 1252; }
#ifndef UIntToPtr
#define UIntToPtr(ui) ((void *)(uintptr_t)(ui))
#endif

#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define MAKELPARAM(l, h)    ((LPARAM)MAKELONG(l, h))
#define MAKEINTRESOURCE(i)  ((LPCTSTR)((ULONG_PTR)((WORD)(i))))
#define IS_INTRESOURCE(_r)  ((((ULONG_PTR)(_r)) >> 16) == 0)

// Color macros (Win32 COLORREF is 0x00BBGGRR)
typedef DWORD COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define PALETTEINDEX(i)     ((COLORREF)(0x01000000 | (DWORD)(WORD)(i)))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#ifndef S_OK
#define S_OK            ((HRESULT)0x00000000L)
#define NOERROR         0
#define S_FALSE         ((HRESULT)0x00000001L)
#define E_NOTIMPL       ((HRESULT)0x80004001L)
#define E_NOINTERFACE   ((HRESULT)0x80004002L)
#define E_FAIL          ((HRESULT)0x80004005L)
#define E_POINTER       ((HRESULT)0x80004003L)
#define E_INVALIDARG    ((HRESULT)0x80070057L)
#define E_OUTOFMEMORY   ((HRESULT)0x8007000EL)
#endif

typedef struct tagCWPSTRUCT {
    LPARAM lParam;
    WPARAM wParam;
    UINT   message;
    HWND   hwnd;
} CWPSTRUCT, *PCWPSTRUCT, *LPCWPSTRUCT;

// GUID / CLSID
struct GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
    bool operator==(const GUID& o) const {
        return std::memcmp(this, &o, sizeof(GUID)) == 0;
    }
    bool operator!=(const GUID& o) const { return !(*this == o); }
};
typedef GUID CLSID;
typedef GUID IID;
typedef const GUID& REFGUID;
typedef const IID& REFIID;
typedef const CLSID& REFCLSID;
typedef GUID* LPCLSID;

// Basic Geometry Structs
struct POINT {
    LONG x;
    LONG y;
};
typedef POINT* LPPOINT;

struct POINTS {
    short x;
    short y;
};

struct SIZE {
    LONG cx;
    LONG cy;
};
typedef SIZE* LPSIZE;

struct RECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};
typedef RECT* LPRECT;
typedef const RECT* LPCRECT;

// CSize, CPoint, CRect
class CPoint;
class CRect;

class CSize : public SIZE {
public:
    CSize() { cx = 0; cy = 0; }
    CSize(int initCX, int initCY) { cx = initCX; cy = initCY; }
    CSize(SIZE initSize) { cx = initSize.cx; cy = initSize.cy; }
    CSize(POINT initPt) { cx = initPt.x; cy = initPt.y; }
    CSize(DWORD dwSize) {
        cx = (short)LOWORD(dwSize);
        cy = (short)HIWORD(dwSize);
    }
    bool operator==(SIZE size) const { return (cx == size.cx && cy == size.cy); }
    bool operator!=(SIZE size) const { return (cx != size.cx || cy != size.cy); }
    void operator+=(SIZE size) { cx += size.cx; cy += size.cy; }
    void operator-=(SIZE size) { cx -= size.cx; cy -= size.cy; }
    CSize operator+(SIZE size) const { return CSize(cx + size.cx, cy + size.cy); }
    CSize operator-(SIZE size) const { return CSize(cx - size.cx, cy - size.cy); }
    CSize operator-() const { return CSize(-cx, -cy); }
    CPoint operator+(POINT point) const;
    CPoint operator-(POINT point) const;
};

class CPoint : public POINT {
public:
    CPoint() { x = 0; y = 0; }
    CPoint(int initX, int initY) { x = initX; y = initY; }
    CPoint(POINT initPt) { x = initPt.x; y = initPt.y; }
    CPoint(SIZE initSize) { x = initSize.cx; y = initSize.cy; }
    CPoint(DWORD dwPoint) {
        x = (short)LOWORD(dwPoint);
        y = (short)HIWORD(dwPoint);
    }
    void Offset(int xOffset, int yOffset) { x += xOffset; y += yOffset; }
    void Offset(POINT point) { x += point.x; y += point.y; }
    void Offset(SIZE size) { x += size.cx; y += size.cy; }
    bool operator==(POINT point) const { return (x == point.x && y == point.y); }
    bool operator!=(POINT point) const { return (x != point.x || y != point.y); }
    void operator+=(SIZE size) { x += size.cx; y += size.cy; }
    void operator-=(SIZE size) { x -= size.cx; y -= size.cy; }
    void operator+=(POINT point) { x += point.x; y += point.y; }
    void operator-=(POINT point) { x -= point.x; y -= point.y; }
    CPoint operator+(SIZE size) const { return CPoint(x + size.cx, y + size.cy); }
    CPoint operator-(SIZE size) const { return CPoint(x - size.cx, y - size.cy); }
    CPoint operator-() const { return CPoint(-x, -y); }
    CPoint operator+(POINT point) const { return CPoint(x + point.x, y + point.y); }
    CSize operator-(POINT point) const { return CSize(x - point.x, y - point.y); }
};

inline CPoint CSize::operator+(POINT point) const { return CPoint(cx + point.x, cy + point.y); }
inline CPoint CSize::operator-(POINT point) const { return CPoint(cx - point.x, cy - point.y); }

class CRect : public RECT {
public:
    CRect() { left = top = right = bottom = 0; }
    CRect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
    CRect(const RECT& srcRect) { left = srcRect.left; top = srcRect.top; right = srcRect.right; bottom = srcRect.bottom; }
    CRect(LPCRECT lpSrcRect) {
        if (lpSrcRect) { left = lpSrcRect->left; top = lpSrcRect->top; right = lpSrcRect->right; bottom = lpSrcRect->bottom; }
        else { left = top = right = bottom = 0; }
    }
    CRect(POINT point, SIZE size) { right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
    CRect(POINT topLeft, POINT bottomRight) { left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

    int Width() const { return right - left; }
    int Height() const { return bottom - top; }
    CSize Size() const { return CSize(right - left, bottom - top); }
    CPoint& TopLeft() { return *((CPoint*)this); }
    CPoint& BottomRight() { return *((CPoint*)this + 1); }
    const CPoint& TopLeft() const { return *((const CPoint*)this); }
    const CPoint& BottomRight() const { return *((const CPoint*)this + 1); }
    CPoint CenterPoint() const { return CPoint((left + right) / 2, (top + bottom) / 2); }

    operator LPRECT() { return this; }
    operator LPCRECT() const { return this; }

    BOOL IsRectEmpty() const { return (left >= right || top >= bottom); }
    BOOL IsRectNull() const { return (left == 0 && right == 0 && top == 0 && bottom == 0); }
    BOOL PtInRect(POINT point) const {
        return (point.x >= left && point.x < right && point.y >= top && point.y < bottom);
    }

    void SetRect(int x1, int y1, int x2, int y2) { left = x1; top = y1; right = x2; bottom = y2; }
    void SetRect(POINT topLeft, POINT bottomRight) { left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }
    void SetRectEmpty() { left = top = right = bottom = 0; }
    void CopyRect(LPCRECT lpSrcRect) {
        if (lpSrcRect) { left = lpSrcRect->left; top = lpSrcRect->top; right = lpSrcRect->right; bottom = lpSrcRect->bottom; }
    }
    BOOL EqualRect(LPCRECT lpRect) const {
        return lpRect && left == lpRect->left && top == lpRect->top && right == lpRect->right && bottom == lpRect->bottom;
    }

    void InflateRect(int x, int y) { left -= x; top -= y; right += x; bottom += y; }
    void InflateRect(SIZE size) { left -= size.cx; top -= size.cy; right += size.cx; bottom += size.cy; }
    void InflateRect(LPCRECT lpRect) {
        left -= lpRect->left; top -= lpRect->top; right += lpRect->right; bottom += lpRect->bottom;
    }
    void InflateRect(int l, int t, int r, int b) { left -= l; top -= t; right += r; bottom += b; }
    void DeflateRect(int x, int y) { InflateRect(-x, -y); }
    void DeflateRect(SIZE size) { InflateRect(-size.cx, -size.cy); }
    void DeflateRect(LPCRECT lpRect) {
        left += lpRect->left; top += lpRect->top; right -= lpRect->right; bottom -= lpRect->bottom;
    }
    void DeflateRect(int l, int t, int r, int b) { left += l; top += t; right -= r; bottom -= b; }

    void OffsetRect(int x, int y) { left += x; right += x; top += y; bottom += y; }
    void OffsetRect(SIZE size) { OffsetRect(size.cx, size.cy); }
    void OffsetRect(POINT point) { OffsetRect(point.x, point.y); }
    void NormalizeRect() {
        if (left > right) std::swap(left, right);
        if (top > bottom) std::swap(top, bottom);
    }

    BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2) {
        if (!lpRect1 || !lpRect2) { SetRectEmpty(); return FALSE; }
        LONG l = std::max(lpRect1->left, lpRect2->left);
        LONG t = std::max(lpRect1->top, lpRect2->top);
        LONG r = std::min(lpRect1->right, lpRect2->right);
        LONG b = std::min(lpRect1->bottom, lpRect2->bottom);
        if (l < r && t < b) {
            left = l; top = t; right = r; bottom = b;
            return TRUE;
        }
        SetRectEmpty();
        return FALSE;
    }

    BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2) {
        if (!lpRect1 || !lpRect2) return FALSE;
        bool e1 = (lpRect1->left >= lpRect1->right || lpRect1->top >= lpRect1->bottom);
        bool e2 = (lpRect2->left >= lpRect2->right || lpRect2->top >= lpRect2->bottom);
        if (e1 && e2) { SetRectEmpty(); return FALSE; }
        if (e1) { CopyRect(lpRect2); return TRUE; }
        if (e2) { CopyRect(lpRect1); return TRUE; }
        left = std::min(lpRect1->left, lpRect2->left);
        top = std::min(lpRect1->top, lpRect2->top);
        right = std::max(lpRect1->right, lpRect2->right);
        bottom = std::max(lpRect1->bottom, lpRect2->bottom);
        return TRUE;
    }

    BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) {
        if (!lpRectSrc1) return FALSE;
        CopyRect(lpRectSrc1);
        return !IsRectEmpty();
    }

    void operator=(const RECT& srcRect) { CopyRect(&srcRect); }
    BOOL operator==(const RECT& rect) const { return EqualRect(&rect); }
    BOOL operator!=(const RECT& rect) const { return !EqualRect(&rect); }
    void operator+=(POINT point) { OffsetRect(point.x, point.y); }
    void operator+=(SIZE size) { OffsetRect(size.cx, size.cy); }
    void operator+=(LPCRECT lpRect) { InflateRect(lpRect); }
    void operator-=(POINT point) { OffsetRect(-point.x, -point.y); }
    void operator-=(SIZE size) { OffsetRect(-size.cx, -size.cy); }
    void operator-=(LPCRECT lpRect) { DeflateRect(lpRect); }
    void operator&=(const RECT& rect) { CRect r(*this); IntersectRect(&r, &rect); }
    void operator|=(const RECT& rect) { CRect r(*this); UnionRect(&r, &rect); }

    CRect operator+(POINT pt) const { CRect r(*this); r.OffsetRect(pt); return r; }
    CRect operator-(POINT pt) const { CRect r(*this); r.OffsetRect(-pt.x, -pt.y); return r; }
    CRect operator+(SIZE s) const { CRect r(*this); r.OffsetRect(s); return r; }
    CRect operator-(SIZE s) const { CRect r(*this); r.OffsetRect(-s.cx, -s.cy); return r; }
    CRect operator&(const RECT& rect2) const { CRect r; r.IntersectRect(this, &rect2); return r; }
    CRect operator|(const RECT& rect2) const { CRect r; r.UnionRect(this, &rect2); return r; }
};

// Win32 Rect helper functions
inline BOOL SetRect(LPRECT lprc, int xLeft, int yTop, int xRight, int yBottom) {
    if (!lprc) return FALSE;
    lprc->left = xLeft; lprc->top = yTop; lprc->right = xRight; lprc->bottom = yBottom;
    return TRUE;
}
inline BOOL SetRectEmpty(LPRECT lprc) {
    if (!lprc) return FALSE;
    lprc->left = lprc->top = lprc->right = lprc->bottom = 0;
    return TRUE;
}
inline BOOL CopyRect(LPRECT lprcDst, LPCRECT lprcSrc) {
    if (!lprcDst || !lprcSrc) return FALSE;
    *lprcDst = *lprcSrc;
    return TRUE;
}
inline BOOL InflateRect(LPRECT lprc, int dx, int dy) {
    if (!lprc) return FALSE;
    lprc->left -= dx; lprc->top -= dy; lprc->right += dx; lprc->bottom += dy;
    return TRUE;
}
inline BOOL OffsetRect(LPRECT lprc, int dx, int dy) {
    if (!lprc) return FALSE;
    lprc->left += dx; lprc->right += dx; lprc->top += dy; lprc->bottom += dy;
    return TRUE;
}
inline BOOL PtInRect(LPCRECT lprc, POINT pt) {
    if (!lprc) return FALSE;
    return (pt.x >= lprc->left && pt.x < lprc->right && pt.y >= lprc->top && pt.y < lprc->bottom);
}
inline BOOL GetCaretPos(LPPOINT lpPoint) { if (lpPoint) { lpPoint->x = 0; lpPoint->y = 0; } return TRUE; }
inline BOOL IntersectRect(LPRECT lprcDst, LPCRECT lprcSrc1, LPCRECT lprcSrc2) {
    CRect r;
    BOOL b = r.IntersectRect(lprcSrc1, lprcSrc2);
    if (lprcDst) *lprcDst = r;
    return b;
}
inline BOOL UnionRect(LPRECT lprcDst, LPCRECT lprcSrc1, LPCRECT lprcSrc2) {
    CRect r;
    BOOL b = r.UnionRect(lprcSrc1, lprcSrc2);
    if (lprcDst) *lprcDst = r;
    return b;
}
inline BOOL IsRectEmpty(LPCRECT lprc) {
    return !lprc || lprc->left >= lprc->right || lprc->top >= lprc->bottom;
}
inline BOOL EqualRect(LPCRECT lprc1, LPCRECT lprc2) {
    return lprc1 && lprc2 && lprc1->left == lprc2->left && lprc1->top == lprc2->top &&
           lprc1->right == lprc2->right && lprc1->bottom == lprc2->bottom;
}

#ifndef NOMINMAX
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#endif

#define DYNAMIC_DOWNCAST(class_name, object) dynamic_cast<class_name*>(object)
#define STATIC_DOWNCAST(class_name, object) static_cast<class_name*>(object)

#ifndef __try
#define __try try
#endif
#ifndef __except
#define __except(x) catch(...)
#endif
#ifndef EXCEPTION_EXECUTE_HANDLER
#define EXCEPTION_EXECUTE_HANDLER 1
#endif

#ifndef DVASPECT_CONTENT
typedef DWORD DVASPECT;
#define DVASPECT_CONTENT    1
#define DVASPECT_THUMBNAIL  2
#define DVASPECT_ICON       4
#define DVASPECT_DOCPRINT   8
#endif

#include "mfc_types.h"
