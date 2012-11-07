#pragma once;

#include "pocketsphinx.h"
#include "sphinxbase/yin.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class PitchEstimator
	{
	internal:

		yin_t* yin;

		int frame_size;

	public:

		PitchEstimator()
		{
			this->yin = 0;
			this->frame_size = 0;
		}
		~PitchEstimator()
		{
			this->Free();
		}

	internal:

		PitchEstimator(yin_t* yin)
		{
			this->yin = yin;
		}
		operator yin_t* ()
		{
			return this->yin;
		}

	public:

		bool Init(int frame_size, float search_threshold,
                float search_range, int smooth_window)
		{
			return (this->yin = yin_init(this->frame_size = frame_size,search_threshold,search_range,smooth_window))!=0;
		}


		bool Start()
		{
			if(this->yin!=0)
			{
				yin_start(this->yin);
				return true;
			}
			return false;
		}

		bool End()
		{
			if(this->yin!=0)
			{
				yin_end(this->yin);
				return true;
			}
			return false;
		}

		bool Write(array<short>^ frame)
		{
			if(this->yin!=0 && frame!=nullptr && frame->Length>=this->frame_size)
			{
				pin_ptr<short> p = &frame[0];

				yin_write(this->yin, p);

				return true;
			}
			return false;
		}
		bool Read(unsigned short% out_period, unsigned short% out_bestdiff)
		{
			if(this->yin!=0)
			{
				unsigned short op = 0;
				unsigned short ob = 0;

				int r = yin_read(this->yin, &op,&ob);

				if(r!=0)
				{
					out_period = op;
					out_bestdiff = ob;
					return true;
				}
			}
			return false;
		}
		bool Free()
		{
			bool done = true;
			if(this->yin !=0)
			{
				yin_free(this->yin);
				if(done)
				{
					this->yin = 0;
				}
			}
			return done;
		}


	};
}