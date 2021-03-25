/*
 * magic.c
 *
 * JoungKyun.Kim, <http://oops.org>
 *
 * Copyright 2021. JoungKyun.Kim all rights reserved.
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

#if PHP_VERSION_ID < 70000
#error "************ PHP version dependency problems *******************"
#error "This package requires over php 7.0.0 !!"
#error "If you build with php under 7.0.0, use mod_magic 2.x version"
#error "You can download mod_magic 2.x at https://github.com/OOPS-ORG-PHP/mod_magic/releases"
#endif

#if HAVE_MAGIC

/* True global resources - no need for thread safety here */
static int le_magic;

#include "magic_arginfo.h"

/* {{{ magic_functions[]
 *
 * Every user visible function must have an entry in magic_functions[].
 */
const zend_function_entry magic_functions[] = {
	ZEND_FE(filemagic, arginfo_filemagic)
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
	php_info_print_table_row(2, "URL", "https://github.com/OOPS-ORG-PHP/mod_magic");
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
	va_list       args;
	char        * buffer     = NULL;
	int           buffer_len = 0;
#if PHP_VERSION_ID >= 80000
	zend_string * zbuffer;
#endif

	// php_error_cb in main.c
	va_start (args, format);
	buffer_len = vspprintf (&buffer, PG(log_errors_max_len), format, args);
	va_end (args);

#if PHP_VERSION_ID >= 80000
	zbuffer = zend_string_init (buffer, buffer_len, 0);
#endif

#if PHP_VERSION_ID >= 80000
	if (PG(last_error_message)) {
		zend_string_release(PG(last_error_message));
		PG(last_error_message) = NULL;
	}
	if (PG(last_error_file)) {
		free(PG(last_error_file));
		PG(last_error_file) = NULL;
	}
#else
	if ( PG (last_error_message) ) {
	#if PHP_VERSION_ID >= 70200
		char * s = PG(last_error_message);
		PG(last_error_message) = NULL;
		free (s);
	#else
		free (PG (last_error_message));
		PG (last_error_message) = NULL;
	#endif
	}
	if ( PG(last_error_file) ) {
	#if PHP_VERSION_ID >= 70200
		char * s = PG(last_error_file);
		PG(last_error_file) = NULL;
		free (s);
	#else
		free (PG (last_error_file));
		PG (last_error_file) = NULL;
	#endif
	}
#endif
	PG (last_error_lineno) = 0;

	/*
	 * last_error_file, last_error_lineno
	 * see also zend_error in zend.c
	 *   -> zend_get_compiled_filename
	 *   -> zend_get_executed_lineno
	 */
	PG (last_error_type) = type;
#if PHP_VERSION_ID >= 80000
	PG (last_error_message) = zend_string_copy (zbuffer);
	zend_string_release_ex (zbuffer, 0);
#else
	PG (last_error_message) = strdup (buffer);
#endif
	if ( zend_is_compiling () ) {
		PG (last_error_file) = strdup (ZSTR_VAL (zend_get_compiled_filename ()));
		PG (last_error_lineno) = zend_get_compiled_lineno ();
	} else if ( zend_is_executing () ) {
		PG (last_error_file) = strdup (zend_get_executed_filename ());
		PG (last_error_lineno) = zend_get_executed_lineno ();
	}

#if PHP_VERSION_ID < 70200
	/*
	 * $php_errormsg don't support 7.2.0 and after.
	 */
	if ( PG(track_errors) ) {
		zval tmp;

		ZVAL_STRINGL(&tmp, buffer, buffer_len);
		if (EG(current_execute_data)) {
			if (zend_set_local_var_str("php_errormsg", sizeof("php_errormsg")-1, &tmp, 0) == FAILURE) {
				zval_ptr_dtor(&tmp);
			}
		} else {
			zend_hash_str_update_ind(&EG(symbol_table), "php_errormsg", sizeof("php_errormsg")-1, &tmp);
		}
	}
#endif
	if ( buffer_len > 0 )
		efree (buffer);
}
/* }}} */

/* {{{ proto filemagic(string path[, int flag[, string magic-path]]): string|false
 *     proto filemagic(string path[, string magic-path[, int flag]]): string|false
 */
ZEND_FUNCTION(filemagic) {
	zval         * zflag;
	zval         * zpath; // path of Magic file
	zend_string  * path   = NULL;
	char         * mpath  = NULL;
	const char   * buf;
	int            type   = MAGIC_FILE_SET,
	               flags  = 0,
	               flag   = 0,
	               action = 0, // FILE_LOAD
	               fargs  = ZEND_NUM_ARGS ();

	struct stat    filestat;
	struct magic_set * mp = NULL;

	HashTable    * args_arr = NULL;
	HashPosition   pos;

	/* makes islower etc work for other langs */
	(void) setlocale (LC_CTYPE, "");

#ifdef S_IFLNK
	flags |= getenv("POSIXLY_CORRECT") ? MAGIC_SYMLINK : 0;
#endif

	if ( zend_parse_parameters  (fargs TSRMLS_CC, "S|zz", &path, &zflag, &zpath) == FAILURE )
		return;

	if ( ZSTR_LEN (path) == 0 ) {
		magic_set_error (E_WARNING, "The value of 1st argument was empty.");
		RETURN_FALSE;
	}

	if ( strncmp (ZSTR_VAL (path), "DATA:", 5) == 0 ) {
		type = MAGIC_DATA_SET;

		if ( (ZSTR_LEN (path) - 5) < 1 ) {
			magic_set_error (E_WARNING, "The value of 1st argument was empty.");
			RETURN_FALSE;
		}
	}

	if ( type == MAGIC_FILE_SET && stat (ZSTR_VAL (path), &filestat) != 0 ) {
		magic_set_error (E_WARNING, "%s file not found.", ZSTR_VAL (path));
		RETURN_FALSE;
	}

	if ( fargs == 2 ) {
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
	} else if ( fargs == 3 ) {
		if ( Z_TYPE_P (zflag) == IS_LONG && Z_TYPE_P(zpath) == IS_STRING ) {
			flag = Z_LVAL_P (zflag);
			mpath = Z_STRLEN_P (zpath) ? Z_STRVAL_P (zpath) : MAGIC;
		} else if ( Z_TYPE_P (zflag) == IS_STRING && Z_TYPE_P(zpath) == IS_LONG  ) {
			flag = Z_LVAL_P (zpath);
			mpath = Z_STRLEN_P (zflag) ? Z_STRVAL_P (zflag) : MAGIC;
		} else {
			magic_set_error (E_WARNING, "The 2th and 3th argument can only be integer or strings.");
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
		buf = magic_file (mp, ZSTR_VAL (path));
	else
		buf = magic_buffer (mp, ZSTR_VAL (path) + 5, ZSTR_LEN (path) - 5);

	if ( buf == NULL ) {
		magic_set_error (E_WARNING, magic_error(mp));
		magic_close (mp);
		RETURN_FALSE;
	}

	RETVAL_STRING (buf);
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
