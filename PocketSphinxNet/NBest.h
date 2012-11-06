#pragma once;

#include "pocketsphinx.h"

#include "Segment.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class NBest
	{
	internal:

		ps_nbest_t* nb;

	public:

		NBest()
		{
			this->nb = 0;
		}
		~NBest()
		{
			this->Free();
		}

	internal:

		NBest(ps_nbest_t* fe)
		{
			this->nb = fe;
		}
		operator ps_nbest_t* ()
		{
			return this->nb;
		}

	public:


		bool Free()
		{
			bool done = true;
			if(this->nb !=0)
			{
				ps_nbest_free(this->nb);
				this->nb = 0;
				done = true;
			}
			return done;
		}

		Segment^ GetSegment([Out]Int32% out_score)
		{
			Segment^ segment = nullptr;
			if(this->nb!=0)
			{
				int n_out_score = 0;

				ps_seg_t* ns = ps_nbest_seg(this->nb,&n_out_score);
				if(ns!=0)
				{
					segment = gcnew Segment(ns);
					out_score = n_out_score;
				}
			}
			return segment;

		}

				
		String^ GetHypothesis([Out]Int32% out_score)
		{
			String^ hypo = nullptr;
			if(this->nb!=0)
			{
				int n_out_score = 0;

				const char* ns = ps_nbest_hyp(this->nb,&n_out_score);
				if(ns!=0)
				{
					hypo = gcnew String(ns);
					out_score = n_out_score;
				}
			}
			return hypo;

		}

				
		NBest^ Next()
		{
			if(this->nb!=0)
			{
				ps_nbest_t * nb = ps_nbest_next(this->nb);
				if(nb!=0)
				{
					return gcnew NBest(nb);
				}
			}
			return nullptr;
		}
		
	};
}