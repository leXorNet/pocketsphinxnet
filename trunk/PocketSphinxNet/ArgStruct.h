#pragma once

#include "pocketsphinx.h"

using namespace System;

namespace PocketSphinxNet
{
	public value class ArgStruct
	{
		String^ name;
		int type;
		String^ deflt;
		String^ doc;
	internal:

		ArgStruct(const arg_t* src)
		{
			if(src!=0)
			{
				this->name = gcnew String(src->name);
				this->type = src->type;
				this->deflt = gcnew String(src->deflt);
				this->doc = gcnew String(src->doc);
			}
		}

	};
}