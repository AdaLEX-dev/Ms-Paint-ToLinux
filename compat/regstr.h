#pragma once
#include "afxwin.h"

#define REGSTR_PATH_POLICIES "Software\\Microsoft\\Windows\\CurrentVersion\\Policies"

#define KEY_QUERY_VALUE         0x0001
#define KEY_SET_VALUE           0x0002
#define KEY_CREATE_SUB_KEY      0x0004
#define KEY_ENUMERATE_SUB_KEYS  0x0008
#define KEY_NOTIFY              0x0010
#define KEY_CREATE_LINK         0x0020
#define KEY_READ                0x20019
#define KEY_WRITE               0x20006
#define KEY_EXECUTE             0x20019
#define KEY_ALL_ACCESS          0xF003F

#ifndef _WINREG_FUNCS_
#define _WINREG_FUNCS_
inline LONG RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult) { return -1; }
inline LONG RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) { return -1; }
inline LONG RegCloseKey(HKEY hKey) { return 0; }
#endif
