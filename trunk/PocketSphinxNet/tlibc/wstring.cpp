// wstring.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)
// 08/13/06 (mv)

#ifndef POCKETSPHINX_NET
#include <windows.h>
#include <string.h>
#else
#include "libct.h"
using namespace System;
using namespace System::Runtime::InteropServices;
#endif

int _wcsicmp(const wchar_t *s1, const wchar_t *s2) 
{
	return wcsicmp(s1, s2);
}
int wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
	String^ ws1 = s1!=0 ? gcnew String(s1) : nullptr;
	String^ ws2 = s2!=0 ? gcnew String(s2) : nullptr;

	if(ws1!=nullptr&&ws2!=nullptr)
	{
		return String::Compare(ws1,ws2,true);
	}

	return _NLSCMPERROR;
}
#ifdef _DEBUG

int wcscmp(const wchar_t *s1, const wchar_t *s2)
{
	String^ ws1 = s1!=0 ? gcnew String(s1) : nullptr;
	String^ ws2 = s2!=0 ? gcnew String(s2) : nullptr;

	if(ws1!=nullptr&&ws2!=nullptr)
	{
		return String::Compare(ws1,ws2);
	}

	return _NLSCMPERROR;
}
size_t wcslen(const wchar_t *str)
{
	String^ ws1 = str!=0 ? gcnew String(str) : nullptr;

	if(ws1!=nullptr)
	{
		return ws1->Length;
	}
	return 0 ;
}

wchar_t *wcscat(wchar_t *dst, const wchar_t *src)
{
	if(dst!=0)
	{
		if(src!=0)
		{
			size_t ld = wcslen(dst);

			size_t len = wcslen(src);

			for(size_t i = 0;i<=len;i++) //with trailing zero
			{
				*(dst + ld + i) = *(src + i);
			}
		}
	}

	return dst;

}
wchar_t *wcscpy(wchar_t *dest, const wchar_t *src)
{
	if(dest!=0 && src!=0)
	{
		memcpy(dest,src,sizeof(wchar_t*) * wcslen(src));
	}
	return dest;
}
#endif

int wcsncmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	if (!n)
		return 0;

	for (size_t i = 0; i < n; i++)
	{
		if (!s1[i] || s1[i] != s2[i])
			return s1[i] - s2[i];
	}

	return _NLSCMPERROR;
}

int wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
	int l1 = s1!=0 ? wcslen(s1) : 0;
	int l2 = s2!=0 ? wcslen(s2) : 0;

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

wchar_t *wcsdup(const wchar_t *src)
{
	if (!src)
		return 0;
	wchar_t *wcscat(wchar_t *dst, const wchar_t *src);
	size_t wcslen(const wchar_t *str);
	wchar_t *dst = (wchar_t*)malloc((wcslen(src)+1)*sizeof(wchar_t));
	wcscpy(dst, src);
	return dst;
}

wchar_t *_wcsdup(const wchar_t *src)
{
	wchar_t *wcsdup(const wchar_t *src);
	return wcsdup(src);
}



wchar_t *wcsncpy(wchar_t *dest, const wchar_t *src, size_t n)
{
	memcpy(dest, src, n*sizeof(wchar_t));
	size_t wcslen(const wchar_t *str);
	size_t len = wcslen(src);
	if (n > len)
		memset(&dest[len], 0, (n-len)*sizeof(wchar_t));
	return dest;
}



const wchar_t *wcschr(const wchar_t *str, wchar_t ch)
{
	while (*str)
	{
		if (*str == ch)
			return str;
		str++;
	}
	return 0;
}

const wchar_t *wcsrchr(const wchar_t *str, wchar_t ch)
{
	size_t wcslen(const wchar_t *str);
	const wchar_t *end = str + wcslen(str) + 1;
	while (end != str)
	{
		end--;
		if (*end == ch)
			return end;
	}
	return 0;
}


const wchar_t *wcsstr(const wchar_t *str, const wchar_t *substr)
{
	size_t wcslen(const wchar_t *str);
	int str_len = wcslen(str);
	int substr_len = wcslen(substr);

	if (substr_len == 0)
		return str;
	if (str_len < substr_len)
		return 0;
	for (int i = 0; i < (int)(str_len-substr_len+1); i++)
	{
		int wcscmp(const wchar_t *s1, const wchar_t *s2);
		if (!wcscmp(&str[i], substr))
			return (const wchar_t *)(&str[i]);
	}
	return 0;
}

wchar_t *wcsncat(
   wchar_t *strDest,
   const wchar_t *strSource,
   size_t count 
)
{
	if(strDest!=0)
	{
		if(strSource!=0)
		{
			size_t ld = wcslen(strDest);

			size_t len = wcslen(strSource);

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
