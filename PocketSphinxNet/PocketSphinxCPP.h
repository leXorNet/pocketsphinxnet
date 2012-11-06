#pragma once;

#include "pocketsphinx.h"
#include "CommandLine.h"
#include "ArgStruct.h"
#include "LogMath.h"
#include "FrontEnd.h"
#include "FeatureComputation.h"
#include "FeatureTrans.h"
#include "NGramModel.h"
#include "FiniteStateGrammar.h"
#include "Lattice.h"
#include "Segment.h"
#include "NBest.h"
#include "PitchEstimator.h"
#include "Agc.h"
#include "Cmn.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

namespace PocketSphinxNet
{

	public ref class PocketSphinx
	{
	 internal:
		ps_decoder_t* decoder;


	public:
		PocketSphinx()
		{
			this->decoder = 0;
		}

		~PocketSphinx()
		{
			if(this->decoder == 0)
			{
				ps_free(this->decoder);
			}
		}

	internal:
		operator ps_decoder_t* ()
		{
			return this->decoder;
		}
		PocketSphinx(ps_decoder_t* decoder)
		{
			this->decoder = decoder;
		}

	public:


		bool Create(CommandLine^ Config)
		{
			if(this->decoder == 0)
			{
				return (this->decoder = ps_init(Config))!=0;
			}
			else 
			{
				return ps_reinit(this->decoder,Config)==0;

			}
		}


		CommandLine^ GetConfig()
		{
			if(this->decoder!=0)
			{
				return gcnew CommandLine(ps_get_config(this->decoder));
			}
			return nullptr;
		}

		List<ArgStruct^>^ GetArgs()
		{
			List<ArgStruct^>^ Args  = gcnew List<ArgStruct^>(ps_args_size());

			for(int i = 0;i<Args->Count;i++)
			{
				Args->Add(gcnew ArgStruct((ps_args()+i)));
			}
			return Args;
		}

		LogMath^ GetLogMath()
		{
			if(this->decoder!=0)
			{
				return gcnew LogMath(ps_get_logmath(this->decoder));
			}
			return nullptr;
		}

		FrontEnd^ GetFrontEnd()
		{
			if(this->decoder!=0)
			{
				return gcnew FrontEnd(ps_get_fe(this->decoder));
			}
			return nullptr;
		}

		FeatureComputation^ GetFeatureComputation()
		{
			if(this->decoder!=0)
			{
				return gcnew FeatureComputation(ps_get_feat(this->decoder));
			}
			return nullptr;
		}

		NGramModel^ GetLMSet()
		{
			if(this->decoder!=0)
			{
				return gcnew NGramModel(ps_get_lmset(this->decoder));
			}
			return nullptr;
		}

		NGramModel^ UpdateLMSet(NGramModel^ model)
		{
			if(this->decoder!=0)
			{
				return gcnew NGramModel(ps_update_lmset(this->decoder,model));
			}
			return nullptr;
		}


		FeatureTrans^ UpdateFeatureTrans(FeatureTrans^ feature)
		{
			if(this->decoder!=0)
			{
				return gcnew FeatureTrans(ps_update_mllr(this->decoder,feature));
			}
			return nullptr;
		}

		FiniteStateGrammar^ GetFiniteStateGrammarSet()
		{
			if(this->decoder!=0)
			{
				return gcnew FiniteStateGrammar(ps_get_fsgset(this->decoder));
			}
			return nullptr;
		}

		FiniteStateGrammar^ UpdateFiniteStateGrammarSet()
		{
			if(this->decoder!=0)
			{
				return gcnew FiniteStateGrammar(ps_get_fsgset(this->decoder));
			}
			return nullptr;
		}
	
		Lattice^ GetLattice()
		{
			if(this->decoder!=0)
			{
				return gcnew Lattice(ps_get_lattice(this->decoder));
			}
			return nullptr;
		}
				
		Segment^ GetSegment([Out]Int32% out_best_score)
		{
			Segment^ segment = nullptr;
			if(this->decoder!=0)
			{
				int n_out_best_score = 0;

				ps_seg_t* seg = ps_seg_iter(this->decoder,& n_out_best_score);

				if(seg!=0)
				{
					segment =  gcnew Segment(seg);
					out_best_score = n_out_best_score;
				}
			}
			return segment;
		}

		NBest^ GetNBest(int sf, int ef,String^ ctx1, String^ ctx2)
		{
			NBest^ nb = nullptr;
			if(this->decoder!=0)
			{
				array<IntPtr>^ ps =  gcnew array<IntPtr>(2)
				{ 
					ctx1!=nullptr ? Marshal::StringToHGlobalAnsi(ctx1): IntPtr::Zero,
					ctx2!=nullptr ? Marshal::StringToHGlobalAnsi(ctx2): IntPtr::Zero,
				};


				ps_nbest_t* pnb = ps_nbest(this->decoder,sf,ef,(char*)ps[0].ToPointer(),(char*)ps[1].ToPointer());

				
				for each (IntPtr var in ps)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}

				}

				if(pnb!=0)
				{
					nb =  gcnew NBest(pnb);
				}

			}
			return nb;
		}

		PocketSphinx^ Clone()
		{
			return this->decoder!=0 ? gcnew
				PocketSphinx(ps_retain(this->decoder))
				: nullptr;
		}

		bool LoadDict(String^ dictfile,String^ fdictfile, String^ format)
		{
			bool done = false;
			if(this->decoder!=0 && dictfile!=nullptr)
			{
				array<IntPtr>^ str_dictfiles =  gcnew array<IntPtr>(3)
				{ 
					Marshal::StringToHGlobalAnsi(dictfile), 
						fdictfile!=nullptr ? Marshal::StringToHGlobalAnsi(fdictfile): IntPtr::Zero,
						format!=nullptr ? Marshal::StringToHGlobalAnsi(format): IntPtr::Zero
				};

				done = ps_load_dict(this->decoder, 
					(char*)str_dictfiles[0].ToPointer(),
					(char*)str_dictfiles[1].ToPointer(),
					(char*)str_dictfiles[2].ToPointer())
					== 0;

				for each (IntPtr var in str_dictfiles)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}

				}

			}
			return done;
		}


		bool SaveDict(String^ dictfile, String^ format)
		{
			bool done = false;
			if(this->decoder!=0 && dictfile!=nullptr)
			{
				array<IntPtr>^ str_dictfiles =  gcnew array<IntPtr>(2)
				{ 
					Marshal::StringToHGlobalAnsi(dictfile), 
						format!=nullptr ? Marshal::StringToHGlobalAnsi(format): IntPtr::Zero
				};

				done = ps_save_dict(this->decoder, 
					(char*)str_dictfiles[0].ToPointer(),
					(char*)str_dictfiles[1].ToPointer()
					)
					== 0;

				for each (IntPtr var in str_dictfiles)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}

				}

			}
			return done;
		}


		int AddWord(String^ word, String^ phones,int update)
		{
			int ret = -1;
			if(this->decoder!=0 && word!=nullptr)
			{
				array<IntPtr>^ params =  gcnew array<IntPtr>(2)
				{ 
					Marshal::StringToHGlobalAnsi(word), 
						Marshal::StringToHGlobalAnsi(phones)
				};

				ret = ps_add_word(this->decoder,(char*)params[0].ToPointer(),(char*)params[1].ToPointer(),update);

				for each (IntPtr var in params)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}
				}
			}
			return ret;
		}

		int DecodeSenscr(String^ file,String^ uutid,long maxsamps)
		{
			int ret = -1;
			if(this->decoder!=0 && file!=nullptr)
			{
				array<IntPtr>^ str_dictfiles =  gcnew array<IntPtr>(2)
				{ 
					Marshal::StringToHGlobalAnsi(file), 
						uutid!=nullptr ? Marshal::StringToHGlobalAnsi(uutid): IntPtr::Zero
				};

				if(str_dictfiles[0]!=IntPtr::Zero)
				{
					FILE* file = fopen((const char*)str_dictfiles[0].ToPointer(),"rb");

					if(file!=0)
					{
						ret = ps_decode_raw(
							this->decoder, 
							file,
							(char*)str_dictfiles[1].ToPointer(),
							maxsamps
							);

						fclose(file);
					}
				}
				for each (IntPtr var in str_dictfiles)
				{
					if(var!=IntPtr::Zero)
					{
						Marshal::FreeHGlobal(var);
					}

				}
			}

			return ret;
		}
		int ProcessRaw(array<Int16>^ data,bool no_search,bool full_ut)
		{
			int ret = -1;
			if(this->decoder!=0 && data!=nullptr)
			{
				IntPtr buffer = Marshal::AllocHGlobal(data->Length*sizeof(Int16)) ;
				if(buffer!=IntPtr::Zero)
				{
					Marshal::Copy(data,0,buffer,data->Length);

					ret = ps_process_raw(this->decoder,(const int16*)buffer.ToPointer(),data->Length,no_search?1:0,full_ut?1:0);

					Marshal::FreeHGlobal(buffer);
				}
			}
			return ret;
		}

		int ProcessCep([In,Out]array<float>^ data,bool no_search,bool full_utt)
		{
			int ret = -1;

			if(this->decoder!=0 && data!=nullptr)
			{
				IntPtr buffer = Marshal::AllocHGlobal(data->Length*sizeof(float)) ;

				if(buffer!=IntPtr::Zero)
				{
					Marshal::Copy(data,0,buffer,data->Length);

					IntPtr ptrs = Marshal::AllocHGlobal(data->Length*sizeof(IntPtr)) ;

					if(ptrs!=IntPtr::Zero)
					{
						//TODO: put address of buffer items to ptrs
						mfcc_t* first = ((mfcc_t*)buffer.ToPointer());

						for(int i = 0;i<data->Length;i++)
						{
							*((mfcc_t**)ptrs.ToPointer()+i)=first;
							first +=sizeof(mfcc_t);
						}
						ret = ps_process_cep(this->decoder,(mfcc_t**)ptrs.ToPointer(),data->Length,no_search?1:0,full_utt?1:0);

						Marshal::FreeHGlobal(ptrs);
					}

					Marshal::Copy(buffer,data,0,data->Length);


					Marshal::FreeHGlobal(buffer);
				}
			}
			return ret;
		}

		bool StartUtt(String^ uttid)
		{
			bool done = false;
			if(this->decoder!=0)
			{
				uttid = uttid !=nullptr ? uttid : String::Empty;

				IntPtr str = Marshal::StringToHGlobalAnsi(uttid);
				// use str here for the ofstream filename
				if(str!=IntPtr::Zero)
				{
					done = ps_start_utt(this->decoder,(char*)str.ToPointer())==0;
				}
				Marshal::FreeHGlobal(str);
			}
			return done;
		}

		bool EndUtt()
		{
			if(this->decoder!=0)
			{
				return ps_end_utt(this->decoder)==0;

			}
			return false;
		}


		String^ GetUttId()
		{
			String^ uttid = nullptr;
			if(this->decoder!=0)
			{
				uttid = gcnew String(ps_get_uttid(this->decoder));
			}
			return uttid;
		}



		int GetFrameNumber()
		{
			if(this->decoder!=0)
			{
				return ps_get_n_frames(this->decoder);
			}
			return -1;
		}

		String^ GetHypothesis([Out]Int32% out_best_score,[Out]String^% out_uttid)
		{
			String^ best = nullptr;
						
			if(this->decoder!=0)
			{
				int n_out_best_score = 0;

				const char* uttid = 0;

				const char* nbest = ps_get_hyp(this->decoder,&n_out_best_score,&uttid);


				if(nbest!=0)
				{
					best = gcnew String(nbest);
					
				}

				if(uttid!=0)
				{
					out_uttid = gcnew String(uttid);;
				}
				else
				{
					out_uttid = nullptr;
				}

				out_best_score = n_out_best_score;
			}

			return best;
		}
		String^ GetHypothesisFinal([Out]Int32% out_is_final)
		{
			String^ best = nullptr;
			if(this->decoder!=0)
			{
				int n_out_is_final = 0;

				const char* uttid = 0;

				const char* nbest = ps_get_hyp_final(this->decoder,&n_out_is_final);


				if(nbest!=0)
				{
					best = gcnew String(nbest);
				}

				out_is_final = n_out_is_final;
			}

			return best;
		}

		int GetProb([Out]String^% out_uttid)
		{
			int ret = -1;
			if(this->decoder!=0)
			{
				const char* uttid = 0;

				ret = ps_get_prob(this->decoder,&uttid);

				if(uttid!=0)
				{
					out_uttid = gcnew String(uttid);
				}
			}
			return ret;
		}

		bool GetUttTime([Out]double% nspeech, [Out]double% ncpu, [Out]double% nwall)
		{
			if(this->decoder!=0)
			{
				double ns = 0.0,nc = 0.0,nw = 0.0;

				ps_get_utt_time(this->decoder, &ns,&nc,&nw);

				nspeech = ns;
				ncpu = nc;
				nwall = nw;
				return true;
			}
			return false;
		}
		bool GetAllTime([Out]double% nspeech, [Out]double% ncpu, [Out]double% nwall)
		{
			if(this->decoder!=0)
			{
				double ns = 0.0,nc = 0.0,nw = 0.0;

				ps_get_all_time(this->decoder, &ns,&nc,&nw);

				nspeech = ns;
				ncpu = nc;
				nwall = nw;
				return true;
			}
			return false;
		}
	};
}