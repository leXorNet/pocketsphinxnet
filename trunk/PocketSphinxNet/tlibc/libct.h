// libct.h

#pragma once

//#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

#ifndef _WCTYPE_T_DEFINED
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#define _WCTYPE_T_DEFINED
#endif

#ifndef _VA_LIST_DEFINED
#ifdef _M_CEE_PURE
typedef System::ArgIterator va_list;
#else
typedef char *  va_list;
#endif
#define _VA_LIST_DEFINED
#endif

#define __T(x)      x
#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)


#ifndef VOID
#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef int INT;
typedef unsigned long DWORD;
#endif

#undef  assert

#define _In_ 
#define _In_z_

#ifndef _CRT_WIDE
#define __CRT_WIDE(_String) L ## _String
#define _CRT_WIDE(_String) __CRT_WIDE(_String)
#endif


#ifndef POCKETSPHINX_NET
#ifdef  NDEBUG

#define assert(_Expression)     ((void)0)

#else


void _wassert(const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);
#endif

#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )
#else
#define assert(_Expression)     ((void)0)

#endif

//extern int _doserrno; 
//extern int errno; 
//extern char *_sys_errlist[ ];
//extern int _sys_nerr;
//
//extern TCHAR *_argv[];
//int _init_args();
//void _term_args();
//
//typedef void (__cdecl *_PVFV)();
//extern _PVFV __xc_a[], __xc_z[];    /* C++ initializers */

//void _initterm(_PVFV *pfbegin, _PVFV *pfend);
//void _init_atexit();
//void _doexit();

__declspec(restrict, noalias) void *malloc(size_t size);
__declspec(noalias) void free(void *p);
__declspec(restrict, noalias) void *realloc(void *p, size_t size);
__declspec(restrict, noalias) void *calloc(size_t nitems, size_t size);
void *_calloc_crt(size_t nitems, size_t size);
void *_nh_malloc(size_t size, int nhFlag);

size_t _msize(void *p);


long atol(const char *str);
int atoi(const char *str);
long wtol(const wchar_t *str);
int wtoi(const wchar_t *str);
float atof(const char *str);

#ifndef _NLSCMP_DEFINED
#define _NLSCMPERROR    2147483647  /* currently == INT_MAX */
#define _NLSCMP_DEFINED
#endif

int _wcsicmp(const wchar_t *s1, const wchar_t *s2) ;
int wcsicmp(const wchar_t *s1, const wchar_t *s2);
int wcscmp(const wchar_t *s1, const wchar_t *s2);
int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n);
int wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n);
wchar_t *wcsdup(const wchar_t *src);
wchar_t *_wcsdup(const wchar_t *src);
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src);
wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n);
size_t wcslen(const wchar_t *str);
const wchar_t *wcschr(const wchar_t *str, wchar_t ch);
const wchar_t *wcsrchr(const wchar_t *str, wchar_t ch);
wchar_t *wcscat(wchar_t *dst, const wchar_t *src);
const wchar_t *wcsstr(const wchar_t *str, const wchar_t *substr);


int memcmp(const void *b1, const void *b2, size_t n);
void *memset(void *dst, int val, size_t size);
void *memcpy(void *dst, const void *src, size_t size);
void *memmove(void *dst, const void *src, size_t count);

char *strupr(char *s);
wchar_t *wcsupr(wchar_t *s);
char *strlwr(char *s);
wchar_t *wcslwr(wchar_t *s);
int toupper(int c);
wint_t towupper(wint_t c);
int tolower(int c);
wint_t towlower(wint_t c);

int iswctype(wint_t c, wctype_t type);
int isspace(int c);
int iswspace(wint_t c);
int isupper(int c)	;
int iswupper(wint_t c);
int isalpha(int c)	;
int iswalpha(wint_t c);
int isdigit(int c);
int iswdigit(wint_t c);
int isxdigit(int c);
int iswxdigit(wint_t c);
int isalnum(int c);
int iswalnum(wint_t c);
int isprint(int c);
int iswprint(wint_t c);

//math.h
int abs(int n);
double fabs(double n);
double log(double x);
double sqrt(double x);
//void srand(unsigned int seed);
//int rand();
//void _CIacos();
//void _ftol2();
double sin(double x);
double cos(double x);
double log10(double x);
double pow(double x,double y);
double floor(double x);

void exit(int code);

int puts(const char *s);
int _putws(const wchar_t *s);

int strcmpi(const char *s1, const char *s2);
int stricmp(const char *s1, const char *s2);
int _stricmp(const char *s1, const char *s2);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
int strnicmp(const char *s1, const char *s2, size_t n);
char *strdup(const char *src);
char *_strdup(const char *src);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *str);
const char *strchr(const char *str, int ch);
const char *strrchr(const char *str, int ch);
char *strcat(char *dst, const char *src);
const char *strstr(const char *str, const char *substr);
size_t strspn(const char *str, const char *strCharSet);

char *strncat(
   char *strDest,
   const char *strSource,
   size_t count 
);

//int sprintf(char *buffer, const char *format, ...);
//int _snprintf(char *dest, size_t n, const char *fmt, ...);
//int vsnprintf(char *dest, size_t n, const char *fmt, va_list args);
//int sscanf(const char *buffer,const char *format, ... );


//int printf(const char *format, ...);
//int vprintf(const char *format, va_list args);

//qsort
void qsort(void *base, size_t num, size_t width, int (*comp)(const void *, const void *));

//_flag values (not the ones used by the normal CRT
#define _FILE_TEXT		0x0001
#define _FILE_EOF		0x0002
#define _FILE_ERROR		0x0004


#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

/* Seek method constants */

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0

#define EOF     (-1)


#ifndef _FILE_DEFINED
struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif

#ifndef POCKETSPHINX_NET
extern "C" FILE *stdin; 
extern "C" FILE *stdout; 
extern "C" FILE *stderr;
#endif

FILE *fopen(const char *path, const char *attrs);

//int fprintf(FILE *fp, const char *s, ...);
//int fscanf(FILE *stream,const char *format... );

int fclose(FILE *fp);
int fflush(FILE *fp);
int unlink( const char * pathname ); 
int fputc(int c,FILE *stream );

int getc(FILE *stream );

int feof(FILE *fp);
int fseek(FILE *str, long long offset, int origin);
long long ftell(FILE *fp);
size_t fread(void *buffer, size_t size, size_t count, FILE *str);
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *str);
char *fgets(char *str, int n, FILE *s);
int fgetc(FILE *s);

int ungetc(int c,FILE *stream);

void rewind(FILE *stream);
int clearerr( 
   FILE *stream 
);

int ferror( 
   FILE *stream 
);

int _mkdir(
   const char *dirname 
);



void _init_file();

//limits.h
#define CHAR_BIT      8         /* number of bits in a char */
#define SCHAR_MIN   (-128)      /* minimum signed char value */
#define SCHAR_MAX     127       /* maximum signed char value */
#define UCHAR_MAX     0xff      /* maximum unsigned char value */

#ifndef _CHAR_UNSIGNED
#define CHAR_MIN    SCHAR_MIN   /* mimimum char value */
#define CHAR_MAX    SCHAR_MAX   /* maximum char value */
#else
#define CHAR_MIN      0
#define CHAR_MAX    UCHAR_MAX
#endif  /* _CHAR_UNSIGNED */

#define MB_LEN_MAX    5             /* max. # bytes in multibyte char */
#define SHRT_MIN    (-32768)        /* minimum (signed) short value */
#define SHRT_MAX      32767         /* maximum (signed) short value */
#define USHRT_MAX     0xffff        /* maximum unsigned short value */
#define INT_MIN     (-2147483647 - 1) /* minimum (signed) int value */
#define INT_MAX       2147483647    /* maximum (signed) int value */
#define UINT_MAX      0xffffffff    /* maximum unsigned int value */
#define LONG_MIN    (-2147483647L - 1) /* minimum (signed) long value */
#define LONG_MAX      2147483647L   /* maximum (signed) long value */
#define ULONG_MAX     0xffffffffUL  /* maximum unsigned long value */
#define LLONG_MAX     9223372036854775807i64       /* maximum signed long long int value */
#define LLONG_MIN   (-9223372036854775807i64 - 1)  /* minimum signed long long int value */
#define ULLONG_MAX    0xffffffffffffffffui64       /* maximum unsigned long long int value */

#define _I8_MIN     (-127i8 - 1)    /* minimum signed 8 bit value */
#define _I8_MAX       127i8         /* maximum signed 8 bit value */
#define _UI8_MAX      0xffui8       /* maximum unsigned 8 bit value */

#define _I16_MIN    (-32767i16 - 1) /* minimum signed 16 bit value */
#define _I16_MAX      32767i16      /* maximum signed 16 bit value */
#define _UI16_MAX     0xffffui16    /* maximum unsigned 16 bit value */

#define _I32_MIN    (-2147483647i32 - 1) /* minimum signed 32 bit value */
#define _I32_MAX      2147483647i32 /* maximum signed 32 bit value */
#define _UI32_MAX     0xffffffffui32 /* maximum unsigned 32 bit value */

/* minimum signed 64 bit value */
#define _I64_MIN    (-9223372036854775807i64 - 1)
/* maximum signed 64 bit value */
#define _I64_MAX      9223372036854775807i64
/* maximum unsigned 64 bit value */
#define _UI64_MAX     0xffffffffffffffffui64

#if     _INTEGRAL_MAX_BITS >= 128
/* minimum signed 128 bit value */
#define _I128_MIN   (-170141183460469231731687303715884105727i128 - 1)
/* maximum signed 128 bit value */
#define _I128_MAX     170141183460469231731687303715884105727i128
/* maximum unsigned 128 bit value */
#define _UI128_MAX    0xffffffffffffffffffffffffffffffffui128
#endif

#ifndef SIZE_MAX
#ifdef _WIN64 
#define SIZE_MAX _UI64_MAX
#else
#define SIZE_MAX UINT_MAX
#endif
#endif

#if __STDC_WANT_SECURE_LIB__
#ifndef RSIZE_MAX
#define RSIZE_MAX    (SIZE_MAX >> 1)
#endif
#endif

#ifdef  _POSIX_

#define _POSIX_ARG_MAX      4096
#define _POSIX_CHILD_MAX    6
#define _POSIX_LINK_MAX     8
#define _POSIX_MAX_CANON    255
#define _POSIX_MAX_INPUT    255
#define _POSIX_NAME_MAX     14
#define _POSIX_NGROUPS_MAX  0
#define _POSIX_OPEN_MAX     16
#define _POSIX_PATH_MAX     255
#define _POSIX_PIPE_BUF     512
#define _POSIX_SSIZE_MAX    32767
#define _POSIX_STREAM_MAX   8
#define _POSIX_TZNAME_MAX   3

#define ARG_MAX             14500       /* 16k heap, minus overhead */
#define LINK_MAX            1024
#define MAX_CANON           _POSIX_MAX_CANON
#define MAX_INPUT           _POSIX_MAX_INPUT
#define NAME_MAX            255
#define NGROUPS_MAX         16
#define OPEN_MAX            32
#define PATH_MAX            512
#define PIPE_BUF            _POSIX_PIPE_BUF
#define SSIZE_MAX           _POSIX_SSIZE_MAX
#define STREAM_MAX          20
#define TZNAME_MAX          10

#endif  /* POSIX */
/* Error Codes */

#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
#define EINVAL          22
#define ERANGE          34
#define EILSEQ          42
#define STRUNCATE       80

//stat.h
#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86))
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifndef _TIME32_T_DEFINED
typedef _W64 long __time32_t;   /* 32-bit time value */
#define _TIME32_T_DEFINED
#endif

#ifndef _TIME64_T_DEFINED
typedef __int64 __time64_t;     /* 64-bit time value */
#define _TIME64_T_DEFINED
#endif

#ifndef _TIME_T_DEFINED
#ifdef _USE_32BIT_TIME_T
typedef __time32_t time_t;      /* time value */
#else
typedef __time64_t time_t;      /* time value */
#endif
#define _TIME_T_DEFINED         /* avoid multiple def's of time_t */
#endif


#ifndef _INO_T_DEFINED

typedef unsigned short _ino_t;          /* i-node number (not used on DOS) */

#if     !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned short ino_t;
#endif

#define _INO_T_DEFINED
#endif


#ifndef _DEV_T_DEFINED

typedef unsigned int _dev_t;            /* device code */

#if     !__STDC__
/* Non-ANSI name for compatibility */
typedef unsigned int dev_t;
#endif

#define _DEV_T_DEFINED
#endif


#ifndef _OFF_T_DEFINED

typedef long _off_t;                    /* file offset value */

#if     !__STDC__
/* Non-ANSI name for compatibility */
typedef long off_t;
#endif

#define _OFF_T_DEFINED
#endif


/* define structure for returning status information */

#ifndef _STAT_DEFINED

struct _stat32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
        };

#if     !__STDC__
/* Non-ANSI names for compatibility */
struct stat {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        time_t st_atime;
        time_t st_mtime;
        time_t st_ctime;
        };

#endif  /* __STDC__ */

struct _stat32i64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time32_t st_atime;
        __time32_t st_mtime;
        __time32_t st_ctime;
        };

struct _stat64i32 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        _off_t     st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
        };

struct _stat64 {
        _dev_t     st_dev;
        _ino_t     st_ino;
        unsigned short st_mode;
        short      st_nlink;
        short      st_uid;
        short      st_gid;
        _dev_t     st_rdev;
        __int64    st_size;
        __time64_t st_atime;
        __time64_t st_mtime;
        __time64_t st_ctime;
        };

/*
 * We have to have same name for structure and the fuction so as to do the
 * macro magic.we need the structure name and function name the same.
 */
#define __stat64    _stat64

#ifdef _USE_32BIT_TIME_T
#define _fstat      _fstat32
#define _fstati64   _fstat32i64
#define _stat       _stat32
#define _stati64    _stat32i64
#define _wstat      _wstat32
#define _wstati64   _wstat32i64

#else
#define _fstat      _fstat64i32
#define _fstati64   _fstat64
#define _stat       _stat64i32
#define _stati64    _stat64
#define _wstat      _wstat64i32
#define _wstati64   _wstat64

#endif

#define _STAT_DEFINED
#endif


#define _S_IFMT         0xF000          /* file type mask */
#define _S_IFDIR        0x4000          /* directory */
#define _S_IFCHR        0x2000          /* character special */
#define _S_IFIFO        0x1000          /* pipe */
#define _S_IFREG        0x8000          /* regular */
#define _S_IREAD        0x0100          /* read permission, owner */
#define _S_IWRITE       0x0080          /* write permission, owner */
#define _S_IEXEC        0x0040          /* execute/search permission, owner */



#define _S_IFMT         0xF000          /* file type mask */
#define _S_IFDIR        0x4000          /* directory */
#define _S_IFCHR        0x2000          /* character special */
#define _S_IFIFO        0x1000          /* pipe */
#define _S_IFREG        0x8000          /* regular */
#define _S_IREAD        0x0100          /* read permission, owner */
#define _S_IWRITE       0x0080          /* write permission, owner */
#define _S_IEXEC        0x0040          /* execute/search permission, owner */


/* Function prototypes */


//int stat(_In_z_ const char * _Name,  struct stat * _Stat);



#if     !__STDC__

/* Non-ANSI names for compatibility */

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

#endif  /* __STDC__ */

time_t time(time_t *timer );

//NOTICE: need to free the string
char *getenv( 
   const char *varname 
);

#ifdef __cplusplus
}
#endif

//void *operator new(unsigned int s);
//void operator delete(void *p);
//void operator delete[](void *p);
//void *operator new[](unsigned int s);
//void* operator new(size_t size);
//void operator delete(void *p);
//void operator delete(void *p,size_t size);
//
//void* operator new[](size_t size);
//void operator delete[](void *p);


#ifdef POCKETSPHINX_NET
using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::Runtime::InteropServices;


struct _FILE : public FILE
{
	void* file;

};

ref class FILEHelper{
public:

	static void SetHandle(_FILE* file,GCHandle handle){ file->file = (void*) GCHandle::ToIntPtr(handle).ToPointer();}


	static void SetStream(_FILE* file,FileStream^ fs){ file->file = (void*) GCHandle::ToIntPtr(GCHandle::Alloc(fs)).ToPointer();}

	static FileStream^ GetFileStream(_FILE* file){return (FileStream^) (GCHandle::FromIntPtr(IntPtr(file->file)).Target);}

	static void FreeStream(_FILE* file){ GCHandle::FromIntPtr(IntPtr(file->file)).Free();}
};


int net_fprintf(FILE* fp,String^ str,... array<Object^>^ args);


int net_fscanf_str(FILE* fp, size_t count, char* buffer);

int net_fscanf_int_space_sep(FILE* fp, bool skip_space,  int* n);

int net_fscanf_float_space_sep(FILE* fp, bool skip_space, float* n);

int net_sscanf_word(const char* src, char* buffer, int* length);

int net_sscanf_int(const char* src, int* val, int* length);

#endif
