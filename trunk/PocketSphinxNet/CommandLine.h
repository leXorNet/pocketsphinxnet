#pragma once

#include "cmdln_macro.h"

using namespace System;

using namespace System::Runtime::InteropServices;

namespace PocketSphinxNet
{
	public ref class CommandLine
	{
	public:

		static CommandLine^ TakeGlobalCommandLine()
		{
			return gcnew CommandLine(cmd_ln_get());
		}
	protected:

		cmd_ln_t* cmd ;


	public:
		CommandLine(void)
		{
			this->cmd = 0;
		}


		~CommandLine()
		{
			this->Free();
		}
	public:
		bool Init()
		{
			return ((this->cmd = cmd_ln_init(0,0,0))!=0);
		}
		bool Free()
		{
			bool done = true;
			if(this->cmd!=0)
			{
				done = cmd_ln_free_r(this->cmd) == 0;
				this->cmd = 0;
			}
			return done ;
		}


	public:

		bool SetString(String^ name,String^ value)
		{
			if(name!=nullptr && name!=nullptr)
			{
				char *n = (char*)Marshal::StringToHGlobalAnsi(name).ToPointer();
				char *v = (char*)Marshal::StringToHGlobalAnsi(value).ToPointer();

				cmd_ln_set_str_r(this->cmd,n,v);

				if(n!=0)
				{
					Marshal::FreeHGlobal(IntPtr(n));
				}
				if(v!=0)
				{
					Marshal::FreeHGlobal(IntPtr(v));
				}
				return true;
			}
			return false;
		}

	public:

		CommandLine^ Clone()
		{
			return this->cmd!=0 ? gcnew
				CommandLine(cmd_ln_retain(this->cmd))
				: nullptr;
		}

	internal:

		operator cmd_ln_t* ()
		{
			return this->cmd;
		}

		CommandLine(cmd_ln_t* cmd)
		{
			if((this->cmd = cmd)!=0)
			{
			}
		}

	};

}