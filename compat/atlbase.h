#pragma once
#include "afxwin.h"
#include "objbase.h"

typedef DWORD REGSAM;
typedef void* LPSECURITY_ATTRIBUTES;

class CComBSTR {
public:
    BSTR m_str = nullptr;
    CComBSTR() {}
    CComBSTR(LPCWSTR pSrc) {}
    CComBSTR(LPCSTR pSrc) {}
    CComBSTR(int nSize, LPCWSTR sz) {}
    ~CComBSTR() {}
    operator BSTR() const { return m_str; }
    BSTR* operator&() { return &m_str; }
    unsigned int Length() const { return 0; }
    void Empty() {}
};

template<typename T>
class CComPtr {
    T* p = nullptr;
public:
    CComPtr() : p(nullptr) {}
    CComPtr(T* lp) : p(lp) { if (p) p->AddRef(); }
    CComPtr(const CComPtr<T>& lp) : p(lp.p) { if (p) p->AddRef(); }
    ~CComPtr() { if (p) p->Release(); }
    operator T*() const { return p; }
    T& operator*() const { return *p; }
    T** operator&() { return &p; }
    T* operator->() const { return p; }
    T* operator=(T* lp) {
        if (p) p->Release();
        p = lp;
        if (p) p->AddRef();
        return p;
    }
    T* operator=(const CComPtr<T>& lp) {
        return operator=(lp.p);
    }
    bool operator!() const { return p == nullptr; }
    void Release() { if (p) { p->Release(); p = nullptr; } }
    void Attach(T* p2) { if (p) p->Release(); p = p2; }
    T* Detach() { T* pt = p; p = nullptr; return pt; }
    HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = nullptr, DWORD dwClsContext = 1) { return -1; }
};

template<typename T>
class CComQIPtr {
    T* p = nullptr;
public:
    CComQIPtr() : p(nullptr) {}
    CComQIPtr(IUnknown* lp) : p(nullptr) {}
    ~CComQIPtr() { if (p) p->Release(); }
    operator T*() const { return p; }
    T* operator->() const { return p; }
    bool operator==(int n) const { return p == nullptr; }
    bool operator!=(int n) const { return p != nullptr; }
};

class CRegKey {
public:
    HKEY m_hKey = nullptr;
    CRegKey() {}
    ~CRegKey() { Close(); }
    LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired = 0) { return 0; }
    LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName, LPTSTR lpszClass = (LPTSTR)"", DWORD dwOptions = 0, REGSAM samDesired = 0, LPSECURITY_ATTRIBUTES lpSecAttr = nullptr, LPDWORD lpdwDisp = nullptr) { return 0; }
    LONG Close() { m_hKey = nullptr; return 0; }
    LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName) { return -1; }
    LONG QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount) { return -1; }
    LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName) { return 0; }
    LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = nullptr) { return 0; }
    LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = nullptr) { return 0; }
    operator HKEY() const { return m_hKey; }
};

#define USES_CONVERSION
class _A2W_Helper {
    std::wstring m_ws;
public:
    _A2W_Helper(const char* s) {
        if (s) {
            while (*s) m_ws += (wchar_t)(unsigned char)*s++;
        }
    }
    _A2W_Helper(const wchar_t* ws) {
        if (ws) m_ws = ws;
    }
    operator LPCWSTR() const { return m_ws.c_str(); }
};

#define T2CW(s) (_A2W_Helper(s))
#define T2W(s)  ((LPWSTR)(LPCWSTR)_A2W_Helper(s))
#define T2CA(s) ((LPCSTR)(s))
#define T2A(s)  ((LPSTR)(s))
#define W2T(s)  ((LPCTSTR)(s))
#define W2CT(s) ((LPCTSTR)(s))
#define A2W(s)  ((LPWSTR)(LPCWSTR)_A2W_Helper(s))
#define W2A(s)  ((LPCSTR)(s))
#define A2CW(s) (_A2W_Helper(s))
#define W2CA(s) ((LPCSTR)(s))
#define T2COLE(s) ((LPOLESTR)(LPCTSTR)(s))
#define T2OLE(s)  ((LPOLESTR)(LPCTSTR)(s))
#define OLE2T(s)  ((LPCTSTR)(s))
#define OLE2CT(s) ((LPCTSTR)(s))

inline HRESULT CLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid) { return E_FAIL; }
