/*
 * php_version_id.h
 *
 * php magic extension.  
 *
 * Copyright 2021. JoungKyun.Kim all rights reserved.
 *
 * Before PHP 5.2, PHP_VERSION_ID does not exist.
 * For this case, PHP_VERSION_ID is created.
 * This file should be included after php.h or php_version.h.
 *
 */

#ifndef _PHP_VERSION_ID_H
#define _PHP_VERSION_ID_H

/* PHP_VERSION_ID is supported from 5.2 */
#ifndef PHP_VERSION_ID
#if ZEND_MODULE_API_NO > 20050922
#define PHP_VERSION_ID 50100
#elif ZEND_MODULE_API_NO > 20041030
#define PHP_VERSION_ID 50000
#elif ZEND_MODULE_API_NO > 20020429
#define PHP_VERSION_ID 40400
#elif ZEND_MODULE_API_NO > 20020429
#define PHP_VERSION_ID 40300
#elif ZEND_MODULE_API_NO > 20010901
#define PHP_VERSION_ID 40200
#elif ZEND_MODULE_API_NO > 20001222
#define PHP_VERSION_ID 40100
#elif ZEND_MODULE_API_NO == 20001222
#define PHP_VERSION_ID 40000
#endif

#ifndef ZEND_MODULE_API_NO
#define PHP_VERSION_ID 30000
#endif

#endif /* end of ifndef PHP_VERSION_ID */

#endif  /* _PHP_VERSION_ID_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
