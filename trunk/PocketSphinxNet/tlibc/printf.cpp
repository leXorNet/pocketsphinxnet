// printf.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#else
#include "libct.h"
#endif

// Force the linker to include USER32.LIB
#pragma comment(linker, "/defaultlib:user32.lib")

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )

#ifdef  __cplusplus
#define _ADDRESSOF(v)   ( &reinterpret_cast<const char &>(v) )
#else
#define _ADDRESSOF(v)   ( &(v) )
#endif
//BEGIN_EXTERN_C
#define _crt_va_start(ap,v)  ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
#define _crt_va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define _crt_va_end(ap)      ( ap = (va_list)0 )
#define va_start _crt_va_start
#define va_arg _crt_va_arg
#define va_end _crt_va_end

int printf(const char *format, ...)
{
//NOTICE:BIG_TODO 
 //  va_list args;
 //   va_start(args, (va_list)format);
 //   int ret = vprintf(format, args);
 //   va_end((va_list)args);

	////return ret;
	return 0;
}

int vprintf(const char *format, va_list args)
{
//NOTICE:BIG_TODO 	
	//char szBuff[1024];

	//int retValue = wvsprintfA(szBuff, format, args);

	//fwrite(szBuff, retValue, 1, stdout);

	//return retValue;
	return 0;
}




//END_EXTERN_C