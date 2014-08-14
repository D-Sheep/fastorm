
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_FASTORM_H
#define PHP_FASTORM_H 1

#define ZEPHIR_RELEASE 1

#define PHP_FASTORM_NAME        "Fastorm"
#define PHP_FASTORM_VERSION     "0.3"
#define PHP_FASTORM_EXTNAME     "fastorm"
#define PHP_FASTORM_AUTHOR      "David Menger, Katerina Haslarova"
#define PHP_FASTORM_ZEPVERSION  "0.4.4a"
#define PHP_FASTORM_DESCRIPTION ""

#include "main/php.h"


#define ZEPHIR_MAX_MEMORY_STACK 48

/** Memory frame */
typedef struct _zephir_memory_entry {
	size_t pointer;
	size_t capacity;
	zval ***addresses;
	size_t hash_pointer;
	size_t hash_capacity;
	zval ***hash_addresses;
	struct _zephir_memory_entry *prev;
	struct _zephir_memory_entry *next;
#ifndef ZEPHIR_RELEASE
	const char *func;
#endif
} zephir_memory_entry;

/** Virtual Symbol Table */
typedef struct _zephir_symbol_table {
	struct _zephir_memory_entry *scope;
	HashTable *symbol_table;
	struct _zephir_symbol_table *prev;
} zephir_symbol_table;

typedef struct _zephir_function_cache {
	zend_class_entry *ce;
	zend_function *func;
} zephir_function_cache;

#if PHP_VERSION_ID >= 50400
	#define ZEPHIR_INIT_FUNCS(class_functions) static const zend_function_entry class_functions[] =
#else
	#define ZEPHIR_INIT_FUNCS(class_functions) static const function_entry class_functions[] =
#endif

#ifndef PHP_FE_END
	#define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

/** Define FASTCALL */
#if defined(__GNUC__) && ZEND_GCC_VERSION >= 3004 && defined(__i386__)
# define ZEPHIR_FASTCALL __attribute__((fastcall))
#elif defined(_MSC_VER) && defined(_M_IX86)
# define ZEPHIR_FASTCALL __fastcall
#else
# define ZEPHIR_FASTCALL
#endif

#define ZEPHIR_INIT_CLASS(name) \
	int zephir_ ##name## _init(INIT_FUNC_ARGS)

#define ZEPHIR_INIT(name) \
	if (zephir_ ##name## _init(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) { \
		return FAILURE; \
	}

/* Compatibility macros for PHP 5.3 */
#ifndef PHP_FE_END
#define PHP_FE_END { NULL, NULL, NULL, 0, 0 }
#endif

#ifndef INIT_PZVAL_COPY
# define INIT_PZVAL_COPY(z, v) \
	ZVAL_COPY_VALUE(z, v); \
	Z_SET_REFCOUNT_P(z, 1); \
	Z_UNSET_ISREF_P(z);
#endif

#ifndef ZVAL_COPY_VALUE
# define ZVAL_COPY_VALUE(z, v) \
	(z)->value = (v)->value; \
	Z_TYPE_P(z) = Z_TYPE_P(v);
#endif

#ifndef HASH_KEY_NON_EXISTENT
# define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

/** Macros for branch prediction */
#define likely(x) EXPECTED(x)
#define unlikely(x) UNEXPECTED(x)

#if defined(__GNUC__) && (defined(__clang__) || ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405))
# define UNREACHABLE() __builtin_unreachable()
# define ASSUME(x) if (x) {} else __builtin_unreachable()
#else
# define UNREACHABLE() assert(0)
# define ASSUME(x) assert(!!(x));
#endif

#if defined(__GNUC__) || defined(__clang__)
# define ZEPHIR_ATTR_NONNULL __attribute__((nonnull))
# define ZEPHIR_ATTR_NONNULL1(x) __attribute__((nonnull (x)))
# define ZEPHIR_ATTR_NONNULL2(x, y) __attribute__((nonnull (x, y)))
# define ZEPHIR_ATTR_NONNULL3(x, y, z) __attribute__((nonnull (x, y, z)))
# define ZEPHIR_ATTR_PURE __attribute__((pure))
# define ZEPHIR_ATTR_CONST __attribute__((const))
# define ZEPHIR_ATTR_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define ZEPHIR_ATTR_NONNULL
# define ZEPHIR_ATTR_NONNULL1(x)
# define ZEPHIR_ATTR_NONNULL2(x, y)
# define ZEPHIR_ATTR_NONNULL3(x, y, z)
# define ZEPHIR_ATTR_PURE
# define ZEPHIR_ATTR_CONST
# define ZEPHIR_ATTR_WARN_UNUSED_RESULT
#endif

#if !defined(__GNUC__) && !(defined(__SUNPRO_C) && (__SUNPRO_C >= 0x590))
# define __builtin_constant_p(s) (0)
#endif

#ifndef ZEND_MOD_END
# define ZEND_MOD_END { NULL, NULL, NULL, 0 }
#endif

#ifndef __func__
# define __func__ __FUNCTION__
#endif

/*#if PHP_VERSION_ID > 50399
# define ZLK_DC , const struct _zend_literal* key
# define ZLK_CC , key
# define ZLK_NULL_CC , NULL
#else
# define ZLK_DC
# define ZLK_CC
# define ZLK_NULL_CC
#endif*/

#ifdef ZTS
#define zephir_nts_static
#else
#define zephir_nts_static static
#endif




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
