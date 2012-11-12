/*
 * filebin.c
 *
 * JoungKyun Kim, <http://devel.oops.org>
 *
 * Copyright (c) 2012, JoungKyun.Kim
 * All rights reserved.
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
#include "php_filebin.h"
/* }}} */

#if HAVE_FILEBIN

/* True global resources - no need for thread safety here */
static int le_filebin;

ZEND_BEGIN_ARG_INFO_EX(arginfo_filebin, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, flag)
	ZEND_ARG_INFO(0, magic)
ZEND_END_ARG_INFO()

/* {{{ filebin_functions[]
 *
 * Every user visible function must have an entry in filebin_functions[].
 */
const zend_function_entry filebin_functions[] = {
	PHP_FE(filebin, arginfo_filebin)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ filebin_module_entry
 */
zend_module_entry filebin_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"filebin",
	filebin_functions,
	PHP_MINIT(filebin),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(filebin),
#if ZEND_MODULE_API_NO >= 20010901
	FILEBIN_BUILDVER,
#endif
	STANDARD_MODULE_PROPERTIES,
};
/* }}} */

#ifdef COMPILE_DL_FILEBIN
ZEND_GET_MODULE(filebin)
#endif

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(filebin)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "filebin support", "enabled");
	php_info_print_table_row(2, "Build version", FILEBIN_BUILDVER);
	php_info_print_table_end();
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(filebin)
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
}
/* }}} */

/* {{{ proto int filebin(string path, array args_arr, int argc)
 */ 
PHP_FUNCTION(filebin) {
	zval         * zflag;
	char         * path = NULL;
	char         * magicpath = NULL;
	const char   * type;
	int            path_len = 0,
				   magic_len = 0,
				   flags = 0,
				   flag = 0,
				   action = 0, // FILE_LOAD
				   chkargs = ZEND_NUM_ARGS ();

	struct stat    filestat;
	struct magic_set * magic = NULL;


	HashTable    * args_arr = NULL;
	HashPosition   pos;

	/* makes islower etc work for other langs */
	(void) setlocale (LC_CTYPE, "");

#ifdef S_IFLNK
	flags |= getenv("POSIXLY_CORRECT") ? MAGIC_SYMLINK : 0;
#endif

	if (
		zend_parse_parameters  (
			chkargs TSRMLS_CC,
			"s|zs", &path, &path_len, &zflag, &magicpath, &magic_len) == FAILURE
	   )
		return;

	if ( path_len == 0 ) {
		php_error (E_WARNING, "Must need filename for checking.");
		RETURN_NULL ();
	}

	if ( stat (path, &filestat) != 0 ) {
		php_error (E_WARNING, "%s is not found.", path);
		RETURN_NULL ();
	}

	if ( chkargs == 2 ) {
		int p = Z_TYPE_P (zflag);

		switch (p) {
			case IS_STRING :
				magicpath = Z_STRVAL_P (zflag);
				flag = 0;
				break;
			case IS_LONG :
				flag = Z_LVAL_P (zflag);
				break;
			default :
				php_error (E_WARNING, "filebin: Only permit flag or magic file path.");
				RETURN_NULL ();
		}
	} else if ( chkargs == 3 ) {
		if ( Z_TYPE_P (zflag) != IS_LONG ) {
			php_error (E_WARNING, "filebin: Only permit flag on 2th argument.");
			RETURN_NULL ();
		}

		flag = Z_LVAL_P (zflag);
	}

	if ( flag )
		flags |= flag;

	magic = magic_open (flag);
	if ( magic == NULL ) {
		php_error (E_WARNING, strerror (errno));
		RETURN_NULL ();
	}

	if ( magic_load (magic, magicpath) == -1 ) {
		php_error (E_WARNING, magic_error (magic));
		magic_close (magic);
		RETURN_NULL ();
	}

	if ( (type = magic_file (magic, path)) == NULL ) {
		php_error (E_WARNING, magic_error(magic));
		magic_close (magic);
		RETURN_NULL ();
	}

	RETVAL_STRING (type, 1);
	magic_close (magic);
}
/* }}} */

#endif	/* HAVE_FILEBIN */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
