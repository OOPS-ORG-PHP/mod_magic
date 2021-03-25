/*
 * php_magic.h
 *
 * php magic extension.  
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
 * $Id$
 *
 */

#ifndef _MOD_MAGIC_H
#define _MOD_MAGIC_H

#ifdef PHP_WIN32
#define PHP_MAGIC_API __declspec(dllexport)
#else
#define PHP_MAGIC_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry magic_module_entry;
#define magic_module_ptr &magic_module_entry

PHP_MINIT_FUNCTION(magic);
PHP_MSHUTDOWN_FUNCTION(magic);
PHP_RINIT_FUNCTION(magic);
PHP_RSHUTDOWN_FUNCTION(magic);
PHP_MINFO_FUNCTION(magic);

PHP_FUNCTION(filemagic);

#ifdef ZTS
#define MAGIC_G(v) TSRMG(magic_globals_id, zend_magic_globals *, v)
#else
#define MAGIC_G(v) (magic_globals.v)

#endif

#define MAGIC_BUILDVER "2.0.1"

#define phpext_magic_ptr magic_module_ptr

#if PHP_VERSION_ID >= 50100
#	define OB_START_BUFFER php_output_start_default(TSRMLS_C)
#	define OB_GET_BUFFER php_output_get_contents
#	define OB_END_BUFFER php_output_discard(TSRMLS_C)
#else
#	define OB_START_BUFFER php_start_ob_buffer(NULL, 0, 1 TSRMLS_CC);
#	define OB_GET_BUFFER php_ob_get_buffer
#	define OB_END_BUFFER php_end_ob_buffer(0, 0 TSRMLS_CC);
#endif

#define MAGIC_FILE_SET 0
#define MAGIC_DATA_SET 1

#endif  /* _MOD_MAGIC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
