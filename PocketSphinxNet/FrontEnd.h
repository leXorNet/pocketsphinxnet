#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class FrontEnd
	{
	protected:

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


	};
}