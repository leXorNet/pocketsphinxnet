#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;
namespace PocketSphinxNet
{
	public ref class LogMath
	{
	internal:

		logmath_t* logmath;

	public:

		LogMath()
		{
			this->logmath = 0;
		}
		~LogMath()
		{
			this->Free();
		}

	internal:

		LogMath(logmath_t* logmath)
		{
			this->logmath = logmath;
		}


		operator logmath_t* ()
		{
			return this->logmath;
		}


	public:



		bool Init(float64 base, int shift, int use_table)
		{
			if(this->logmath == 0)
			{
				return (this->logmath = logmath_init(base,shift,use_table))!=0;
			}

			return false;
		}

		bool Free()
		{
			bool done = true;
			if(this->logmath !=0)
			{
				done = logmath_free(this->logmath) == 0;
				if(done)
				{
					this->logmath = 0;
				}
			}
			return done;
		}

		bool Init(String^ file)
		{
			bool done = false;
			if(this->logmath==0 && file!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(file);
				// use str here for the ofstream filename
				if(str!=IntPtr::Zero)
				{
					done = (this->logmath = logmath_read((const char*)(void*)str))!=0;
				}
				Marshal::FreeHGlobal(str);
			}
			return done;
		}

	};
}