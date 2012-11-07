#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{

	public ref class FeatureComputation
	{
	public:
		static int** ParseSubVector(String^ feature)
		{
			int** p = 0;
			if(!String::IsNullOrEmpty(feature))
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(feature);

				if(str!=IntPtr::Zero)
				{
					p = parse_subvecs((char*)str.ToPointer());
					
					Marshal::FreeHGlobal(str);
				}
			}
			return 0;
		}

		static void SubVectorFree(int** sv)
		{
			if(sv!=0)
			{
				subvecs_free(sv);
			}
		}

	internal:

		feat_t* feat;

	public:

		FeatureComputation()
		{
			this->feat = 0;
		}
		~FeatureComputation()
		{
			this->Free();
		}

	internal:

		FeatureComputation(feat_t* feat)
		{
			this->feat = feat;
		}
		operator feat_t* ()
		{
			return this->feat;
		}
	public:

		bool SetSubVector(int** sv)
		{
			if(this->feat!=0 && sv!=0)
			{
				return feat_set_subvecs(this->feat, sv)==0;
			}
			return false;
		}

		FeatureComputation^ Clone()
		{
			if(this->feat!=0)
			{
				feat_t* pc = feat_retain(this->feat);

				if(pc!=0)
				{
					return gcnew FeatureComputation(pc);
				}
			}
			return nullptr;
		}
		bool Free()
		{
			bool done = true;
			if(this->feat !=0)
			{
				done = feat_free(this->feat) == 0;
				if(done)
				{
					this->feat = 0;
				}
			}
			return done;
		}

		float*** ArrayAlloc(int nfr)
		{
			if(this->feat!=0)
			{
				return feat_array_alloc(this->feat,nfr);
			}
			return 0;
		}

		float*** ArrayRelloc(float*** oldfeat, int oldnfr,int nfr)
		{
			if(this->feat!=0)
			{
				return feat_array_realloc(this->feat,oldfeat,oldnfr, nfr);
			}
			return 0;
		}
		void ArrayFree(float*** featarray)
		{
			if(this->feat!=0)
			{
				feat_array_free(featarray);
			}

		}

		void Print(float*** featarray, int nfr, FILE* fp)
		{
			if(this->feat!=0)
			{
				feat_print(this->feat,featarray,nfr,fp);
			}
		}
		void Report()
		{
			if(this->feat!=0)
			{
				feat_report(this->feat);
			}
		}


	};
}