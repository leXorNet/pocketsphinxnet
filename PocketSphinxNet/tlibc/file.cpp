// file.cpp

// file routine overrides

// 08/20/05 (mv)

#ifndef POCKETSPHINX_NET

#include <windows.h>
#include <stdio.h>

typedef void *HANDLE;


#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86))
#define _W64 __w64
#else
#define _W64
#endif
#endif

#if defined(_WIN64)
typedef __int64 INT_PTR, *PINT_PTR;
typedef unsigned __int64 UINT_PTR, *PUINT_PTR;

typedef __int64 LONG_PTR, *PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;

#define __int3264   __int64

#else
typedef _W64 int INT_PTR, *PINT_PTR;
typedef _W64 unsigned int UINT_PTR, *PUINT_PTR;

typedef _W64 long LONG_PTR, *PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, *PULONG_PTR;

#define __int3264   __int32

#endif


#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
//
// Constants
//
#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

//
// Constants
//
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

/* FILE, as defined in stdio.h
struct _iobuf {
char *_ptr;
int   _cnt;
char *_base;				Used to store HANDLE
int   _flag;
int   _file;
int   _charbuf;
int   _bufsiz;
char *_tmpfname;
};
typedef struct _iobuf FILE;
*/


//
//  These are the generic rights.
//

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)


struct _FILE : public FILE
{
	void set_handle(HANDLE h) {_base = (char*)h;};
	HANDLE get_handle() const {return (HANDLE)_base;};
};

// used directly by the stdin, stdout, and stderr macros
_FILE __iob[3];
FILE *__iob_func() {return (FILE*)__iob;}

void _init_file()
{
	// STDIN
	__iob[0].set_handle(
		GetStdHandle(STD_INPUT_HANDLE)
		);
	__iob[0]._flag = _FILE_TEXT;

	// STDOUT
	__iob[1].set_handle(
		GetStdHandle(STD_OUTPUT_HANDLE)
		);
	__iob[1]._flag = _FILE_TEXT;

	// STDERR
	__iob[2].set_handle(
		GetStdHandle(STD_ERROR_HANDLE)
		);
	__iob[2]._flag = _FILE_TEXT;
}


//BEGIN_EXTERN_C

/*int _fileno(FILE *fp)
{
return (int)fp;			// FIXME:  This doesn't work under Win64
}

HANDLE _get_osfhandle(int i)
{
return (HANDLE)i;		// FIXME:  This doesn't work under Win64
}*/

FILE *fopen(const char *path, const char *attrs)
{
	DWORD access, disp;
	if (strchr(attrs, 'w'))
	{
		access = GENERIC_WRITE;
		disp = CREATE_ALWAYS;
	}
	else
	{
		access = GENERIC_READ;
		disp = OPEN_EXISTING;
	}

	HANDLE hFile = 0
		CreateFileA(path, access, 0, 0, disp, 0, 0)
		;
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	_FILE *file = (_FILE*)malloc(sizeof( _FILE));
	memset(file, 0, sizeof(_FILE));
	file->set_handle(hFile);

	if (strchr(attrs, 't'))
		file->_flag |= _FILE_TEXT;

	return file;
}

FILE *_wfopen(const wchar_t *path, const wchar_t *attrs)
{
	DWORD access, disp;
	if (wcschr(attrs, L'w'))
	{
		access = GENERIC_WRITE;
		disp = CREATE_ALWAYS;
	}
	else
	{
		access = GENERIC_READ;
		disp = OPEN_EXISTING;
	}

	HANDLE hFile = 0
		CreateFileW(path, access, 0, 0, disp, 0, 0)
		;
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	_FILE *file = (_FILE*)malloc(sizeof( _FILE));
	memset(file, 0, sizeof(_FILE));
	file->set_handle(hFile);

	if (wcschr(attrs, L't'))
		file->_flag |= _FILE_TEXT;

	return file;
}

int fprintf(FILE *fp, const char *s, ...)
{
	va_list args;
	va_start(args, s);

	char bfr[1024];
	int len = wvsprintfA(bfr, s, args);
	va_end(args);

	fwrite(bfr, len+1, sizeof(char), fp);
	return len;
	return 0;

}

int fwprintf(FILE *fp, const wchar_t *s, ...)
{
	va_list args;
	va_start(args, s);

	wchar_t bfr[1024];
	int len = wvsprintfW(bfr, s, args);

	va_end(args);

	char ansibfr[1024];
	WideCharToMultiByte(CP_ACP, 0, bfr, -1, ansibfr, sizeof(ansibfr), 0, 0);

	fwrite(ansibfr, len+1, sizeof(char), fp);
	return len;
	return 0;
}

FILE *stdin = 0; 
FILE *stdout= 0; 
FILE *stderr= 0; 

int fclose(FILE *fp)
{
	CloseHandle(((_FILE*)fp)->get_handle());
	free(fp);
	return 0;
}

int fflush(FILE* fp)
{

	return 0;
}


int feof(FILE *fp)
{
	return (fp->_flag & _FILE_EOF) ? 1 : 0;
}

int fseek(FILE *str, long offset, int origin)
{
	DWORD meth = FILE_BEGIN;
	if (origin == SEEK_CUR)
		meth = FILE_CURRENT;
	else if (origin == SEEK_END)
		meth = FILE_END;
	SetFilePointer(((_FILE*)str)->get_handle(), offset, 0, meth);
	((_FILE*)str)->_flag &= ~_FILE_EOF;
	return 0;
}

long ftell(FILE *fp)
{
	return SetFilePointer(((_FILE*)fp)->get_handle(), 0, 0, FILE_CURRENT)
		;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *str)
{
	if (size*count == 0)
		return 0;
	if (feof(str))
		return 0;

	HANDLE hFile = ((_FILE*)str)->get_handle();
	int textMode = ((_FILE*)str)->_flag & _FILE_TEXT;

	char *src;
	if (textMode)
		src = (char*)malloc(size*count);
	else
		src = (char*)buffer;

	DWORD br;
	if ( !ReadFile(hFile, src, (DWORD)(size*count), &br, 0)
		)
		((_FILE*)str)->_flag |= _FILE_ERROR;
	else if (!br)		// nonzero return value and no bytes read = EOF
		((_FILE*)str)->_flag |= _FILE_EOF;

	if (!br)
		return 0;

	// Text-mode translation is always ANSI
	if (textMode)		// text mode: must translate CR -> LF
	{
		char *dst = (char*)buffer;
		for (DWORD i = 0; i < br; i++)
		{
			if (src[i] != '\r')
			{
				*dst++ = src[i];
				continue;
			}

			// If next char is LF -> convert CR to LF
			if (i+1 < br)
			{
				if (src[i+1] == '\n')
				{
					*dst++ = '\n';
					i++;
				}
				else
					*dst++ = src[i];
			}
			else if (br > 1)
			{
				// This is the hard part: must peek ahead one byte
				DWORD br2 = 0;
				char peekChar = ReadFile(hFile, &peekChar, 1, &br2, 0);
				if (!br2)
					*dst++ = src[i];
				else if (peekChar == '\n')
					*dst++ = '\n';
				else
				{
					fseek(str, -1, SEEK_CUR);
					*dst++ = src[i];
				}
			}
			else
				*dst++ = src[i];
		}

		free(src);
	}

	return br/size;
}

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *str)
{
	DWORD bw = 0, bw2 = 0;

	if (size*count == 0)
		return 0;

	HANDLE hFile = ((_FILE*)str)->get_handle();
	int textMode = ((_FILE*)str)->_flag & _FILE_TEXT;

	// Text-mode translation is always ANSI!
	if (textMode)			// text mode -> translate LF -> CRLF
	{
		const char *src = (const char*)buffer;
		size_t startpos = 0, i = 0;
		for (i = 0; i < size*count; i++)
		{
			if (src[i] != '\n')
				continue;
			if (i > 0 && src[i-1] == '\r')		// don't translate CRLF
				continue;

			if (i > startpos)
			{

				WriteFile(hFile, &src[startpos], i-startpos, &bw2, 0);
				bw += bw2;
			}

			const char *crlf = "\r\n";

			WriteFile(hFile, crlf, 2, &bw2, 0);
			bw++;		// one '\n' written

			startpos = i+1;
		}

		if (i > startpos)
		{

			WriteFile(hFile, &src[startpos], i-startpos, &bw2, 0);
			bw += bw2;
		}
	}
	else
	{

		WriteFile(hFile, buffer, (DWORD)(size*count), &bw, 0);
	}
	return bw/size;
}

char *fgets(char *str, int n, FILE *s)
{
	if (feof(s))
		return 0;

	int i;
	for (i = 0; i < n-1; i++)
	{
		if (!fread(&str[i], 1, sizeof(char), s))
			break;
		if (str[i] == '\r')
		{
			i--;
			continue;
		}
		if (str[i] == '\n')
		{
			i++;
			break;
		}
	}

	str[i] = 0;
	return str;
}


int fgetc(FILE *s)
{
	if (s == 0 || feof(s))
		return EOF;

	char c;
	fread(&c, 1, sizeof(char), s);

	return (int)c;
}

#else
#include "libct.h"


using namespace System::Runtime::InteropServices;
#endif

int unlink( const char * pathname )
{
	if(pathname!=0)
	{
		File::Delete(gcnew String(pathname));
	}

	return 0;
}
int _mkdir(
	const char *dirname 
	)
{
	if(dirname!=0)
	{
		Directory::CreateDirectory(gcnew String(dirname));
	}

	return 0;
}


FILE *fopen(const char *path, const char *attrs)
{
	if(path!=0)
	{
		FileMode mode = FileMode::Open;
		FileAccess acc = FileAccess::Read;

		if (strchr(attrs, 'w'))
		{
			mode = FileMode::CreateNew;
		}
		else if (strchr(attrs, 'r'))
		{
			mode = FileMode::Open;
		}
		else if (strchr(attrs, 'a'))
		{
			mode = FileMode::Append;
			
		}
		if (strchr(attrs, '+'))
		{
			acc = FileAccess::ReadWrite;
		}
		else
		{
			acc = FileAccess::Write;

		}

		FileStream^ fs = gcnew FileStream(gcnew String(path),mode,acc);
		if(fs!=nullptr)
		{
			_FILE *file = (_FILE*)malloc(sizeof( _FILE));

			memset(file, 0, sizeof(_FILE));

			file->file =  GCHandle::Alloc(fs, GCHandleType::Normal);

			return file;
		}


	}
	return 0;
}
int getc(FILE *stream)
{
	int r = 0;
	if(stream!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)stream)->file.Target);

		if(fs!=nullptr)
		{
			r = fs->ReadByte();
		}

	}
	return r;
}
int fgetc(FILE *s){
	return getc(s);
}

int fputc(
	int c,
	FILE *stream 
	)
{
	if(stream!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)stream)->file.Target);
		if(fs!=nullptr)
		{
			fs->WriteByte((unsigned char)c);
		}

	}
	return 0;
}
int ungetc(
	int c,
	FILE *stream 
	)
{
	if(stream!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)stream)->file.Target);
		if(fs!=nullptr)
		{
			if(fs->Position>0)
			{
				fs->Position--;

				return c;
			}
		}
	}
	return EOF;
}

void rewind(FILE *stream)
{
	if(stream!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)stream)->file.Target);
		if(fs!=nullptr)
		{
			fs->Seek(0,SeekOrigin::Begin);
		}

	}
}

int fclose(FILE *fp){
	if(fp!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr)
		{
			fs->Close();
			((_FILE*)fp)->file.Free();

		}
		free(fp);
	}
	return 0;
}

int fflush(FILE *fp){
	if(fp!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr)
		{
			fs->Flush();
		}

	}
	return 0;
}
int feof(FILE *fp){
	if(fp!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr)
		{
			//NOTICE: check
			return fs->Position == fs->Length;
		}
	}
	return 0;
}
int fseek(FILE *fp, long long offset, int origin){
	if(fp!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr)
		{
			SeekOrigin so =SeekOrigin::Begin;

			switch(origin)
			{
				case SEEK_SET:
					so = SeekOrigin::Begin;
					break;
				case SEEK_CUR:
					so = SeekOrigin::Current;
					break;
				case SEEK_END:
					so = SeekOrigin::End;
					break;
			}
			fs->Seek(offset, so);
		}

	}
	return 0;
}
long long ftell(FILE *fp){
	if(fp!=0)
	{
		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr)
		{
			//NOTICE:check
			return fs->Position;
		}

	}
	return 0;
}
size_t fread(void *buffer, size_t size, size_t count, FILE *fp){

	if(buffer!=0 && fp!=0)
	{
		size_t n = size*count;

		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr && n>0)
		{

			array<unsigned char>^ _buffer = gcnew array<unsigned char>(n);

			int c = 0;
			if((c=fs->Read(_buffer,0,n)) >0)
			{
				Marshal::Copy(_buffer,0,IntPtr(buffer),c);
			}
			return c;
		}

	}
	return 0;
}
size_t fwrite(const void *buffer, size_t size, size_t count, FILE *fp){
	if(buffer!=0 && fp!=0)
	{
		size_t n = size*count;

		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);
		if(fs!=nullptr && n>0)
		{

			array<unsigned char>^ _buffer = gcnew array<unsigned char>(n);

			Marshal::Copy(IntPtr((void*)buffer),_buffer,0,n);

			fs->Write(_buffer,0,n);

			return n;
		}

	}
	return 0;
}
char *fgets(char *str, int n, FILE *fp){

	if (feof(fp))
	{
		return 0;
	}
	else
	{
		int i;
		for (i = 0; i < n-1; i++)
		{
			if (!fread(&str[i], 1, sizeof(char), fp))
				break;
			if (str[i] == '\r')
			{
				i--;
				continue;
			}
			if (str[i] == '\n')
			{
				i++;
				break;
			}
		}

		str[i] = 0;
		return str;
	}
}

int ferror( 
	FILE *stream 
	)
{
	//DO NOTHING
	return 0;
}
int clearerr( 
	FILE *stream 
	)
{
	//DO NOTHING
	return 0;
}
