#pragma once

// ============================================================================
// MFC Debug & Exception Macros
// ============================================================================

#ifndef ASSERT
#define ASSERT(f)       ((void)0)
#endif
#ifndef VERIFY
#define VERIFY(f)       ((void)(f))
#endif
#ifndef ASSERT_VALID
#define ASSERT_VALID(p) ((void)0)
#endif
#ifndef TRACE
#define TRACE(...)      ((void)0)
#endif
#ifndef TRACE0
#define TRACE0(sz)      ((void)0)
#endif
#ifndef TRACE1
#define TRACE1(sz, p1)  ((void)0)
#endif
#ifndef TRACE2
#define TRACE2(sz, p1, p2) ((void)0)
#endif
#ifndef TRACE3
#define TRACE3(sz, p1, p2, p3) ((void)0)
#endif
#ifndef DEBUG_NEW
#define DEBUG_NEW new
#endif

#define TRY             try
#define CATCH(cls, e)   catch (cls* e)
#define AND_CATCH(cls, e) catch (cls* e)
#define END_CATCH       
#define CATCH_ALL(e)    catch (CException* e)
#define END_CATCH_ALL   
#define THROW_LAST()    throw

// ============================================================================
// CRuntimeClass & CObject
// ============================================================================

class CObject;

struct CRuntimeClass {
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;
    CObject* (*m_pfnCreateObject)();
    CRuntimeClass* m_pBaseClass;

    CObject* CreateObject() {
        return m_pfnCreateObject ? (*m_pfnCreateObject)() : nullptr;
    }
    BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const {
        const CRuntimeClass* pClassThis = this;
        while (pClassThis != nullptr) {
            if (pClassThis == pBaseClass) return TRUE;
            pClassThis = pClassThis->m_pBaseClass;
        }
        return FALSE;
    }
};

class CDumpContext {};

class CObject {
public:
    static CRuntimeClass classCObject;
    virtual CRuntimeClass* GetRuntimeClass() const { return &classCObject; }
    virtual ~CObject() {}
    BOOL IsKindOf(const CRuntimeClass* pClass) const {
        CRuntimeClass* pClassThis = GetRuntimeClass();
        return pClassThis ? pClassThis->IsDerivedFrom(pClass) : FALSE;
    }
    virtual void AssertValid() const {}
    virtual void Dump(CDumpContext&) const {}
    virtual void Serialize(class CArchive&) {}
};

#define RUNTIME_CLASS(class_name) (&class_name::class##class_name)

#define DECLARE_DYNAMIC(class_name) \
public: \
    static CRuntimeClass class##class_name; \
    virtual CRuntimeClass* GetRuntimeClass() const;

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, sizeof(class_name), 0xFFFF, nullptr, &base_class_name::class##base_class_name \
    }; \
    CRuntimeClass* class_name::GetRuntimeClass() const { return &class_name::class##class_name; }

#define DECLARE_DYNCREATE(class_name) \
    DECLARE_DYNAMIC(class_name) \
    static CObject* CreateObject();

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
    CObject* class_name::CreateObject() { return new class_name; } \
    CRuntimeClass class_name::class##class_name = { \
        #class_name, sizeof(class_name), 0xFFFF, class_name::CreateObject, &base_class_name::class##base_class_name \
    }; \
    CRuntimeClass* class_name::GetRuntimeClass() const { return &class_name::class##class_name; }

// ============================================================================
// CString
// ============================================================================

BOOL AfxLoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf = 256);

class CString {
    std::string m_str;
public:
    CString() {}
    CString(const CString& s) : m_str(s.m_str) {}
    CString(char ch, int nRepeat = 1) : m_str(nRepeat, ch) {}
    CString(LPCSTR lpsz) : m_str(lpsz ? lpsz : "") {}
    CString(LPCSTR lpch, int nLength) : m_str(lpch ? lpch : "", lpch ? nLength : 0) {}
    CString(const WCHAR* lpwsz) {
        if (lpwsz) {
            while (*lpwsz) {
                m_str += (char)*lpwsz++;
            }
        }
    }

    int GetLength() const { return (int)m_str.size(); }
    BOOL IsEmpty() const { return m_str.empty(); }
    void Empty() { m_str.clear(); }

    char GetAt(int nIndex) const { return (nIndex >= 0 && nIndex < (int)m_str.size()) ? m_str[nIndex] : 0; }
    char operator[](int nIndex) const { return GetAt(nIndex); }
    char& operator[](int nIndex) { return m_str[nIndex]; }
    void SetAt(int nIndex, char ch) { if (nIndex >= 0 && nIndex < (int)m_str.size()) m_str[nIndex] = ch; }

    operator LPCSTR() const { return m_str.c_str(); }

    const CString& operator=(const CString& s) { m_str = s.m_str; return *this; }
    const CString& operator=(char ch) { m_str.assign(1, ch); return *this; }
    const CString& operator=(LPCSTR lpsz) { m_str = lpsz ? lpsz : ""; return *this; }
    const CString& operator=(const WCHAR* lpwsz) {
        m_str.clear();
        if (lpwsz) {
            while (*lpwsz) m_str += (char)*lpwsz++;
        }
        return *this;
    }

    const CString& operator+=(const CString& s) { m_str += s.m_str; return *this; }
    const CString& operator+=(char ch) { m_str += ch; return *this; }
    const CString& operator+=(LPCSTR lpsz) { if (lpsz) m_str += lpsz; return *this; }
    const CString& operator+=(const WCHAR* lpwsz) {
        if (lpwsz) {
            while (*lpwsz) m_str += (char)*lpwsz++;
        }
        return *this;
    }

    friend CString operator+(const CString& s1, const CString& s2) { return CString((s1.m_str + s2.m_str).c_str()); }
    friend CString operator+(const CString& s1, char ch) { return CString((s1.m_str + ch).c_str()); }
    friend CString operator+(char ch, const CString& s2) { return CString(( std::string(1, ch) + s2.m_str ).c_str()); }
    friend CString operator+(const CString& s1, LPCSTR lpsz) { return CString((s1.m_str + (lpsz ? lpsz : "")).c_str()); }
    friend CString operator+(LPCSTR lpsz, const CString& s2) { return CString(((lpsz ? lpsz : "") + s2.m_str).c_str()); }

    int Compare(LPCSTR lpsz) const { return std::strcmp(m_str.c_str(), lpsz ? lpsz : ""); }
    int CompareNoCase(LPCSTR lpsz) const { return strcasecmp(m_str.c_str(), lpsz ? lpsz : ""); }

    bool operator==(const CString& s) const { return m_str == s.m_str; }
    bool operator==(LPCSTR s) const { return m_str == (s ? s : ""); }
    friend bool operator==(LPCSTR s1, const CString& s2) { return (s1 ? s1 : "") == s2.m_str; }
    bool operator!=(const CString& s) const { return m_str != s.m_str; }
    bool operator!=(LPCSTR s) const { return m_str != (s ? s : ""); }
    friend bool operator!=(LPCSTR s1, const CString& s2) { return (s1 ? s1 : "") != s2.m_str; }

    CString Mid(int nFirst, int nCount) const {
        if (nFirst < 0) nFirst = 0;
        if (nFirst >= (int)m_str.size() || nCount <= 0) return CString();
        return CString(m_str.substr(nFirst, nCount).c_str());
    }
    CString Mid(int nFirst) const {
        if (nFirst < 0) nFirst = 0;
        if (nFirst >= (int)m_str.size()) return CString();
        return CString(m_str.substr(nFirst).c_str());
    }
    CString Left(int nCount) const {
        if (nCount <= 0) return CString();
        return CString(m_str.substr(0, nCount).c_str());
    }
    CString Right(int nCount) const {
        if (nCount <= 0) return CString();
        if (nCount >= (int)m_str.size()) return *this;
        return CString(m_str.substr(m_str.size() - nCount).c_str());
    }

    void MakeUpper() { for (char& c : m_str) c = (char)std::toupper((unsigned char)c); }
    void MakeLower() { for (char& c : m_str) c = (char)std::tolower((unsigned char)c); }
    void TrimLeft() {
        size_t i = 0;
        while (i < m_str.size() && std::isspace((unsigned char)m_str[i])) i++;
        m_str.erase(0, i);
    }
    void TrimRight() {
        while (!m_str.empty() && std::isspace((unsigned char)m_str.back())) m_str.pop_back();
    }

    int Find(char ch, int nStart = 0) const {
        size_t p = m_str.find(ch, nStart);
        return p == std::string::npos ? -1 : (int)p;
    }
    int Find(LPCSTR lpszSub, int nStart = 0) const {
        if (!lpszSub) return -1;
        size_t p = m_str.find(lpszSub, nStart);
        return p == std::string::npos ? -1 : (int)p;
    }
    int ReverseFind(char ch) const {
        size_t p = m_str.rfind(ch);
        return p == std::string::npos ? -1 : (int)p;
    }
    int FindOneOf(LPCSTR lpszCharSet) const {
        if (!lpszCharSet) return -1;
        size_t p = m_str.find_first_of(lpszCharSet);
        return p == std::string::npos ? -1 : (int)p;
    }

    LPSTR GetBuffer(int nMinBufLength) {
        if ((int)m_str.size() < nMinBufLength) m_str.resize(nMinBufLength, '\0');
        return m_str.data();
    }
    LPSTR GetBufferSetLength(int nNewLength) {
        m_str.resize(nNewLength, '\0');
        return m_str.data();
    }
    void ReleaseBuffer(int nNewLength = -1) {
        if (nNewLength >= 0) m_str.resize(nNewLength);
        else m_str.resize(std::strlen(m_str.c_str()));
    }

    void Format(LPCSTR lpszFormat, ...) {
        char buf[1024];
        va_list args;
        va_start(args, lpszFormat);
        std::vsnprintf(buf, sizeof(buf), lpszFormat, args);
        va_end(args);
        m_str = buf;
    }
    void Format(UINT nFormatID, ...) {
        char fmt[512] = "";
        AfxLoadString(nFormatID, fmt, sizeof(fmt));
        char buf[1024];
        va_list args;
        va_start(args, nFormatID);
        std::vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        m_str = buf;
    }

    BOOL LoadString(UINT nID) {
        char buf[512] = "";
        BOOL ok = AfxLoadString(nID, buf, sizeof(buf));
        m_str = buf;
        return ok;
    }
};

// ============================================================================
// MFC Collections (CByteArray, CObArray, CPtrArray, CObList, CPtrList)
// ============================================================================

struct __POSITION {};
typedef __POSITION* POSITION;

class CByteArray : public CObject {
    std::vector<BYTE> m_data;
public:
    CByteArray() {}
    int GetSize() const { return (int)m_data.size(); }
    int GetUpperBound() const { return (int)m_data.size() - 1; }
    void SetSize(int nNewSize, int nGrowBy = -1) { m_data.resize(nNewSize, 0); }
    void RemoveAll() { m_data.clear(); }
    BYTE GetAt(int nIndex) const { return m_data[nIndex]; }
    void SetAt(int nIndex, BYTE newElement) { m_data[nIndex] = newElement; }
    void SetAtGrow(int nIndex, BYTE newElement) {
        if (nIndex >= (int)m_data.size()) m_data.resize(nIndex + 1, 0);
        m_data[nIndex] = newElement;
    }
    int Add(BYTE newElement) { m_data.push_back(newElement); return (int)m_data.size() - 1; }
    BYTE& ElementAt(int nIndex) { return m_data[nIndex]; }
    const BYTE* GetData() const { return m_data.data(); }
    BYTE* GetData() { return m_data.data(); }
    void InsertAt(int nIndex, BYTE newElement, int nCount = 1) {
        if (nIndex > (int)m_data.size()) m_data.resize(nIndex, 0);
        m_data.insert(m_data.begin() + nIndex, nCount, newElement);
    }
    void RemoveAt(int nIndex, int nCount = 1) {
        if (nIndex >= 0 && nIndex < (int)m_data.size()) {
            int end = (std::min)((int)m_data.size(), nIndex + nCount);
            m_data.erase(m_data.begin() + nIndex, m_data.begin() + end);
        }
    }
    BYTE operator[](int nIndex) const { return m_data[nIndex]; }
    BYTE& operator[](int nIndex) { return m_data[nIndex]; }
};

class CObArray : public CObject {
    std::vector<CObject*> m_data;
public:
    CObArray() {}
    int GetSize() const { return (int)m_data.size(); }
    int GetUpperBound() const { return (int)m_data.size() - 1; }
    void SetSize(int nNewSize, int nGrowBy = -1) { m_data.resize(nNewSize, nullptr); }
    void RemoveAll() { m_data.clear(); }
    CObject* GetAt(int nIndex) const { return (nIndex >= 0 && nIndex < (int)m_data.size()) ? m_data[nIndex] : nullptr; }
    void SetAt(int nIndex, CObject* newElement) { if (nIndex >= 0 && nIndex < (int)m_data.size()) m_data[nIndex] = newElement; }
    void SetAtGrow(int nIndex, CObject* newElement) {
        if (nIndex >= (int)m_data.size()) m_data.resize(nIndex + 1, nullptr);
        m_data[nIndex] = newElement;
    }
    int Add(CObject* newElement) { m_data.push_back(newElement); return (int)m_data.size() - 1; }
    CObject*& ElementAt(int nIndex) { return m_data[nIndex]; }
    void InsertAt(int nIndex, CObject* newElement, int nCount = 1) {
        if (nIndex > (int)m_data.size()) m_data.resize(nIndex, nullptr);
        m_data.insert(m_data.begin() + nIndex, nCount, newElement);
    }
    void RemoveAt(int nIndex, int nCount = 1) {
        if (nIndex >= 0 && nIndex < (int)m_data.size()) {
            int end = (std::min)((int)m_data.size(), nIndex + nCount);
            m_data.erase(m_data.begin() + nIndex, m_data.begin() + end);
        }
    }
    CObject* operator[](int nIndex) const { return GetAt(nIndex); }
    CObject*& operator[](int nIndex) { return m_data[nIndex]; }
};

class CPtrArray : public CObject {
    std::vector<void*> m_data;
public:
    int GetSize() const { return (int)m_data.size(); }
    void SetSize(int nNewSize, int nGrowBy = -1) { m_data.resize(nNewSize, nullptr); }
    void RemoveAll() { m_data.clear(); }
    void* GetAt(int nIndex) const { return m_data[nIndex]; }
    void SetAt(int nIndex, void* newElement) { m_data[nIndex] = newElement; }
    int Add(void* newElement) { m_data.push_back(newElement); return (int)m_data.size() - 1; }
    void RemoveAt(int nIndex, int nCount = 1) {
        if (nIndex >= 0 && nIndex < (int)m_data.size())
            m_data.erase(m_data.begin() + nIndex, m_data.begin() + (std::min)((int)m_data.size(), nIndex + nCount));
    }
    void* operator[](int nIndex) const { return m_data[nIndex]; }
    void*& operator[](int nIndex) { return m_data[nIndex]; }
};

class CObList : public CObject {
    struct Node {
        CObject* val;
        Node* prev;
        Node* next;
    };
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
    int m_count = 0;
public:
    CObList() {}
    ~CObList() { RemoveAll(); }
    int GetCount() const { return m_count; }
    BOOL IsEmpty() const { return m_count == 0; }
    CObject*& GetHead() { return m_head->val; }
    CObject* GetHead() const { return m_head->val; }
    CObject*& GetTail() { return m_tail->val; }
    CObject* GetTail() const { return m_tail->val; }
    CObject* RemoveHead() {
        if (!m_head) return nullptr;
        Node* n = m_head;
        CObject* v = n->val;
        m_head = n->next;
        if (m_head) m_head->prev = nullptr;
        else m_tail = nullptr;
        delete n;
        m_count--;
        return v;
    }
    CObject* RemoveTail() {
        if (!m_tail) return nullptr;
        Node* n = m_tail;
        CObject* v = n->val;
        m_tail = n->prev;
        if (m_tail) m_tail->next = nullptr;
        else m_head = nullptr;
        delete n;
        m_count--;
        return v;
    }
    POSITION AddHead(CObject* newElement) {
        Node* n = new Node{newElement, nullptr, m_head};
        if (m_head) m_head->prev = n;
        else m_tail = n;
        m_head = n;
        m_count++;
        return (POSITION)n;
    }
    POSITION AddTail(CObject* newElement) {
        Node* n = new Node{newElement, m_tail, nullptr};
        if (m_tail) m_tail->next = n;
        else m_head = n;
        m_tail = n;
        m_count++;
        return (POSITION)n;
    }
    void RemoveAll() {
        while (m_head) RemoveHead();
    }
    POSITION GetHeadPosition() const { return (POSITION)m_head; }
    POSITION GetTailPosition() const { return (POSITION)m_tail; }
    CObject*& GetNext(POSITION& rPosition) {
        Node* n = (Node*)rPosition;
        rPosition = (POSITION)n->next;
        return n->val;
    }
    CObject* GetNext(POSITION& rPosition) const {
        Node* n = (Node*)rPosition;
        rPosition = (POSITION)n->next;
        return n->val;
    }
    CObject*& GetPrev(POSITION& rPosition) {
        Node* n = (Node*)rPosition;
        rPosition = (POSITION)n->prev;
        return n->val;
    }
    CObject* GetPrev(POSITION& rPosition) const {
        Node* n = (Node*)rPosition;
        rPosition = (POSITION)n->prev;
        return n->val;
    }
    CObject*& GetAt(POSITION position) { return ((Node*)position)->val; }
    CObject* GetAt(POSITION position) const { return ((Node*)position)->val; }
    void SetAt(POSITION pos, CObject* newElement) { ((Node*)pos)->val = newElement; }
    POSITION InsertAfter(POSITION position, CObject* newElement) {
        if (!position) return AddTail(newElement);
        Node* posNode = (Node*)position;
        Node* n = new Node{newElement, posNode, posNode->next};
        if (posNode->next) posNode->next->prev = n;
        else m_tail = n;
        posNode->next = n;
        m_count++;
        return (POSITION)n;
    }
    POSITION InsertBefore(POSITION position, CObject* newElement) {
        if (!position) return AddHead(newElement);
        Node* posNode = (Node*)position;
        Node* n = new Node{newElement, posNode->prev, posNode};
        if (posNode->prev) posNode->prev->next = n;
        else m_head = n;
        posNode->prev = n;
        m_count++;
        return (POSITION)n;
    }
    void RemoveAt(POSITION position) {
        Node* n = (Node*)position;
        if (!n) return;
        if (n->prev) n->prev->next = n->next;
        else m_head = n->next;
        if (n->next) n->next->prev = n->prev;
        else m_tail = n->prev;
        delete n;
        m_count--;
    }
    POSITION Find(CObject* searchValue, POSITION startAfter = nullptr) const {
        Node* n = startAfter ? ((Node*)startAfter)->next : m_head;
        while (n) {
            if (n->val == searchValue) return (POSITION)n;
            n = n->next;
        }
        return nullptr;
    }
    POSITION FindIndex(int nIndex) const {
        Node* n = m_head;
        while (n && nIndex > 0) { n = n->next; nIndex--; }
        return (POSITION)n;
    }
};

class CPtrList : public CObList {};

// ============================================================================
// Exceptions, CFile, CArchive
// ============================================================================

class CException : public CObject {
public:
    void Delete() { delete this; }
};

class CMemoryException : public CException {};
class CResourceException : public CException {};
class CUserException : public CException {};

class CFileException : public CException {
public:
    enum {
        none,
        generic,
        fileNotFound,
        badPath,
        tooManyOpenFiles,
        accessDenied,
        invalidFile,
        removeCurrentDir,
        directoryFull,
        badSeek,
        hardIO,
        sharingViolation,
        lockViolation,
        diskFull,
        endOfFile
    };
    int m_cause = none;
    LONG m_lOsError = 0;
};

class CNotSupportedException : public CException {};
class COleException : public CException { public: SCODE m_sc = 0; };

void AfxThrowMemoryException();
void AfxThrowResourceException();
void AfxThrowUserException();
inline void AfxThrowFileException(int cause = 0, LONG lOsError = -1, LPCTSTR lpszFileName = NULL) { throw CFileException(); }
inline void AfxThrowNotSupportedException() { throw CNotSupportedException(); }
inline void AfxThrowOleException(SCODE sc = 0) { throw COleException(); }

struct CFileStatus {
    LONG m_size = 0;
    BYTE m_attribute = 0;
    TCHAR m_szFullName[_MAX_PATH] = "";
};

class CFile : public CObject {
    FILE* m_fp = nullptr;
    CString m_strFileName;
public:
    HANDLE m_hFile = (HANDLE)1;
    enum OpenFlags {
        modeRead =          0x0000,
        modeWrite =         0x0001,
        modeReadWrite =     0x0002,
        shareCompat =       0x0000,
        shareExclusive =    0x0010,
        shareDenyWrite =    0x0020,
        shareDenyRead =     0x0030,
        shareDenyNone =     0x0040,
        modeNoInherit =     0x0080,
        modeCreate =        0x1000,
        modeNoTruncate =    0x2000,
        typeText =          0x4000,
        typeBinary =   (int)0x8000
    };
    enum Attribute {
        normal =    0x00,
        readOnly =  0x01,
        hidden =    0x02,
        system =    0x04,
        volume =    0x08,
        directory = 0x10,
        archive =   0x20
    };
    enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

    CFile();
    CFile(LPCTSTR lpszFileName, UINT nOpenFlags);
    virtual ~CFile();

    virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError = nullptr);
    virtual UINT Read(void* lpBuf, UINT nCount);
    virtual void Write(const void* lpBuf, UINT nCount);
    virtual LONG Seek(LONG lOff, UINT nFrom);
    virtual DWORD GetPosition() const;
    virtual DWORD GetLength() const;
    virtual CString GetFilePath() const { return m_strFileName; }
    virtual void SetLength(DWORD dwNewLen);
    virtual void Close();
    virtual void Abort();
    virtual void Flush();

    static BOOL GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus);
    static void Remove(LPCTSTR lpszFileName);
    static void Rename(LPCTSTR lpszOldName, LPCTSTR lpszNewName);
};

class CMemFile : public CFile {
public:
    CMemFile(UINT nGrowBytes = 1024) {}
    CMemFile(BYTE* lpBuffer, UINT nBufferSize, UINT nGrowBytes = 0) {}
};

class CDocument;

class CArchive {
    CFile* m_pFile = nullptr;
    UINT m_nMode = 0;
public:
    CDocument* m_pDocument = nullptr;
    BOOL m_bForceFlat = FALSE;
    enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2 };
    CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = nullptr)
        : m_pFile(pFile), m_nMode(nMode) {}
    BOOL IsLoading() const { return (m_nMode & load) != 0; }
    BOOL IsStoring() const { return (m_nMode & load) == 0; }
    CFile* GetFile() const { return m_pFile; }
    UINT Read(void* lpBuf, UINT nMax) { return m_pFile ? m_pFile->Read(lpBuf, nMax) : 0; }
    void Write(const void* lpBuf, UINT nMax) { if (m_pFile) m_pFile->Write(lpBuf, nMax); }
    void Flush() { if (m_pFile) m_pFile->Flush(); }
    void Close() {}
    void Abort() {}
};

// ============================================================================
// GDI Objects (CGdiObject, CPen, CBrush, CFont, CBitmap, CPalette, CRgn, CDC)
// ============================================================================

class CGdiObject : public CObject {
public:
    DECLARE_DYNCREATE(CGdiObject)
    HGDIOBJ m_hObject = nullptr;

    CGdiObject() : m_hObject(nullptr) {}
    virtual ~CGdiObject() { DeleteObject(); }

    operator HGDIOBJ() const { return m_hObject; }
    HGDIOBJ GetSafeHandle() const { return this ? m_hObject : nullptr; }

    static CGdiObject* FromHandle(HGDIOBJ hObject);
    BOOL Attach(HGDIOBJ hObject);
    HGDIOBJ Detach();
    BOOL DeleteObject();
    int GetObject(int nCount, LPVOID lpObject) const;
    BOOL CreateStockObject(int nIndex);
    BOOL UnrealizeObject() { return TRUE; }
};

class CPen : public CGdiObject {
public:
    DECLARE_DYNAMIC(CPen)
    static CPen* FromHandle(HPEN hPen);
    CPen() {}
    CPen(int nPenStyle, int nWidth, COLORREF crColor);
    CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = nullptr);
    operator HPEN() const { return (HPEN)(this ? m_hObject : nullptr); }
    BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
    BOOL CreatePenIndirect(LPLOGPEN lpLogPen);
};

class CBitmap;

class CBrush : public CGdiObject {
public:
    DECLARE_DYNAMIC(CBrush)
    static CBrush* FromHandle(HBRUSH hBrush);
    CBrush() {}
    CBrush(COLORREF crColor);
    CBrush(int nIndex, COLORREF crColor);
    CBrush(CBitmap* pBitmap);
    operator HBRUSH() const { return (HBRUSH)(this ? m_hObject : nullptr); }
    BOOL CreateSolidBrush(COLORREF crColor);
    BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
    BOOL CreateBrushIndirect(const LOGBRUSH* lpLogBrush);
    BOOL CreatePatternBrush(CBitmap* pBitmap);
    BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
    BOOL CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage);
    BOOL CreateSysColorBrush(int nIndex);
};

class CFont : public CGdiObject {
public:
    DECLARE_DYNAMIC(CFont)
    static CFont* FromHandle(HFONT hFont);
    CFont() {}
    operator HFONT() const { return (HFONT)(this ? m_hObject : nullptr); }
    BOOL CreateFontIndirect(const LOGFONT* lpLogFont);
    BOOL CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation,
                    int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut,
                    BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision,
                    BYTE nQuality, BYTE nPitchAndFamily, LPCTSTR lpszFacename);
    BOOL CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, class CDC* pDC = nullptr);
    int GetLogFont(LOGFONT* pLogFont);
};

class CBitmap : public CGdiObject {
public:
    DECLARE_DYNAMIC(CBitmap)
    static CBitmap* FromHandle(HBITMAP hBitmap);
    CBitmap() {}
    operator HBITMAP() const { return (HBITMAP)(this ? m_hObject : nullptr); }
    BOOL LoadBitmap(LPCTSTR lpszResourceName);
    BOOL LoadBitmap(UINT nIDResource);
    BOOL LoadOEMBitmap(UINT nIDBitmap);
    BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits);
    BOOL CreateBitmapIndirect(BITMAP* lpBitmap);
    BOOL CreateCompatibleBitmap(class CDC* pDC, int nWidth, int nHeight);
    BOOL CreateDiscardableBitmap(class CDC* pDC, int nWidth, int nHeight);
    DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
    DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
    CSize SetBitmapDimension(int nWidth, int nHeight);
    CSize GetBitmapDimension() const;
    int GetBitmap(BITMAP* pBitMap) { return GetObject(sizeof(BITMAP), pBitMap); }
};

class CPalette : public CGdiObject {
public:
    DECLARE_DYNAMIC(CPalette)
    static CPalette* FromHandle(HPALETTE hPalette);
    CPalette() {}
    operator HPALETTE() const { return (HPALETTE)(this ? m_hObject : nullptr); }
    BOOL CreatePalette(LPLOGPALETTE lpLogPalette);
    BOOL CreateHalftonePalette(class CDC* pDC);
    int GetEntryCount();
    UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
    UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
    void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
    UINT GetNearestPaletteIndex(COLORREF crColor) const;
    BOOL ResizePalette(UINT nNumEntries);
};

class CRgn : public CGdiObject {
public:
    DECLARE_DYNAMIC(CRgn)
    static CRgn* FromHandle(HRGN hRgn);
    CRgn() {}
    operator HRGN() const { return (HRGN)(this ? m_hObject : nullptr); }
    BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
    BOOL CreateRectRgnIndirect(LPCRECT lpRect);
    BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
    BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
    BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
    BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, int* lpPolyCounts, int nCount, int nPolyFillMode);
    void SetRectRgn(int x1, int y1, int x2, int y2);
    void SetRectRgn(LPCRECT lpRect);
    int CombineRgn(const CRgn* pRgn1, const CRgn* pRgn2, int nCombineMode);
    int CopyRgn(const CRgn* pRgnSrc);
    BOOL EqualRgn(const CRgn* pRgn) const;
    int OffsetRgn(int x, int y);
    int OffsetRgn(POINT point) { return OffsetRgn(point.x, point.y); }
    int GetRgnBox(LPRECT lpRect) const;
    BOOL PtInRegion(int x, int y) const;
    BOOL PtInRegion(POINT point) const { return PtInRegion(point.x, point.y); }
    BOOL RectInRegion(LPCRECT lpRect) const;
};

// ============================================================================
// CDC (Device Context)
// ============================================================================

class CWnd;

class CDC : public CObject {
public:
    DECLARE_DYNCREATE(CDC)
    HDC m_hDC = nullptr;
    HDC m_hAttribDC = nullptr;
    BOOL m_bAutoDelete = FALSE;

    CDC();
    virtual ~CDC();

    static CDC* FromHandle(HDC hDC);
    HDC GetSafeHdc() const { return this ? m_hDC : nullptr; }
    operator HDC() const { return this ? m_hDC : nullptr; }
    CWnd* GetWindow() const;

    BOOL Attach(HDC hDC);
    HDC Detach();

    BOOL CreateDC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData);
    BOOL CreateIC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName, LPCTSTR lpszOutput, const void* lpInitData);
    BOOL CreateCompatibleDC(CDC* pDC);
    BOOL DeleteDC();

    virtual int SaveDC();
    virtual BOOL RestoreDC(int nSavedDC);
    void DPtoHIMETRIC(LPSIZE lpSize) const {
        if (lpSize) {
            lpSize->cx = (lpSize->cx * 2540) / 96;
            lpSize->cy = (lpSize->cy * 2540) / 96;
        }
    }
    void HIMETRICtoDP(LPSIZE lpSize) const {
        if (lpSize) {
            lpSize->cx = (lpSize->cx * 96) / 2540;
            lpSize->cy = (lpSize->cy * 96) / 2540;
        }
    }
    int GetDeviceCaps(int nIndex) const;
    UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags);
    UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);

    CPoint GetBrushOrg() const;
    CPoint SetBrushOrg(int x, int y);
    CPoint SetBrushOrg(POINT point) { return SetBrushOrg(point.x, point.y); }
    int EnumObjects(int nObjectType, int (*lpfn)(LPVOID, LPARAM), LPARAM lpData);

    virtual CGdiObject* SelectStockObject(int nIndex);
    CPen* SelectObject(CPen* pPen);
    CBrush* SelectObject(CBrush* pBrush);
    virtual CFont* SelectObject(CFont* pFont);
    CBitmap* SelectObject(CBitmap* pBitmap);
    int SelectObject(CRgn* pRgn);
    CGdiObject* SelectObject(CGdiObject* pObject);
    HGDIOBJ SelectObject(HGDIOBJ hObject);

    COLORREF GetNearestColor(COLORREF crColor) const;
    CPalette* SelectPalette(CPalette* pPalette, BOOL bForceBackground);
    UINT RealizePalette();
    void UpdateColors();

    COLORREF GetBkColor() const;
    int GetBkMode() const;
    int GetPolyFillMode() const;
    int GetROP2() const;
    int GetStretchBltMode() const;
    COLORREF GetTextColor() const;

    virtual COLORREF SetBkColor(COLORREF crColor);
    int SetBkMode(int nBkMode);
    int SetMapMode(int nMapMode) { return 1; }
    int GetMapMode() const { return 1; }
    int SetPolyFillMode(int nPolyFillMode);
    int SetROP2(int nDrawMode);
    int SetStretchBltMode(int nStretchMode);
    virtual COLORREF SetTextColor(COLORREF crColor);

    CPoint GetWindowOrg() const;
    CPoint SetWindowOrg(int x, int y);
    CPoint SetWindowOrg(POINT point) { return SetWindowOrg(point.x, point.y); }
    CPoint OffsetWindowOrg(int nWidth, int nHeight);
    CSize GetWindowExt() const;
    virtual CSize SetWindowExt(int cx, int cy);
    virtual CSize SetWindowExt(SIZE size) { return SetWindowExt(size.cx, size.cy); }
    CPoint GetViewportOrg() const;
    virtual CPoint SetViewportOrg(int x, int y);
    virtual CPoint SetViewportOrg(POINT point) { return SetViewportOrg(point.x, point.y); }
    virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
    CSize GetViewportExt() const;
    virtual CSize SetViewportExt(int cx, int cy);
    virtual CSize SetViewportExt(SIZE size) { return SetViewportExt(size.cx, size.cy); }

    void DPtoLP(LPPOINT lpPoints, int nCount = 1) const {}
    void DPtoLP(LPRECT lpRect) const {}
    void LPtoDP(LPPOINT lpPoints, int nCount = 1) const {}
    void LPtoDP(LPRECT lpRect) const {}

    BOOL FillRgn(CRgn* pRgn, CBrush* pBrush);
    BOOL FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight);
    BOOL InvertRgn(CRgn* pRgn);
    BOOL PaintRgn(CRgn* pRgn);

    virtual int GetClipBox(LPRECT lpRect) const;
    virtual BOOL PtVisible(int x, int y) const;
    BOOL PtVisible(POINT point) const { return PtVisible(point.x, point.y); }
    virtual BOOL RectVisible(LPCRECT lpRect) const;
    int SelectClipRgn(CRgn* pRgn);
    int ExcludeClipRect(int x1, int y1, int x2, int y2);
    int ExcludeClipRect(LPCRECT lpRect);
    int ExcludeUpdateRgn(class CWnd* pWnd);
    int IntersectClipRect(int x1, int y1, int x2, int y2);
    int IntersectClipRect(LPCRECT lpRect);
    int OffsetClipRgn(int x, int y);
    int OffsetClipRgn(SIZE size) { return OffsetClipRgn(size.cx, size.cy); }

    CPoint GetCurrentPosition() const;
    CPoint MoveTo(int x, int y);
    CPoint MoveTo(POINT point) { return MoveTo(point.x, point.y); }
    BOOL LineTo(int x, int y);
    BOOL LineTo(POINT point) { return LineTo(point.x, point.y); }
    BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
    BOOL Polyline(LPPOINT lpPoints, int nCount);

    void FillRect(LPCRECT lpRect, CBrush* pBrush);
    void FrameRect(LPCRECT lpRect, CBrush* pBrush);
    void InvertRect(LPCRECT lpRect);
    BOOL DrawIcon(int x, int y, HICON hIcon);
    BOOL DrawIcon(POINT point, HICON hIcon) { return DrawIcon(point.x, point.y, hIcon); }
    void FillSolidRect(LPCRECT lpRect, COLORREF clr);
    void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
    void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
    void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);

    BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
    BOOL DrawFocusRect(LPCRECT lpRect);
    BOOL Ellipse(int x1, int y1, int x2, int y2);
    BOOL Ellipse(LPCRECT lpRect);
    BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
    BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
    BOOL Polygon(LPPOINT lpPoints, int nCount);
    BOOL PolyPolygon(LPPOINT lpPoints, int* lpPolyCounts, int nCount);
    BOOL Rectangle(int x1, int y1, int x2, int y2);
    BOOL Rectangle(LPCRECT lpRect);
    BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
    BOOL RoundRect(LPCRECT lpRect, POINT point);

    BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
    BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop);
    BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
    COLORREF GetPixel(int x, int y) const;
    COLORREF GetPixel(POINT point) const { return GetPixel(point.x, point.y); }
    COLORREF SetPixel(int x, int y, COLORREF crColor);
    COLORREF SetPixel(POINT point, COLORREF crColor) { return SetPixel(point.x, point.y, crColor); }
    BOOL FloodFill(int x, int y, COLORREF crColor);
    BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
    BOOL MaskBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask, DWORD dwRop);

    virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
    BOOL TextOut(int x, int y, const CString& str) { return TextOut(x, y, (LPCTSTR)str, str.GetLength()); }
    virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, int* lpDxWidths);
    BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, const CString& str, int* lpDxWidths) {
        return ExtTextOut(x, y, nOptions, lpRect, (LPCTSTR)str, str.GetLength(), lpDxWidths);
    }
    virtual CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, int* lpnTabStopPositions, int nTabOrigin);
    virtual int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat);
    int DrawText(const CString& str, LPRECT lpRect, UINT nFormat) { return DrawText((LPCTSTR)str, str.GetLength(), lpRect, nFormat); }
    CSize GetTextExtent(LPCTSTR lpszString, int nCount) const;
    CSize GetTextExtent(const CString& str) const { return GetTextExtent((LPCTSTR)str, str.GetLength()); }
    CSize GetOutputTextExtent(LPCTSTR lpszString, int nCount) const { return GetTextExtent(lpszString, nCount); }
    CSize GetOutputTextExtent(const CString& str) const { return GetTextExtent(str); }
    CSize GetTabbedTextExtent(LPCTSTR lpszString, int nCount, int nTabPositions, int* lpnTabStopPositions) const { return GetTextExtent(lpszString, nCount); }
    UINT GetTextAlign() const;
    UINT SetTextAlign(UINT nFlags);
    int GetTextFace(int nCount, LPTSTR lpszFacename) const;
    int GetTextFace(CString& rString) const;
    BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
    BOOL GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const { return GetTextMetrics(lpMetrics); }
    BOOL GrayString(CBrush* pBrush, BOOL (*lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight);
    BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
    BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);
};

class CPaintDC : public CDC {
public:
    DECLARE_DYNAMIC(CPaintDC)
    HWND m_hWnd;
    PAINTSTRUCT m_ps;
    CPaintDC(CWnd* pWnd);
    virtual ~CPaintDC();
};

class CClientDC : public CDC {
public:
    DECLARE_DYNAMIC(CClientDC)
    HWND m_hWnd;
    CClientDC(CWnd* pWnd);
    virtual ~CClientDC();
};

class CWindowDC : public CDC {
public:
    DECLARE_DYNAMIC(CWindowDC)
    HWND m_hWnd;
    CWindowDC(CWnd* pWnd);
    virtual ~CWindowDC();
};

class CWaitCursor {
public:
    CWaitCursor() {}
    ~CWaitCursor() {}
    void Restore() {}
};

#include "mfc_windows.h"
