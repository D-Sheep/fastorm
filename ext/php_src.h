
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_SRC_H
#define PHP_SRC_H 1

#define ZEPHIR_RELEASE 1

#include "kernel/globals.h"

#define PHP_SRC_NAME        ""
#define PHP_SRC_VERSION     "2.0.0a"
#define PHP_SRC_EXTNAME     "src"
#define PHP_SRC_AUTHOR      "David Menger"
#define PHP_SRC_ZEPVERSION  "0.4.4a"
#define PHP_SRC_DESCRIPTION ""



ZEND_BEGIN_MODULE_GLOBALS(src)

	/* Memory */
	zephir_memory_entry *start_memory; /**< The first preallocated frame */
	zephir_memory_entry *end_memory; /**< The last preallocate frame */
	zephir_memory_entry *active_memory; /**< The current memory frame */

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *fcache;

	/* Max recursion control */
	unsigned int recursive_lock;

	/* Global constants */
	zval *global_true;
	zval *global_false;
	zval *global_null;
	
ZEND_END_MODULE_GLOBALS(src)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(src)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) TSRMG(src_globals_id, zend_src_globals *, v)
#else
	#define ZEPHIR_GLOBAL(v) (src_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIR_VGLOBAL ((zend_src_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(src_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(src_globals)
#endif

#define zephir_globals_def src_globals
#define zend_zephir_globals_def zend_src_globals

extern zend_module_entry src_module_entry;
#define phpext_src_ptr &src_module_entry

#endif
