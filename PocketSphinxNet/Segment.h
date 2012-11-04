#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class Segment
	{
	protected:

		ps_seg_t* segment;

	public:

		Segment()
		{
			this->segment = 0;
		}
		~Segment()
		{
			this->Free();
		}

	internal:

		Segment(ps_seg_t* fe)
		{
			this->segment = fe;
		}
		operator ps_seg_t* ()
		{
			return this->segment;
		}

	public:


		bool Free()
		{
			bool done = true;
			if(this->segment !=0)
			{
				ps_seg_free(this->segment);
				if(done)
				{
					this->segment = 0;
				}
			}
			return done;
		}

		Segment^ Next()
		{
			if(this->segment!=0)
			{
				ps_seg_t * seg = ps_seg_next(this->segment);
				if(seg!=0)
				{
					return gcnew Segment(seg);
				}
			}
			return nullptr;
		}
				
		String^ GetWord()
		{
			String^ word = nullptr;
			if(this->segment!=0)
			{
				word = gcnew String(ps_seg_word(this->segment));
			}
			return word;
		}

		bool GetFrame([Out]Int32% sf, [Out]Int32% ef)
		{
			if(this->segment!=0)
			{
				int n_sf = 0,n_ef = 0;

				ps_seg_frames(this->segment,&n_sf,&n_ef);

				sf = n_sf;
				ef = n_ef;
					
				return true;
			}
			return false;
		}
				
		bool GetProb([Out]Int32% out_ascr, [Out]Int32% out_lscr, [Out]Int32% out_lback)
		{
			if(this->segment!=0)
			{
				int32 n_out_ascr = 0, n_out_lscr = 0, n_out_lback=0;

				ps_seg_prob(this->segment,&n_out_ascr,&n_out_lscr,&n_out_lback);

				out_ascr = n_out_ascr;
				out_lscr = n_out_lscr;
				out_lback = n_out_lback;
					
				return true;
			}
			return false;
		}

	};
}