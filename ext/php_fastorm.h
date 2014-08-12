
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_FASTORM_H
#define PHP_FASTORM_H 1

#define ZEPHIR_RELEASE 1

#include "kernel/globals.h"

#define PHP_FASTORM_NAME        "Fastorm"
#define PHP_FASTORM_VERSION     "0.1"
#define PHP_FASTORM_EXTNAME     "fastorm"
#define PHP_FASTORM_AUTHOR      "David Menger, Katerina Haslarova"
#define PHP_FASTORM_ZEPVERSION  "0.4.4a"
#define PHP_FASTORM_DESCRIPTION ""



ZEND_BEGIN_MODULE_GLOBALS(fastorm)

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
	
ZEND_END_MODULE_GLOBALS(fastorm)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(fastorm)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) TSRMG(fastorm_globals_id, zend_fastorm_globals *, v)
#else
	#define ZEPHIR_GLOBAL(v) (fastorm_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIR_VGLOBAL ((zend_fastorm_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(fastorm_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(fastorm_globals)
#endif

#define zephir_globals_def fastorm_globals
#define zend_zephir_globals_def zend_fastorm_globals

extern zend_module_entry fastorm_module_entry;
#define phpext_fastorm_ptr &fastorm_module_entry

#endif
