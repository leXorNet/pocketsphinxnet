#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public enum class CmnType 
	{
		CMN_NONE = 0,
		CMN_CURRENT,
		CMN_PRIOR
	} ;	

	public ref class Cmn
	{
	public:
	
	   static const int _CMN_WIN_HWM = 800;

	   static const int _CMN_WIN = 500;

		static CmnType FromString(String^ text)
		{
			CmnType cmn = CmnType::CMN_NONE;

			if(text!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(text);

				if(str!=IntPtr::Zero)
				{
					cmn = (CmnType)cmn_type_from_str((char*)str.ToPointer());
				}
				Marshal::FreeHGlobal(str);
			}
			return cmn;
		}
	internal:

		cmn_t* _cmn;

		int veclen;

	public:

		Cmn()
		{
			this->_cmn = 0;
			this->veclen = 0;
		}
		~Cmn()
		{
			this->Free();
		}

	internal:

		Cmn(cmn_t* cmn)
		{
			this->_cmn = cmn;
		}
		operator cmn_t* ()
		{
			return this->_cmn;
		}

	public:

		bool Init(int veclen)
		{
			return ((this->_cmn = cmn_init(this->veclen = veclen))!=0);
		}

		bool Free()
		{
			bool done = true;
			if(this->_cmn !=0)
			{
				cmn_free(this->_cmn);
				this->_cmn = 0;
				this->veclen = 0;
				return true;
			}
			return done;
		}

		bool PriorUpdate()
		{
			if(this->_cmn!=0)
			{
				cmn_prior_update(this->_cmn);
				return true;
			}
			return false;
		}


		bool GetPrior(array<float>^ vec)
		{
			if(this->_cmn!=0 && vec!=nullptr && vec->Length == this->veclen )
			{
				pin_ptr<float> p = &vec[0];

				cmn_prior_get(this->_cmn, p);

				return true;
			}
			return false;
		}

		bool SetPrior(array<float>^ vec)
		{
			if(this->_cmn!=0 && vec!=nullptr && vec->Length == this->veclen )
			{
				pin_ptr<float> p = &vec[0];

				cmn_prior_set(this->_cmn, p);

				return true;
			}
			return false;
		}


		bool DoCmn(float** incep, int varnorm,int n_frame)
		{
			if(this->_cmn!=0)
			{
				cmn(this->_cmn, incep,varnorm, n_frame);

				return true;
			}
			return false;
		}

		bool DoCmnPrior(float** incep, int varnorm,int n_frame)
		{
			if(this->_cmn!=0)
			{
				cmn_prior(this->_cmn, incep,varnorm, n_frame);

				return true;
			}
			return false;
		}
	};
}