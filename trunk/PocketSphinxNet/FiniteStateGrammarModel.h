#pragma once;

#include "pocketsphinx.h"

#include "sphinxbase/fsg_model.h"
#include "LogMath.h"

using namespace System;
using namespace System::Runtime::InteropServices;
namespace PocketSphinxNet
{
	public ref class FiniteStateGrammarModel
	{
	internal:

		fsg_model_t* fsg;

	public:

		FiniteStateGrammarModel()
		{
			this->fsg = 0;
		}
		~FiniteStateGrammarModel()
		{
			this->Free();
		}

	internal:

		FiniteStateGrammarModel(fsg_model_t* fsg)
		{
			this->fsg = fsg;
		}


		operator fsg_model_t* ()
		{
			return this->fsg;
		}


	public:



		FiniteStateGrammarModel^ Clone()
		{
			if(this->fsg!=0)
			{
				fsg_model_t* pc = fsg_model_retain(this->fsg);

				if(pc!=0)
				{
					return gcnew FiniteStateGrammarModel(pc);
				}
			}
			return nullptr;
		}


		bool Free()
		{
			bool done = true;
			if(this->fsg !=0)
			{
				fsg_model_free(this->fsg);

				this->fsg = 0;
			}
			return done;
		}


		bool Init(String^ name, LogMath^ logmath, float lw, int n_state)
		{
			this->Free();

			


			return false;
		}
	};
}