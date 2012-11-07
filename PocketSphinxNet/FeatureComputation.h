#pragma once;

#include "pocketsphinx.h"
#include "Agc.h"
#include "Cmn.h"

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
			return p;
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

		String^ GetName()
		{
			if(this->feat!=0 && this->feat->name!=0)
			{
				return gcnew String(this->feat->name);
			}
			return String::Empty;
		}

		int GetCepSize()
		{
			if(this->feat!=0)
			{
				return this->feat->cepsize;
			}
			return 0;
		}
		int GetWindowSize()
		{
			if(this->feat!=0)
			{
				return this->feat->window_size;
			}
			return 0;
		}
		int GetStreamCount()
		{
			if(this->feat!=0)
			{
				return this->feat->n_stream;
			}
			return 0;
		}
		int GetStreamLength(int n)
		{
			if(this->feat!=0 && n>=0 && n<this->GetStreamCount())
			{
				return this->feat->stream_len[n];
			}
			return 0;
		}
		int GetDemision1()
		{
			if(this->feat!=0)
			{
				return this->feat->n_sv!=0 ? this->feat->n_sv : this->feat->n_stream;
			}
			return 0;
		}
		int GetDemision2(int n)
		{
			if(this->feat!=0)
			{
				if(this->feat->lda!=0)
				{
					return this->feat->out_dim;
				}
				else if(this->feat->sv_len!=0)
				{
					if(n>=0&&n<this->feat->n_sv)
					{
						return this->feat->sv_len[n];
					}
				}
				else 
				{
					if(n>=0 && n<this->feat->n_stream)
					{
						return this->feat->stream_len[n];
					}
				}
			}
			return 0;
		}

		int GetDemision()
		{
			if(this->feat!=0)
			{
				return this->feat->out_dim;
			}
			return 0;
		}

		bool Init(String^ type, CmnType cmn, int varnorm,  AgcType agc,int breport, int cepsize)
		{
			if(type!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(type);

				if(str!=IntPtr::Zero)
				{
					cmn_type_t ct = cmn_type_t::CMN_NONE;

					if(cmn == CmnType::CMN_CURRENT)
					{
						ct = cmn_type_t::CMN_CURRENT;
					}
					else if(cmn == CmnType::CMN_PRIOR)
					{
						ct = cmn_type_t::CMN_PRIOR;
					}
					else if(cmn == CmnType::CMN_NONE)
					{
						ct = cmn_type_t::CMN_NONE;
					}

					agc_type_t at = agc_type_t::AGC_NONE;

					if(agc == AgcType::AGC_EMAX)
					{
						at = agc_type_t::AGC_EMAX;
					}
					else if(agc == AgcType::AGC_MAX)
					{
						at = agc_type_t::AGC_MAX;
					}
					else if(agc == AgcType::AGC_NOISE)
					{
						at = agc_type_t::AGC_NOISE;
					}
					else if(agc == AgcType::AGC_NONE)
					{
						at = agc_type_t::AGC_NONE;
					}

					this->feat = feat_init((char*)str.ToPointer(), ct, varnorm, at, breport, cepsize);
					Marshal::FreeHGlobal(str);
				}

			}
			return this->feat!=0;
		}


		bool ReadLDA(String^ ldafile, int dim)
		{
			bool done = false;
			if(this->feat!=0)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(ldafile);

				if(str!=IntPtr::Zero)
				{
					done = feat_read_lda(this->feat,(char*)str.ToPointer(), dim) == 0;
					
					Marshal::FreeHGlobal(str);
				}
			}
			return done;
		}

		void LDATransform(float*** inout_feat,unsigned int nfr)
		{
			if(this->feat!=0)
			{
				feat_lda_transform(this->feat,inout_feat,nfr);
			}

		}
  

		int S2Mfc2Feat(String^ file, String^ dir, String^ cepext, int sf, int ef, float*** featarray, int maxfr)
		{
			int r = -1;
			if(this->feat!=0)
			{
				array<IntPtr>^ ps =  gcnew array<IntPtr>(3)
				{ 
					file!=nullptr ? Marshal::StringToHGlobalAnsi(file): IntPtr::Zero,
					dir!=nullptr ? Marshal::StringToHGlobalAnsi(dir): IntPtr::Zero,
					cepext!=nullptr ? Marshal::StringToHGlobalAnsi(cepext): IntPtr::Zero,
				};


				r = feat_s2mfc2feat(this->feat, (char*)ps[0].ToPointer(), (char*)ps[1].ToPointer(), (char*)ps[2].ToPointer(), sf,ef,featarray,maxfr);
				
				for each (IntPtr var in ps)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}

				}

			}
			return r;
		}
		int S2Mfc2Feat_Live(float** uttcep, int% inout_ncep, int beginutt, int endutt, float*** ofeat)
		{
			int ret = -1;
			if(this->feat!=0)
			{
				int inout_ncep_n = 0;

				ret = feat_s2mfc2feat_live(this->feat, uttcep, &inout_ncep_n, beginutt,endutt, ofeat);

				inout_ncep = inout_ncep_n;
			}
			return ret;
		}



	};
}