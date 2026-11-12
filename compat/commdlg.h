#pragma once
#include "afxwin.h"
#include "cderr.h"

typedef UINT_PTR (CALLBACK *LPOFNHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

struct OPENFILENAME {
    DWORD        lStructSize;
    HWND         hwndOwner;
    HINSTANCE    hInstance;
    LPCTSTR      lpstrFilter;
    LPTSTR       lpstrCustomFilter;
    DWORD        nMaxCustFilter;
    DWORD        nFilterIndex;
    LPTSTR       lpstrFile;
    DWORD        nMaxFile;
    LPTSTR       lpstrFileTitle;
    DWORD        nMaxFileTitle;
    LPCTSTR      lpstrInitialDir;
    LPCTSTR      lpstrTitle;
    DWORD        Flags;
    WORD         nFileOffset;
    WORD         nFileExtension;
    LPCTSTR      lpstrDefExt;
    LPARAM       lCustData;
    LPOFNHOOKPROC lpfnHook;
    LPCTSTR      lpTemplateName;
    void *       pvReserved;
    DWORD        dwReserved;
    DWORD        FlagsEx;
};
typedef OPENFILENAME *LPOPENFILENAME;
#define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)

#define OFN_READONLY                 0x00000001
#define OFN_OVERWRITEPROMPT          0x00000002
#define OFN_HIDEREADONLY             0x00000004
#define OFN_NOCHANGEDIR              0x00000008
#define OFN_SHOWHELP                 0x00000010
#define OFN_ENABLEHOOK               0x00000020
#define OFN_ENABLETEMPLATE           0x00000040
#define OFN_ENABLETEMPLATEHANDLE     0x00000080
#define OFN_NOVALIDATE               0x00000100
#define OFN_ALLOWMULTISELECT         0x00000200
#define OFN_EXTENSIONDIFFERENT       0x00000400
#define OFN_PATHMUSTEXIST            0x00000800
#define OFN_FILEMUSTEXIST            0x00001000
#define OFN_CREATEPROMPT             0x00002000
#define OFN_SHAREAWARE               0x00004000
#define OFN_NOREADONLYRETURN         0x00008000
#define OFN_NOTESTFILECREATE         0x00010000
#define OFN_NONETWORKBUTTON          0x00020000
#define OFN_NOLONGNAMES              0x00040000
#define OFN_EXPLORER                 0x00080000
#define OFN_NODEREFERENCELINKS       0x00100000
#define OFN_LONGNAMES                0x00200000
#define OFN_ENABLEINCLUDENOTIFY      0x00400000
#define OFN_ENABLESIZING             0x00800000

#define PSD_ENABLEPAGESETUPHOOK           0x00002000
#define PSD_ENABLEPAGEPAINTHOOK           0x00040000
#define PSD_ENABLEPAGESETUPTEMPLATE       0x00008000
#define PSD_ENABLEPAGESETUPTEMPLATEHANDLE 0x00020000
#define PSD_MARGINS                       0x00000002
#define PSD_INTHOUSANDTHSOFINCHES         0x00000004
#define PSD_INHUNDREDTHSOFMILLIMETERS     0x00000008

#define WM_PSD_PAGESETUPDLG   (WM_USER  )
#define WM_PSD_FULLPAGERECT   (WM_USER+1)
#define WM_PSD_MINMARGINRECT  (WM_USER+2)
#define WM_PSD_MARGINRECT     (WM_USER+3)
#define WM_PSD_GREEKTEXTRECT  (WM_USER+4)
#define WM_PSD_ENVSTAMPRECT   (WM_USER+5)
#define WM_PSD_YAFULLPAGERECT (WM_USER+6)

inline BOOL GetOpenFileName(LPOPENFILENAME lpofn) { return FALSE; }
inline BOOL GetSaveFileName(LPOPENFILENAME lpofn) { return FALSE; }
