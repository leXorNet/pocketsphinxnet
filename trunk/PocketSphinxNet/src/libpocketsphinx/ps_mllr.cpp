/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 2009 Carnegie Mellon University.  All rights
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

/**
 * @file ps_mllr.c Model-space linear transforms for speaker adaptation
 */

/* System headers. */
#ifndef POCKETSPHINX_NET
#include <stdio.h>
#else
#include "libct.h"
#endif

/* SphinxBase headers. */
#include <sphinxbase/ckd_alloc.h>

/* Local headers. */
#include "acmod.h"

ps_mllr_t *
ps_mllr_read(char const *regmatfile)
{
    ps_mllr_t *mllr;
    FILE *fp;
    int n, i, m, j, k;

    mllr = (ps_mllr_t*)ckd_calloc(1, sizeof(*mllr));
    mllr->refcnt = 1;

    if ((fp = fopen(regmatfile, "r")) == 0) {
        E_ERROR_SYSTEM("Failed to open MLLR file '%s' for reading", regmatfile);
        goto error_out;
    }
    else
	{
        E_INFO("Reading MLLR transformation file '%s'\n", regmatfile);
	}
#ifndef POCKETSPHINX_NET
    if ((fscanf(fp, "%d", &n) != 1) || (n < 1)) {
        E_ERROR("Failed to read number of MLLR classes\n");
        goto error_out;
    }
#else
    if ((net_fscanf_int_space_sep(fp,false, &n) != 1) || (n < 1)) {
        E_ERROR("Failed to read number of MLLR classes\n");
        goto error_out;
    }
#endif

    mllr->n_class = n;
#ifndef POCKETSPHINX_NET
    if ((fscanf(fp, "%d", &n) != 1)) {
        E_ERROR("Failed to read number of feature streams\n");
        goto error_out;
    }
#else
    if ((net_fscanf_int_space_sep(fp,false, &n) != 1)) {
        E_ERROR("Failed to read number of feature streams\n");
        goto error_out;
    }
#endif

    mllr->n_feat = n;
    mllr->veclen = (int*)ckd_calloc(mllr->n_feat, sizeof(*mllr->veclen));

    mllr->A = (float32 ****) ckd_calloc(mllr->n_feat, sizeof(float32 **));
    mllr->b = (float32 ***) ckd_calloc(mllr->n_feat, sizeof(float32 *));
    mllr->h = (float32 ***) ckd_calloc(mllr->n_feat, sizeof(float32 *));

    for (i = 0; i < mllr->n_feat; ++i) {
#ifndef POCKETSPHINX_NET
        if (fscanf(fp, "%d",&n) != 1) {
            E_ERROR("Failed to read stream length for feature %d\n", i);
            goto error_out;
        }
#else
        if (net_fscanf_int_space_sep(fp,false,&n) != 1) {
            E_ERROR("Failed to read stream length for feature %d\n", i);
            goto error_out;
        }
#endif
        mllr->veclen[i] = n;
        mllr->A[i] =
            (float32 ***) ckd_calloc_3d(mllr->n_class, mllr->veclen[i],
                                        mllr->veclen[i], sizeof(float32));
        mllr->b[i] =
            (float32 **) ckd_calloc_2d(mllr->n_class, mllr->veclen[i],
                                       sizeof(float32));
        mllr->h[i] =
            (float32 **) ckd_calloc_2d(mllr->n_class, mllr->veclen[i],
                                       sizeof(float32));

        for (m = 0; m < mllr->n_class; ++m) {
            for (j = 0; j < mllr->veclen[i]; ++j) {
                for (k = 0; k < mllr->veclen[i]; ++k) {

#ifndef POCKETSPHINX_NET    
					if (fscanf(fp, "%f ", &mllr->A[i][m][j][k]) != 1) {
                        E_ERROR("Failed reading MLLR rotation (%d,%d,%d,%d)\n",
                                i, m, j, k);
                        goto error_out;
                    }
#else
					if (net_fscanf_float_space_sep(fp,true, &mllr->A[i][m][j][k]) != 1) {
                        E_ERROR("Failed reading MLLR rotation (%d,%d,%d,%d)\n",
                                i, m, j, k);
                        goto error_out;
                    }
#endif
                }
            }
            for (j = 0; j < mllr->veclen[i]; ++j) {
#ifndef POCKETSPHINX_NET
                if (fscanf(fp, "%f ", &mllr->b[i][m][j]) != 1) {
                    E_ERROR("Failed reading MLLR bias (%d,%d,%d)\n",
                            i, m, j);
                    goto error_out;
                }
#else
                if (net_fscanf_float_space_sep(fp,true,  &mllr->b[i][m][j]) != 1) {
                    E_ERROR("Failed reading MLLR bias (%d,%d,%d)\n",
                            i, m, j);
                    goto error_out;
                }
#endif
            }
            for (j = 0; j < mllr->veclen[i]; ++j) {
#ifndef POCKETSPHINX_NET
                if (fscanf(fp, "%f ", &mllr->h[i][m][j]) != 1) {
                    E_ERROR("Failed reading MLLR variance scale (%d,%d,%d)\n",
                            i, m, j);
                    goto error_out;
                }
#else
                if (net_fscanf_float_space_sep(fp,true, &mllr->h[i][m][j]) != 1) {
                    E_ERROR("Failed reading MLLR variance scale (%d,%d,%d)\n",
                            i, m, j);
                    goto error_out;
                } 
#endif
            }
        }
    }
    fclose(fp);
    return mllr;

error_out:
    if (fp)
        fclose(fp);
    ps_mllr_free(mllr);
    return (ps_mllr_t*)0;
}

ps_mllr_t *
ps_mllr_retain(ps_mllr_t *mllr)
{
    ++mllr->refcnt;
    return mllr;
}

int
ps_mllr_free(ps_mllr_t *mllr)
{
    int i;

    if (mllr == 0)
        return 0;
    if (--mllr->refcnt > 0)
        return mllr->refcnt;

    for (i = 0; i < mllr->n_feat; ++i) {
        if (mllr->A)
            ckd_free_3d(mllr->A[i]);
        if (mllr->b)
            ckd_free_2d(mllr->b[i]);
        if (mllr->h)
            ckd_free_2d(mllr->h[i]);
    }
    ckd_free(mllr->veclen);
    ckd_free(mllr->A);
    ckd_free(mllr->b);
    ckd_free(mllr->h);
    ckd_free(mllr);

    return 0;
}
