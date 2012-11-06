// string.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)
// 03/24/07 (mv)	fix strnicmp function

#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <string.h>
#include <tchar.h>
#else
#include "libct.h"
using namespace System;
using namespace System::Runtime::InteropServices;
#endif

//BEGIN_EXTERN_C

int strcmpi(const char *s1, const char *s2)
{
	String^ ws1 = s1!=0 ? gcnew String(s1) : nullptr;
	String^ ws2 = s2!=0 ? gcnew String(s2) : nullptr;

	if(ws1!=nullptr&&ws2!=nullptr)
	{
		return String::Compare(ws1,ws2,true);
	}

	return _NLSCMPERROR;		;
}

int stricmp(const char *s1, const char *s2) 
{
	return _stricmp(s1, s2);
}
int _stricmp(const char *s1, const char *s2)
{
	return strcmpi(s1, s2);
}


int strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return 0;

	const unsigned char *p1 = (const unsigned char*)s1;
	const unsigned char *p2 = (const unsigned char*)s2;

	for (size_t i = 0; i < n; i++)
	{
		if (!p1[i] || p1[i] != p2[i])
			return p1[i] - p2[i];
	}

	return 0;
}

int strnicmp(const char *s1, const char *s2, size_t n)
{
	int l1 = s1!=0 ? strlen(s1) : 0;
	int l2 = s2!=0 ? strlen(s2) : 0;

	int k = Math::Min(l1,l2);

	k = Math::Min(k,(int)n);

	String^ ws1 = s1!=0 ? gcnew String(s1,0,k) : nullptr;
	String^ ws2 = s2!=0 ? gcnew String(s2,0,k) : nullptr;

	if(ws1!=nullptr&&ws2!=nullptr)
	{
		return String::Compare(ws1,ws2,true);
	}

	return _NLSCMPERROR;
}

char *strdup(const char *src)
{
	if (!src)
		return 0;

	char *dst = (char*)malloc(strlen(src)+1);
	strcpy(dst, src);
	return dst;
}

char *_strdup(const char *src)
{
	return strdup(src);
}


char *strncpy(char *dest, const char *src, size_t n)
{
	memcpy(dest, src, n);
	size_t len = strlen(src);
	if (n > len)
		memset(&dest[len], 0, n-len);
	return dest;
}



const char *strchr(const char *str, int ch)
{
	while (*str)
	{
		if (*str == ch)
			return str;
		str++;
	}
	return 0;
}

const char *strrchr(const char *str, int ch)
{
	const char *end = str + strlen(str) + 1;
	while (end != str)
	{
		end--;
		if (*end == ch)
			return end;
	}
	return 0;
}


#ifdef _DEBUG
char *strcat(char *dst, const char *src)
{
	if(dst!=0)
	{
		if(src!=0)
		{
			size_t ld = strlen(dst);

			size_t len = strlen(src);

			for(size_t i = 0;i<=len;i++) //with trailing zero
			{
				*(dst + ld + i) = *(src + i);
			}
		}
	}

	return dst;
		;
}
int strcmp(const char *s1, const char *s2)
{
	String^ ws1 = s1!=0 ? gcnew String(s1) : nullptr;
	String^ ws2 = s2!=0 ? gcnew String(s2) : nullptr;

	if(ws1!=nullptr&&ws2!=nullptr)
	{
		return String::Compare(ws1,ws2);
	}

	return _NLSCMPERROR;
		;
}
size_t strlen(const char *str)
{
	String^ ws1 = str!=0 ? gcnew String(str) : nullptr;

	if(ws1!=nullptr)
	{
		return ws1->Length;
	}
	return 0 ;
		;
}

char *strcpy(char *dest, const char *src)
{
	if(dest!=0 && src!=0)
	{
		memcpy(dest,src,sizeof(char) * strlen(src) + 1);
	}
	return dest;
		;
}


#endif

size_t strspn(
   const char *str,
   const char *strCharSet 
)
{
	if(str!=0)
	{
		String^ s = gcnew String(str);

		String^ p = strCharSet!=0 ? gcnew String(strCharSet): nullptr;

		if(p == nullptr)
		{
			return 0;
		}
		else
		{
			for(size_t i = 0;i<(size_t)s->Length;i++)
			{
				if( p->IndexOf(s[i])<0)
				{
					return i -1;
				}
			}
		}
		
	}
	return -1;
}
const char *strstr(const char *str, const char *substr)
{
	int str_len = strlen(str);
	int substr_len = strlen(substr);

	if (substr_len == 0)
		return str;
	if (str_len < substr_len)
		return 0;
	for (int i = 0; i < (int)(str_len-substr_len+1); i++)
	{
		if (!strcmp(&str[i], substr))
			return (const char *)(&str[i]);
	}
	return 0;
}


char *strncat(
   char *strDest,
   const char *strSource,
   size_t count 
)
{
	if(strDest!=0)
	{
		if(strSource!=0)
		{
			size_t ld = strlen(strDest);

			size_t len = strlen(strSource);

			size_t cnt = ld;

			for(size_t i = 0;i<=len;i++) //with trailing zero
			{

				if(cnt == count)
				{
					break;
				}				
				*(strDest + ld + i) = *(strSource + i);
				cnt ++;


			}
		}
	}

	return strDest;
		;
}



//END_EXTERN_C