#pragma once
#include "afxwin.h"

enum TOOLBARPARTS {
    TP_BUTTON = 1,
    TP_DROPDOWNBUTTON = 2,
    TP_SPLITBUTTON = 3,
    TP_SPLITBUTTONDROPDOWN = 4,
    TP_SEPARATOR = 5,
};

enum TOOLBARSTATES {
    TS_NORMAL = 1,
    TS_HOT = 2,
    TS_PRESSED = 3,
    TS_DISABLED = 4,
    TS_CHECKED = 5,
    TS_HOTCHECKED = 6,
};
