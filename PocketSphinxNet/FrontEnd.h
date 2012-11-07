#pragma once;

#include "pocketsphinx.h"
#include "CommandLine.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class FrontEnd
	{
	public:

		static array<ArgStruct^>^ GetArgs()
		{
			List<ArgStruct^>^ arglist = gcnew List<ArgStruct^>();

			const arg_t* args = fe_get_args();

			while(args!=0 && args->name!=0)
			{
				arglist->Add(gcnew ArgStruct(args));
				args++;
			}

			return arglist->Count>0 ? arglist->ToArray() : nullptr;

		}

		static void Free2D(void* array_data)
		{
			if(array_data!=0)
			{
				fe_free_2d(array_data);
			}
		}


	internal:

		fe_t* fe;

	public:

		FrontEnd()
		{
			this->fe = 0;
		}
		~FrontEnd()
		{
			this->Free();
		}

	internal:

		FrontEnd(fe_t* fe)
		{
			this->fe = fe;
		}
		operator fe_t* ()
		{
			return this->fe;
		}

	public:


		bool Free()
		{
			bool done = true;
			if(this->fe !=0)
			{
				done = fe_free(this->fe) == 0;
				if(done)
				{
					this->fe = 0;
				}
			}
			return done;
		}
		bool InitAuto()
		{
			this->Free();

			this->fe = fe_init_auto();
			
			return this->fe!=0;
		}
		bool InitAuto(CommandLine cmdln)
		{
			this->Free();

			this->fe = fe_init_auto_r(cmdln!=nullptr ? (cmd_ln_t*)cmdln :0);
			
			return this->fe!=0;
		}


		CommandLine^ GetConfig()
		{
			if(this->fe!=0)
			{
				return gcnew CommandLine((cmd_ln_t*)fe_get_config(this->fe));
			}
			return nullptr;
		}


		int GetOutputSize()
		{
			if(this->fe!=0)
			{
				return fe_get_output_size(this->fe);
			}
			return 0;
		}


		bool GetInputSize(int% out_frame_shift, int% out_frame_size)
		{
			if(this->fe!=0)
			{
				int ofshift = 0, ofsize=0;

				fe_get_input_size(this->fe, &ofshift, & ofsize);

				out_frame_shift = ofshift;
				out_frame_size = ofsize;

				return true;
			}
			return false;
		}

		bool StartUtt()
		{
			if(this->fe!=0)
			{
				return fe_start_utt(this->fe) == 0;

			}
			return false;
		}

		bool EndUtt(float* out_cepvector, int% out_nframes)
		{
			bool done = false;
			if(this->fe!=0)
			{
				int on = out_nframes;

				done = fe_end_utt(this->fe, out_cepvector, &on) == 0;

				out_nframes = on;
			}
			return done;
		}
		


		bool ProcessFrame(array<short>^ speech_data, array<float>^ out_cep)
		{
			int r = -1;
			if(this->fe!=0)
			{
				pin_ptr<short> psd = &speech_data[0];
				pin_ptr<float> ocp = &out_cep[0];


				r = fe_process_frame(this->fe,psd, speech_data->Length,ocp); 
								
			}
			return r == 0;
		}
		int ProcessFrames(const short** inout_spch, array<size_t>^ inout_nsamps, float** buf_cep,int% inout_nframes)
		{
			int r = -1;
			if(this->fe!=0)
			{
				pin_ptr<size_t> ns = &inout_nsamps[0];

				int ionf;
				r = fe_process_frames(this->fe,inout_spch,ns,buf_cep,&ionf); 

				inout_nframes = ionf;
			}
			return 0;
		}

		bool ProcessUtt(array<short>^ speech_data, float*** out_cep, int% nframes)
		{
			int r = -1;
			if(this->fe!=0)
			{
				pin_ptr<short> psd = &speech_data[0];

				int nf = nframes;

				r = fe_process_utt(this->fe,psd, speech_data->Length,out_cep, &nf); 
				
				nframes = nf;
			}
			return r == 0;
		}


		int MfccToFloat(float** input, float** output, int nframes)
		{
			if(this->fe!=0)
			{
				return fe_mfcc_to_float(this->fe,input, output,nframes);
			}
			return 0;
		}
		int FloatToMfcc(float** input, float** output, int nframes)
		{
			if(this->fe!=0)
			{
				return fe_float_to_mfcc(this->fe,input, output,nframes);
			}
			return 0;
		}

		int LogspecToMfcc(float* fr_spec, float* fr_cep)
		{
			if(this->fe!=0)
			{
				return fe_logspec_to_mfcc(this->fe,fr_spec, fr_cep);
			}
			return 0;
		}

		int LogspecDCT2(float* fr_spec, float* fr_cep)
		{
			if(this->fe!=0)
			{
				return fe_logspec_dct2(this->fe,fr_spec, fr_cep);
			}
			return 0;
		}
		int MfccDCT3(float * fr_cep, float* fr_spec)
		{
			if(this->fe!=0)
			{
				return fe_mfcc_dct3(this->fe, fr_cep, fr_spec);
			}
			return 0;
		}


	};
}