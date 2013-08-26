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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_appserver.h"
#include "TSRM.h"
#include "SAPI.h"

#include "zend_API.h"
#include "zend_hash.h"
#include "zend_alloc.h"
#include "zend_operators.h"
#include "zend_globals.h"
#include "zend_compile.h"
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

ZEND_DECLARE_MODULE_GLOBALS(appserver)

/* True global resources - no need for thread safety here */
static int le_appserver;

int (*appserver_orig_header_handler)(sapi_header_struct *h AS_SAPI_HEADER_OP_DC, sapi_headers_struct *s TSRMLS_DC);

/* {{{ appserver_functions[]
 *
 * Every user visible function must have an entry in appserver_functions[].
 */
const zend_function_entry appserver_functions[] = {
	PHP_FE(appserver_get_headers, NULL)
	PHP_FE(exit, NULL)
	PHP_FE(die, NULL)
	PHP_FE_END	/* Must be the last line in appserver_functions[] */
};
/* }}} */

/* {{{ appserver_module_entry
 */
zend_module_entry appserver_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"appserver",
	appserver_functions,
	PHP_MINIT(appserver),
	PHP_MSHUTDOWN(appserver),
	PHP_RINIT(appserver),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(appserver),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(appserver),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */


#ifdef COMPILE_DL_APPSERVER
ZEND_GET_MODULE(appserver)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("appserver.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_appserver_globals, appserver_globals)
    STD_PHP_INI_ENTRY("appserver.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_appserver_globals, appserver_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_appserver_init_globals
 */
static void php_appserver_init_globals(zend_appserver_globals *appserver_globals)
{

}
/* }}} */

static void appserver_llist_string_dtor(void *dummy, void *elem)
{
	char *s = elem;

	if (s) {
		free(s);
	}
}

static int appserver_header_handler(sapi_header_struct *h AS_SAPI_HEADER_OP_DC, sapi_headers_struct *s TSRMLS_DC)
{

#if PHP_VERSION_ID >= 50300
		switch (op) {
			case SAPI_HEADER_ADD:
				appserver_llist_insert_next(APPSERVER_GLOBALS(headers), APPSERVER_LLIST_TAIL(APPSERVER_GLOBALS(headers)), strdup(h->header));
				break;
			case SAPI_HEADER_REPLACE: {
				char *colon_offset = strchr(h->header, ':');

				if (colon_offset) {
					char save = *colon_offset;

					*colon_offset = '\0';
					appserver_header_remove_with_prefix(APPSERVER_GLOBALS(headers), h->header, strlen(h->header) TSRMLS_CC);
					*colon_offset = save;
				}

				appserver_llist_insert_next(APPSERVER_GLOBALS(headers), APPSERVER_LLIST_TAIL(APPSERVER_GLOBALS(headers)), strdup(h->header));
			} break;
			case SAPI_HEADER_DELETE_ALL:
				appserver_llist_empty(APPSERVER_GLOBALS(headers), NULL);
			case SAPI_HEADER_DELETE:
			case SAPI_HEADER_SET_STATUS:
				break;

		}
#else
		appserver_llist_insert_next(APPSERVER_GLOBALS(headers), APPSERVER_LLIST_TAIL(APPSERVER_GLOBALS(headers)), strdup(h->header));
#endif

	if (appserver_orig_header_handler) {
		return appserver_orig_header_handler(h AS_SAPI_HEADER_OP_CC, s TSRMLS_CC);
	}

	return SAPI_HEADER_ADD;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(appserver)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	//APPSERVER_GLOBALS(headers) = appserver_llist_alloc(appserver_llist_string_dtor);



	/* Override header generation in SAPI */
	if (sapi_module.header_handler != appserver_header_handler) {
		appserver_orig_header_handler = sapi_module.header_handler;
		sapi_module.header_handler = appserver_header_handler;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(appserver)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(appserver)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(appserver)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(appserver)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "appserver support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto void appserver_get_headers(void)
*   Print a message to show how much PHP extensions rock */
PHP_FUNCTION(appserver_get_headers)
{
	appserver_llist_element *le;
	char                 	*string;

	array_init(return_value);
	for (le = APPSERVER_LLIST_HEAD(APPSERVER_GLOBALS(headers)); le != NULL; le = APPSERVER_LLIST_NEXT(le)) {
		string = APPSERVER_LLIST_VALP(le);
		add_next_index_string(return_value, string, 1);
	}
}


/* }}} */

/* {{{ proto void exit(void)
*   Print a message to show how much PHP extensions rock */
PHP_FUNCTION(exit)
{
    php_printf("exit");
}
/* }}} */

/* {{{ proto void die(void)
*   Print a message to show how much PHP extensions rock */
PHP_FUNCTION(die)
{
    php_printf("die");
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
