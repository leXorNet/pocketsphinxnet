#pragma once;

#include "pocketsphinx.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
    ref class PocketSphinx;
	public ref class LatticeIteratorNode
	{
	internal:
		ps_latnode_t* node;
		ps_lattice_t* lattice;

		LatticeIteratorNode(ps_lattice_t* lattice,ps_latnode_t* node)
		{
			this->lattice = lattice;
			this->node = node;
		}


	};
	public ref class LatticeIterator
	{
	internal:
		ps_latnode_iter_t* iterator;

	};

	public ref class Lattice
	{
	internal:

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

		bool Read(PocketSphinx^ ps, String^ file);
		
		bool Write(String^ file)
		{
			if(this->lattice!=0 && file!=nullptr)
			{
				int r = 0;

				IntPtr str = Marshal::StringToHGlobalAnsi(file);

				if(str!=IntPtr::Zero)
				{
					r = ps_lattice_write(this->lattice,(char*)str.ToPointer());
				}
				Marshal::FreeHGlobal(str);

				return r!=0;
			}
			return false;
		}

		bool WriteHTK(String^ file)
		{
			if(this->lattice!=0 && file!=nullptr)
			{
				int r = 0;

				IntPtr str = Marshal::StringToHGlobalAnsi(file);

				if(str!=IntPtr::Zero)
				{
					r = ps_lattice_write_htk(this->lattice,(char*)str.ToPointer());
				}
				Marshal::FreeHGlobal(str);

				return r!=0;
			}
			return false;
		}

		LogMath^ GetLogMath()
		{
			if(this->lattice!=0)
			{
				logmath_t* lg = ps_lattice_get_logmath(this->lattice);

				if(lg!=0)
				{
					return gcnew LogMath(lg);
				}
			}
			return nullptr;
		}

		Lattice^ Clone()
		{
			if(this->lattice!=0)
			{
				ps_lattice_t* p = ps_lattice_retain(this->lattice);

				return p!=0 ? gcnew Lattice(p) : nullptr;
				
			}
			return nullptr;
		}


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