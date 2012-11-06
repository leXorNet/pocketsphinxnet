#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{

	public ref class FeatureComputation
	{
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


	};
}