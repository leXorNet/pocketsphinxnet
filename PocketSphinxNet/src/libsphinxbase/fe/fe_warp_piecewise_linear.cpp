/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 2006 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*********************************************************************
 *
 * File: fe_warp_piecewise_linear.c
 * 
 * Description: 
 *
 * 	Warp the frequency axis according to an piecewise linear
 * 	function. The function is linear up to a frequency F, where
 * 	the slope changes so that the Nyquist frequency in the warped
 * 	axis maps to the Nyquist frequency in the unwarped.
 *
 *		w' = a * w, w < F
 *              w' = a' * w + b, W > F
 *              w'(0) = 0
 *              w'(F) = F
 *              w'(Nyq) = Nyq
 *	
 *********************************************************************/

/* static char rcsid[] = "@(#)$Id: fe_warp_piecewise_linear.c,v 1.2 2006/02/17 00:31:34 egouvea Exp $"; */


 /* System headers. */
#ifndef POCKETSPHINX_NET
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#else
#include "libct.h"
#endif



#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif

#include "sphinxbase/strfuncs.h"
#include "sphinxbase/err.h"

#include "fe_warp.h"
#include "fe_warp_piecewise_linear.h"

#define N_PARAM		2
#define YES             1
#define NO              0

/*
 * params[0] : a
 * params[1] : F (the non-differentiable point)
 */
//static float params[N_PARAM] = { 1.0f, 6800.0f };
//static float final_piece[2];
//static int32 is_neutral = YES;
//static char p_str[256] = "";
//static float nyquist_frequency = 0.0f;


static float* Get_Final_Piece()
{
	static float final_piece[2] = {0};
	return final_piece;
}
static float* GetParams()
{
	static float params[N_PARAM] = { 1.0f, 6800.0f };
	return params;
}

static char* GetP_Str()
{
	static char p_str[256] = "";
	return p_str;
}

static int32& Get_Is_Neutral()
{
	static int32 is_neutral = YES;

	return is_neutral;
}

static float& Get_Nyquist_Frequency()
{
	static float nyquist_frequency = 0.0f;

	return nyquist_frequency;
}


const char *
fe_warp_piecewise_linear_doc()
{
    return "piecewise_linear :== < w' = a * w, w < F >";
}

uint32
fe_warp_piecewise_linear_id()
{
    return FE_WARP_ID_PIECEWISE_LINEAR;
}

uint32
fe_warp_piecewise_linear_n_param()
{
    return N_PARAM;
}

void
fe_warp_piecewise_linear_set_parameters(char const *param_str,
                                        float sampling_rate)
{
    char *tok = 0;
    char *seps = " \t";
    char temp_param_str[256];
    int param_index = 0;

	Get_Nyquist_Frequency() = sampling_rate / 2;
    if (param_str == 0) {
		Get_Is_Neutral() = YES;
        return;
    }
    /* The new parameters are the same as the current ones, so do nothing. */
	if (strcmp(param_str, GetP_Str()) == 0) {
        return;
    }
    Get_Is_Neutral() = NO;
    strcpy(temp_param_str, param_str);
    memset(GetParams(), 0, N_PARAM * sizeof(float));
	memset(Get_Final_Piece(), 0, 2 * sizeof(float));
    strcpy(GetP_Str(), param_str);
    /* FIXME: strtok() is not re-entrant... */
#ifndef POCKETSPHINX_NET
    tok = strtok(temp_param_str, seps);
    while (tok != 0) {
        GetParams()[param_index++] = (float) atof_c(tok);
        tok = strtok(0, seps);
        if (param_index >= N_PARAM) {
            break;
        }
    }
	if (tok != 0) {
        E_INFO
            ("Piecewise linear warping takes up to two arguments, %s ignored.\n",
             tok);
    }
#else
	String^ buffer = gcnew String(temp_param_str);

	array<String^>^ parts = buffer->Split(gcnew array<wchar_t>(2){' ','\t'},StringSplitOptions::RemoveEmptyEntries);

	if(parts!=nullptr && parts->Length>0)
	{
		for(int i = 0;i<parts->Length;i++)
		{
			if(Single::TryParse(parts[i],GetParams()[param_index]))
			{
				param_index++;

				if(param_index>=N_PARAM)
				{
					break;
				}
			}
		}
	}
#endif    

    if (GetParams()[1] < sampling_rate) {
        /* Precompute these. These are the coefficients of a
         * straight line that contains the points (F, aF) and (N,
         * N), where a = params[0], F = params[1], N = Nyquist
         * frequency.
         */
		if (GetParams()[1] == 0) {
            GetParams()[1] = sampling_rate * 0.85f;
        }
        Get_Final_Piece()[0] =
			(Get_Nyquist_Frequency() -
             GetParams()[0] * GetParams()[1]) / (Get_Nyquist_Frequency() - GetParams()[1]);
        Get_Final_Piece()[1] =
            Get_Nyquist_Frequency() * GetParams()[1] * (GetParams()[0] -
                                         1.0f) / (Get_Nyquist_Frequency() -
                                                  GetParams()[1]);
    }
    else {
        memset(Get_Final_Piece(), 0, 2 * sizeof(float));
    }
    if (GetParams()[0] == 0) {
        Get_Is_Neutral() = YES;
        E_INFO
            ("Piecewise linear warping cannot have slope zero, warping not applied.\n");
    }
}

float
fe_warp_piecewise_linear_warped_to_unwarped(float nonlinear)
{
    if (Get_Is_Neutral()) {
        return nonlinear;
    }
    else {
        /* linear = (nonlinear - b) / a */
        float temp;
        if (nonlinear < GetParams()[0] * GetParams()[1]) {
            temp = nonlinear / GetParams()[0];
        }
        else {
            temp = nonlinear - Get_Final_Piece()[1];
            temp /= Get_Final_Piece()[0];
        }
        if (temp > Get_Nyquist_Frequency()) {
            E_WARN
                ("Warp factor %g results in frequency (%.1f) higher than Nyquist (%.1f)\n",
                 GetParams()[0], temp, Get_Nyquist_Frequency());
        }
        return temp;
    }
}

float
fe_warp_piecewise_linear_unwarped_to_warped(float linear)
{
    if (Get_Is_Neutral()) {
        return linear;
    }
    else {
        float temp;
        /* nonlinear = a * linear - b */
        if (linear < GetParams()[1]) {
            temp = linear * GetParams()[0];
        }
        else {
            temp = Get_Final_Piece()[0] * linear + Get_Final_Piece()[1];
        }
        return temp;
    }
}

void
fe_warp_piecewise_linear_print(const char *label)
{
#ifndef POCKETSPHINX_NET
    uint32 i;

    for (i = 0; i < N_PARAM; i++) {
        printf("%s[%04u]: %6.3f ", label, i, GetParams()[i]);
    }
    printf("\n");
#endif
}

/*
 * Log record.  Maintained by RCS.
 *
 * $Log: fe_warp_piecewise_linear.c,v $
 * Revision 1.2  2006/02/17 00:31:34  egouvea
 * Removed switch -melwarp. Changed the default for window length to
 * 0.025625 from 0.256 (so that a window at 16kHz sampling rate has
 * exactly 410 samples). Cleaned up include's. Replaced some E_FATAL()
 * with E_WARN() and return.
 *
 * Revision 1.1  2006/02/16 00:18:26  egouvea
 * Implemented flexible warping function. The user can specify at run
 * time which of several shapes they want to use. Currently implemented
 * are an affine function (y = ax + b), an inverse linear (y = a/x) and a
 * piecewise linear (y = ax, up to a frequency F, and then it "breaks" so
 * Nyquist frequency matches in both scales.
 *
 * Added two switches, -warp_type and -warp_params. The first specifies
 * the type, which valid values:
 *
 * -inverse or inverse_linear
 * -linear or affine
 * -piecewise or piecewise_linear
 *
 * The inverse_linear is the same as implemented by EHT. The -mel_warp
 * switch was kept for compatibility (maybe remove it in the
 * future?). The code is compatible with EHT's changes: cepstra created
 * from code after his changes should be the same as now. Scripts that
 * worked with his changes should work now without changes. Tested a few
 * cases, same results.
 *
 */
