/**
 * appserver.c
 *
 * NOTICE OF LICENSE
 *
 * This source file is subject to the Open Software License (OSL 3.0)
 * that is available through the world-wide-web at this URL:
 * http://opensource.org/licenses/osl-3.0.php
 */

/**
 * A php extension for the appserver project (http://appserver.io)
 *
 * It provides various functionality for usage within the appserver runtime.
 *
 * @copyright  	Copyright (c) 2013 <info@techdivision.com> - TechDivision GmbH
 * @license    	http://opensource.org/licenses/osl-3.0.php
 *              Open Software License (OSL 3.0)
 * @author      Johann Zelger <jz@techdivision.com>
 */

#ifndef PHP_APPSERVER_H
#define PHP_APPSERVER_H

extern zend_module_entry appserver_module_entry;
#define phpext_appserver_ptr &appserver_module_entry

#define APPSERVER_NAME "${php.ext.name}"
#define APPSERVER_SAPI_NAME "${php.sapi.name}"
#define APPSERVER_VERSION "${release.version}"
#define APPSERVER_CONSTANT_PHP_SAPI "PHP_SAPI"

#ifdef PHP_WIN32
#    define PHP_APPSERVER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_APPSERVER_API __attribute__ ((visibility("default")))
#else
#    define PHP_APPSERVER_API
#endif


#if PHP_VERSION_ID >= 50300
# define AS_SAPI_HEADER_OP_DC   , sapi_header_op_enum op
# define AS_SAPI_HEADER_OP_CC   , op
#else
# define AS_SAPI_HEADER_OP_DC
# define AS_SAPI_HEADER_OP_CC
#endif

#include "php.h"
#include "main/SAPI.h"
#include "appserver_llist.h"

#include "zend.h"
#include "zend_API.h"
#include "zend_execute.h"
#include "zend_compile.h"
#include "zend_extensions.h"

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(appserver);
PHP_MSHUTDOWN_FUNCTION(appserver);
PHP_RINIT_FUNCTION(appserver);
PHP_RSHUTDOWN_FUNCTION(appserver);
PHP_MINFO_FUNCTION(appserver);

PHP_FUNCTION(appserver_get_headers);
PHP_FUNCTION(appserver_register_file_upload);
PHP_FUNCTION(appserver_set_headers_sent);
PHP_FUNCTION(appserver_redefine);
PHP_FUNCTION(appserver_set_raw_post_data);
PHP_FUNCTION(appserver_get_http_response_code);

ZEND_BEGIN_MODULE_GLOBALS(appserver)
    appserver_llist         *headers;
	HashTable               *uploaded_files;
	HashTable                redefined;
    long                     pproftrace;
ZEND_END_MODULE_GLOBALS(appserver)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(apd)

#ifdef ZTS
#define APPSERVER_GLOBALS(v) TSRMG(appserver_globals_id, zend_appserver_globals *, v)
#else
#define APPSERVER_GLOBALS(v) (appserver_globals.v)
#endif

#define APPSERVER_TSRMLS_PARAM(param) (param) TSRMLS_CC
#define TEMP_OVRD_FUNC_NAME "__overridden__"


#endif    /* PHP_APPSERVER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
