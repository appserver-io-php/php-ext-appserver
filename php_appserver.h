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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_APPSERVER_H
#define PHP_APPSERVER_H

extern zend_module_entry appserver_module_entry;
#define phpext_appserver_ptr &appserver_module_entry

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

#include "zend.h"
#include "zend_API.h"
#include "zend_execute.h"
#include "zend_compile.h"
#include "zend_extensions.h"

#include "appserver_llist.h"
#ifdef ZTS
#include "TSRM.h"
#endif


PHP_MINIT_FUNCTION(appserver);
PHP_MSHUTDOWN_FUNCTION(appserver);
PHP_RINIT_FUNCTION(appserver);
PHP_RSHUTDOWN_FUNCTION(appserver);
PHP_MINFO_FUNCTION(appserver);

PHP_FUNCTION(appserver_get_headers);
PHP_FUNCTION(exit);
PHP_FUNCTION(die);

ZEND_BEGIN_MODULE_GLOBALS(appserver)

	/* headers */
	appserver_llist *headers;

ZEND_END_MODULE_GLOBALS(appserver)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(appserver)

#ifdef ZTS
#define APPSERVER_GLOBALS(v) TSRMG(appserver_globals_id, zend_appserver_globals *, v)
#else
#define APPSERVER_GLOBALS(v) (appserver_globals.v)
#endif

/* In every utility function you add that needs to use variables 
   in php_appserver_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as APPSERVER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#endif	/* PHP_APPSERVER_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
