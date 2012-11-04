#pragma once

#undef  assert

#define _In_ 
#define _In_z_

#ifndef _CRT_WIDE
#define __CRT_WIDE(_String) L ## _String
#define _CRT_WIDE(_String) __CRT_WIDE(_String)
#endif


#ifdef  NDEBUG

#define assert(_Expression)     ((void)0)

#else

#ifdef  __cplusplus
extern "C" {
#endif

void _wassert(const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);

#ifdef  __cplusplus
}
#endif

#define assert(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#endif  /* NDEBUG */
