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
		
		LogMath^ Clone()
		{
			if(this->logmath!=0)
			{
				logmath_t* pc = logmath_retain(this->logmath);

				if(pc!=0)
				{
					return gcnew LogMath(pc);
				}
			}
			return nullptr;
		}
		bool Read(String^ file)
		{
			bool done = false;
			if(this->logmath==0 && file!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(file);
				// use str here for the ofstream filename
				if(str!=IntPtr::Zero)
				{
					done = (this->logmath = logmath_read((const char*)(void*)str))!=0;
					Marshal::FreeHGlobal(str);
				}
			}
			return done;
		}
		bool Write(String^ file)
		{
			bool done = false;
			if(this->logmath!=0 && file!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(file);
				// use str here for the ofstream filename
				if(str!=IntPtr::Zero)
				{
					done = logmath_write(this->logmath,(const char*)(void*)str)==0;
					Marshal::FreeHGlobal(str);
				}
			}
			return done;
		}

		double GetBase()
		{
			if(this->logmath!=0)
			{
				return logmath_get_base(this->logmath);
			}
			return 0.0;
		}
		double GetZero()
		{
			if(this->logmath!=0)
			{
				return logmath_get_zero(this->logmath);
			}
			return 0.0;
		}
		int GetWidth()
		{
			if(this->logmath!=0)
			{
				return logmath_get_width(this->logmath);
			}
			return 0;
		}
		int GetShift()
		{
			if(this->logmath!=0)
			{
				return logmath_get_shift(this->logmath);
			}
			return 0;
		}


		int GetTableShape(unsigned int% out_size, unsigned int% out_width, unsigned int% out_shift)
		{
			int r = 0;
			if(this->logmath!=0)
			{
				unsigned int os = 0, ow = 0, of = 0;

				r = logmath_get_table_shape(this->logmath, &os, &ow, &of);

				out_size = os;
				out_width = ow;
				out_shift = of;
			}

			return r;

		}


		int Log10ToLog(int log_p)
		{
			if(this->logmath!=0)
			{
				return logmath_log10_to_log(this->logmath,log_p);
			}
			return 0;
		}


		double LogToLog10(int logb_p)
		{
			if(this->logmath!=0)
			{
				return logmath_log_to_log10(this->logmath,logb_p);
			}
			return 0;
		}

		double LogToLn(int logb_p)
		{
			if(this->logmath!=0)
			{
				return logmath_log_to_ln(this->logmath,logb_p);
			}
			return 0;
		}
		int LnToLog(double log_p)
		{
			if(this->logmath!=0)
			{
				return logmath_ln_to_log(this->logmath,log_p);
			}
			return 0;
		}

		double Exp(int logb_p)
		{
			if(this->logmath!=0)
			{
				return logmath_exp(this->logmath,logb_p);
			}
			return 0;
		}
		int Log(double p)
		{
			if(this->logmath!=0)
			{
				return logmath_log(this->logmath,p);
			}
			return 0;
		}

		int LogAdd(int logb_p, int logb_q)
		{
			if(this->logmath!=0)
			{
				return logmath_add(this->logmath,logb_p,logb_q);
			}
			return 0;
		}
		int LogAddExact(int logb_p, int logb_q)
		{
			if(this->logmath!=0)
			{
				return logmath_add_exact(this->logmath,logb_p,logb_q);
			}
			return 0;
		}
	};
}