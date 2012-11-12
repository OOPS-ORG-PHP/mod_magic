/*
 * php_filebin.h
 *
 * php filebin extension.  
 *
 * JoungKyun Kim, <http://www.oops.org> 8/19/2004
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
 * $Id: php_filebin.h,v 1.1 2004-08-20 13:53:42 oops Exp $
 *
 */

#ifndef _MOD_FILEBIN_H
#define _MOD_FILEBIN_H

#ifdef PHP_WIN32
#define PHP_FILEBIN_API __declspec(dllexport)
#else
#define PHP_FILEBIN_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry filebin_module_entry;
#define filebin_module_ptr &filebin_module_entry

PHP_MINIT_FUNCTION(filebin);
PHP_MSHUTDOWN_FUNCTION(filebin);
PHP_RINIT_FUNCTION(filebin);
PHP_RSHUTDOWN_FUNCTION(filebin);
PHP_MINFO_FUNCTION(filebin);

PHP_FUNCTION(filebin);

#ifdef ZTS
#define FILEBIN_G(v) TSRMG(filebin_globals_id, zend_filebin_globals *, v)
#else
#define FILEBIN_G(v) (filebin_globals.v)

#endif

#define FILEBIN_BUILDNO "200408192144"
#define FILEBIN_BUILDVER "1.0.0"

#define phpext_filebin_ptr filebin_module_ptr

int file_main(int argc, char *argv[]);

#endif  /* _MOD_FILEBIN_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
