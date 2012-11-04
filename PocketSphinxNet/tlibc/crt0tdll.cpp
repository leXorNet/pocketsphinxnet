// crt0tdll.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/13/06 (mv)

#include "libct.h"

#undef far
#undef near
#undef pascal

#define far
#define near
#undef FAR
#undef  NEAR
#define FAR                 far
#define NEAR                near
#ifndef CONST
#define CONST               const
#endif

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef void*				LPVOID;
#define WINAPI				__stdcall
typedef void far			*HANDLE;
#define DLL_PROCESS_ATTACH   1    
#define DLL_THREAD_ATTACH    2    
#define DLL_THREAD_DETACH    3    
#define DLL_PROCESS_DETACH   0  

BOOL WINAPI DllMain(HANDLE, DWORD, LPVOID);

//EXTERN_C 
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, DWORD reason, LPVOID imp)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		_init_atexit();
		_init_file();
		_initterm(__xc_a, __xc_z);
	}

	BOOL ret = DllMain(hInst, reason, imp);

	if (reason == DLL_PROCESS_DETACH)
	{
		_doexit();
	}

	return ret;
}