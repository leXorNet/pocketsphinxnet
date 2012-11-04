// crt0twin.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

//#include <windows.h>
#include "libct.h"

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:libcmtd.lib")

//
// Dual Mode API below this line. Dual Mode Structures also included.
//

#define STARTF_USESHOWWINDOW       0x00000001
#define STARTF_USESIZE             0x00000002
#define STARTF_USEPOSITION         0x00000004
#define STARTF_USECOUNTCHARS       0x00000008
#define STARTF_USEFILLATTRIBUTE    0x00000010
#define STARTF_RUNFULLSCREEN       0x00000020  // ignored for non-x86 platforms
#define STARTF_FORCEONFEEDBACK     0x00000040
#define STARTF_FORCEOFFFEEDBACK    0x00000080
#define STARTF_USESTDHANDLES       0x00000100

int __argc;
TCHAR **__targv;

#ifdef UNICODE
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);
#endif

//EXTERN_C
extern "C" void _tWinMainCRTStartup()
{
	__argc = _init_args();
	__targv = _argv;
	_init_file();

	TCHAR *cmd = 0
		//NOTICE:BIG_TODO GetCommandLine()
		;

	// Skip program name
	if (*cmd == _T('"'))
	{
	    while (*cmd && *cmd != _T('"'))
	        cmd++;
	    if (*cmd == _T('"'))
	        cmd++;
	}
	else
	{
	    while (*cmd > _T(' '))
	        cmd++;
	}

	// Skip any white space
	while (*cmd && *cmd <= _T(' '))
	    cmd++;
//NOTICE:BIG_TODO 
	//STARTUPINFO si = {0};
	//si.dwFlags = 0;
	//GetStartupInfo(&si);

	_init_atexit();
	_initterm(__xc_a, __xc_z);			// call C++ constructors

	//NOTICE:BIG_TODO int ret = _tWinMain(GetModuleHandle(0), 0, cmd, si.dwFlags&STARTF_USESHOWWINDOW ? si.wShowWindow : SW_SHOWDEFAULT);

	_doexit();
	_term_args();
	//NOTICE:BIG_TODO ExitProcess(ret);
}