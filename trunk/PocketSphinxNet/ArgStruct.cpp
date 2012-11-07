#pragma once

#include "stdafx.h"

#include "PocketSphinxCPP.h"

namespace PocketSphinxNet
{
	ArgStruct::ArgStruct(const arg_t* src)
	{
		if(src!=0)
		{
			this->name = src->name!=0 ? gcnew String(src->name) : nullptr;
			this->type = (ArgType)src->type;
			this->deflt = src->deflt!=0 ? gcnew String(src->deflt) : nullptr;
			this->doc = src->doc!=0 ? gcnew String(src->doc) : nullptr;
		}
	}

	arg_t ArgStruct::Generate()
	{
		arg_t ret = {0};

			ret.type = (int)this->type;
			ret.name =  this->name!=nullptr ? (char*) Marshal::StringToHGlobalAnsi(this->name).ToPointer() : 0;
			ret.deflt = this->name!=nullptr ?  (char*) Marshal::StringToHGlobalAnsi(this->deflt).ToPointer() : 0;
			ret.doc = this->name!=nullptr ?  (char*) Marshal::StringToHGlobalAnsi(this->doc).ToPointer() : 0;
		
		return ret;
	}


	void ArgStruct::Release(arg_t& arg)
	{		arg.type = 0;

		if(arg.name!=0)
		{
			Marshal::FreeHGlobal(IntPtr((void*)arg.name));
			arg.name = 0;
		}


		if(arg.deflt!=0)
		{
			Marshal::FreeHGlobal(IntPtr((void*)arg.deflt));
			arg.deflt = 0;
		}


		if(arg.doc!=0)
		{
			Marshal::FreeHGlobal(IntPtr((void*)arg.doc));
			arg.doc = 0;
		}

	}

}