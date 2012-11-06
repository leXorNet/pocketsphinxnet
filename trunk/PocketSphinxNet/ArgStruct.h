#pragma once

#include "pocketsphinx.h"

using namespace System;

namespace PocketSphinxNet
{
	public enum class ArgType
	{

		/**
		* @name Values for arg_t::type
		*/
		/* @{ */
		/**
		* Bit indicating a required argument.
		*/
		_ARG_REQUIRED = (1<<0),
		/**
		* Integer argument (optional).
		*/
		_ARG_INTEGER = (1<<1),
		/**
		* Floating point argument (optional).
		*/
		_ARG_FLOATING = (1<<2),
		/**
		* String argument (optional).
		*/
		_ARG_STRING  = (1<<3),
		/**
		* Boolean (true/false) argument (optional).
		*/
		_ARG_BOOLEAN  =(1<<4),
		/**
		* Boolean (true/false) argument (optional).
		*/
		_ARG_STRING_LIST = (1<<5),

		/**
		* Required integer argument.
		*/
		_REQARG_INTEGER = (_ARG_INTEGER | _ARG_REQUIRED),
		/**
		* Required floating point argument.
		*/
		_REQARG_FLOATING = (_ARG_FLOATING | _ARG_REQUIRED),
		/**
		* Required string argument.
		*/
		_REQARG_STRING = (_ARG_STRING | _ARG_REQUIRED),
		/**
		* Required boolean argument.
		*/
		_REQARG_BOOLEAN = (_ARG_BOOLEAN | _ARG_REQUIRED),

		/**
		* @deprecated Use ARG_INTEGER instead.
		*/
		_ARG_INT32  = _ARG_INTEGER,
		/**
		* @deprecated Use ARG_FLOATING instead.
		*/
		_ARG_FLOAT32 = _ARG_FLOATING,
		/**
		* @deprecated Use ARG_FLOATING instead.
		*/
		_ARG_FLOAT64 = _ARG_FLOATING,
		/**
		* @deprecated Use REQARG_INTEGER instead.
		*/
		_REQARG_INT32 = (_ARG_INT32 | _ARG_REQUIRED),
		/**
		* @deprecated Use REQARG_FLOATING instead.
		*/
		_REQARG_FLOAT32 = (_ARG_FLOAT32 | _ARG_REQUIRED),
		/**
		* @deprecated Use REQARG_FLOATING instead.
		*/
		_REQARG_FLOAT64 = (_ARG_FLOAT64 | _ARG_REQUIRED),
		/* @} */
	};

	public ref class ArgStruct
	{

	public:

		String^ name;
		ArgType type;
		String^ deflt;
		String^ doc;

		ArgStruct(String^ name, ArgType type, String^ defaultValue, String^ document)
		{
			this->name = name;
			this->deflt = defaultValue;
			this->type = type;
			this->doc = document;
		}

	internal:

		ArgStruct(const arg_t* src);

		arg_t Generate();

		static void Release(arg_t& arg);

	};
}