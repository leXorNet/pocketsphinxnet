#pragma once

#include "cmdln_macro.h"
#include "ArgStruct.h"

using namespace System;
using namespace System::Collections::Generic;

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
	internal:

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
		bool Init(array<ArgStruct^>^ Args, bool Strict)
		{
			if(Args!=nullptr)
			{
				arg_t* argslist = (arg_t*)malloc(sizeof(arg_t)*(Args->Length + 1));

				if(argslist!=0)
				{
					memset(&argslist[Args->Length],0,sizeof(arg_t));

					for(int i = 0;i<Args->Length;i++)
					{
						argslist[i] = Args[i]->Generate();
					}


					this->cmd = cmd_ln_init(0, argslist, Strict);

					for(int i = 0;i<Args->Length;i++)
					{
						ArgStruct::Release(argslist[i]);
					}


					free(argslist);
				}


			}
			return this->cmd!=0;
		}
		bool ParseFile(array<ArgStruct^>^ Args, String^ file, bool Strict)
		{
			if(Args!=nullptr && file!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(file);

				if(str!=IntPtr::Zero)
				{
					if(Args!=nullptr)
					{
						arg_t* argslist = (arg_t*)malloc(sizeof(arg_t)*(Args->Length + 1));

						if(argslist!=0)
						{
							memset(&argslist[Args->Length],0,sizeof(arg_t));

							for(int i = 0;i<Args->Length;i++)
							{
								argslist[i] = Args[i]->Generate();
							}


							this->cmd = cmd_ln_parse_file_r(0,argslist,(char*) str.ToPointer(),Strict);

							for(int i = 0;i<Args->Length;i++)
							{
								ArgStruct::Release(argslist[i]);
							}


							free(argslist);
						}


					}

				}
				Marshal::FreeHGlobal(str);
			}
			return this->cmd!=0;
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
		bool Exists(String^ name)
		{
			bool found = false;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					found = cmd_ln_exists_r(this->cmd,(char*) str.ToPointer())!=0;
				}
				Marshal::FreeHGlobal(str);
			}
			return found;
		}
		//Object^ Access(String^ name)
		//{
		//	Object^ found = nullptr;
		//	if(this->cmd!=0 && name!=nullptr)
		//	{
		//		IntPtr str = Marshal::StringToHGlobalAnsi(name);

		//		if(str!=IntPtr::Zero)
		//		{
		//			anytype_t* arg =cmd_ln_access_r(this->cmd,(char*) str.ToPointer());

		//			if(arg!=0)
		//			{
		//			
		//				//TODO: conver to object
		//			}

		//		}
		//		Marshal::FreeHGlobal(str);
		//	}
		//	return found;
		//}

		String^ GetString(String^ name)
		{
			String^ found = nullptr;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					found = gcnew String(cmd_ln_str_r(this->cmd, (char*)str.ToPointer()));

				}
				Marshal::FreeHGlobal(str);
			}
			return found;
		}

		List<String^>^ GetStringList(String^ name)
		{
			List<String^>^ texts = gcnew List<String^>();
			
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					const char** pl = cmd_ln_str_list_r(this->cmd, (char*)str.ToPointer());

					while(pl!=0)
					{
						texts->Add(gcnew String(*pl));

						pl ++;
					}

				}
				Marshal::FreeHGlobal(str);
			}
			return texts;
		}
		int GetInt(String^ name)
		{
			int ret = 0;
			String^ found = nullptr;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					found = gcnew String(cmd_ln_str_r(this->cmd, (char*)str.ToPointer()));

					if(Int32::TryParse(found, ret))
					{
						//OK
					}


				}
				Marshal::FreeHGlobal(str);
			}
			return ret;
		}
		double GetFloat(String^ name)
		{
			double ret = 0;
			String^ found = nullptr;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					found = gcnew String(cmd_ln_str_r(this->cmd, (char*)str.ToPointer()));

					if(Double::TryParse(found, ret))
					{
						//OK
					}
				}
				Marshal::FreeHGlobal(str);
			}
			return ret;
		}


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

		bool SetInt(String^ name,int value)
		{
			bool done = false;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					cmd_ln_set_int_r(this->cmd,(char*)str.ToPointer(),value);
					done = true;
				}
				Marshal::FreeHGlobal(str);
			}
			return done;
		}
		bool SetFloat(String^ name,double value)
		{
			bool done = false;
			if(this->cmd!=0 && name!=nullptr)
			{
				IntPtr str = Marshal::StringToHGlobalAnsi(name);

				if(str!=IntPtr::Zero)
				{
					cmd_ln_set_float_r(this->cmd,(char*)str.ToPointer(),value);
					done = true;

				}
				Marshal::FreeHGlobal(str);
			}
			return done;
		}


	public:

		CommandLine^ Clone()
		{
			if(this->cmd!=0)
			{
				cmd_ln_t* pc = cmd_ln_retain(this->cmd);

				if(pc!=0)
				{
					return gcnew CommandLine(pc);
				}
			}
			return nullptr;
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