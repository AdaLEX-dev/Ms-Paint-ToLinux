#pragma once

// ============================================================================
// Win32 & GDI Constants
// ============================================================================

// Ternary Raster Operations
#define SRCCOPY             (DWORD)0x00CC0020
#define SRCPAINT            (DWORD)0x00EE0086
#define SRCAND              (DWORD)0x008800C6
#define SRCINVERT           (DWORD)0x00660046
#define SRCERASE            (DWORD)0x00440328
#define NOTSRCCOPY          (DWORD)0x00330008
#define NOTSRCERASE         (DWORD)0x001100A6
#define MERGECOPY           (DWORD)0x00C000CA
#define MERGEPAINT          (DWORD)0x00BB0226
#define PATCOPY             (DWORD)0x00F00021
#define PATPAINT            (DWORD)0x00FB0A09
#define PATINVERT           (DWORD)0x005A0049
#define DSTINVERT           (DWORD)0x00550009
#define BLACKNESS           (DWORD)0x00000042
#define WHITENESS           (DWORD)0x00FF0062

// Binary Raster Ops (SetROP2)
#define R2_BLACK            1
#define R2_NOTMERGEPEN      2
#define R2_MASKNOTPEN       3
#define R2_NOTCOPYPEN       4
#define R2_MASKPENNOT       5
#define R2_NOT              6
#define R2_XORPEN           7
#define R2_NOTMASKPEN       8
#define R2_MASKPEN          9
#define R2_NOTXORPEN        10
#define R2_NOP              11
#define R2_MERGENOTPEN      12
#define R2_COPYPEN          13
#define R2_MERGEPENNOT      14
#define R2_MERGEPEN         15
#define R2_WHITE            16

// Stock Objects
#define WHITE_BRUSH         0
#define LTGRAY_BRUSH        1
#define GRAY_BRUSH          2
#define DKGRAY_BRUSH        3
#define BLACK_BRUSH         4
#define NULL_BRUSH          5
#define HOLLOW_BRUSH        NULL_BRUSH
#define WHITE_PEN           6
#define BLACK_PEN           7
#define NULL_PEN            8
#define OEM_FIXED_FONT      10
#define ANSI_FIXED_FONT     11
#define ANSI_VAR_FONT       12
#define SYSTEM_FONT         13
#define DEVICE_DEFAULT_FONT 14
#define DEFAULT_PALETTE     15
#define SYSTEM_FIXED_FONT   16
#define DEFAULT_GUI_FONT    17

// Pen Styles
#define PS_SOLID            0
#define PS_DASH             1
#define PS_DOT              2
#define PS_DASHDOT          3
#define PS_DASHDOTDOT       4
#define PS_NULL             5
#define PS_INSIDEFRAME      6

// Brush Styles
#define BS_SOLID            0
#define BS_NULL             1
#define BS_HOLLOW           BS_NULL
#define BS_HATCHED          2
#define BS_PATTERN          3

// Background Modes
#define TRANSPARENT         1
#define OPAQUE              2

// PolyFill Modes
#define ALTERNATE           1
#define WINDING             2

// Mapping Modes
#define MM_TEXT             1
#define MM_LOMETRIC         2
#define MM_HIMETRIC         3
#define MM_LOENGLISH        4
#define MM_HIENGLISH        5
#define MM_TWIPS            6
#define MM_ISOTROPIC        7
#define MM_ANISOTROPIC      8

// StretchBlt Modes
#define BLACKONWHITE        1
#define WHITEONBLACK        2
#define COLORONCOLOR        3
#define HALFTONE            4

// Region Combine Modes
#define ERROR               0
#define NULLREGION          1
#define SIMPLEREGION        2
#define COMPLEXREGION       3
#define RGN_AND             1
#define RGN_OR              2
#define RGN_XOR             3
#define RGN_DIFF            4
#define RGN_COPY            5

// FloodFill Modes
#define FLOODFILLBORDER     0
#define FLOODFILLSURFACE    1

// DIB Constants
#define BI_RGB              0L
#define BI_RLE8             1L
#define BI_RLE4             2L
#define BI_BITFIELDS        3L
#define DIB_RGB_COLORS      0
#define DIB_PAL_COLORS      1
#define CBM_INIT            0x04L

// Palette Flags
#define PC_RESERVED         0x01
#define PC_EXPLICIT         0x02
#define PC_NOCOLLAPSE       0x04

#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13

#define DIBINDEX(n)         PALETTEINDEX(n)

// Device Caps
#define DRIVERVERSION       0
#define TECHNOLOGY          2
#define HORZSIZE            4
#define VERTSIZE            6
#define HORZRES             8
#define VERTRES             10
#define BITSPIXEL           12
#define PLANES              14
#define NUMBRUSHES          16
#define NUMPENS             18
#define NUMMARKERS          20
#define NUMFONTS            22
#define NUMCOLORS           24
#define RASTERCAPS          38
#define LOGPIXELSX          88
#define LOGPIXELSY          90
#define SIZEPALETTE         104
#define RC_PALETTE          0x0100
#define PHYSICALWIDTH       110
#define PHYSICALHEIGHT      111
#define PHYSICALOFFSETX     112
#define PHYSICALOFFSETY     113
#define RC_BITBLT           1
#define RC_STRETCHBLT       0x0800
#define RC_STRETCHDIB       0x2000

// System Colors
#define COLOR_SCROLLBAR         0
#define COLOR_BACKGROUND        1
#define COLOR_ACTIVECAPTION     2
#define COLOR_INACTIVECAPTION   3
#define COLOR_MENU              4
#define COLOR_WINDOW            5
#define COLOR_WINDOWFRAME       6
#define COLOR_MENUTEXT          7
#define COLOR_WINDOWTEXT        8
#define COLOR_CAPTIONTEXT       9
#define COLOR_ACTIVEBORDER      10
#define COLOR_INACTIVEBORDER    11
#define COLOR_APPWORKSPACE      12
#define COLOR_HIGHLIGHT         13
#define COLOR_HIGHLIGHTTEXT     14
#define COLOR_BTNFACE           15
#define COLOR_BTNSHADOW         16
#define COLOR_GRAYTEXT          17
#define COLOR_BTNTEXT           18
#define COLOR_INACTIVECAPTIONTEXT 19
#define COLOR_BTNHIGHLIGHT      20
#define COLOR_3DDKSHADOW        21
#define COLOR_3DLIGHT           22
#define COLOR_INFOTEXT          23
#define COLOR_INFOBK            24
#define COLOR_3DFACE            COLOR_BTNFACE
#define COLOR_3DSHADOW          COLOR_BTNSHADOW
#define COLOR_3DHIGHLIGHT       COLOR_BTNHIGHLIGHT
#define COLOR_3DHILIGHT         COLOR_BTNHIGHLIGHT

// System Metrics
#define SM_CXSCREEN             0
#define SM_CYSCREEN             1
#define SM_CXVSCROLL            2
#define SM_CYHSCROLL            3
#define SM_CYCAPTION            4
#define SM_CXBORDER             5
#define SM_CYBORDER             6
#define SM_CXDLGFRAME           7
#define SM_CYDLGFRAME           8
#define SM_CXICON               11
#define SM_CYICON               12
#define SM_CXCURSOR             13
#define SM_CYCURSOR             14
#define SM_CYMENU               15
#define SM_CXFULLSCREEN         16
#define SM_CYFULLSCREEN         17
#define SM_CXFRAME              32
#define SM_CYFRAME              33
#define SM_PENWINDOWS           41
#define SM_CXEDGE               45
#define SM_CYEDGE               46
#define SM_CYSMCAPTION          54
#define SM_SLOWMACHINE          73

// DrawEdge / DrawFrameControl
#define BDR_RAISEDOUTER         0x0001
#define BDR_SUNKENOUTER         0x0002
#define BDR_RAISEDINNER         0x0004
#define BDR_SUNKENINNER         0x0008
#define BDR_OUTER               0x0003
#define BDR_INNER               0x000c
#define EDGE_RAISED             (BDR_RAISEDOUTER | BDR_RAISEDINNER)
#define EDGE_SUNKEN             (BDR_SUNKENOUTER | BDR_SUNKENINNER)
#define EDGE_ETCHED             (BDR_SUNKENOUTER | BDR_RAISEDINNER)
#define EDGE_BUMP               (BDR_RAISEDOUTER | BDR_SUNKENINNER)
#define BF_LEFT                 0x0001
#define BF_TOP                  0x0002
#define BF_RIGHT                0x0004
#define BF_BOTTOM               0x0008
#define BF_TOPLEFT              (BF_TOP | BF_LEFT)
#define BF_TOPRIGHT             (BF_TOP | BF_RIGHT)
#define BF_BOTTOMLEFT           (BF_BOTTOM | BF_LEFT)
#define BF_BOTTOMRIGHT          (BF_BOTTOM | BF_RIGHT)
#define BF_RECT                 (BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM)
#define BF_MIDDLE               0x0800
#define BF_SOFT                 0x1000
#define BF_ADJUST               0x2000
#define BF_FLAT                 0x4000
#define BF_MONO                 0x8000

// DrawText flags
#define DT_TOP                  0x00000000
#define DT_LEFT                 0x00000000
#define DT_CENTER               0x00000001
#define DT_RIGHT                0x00000002
#define DT_VCENTER              0x00000004
#define DT_BOTTOM               0x00000008
#define DT_WORDBREAK            0x00000010
#define DT_SINGLELINE           0x00000020
#define DT_EXPANDTABS           0x00000040
#define DT_TABSTOP              0x00000080
#define DT_NOCLIP               0x00000100
#define DT_EXTERNALLEADING      0x00000200
#define DT_CALCRECT             0x00000400
#define DT_NOPREFIX             0x00000800
#define DT_INTERNAL             0x00001000

// Text Align
#define TA_NOUPDATECP           0
#define TA_UPDATECP             1
#define TA_LEFT                 0
#define TA_RIGHT                2
#define TA_CENTER               6
#define TA_TOP                  0
#define TA_BOTTOM               8
#define TA_BASELINE             24

#define ETO_OPAQUE              0x0002
#define ETO_CLIPPED             0x0004

// Window Styles
#define WS_OVERLAPPED           0x00000000L
#define WS_POPUP                0x80000000L
#define WS_CHILD                0x40000000L
#define WS_MINIMIZE             0x20000000L
#define WS_VISIBLE              0x10000000L
#define WS_DISABLED             0x08000000L
#define WS_CLIPSIBLINGS         0x04000000L
#define WS_CLIPCHILDREN         0x02000000L
#define WS_MAXIMIZE             0x01000000L
#define WS_CAPTION              0x00C00000L
#define WS_BORDER               0x00800000L
#define WS_DLGFRAME             0x00400000L
#define WS_VSCROLL              0x00200000L
#define WS_HSCROLL              0x00100000L
#define WS_SYSMENU              0x00080000L
#define WS_THICKFRAME           0x00040000L
#define WS_GROUP                0x00020000L
#define WS_TABSTOP              0x00010000L
#define WS_MINIMIZEBOX          0x00020000L
#define WS_MAXIMIZEBOX          0x00010000L
#define WS_OVERLAPPEDWINDOW     (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#define WS_POPUPWINDOW          (WS_POPUP | WS_BORDER | WS_SYSMENU)

inline int MulDiv(int nNumber, int nNumerator, int nDenominator) {
    if (nDenominator == 0) return -1;
    return (int)(((int64_t)nNumber * nNumerator) / nDenominator);
}

#define WS_EX_DLGMODALFRAME     0x00000001L
#define WS_EX_TOPMOST           0x00000008L
#define WS_EX_ACCEPTFILES       0x00000010L
#define WS_EX_TRANSPARENT       0x00000020L
#define WS_EX_TOOLWINDOW        0x00000080L
#define WS_EX_WINDOWEDGE        0x00000100L
#define WS_EX_CLIENTEDGE        0x00000200L
#define WS_EX_STATICEDGE        0x00020000L

// Class Styles
#define CS_VREDRAW              0x0001
#define CS_HREDRAW              0x0002
#define CS_DBLCLKS              0x0008
#define CS_OWNDC                0x0020
#define CS_CLASSDC              0x0040
#define CS_PARENTDC             0x0080
#define CS_SAVEBITS             0x0800
#define CS_BYTEALIGNCLIENT      0x1000
#define CS_BYTEALIGNWINDOW      0x2000

#define GWL_WNDPROC             (-4)
#define GWL_HINSTANCE           (-6)
#define GWL_HWNDPARENT          (-8)
#define GWL_STYLE               (-16)
#define GWL_EXSTYLE             (-20)
#define GWL_USERDATA            (-21)
#define GWL_ID                  (-12)

#define GWLP_WNDPROC            GWL_WNDPROC
#define GWLP_HINSTANCE          GWL_HINSTANCE
#define GWLP_HWNDPARENT         GWL_HWNDPARENT
#define GWLP_USERDATA           GWL_USERDATA
#define GWLP_ID                 GWL_ID

#define GCL_MENUNAME            (-8)
#define GCL_HBRBACKGROUND       (-10)
#define GCL_HCURSOR             (-12)
#define GCL_HICON               (-14)
#define GCL_HMODULE             (-16)
#define GCL_WNDPROC             (-24)
#define GCL_HICONSM             (-34)

#define GCLP_MENUNAME           GCL_MENUNAME
#define GCLP_HBRBACKGROUND      GCL_HBRBACKGROUND
#define GCLP_HCURSOR            GCL_HCURSOR
#define GCLP_HICON              GCL_HICON
#define GCLP_HMODULE            GCL_HMODULE
#define GCLP_WNDPROC            GCL_WNDPROC
#define GCLP_HICONSM            GCL_HICONSM

#define FILE_ATTRIBUTE_READONLY     0x00000001
#define FILE_ATTRIBUTE_HIDDEN       0x00000002
#define FILE_ATTRIBUTE_SYSTEM       0x00000004
#define FILE_ATTRIBUTE_DIRECTORY    0x00000010
#define FILE_ATTRIBUTE_ARCHIVE      0x00000020
#define FILE_ATTRIBUTE_NORMAL       0x00000080
#define INVALID_FILE_ATTRIBUTES     ((DWORD)-1)

#define CB_ERR                      (-1)
#define CB_ERRSPACE                 (-2)
#define CBS_DROPDOWN                0x0002L
#define CBS_DROPDOWNLIST            0x0003L
#define CBS_OWNERDRAWFIXED          0x0010L
#define CBS_OWNERDRAWVARIABLE       0x0020L
#define CBS_AUTOHSCROLL             0x0040L
#define CBS_SORT                    0x0100L
#define CBS_HASSTRINGS              0x0200L

// SysCommands
#define SC_SIZE                 0xF000
#define SC_MOVE                 0xF010
#define SC_MINIMIZE             0xF020
#define SC_MAXIMIZE             0xF030
#define SC_NEXTWINDOW           0xF040
#define SC_PREVWINDOW           0xF050
#define SC_CLOSE                0xF060
#define SC_VSCROLL              0xF070
#define SC_HSCROLL              0xF080
#define SC_MOUSEMENU            0xF090
#define SC_KEYMENU              0xF100
#define SC_ARRANGE              0xF110
#define SC_RESTORE              0xF120
#define SC_TASKLIST             0xF130
#define SC_SCREENSAVE           0xF140
#define SC_HOTKEY               0xF150

// GetWindow Commands
#define GW_HWNDFIRST            0
#define GW_HWNDLAST             1
#define GW_HWNDNEXT             2
#define GW_HWNDPREV             3
#define GW_OWNER                4
#define GW_CHILD                5

#define GDI_ERROR               (0xFFFFFFFFL)

// ShowWindow Commands
#define SW_HIDE                 0
#define SW_SHOWNORMAL           1
#define SW_NORMAL               1
#define SW_SHOWMINIMIZED        2
#define SW_SHOWMAXIMIZED        3
#define SW_MAXIMIZE             3
#define SW_SHOWNOACTIVATE       4
#define SW_SHOW                 5
#define SW_MINIMIZE             6
#define SW_SHOWMINNOACTIVE      7
#define SW_SHOWNA               8
#define SW_RESTORE              9
#define SW_SHOWDEFAULT          10

// ScrollBar Constants
#define SB_HORZ                 0
#define SB_VERT                 1
#define SB_CTL                  2
#define SB_BOTH                 3
#define SB_LINEUP               0
#define SB_LINELEFT             0
#define SB_LINEDOWN             1
#define SB_LINERIGHT            1
#define SB_PAGEUP               2
#define SB_PAGELEFT             2
#define SB_PAGEDOWN             3
#define SB_PAGERIGHT            3
#define SB_THUMBPOSITION        4
#define SB_THUMBTRACK           5
#define SB_TOP                  6
#define SB_LEFT                 6
#define SB_BOTTOM               7
#define SB_RIGHT                7
#define SB_ENDSCROLL            8
#define ESB_ENABLE_BOTH         0x0000
#define ESB_DISABLE_BOTH        0x0003

typedef struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int  nMin;
    int  nMax;
    UINT nPage;
    int  nPos;
    int  nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;
typedef const SCROLLINFO *LPCSCROLLINFO;

#define SIF_RANGE           0x0001
#define SIF_PAGE            0x0002
#define SIF_POS             0x0004
#define SIF_DISABLENOSCROLL 0x0008
#define SIF_TRACKPOS        0x0010
#define SIF_ALL             (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS)

// Window Messages
#define WM_NULL                 0x0000
#define WM_CREATE               0x0001
#define WM_DESTROY              0x0002
#define WM_MOVE                 0x0003
#define WM_SIZE                 0x0005
#define SIZE_RESTORED           0
#define SIZE_MINIMIZED          1
#define SIZE_MAXIMIZED          2
#define SIZE_MAXSHOW            3
#define SIZE_MAXHIDE            4
#define WM_ACTIVATE             0x0006
#define WM_SETFOCUS             0x0007
#define WM_KILLFOCUS            0x0008
#define WM_ENABLE               0x000A
#define WM_SETREDRAW            0x000B
#define WM_SETTEXT              0x000C
#define WM_GETTEXT              0x000D
#define WM_GETTEXTLENGTH        0x000E
#define WM_PAINT                0x000F
#define WM_CLOSE                0x0010
#define WM_QUIT                 0x0012
#define WM_ERASEBKGND           0x0014
#define WM_SYSCOLORCHANGE       0x0015
#define WM_SHOWWINDOW           0x0018
#define WM_WININICHANGE         0x001A
#define WM_DEVMODECHANGE        0x001B
#define WM_ACTIVATEAPP          0x001C
#define WM_FONTCHANGE           0x001D
#define WM_CANCELMODE           0x001F
#define WM_SETCURSOR            0x0020
#define WM_MOUSEACTIVATE        0x0021
#define WM_GETMINMAXINFO        0x0024
#define WM_DRAWITEM             0x002B
#define WM_MEASUREITEM          0x002C
#define WM_SETFONT              0x0030
#define WM_GETFONT              0x0031
#define WM_WINDOWPOSCHANGING    0x0046
#define WM_WINDOWPOSCHANGED     0x0047
#define WM_INPUTLANGCHANGE      0x0051
#define WM_HELP                 0x0053
#define WM_CONTEXTMENU          0x007B
#define WM_NCCREATE             0x0081
#define WM_NCDESTROY            0x0082
#define WM_NCCALCSIZE           0x0083
#define WM_NCHITTEST            0x0084
#define WM_NCPAINT              0x0085
#define WM_NCACTIVATE           0x0086
#define WM_NCMOUSEMOVE          0x00A0
#define WM_NCLBUTTONDOWN        0x00A1
#define WM_NCLBUTTONUP          0x00A2
#define WM_KEYDOWN              0x0100
#define WM_KEYUP                0x0101
#define WM_CHAR                 0x0102
#define WM_SYSKEYDOWN           0x0104
#define WM_SYSKEYUP             0x0105
#define WM_SYSCHAR              0x0106
#define WM_IME_COMPOSITION      0x010F
#define WM_INITDIALOG           0x0110
#define WM_COMMAND              0x0111
#define WM_SYSCOMMAND           0x0112
#define WM_TIMER                0x0113
#define WM_HSCROLL              0x0114
#define WM_VSCROLL              0x0115
#define WM_INITMENU             0x0116
#define WM_INITMENUPOPUP        0x0117
#define WM_CTLCOLOR             0x0019
#define WM_CTLCOLORMSGBOX       0x0132
#define WM_CTLCOLOREDIT         0x0133
#define WM_CTLCOLORLISTBOX      0x0134
#define WM_CTLCOLORBTN          0x0135
#define WM_CTLCOLORDLG          0x0136
#define WM_CTLCOLORSCROLLBAR    0x0137
#define WM_CTLCOLORSTATIC       0x0138

#define TBBS_BUTTON             0x0000
#define TBBS_SEPARATOR          0x0001
#define TBBS_CHECKBOX           0x0002
#define TBBS_CHECKED            0x0004
#define TBBS_PRESSED            0x0008
#define TBBS_DISABLED           0x0010
#define TBBS_INDETERMINATE      0x0020
#define WM_MOUSEMOVE            0x0200
#define WM_LBUTTONDOWN          0x0201
#define WM_LBUTTONUP            0x0202
#define WM_LBUTTONDBLCLK        0x0203
#define WM_RBUTTONDOWN          0x0204
#define WM_RBUTTONUP            0x0205
#define WM_RBUTTONDBLCLK        0x0206
#define WM_MBUTTONDOWN          0x0207
#define WM_MBUTTONUP            0x0208
#define WM_MOUSEWHEEL           0x020A
#define WHEEL_DELTA             120
#define WM_SIZING               0x0214
#define WM_CAPTURECHANGED       0x0215
#define WM_MOVING               0x0216
#define WM_IME_CHAR             0x0286
#define WM_CUT                  0x0300
#define WM_COPY                 0x0301
#define WM_PASTE                0x0302
#define WM_CLEAR                0x0303
#define WM_UNDO                 0x0304
#define WM_DESTROYCLIPBOARD     0x0307
#define WM_QUERYNEWPALETTE      0x030F
#define WM_PALETTEISCHANGING    0x0310
#define WM_PALETTECHANGED       0x0311
#define WM_THEMECHANGED         0x031A
#define WM_SETMESSAGESTRING     0x0362
#define WM_USER                 0x0400
#define WM_APP                  0x8000
#define WM_SIZEPARENT           0x0361
#define WM_HELPHITTEST          0x0366

// HitTest codes
#define HTERROR                 (-2)
#define HTTRANSPARENT           (-1)
#define HTNOWHERE               0
#define HTCLIENT                1
#define HTCAPTION               2
#define HTSYSMENU               3
#define HTGROWBOX               4
#define HTSIZE                  HTGROWBOX
#define HTMENU                  5
#define HTHSCROLL               6
#define HTVSCROLL               7
#define HTMINBUTTON             8
#define HTMAXBUTTON             9
#define HTLEFT                  10
#define HTRIGHT                 11
#define HTTOP                   12
#define HTTOPLEFT               13
#define HTTOPRIGHT              14
#define HTBOTTOM                15
#define HTBOTTOMLEFT            16
#define HTBOTTOMRIGHT           17
#define HTBORDER                18

// Mouse & Virtual Key Flags
#define MK_LBUTTON              0x0001
#define MK_RBUTTON              0x0002
#define MK_SHIFT                0x0004
#define MK_CONTROL              0x0008
#define MK_MBUTTON              0x0010

#define VK_LBUTTON              0x01
#define VK_RBUTTON              0x02
#define VK_CANCEL               0x03
#define VK_MBUTTON              0x04
#define VK_BACK                 0x08
#define VK_TAB                  0x09
#define VK_RETURN               0x0D
#define VK_SHIFT                0x10
#define VK_CONTROL              0x11
#define VK_MENU                 0x12
#define VK_PAUSE                0x13
#define VK_CAPITAL              0x14
#define VK_ESCAPE               0x1B
#define VK_SPACE                0x20
#define VK_PRIOR                0x21
#define VK_NEXT                 0x22
#define VK_END                  0x23
#define VK_HOME                 0x24
#define VK_LEFT                 0x25
#define VK_UP                   0x26
#define VK_RIGHT                0x27
#define VK_DOWN                 0x28
#define VK_SELECT               0x29
#define VK_PRINT                0x2A
#define VK_EXECUTE              0x2B
#define VK_SNAPSHOT             0x2C
#define VK_INSERT               0x2D
#define VK_DELETE               0x2E
#define VK_HELP                 0x2F
#define VK_NUMPAD0              0x60
#define VK_ADD                  0x6B
#define VK_SUBTRACT             0x6D
#define VK_F1                   0x70
#define VK_F2                   0x71
#define VK_F3                   0x72
#define VK_F4                   0x73
#define VK_F5                   0x74
#define VK_F6                   0x75
#define VK_F7                   0x76
#define VK_F8                   0x77
#define VK_F9                   0x78
#define VK_F10                  0x79
#define VK_F11                  0x7A
#define VK_F12                  0x7B

// Standard Cursors
#define IDC_ARROW               MAKEINTRESOURCE(32512)
#define IDC_IBEAM               MAKEINTRESOURCE(32513)
#define IDC_WAIT                MAKEINTRESOURCE(32514)
#define IDC_CROSS               MAKEINTRESOURCE(32515)
#define IDC_UPARROW             MAKEINTRESOURCE(32516)
#define IDC_SIZE                MAKEINTRESOURCE(32640)
#define IDC_ICON                MAKEINTRESOURCE(32641)
#define IDC_SIZENWSE            MAKEINTRESOURCE(32642)
#define IDC_SIZENESW            MAKEINTRESOURCE(32643)
#define IDC_SIZEWE              MAKEINTRESOURCE(32644)
#define IDC_SIZENS              MAKEINTRESOURCE(32645)
#define IDC_SIZEALL             MAKEINTRESOURCE(32646)
#define IDC_NO                  MAKEINTRESOURCE(32648)
#define IDC_HAND                MAKEINTRESOURCE(32649)

// MessageBox & Dialog IDs
#define IDOK                    1
#define IDCANCEL                2
#define IDABORT                 3
#define IDRETRY                 4
#define IDIGNORE                5
#define IDYES                   6
#define IDNO                    7
#define MB_OK                   0x00000000L
#define MB_OKCANCEL             0x00000001L
#define MB_ABORTRETRYIGNORE     0x00000002L
#define MB_YESNOCANCEL          0x00000003L
#define MB_YESNO                0x00000004L
#define MB_RETRYCANCEL          0x00000005L
#define MB_ICONHAND             0x00000010L
#define MB_ICONQUESTION         0x00000020L
#define MB_ICONEXCLAMATION      0x00000030L
#define MB_ICONASTERISK         0x00000040L
#define MB_ICONWARNING          MB_ICONEXCLAMATION
#define MB_ICONERROR            MB_ICONHAND
#define MB_ICONINFORMATION      MB_ICONASTERISK
#define MB_ICONSTOP             MB_ICONHAND
#define MB_APPLMODAL            0x00000000L
#define MB_SYSTEMMODAL          0x00001000L
#define MB_TASKMODAL            0x00002000L
#define MB_SETFOREGROUND        0x00010000L

// Menu Flags
#define MF_INSERT               0x00000000L
#define MF_CHANGE               0x00000080L
#define MF_APPEND               0x00000100L
#define MF_DELETE               0x00000200L
#define MF_REMOVE               0x00001000L
#define MF_BYCOMMAND            0x00000000L
#define MF_BYPOSITION           0x00000400L
#define MF_SEPARATOR            0x00000800L
#define MF_ENABLED              0x00000000L
#define MF_GRAYED               0x00000001L
#define MF_DISABLED             0x00000002L
#define MF_UNCHECKED            0x00000000L
#define MF_CHECKED              0x00000008L
#define MF_POPUP                0x00000010L
#define MF_STRING               0x00000000L
#define TPM_LEFTALIGN           0x0000L
#define TPM_CENTERALIGN         0x0004L
#define TPM_RIGHTALIGN          0x0008L
#define TPM_LEFTBUTTON          0x0000L
#define TPM_RIGHTBUTTON         0x0002L

// GlobalAlloc / Clipboard Flags
#define GMEM_FIXED              0x0000
#define GMEM_MOVEABLE           0x0002
#define GMEM_ZEROINIT           0x0040
#define GPTR                    (GMEM_FIXED | GMEM_ZEROINIT)
#define GHND                    (GMEM_MOVEABLE | GMEM_ZEROINIT)
#define GMEM_DDESHARE           0x2000
#define GMEM_SHARE              0x2000

#define CF_TEXT                 1
#define CF_BITMAP               2
#define CF_METAFILEPICT         3
#define CF_DIB                  8
#define CF_PALETTE              9
#define CF_UNICODETEXT          13
#define CF_ENHMETAFILE          14

// OpenFileName Flags
#define OFN_READONLY            0x00000001
#define OFN_OVERWRITEPROMPT     0x00000002
#define OFN_HIDEREADONLY        0x00000004
#define OFN_PATHMUSTEXIST       0x00000800
#define OFN_FILEMUSTEXIST       0x00001000

// WindowLong offsets
#define GWL_STYLE               (-16)
#define GWL_EXSTYLE             (-20)
#define SWP_NOSIZE              0x0001
#define SWP_NOMOVE              0x0002
#define SWP_NOZORDER            0x0004
#define SWP_NOREDRAW            0x0008
#define SWP_NOACTIVATE          0x0010
#define SWP_FRAMECHANGED        0x0020
#define SWP_SHOWWINDOW          0x0040
#define SWP_HIDEWINDOW          0x0080

// Standard MFC Command IDs
#define ID_SEPARATOR                    0
#define ID_DEFAULT_HELP                 0xE147
#define ID_FILE_NEW                     0xE100
#define ID_FILE_OPEN                    0xE101
#define ID_FILE_CLOSE                   0xE102
#define ID_FILE_SAVE                    0xE103
#define ID_FILE_SAVE_AS                 0xE104
#define ID_FILE_PAGE_SETUP              0xE105
#define ID_FILE_PRINT_SETUP             0xE106
#define ID_FILE_PRINT                   0xE107
#define ID_FILE_PRINT_DIRECT            0xE108
#define ID_FILE_PRINT_PREVIEW           0xE109
#define ID_FILE_UPDATE                  0xE10A
#define ID_FILE_SAVE_COPY_AS            0xE10B
#define ID_FILE_MRU_FILE1               0xE110
#define ID_EDIT_CLEAR                   0xE120
#define ID_EDIT_CLEAR_ALL               0xE121
#define ID_EDIT_COPY                    0xE122
#define ID_EDIT_CUT                     0xE123
#define ID_EDIT_FIND                    0xE124
#define ID_EDIT_PASTE                   0xE125
#define ID_EDIT_REPEAT                  0xE128
#define ID_EDIT_REPLACE                 0xE129
#define ID_EDIT_SELECT_ALL              0xE12A
#define ID_EDIT_UNDO                    0xE12B
#define ID_EDIT_REDO                    0xE12C
#define ID_VIEW_TOOLBAR                 0xE800
#define ID_VIEW_STATUS_BAR              0xE801
#define ID_APP_ABOUT                    0xE140
#define ID_APP_EXIT                     0xE141
#define ID_HELP_INDEX                   0xE142
#define ID_HELP_FINDER                  0xE143
#define ID_HELP_USING                   0xE144
#define ID_CONTEXT_HELP                 0xE145
#define ID_HELP                         0xE146

#define AFX_IDS_APP_TITLE               0xE000
#define AFX_IDS_IDLEMESSAGE             0xE001
#define AFX_IDS_UNTITLED                0xF003
#define AFX_IDS_SAVEFILE                0xF004
#define AFX_IDS_SAVEFILECOPY            0xF005
#define AFX_IDS_PREVIEWPAGEDESC         0xF006
#define AFX_IDW_TOOLBAR                 0xE800
#define AFX_IDW_STATUS_BAR              0xE801
#define AFX_IDW_DOCKBAR_TOP             0xE81B
#define AFX_IDW_DOCKBAR_LEFT            0xE81C
#define AFX_IDW_DOCKBAR_RIGHT           0xE81D
#define AFX_IDW_DOCKBAR_BOTTOM          0xE81E
#define AFX_IDW_DOCKBAR_FLOAT           0xE81F

// Control Bar Styles
#define CBRS_NOALIGN                    0x0000L
#define CBRS_LEFT                       0x1000L
#define CBRS_TOP                        0x2000L
#define CBRS_RIGHT                      0x4000L
#define CBRS_BOTTOM                     0x8000L
#define CBRS_ALIGN_LEFT                 CBRS_LEFT
#define CBRS_ALIGN_TOP                  CBRS_TOP
#define CBRS_ALIGN_RIGHT                CBRS_RIGHT
#define CBRS_ALIGN_BOTTOM               CBRS_BOTTOM
#define CBRS_ALIGN_ANY                  0xF000L
#define CBRS_BORDER_LEFT                0x0100L
#define CBRS_BORDER_TOP                 0x0200L
#define CBRS_BORDER_RIGHT               0x0400L
#define CBRS_BORDER_BOTTOM              0x0800L
#define CBRS_BORDER_ANY                 0x0F00L
#define CBRS_TOOLTIPS                   0x0010L
#define CBRS_FLYBY                      0x0020L
#define CBRS_FLOAT_MULTI                0x0040L
#define CBRS_SIZE_DYNAMIC               0x0004L
#define CBRS_SIZE_FIXED                 0x0008L
#define CBRS_FLOATING                   0x0001L

// Status Bar Pane Styles
#define SBPS_NORMAL                     0x0000
#define SBPS_NOBORDERS                  0x0100
#define SBPS_POPOUT                     0x0200
#define SBPS_DISABLED                   0x0400
#define SBPS_STRETCH                    0x0800
#define SBPS_OWNERDRAW                  0x1000

// Font Constants
#define LF_FACESIZE                     32
#define LF_FULLFACESIZE                 64
#define FW_NORMAL                       400
#define FW_BOLD                         700
#define ANSI_CHARSET                    0
#define DEFAULT_CHARSET                 1
#define SYMBOL_CHARSET                  2
#define SHIFTJIS_CHARSET                128
#define HANGEUL_CHARSET                 129
#define GB2312_CHARSET                  134
#define CHINESEBIG5_CHARSET             136
#define OEM_CHARSET                     255
#define MAC_CHARSET                     77
#define OUT_DEFAULT_PRECIS              0
#define CLIP_DEFAULT_PRECIS             0
#define DEFAULT_QUALITY                 0
#define DEFAULT_PITCH                   0

#define NTM_PS_OPENTYPE                 0x00020000
#define NTM_TYPE1                       0x00080000
#define NTM_TT_OPENTYPE                 0x00040000

#define FF_DONTCARE                     (0<<4)
#define ODT_COMBOBOX                    3

// ============================================================================
// GDI & Win32 Structs
// ============================================================================

#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
};

struct BITMAPCOREHEADER {
    DWORD   bcSize;
    WORD    bcWidth;
    WORD    bcHeight;
    WORD    bcPlanes;
    WORD    bcBitCount;
};

struct BITMAPINFOHEADER {
    DWORD   biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
};

struct RGBQUAD {
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
};
typedef RGBQUAD* LPRGBQUAD;
typedef WORD* PWORD;

struct RGBTRIPLE {
    BYTE    rgbtBlue;
    BYTE    rgbtGreen;
    BYTE    rgbtRed;
};

struct BITMAPCOREINFO {
    BITMAPCOREHEADER bmciHeader;
    RGBTRIPLE        bmciColors[1];
};
typedef BITMAPCOREINFO* LPBITMAPCOREINFO;

struct BITMAPV4HEADER {
    DWORD        bV4Size;
    LONG         bV4Width;
    LONG         bV4Height;
    WORD         bV4Planes;
    WORD         bV4BitCount;
    DWORD        bV4V4Compression;
    DWORD        bV4SizeImage;
    LONG         bV4XPelsPerMeter;
    LONG         bV4YPelsPerMeter;
    DWORD        bV4ClrUsed;
    DWORD        bV4ClrImportant;
    DWORD        bV4RedMask;
    DWORD        bV4GreenMask;
    DWORD        bV4BlueMask;
    DWORD        bV4AlphaMask;
    DWORD        bV4CSType;
    DWORD        bV4Endpoints[9];
    DWORD        bV4GammaRed;
    DWORD        bV4GammaGreen;
    DWORD        bV4GammaBlue;
};

struct BITMAPV5HEADER {
    DWORD        bV5Size;
    LONG         bV5Width;
    LONG         bV5Height;
    WORD         bV5Planes;
    WORD         bV5BitCount;
    DWORD        bV5Compression;
    DWORD        bV5SizeImage;
    LONG         bV5XPelsPerMeter;
    LONG         bV5YPelsPerMeter;
    DWORD        bV5ClrUsed;
    DWORD        bV5ClrImportant;
    DWORD        bV5RedMask;
    DWORD        bV5GreenMask;
    DWORD        bV5BlueMask;
    DWORD        bV5AlphaMask;
    DWORD        bV5CSType;
    DWORD        bV5Endpoints[9];
    DWORD        bV5GammaRed;
    DWORD        bV5GammaGreen;
    DWORD        bV5GammaBlue;
    DWORD        bV5Intent;
    DWORD        bV5ProfileData;
    DWORD        bV5ProfileSize;
    DWORD        bV5Reserved;
};
typedef BITMAPV5HEADER* PBITMAPV5HEADER;
typedef BITMAPV5HEADER* LPBITMAPV5HEADER;

struct BITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
};
typedef BITMAPINFO* LPBITMAPINFO;
typedef BITMAPINFOHEADER* LPBITMAPINFOHEADER;
typedef BITMAPINFOHEADER* PBITMAPINFOHEADER;
typedef BITMAPCOREHEADER* LPBITMAPCOREHEADER;
typedef BITMAPCOREHEADER* PBITMAPCOREHEADER;
#pragma pack(pop)

struct BITMAP {
    LONG    bmType;
    LONG    bmWidth;
    LONG    bmHeight;
    LONG    bmWidthBytes;
    WORD    bmPlanes;
    WORD    bmBitsPixel;
    LPVOID  bmBits;
};
typedef BITMAP* LPBITMAP;

struct DIBSECTION {
    BITMAP              dsBm;
    BITMAPINFOHEADER    dsBmih;
    DWORD               dsBitfields[3];
    HANDLE              dshSection;
    DWORD               dsOffset;
};

struct PALETTEENTRY {
    BYTE    peRed;
    BYTE    peGreen;
    BYTE    peBlue;
    BYTE    peFlags;
};
typedef PALETTEENTRY* LPPALETTEENTRY;

struct LOGPALETTE {
    WORD         palVersion;
    WORD         palNumEntries;
    PALETTEENTRY palPalEntry[1];
};
typedef LOGPALETTE* LPLOGPALETTE;

struct LOGPEN {
    UINT        lopnStyle;
    POINT       lopnWidth;
    COLORREF    lopnColor;
};
typedef LOGPEN* LPLOGPEN;

struct LOGBRUSH {
    UINT        lbStyle;
    COLORREF    lbColor;
    ULONG_PTR   lbHatch;
};

struct LOGFONT {
    LONG    lfHeight;
    LONG    lfWidth;
    LONG    lfEscapement;
    LONG    lfOrientation;
    LONG    lfWeight;
    BYTE    lfItalic;
    BYTE    lfUnderline;
    BYTE    lfStrikeOut;
    BYTE    lfCharSet;
    BYTE    lfOutPrecision;
    BYTE    lfClipPrecision;
    BYTE    lfQuality;
    BYTE    lfPitchAndFamily;
    TCHAR   lfFaceName[LF_FACESIZE];
};
typedef LOGFONT* LPLOGFONT;

#define SPI_GETNONCLIENTMETRICS 0x0029
struct NONCLIENTMETRICS {
    UINT    cbSize;
    int     iBorderWidth;
    int     iScrollWidth;
    int     iScrollHeight;
    int     iCaptionWidth;
    int     iCaptionHeight;
    LOGFONT lfCaptionFont;
    int     iSmCaptionWidth;
    int     iSmCaptionHeight;
    LOGFONT lfSmCaptionFont;
    int     iMenuWidth;
    int     iMenuHeight;
    LOGFONT lfMenuFont;
    LOGFONT lfStatusFont;
    LOGFONT lfMessageFont;
};
typedef NONCLIENTMETRICS* LPNONCLIENTMETRICS;

struct TEXTMETRIC {
    LONG    tmHeight;
    LONG    tmAscent;
    LONG    tmDescent;
    LONG    tmInternalLeading;
    LONG    tmExternalLeading;
    LONG    tmAveCharWidth;
    LONG    tmMaxCharWidth;
    LONG    tmWeight;
    LONG    tmOverhang;
    LONG    tmDigitizedAspectX;
    LONG    tmDigitizedAspectY;
    TCHAR   tmFirstChar;
    TCHAR   tmLastChar;
    TCHAR   tmDefaultChar;
    TCHAR   tmBreakChar;
    BYTE    tmItalic;
    BYTE    tmUnderlined;
    BYTE    tmStruckOut;
    BYTE    tmPitchAndFamily;
    BYTE    tmCharSet;
};
typedef TEXTMETRIC* LPTEXTMETRIC;

struct ENUMLOGFONT {
    LOGFONT elfLogFont;
    TCHAR   elfFullName[LF_FULLFACESIZE];
    TCHAR   elfStyle[LF_FACESIZE];
};
typedef ENUMLOGFONT* LPENUMLOGFONT;

struct ENUMLOGFONTEX {
    LOGFONT elfLogFont;
    TCHAR   elfFullName[LF_FULLFACESIZE];
    TCHAR   elfStyle[LF_FACESIZE];
    TCHAR   elfScript[LF_FACESIZE];
};

struct NEWTEXTMETRIC : public TEXTMETRIC {
    DWORD   ntmFlags;
    UINT    ntmSizeEM;
    UINT    ntmCellHeight;
    UINT    ntmAvgWidth;
};
typedef NEWTEXTMETRIC* LPNEWTEXTMETRIC;

struct NEWTEXTMETRICEX {
    NEWTEXTMETRIC ntmTm;
    DWORD         ntmFontSig[6];
};

struct PAINTSTRUCT {
    HDC     hdc;
    BOOL    fErase;
    RECT    rcPaint;
    BOOL    fRestore;
    BOOL    fIncUpdate;
    BYTE    rgbReserved[32];
};
typedef PAINTSTRUCT* LPPAINTSTRUCT;

struct CREATESTRUCT {
    LPVOID      lpCreateParams;
    HINSTANCE   hInstance;
    HMENU       hMenu;
    HWND        hwndParent;
    int         cy;
    int         cx;
    int         y;
    int         x;
    LONG        style;
    LPCTSTR     lpszName;
    LPCTSTR     lpszClass;
    DWORD       dwExStyle;
};
typedef CREATESTRUCT* LPCREATESTRUCT;

struct WINDOWPLACEMENT {
    UINT    length;
    UINT    flags;
    UINT    showCmd;
    POINT   ptMinPosition;
    POINT   ptMaxPosition;
    RECT    rcNormalPosition;
};

struct MINMAXINFO {
    POINT   ptReserved;
    POINT   ptMaxSize;
    POINT   ptMaxPosition;
    POINT   ptMinTrackSize;
    POINT   ptMaxTrackSize;
};

struct WINDOWPOS {
    HWND    hwnd;
    HWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
};

struct NCCALCSIZE_PARAMS {
    RECT        rgrc[3];
    WINDOWPOS*  lppos;
};

struct DRAWITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    UINT        itemID;
    UINT        itemAction;
    UINT        itemState;
    HWND        hwndItem;
    HDC         hDC;
    RECT        rcItem;
    ULONG_PTR   itemData;
};
typedef DRAWITEMSTRUCT* LPDRAWITEMSTRUCT;

#define ODA_DRAWENTIRE          0x0001
#define ODA_SELECT              0x0002
#define ODA_FOCUS               0x0004

#define ODS_SELECTED            0x0001
#define ODS_GRAYED              0x0002
#define ODS_DISABLED            0x0004
#define ODS_CHECKED             0x0008
#define ODS_FOCUS               0x0010
#define ODS_DEFAULT             0x0020
#define ODS_COMBOBOXEDIT        0x1000

struct FONTSIGNATURE {
    DWORD fsUsb[4];
    DWORD fsCsb[2];
};
struct CHARSETINFO {
    UINT ciCharset;
    UINT ciACP;
    FONTSIGNATURE fs;
};
#define TCI_SRCCHARSET          1
#define TCI_SRCCODEPAGE         2
#define TCI_SRCFONTSIG          3

#define HKEY_CLASSES_ROOT       ((HKEY)(ULONG_PTR)((LONG)0x80000000))
#define HKEY_CURRENT_USER       ((HKEY)(ULONG_PTR)((LONG)0x80000001))
#define HKEY_LOCAL_MACHINE      ((HKEY)(ULONG_PTR)((LONG)0x80000002))
#define HKEY_USERS              ((HKEY)(ULONG_PTR)((LONG)0x80000003))
#define ERROR_SUCCESS           0L

struct MEASUREITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    UINT        itemID;
    UINT        itemWidth;
    UINT        itemHeight;
    ULONG_PTR   itemData;
};
typedef MEASUREITEMSTRUCT* LPMEASUREITEMSTRUCT;

struct COMPAREITEMSTRUCT {
    UINT        CtlType;
    UINT        CtlID;
    HWND        hwndItem;
    UINT        itemID1;
    ULONG_PTR   itemData1;
    UINT        itemID2;
    ULONG_PTR   itemData2;
    DWORD       dwLocaleId;
};
typedef COMPAREITEMSTRUCT* LPCOMPAREITEMSTRUCT;

struct MSG {
    HWND    hwnd;
    UINT    message;
    WPARAM  wParam;
    LPARAM  lParam;
    DWORD   time;
    POINT   pt;
};
typedef MSG* LPMSG;

struct WNDCLASS {
    UINT        style;
    LPVOID      lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCTSTR     lpszMenuName;
    LPCTSTR     lpszClassName;
};

struct TOOLINFO {
    UINT        cbSize;
    UINT        uFlags;
    HWND        hwnd;
    UINT_PTR    uId;
    RECT        rect;
    HINSTANCE   hinst;
    LPTSTR      lpszText;
    LPARAM      lParam;
};
#define LPSTR_TEXTCALLBACK ((LPTSTR)-1L)

typedef UINT_PTR (CALLBACK *LPPAGEPAINTHOOK)(HWND, UINT, WPARAM, LPARAM);
typedef UINT_PTR (CALLBACK *LPPAGESETUPHOOK)(HWND, UINT, WPARAM, LPARAM);

struct PAGESETUPDLGA {
    DWORD           lStructSize;
    HWND            hwndOwner;
    HGLOBAL         hDevMode;
    HGLOBAL         hDevNames;
    DWORD           Flags;
    POINT           ptPaperSize;
    RECT            rtMinMargin;
    RECT            rtMargin;
    HINSTANCE       hInstance;
    LPARAM          lCustData;
    LPPAGESETUPHOOK lpfnPageSetupHook;
    LPPAGEPAINTHOOK lpfnPagePaintHook;
    LPCSTR          lpPageSetupTemplateName;
    HGLOBAL         hPageSetupTemplate;
};
typedef PAGESETUPDLGA* LPPAGESETUPDLGA;
typedef PAGESETUPDLGA  PAGESETUPDLG;
typedef PAGESETUPDLGA* LPPAGESETUPDLG;

struct PRINTDLGA {
    DWORD           lStructSize;
    HWND            hwndOwner;
    HGLOBAL         hDevMode;
    HGLOBAL         hDevNames;
    HDC             hDC;
    DWORD           Flags;
    WORD            nFromPage;
    WORD            nToPage;
    WORD            nMinPage;
    WORD            nMaxPage;
    WORD            nCopies;
    HINSTANCE       hInstance;
    LPARAM          lCustData;
    LPVOID          lpfnPrintHook;
    LPVOID          lpfnSetupHook;
    LPCTSTR         lpPrintTemplateName;
    LPCTSTR         lpSetupTemplateName;
    HGLOBAL         hPrintTemplate;
    HGLOBAL         hSetupTemplate;
};
typedef PRINTDLGA PRINTDLG;
typedef PRINTDLGA* LPPRINTDLG;

struct AFX_SIZEPARENTPARAMS {
    HDWP    hDWP;
    RECT    rect;
    SIZE    sizeTotal;
    BOOL    bStretch;
};

#define LOCALE_USER_DEFAULT     0x0400
#define LOCALE_SDECIMAL         0x0000000C
#define HID_BASE_RESOURCE       0x00020000UL

typedef DWORD LCID;
typedef DWORD REGSAM;

#define REG_NONE                0
#define REG_SZ                  1
#define REG_EXPAND_SZ           2
#define REG_BINARY              3
#define REG_DWORD               4
#define REG_DWORD_LITTLE_ENDIAN 4
#define REG_DWORD_BIG_ENDIAN    5
#define REG_LINK                6
#define REG_MULTI_SZ            7

#define WH_CALLWNDPROC          4

struct HELPINFO {
    UINT cbSize;
    int iContextType;
    int iCtrlId;
    HANDLE hItemHandle;
    DWORD_PTR dwContextId;
    POINT MousePos;
};
typedef HELPINFO* LPHELPINFO;
#define HELP_WM_HELP            0x000c
#define HELP_SETPOPUP_POS       0x000d

typedef int (CALLBACK* FONTENUMPROC)(const LOGFONT*, const TEXTMETRIC*, DWORD, LPARAM);

#include "mfc_classes.h"
