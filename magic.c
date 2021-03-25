/*
 * magic.c
 *
 * Copyright 2015. JoungKyun.Kim all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of the Joseph Miller nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * See README, INSTALL, and USAGE files for more details.
 *
 * $Id$
 *
 */

/* {{{ includes 
 * */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include <fcntl.h>
#include <magic.h>
#include "php_magic.h"
/* }}} */

#if HAVE_MAGIC

/* True global resources - no need for thread safety here */
static int le_magic;

ZEND_BEGIN_ARG_INFO_EX(arginfo_magic, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, flag)
	ZEND_ARG_INFO(0, mpath)
ZEND_END_ARG_INFO()

/* {{{ magic_functions[]
 *
 * Every user visible function must have an entry in magic_functions[].
 */
const zend_function_entry magic_functions[] = {
	PHP_FE(filemagic, arginfo_magic)
	PHP_FALIAS(filebin, filemagic, arginfo_magic)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ magic_module_entry
 */
zend_module_entry magic_module_entry = {
	STANDARD_MODULE_HEADER,
	"magic",
	magic_functions,
	PHP_MINIT(magic),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(magic),
	MAGIC_BUILDVER,
	STANDARD_MODULE_PROPERTIES,
};
/* }}} */

#ifdef COMPILE_DL_MAGIC
ZEND_GET_MODULE(magic)
#endif

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(magic)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "file magic support", "enabled");
	php_info_print_table_row(2, "Build version", MAGIC_BUILDVER);
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(magic)
{
	// magic flags
	REGISTER_LONG_CONSTANT ("MAGIC_NONE", MAGIC_NONE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_DEBUG", MAGIC_DEBUG, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_SYMLINK", MAGIC_SYMLINK, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_COMPRESS", MAGIC_COMPRESS, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_DEVICES", MAGIC_DEVICES, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_MIME_TYPE", MAGIC_MIME_TYPE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_CONTINUE", MAGIC_CONTINUE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_CHECK", MAGIC_CHECK, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_PRESERVE_ATIME", MAGIC_PRESERVE_ATIME, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_RAW", MAGIC_RAW, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_ERROR", MAGIC_ERROR, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_MIME_ENCODING", MAGIC_MIME_ENCODING, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_MIME", MAGIC_MIME, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_APPLE", MAGIC_APPLE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_COMPRESS", MAGIC_NO_CHECK_COMPRESS, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_TAR", MAGIC_NO_CHECK_TAR, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_SOFT", MAGIC_NO_CHECK_SOFT, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_APPTYPE", MAGIC_NO_CHECK_APPTYPE, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_ELF", MAGIC_NO_CHECK_ELF, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_TEXT", MAGIC_NO_CHECK_TEXT, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_CDF", MAGIC_NO_CHECK_CDF, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_TOKENS", MAGIC_NO_CHECK_TOKENS, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_ENCODING", MAGIC_NO_CHECK_ENCODING, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_ASCII", MAGIC_NO_CHECK_ASCII, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_FORTRAN", MAGIC_NO_CHECK_FORTRAN, CONST_PERSISTENT | CONST_CS);
	REGISTER_LONG_CONSTANT ("MAGIC_NO_CHECK_TROFF", MAGIC_NO_CHECK_TROFF, CONST_PERSISTENT | CONST_CS);
	REGISTER_STRING_CONSTANT ("MAGIC_FILE", MAGIC, CONST_PERSISTENT | CONST_CS);
}
/* }}} */

/* {{{ static void magic_set_error (int type, const char * format, ...)
 */
static void magic_set_error (int type, const char * format, ...) {
	va_list args;
	char * buffer     = NULL;
	int    buffer_len = 0;
	TSRMLS_FETCH ();

	// php_error_cb in main.c
	va_start (args, format);
	buffer_len = vspprintf (&buffer, PG(log_errors_max_len), format, args);
	va_end (args);

	if ( PG (last_error_message) ) {
		free (PG (last_error_message));
		PG (last_error_message) = NULL;
	}
	if ( PG(last_error_file) ) {
		free (PG (last_error_file));
		PG (last_error_file) = NULL;
	}
	PG (last_error_lineno) = 0;

	/*
	 * last_error_file, last_error_lineno
	 * see also zend_error in zend.c
	 *   -> zend_get_compiled_filename
	 *   -> zend_get_executed_lineno
	 */
	PG (last_error_type) = type;
	PG (last_error_message) = strdup (buffer);
	if ( zend_is_compiling (TSRMLS_C) ) {
		PG (last_error_file) = strdup (zend_get_compiled_filename (TSRMLS_C));
		PG (last_error_lineno) = zend_get_compiled_lineno (TSRMLS_C);
	} else if ( zend_is_executing (TSRMLS_C) ) {
		PG (last_error_file) = strdup (zend_get_executed_filename(TSRMLS_C));
		PG (last_error_lineno) = zend_get_executed_lineno (TSRMLS_C);
	}

	if ( PG(track_errors) ) {
		if (!EG (active_symbol_table)) {
			zend_rebuild_symbol_table (TSRMLS_C);
		}
		if ( EG (active_symbol_table) ) {
			zval * tmp;
			ALLOC_INIT_ZVAL (tmp);
			ZVAL_STRINGL (tmp, buffer, buffer_len, 1);
			zend_hash_update (
				EG (active_symbol_table), "php_errormsg", sizeof("php_errormsg"),
				(void **) &tmp, sizeof(zval *), NULL
			);
		}
	}
	if ( buffer_len > 0 ) {
		str_efree (buffer);
	}
}
/* }}} */

/* {{{ proto (string|null) filemagic(string path, int flag, string mpath)
 */ 
PHP_FUNCTION(filemagic) {
	zval         * zflag;
	zval         * zpath;
	char         * path = NULL;
	char         * mpath = NULL;
	const char   * buf;
	int            type = MAGIC_FILE_SET,
				   path_len = 0,
				   flags = 0,
				   flag = 0,
				   action = 0, // FILE_LOAD
				   chkargs = ZEND_NUM_ARGS ();

	struct stat    filestat;
	struct magic_set * mp = NULL;


	HashTable    * args_arr = NULL;
	HashPosition   pos;

	/* makes islower etc work for other langs */
	(void) setlocale (LC_CTYPE, "");

#ifdef S_IFLNK
	flags |= getenv("POSIXLY_CORRECT") ? MAGIC_SYMLINK : 0;
#endif

	if ( zend_parse_parameters (chkargs TSRMLS_CC, "s|zz", &path, &path_len, &zflag, &zpath) == FAILURE )
		return;

	if ( path_len == 0 ) {
		magic_set_error (E_WARNING, "The value of 1st argument was empty.");
		RETURN_FALSE;
	}

	if ( strncmp (path, "DATA:", 5 ) == 0 ) {
		type = MAGIC_DATA_SET;
		path += 5;
		path_len -= 5;

		if ( path_len == 0 ) {
			magic_set_error (E_WARNING, "The value of 1st argument was empty.");
			RETURN_FALSE;
		}
	}

	if ( type == MAGIC_FILE_SET && stat (path, &filestat) != 0 ) {
		magic_set_error (E_WARNING, "%s file not found.", path);
		RETURN_FALSE;
	}

	if ( chkargs == 2 ) {
		int p = Z_TYPE_P (zflag);

		switch (p) {
			case IS_STRING :
				mpath = Z_STRVAL_P (zflag);
				flag = MAGIC_NONE;
				break;
			case IS_LONG :
				flag = Z_LVAL_P (zflag);
				mpath = MAGIC;
				break;
			default :
				magic_set_error (E_WARNING, "2th argument is only available for integer(flag) or MAGIC file path.");
				RETURN_FALSE;
		}
	} else if ( chkargs == 3 ) {
		if ( Z_TYPE_P (zflag) == IS_LONG && Z_TYPE_P (zpath) == IS_STRING ) {
			flag = Z_LVAL_P (zflag);
			mpath = Z_STRLEN_P (zpath) ? Z_STRVAL_P (zpath) : MAGIC;
		} else if ( Z_TYPE_P (zflag) == IS_STRING && Z_TYPE_P (zpath) == IS_LONG  ) {
			flag = Z_LVAL_P (zpath);
			mpath = Z_STRLEN_P (zflag) ? Z_STRVAL_P (zflag) : MAGIC;
		} else {
			if ( Z_TYPE_P (zflag) == IS_LONG && Z_TYPE_P (zpath) != IS_STRING ) {
				magic_set_error (
					E_WARNING,
					"If the 2th argument is an integer, the 3th argument must specify strings(magic path)."
				);
			} else {
				magic_set_error (
					E_WARNING,
					"If the 2th argument is an stings, the 3th argument must specify integer (flag)."
				);
				magic_set_error (E_WARNING, "The 2th and 3th argument can only be integer or strings.");
			}
			RETURN_FALSE;
		}
	}

	if ( flag )
		flags |= flag;

	mp = magic_open (flag);
	if ( mp == NULL ) {
		magic_set_error (E_WARNING, strerror (errno));
		RETURN_FALSE;
	}

	if ( magic_load (mp, mpath) == -1 ) {
		magic_set_error (E_WARNING, magic_error (mp));
		magic_close (mp);
		RETURN_FALSE;
	}

	if ( type == MAGIC_FILE_SET )
		buf = magic_file (mp, path);
	else
		buf = magic_buffer (mp, path, path_len);
	
	if ( buf == NULL ) {
		magic_set_error (E_WARNING, magic_error(mp));
		magic_close (mp);
		RETURN_FALSE;
	}

	RETVAL_STRING (buf, 1);
	magic_close (mp);
}
/* }}} */

#endif	/* HAVE_MAGIC */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
