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
#include "zend_constants.h"
#include "zend_closures.h"
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

ZEND_DECLARE_MODULE_GLOBALS(appserver)

static int le_appserver;

int (*appserver_orig_header_handler)(sapi_header_struct *h AS_SAPI_HEADER_OP_DC, sapi_headers_struct *s TSRMLS_DC);

const zend_function_entry appserver_functions[] = {
	PHP_FE(appserver_get_headers, NULL)
	PHP_FE(appserver_override_function, NULL)
	PHP_FE(appserver_rename_function, NULL)
	PHP_FE_END
};

zend_module_entry appserver_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"appserver",
	appserver_functions,
	PHP_MINIT(appserver),
	PHP_MSHUTDOWN(appserver),
	PHP_RINIT(appserver),
	PHP_RSHUTDOWN(appserver),
	PHP_MINFO(appserver),
#if ZEND_MODULE_API_NO >= 20010901
	APPSERVER_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_APPSERVER
ZEND_GET_MODULE(appserver)
#endif


static int php_appserver_constant_remove(char *classname, int classname_len, char *constname, int constname_len TSRMLS_DC)
{
	zend_constant *constant;
	char *found_constname;

	if (classname && (classname_len > 0)) {
#ifdef ZEND_ENGINE_2
		zend_class_entry *ce;

		if (php_runkit_fetch_class(classname, classname_len, &ce TSRMLS_CC)==FAILURE) {
			return FAILURE;
		}

		if (!zend_hash_exists(&ce->constants_table, constname, constname_len+1)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Constant %s::%s does not exist", classname, constname);
			return FAILURE;
		}
		if (zend_hash_del(&ce->constants_table, constname, constname_len+1)==FAILURE) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to remove constant %s::%s", classname, constname);
			return FAILURE;
		}
#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
		php_runkit_clear_all_functions_runtime_cache(TSRMLS_C);
#endif
		return SUCCESS;
#else
		/* PHP4 doesn't support class constants */
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Class constants require PHP 5.0 or above");
		return FAILURE;
#endif
	}

	if (php_runkit_fetch_const(constname, constname_len, &constant, &found_constname TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}

#ifdef ZEND_ENGINE_2
	if (constant->module_number != PHP_USER_CONSTANT) {
#else
	/* Not strictly legal/safe
	 * module_number can't necessarily be counted on to == 0 since it's not initialized (thanks guys)
	 * But do you know of any internal constants that aren't persistent?  I don't.
	 */
	if (constant->flags & CONST_PERSISTENT) {
#endif
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Only user defined constants may be removed.");
		return FAILURE;
	}

	if (zend_hash_del(EG(zend_constants), found_constname, constant->name_len) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to remove constant");
		efree(found_constname);
		return FAILURE;
	}
	efree(found_constname);

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 4) || (PHP_MAJOR_VERSION > 5)
	php_runkit_clear_all_functions_runtime_cache(TSRMLS_C);
#endif

	return SUCCESS;
}




static void appserver_header_remove_with_prefix(appserver_llist *headers, char *prefix, int prefix_len TSRMLS_DC)
{
	appserver_llist_element *le;
	char                 *header;

	for (le = APPSERVER_LLIST_HEAD(APPSERVER_GLOBALS(headers)); le != NULL;) {
		header = APPSERVER_LLIST_VALP(le);

		if ((strlen(header) > prefix_len + 1) && (header[prefix_len] == ':') && (strncasecmp(header, prefix, prefix_len) == 0)) {
			appserver_llist_element *current = le;

			le = APPSERVER_LLIST_NEXT(le);
			appserver_llist_remove(headers, current, NULL);
		} else {
			le = APPSERVER_LLIST_NEXT(le);
		}
	}
}

static int appserver_header_handler(sapi_header_struct *h AS_SAPI_HEADER_OP_DC, sapi_headers_struct *s TSRMLS_DC)
{

	if (APPSERVER_GLOBALS(headers)) {
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
				}
				break;

			case SAPI_HEADER_DELETE_ALL:
				appserver_llist_empty(APPSERVER_GLOBALS(headers), NULL);
				break;

			case SAPI_HEADER_DELETE:
				break;

			case SAPI_HEADER_SET_STATUS:
				break;

		}
#else
		appserver_llist_insert_next(APPSERVER_GLOBALS(headers), APPSERVER_LLIST_TAIL(APPSERVER_GLOBALS(headers)), strdup(h->header));
#endif
	}

	if (appserver_orig_header_handler) {
		return appserver_orig_header_handler(h AS_SAPI_HEADER_OP_CC, s TSRMLS_CC);
	}

	return SAPI_HEADER_ADD;
}

static void php_appserver_shutdown_globals (zend_appserver_globals *appserver_globals TSRMLS_DC)
{

}

static void php_appserver_init_globals(zend_appserver_globals *appserver_globals)
{

	/* Override header generation in SAPI */
	if (sapi_module.header_handler != appserver_header_handler) {
		appserver_orig_header_handler = sapi_module.header_handler;
		sapi_module.header_handler = appserver_header_handler;
	}

	appserver_globals->headers = NULL;

}

static void appserver_llist_string_dtor(void *dummy, void *elem)
{
	char *s = elem;

	if (s) {
		free(s);
	}
}

PHP_MSHUTDOWN_FUNCTION(appserver)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/

#ifdef ZTS
	ts_free_id(appserver_globals_id);
#else
	php_appserver_shutdown_globals(&appserver_globals TSRMLS_CC);
#endif

	return SUCCESS;
}

PHP_MINIT_FUNCTION(appserver)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	ZEND_INIT_MODULE_GLOBALS(appserver, php_appserver_init_globals, php_appserver_shutdown_globals);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(appserver)
{

	APPSERVER_GLOBALS(headers) = appserver_llist_alloc(appserver_llist_string_dtor);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(appserver)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(appserver)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "appserver support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}

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

PHP_FUNCTION(appserver_constant_redefine)
{
	char *classname = NULL, *constname;
	int classname_len = 0, constname_len;
	zval *value;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &constname, &constname_len, &value) == FAILURE) {
		RETURN_FALSE;
	}

	PHP_APPSERVER_SPLIT_PN(classname, classname_len, constname, constname_len);

	php_appserver_constant_remove(classname, classname_len, constname, constname_len TSRMLS_CC);
	RETURN_BOOL(php_appserver_constant_add(classname, classname_len, constname, constname_len, value TSRMLS_CC) == SUCCESS);
}

PHP_FUNCTION(appserver_override_function)
{
	zval *retval_ptr = NULL;
	char *selector;
	int selector_len;
	zend_fcall_info fci;
	zend_fcall_info_cache fcic;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sf", &selector, &selector_len, &fci, &fcic) == FAILURE) {
		return;
	}

	fci.retval_ptr_ptr = &retval_ptr;
	fci.param_count = 0;
	//fci.params = args;
	fci.no_separation = 0;

	zend_call_function(&fci, &fcic TSRMLS_CC);

	php_printf("END");

	/*
	zval *z_function_name;
	int z_function_name_len;
	zend_function *func;
	zval *retval_ptr = NULL;

	php_printf("start function");

	// , &APPSERVER_GLOBALS(override_fci), &APPSERVER_GLOBALS(override_fci_cache)

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sf", &z_function_name, &z_function_name_len, &APPSERVER_GLOBALS(override_fci), &APPSERVER_GLOBALS(override_fci_cache)) == FAILURE) {

	}
*/

	//ZEND_WRONG_PARAM_COUNT();

	//zend_call_function(&APPSERVER_GLOBALS(override_fci), &APPSERVER_GLOBALS(override_fci_cache) TSRMLS_CC);

	// zend_hash_del(EG(function_table), Z_STRVAL_PP(z_function_name), Z_STRLEN_PP(z_function_name) + 1);

	RETURN_TRUE;

/*
	char *eval_code,*eval_name, *temp_function_name;
	int eval_code_length, retval, temp_function_name_length;
	zval **z_function_name, **z_function_args, **z_function_code;

	if (ZEND_NUM_ARGS() != 3 ||
		zend_get_parameters_ex(3, &z_function_name, &z_function_args,
							   &z_function_code) == FAILURE)
		{
			ZEND_WRONG_PARAM_COUNT();
		}

	convert_to_string_ex(z_function_name);
	convert_to_string_ex(z_function_args);
	convert_to_string_ex(z_function_code);

	eval_code_length = sizeof("function " TEMP_OVRD_FUNC_NAME)
		+ Z_STRLEN_PP(z_function_args)
		+ 2
		+ 2
		+ Z_STRLEN_PP(z_function_code);
	eval_code = (char *) emalloc(eval_code_length);
	sprintf(eval_code, "function " TEMP_OVRD_FUNC_NAME "(%s){%s}",
			Z_STRVAL_PP(z_function_args), Z_STRVAL_PP(z_function_code));
	eval_name = zend_make_compiled_string_description("runtime-created override function" TSRMLS_CC);
	retval = zend_eval_string(eval_code, NULL, eval_name TSRMLS_CC);
	efree(eval_code);
	efree(eval_name);

	if (retval == SUCCESS) {
		zend_function *func;

		if (zend_hash_find(EG(function_table), TEMP_OVRD_FUNC_NAME,
						   sizeof(TEMP_OVRD_FUNC_NAME), (void **) &func) == FAILURE)
			{
				zend_error(E_ERROR, "%s() temporary function name not present in global function_table", get_active_function_name(TSRMLS_C));
				RETURN_FALSE;
			}
		function_add_ref(func);
		zend_hash_del(EG(function_table), Z_STRVAL_PP(z_function_name),
					  Z_STRLEN_PP(z_function_name) + 1);
		if(zend_hash_add(EG(function_table), Z_STRVAL_PP(z_function_name),
						 Z_STRLEN_PP(z_function_name) + 1, func, sizeof(zend_function),
						 NULL) == FAILURE)
			{
				RETURN_FALSE;
			}
		RETURN_TRUE;
	}
	else {
		RETURN_FALSE;
	}
	*/
}

PHP_FUNCTION(appserver_rename_function)
{
	zval **z_orig_fname, **z_new_fname;
	zend_function *func, *dummy_func;

	if( ZEND_NUM_ARGS() != 2 ||
		zend_get_parameters_ex(2, &z_orig_fname, &z_new_fname) == FAILURE )
		{
			ZEND_WRONG_PARAM_COUNT();
		}

	convert_to_string_ex(z_orig_fname);
	convert_to_string_ex(z_new_fname);

	if(zend_hash_find(EG(function_table), Z_STRVAL_PP(z_orig_fname),
					  Z_STRLEN_PP(z_orig_fname) + 1, (void **) &func) == FAILURE)
		{
			zend_error(E_WARNING, "%s(%s, %s) failed: %s does not exist!",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_PP(z_orig_fname),  Z_STRVAL_PP(z_new_fname),
					   Z_STRVAL_PP(z_orig_fname));
			RETURN_FALSE;
		}
	if(zend_hash_find(EG(function_table), Z_STRVAL_PP(z_new_fname),
					  Z_STRLEN_PP(z_new_fname) + 1, (void **) &dummy_func) == SUCCESS)
		{
			zend_error(E_WARNING, "%s(%s, %s) failed: %s already exists!",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_PP(z_orig_fname),  Z_STRVAL_PP(z_new_fname),
					   Z_STRVAL_PP(z_new_fname));
			RETURN_FALSE;
		}
	if(zend_hash_add(EG(function_table), Z_STRVAL_PP(z_new_fname),
					 Z_STRLEN_PP(z_new_fname) + 1, func, sizeof(zend_function),
					 NULL) == FAILURE)
		{
			zend_error(E_WARNING, "%s() failed to insert %s into EG(function_table)",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_PP(z_new_fname));
			RETURN_FALSE;
		}
	if(zend_hash_del(EG(function_table), Z_STRVAL_PP(z_orig_fname),
					 Z_STRLEN_PP(z_orig_fname) + 1) == FAILURE)
		{
			zend_error(E_WARNING, "%s() failed to remove %s from function table",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_PP(z_orig_fname));
			zend_hash_del(EG(function_table), Z_STRVAL_PP(z_new_fname),
						  Z_STRLEN_PP(z_new_fname) + 1);
			RETURN_FALSE;
		}
	RETURN_TRUE;
}


// ---------------------------------------------------------------------------
// Zend Extension Functions
// ---------------------------------------------------------------------------
ZEND_DLEXPORT void onStatement(zend_op_array *op_array)
{
	TSRMLS_FETCH();
}

int appserver_zend_startup(zend_extension *extension)
{
	TSRMLS_FETCH();
	CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
	return zend_startup_module(&appserver_module_entry);
}

ZEND_DLEXPORT void appserver_zend_shutdown(zend_extension *extension)
{

}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API	ZEND_DLEXPORT
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
	"Application Server (appserver)",
	APPSERVER_VERSION,
	"TechDivision GmbH",
	"http://appserver.io/",
	"",
	appserver_zend_startup,
	appserver_zend_shutdown,
	NULL,	  		// activate_func_t
	NULL,	   		// deactivate_func_t
	NULL,	   		// message_handler_func_t
	NULL,	   		// op_array_handler_func_t
	onStatement, 	// statement_handler_func_t
	NULL,   		// fcall_begin_handler_func_t
	NULL,   		// fcall_end_handler_func_t
	NULL,	   		// op_array_ctor_func_t
	NULL,	   		// op_array_dtor_func_t
	NULL,	   		// api_no_check
	COMPAT_ZEND_EXTENSION_PROPERTIES
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
