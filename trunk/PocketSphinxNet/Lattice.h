#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class Lattice
	{
	protected:

		ps_lattice_t* lattice;

	public:

		Lattice()
		{
			this->lattice = 0;
		}
		~Lattice()
		{
			this->Free();
		}

	internal:

		Lattice(ps_lattice_t* lattice)
		{
			this->lattice = lattice;
		}
		operator ps_lattice_t* ()
		{
			return this->lattice;
		}

	public:


		bool Free()
		{
			bool done = true;
			if(this->lattice !=0)
			{
				done = ps_lattice_free(this->lattice) == 0;
				if(done)
				{
					this->lattice = 0;
				}
			}
			return done;
		}


	};
}