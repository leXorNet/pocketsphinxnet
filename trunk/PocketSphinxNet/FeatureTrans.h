#pragma once;

#include "pocketsphinx.h"
#include "ps_mllr.h"

using namespace System;
using namespace System::Runtime::InteropServices;
namespace PocketSphinxNet
{
	public ref class FeatureTrans
	{
	protected:

		ps_mllr_t* mllr;

	public:

		FeatureTrans()
		{
			this->mllr = 0;
		}
		~FeatureTrans()
		{
			this->Free();
		}

	internal:

		FeatureTrans(ps_mllr_t* mllr)
		{
			this->mllr = mllr;
		}

		operator ps_mllr_t* ()
		{
			return this->mllr;
		}

	public:
		bool Init(String^ file)
		{
			bool done = false;
			if(this->mllr!=0 && file!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(file);
				// use str here for the ofstream filename
				if(str!=IntPtr::Zero)
				{
					done = (this->mllr = 0/* ps_mllr_read((const char*)(void*)str)*/)!=0;
				}
				Marshal::FreeHGlobal(str);
			}
			return done;
		}
		FeatureTrans^ Clone()
		{
			return this->mllr!=0 ? gcnew
				FeatureTrans(ps_mllr_retain(this->mllr))
				: nullptr;
		}
		bool Free()
		{
			bool done = true;
			if(this->mllr !=0)
			{
				done = ps_mllr_free(this->mllr) == 0;
				if(done)
				{
					this->mllr = 0;
				}
			}
			return done;
		}


	};
}