// isctype.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <ctype.h>
#else
#include "libct.h"
using namespace System;
using namespace System::Globalization;

#endif

//BEGIN_EXTERN_C



int isdigit(int c)		{return (c >= '0' && c <= '9');}
int isspace(int c)		{return ((c >= 0x09 && c <= 0x0D) || (c == 0x20));}


#ifndef POCKETSPHINX_NET

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short WORD;
#endif // !_WORD_DEFINED


#define _UPPER          0x1     /* upper case letter */
#define _LOWER          0x2     /* lower case letter */
#define _DIGIT          0x4     /* digit[0-9] */
#define _SPACE          0x8     /* tab, carriage return, newline, */
                                /* vertical tab or form feed */
#define _PUNCT          0x10    /* punctuation character */
#define _CONTROL        0x20    /* control character */
#define _BLANK          0x40    /* space char */
#define _HEX            0x80    /* hexadecimal digit */

#define _LEADBYTE       0x8000                  /* multibyte leadbyte */
#define _ALPHA          (0x0100|_UPPER|_LOWER)  /* alphabetic character */


//int ismbcdigit(int c)	{return isdigit(c);}
int iswdigit(wint_t c)	{return iswctype(c, _DIGIT);}

//int _ismbcspace(int c)	{return isspace(c);}
int iswspace(wint_t c)	{return iswctype(c, _BLANK);}

//int _ismbcupper(int c)	{return isupper(c);}
int isupper(int c)		{return (c >= 'A' && c <= 'Z');}
int iswupper(wint_t c)	{return iswctype(c, _UPPER);}

//int ismbcalpha(int c)	{return isalpha(c);}
int isalpha(int c)		{return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');}
int iswalpha(wint_t c)	{return iswctype(c, _ALPHA);}


//int ismbcxdigit(int c)	{return isxdigit(c);}
int isxdigit(int c)		{return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');}
int iswxdigit(wint_t c)	{return iswctype(c, _HEX);}

//int ismbcalnum(int c)	{return isalnum(c);}
int isalnum(int c)		{return isalpha(c) || isdigit(c);}
int iswalnum(wint_t c)	{return iswctype(c, _ALPHA|_DIGIT);}

//int ismbcprint(int c)	{return isprint(c);}
int isprint(int c)		{return c >= ' ';}
int iswprint(wint_t c)	{return iswctype(c, (wctype_t)(~_CONTROL));}

int iswctype(wint_t c, wctype_t type)
{
	WORD ret =0;
    GetStringTypeW(CT_CTYPE1, (LPCWSTR)&c, 1, &ret);
	if ((ret & type) != 0)
		return 1;
	return 0;
}
#endif

//END_EXTERN_C