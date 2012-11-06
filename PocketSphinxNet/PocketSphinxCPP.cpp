#pragma once

#include "stdafx.h"

#include "PocketSphinxCPP.h"
#include "cmdln_macro.h"

const arg_t* GetDefs()
{
	static arg_t cont_args_def[] = {
		POCKETSPHINX_OPTIONS,
		///* Argument file. */
		//{ "-argfile",
		//ARG_STRING,
		//0,
		//"Argument file giving extra arguments." },
		{ "-adcdev", 
		ARG_STRING, 
		0, 
		"Name of audio device to use for input." },
		{ "-infile", 
		ARG_STRING, 
		0, 
		"Audio file to transcribe." },
		{ "-time", 
		ARG_BOOLEAN, 
		"no", 
		"Print word times in file transcription." },
		CMDLN_EMPTY_OPTION
	};


	return cont_args_def;
}