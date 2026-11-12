#pragma once
#include "../afxwin.h"

namespace Gdiplus {
    typedef void (*DebugEventProc)(int level, char *message);
    enum Status {
        Ok = 0,
        GenericError = 1,
        InvalidParameter = 2,
        OutOfMemory = 3,
        ObjectBusy = 4,
        InsufficientBuffer = 5,
        PropertyNotFound = 6,
        PropertyNotSupported = 7,
        Undefined = 8,
        GdiplusNotInitialized = 19
    };

    struct GdiplusStartupInput {
        UINT GdiplusVersion;
        DebugEventProc DebugEventCallback;
        BOOL SuppressBackgroundThread;
        BOOL SuppressExternalCodecs;

        GdiplusStartupInput(
            DebugEventProc debugEventCallback = 0,
            BOOL suppressBackgroundThread = FALSE,
            BOOL suppressExternalCodecs = FALSE)
        {
            GdiplusVersion = 1;
            DebugEventCallback = debugEventCallback;
            SuppressBackgroundThread = suppressBackgroundThread;
            SuppressExternalCodecs = suppressExternalCodecs;
        }
    };

    struct GdiplusStartupOutput {
        void* NotificationHook;
        void* NotificationUnhook;
    };

    struct ImageCodecInfo {
        CLSID Clsid;
        GUID FormatID;
        const WCHAR* CodecName;
        const WCHAR* DllName;
        const WCHAR* FormatDescription;
        const WCHAR* FilenameExtension;
        const WCHAR* MimeType;
        DWORD Flags;
        DWORD Version;
        DWORD SigCount;
        DWORD SigSize;
        const BYTE* SigPattern;
        const BYTE* SigMask;
    };

    class Bitmap {
    public:
        Bitmap(struct IStream* pStream) {}
        Bitmap(const WCHAR* filename, BOOL useEmbeddedColorManagement = FALSE) {}
        Status GetLastStatus() { return GenericError; }
        Status GetRawFormat(GUID* format) { return GenericError; }
        Status Save(const WCHAR* filename, const CLSID* clsidEncoder, void* encoderParams) { return GenericError; }
        Status Save(struct IStream* stream, const CLSID* clsidEncoder, void* encoderParams) { return GenericError; }
    };

    inline Status GdiplusStartup(ULONG_PTR *token, const GdiplusStartupInput *input, GdiplusStartupOutput *output) { return Ok; }
    inline void GdiplusShutdown(ULONG_PTR token) {}
    inline Status GetImageDecodersSize(UINT *numDecoders, UINT *size) { if (numDecoders) *numDecoders = 0; if (size) *size = 0; return Ok; }
    inline Status GetImageDecoders(UINT numDecoders, UINT size, ImageCodecInfo *decoders) { return Ok; }
    inline Status GetImageEncodersSize(UINT *numEncoders, UINT *size) { if (numEncoders) *numEncoders = 0; if (size) *size = 0; return Ok; }
    inline Status GetImageEncoders(UINT numEncoders, UINT size, ImageCodecInfo *encoders) { return Ok; }
}
