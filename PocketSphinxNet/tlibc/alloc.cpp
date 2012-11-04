// alloc.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)
#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <malloc.h>
#else
#include "libct.h"
#endif

using namespace System;
using namespace System::Runtime::InteropServices;

//BEGIN_EXTERN_C
#ifndef POCKETSPHINX_NET
#define HEAP_NO_SERIALIZE               0x00000001      
#define HEAP_GROWABLE                   0x00000002      
#define HEAP_GENERATE_EXCEPTIONS        0x00000004      
#define HEAP_ZERO_MEMORY                0x00000008      
#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010      
#define HEAP_TAIL_CHECKING_ENABLED      0x00000020      
#define HEAP_FREE_CHECKING_ENABLED      0x00000040      
#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080      
#define HEAP_CREATE_ALIGN_16            0x00010000      
#define HEAP_CREATE_ENABLE_TRACING      0x00020000      
#define HEAP_CREATE_ENABLE_EXECUTE      0x00040000      
#define HEAP_MAXIMUM_TAG                0x0FFF              
#define HEAP_PSEUDO_TAG_FLAG            0x8000              
#define HEAP_TAG_SHIFT                  18    
#endif

__declspec(restrict, noalias) void *malloc(size_t size)
{
#ifndef POCKETSPHINX_NET
    return HeapAlloc(GetProcessHeap(), 0, size);
#else
	return Marshal::AllocHGlobal((long long)size).ToPointer();
#endif
}

__declspec(noalias) void free(void *p)
{
#ifndef POCKETSPHINX_NET
    HeapFree(GetProcessHeap(), 0, p);
#else
	Marshal::FreeHGlobal(IntPtr(p));
#endif
}

__declspec(restrict, noalias) void *realloc(void *p, size_t size)
{
#ifndef POCKETSPHINX_NET
    if (p)
        return HeapReAlloc(GetProcessHeap(), 0, p, size)
		;
    else
        return HeapAlloc(GetProcessHeap(), 0, size)
		;
#else
	if(p!=0)
	{
		return Marshal::ReAllocHGlobal(IntPtr(p),IntPtr((long)size)).ToPointer();
	}
	else
	{
		return Marshal::AllocHGlobal((int)size).ToPointer();

	}
#endif
}

__declspec(restrict, noalias) void *calloc(size_t nitems, size_t size)
{
#ifndef POCKETSPHINX_NET
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nitems * size);

#else
	long long sz = nitems*size;

	IntPtr p = Marshal::AllocHGlobal(IntPtr(sz));

	if(p!=IntPtr::Zero)
	{
		memset(p.ToPointer(),0,(size_t)sz);
	}
	return p.ToPointer();

#endif
}

void *_calloc_crt(size_t nitems, size_t size)
{
	__declspec(restrict, noalias) void *calloc(size_t nitems, size_t size);
	return calloc(nitems, size);
}

void *_nh_malloc(size_t size, int nhFlag)
{
	nhFlag;
#ifndef POCKETSPHINX_NET
    return HeapAlloc(GetProcessHeap(), 0, size);
#else
	return malloc(size);
#endif
}

size_t _msize(void *p)
{
#ifndef POCKETSPHINX_NET
    return HeapSize(GetProcessHeap(), 0, p);
#else
	//NOTICE:not used, and can not be used!
	return 0;
#endif
}

//END_EXTERN_C