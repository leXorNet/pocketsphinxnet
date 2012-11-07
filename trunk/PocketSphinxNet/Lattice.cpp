#pragma once

#include "stdafx.h"

#include "PocketSphinxCPP.h"
#include "Lattice.h"

namespace PocketSphinxNet
{

	bool Lattice::Read(PocketSphinx^ ps, String^ file)
	{
		if(ps!=nullptr && ps->decoder!=0 && file!=nullptr)
		{
			IntPtr str = Marshal::StringToHGlobalAnsi(file);

			if(str!=IntPtr::Zero)
			{
				this->lattice = ps_lattice_read(ps,(char*)str.ToPointer());
			}
			Marshal::FreeHGlobal(str);

			return this->lattice!=0;
		}
		return false;
	}
}

