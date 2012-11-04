// newdel.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

//#include <windows.h>
#include "libct.h"

void* operator new(size_t size)
{
//NOTICE:BIG_TODO 	
	return 0;
}
void operator delete(void *p)
{
//NOTICE:BIG_TODO 	
}
void operator delete(void *p,size_t size)
{
//NOTICE:BIG_TODO 	
}

void* operator new[](size_t size)
{
//NOTICE:BIG_TODO 	
	return 0;
}
void operator delete[](void *p)
{
//NOTICE:BIG_TODO 	

}
//
//
//
//void *operator new(unsigned int s)
//{
//    return 0
//		//NOTICE:BIG_TODO HeapAlloc(GetProcessHeap(), 0, s)
//		;
//}
//
//void operator delete(void *p)
//{
//    //NOTICE:BIG_TODO HeapFree(GetProcessHeap(), 0, p);
//}
//
//void operator delete[](void *p)
//{
//	//NOTICE:BIG_TODO HeapFree(GetProcessHeap(), 0, p);
//}
//
//void *operator new[](unsigned int s)
//{
//	return 0
//		//NOTICE:BIG_TODO HeapAlloc(GetProcessHeap(), 0, s)
//		;
//}