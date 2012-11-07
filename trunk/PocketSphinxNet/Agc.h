#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public enum class AgcType 
	{
		AGC_NONE = 0,
		AGC_MAX,
		AGC_EMAX,
		AGC_NOISE
	} ;	

	public ref class Agc
	{
	public:
		static AgcType FromString(String^ text)
		{
			AgcType agct = AgcType::AGC_NONE;

			if(text!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(text);

				if(str!=IntPtr::Zero)
				{
					agct = (AgcType)agc_type_from_str((char*)str.ToPointer());
				}
				Marshal::FreeHGlobal(str);
			}
			return agct;
		}
	internal:

		agc_t* agc;

	public:

		Agc()
		{
			this->agc = 0;
		}
		~Agc()
		{
			this->Free();
		}

	internal:

		Agc(agc_t* agc)
		{
			this->agc = agc;
		}
		operator agc_t* ()
		{
			return this->agc;
		}

	public:

		bool Init()
		{
			return ((this->agc = agc_init())!=0);
		}

		bool Free()
		{
			bool done = true;
			if(this->agc !=0)
			{
				agc_free(this->agc);
				this->agc = 0;
				return true;
			}
			return done;
		}

		
		float GetEmax()
		{
			if(this->agc!=0)
			{
				return agc_emax_get(this->agc);

			}
			return 0.0f;
		}

		bool SetEmax(float threshold)
		{
			if(this->agc!=0)
			{
				agc_emax_set(this->agc,threshold);
				return true;
			}
			return false;
		}
		bool UpdateEmax(float threshold)
		{
			if(this->agc!=0)
			{
				agc_emax_update(this->agc);
				return true;
			}
			return false;
		}


		bool Noice(float** mfcc, int n_frame)
		{
			if(this->agc!=0)
			{

				agc_noise(this->agc, mfcc, n_frame);

				return true;
			}
			return false;
		}
		bool Max(float** mfcc, int n_frame)
		{
			if(this->agc!=0)
			{

				agc_max(this->agc, mfcc, n_frame);

				return true;
			}
			return false;
		}
		bool EMax(float** mfcc, int n_frame)
		{
			if(this->agc!=0)
			{

				agc_emax(this->agc, mfcc, n_frame);

				return true;
			}
			return false;
		}
		float GetThreadshold()
		{
			if(this->agc!=0)
			{
				return agc_get_threshold(this->agc);

			}
			return 0.0f;
		}

		bool SetThreadshold(float threshold)
		{
			if(this->agc!=0)
			{
				agc_set_threshold(this->agc,threshold);
				return true;
			}
			return false;
		}


	};
}