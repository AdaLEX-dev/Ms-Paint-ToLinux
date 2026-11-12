#pragma once
#include "afxwin.h"
#include "objbase.h"

typedef wchar_t* BSTR;
typedef const GUID* LPCGUID;

struct IWiaEventCallback : public IUnknown {
    virtual HRESULT ImageEventCallback(
        LPCGUID pEventGuid,
        BSTR    bstrEventDescription,
        BSTR    bstrDeviceID,
        BSTR    bstrDeviceDescription,
        DWORD   dwDeviceType,
        BSTR    bstrFullItemName,
        ULONG  *pulEventType,
        ULONG   ulReserved
    ) = 0;
};

struct IWiaProgressDialog : public IUnknown {
    virtual HRESULT Create(HWND hwndParent, LONG lFlags) = 0;
    virtual HRESULT Show() = 0;
    virtual HRESULT Hide() = 0;
    virtual HRESULT Cancelled(BOOL *pbCancelled) = 0;
    virtual HRESULT SetTitle(LPCWSTR pszTitle) = 0;
    virtual HRESULT SetMessage(LPCWSTR pszMessage) = 0;
    virtual HRESULT SetPercentComplete(UINT uPercent) = 0;
    virtual HRESULT Destroy() = 0;
};

#define WIA_DIP_DEV_TYPE 4
#define GET_STIDEVICE_TYPE(dw) ((dw) & 0xFF)
#define StiDeviceTypeDefault 0
#define StiDeviceTypeScanner 1
#define StiDeviceTypeDigitalCamera 2
#define StiDeviceTypeStreamingVideo 3

#define WIA_PROGRESSDLG_NO_ANIM 0
#define WIA_PROGRESSDLG_ANIM_SCANNER_ACQUIRE 1
#define WIA_PROGRESSDLG_ANIM_CAMERA_ACQUIRE 2
#define WIA_PROGRESSDLG_ANIM_VIDEO_ACQUIRE 3

extern const GUID IID_IWiaDataTransfer;
extern const GUID IID_IWiaDataCallback;

struct IWiaDataCallback : public IUnknown {
    virtual HRESULT BandedDataCallback(
        LONG  lReason,
        LONG  lStatus,
        LONG  lPercentComplete,
        LONG  lOffset,
        LONG  lLength,
        LONG  lReserved,
        LONG  lResLength,
        PBYTE pbBuffer
    ) = 0;
};

struct STGMEDIUM;
typedef STGMEDIUM* LPSTGMEDIUM;

struct WIA_DATA_TRANSFER_INFO {
    ULONG ulSize;
    ULONG ulSection;
    ULONG ulBufferSize;
    BOOL  bDoubleBuffer;
    ULONG ulReserved1;
    ULONG ulReserved2;
    ULONG ulReserved3;
};
typedef WIA_DATA_TRANSFER_INFO* PWIA_DATA_TRANSFER_INFO;

struct IWiaDataTransfer : public IUnknown {
    virtual HRESULT idtGetData(LPSTGMEDIUM pMedium, IWiaDataCallback *pIWiaDataCallback) = 0;
    virtual HRESULT idtGetBandedData(PWIA_DATA_TRANSFER_INFO pWiaDataTransInfo, IWiaDataCallback *pIWiaDataCallback) = 0;
};

#define IT_MSG_DATA_HEADER 1
#define IT_MSG_DATA 2
#define IT_MSG_STATUS 3
#define IT_MSG_TERMINATION 4
#define IT_MSG_NEW_PAGE 5
#define IT_MSG_FILE_PREVIEW_DATA 6
#define IT_MSG_FILE_PREVIEW_DATA_HEADER 7

#define IT_STATUS_TRANSFER_FROM_DEVICE 1
#define IT_STATUS_PROCESSING_DATA 2
#define IT_STATUS_TRANSFER_TO_CLIENT 3

struct WIA_DATA_CALLBACK_HEADER {
    LONG lSize;
    GUID guidFormatID;
    ULONG lBufferSize;
    ULONG lPageCount;
};
typedef WIA_DATA_CALLBACK_HEADER* PWIA_DATA_CALLBACK_HEADER;

struct IEnumWIA_DEV_INFO : public IUnknown {
    virtual HRESULT Next(ULONG celt, struct IWiaPropertyStorage **rgelt, ULONG *pceltFetched) = 0;
    virtual HRESULT Skip(ULONG celt) = 0;
    virtual HRESULT Reset() = 0;
    virtual HRESULT Clone(IEnumWIA_DEV_INFO **ppIEnum) = 0;
    virtual HRESULT GetCount(ULONG *celt) = 0;
};

#define WIA_SELECT_DEVICE_NODEFAULT     0x00000001
#define WIA_DEVICE_DIALOG_SINGLE_IMAGE  0x00000002
#define WIA_INTENT_NONE                 0x00000000
#define StiDeviceTypeDefault            0

#define TYMED_CALLBACK                  128
#define WIA_IPA_FIRST                   4096
#define WIA_IPA_TYMED                   (WIA_IPA_FIRST + 7)
#define WIA_IPA_FORMAT                  (WIA_IPA_FIRST + 6)
#define WIA_IPA_MIN_BUFFER_SIZE         (WIA_IPA_FIRST + 16)

struct IWiaItem : public IUnknown {
    virtual HRESULT DeviceDlg(HWND hwndParent, LONG lFlags, LONG lIntent, LONG *plItemCount, IWiaItem ***pppIWiaItem) = 0;
};
struct IWiaPropertyStorage : public IUnknown {
    virtual HRESULT ReadMultiple(ULONG cpspec, const PROPSPEC rgpspec[], PROPVARIANT rgpropvar[]) = 0;
    virtual HRESULT WriteMultiple(ULONG cpspec, const PROPSPEC rgpspec[], const PROPVARIANT rgpropvar[], PROPID propidNameFirst = 2) = 0;
};
struct IWiaDevMgr : public IUnknown {
    virtual HRESULT SelectDeviceDlg(HWND hwndParent, LONG lDeviceType, LONG lFlags, BSTR *pbstrDeviceID, IWiaItem **ppWiaItemRoot) = 0;
    virtual HRESULT SelectDeviceDlgID(HWND hwndParent, LONG lDeviceType, LONG lFlags, BSTR *pbstrDeviceID) = 0;
    virtual HRESULT CreateDevice(BSTR bstrDeviceID, IWiaItem **ppWiaItemRoot) = 0;
    virtual HRESULT EnumDeviceInfo(LONG lFlags, IEnumWIA_DEV_INFO **ppIEnum) = 0;
    virtual HRESULT RegisterEventCallbackInterface(LONG lFlags, BSTR bstrDeviceID, const GUID *pEventGUID, IWiaEventCallback *pIWiaEventCallback, IUnknown **pEventObject) = 0;
};
inline const CLSID CLSID_WiaDevMgr = {0xa1f4e726, 0x8cf1, 0x11d1, {0xbf, 0x92, 0x00, 0x10, 0x4b, 0x85, 0xb5, 0x96}};
inline const CLSID CLSID_WiaDefaultUi = {0x4db1ad10, 0x3391, 0x11d2, {0xba, 0xa3, 0x00, 0xc0, 0x4f, 0xa3, 0x1a, 0x86}};
inline const GUID WIA_EVENT_DEVICE_CONNECTED = {0xa28bbade, 0x64b6, 0x11d2, {0xa5, 0x21, 0x00, 0xc0, 0x4f, 0xb9, 0x2e, 0x3c}};
inline const GUID WIA_EVENT_DEVICE_DISCONNECTED = {0x143e4e83, 0x6497, 0x11d2, {0xa5, 0x20, 0x00, 0xc0, 0x4f, 0xb9, 0x2e, 0x3c}};
inline const IID IID_IWiaProgressDialog = {0xa9922e96, 0x01a6, 0x487a, {0x87, 0x82, 0x1e, 0x3d, 0x98, 0x6e, 0xb6, 0xeb}};
inline const GUID IID_IWiaEventCallback = {0xae62e697, 0x47c2, 0x11d2, {0xb5, 0x35, 0x00, 0xc0, 0x4f, 0x68, 0x34, 0xcf}};

extern const GUID WiaImgFmt_UNDEFINED;
extern const GUID WiaImgFmt_RAWRGB;
extern const GUID WiaImgFmt_MEMORYBMP;
extern const GUID WiaImgFmt_BMP;
extern const GUID WiaImgFmt_EMF;
extern const GUID WiaImgFmt_WMF;
extern const GUID WiaImgFmt_JPEG;
extern const GUID WiaImgFmt_PNG;
extern const GUID WiaImgFmt_GIF;
extern const GUID WiaImgFmt_TIFF;
extern const GUID WiaImgFmt_EXIF;
extern const GUID WiaImgFmt_PHOTOCD;
extern const GUID WiaImgFmt_FLASHPIX;
extern const GUID WiaImgFmt_ICO;
extern const GUID WiaImgFmt_CIFF;
extern const GUID WiaImgFmt_PICT;
extern const GUID WiaImgFmt_JPEG2K;
extern const GUID WiaImgFmt_JPEG2KX;
