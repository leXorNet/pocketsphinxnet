#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;
namespace PocketSphinxNet
{
	public ref class FiniteStateGrammarSet
	{
	internal:

		fsg_set_t* fsg;

	public:

		FiniteStateGrammarSet()
		{
			this->fsg = 0;
		}
		~FiniteStateGrammarSet()
		{
			this->Free();
		}

	internal:

		FiniteStateGrammarSet(fsg_set_t* fsg)
		{
			this->fsg = fsg;
		}


		operator fsg_set_t* ()
		{
			return this->fsg;
		}


	public:



		bool Free()
		{
			bool done = true;
			if(this->fsg !=0)
			{

				//don't free
				this->fsg = 0;
			}
			return done;
		}


	};
}