// sprintf.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)
#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <stdio.h>
// Force the linker to include USER32.LIB
#pragma comment(linker, "/defaultlib:user32.lib")
#else
#include "libct.h"
#endif


#define EOF     (-1)
//
//int sscanf(
//   const char *buffer,
//   const char *format, ... 
//)
//{
////NOTICE:BIG_TODO     
//	return 0;
//}

//int sprintf(char *buffer, const char *format, ...)
//{
////NOTICE:BIG_TODO     
//	//int retValue;
//    //va_list argptr;
//    //      
//    //va_start(argptr, format);
//    //retValue = vsprintf(buffer, format, argptr);
//    //va_end(argptr);
//
//    //return retValue;
//
//	return 0;
//}


//int vsnprintf(char *dest, size_t n, const char *fmt, va_list args)
//{
//	//n;
//	//return vsprintf(dest, fmt, args);
////NOTICE:BIG_TODO    
//
//	return 0;
//}


//int _snprintf(char *dest, size_t n, const char *fmt, ...)
//{	
//	//n;
//
//	//va_list args;
//	//va_start(args, fmt);
//	//int retValue = vsprintf(dest, fmt, args);
//	//va_end(args);
//	//return retValue;
//
//	return 0;
//}