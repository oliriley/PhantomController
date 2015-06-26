#ifndef SYSDEP_H_
#define SYSDEP_H_

#ifdef UNIX
// Windows type equivalencies for ease of porting
#define VOID            void
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif
#ifndef NULL
#define NULL            0L
#endif

#define ASSERT assert

typedef char            CHAR;
typedef short           SHORT;
typedef long            LONG;

typedef unsigned char   UCHAR, BYTE;
typedef unsigned short  USHORT, WORD;
typedef unsigned long   ULONG, DWORD;
typedef DWORD           COLORREF;
#define RGB(r,g,b)      ((COLORREF)(((DWORD)(BYTE)(r))<<24)|(((DWORD)(BYTE)(g))<<16)|(((DWORD)(BYTE)(b))<<8))

typedef char            WCHAR;

typedef CHAR            *PCHAR, *PSTR, *PWSTR, *LPCSTR, *PCWSTR, *LPCWSTR;
typedef UCHAR           *PUCHAR, *PBYTE;
typedef SHORT           *PSHORT;
typedef LONG            *PLONG;
typedef UCHAR           *PUCHAR;
typedef USHORT          *PUSHORT;
typedef ULONG           *PULONG;
typedef VOID            *PVOID;

typedef UCHAR           BOOL, BOOLEAN;

typedef VOID            *HANDLE;
#endif //UNIX

#endif
