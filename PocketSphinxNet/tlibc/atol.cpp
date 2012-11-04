// atol.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

#ifndef POCKETSPHINX_NET
#include <stdlib.h>
#include <ctype.h>
#else
#include "libct.h"
using namespace System;
#endif

//BEGIN_EXTERN_C
float atof(const char *str)
{
#ifndef POCKETSPHINX_NET
	float f = 0.0f;
	
	sscanf(str,"%f",%f);

	return f;
#else
	if(str!=0)
	{
		String^ s = gcnew String(str);
		float f = 0.0f;
		
		bool done = System::Single::TryParse(s, f);

		if(done)
		{

		}
		return f;
	}
	
	return 0.0f;
#endif
}
long atol(const char *str)
{
    while (isspace(*str))			// skip whitespace
        ++str;

    int cur = *str++;
    int neg = cur;					// Save the negative sign, if it exists

    if (cur == '-' || cur == '+')
        cur = *str++;

    // While we have digits, add 'em up.

	long total = 0;
    while (isdigit(cur))
    {
        total = 10*total + (cur-'0');			// Add this digit to the total.
        cur = *str++;							// Do the next character.
    }

    // If we have a negative sign, convert the value.
    if (neg == '-')
        return -total;
    else
        return total;
}

int atoi(const char *str)
{
    return (int)atol(str);
}


#ifndef POCKETSPHINX_NET


long wtol(const wchar_t *str)
{
    while (iswspace(*str))			// skip whitespace
        ++str;

    wint_t cur = *str++;
    wint_t neg = cur;					// Save the negative sign, if it exists

    if (cur == L'-' || cur == L'+')
        cur = *str++;

    // While we have digits, add 'em up.

	long total = 0;
    while (iswdigit(cur))
    {
        total = 10*total + (cur-L'0');			// Add this digit to the total.
        cur = *str++;							// Do the next character.
    }

    // If we have a negative sign, convert the value.
    if (neg == L'-')
        return -total;
    else
        return total;
}

int wtoi(const wchar_t *str)
{
    return (int)wtol(str);
}
#endif
//END_EXTERN_C