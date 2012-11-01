/*
 * filebin.c
 *
 * JoungKyun Kim, <http://devel.oops.org>
 *
 * Copyright (c) 2004, JoungKyun Kim
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
#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "php_filebin.h"
/* }}} */

#if HAVE_FILEBIN

/* True global resources - no need for thread safety here */
static int le_filebin;

/* {{{ filebin_functions[]
 *
 * Every user visible function must have an entry in filebin_functions[].
 */
const zend_function_entry filebin_functions[] = {
	PHP_FE(filebin, NULL)
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
	NULL,
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

/* {{{ proto int filebin(string file, array args_arr, int argc)
 */ 
PHP_FUNCTION(filebin) {
	zval		** file;
	zval		** args;
	zval		** z_argc;
	char		** argv;
	char		 * fname;
	int			   argc, i, chkargs;
	HashTable	 * args_arr;
	HashPosition   pos;

	struct stat    filestat;

	chkargs = ZEND_NUM_ARGS();

	switch ( chkargs ) {
		case 3:
			if ( zend_get_parameters_ex (chkargs, &file, &args, &z_argc) == FAILURE ) {
				WRONG_PARAM_COUNT;
			}

			if ( Z_TYPE_PP (args) != IS_ARRAY ) {
				php_error(E_WARNING, "2nd Variable passed to filebin is not an array!\n");
				RETURN_FALSE;
			}

			convert_to_long_ex (z_argc);
			argc = Z_LVAL_PP (z_argc) + 2;
			break;
		case 1:
			if ( zend_get_parameters_ex (chkargs, &file) == FAILURE ) {
				WRONG_PARAM_COUNT;
			}
			argc = 2;
			break;
		default:
			WRONG_PARAM_COUNT;
	}

	convert_to_string_ex (file);
	fname = Z_STRVAL_PP (file);

	if ( fname == NULL || strlen (fname) == 0 ) {
		php_error (E_WARNING, "Must need filename for checking\n");
		RETURN_FALSE;
	}

	if ( stat (fname, &filestat) != 0 ) {
		php_error (E_WARNING, "%s is not found\n", fname);
		RETURN_FALSE;
	}

	if ( chkargs > 2 ) {
		convert_to_array_ex(args);
		args_arr = Z_ARRVAL_PP (args);
	}

	argv = ( char ** ) emalloc(argc * sizeof(char *));

	argv[0] = "filebin";

	/* added option */
	if ( chkargs > 2 ) {
		convert_to_array_ex (args);
		args_arr = Z_ARRVAL_PP (args);
		zend_hash_internal_pointer_reset_ex (args_arr, &pos);

		for (i = 1; i < argc - 1; i++) {
			zval **dataptr;

			if ( zend_hash_get_current_data_ex(args_arr, (void **) &dataptr, &pos) == FAILURE )
				continue;

			if ( Z_TYPE_PP (dataptr) != IS_STRING )
				convert_to_string_ex (dataptr);

			argv[i] = estrdup ( Z_STRVAL_PP (dataptr) );

			if ( i < argc )
				zend_hash_move_forward_ex (args_arr, &pos);
		}
	} else {
		i = 1;
	}

	argv[i] = estrdup ( fname );

	optind = 0;
	opterr = 0;

	php_start_ob_buffer (NULL, 0, 1 TSRMLS_CC);
	if ( file_main(argc, argv) != 0 ) {
		php_printf ("@@@\n");
		RETVAL_FALSE;
	} else {
		php_ob_get_buffer (return_value TSRMLS_CC);
	}
	php_end_ob_buffer (0, 0 TSRMLS_CC);

	for (i = 1; i < argc; i++)
		efree(argv[i]);

	efree(argv);

	return;
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
