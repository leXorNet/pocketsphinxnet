// struplwr.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)
#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <string.h>
#else
#include "libct.h"
using namespace System;
using namespace System::Runtime::InteropServices;
#endif

//BEGIN_EXTERN_C

char *strupr(char *s)
{
	if(s!=0)
	{
		String^ str = gcnew String(s);
		String^ upp = str->ToUpper();

		return (char*)Marshal::StringToHGlobalAnsi(upp).ToPointer();
	}
    return 0;
}

wchar_t *wcsupr(wchar_t *s)
{
	if(s!=0)
	{
		String^ str = gcnew String(s);
		String^ upp = str->ToUpper();

		return (wchar_t*)Marshal::StringToHGlobalAnsi(upp).ToPointer();
	}
    return 0;
}

char *strlwr(char *s)
{
	if(s!=0)
	{
		String^ str = gcnew String(s);
		String^ upp = str->ToLower();

		return (char*)Marshal::StringToHGlobalAnsi(upp).ToPointer();
	}
    return 0;
}

wchar_t *wcslwr(wchar_t *s)
{
	if(s!=0)
	{
		String^ str = gcnew String(s);
		String^ upp = str->ToLower();

		return (wchar_t*)Marshal::StringToHGlobalAnsi(upp).ToPointer();
	}
    return 0;
}

int toupper(int c)
{
	if (c < 'a' || c > 'z')
		return c;
	return c-0x20;
}

wint_t towupper(wint_t c)
{
	// yes, that's right:  no &c - see the docs

	return Char::ToUpper(c);
}

int tolower(int c)
{
	if (c < 'A' || c > 'Z')
		return c;
	return c+0x20;
}

wint_t towlower(wint_t c)
{
	return Char::ToLower(c);
}

//END_EXTERN_C