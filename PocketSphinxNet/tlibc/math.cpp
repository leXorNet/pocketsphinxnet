// math.cpp

// math functions

// 08/20/05 (mv)
#ifndef POCKETSPHINX_NET
#include <windows.h>
#else
#include "libct.h"
using namespace System;
#endif

//BEGIN_EXTERN_C

#ifdef _DEBUG
double log(double x)
{
	return System::Math::Log(x);
}

double fabs(double x)
{
	return System::Math::Abs(x);
}
double sqrt(double x)
{
	return System::Math::Sqrt(x);
}
double sin(double x)
{
	return System::Math::Sin(x);
}
double cos(double x)
{
	return System::Math::Cos(x);
}

double log10(double x)
{
	return System::Math::Log10(x);
}

double floor(
   double x
)
{
	return System::Math::Floor(x);
}
double pow(double x,double y)
{
	return System::Math::Pow(x,y);
}

int abs(int n)
{
	return (n>0)?n:-n;
}
double abs(double n)
{
	return (n>0.0)?n:-n;
}
#endif
#ifndef POCKETSPHINX_NET
static int lastrand;
int _fltused = 0;
void srand(unsigned int seed)
{
	lastrand = seed;
}

int rand()
{
	return (((lastrand = lastrand * 214013L + 2531011L) >> 16) & 0x7FFF);
}
#endif
//void _CIacos()
//{
//}
//void _ftol2()
//{
//}
//__declspec(naked) void _CIacos()
//{
//	__asm
//	{
//		fld		st(0)
//		fld		st(0)
//		fmul
//		fld1
//		fsubr
//		fsqrt
//		fxch
//		fpatan
//		ret
//	}
//}
//
//__declspec(naked) void _ftol2()
//{
//	__asm
//	{
//		push        ebp
//		mov         ebp,esp
//		sub         esp,20h
//		and         esp,0FFFFFFF0h
//		fld         st(0)
//		fst         dword ptr [esp+18h]
//		fistp       qword ptr [esp+10h]
//		fild        qword ptr [esp+10h]
//		mov         edx,dword ptr [esp+18h]
//		mov         eax,dword ptr [esp+10h]
//		test        eax,eax
//		je          integer_QnaN_or_zero
//	arg_is_not_integer_QnaN:
//		fsubp       st(1),st
//		test        edx,edx
//		jns         positive
//		fstp        dword ptr [esp]
//		mov         ecx,dword ptr [esp]
//		xor         ecx,80000000h
//		add         ecx,7FFFFFFFh
//		adc         eax,0
//		mov         edx,dword ptr [esp+14h]
//		adc         edx,0
//		jmp         localexit
//	positive:
//		fstp        dword ptr [esp]
//		mov         ecx,dword ptr [esp]
//		add         ecx,7FFFFFFFh
//		sbb         eax,0
//		mov         edx,dword ptr [esp+14h]
//		sbb         edx,0
//		jmp         localexit
//	integer_QnaN_or_zero:
//		mov         edx,dword ptr [esp+14h]
//		test        edx,7FFFFFFFh
//		jne         arg_is_not_integer_QnaN
//		fstp        dword ptr [esp+18h]
//		fstp        dword ptr [esp+18h]
//	localexit:
//  		leave
//  		ret
//	}
//}

//END_EXTERN_C