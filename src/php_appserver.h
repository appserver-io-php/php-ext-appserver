/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Johann Zelger <jz@techdivision.com                           |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_APPSERVER_H
#define PHP_APPSERVER_H

extern zend_module_entry appserver_module_entry;
#define phpext_appserver_ptr &appserver_module_entry

#define APPSERVER_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_APPSERVER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_APPSERVER_API __attribute__ ((visibility("default")))
#else
#	define PHP_APPSERVER_API
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

ZEND_BEGIN_MODULE_GLOBALS(appserver)
	appserver_llist 		*headers;
	zend_fcall_info 		override_fci;
	zend_fcall_info_cache 	override_fci_cache;
	long 					pproftrace;
ZEND_END_MODULE_GLOBALS(appserver)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(apd)

#ifdef ZTS
#define APPSERVER_GLOBALS(v) TSRMG(appserver_globals_id, zend_appserver_globals *, v)
#else
#define APPSERVER_GLOBALS(v) (appserver_globals.v)
#endif

#define APPSERVER_TSRMLS_PARAM(param) (param) TSRMLS_CC
#define TEMP_OVRD_FUNC_NAME "__overridden__"

#define PHP_APPSERVER_SPLIT_PN(classname, classname_len, pnname, pnname_len) { \
	char *colon; \
\
	if ((pnname_len) > 3 && (colon = memchr((pnname), ':', (pnname_len) - 2)) && (colon[1] == ':')) { \
		(classname) = (pnname); \
		(classname_len) = colon - (classname); \
		(pnname) = colon + 2; \
		(pnname_len) -= (classname_len) + 2; \
	} else { \
		(classname) = NULL; \
		(classname_len) = 0; \
	} \
}


#endif	/* PHP_APPSERVER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
