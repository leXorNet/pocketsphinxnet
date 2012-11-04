/* -*- c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* ====================================================================
 * Copyright (c) 1999-2004 Carnegie Mellon University.  All rights
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

#ifndef _LIBUTIL_ERR_H_
#define _LIBUTIL_ERR_H_

#ifdef POCKETSPHINX_NET
#include "libct.h"
#else
#include <stdarg.h>
#include <stdio.h>

#ifndef _WIN32_WCE
#include <errno.h>
#endif
#endif

/* Win32/WinCE DLL gunk */
#include <sphinxbase/sphinxbase_export_hdr.h>

/**
 * @file err.h
 * @brief Implementation of logging routines. 
 *
 * Logging, warning, debug and error message output funtionality is provided in this file.
 * Sphinxbase defines several level of logging messages - INFO, WARNING, ERROR, FATAL. By
 * default output goes to standard error output.
 *
 * Logging is implemented through macros. They take same arguments as printf: format string and 
 * values. By default source file name and source line are prepended to the message. Log output 
 * could be redirected to any file using err_set_logfp() and err_set_logfile() functions. To disable 
 * logging in your application, call err_set_logfp(0).
 *
 * It's possible to log multiline info messages, to do that you need to start message with
 * E_INFO and output other lines with E_INFOCONT.
 */

#ifdef __cplusplus
extern "C" {
#endif
#if 0
/* Fool Emacs. */
}
#endif

#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_header(char const *file, long line, char const *msg);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_debug_header(char const *file, long line, int level);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_info_header(char const *file, long line, char const *tag);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_info_header_wofn(char const *msg);

#ifndef POCKETSPHINX_NET
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_warn(char const *fmt, ...);


#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__pr_info(char const *fmt, ...);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__die_error(char const *fmt, ...);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__abort_error(char const *fmt, ...);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__sys_error(char const *fmt, ...);
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
void _E__fatal_sys_error(char const *fmt, ...);
#endif

/**
 * Direct all logging to a given filehandle.
 *
 * @param logfp Filehandle to send log messages to, or 0 to disable logging.
 * @return Previous logging filehandle, if any.
 */
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
FILE *err_set_logfp(FILE *logfp);

/**
 * Get the current logging filehandle.
 * @return Current logging filehandle, 0 if disabled.
 */
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
FILE * err_get_logfp(void);


/**
 * Append all log messages to a given file.
 *
 * Previous logging filehandle is closed (unless it was stdout or stderr).
 *
 * @param file File to send log messages to, or 0 to disable logging.
 * @return 0 for success, <0 for failure (e.g. if file does not exist)
 */
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
int err_set_logfile(char const *file);

#ifndef POCKETSPHINX_NET

/**
 * Exit with non-zero status after error message 
 */
#define E_FATAL  _E__pr_header(__FILE__, __LINE__, "FATAL_ERROR"),_E__die_error

/**
 * Print error text; Call perror(""); exit(errno); 
 */
#define E_FATAL_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__fatal_sys_error

/**
 * Print error text; Call perror(""); 
 */
#define E_WARN_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__sys_error

/**
 * Print error text; Call perror(""); 
 */
#define E_ERROR_SYSTEM	_E__pr_header(__FILE__, __LINE__, "SYSTEM_ERROR"),_E__sys_error

/**
 * Print logging information to standard error stream
 */
#define E_INFO	  _E__pr_info_header(__FILE__, __LINE__, "INFO"),_E__pr_info

/**
 * Print logging information without header, to standard error stream
 */

#define E_INFOCONT	  _E__pr_info

/**
 * Print logging information without filename.
 */
#define E_INFO_NOFN _E__pr_info_header_wofn("INFO"),_E__pr_info


/**
 * Print warning information to standard error stream
 */
#define E_WARN	  _E__pr_header(__FILE__, __LINE__, "WARNING"),_E__pr_warn

/**
 * Print error message to standard error stream
 */
#define E_ERROR	  _E__pr_header(__FILE__, __LINE__, "ERROR"),_E__pr_warn

#else


/**
 * Exit with non-zero status after error message 
 */
#define E_FATAL

/**
 * Print error text; Call perror(""); exit(errno); 
 */
#define E_FATAL_SYSTEM

/**
 * Print error text; Call perror(""); 
 */
#define E_WARN_SYSTEM
/**
 * Print error text; Call perror(""); 
 */
#define E_ERROR_SYSTEM

/**
 * Print logging information to standard error stream
 */
#define E_INFO

/**
 * Print logging information without header, to standard error stream
 */

#define E_INFOCONT	

/**
 * Print logging information without filename.
 */
#define E_INFO_NOFN


/**
 * Print warning information to standard error stream
 */
#define E_WARN
/**
 * Print error message to standard error stream
 */
#define E_ERROR



#endif

/**
 * Set debugging verbosity level.
 *
 * Note that debugging messages are only enabled when compiled with -DDEBUG.
 *
 * @param level Verbosity level to set, or 0 to disable debug messages.
 */
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
int err_set_debug_level(int level);

/**
 * Get debugging verbosity level.
 *
 * Note that debugging messages are only enabled when compiled with -DDEBUG.
 */
#ifndef POCKETSPHINX_NET
SPHINXBASE_EXPORT
#endif
int err_get_debug_level(void);

/**
 * Print debugging information to standard error stream.
 *
 * This will only print a message if:
 *  1. Debugging is enabled at compile time
 *  2. The debug level is greater than or equal to \a level
 *
 * Note that for portability reasons the format and arguments must be
 * enclosed in an extra set of parentheses.
 */
#ifdef SPHINX_DEBUG
#define E_DEBUG(level,x) {                              \
        if (err_get_debug_level() >= level) {           \
            _E__pr_header(__FILE__, __LINE__, "DEBUG"); \
            _E__pr_info x;                              \
        }                                               \
    }
#define E_DEBUGCONT(level,x) {                          \
        if (err_get_debug_level() >= level) {           \
            _E__pr_info x;                              \
        }                                               \
    }
#else
#define E_DEBUG(level,x)
#define E_DEBUGCONT(level,x)
#endif

#ifdef __cplusplus
}
#endif


#endif /* !_ERR_H */


