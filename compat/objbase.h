#pragma once
#include "afxwin.h"

#ifndef STDMETHOD
#define STDMETHOD(method)       virtual HRESULT method
#define STDMETHOD_(type,method) virtual type method
#define PURE                    = 0
#define THIS_
#define THIS                    void
#endif

#define STDMETHODIMP            HRESULT
#define STDMETHODIMP_(type)     type
#ifndef E_NOINTERFACE
#define E_NOINTERFACE           ((HRESULT)0x80004002L)
#endif
#define STREAM_SEEK_SET         0
#define STREAM_SEEK_CUR         1
#define STREAM_SEEK_END         2
#define CLSCTX_INPROC_SERVER    1

typedef GUID IID;
typedef const IID& REFIID;
typedef GUID CLSID;
typedef const CLSID& REFCLSID;
typedef wchar_t* LPOLESTR;
typedef wchar_t* BSTR;
typedef unsigned short VARTYPE;
#define PRSPEC_PROPID 1
#define PRSPEC_LPWSTR 0

struct PROPSPEC {
    ULONG ulKind;
    union {
        PROPID propid;
        LPOLESTR lpwstr;
    };
};

#define VT_EMPTY    0
#define VT_NULL     1
#define VT_I2       2
#define VT_I4       3
#define VT_R4       4
#define VT_R8       5
#define VT_CY       6
#define VT_DATE     7
#define VT_BSTR     8
#define VT_DISPATCH 9
#define VT_ERROR    10
#define VT_BOOL     11
#define VT_VARIANT  12
#define VT_UNKNOWN  13
#define VT_DECIMAL  14
#define VT_I1       16
#define VT_UI1      17
#define VT_UI2      18
#define VT_UI4      19
#define VT_I8       20
#define VT_UI8      21
#define VT_INT      22
#define VT_UINT     23
#define VT_CLSID    72

struct PROPVARIANT {
    VARTYPE vt;
    WORD wReserved1;
    WORD wReserved2;
    WORD wReserved3;
    union {
        CHAR cVal;
        BYTE bVal;
        short iVal;
        unsigned short uiVal;
        LONG lVal;
        ULONG ulVal;
        INT intVal;
        UINT uintVal;
        float fltVal;
        double dblVal;
        BSTR bstrVal;
        GUID* puuid;
    };
};

inline HRESULT PropVariantClear(PROPVARIANT *pvar) { return S_OK; }
inline void PropVariantInit(PROPVARIANT *pvar) { if (pvar) memset(pvar, 0, sizeof(*pvar)); }

#ifndef _FILETIME_
#define _FILETIME_
struct FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};
#endif

union LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    int64_t QuadPart;
};

union ULARGE_INTEGER {
    struct {
        DWORD LowPart;
        DWORD HighPart;
    };
    uint64_t QuadPart;
};

struct STATSTG {
    LPOLESTR pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
};

struct IUnknown {
    virtual HRESULT QueryInterface(REFIID riid, void **ppvObject) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};
typedef IUnknown* LPUNKNOWN;

struct FORMATETC;
struct STGMEDIUM;

struct IDataObject : public IUnknown {
    virtual HRESULT GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium) = 0;
    virtual HRESULT SetData(FORMATETC *pformatetc, STGMEDIUM *pmedium, BOOL fRelease) = 0;
    virtual HRESULT QueryGetData(FORMATETC *pformatetc) = 0;
};

struct ISequentialStream : public IUnknown {
    virtual HRESULT Read(void *pv, ULONG cb, ULONG *pcbRead) = 0;
    virtual HRESULT Write(const void *pv, ULONG cb, ULONG *pcbWritten) = 0;
};

struct IStream : public ISequentialStream {
    virtual HRESULT Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) = 0;
    virtual HRESULT SetSize(ULARGE_INTEGER libNewSize) = 0;
    virtual HRESULT CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) = 0;
    virtual HRESULT Commit(DWORD grfCommitFlags) = 0;
    virtual HRESULT Revert() = 0;
    virtual HRESULT LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) = 0;
    virtual HRESULT UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) = 0;
    virtual HRESULT Stat(STATSTG *pstatstg, DWORD grfStatFlag) = 0;
    virtual HRESULT Clone(IStream **ppstm) = 0;
};

struct IStorage : public IUnknown {
    virtual HRESULT CreateStream(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream** ppstm) = 0;
    virtual HRESULT OpenStream(const WCHAR* pwcsName, void* reserved1, DWORD grfMode, DWORD reserved2, IStream** ppstm) = 0;
    virtual HRESULT CreateStorage(const WCHAR* pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage** ppstg) = 0;
    virtual HRESULT OpenStorage(const WCHAR* pwcsName, IStorage* pstgPriority, DWORD grfMode, void* snbExclude, DWORD reserved, IStorage** ppstg) = 0;
    virtual HRESULT CopyTo(DWORD ciidExclude, const IID* rgiidExclude, void* snbExclude, IStorage* pstgDest) = 0;
    virtual HRESULT MoveElementTo(const WCHAR* pwcsName, IStorage* pstgDest, const WCHAR* pwcsNewName, DWORD grfFlags) = 0;
    virtual HRESULT Commit(DWORD grfCommitFlags) = 0;
    virtual HRESULT Revert() = 0;
    virtual HRESULT EnumElements(DWORD reserved1, void* reserved2, DWORD reserved3, void** ppenum) = 0;
    virtual HRESULT DestroyElement(const WCHAR* pwcsName) = 0;
    virtual HRESULT RenameElement(const WCHAR* pwcsOldName, const WCHAR* pwcsNewName) = 0;
    virtual HRESULT SetElementTimes(const WCHAR* pwcsName, const FILETIME* pctime, const FILETIME* patime, const FILETIME* pmtime) = 0;
    virtual HRESULT SetClass(REFCLSID clsid) = 0;
    virtual HRESULT SetStateBits(DWORD grfStateBits, DWORD grfMask) = 0;
    virtual HRESULT Stat(STATSTG* pstatstg, DWORD grfStatFlag) = 0;
};
typedef IStorage* LPSTORAGE;

#define STGC_DEFAULT 0
#define STGC_OVERWRITE 1
#define STGC_ONLYIFCURRENT 2
#define STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE 4
#define STGC_CONSOLIDATE 8

inline const IID IID_IUnknown = {0,0,0,{0xC0,0,0,0,0,0,0,0x46}};
inline const IID IID_IStream = {0x0000000c,0,0,{0xC0,0,0,0,0,0,0,0x46}};
inline const IID IID_IPersistStorage = {0x0000010a,0,0,{0xC0,0,0,0,0,0,0,0x46}};
inline const GUID IID_IWiaDataTransfer = {0xa6cef998,0xa5b0,0x11d2,{0xa0,0x87,0x00,0xa0,0xc9,0x06,0x29,0xa8}};
inline const GUID IID_IWiaDataCallback = {0xa558a866,0xa5b0,0x11d2,{0xa0,0x87,0x00,0xa0,0xc9,0x06,0x29,0xa8}};

inline HRESULT CoInitialize(LPVOID pvReserved) { return 0; }
inline void CoUninitialize() {}
inline HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID *ppv) { return -1; }
inline HRESULT WriteClassStg(IStorage* pStg, const GUID& rclsid) { return S_OK; }
inline HRESULT WriteFmtUserTypeStg(IStorage* pStg, WORD cf, LPCWSTR lpszUserType) { return S_OK; }
inline HRESULT CoMarshalInterThreadInterfaceInStream(REFIID riid, IUnknown* pUnk, IStream** ppStm) { return S_OK; }
inline HRESULT CoGetInterfaceAndReleaseStream(IStream* pStm, REFIID riid, void** ppv) { return S_OK; }
inline void CoTaskMemFree(LPVOID pv) { if (pv) free(pv); }
inline LPVOID CoTaskMemAlloc(size_t cb) { return malloc(cb); }
