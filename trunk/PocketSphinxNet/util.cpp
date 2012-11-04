#include "libct.h"

using namespace System;
using namespace System::IO;


int net_fprintf(FILE* fp,String^ str,... array<Object^>^ args)
{
	if(fp!=0)
	{
		String^ result = String::Format(str,args);

		FileStream^ fs = (FileStream^)(((_FILE*)fp)->file.Target);

		if(fs!=nullptr)
		{

			char* ps = (char*)Marshal::StringToHGlobalAnsi(result).ToPointer();

			if(ps!=0)
			{
				size_t len = strlen(ps);

				array<unsigned char>^ buffer = gcnew array<unsigned char>(len);

				Marshal::Copy(IntPtr((void*)ps),buffer,0,len);

				fs->Write(buffer,0,len);

				
				Marshal::FreeHGlobal(IntPtr(ps));
			}
		}
	}
	return 0;
}

int net_fscanf_str(FILE* fp, size_t count, char* buffer)
{
	if(fp!=0)
	{
		return fread(buffer,sizeof(char),count,fp);
	}

	return 0;
}
int net_fscanf_int_space_sep(FILE* fp, bool skip_space, int* n)
{
//need check
	if(fp!=0)
	{
		char buffer[256] = {0};
		int k = 0;
		while(!feof(fp) && k <256)
		{
			char c = fgetc(fp);

			if(isdigit(c))
			{
				buffer[k++] = c;
			}
			else
			{
				if(!skip_space)
				{
					ungetc(c,fp);
				}
				if(n!=0)
				{
					*n = atoi(buffer);
					return 1;
				}
			}
		}
	}

	return 0;
}
int net_fscanf_float_space_sep(FILE* fp, bool skip_space, float* n)
{
//need check
	if(fp!=0)
	{
		char buffer[256] = {0};
		int k = 0;
		while(!feof(fp) && k <256)
		{
			char c = fgetc(fp);

			if(isdigit(c) || c == '.')
			{
				buffer[k++] = c;
			}
			else
			{
				if(!skip_space)
				{
					ungetc(c,fp);
				}
				if(n!=0)
				{
					*n = atof(buffer);
					return 1;
				}
			}
		}
	}

	return 0;
}

int net_sscanf_word(const char* src, char* buffer, int* length)
{
	if(src!=0)
	{
		int len = strlen(src);
		for(int i = 0;i<len;i++)
		{
			char c = src[i];

			if(isspace(c))
			{
				if(length!=0)
				{
					*length = i;
				}
				return 1;
			}
			else
			{
				buffer[i] = c;
			}

		}
	}
	return 0;
}
int net_sscanf_int(const char* src, int* val, int* length)
{
	int ret = 0;
	if(src!=0)
	{
		int len = strlen(src);
		char* buffer = (char*)calloc(len+1,sizeof(char));
		if(buffer!=0)
		{
			for(int i = 0;i<len;i++)
			{
				char c = src[i];

				if(!isdigit(c) && c!='.')
				{
					if(length!=0)
					{
						*length = i;
					}

					if(val!=0)
					{
						*val = atoi(buffer);
					}
					ret = 1;
					break;
				}
				else
				{
					buffer[i] = c;
				}

			}
		}
	}
	return ret;
}
