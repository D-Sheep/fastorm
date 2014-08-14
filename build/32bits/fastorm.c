/**

*/

#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_fastorm.h"
#include "fastorm.h"

#include "main/php_main.h"
#include "main/php_streams.h"
#include "main/php_output.h"
#include "main/php_ini.h"
#include "main/SAPI.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_smart_str.h"
#include "ext/standard/info.h"
#include "ext/pdo/php_pdo_driver.h"
#include "ext/standard/file.h"
#include "ext/standard/php_filestat.h"
#include "ext/standard/php_rand.h"
#include "ext/standard/php_lcg.h"
#include "ext/standard/php_math.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_var.h"
#include "ext/standard/php_http.h"
#include "ext/standard/html.h"
#include "ext/standard/base64.h"
#include "ext/standard/md5.h"
#include "ext/standard/head.h"
#include "ext/standard/url.h"
#include "ext/spl/spl_heap.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/spl/spl_directory.h"
#include "ext/date/php_date.h"

#ifdef PHALCON_USE_PHP_PCRE
#include "ext/pcre/php_pcre.h"
#endif

#ifdef PHALCON_USE_PHP_JSON
#include "ext/json/php_json.h"
#endif

#ifdef PHALCON_USE_PHP_SESSION
#include "ext/session/php_session.h"
#endif

#include "Zend/zend_API.h"
#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_execute.h"
#include "Zend/zend_builtin_functions.h"



#ifndef ZEPHIR_KERNEL_MAIN_H
#define ZEPHIR_KERNEL_MAIN_H


/** Main macros */
#define PH_DEBUG 0

#define PH_NOISY 256
#define PH_SILENT 1024
#define PH_READONLY 4096

#define PH_NOISY_CC PH_NOISY TSRMLS_CC
#define PH_SILENT_CC PH_SILENT TSRMLS_CC

#define PH_SEPARATE 256
#define PH_COPY 1024
#define PH_CTOR 4096

#define SL(str) ZEND_STRL(str)
#define SS(str) ZEND_STRS(str)
#define ISL(str) (zephir_interned_##str), (sizeof(#str)-1)
#define ISS(str) (zephir_interned_##str), (sizeof(#str))


/* Compatibility with PHP 5.3 */
#ifndef ZVAL_COPY_VALUE
 #define ZVAL_COPY_VALUE(z, v)\
  (z)->value = (v)->value;\
  Z_TYPE_P(z) = Z_TYPE_P(v);
#endif

#ifndef INIT_PZVAL_COPY
 #define INIT_PZVAL_COPY(z, v) ZVAL_COPY_VALUE(z, v);\
  Z_SET_REFCOUNT_P(z, 1);\
  Z_UNSET_ISREF_P(z);
#endif

/* Startup functions */
zend_class_entry *zephir_register_internal_interface_ex(zend_class_entry *orig_ce, zend_class_entry *parent_ce TSRMLS_DC);

/* Globals functions */
int zephir_init_global(char *global, unsigned int global_length TSRMLS_DC);
int zephir_get_global(zval **arr, const char *global, unsigned int global_length TSRMLS_DC);

int zephir_is_callable(zval *var TSRMLS_DC);
int zephir_function_exists(const zval *function_name TSRMLS_DC);
int zephir_function_exists_ex(const char *func_name, unsigned int func_len TSRMLS_DC);
int zephir_function_quick_exists_ex(const char *func_name, unsigned int func_len, unsigned long key TSRMLS_DC);
zend_class_entry* zephir_get_internal_ce(const char *class_name, unsigned int class_name_len TSRMLS_DC);

/* types */
void zephir_gettype(zval *return_value, zval *arg TSRMLS_DC);

/* Count */
void zephir_fast_count(zval *result, zval *array TSRMLS_DC);
int zephir_fast_count_ev(zval *array TSRMLS_DC);
int zephir_fast_count_int(zval *value TSRMLS_DC);

/* Utils functions */
static inline int zephir_maybe_separate_zval(zval** z)
{
	if (Z_REFCOUNT_PP(z) > 1 && !Z_ISREF_PP(z)) {
		zval *new_zv;

		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, *z);
		*z = new_zv;
		zval_copy_ctor(new_zv);

		return 1;
	}

	return 0;
}

int zephir_is_iterable_ex(zval *arr, HashTable **arr_hash, HashPosition *hash_position, int duplicate, int reverse);
void zephir_safe_zval_ptr_dtor(zval *pzval);

/* Fetch Parameters */
int zephir_fetch_parameters(int num_args TSRMLS_DC, int required_args, int optional_args, ...);

/** Symbols */
#define ZEPHIR_READ_SYMBOL(var, auxarr, name) if (EG(active_symbol_table)){ \
	if (zend_hash_find(EG(active_symbol_table), name, sizeof(name), (void **)  &auxarr) == SUCCESS) { \
			var = *auxarr; \
		} else { \
			ZVAL_NULL(var); \
		} \
	} else { \
		ZVAL_NULL(var); \
	}

#define RETURN_ON_FAILURE(what) \
	do { \
		if (what == FAILURE) { \
			return; \
		} \
	} while (0)

#define RETURN_MM_ON_FAILURE(what) \
	do { \
		if (what == FAILURE) { \
			ZEPHIR_MM_RESTORE(); \
			return; \
		} \
	} while (0)

#define RETURN_CCTOR(var) { \
		*(return_value) = *(var); \
		if (Z_TYPE_P(var) > IS_BOOL) { \
			zephir_copy_ctor(return_value, var); \
		} \
		INIT_PZVAL(return_value) \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

#define RETURN_CCTORW(var) { \
		*(return_value) = *(var); \
		if (Z_TYPE_P(var) > IS_BOOL) { \
			zephir_copy_ctor(return_value, var); \
		} \
		INIT_PZVAL(return_value) \
	} \
	return;

#define RETURN_CTOR(var) { \
		RETVAL_ZVAL(var, 1, 0); \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

#define RETURN_CTORW(var) { \
		RETVAL_ZVAL(var, 1, 0); \
	} \
	return;

#define RETURN_THIS() { \
		RETVAL_ZVAL(this_ptr, 1, 0); \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

#define RETURN_THISW() \
	RETURN_ZVAL(this_ptr, 1, 0);

#define RETURN_NCTOR(var) { \
		*(return_value) = *(var); \
		INIT_PZVAL(return_value) \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

#define RETURN_NCTORW(var) { \
		*(return_value) = *(var); \
		INIT_PZVAL(return_value) \
	} \
	return;

#define RETURN_SCTOR() \
	if (Z_TYPE_P(return_value) > IS_BOOL) { \
		zval_copy_ctor(return_value); \
	} \
	ZEPHIR_MM_RESTORE(); \
	return;

#define RETURN_LCTOR(var) { \
	zend_uchar is_ref = Z_ISREF_P(return_value);       \
	zend_uint refcount = Z_REFCOUNT_P(return_value);   \
	(return_value)->value = var.value;                \
	Z_TYPE_P(return_value) = Z_TYPE(var);              \
	zval_copy_ctor(return_value);                      \
	Z_SET_ISREF_TO_P(return_value, is_ref);            \
	Z_SET_REFCOUNT_P(return_value, refcount);          \
	ZEPHIR_MM_RESTORE(); \
	return; \
}

#define RETURN_LCTORW(var) { \
	zend_uchar is_ref = Z_ISREF_P(return_value);       \
	zend_uint refcount = Z_REFCOUNT_P(return_value);   \
	(return_value)->value = var.value;                \
	Z_TYPE_P(return_value) = Z_TYPE(var);              \
	zval_copy_ctor(return_value);                      \
	Z_SET_ISREF_TO_P(return_value, is_ref);            \
	Z_SET_REFCOUNT_P(return_value, refcount);          \
	return; \
}

#define RETURN_SCTORW() \
	if (Z_TYPE_P(return_value) > IS_BOOL) { \
		zval_copy_ctor(return_value); \
	} \
	return;

#define RETURN_MEMBER(object, member_name) \
	zephir_return_property_quick(return_value, return_value_ptr, object, SL(member_name), zend_inline_hash_func(SS(member_name)) TSRMLS_CC); \
	return;

#define RETURN_MM_MEMBER(object, member_name) \
  zephir_return_property_quick(return_value, return_value_ptr, object, SL(member_name), zend_inline_hash_func(SS(member_name)) TSRMLS_CC); \
  RETURN_MM();

#define RETURN_MEMBER_QUICK(object, member_name, key) \
 	zephir_return_property_quick(return_value, NULL, object, SL(member_name), key TSRMLS_CC); \
	return;

#define RETURN_MM_MEMBER_QUICK(object, member_name, key) \
  zephir_return_property_quick(return_value, NULL, object, SL(member_name), key TSRMLS_CC); \
  RETURN_MM();

/** Return without change return_value */
#define RETURN_MM()                 { ZEPHIR_MM_RESTORE(); return; }

/** Return null restoring memory frame */
#define RETURN_MM_BOOL(value)       { RETVAL_BOOL(value); ZEPHIR_MM_RESTORE(); return; }

/** Return null restoring memory frame */
#define RETURN_MM_NULL()            { RETVAL_NULL(); ZEPHIR_MM_RESTORE(); return; }

/** Return bool restoring memory frame */
#define RETURN_MM_FALSE             { RETVAL_FALSE; ZEPHIR_MM_RESTORE(); return; }
#define RETURN_MM_TRUE              { RETVAL_TRUE; ZEPHIR_MM_RESTORE(); return; }

/** Return string restoring memory frame */
#define RETURN_MM_STRING(str, copy) { RETVAL_STRING(str, copy); ZEPHIR_MM_RESTORE(); return; }
#define RETURN_MM_EMPTY_STRING()    { RETVAL_EMPTY_STRING(); ZEPHIR_MM_RESTORE(); return; }

/** Return empty array */
#define RETVAL_EMPTY_ARRAY()        { array_init(return_value); }
#define RETURN_EMPTY_ARRAY()        { RETVAL_EMPTY_ARRAY(); return; }
#define RETURN_MM_EMPTY_ARRAY()     { RETVAL_EMPTY_ARRAY(); ZEPHIR_MM_RESTORE(); return; }

/* Return long */
#define RETURN_MM_LONG(value)       { RETVAL_LONG(value); ZEPHIR_MM_RESTORE(); return; }

/* Return double */
#define RETURN_MM_DOUBLE(value)     { RETVAL_DOUBLE(value); ZEPHIR_MM_RESTORE(); return; }

#ifndef IS_INTERNED
#define IS_INTERNED(key) 0
#define INTERNED_HASH(key) 0
#endif

/** Get the current hash key without copying the hash key */
#define ZEPHIR_GET_HKEY(var, hash, hash_position) \
	zephir_get_current_key(&var, hash, &hash_position TSRMLS_CC);

/** Get current hash key copying the hash_value if needed */
#define ZEPHIR_GET_HMKEY(var, hash, hash_pointer) \
	{\
		int hash_type; \
		char *hash_index; \
		uint hash_index_len; \
		ulong hash_num; \
		 \
		ZEPHIR_INIT_NVAR(var); \
		hash_type = zend_hash_get_current_key_ex(hash, &hash_index, &hash_index_len, &hash_num, 0, &hash_pointer); \
		if (hash_type == HASH_KEY_IS_STRING) { \
			if (IS_INTERNED(hash_index)) { \
				ZVAL_STRINGL(var, hash_index, hash_index_len - 1, 0); \
			} else { \
				ZVAL_STRINGL(var, hash_index, hash_index_len - 1, 1); \
			} \
		} else { \
			if (hash_type == HASH_KEY_IS_LONG) { \
				ZVAL_LONG(var, hash_num); \
			}\
		}\
	}

/** Foreach */
#define ZEPHIR_GET_FOREACH_KEY(var, hash, hash_pointer) ZEPHIR_GET_HMKEY(var, hash, hash_pointer)

/** Check if an array is iterable or not */
#define zephir_is_iterable(var, array_hash, hash_pointer, duplicate, reverse, file, line) \
	if (!var || !zephir_is_iterable_ex(var, array_hash, hash_pointer, duplicate, reverse)) { \
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_exception_get_default(TSRMLS_C), "The argument is not initialized or iterable()", file, line); \
		ZEPHIR_MM_RESTORE(); \
		return; \
	}

#define ZEPHIR_GET_FOREACH_VALUE(var) \
	ZEPHIR_OBS_NVAR(var); \
	var = *hd; \
	Z_ADDREF_P(var);

#define ZEPHIR_GET_HVALUE(var, hd) \
	ZEPHIR_OBS_NVAR(var); \
	var = *hd; \
	Z_ADDREF_P(var);

/** class/interface registering */
#define ZEPHIR_REGISTER_CLASS(ns, class_name, lower_ns, name, methods, flags) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		lower_ns## _ ##name## _ce = zend_register_internal_class(&ce TSRMLS_CC); \
		lower_ns## _ ##name## _ce->ce_flags |= flags;  \
	}

#define ZEPHIR_REGISTER_CLASS_EX(ns, class_name, lower_ns, lcname, parent_ce, methods, flags) \
	{ \
		zend_class_entry ce; \
		if (!parent_ce) { \
			fprintf(stderr, "Can't register class %s::%s with null parent\n", #ns, #class_name); \
			return FAILURE; \
		} \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #class_name, methods); \
		lower_ns## _ ##lcname## _ce = zend_register_internal_class_ex(&ce, parent_ce, NULL TSRMLS_CC); \
		if (!lower_ns## _ ##lcname## _ce) { \
			fprintf(stderr, "Phalcon Error: Class to extend '%s' was not found when registering class '%s'\n", (parent_ce ? parent_ce->name : "(null)"), ZEND_NS_NAME(#ns, #class_name)); \
			return FAILURE; \
		} \
		lower_ns## _ ##lcname## _ce->ce_flags |= flags;  \
	}

#define ZEPHIR_REGISTER_INTERFACE(ns, classname, lower_ns, name, methods) \
	{ \
		zend_class_entry ce; \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		lower_ns## _ ##name## _ce = zend_register_internal_interface(&ce TSRMLS_CC); \
	}

#define ZEPHIR_REGISTER_INTERFACE_EX(ns, classname, lower_ns, lcname, parent_ce, methods) \
	{ \
		zend_class_entry ce; \
		if (!parent_ce) { \
			fprintf(stderr, "Can't register interface %s with null parent\n", ZEND_NS_NAME(#ns, #classname)); \
			return FAILURE; \
		} \
		memset(&ce, 0, sizeof(zend_class_entry)); \
		INIT_NS_CLASS_ENTRY(ce, #ns, #classname, methods); \
		lower_ns## _ ##lcname## _ce = zephir_register_internal_interface_ex(&ce, parent_ce TSRMLS_CC); \
		if (!lower_ns## _ ##lcname## _ce) { \
			fprintf(stderr, "Can't register interface %s with parent %s\n", ZEND_NS_NAME(#ns, #classname), (parent_ce ? parent_ce->name : "(null)")); \
			return FAILURE; \
		} \
	}

/** Method declaration for API generation */
#define ZEPHIR_DOC_METHOD(class_name, method)

/** Low overhead parse/fetch parameters */
#define zephir_fetch_params(memory_grow, required_params, optional_params, ...) \
	if (zephir_fetch_parameters(ZEND_NUM_ARGS() TSRMLS_CC, required_params, optional_params, __VA_ARGS__) == FAILURE) { \
		if (memory_grow) { \
			RETURN_MM_NULL(); \
		} else { \
			RETURN_NULL(); \
		} \
	}

#define ZEPHIR_VERIFY_INTERFACE(instance, interface_ce) \
	do { \
		if (Z_TYPE_P(instance) != IS_OBJECT || !instanceof_function_ex(Z_OBJCE_P(instance), interface_ce, 1 TSRMLS_CC)) { \
			char *buf; \
			if (Z_TYPE_P(instance) != IS_OBJECT) { \
				spprintf(&buf, 0, "Unexpected value type: expected object implementing %s, %s given", interface_ce->name, zend_zval_type_name(instance)); \
			} \
			else { \
				spprintf(&buf, 0, "Unexpected value type: expected object implementing %s, object of type %s given", interface_ce->name, Z_OBJCE_P(instance)->name); \
			} \
			ZEPHIR_THROW_EXCEPTION_STR(spl_ce_LogicException, buf); \
			efree(buf); \
			return; \
		} \
	} while (0)


#define ZEPHIR_VERIFY_CLASS(instance, class_ce) \
	do { \
		if (Z_TYPE_P(instance) != IS_OBJECT || !instanceof_function_ex(Z_OBJCE_P(instance), class_ce, 0 TSRMLS_CC)) { \
			char *buf; \
			if (Z_TYPE_P(instance) != IS_OBJECT) { \
				spprintf(&buf, 0, "Unexpected value type: expected object of type %s, %s given", class_ce->name, zend_zval_type_name(instance)); \
			} \
			else { \
				spprintf(&buf, 0, "Unexpected value type: expected object of type %s, object of type %s given", class_ce->name, Z_OBJCE_P(instance)->name); \
			} \
			ZEPHIR_THROW_EXCEPTION_STR(spl_ce_LogicException, buf); \
			efree(buf); \
			return; \
		} \
	} while (0)

#define ZEPHIR_GET_CONSTANT(return_value, const_name) \
	RETURN_MM_ON_FAILURE(zend_get_constant(SL(const_name), return_value TSRMLS_CC));

#ifndef ZEPHIR_RELEASE
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#else
#define ZEPHIR_DEBUG_PARAMS , const char *file, int line
#define ZEPHIR_DEBUG_PARAMS_DUMMY , "", 0
#endif

#define ZEPHIR_CHECK_POINTER(v) if (!v) fprintf(stderr, "%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

#endif /* ZEPHIR_KERNEL_MAIN_H */




#ifndef ZEPHIR_KERNEL_MEMORY_H
#define ZEPHIR_KERNEL_MEMORY_H

/* Variable Tracking */
void zephir_init_nvar(zval **var TSRMLS_DC);
void zephir_cpy_wrt(zval **dest, zval *var TSRMLS_DC);
void zephir_cpy_wrt_ctor(zval **dest, zval *var TSRMLS_DC);

void zephir_value_dtor(zval *zvalue ZEND_FILE_LINE_DC);

/* Memory Frames */
#ifndef ZEPHIR_RELEASE
void ZEPHIR_FASTCALL zephir_memory_grow_stack(const char *func TSRMLS_DC);
int ZEPHIR_FASTCALL zephir_memory_restore_stack(const char *func TSRMLS_DC);

#define ZEPHIR_MM_GROW() zephir_memory_grow_stack(NULL TSRMLS_CC)
#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack(NULL TSRMLS_CC)

#else
void ZEPHIR_FASTCALL zephir_memory_grow_stack(TSRMLS_D);
int ZEPHIR_FASTCALL zephir_memory_restore_stack(TSRMLS_D);

#define ZEPHIR_MM_GROW() zephir_memory_grow_stack(TSRMLS_C)
#define ZEPHIR_MM_RESTORE() zephir_memory_restore_stack(TSRMLS_C)

#endif

void ZEPHIR_FASTCALL zephir_memory_observe(zval **var TSRMLS_DC);
void ZEPHIR_FASTCALL zephir_memory_remove(zval **var TSRMLS_DC);
void ZEPHIR_FASTCALL zephir_memory_alloc(zval **var TSRMLS_DC);
void ZEPHIR_FASTCALL zephir_memory_alloc_pnull(zval **var TSRMLS_DC);

int ZEPHIR_FASTCALL zephir_clean_restore_stack(TSRMLS_D);

/* Virtual symbol tables */
void zephir_create_symbol_table(TSRMLS_D);
/*void zephir_restore_symbol_table(TSRMLS_D);*/
void zephir_clean_symbol_tables(TSRMLS_D);

/** Export symbols to active symbol table */
int zephir_set_symbol(zval *key_name, zval *value TSRMLS_DC);
int zephir_set_symbol_str(char *key_name, unsigned int key_length, zval *value TSRMLS_DC);

void ZEPHIR_FASTCALL zephir_copy_ctor(zval *destiny, zval *origin);

/* Memory macros */
#define ZEPHIR_ALLOC_ZVAL(z) \
	ALLOC_INIT_ZVAL(z)

#define ZEPHIR_SINIT_VAR(z) \
	INIT_PZVAL(&z); \
	ZVAL_NULL(&z);

#define ZEPHIR_SINIT_NVAR(z) Z_SET_REFCOUNT_P(&z, 1)

#define ZEPHIR_INIT_ZVAL_NREF(z) \
	ALLOC_ZVAL(z); \
	Z_SET_REFCOUNT_P(z, 0); \
	Z_UNSET_ISREF_P(z);

#define ZEPHIR_INIT_VAR(z) \
	zephir_memory_alloc(&z TSRMLS_CC)

#define ZEPHIR_INIT_NVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
		} else {\
			zval_dtor(z); \
		} \
		ZVAL_NULL(z); \
	} else { \
		zephir_memory_alloc(&z TSRMLS_CC); \
	}

#define ZEPHIR_INIT_BNVAR(z) \
	if (Z_REFCOUNT_P(z) > 1) { \
		Z_DELREF_P(z); \
		ALLOC_ZVAL(z); \
		Z_SET_REFCOUNT_P(z, 1); \
		Z_UNSET_ISREF_P(z); \
		ZVAL_NULL(z); \
	} else {\
		zval_ptr_dtor(&z); \
		ZEPHIR_ALLOC_ZVAL(z); \
	}

#define ZEPHIR_INIT_NVAR_PNULL(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			if (Z_REFCOUNT_P(z) >= 1) { \
				zval_copy_ctor(z); \
			} \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
		} \
		ZVAL_NULL(z); \
	} else { \
		zephir_memory_alloc_pnull(&z TSRMLS_CC); \
	}

/* only removes the value body of the zval */
#define ZEPHIR_INIT_LNVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
			ALLOC_ZVAL(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
			ZVAL_NULL(z); \
		} else {\
			zephir_value_dtor(z ZEND_FILE_LINE_CC); \
			ZVAL_NULL(z); \
		} \
	} else { \
		zephir_memory_alloc(&z TSRMLS_CC); \
	}

#define ZEPHIR_CPY_WRT(d, v) \
	if (d) { \
		if (Z_REFCOUNT_P(d) > 0) { \
			zval_ptr_dtor(&d); \
		} \
	} else { \
		zephir_memory_observe(&d TSRMLS_CC); \
	} \
	Z_ADDREF_P(v); \
	d = v;

#define ZEPHIR_CPY_WRT_CTOR(d, v) \
	if (d) { \
		if (Z_REFCOUNT_P(d) > 0) { \
			zval_ptr_dtor(&d); \
		} \
	} else { \
		zephir_memory_observe(&d TSRMLS_CC); \
	} \
	ALLOC_ZVAL(d); \
	*d = *v; \
	zval_copy_ctor(d); \
	Z_SET_REFCOUNT_P(d, 1); \
	Z_UNSET_ISREF_P(d);

/* */
#define ZEPHIR_OBS_VAR(z) \
	zephir_memory_observe(&z TSRMLS_CC)

#define ZEPHIR_OBS_NVAR(z)\
	if (z) { \
		if (Z_REFCOUNT_P(z) > 1) { \
			Z_DELREF_P(z); \
		} else {\
			zval_ptr_dtor(&z); \
			z = NULL; \
		} \
	} else { \
		zephir_memory_observe(&z TSRMLS_CC); \
	}

#define ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(ppzv) \
	do { \
		zval **tmp_ = (ppzv); \
		if (tmp_ != NULL) { \
			if (*tmp_) { \
				zval_ptr_dtor(tmp_); \
				*tmp_ = NULL; \
			} \
			else { \
				zephir_memory_observe((ppzv) TSRMLS_CC); \
			} \
		} \
	} while (0)

#define ZEPHIR_OBSERVE_OR_NULLIFY_VAR(z) \
	do { \
		if (z) { \
			zval_ptr_dtor(&z); \
			z = NULL; \
		} \
		else { \
			zephir_memory_observe(&z TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_SEPARATE_ARRAY(a) \
	{ \
		if (Z_REFCOUNT_P(a) > 1) { \
			zval *new_zv; \
			Z_DELREF_P(a); \
			ALLOC_ZVAL(new_zv); \
			INIT_PZVAL_COPY(new_zv, a); \
			a = new_zv; \
			zval_copy_ctor(new_zv); \
		} \
	}

#define ZEPHIR_SEPARATE(z) SEPARATE_ZVAL(&z)

#define ZEPHIR_SEPARATE_PARAM(z) \
	do { \
		zval *orig_ptr = z;\
		zephir_memory_observe(&z TSRMLS_CC);\
		ALLOC_ZVAL(z);\
		*z = *orig_ptr;\
		zval_copy_ctor(z);\
		Z_SET_REFCOUNT_P(z, 1);\
		Z_UNSET_ISREF_P(z);\
	} while (0)

#define ZEPHIR_SEPARATE_PARAM_NMO(z) { \
		zval *orig_ptr = z; \
		if (Z_REFCOUNT_P(orig_ptr) > 1) { \
			ALLOC_ZVAL(z); \
			*z = *orig_ptr; \
			zval_copy_ctor(z); \
			Z_SET_REFCOUNT_P(z, 1); \
			Z_UNSET_ISREF_P(z); \
		} \
	}

#endif




#ifndef ZEPHIR_KERNEL_BACKTRACE_H
#define ZEPHIR_KERNEL_BACKTRACE_H

#ifndef ZEPHIR_RELEASE

void zephir_print_backtrace(void);

#else

#ifndef zephir_print_backtrace
#define zephir_print_backtrace()
#endif

#endif
#endif /* ZEPHIR_KERNEL_BACKTRACE_H */



#ifndef ZEPHIR_KERNEL_FCALL_H
#define ZEPHIR_KERNEL_FCALL_H

#include <Zend/zend_hash.h>
#include <Zend/zend.h>

typedef enum _zephir_call_type {
	zephir_fcall_parent,
	zephir_fcall_self,
	zephir_fcall_static,
	zephir_fcall_ce,
	zephir_fcall_method,
	zephir_fcall_function
} zephir_call_type;

#ifndef ZEPHIR_RELEASE

typedef struct _zephir_fcall_cache_entry {
	zend_function *f;
	zend_uint times;
} zephir_fcall_cache_entry;

#else

typedef zend_function zephir_fcall_cache_entry;

#endif


#define ZEPHIR_CALL_FUNCTIONW(return_value_ptr, func_name, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, sizeof(func_name)-1, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, strlen(func_name), sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_FUNCTION(return_value_ptr, func_name, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, sizeof(func_name)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_func_aparams(return_value_ptr, func_name, strlen(func_name), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_FUNCTIONW(func_name, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, return_value_ptr, func_name, sizeof(func_name)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, return_value_ptr, func_name, strlen(func_name), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_FUNCTION(func_name, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, return_value_ptr, func_name, sizeof(func_name)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_function(return_value, return_value_ptr, func_name, strlen(func_name), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_ZVAL_FUNCTION(return_value_ptr, func_name, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_zval_func_aparams(return_value_ptr, func_name, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_zval_func_aparams(return_value_ptr, func_name, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(func_name, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(func_name)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_zval_function(return_value, return_value_ptr, func_name, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_zval_function(return_value, return_value_ptr, func_name, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)


#define ZEPHIR_CALL_METHODW(return_value_ptr, object, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_METHOD(return_value_ptr, object, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_METHODW(object, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_METHOD(object, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL, zephir_fcall_method, object, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_METHOD_THIS(return_value_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, THIS_CE, zephir_fcall_method, this_ptr, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, THIS_CE, zephir_fcall_method, this_ptr, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_PARENTW(return_value_ptr, class_entry, this_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_PARENT(return_value_ptr, class_entry, this_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_PARENTW(class_entry, this_ptr, method, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_PARENT(class_entry, this_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_parent, this_ptr, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_SELFW(return_value_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, cache, sizeof(method)-1, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, cache, strlen(method), sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_SELF(return_value_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_SELFW(method, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_self, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_SELF(method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_self, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_STATICW(return_value_ptr, method, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_STATIC(return_value_ptr, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_STATICW(method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			RETURN_ON_FAILURE(zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC)); \
		} \
		else { \
			RETURN_ON_FAILURE(zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC)); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_STATIC(method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_static, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_CE_STATICW(return_value_ptr, class_entry, method, cache, ...) \
	do { \
		zval *params[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, cache, sizeof(params)/sizeof(zval*), params TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, strlen(method), cache, sizeof(params)/sizeof(zval*), params TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_CALL_CE_STATIC(return_value_ptr, class_entry, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(return_value_ptr); \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_call_class_method_aparams(return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_CE_STATICW(class_entry, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, strlen(method), sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

#define ZEPHIR_RETURN_CALL_CE_STATIC(class_entry, method, cache, ...) \
	do { \
		zval *params_[] = {__VA_ARGS__}; \
		if (__builtin_constant_p(method)) { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, sizeof(method)-1, cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
		else { \
			ZEPHIR_LAST_CALL_STATUS = zephir_return_call_class_method(return_value, return_value_ptr, class_entry, zephir_fcall_ce, NULL, method, strlen(method), cache, sizeof(params_)/sizeof(zval*), params_ TSRMLS_CC); \
		} \
	} while (0)

/** Use these functions to call functions in the PHP userland using an arbitrary zval as callable */
#define ZEPHIR_CALL_USER_FUNC(return_value, handler) ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, handler, NULL)
#define ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, handler, params) \
	do { \
		ZEPHIR_LAST_CALL_STATUS = zephir_call_user_func_array(return_value, handler, params TSRMLS_CC); \
	} while (0)

#define ZEPHIR_CALL_USER_FUNC_ARRAY_NOEX(return_value, handler, params) \
	do { \
		ZEPHIR_LAST_CALL_STATUS = zephir_call_user_func_array_noex(return_value, handler, params TSRMLS_CC); \
	} while (0)

int zephir_call_func_aparams(zval **return_value_ptr, const char *func_name, uint func_length,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

int zephir_call_zval_func_aparams(zval **return_value_ptr, zval *func_name,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_function(zval *return_value, zval **return_value_ptr,
	const char *func, uint func_len, zephir_fcall_cache_entry **cache_entry, uint param_count, zval **params TSRMLS_DC)
{
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;
	int status;

	if (return_value_ptr) {
		zval_ptr_dtor(return_value_ptr);
		*return_value_ptr = NULL;
	}

	status = zephir_call_func_aparams(rvp, func, func_len, cache_entry, param_count, params TSRMLS_CC);

	if (status == FAILURE) {
		if (return_value_ptr && EG(exception)) {
			ALLOC_INIT_ZVAL(*return_value_ptr);
		}

		return FAILURE;
	}

	if (!return_value_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, rv);
	}

	return SUCCESS;
}

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_zval_function(zval *return_value, zval **return_value_ptr,
	zval *func, zephir_fcall_cache_entry **cache_entry, uint param_count, zval **params TSRMLS_DC)
{
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;
	int status;

	if (return_value_ptr) {
		zval_ptr_dtor(return_value_ptr);
		*return_value_ptr = NULL;
	}

	status = zephir_call_zval_func_aparams(rvp, func, cache_entry, param_count, params TSRMLS_CC);

	if (status == FAILURE) {
		if (return_value_ptr && EG(exception)) {
			ALLOC_INIT_ZVAL(*return_value_ptr);
		}

		return FAILURE;
	}

	if (!return_value_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, rv);
	}

	return SUCCESS;
}

int zephir_call_class_method_aparams(zval **return_value_ptr, zend_class_entry *ce, zephir_call_type type, zval *object,
	const char *method_name, uint method_len,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_return_call_class_method(zval *return_value,
	zval **return_value_ptr, zend_class_entry *ce, zephir_call_type type, zval *object,
	const char *method_name, uint method_len,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC)
{
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;
	int status;

	if (return_value_ptr) {
		zval_ptr_dtor(return_value_ptr);
		*return_value_ptr = NULL;
	}

	status = zephir_call_class_method_aparams(rvp, ce, type, object, method_name, method_len, cache_entry, param_count, params TSRMLS_CC);

	if (status == FAILURE) {
		if (return_value_ptr && EG(exception)) {
			ALLOC_INIT_ZVAL(*return_value_ptr);
		}

		return FAILURE;
	}

	if (!return_value_ptr) {
		COPY_PZVAL_TO_ZVAL(*return_value, rv);
	}

	return SUCCESS;
}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL2(2, 3)
//static inline int zephir_call_method(zval **return_value_ptr, zval *object, const char *method,
//	zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_call_class_method_aparams(return_value_ptr, Z_OBJCE_P(object), zephir_fcall_method,
	//	object, method, strlen(method), cache_entry, nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL3(1, 3, 4)
//static inline int zephir_return_call_method(zval *return_value, zval **return_value_ptr, zval *object, const char *method,
//	zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_return_call_class_method(return_value, return_value_ptr, Z_OBJCE_P(object), zephir_fcall_method, object, method, strlen(method), nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL1(2)
//static inline int zephir_call_static(zval **return_value_ptr, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL2(1, 3)
//static inline int zephir_return_call_static(zval *return_value, zval **return_value_ptr, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_static, NULL, method, strlen(method), cache_entry, nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL1(2)
//static inline int zephir_call_self(zval **return_value_ptr, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_call_class_method_aparams(return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), cache_entry, nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL2(1, 3)
//static inline int zephir_return_call_self(zval *return_value, zval **return_value_ptr, const char *method, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_return_call_class_method(return_value, return_value_ptr, NULL, zephir_fcall_self, NULL, method, strlen(method), nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL1(3)
//static inline int zephir_call_parent(zval **return_value_ptr, zval *object, const char *method, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_call_class_method_aparams(return_value_ptr, (object ? Z_OBJCE_P(object) : NULL), zephir_fcall_parent, object, method, strlen(method), nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL2(1, 4)
//static inline int zephir_return_call_parent(zval *return_value, zval **return_value_ptr, zval *object, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_return_call_class_method(return_value, return_value_ptr, (object ? Z_OBJCE_P(object) : NULL), zephir_fcall_parent, object, method, strlen(method), cache_entry, nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL2(2, 3)
//static inline int zephir_call_ce(zval **return_value_ptr, zend_class_entry *ce, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_call_class_method_aparams(return_value_ptr, ce, zephir_fcall_ce, NULL, method, strlen(method), cache_entry, nparams, params TSRMLS_CC);
//}

//ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL3(1, 3, 4)
//static inline int zephir_return_call_ce(zval *return_value, zval **return_value_ptr, zend_class_entry *ce, const char *method, zephir_fcall_cache_entry **cache_entry, uint nparams, zval **params TSRMLS_DC)
//{
	//return zephir_return_call_class_method(return_value, return_value_ptr, ce, zephir_fcall_ce, NULL, method, strlen(method), nparams, params TSRMLS_CC);
//}

/** Fast call_user_func_array/call_user_func */
int zephir_call_user_func_array_noex(zval *return_value, zval *handler, zval *params TSRMLS_DC) ZEPHIR_ATTR_WARN_UNUSED_RESULT;

ZEPHIR_ATTR_WARN_UNUSED_RESULT static inline int zephir_call_user_func_array(zval *return_value, zval *handler, zval *params TSRMLS_DC)
{
	int status = zephir_call_user_func_array_noex(return_value, handler, params TSRMLS_CC);
	return (EG(exception)) ? FAILURE : status;
}

int zephir_has_constructor_ce(const zend_class_entry *ce) ZEPHIR_ATTR_PURE ZEPHIR_ATTR_NONNULL;

ZEPHIR_ATTR_WARN_UNUSED_RESULT ZEPHIR_ATTR_NONNULL static inline int zephir_has_constructor(const zval *object TSRMLS_DC)
{
	return Z_TYPE_P(object) == IS_OBJECT ? zephir_has_constructor_ce(Z_OBJCE_P(object)) : 0;
}

/** PHP < 5.3.9 has problems with closures */
#if PHP_VERSION_ID <= 50309
int zephir_call_function(zend_fcall_info *fci, zend_fcall_info_cache *fci_cache TSRMLS_DC);
#define ZEPHIR_ZEND_CALL_FUNCTION_WRAPPER zephir_call_function
#else
#define ZEPHIR_ZEND_CALL_FUNCTION_WRAPPER zend_call_function
#endif

#ifndef zend_error_noreturn
#define zend_error_noreturn zend_error
#endif

#define zephir_check_call_status() \
	do \
		if (ZEPHIR_LAST_CALL_STATUS == FAILURE) { \
			ZEPHIR_MM_RESTORE(); \
			return; \
	} \
	while(0)

#define zephir_check_call_status_or_jump(label) \
	if (ZEPHIR_LAST_CALL_STATUS == FAILURE) { \
		if (EG(exception)) { \
			goto label; \
		} else { \
			ZEPHIR_MM_RESTORE(); \
			return; \
		} \
	}

#define zephir_check_temp_parameter(param) do { if (Z_REFCOUNT_P(param) > 1) zval_copy_ctor(param); else ZVAL_NULL(param); } while(0)

void zephir_eval_php(zval *str, zval *retval_ptr, char *context TSRMLS_DC);

#endif /* ZEPHIR_KERNEL_FCALL_H */




#ifndef ZEPHIR_KERNEL_ARRAY_H
#define ZEPHIR_KERNEL_ARRAY_H

/** Combined isset/fetch */
int zephir_array_isset_fetch(zval **fetched, const zval *arr, zval *index, int readonly TSRMLS_DC);
int zephir_array_isset_quick_string_fetch(zval **fetched, zval *arr, char *index, uint index_length, unsigned long key, int readonly TSRMLS_DC);
int zephir_array_isset_string_fetch(zval **fetched, zval *arr, char *index, uint index_length, int readonly TSRMLS_DC);
int zephir_array_isset_long_fetch(zval **fetched, zval *arr, unsigned long index, int readonly TSRMLS_DC);

/** Check for index existence */
int ZEPHIR_FASTCALL zephir_array_isset(const zval *arr, zval *index);
int ZEPHIR_FASTCALL zephir_array_isset_long(const zval *arr, unsigned long index);
int ZEPHIR_FASTCALL zephir_array_isset_string(const zval *arr, const char *index, uint index_length);

/** Fast index existence checking */
int ZEPHIR_FASTCALL zephir_array_isset_quick_string(const zval *arr, const char *index, uint index_length, unsigned long key);

/** Unset existing indexes */
int ZEPHIR_FASTCALL zephir_array_unset(zval **arr, zval *index, int flags);
int ZEPHIR_FASTCALL zephir_array_unset_long(zval **arr, unsigned long index, int flags);
int ZEPHIR_FASTCALL zephir_array_unset_string(zval **arr, const char *index, uint index_length, int flags);

/** Append elements to arrays */
int zephir_array_append(zval **arr, zval *value, int separate ZEPHIR_DEBUG_PARAMS);
int zephir_array_append_long(zval **arr, long value, int separate);
int zephir_array_append_string(zval **arr, char *value, uint value_length, int separate);

/** Modify arrays */
int zephir_array_update_zval(zval **arr, zval *index, zval **value, int flags);
int zephir_array_update_string(zval **arr, const char *index, uint index_length, zval **value, int flags);
int zephir_array_update_long(zval **arr, unsigned long index, zval **value, int flags ZEPHIR_DEBUG_PARAMS);

/** Fetch items from arrays */
int zephir_array_fetch(zval **return_value, zval *arr, zval *index, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC);
int zephir_array_fetch_string(zval **return_value, zval *arr, const char *index, uint index_length, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC);
int zephir_array_fetch_long(zval **return_value, zval *arr, unsigned long index, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC);

/** Merge+Append */
void zephir_merge_append(zval *left, zval *values);

/* Traversing Arays */
void zephir_array_get_current(zval *return_value, zval *array);
void zephir_array_next(zval *array);

/* In Array */
int zephir_fast_in_array(zval *needle, zval *haystack TSRMLS_DC);

/** Fast Array Merge */
void zephir_fast_array_merge(zval *return_value, zval **array1, zval **array2 TSRMLS_DC);

/** Recursive merge */
void zephir_array_merge_recursive_n(zval **a1, zval *a2 TSRMLS_DC);

void zephir_array_unshift(zval *arr, zval *arg TSRMLS_DC);
void zephir_array_keys(zval *return_value, zval *arr TSRMLS_DC);
void zephir_array_values(zval *return_value, zval *arr);
int zephir_array_key_exists(zval *arr, zval *key TSRMLS_DC);
int zephir_array_is_associative(zval *arr);

int zephir_array_update_multi(zval **arr, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...);

#define zephir_array_fast_append(arr, value) \
  Z_ADDREF_P(value); \
  zend_hash_next_index_insert(Z_ARRVAL_P(arr), &value, sizeof(zval *), NULL);

#endif /* ZEPHIR_KERNEL_ARRAY_H */




#ifndef ZEPHIR_KERNEL_HASH_H
#define ZEPHIR_KERNEL_HASH_H

int zephir_hash_exists(const HashTable *ht, const char *arKey, uint nKeyLength);
int zephir_hash_quick_exists(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h);
int zephir_hash_find(const HashTable *ht, const char *arKey, uint nKeyLength, void **pData);
int zephir_hash_quick_find(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h, void **pData);
void zephir_get_current_key(zval **key, const HashTable *hash_table, HashPosition *hash_position TSRMLS_DC);
zval zephir_get_current_key_w(const HashTable *hash_table, HashPosition *hash_position);
int zephir_has_numeric_keys(const zval *data);
void zephir_hash_update_or_insert(HashTable *ht, zval *offset, zval *value);
zval** zephir_hash_get(HashTable *ht, zval *key, int type);
int zephir_hash_unset(HashTable *ht, zval *offset);

#define zephir_hash_move_forward_ex(ht, pos) *pos = (*pos ? (*pos)->pListNext : NULL)

static zend_always_inline int zephir_hash_get_current_data_ex(HashTable *ht, void **pData, HashPosition *pos)
{
	Bucket *p;
	p = pos ? (*pos) : ht->pInternalPointer;  
	if (p) {
		*pData = p->pData;
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

static zend_always_inline int zephir_hash_move_backwards_ex(HashTable *ht, HashPosition *pos)
{
	HashPosition *current = pos ? pos : &ht->pInternalPointer;
	if (*current) {
		*current = (*current)->pListLast;
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

#endif



#ifndef ZEPHIR_KERNEL_OBJECT_H
#define ZEPHIR_KERNEL_OBJECT_H

/** Class Retrieving/Checking */
int zephir_class_exists(const zval *class_name, int autoload TSRMLS_DC);
int zephir_interface_exists(const zval *interface_name, int autoload TSRMLS_DC);
void zephir_get_class(zval *result, zval *object, int lower TSRMLS_DC);
void zephir_get_class_ns(zval *result, zval *object, int lower TSRMLS_DC);
void zephir_get_ns_class(zval *result, zval *object, int lower TSRMLS_DC);
void zephir_get_called_class(zval *return_value TSRMLS_DC);
zend_class_entry *zephir_fetch_class(const zval *class_name TSRMLS_DC);
zend_class_entry* zephir_fetch_self_class(TSRMLS_D);
zend_class_entry* zephir_fetch_parent_class(TSRMLS_D);
zend_class_entry* zephir_fetch_static_class(TSRMLS_D);

#define ZEPHIR_GET_CLASS_CONSTANT(return_value, ce, const_name) \
	do { \
		if (FAILURE == zephir_get_class_constant(return_value, ce, const_name, strlen(const_name)+1 TSRMLS_CC)) { \
			ZEPHIR_MM_RESTORE(); \
			return; \
		} \
	} while (0)

/** Class constants */
int zephir_get_class_constant(zval *return_value, zend_class_entry *ce, char *constant_name, unsigned int constant_length TSRMLS_DC);

/** Cloning/Instance of*/
int zephir_clone(zval *destiny, zval *obj TSRMLS_DC);
int zephir_instance_of(zval *result, const zval *object, const zend_class_entry *ce TSRMLS_DC);
int zephir_is_instance_of(zval *object, const char *class_name, unsigned int class_length TSRMLS_DC);
int zephir_instance_of_ev(const zval *object, const zend_class_entry *ce TSRMLS_DC);

/** Method exists */
int zephir_method_exists(const zval *object, const zval *method_name TSRMLS_DC);
int zephir_method_exists_ex(const zval *object, const char *method_name, unsigned int method_len TSRMLS_DC);
int zephir_method_quick_exists_ex(const zval *object, const char *method_name, unsigned int method_len, unsigned long hash TSRMLS_DC);

/** Isset properties */
int zephir_isset_property(zval *object, const char *property_name, unsigned int property_length TSRMLS_DC);
int zephir_isset_property_quick(zval *object, const char *property_name, unsigned int property_length, unsigned long hash TSRMLS_DC);
int zephir_isset_property_zval(zval *object, const zval *property TSRMLS_DC);

/** Reading properties */
zval* zephir_fetch_property_this_quick(zval *object, const char *property_name, zend_uint property_length, ulong key, int silent TSRMLS_DC);
int zephir_read_property(zval **result, zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC);
int zephir_read_property_zval(zval **result, zval *object, zval *property, int silent TSRMLS_DC);
int zephir_return_property(zval *return_value, zval **return_value_ptr, zval *object, char *property_name, unsigned int property_length TSRMLS_DC);
int zephir_return_property_quick(zval *return_value, zval **return_value_ptr, zval *object, char *property_name, unsigned int property_length, unsigned long key TSRMLS_DC);
int zephir_fetch_property(zval **result, zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC);
int zephir_fetch_property_zval(zval **result, zval *object, zval *property, int silent TSRMLS_DC);

/** Updating properties */
int zephir_update_property_this(zval *object, char *property_name, unsigned int property_length, zval *value TSRMLS_DC);
int zephir_update_property_long(zval *obj, char *property_name, unsigned int property_length, long value TSRMLS_DC);
int zephir_update_property_string(zval *object, char *property_name, unsigned int property_length, char *str, unsigned int str_length TSRMLS_DC);
int zephir_update_property_bool(zval *obj, char *property_name, unsigned int property_length, int value TSRMLS_DC);
int zephir_update_property_null(zval *obj, char *property_name, unsigned int property_length TSRMLS_DC);
int zephir_update_property_zval(zval *obj, const char *property_name, unsigned int property_length, zval *value TSRMLS_DC);
int zephir_update_property_zval_zval(zval *obj, zval *property, zval *value TSRMLS_DC);
int zephir_update_property_empty_array(zend_class_entry *ce, zval *object, char *property, unsigned int property_length TSRMLS_DC);

/** Updating array properties */
int zephir_update_property_array(zval *object, const char *property, zend_uint property_length, const zval *index, zval *value TSRMLS_DC);
int zephir_update_property_array_string(zval *object, char *property, unsigned int property_length, char *index, unsigned int index_length, zval *value TSRMLS_DC);
int zephir_update_property_array_append(zval *object, char *property, unsigned int property_length, zval *value TSRMLS_DC);
int zephir_update_property_array_multi(zval *object, const char *property, zend_uint property_length, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...);

/** Increment/Decrement properties */
int zephir_property_incr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC);
int zephir_property_decr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC);

/** Unset properties */
int zephir_unset_property(zval* object, const char* name TSRMLS_DC);
int zephir_unset_property_array(zval *object, char *property, unsigned int property_length, zval *index TSRMLS_DC);

/** Static properties */
int zephir_read_static_property(zval **result, const char *class_name, unsigned int class_length, char *property_name, unsigned int property_length TSRMLS_DC);
int zephir_update_static_property_ce(zend_class_entry *ce, const char *name, int len, zval *value TSRMLS_DC);
int zephir_update_static_property_ce_cache(zend_class_entry *ce, const char *name, int len, zval *value, zend_property_info **property_info TSRMLS_DC);
int zephir_update_static_property(const char *class_name, unsigned int class_length, char *name, unsigned int name_length, zval *value TSRMLS_DC);
int zephir_read_static_property_ce(zval **result, zend_class_entry *ce, const char *property, int len TSRMLS_DC);
int zephir_read_class_property(zval **result, int type, const char *property, int len TSRMLS_DC);
zval* zephir_fetch_static_property_ce(zend_class_entry *ce, const char *property, int len TSRMLS_DC);
int zephir_update_static_property_array_multi_ce(zend_class_entry *ce, const char *property, zend_uint property_length, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...);

/** Create instances */
int zephir_create_instance(zval *return_value, const zval *class_name TSRMLS_DC);
int zephir_create_instance_params(zval *return_value, const zval *class_name, zval *params TSRMLS_DC);

ZEPHIR_ATTR_NONNULL static inline int zephir_read_property_this_quick(zval **result, zval *object, const char *property_name, zend_uint property_length, ulong key, int silent TSRMLS_DC)
{
  zval *tmp = zephir_fetch_property_this_quick(object, property_name, property_length, key, silent TSRMLS_CC);
  if (EXPECTED(tmp != NULL)) {
    *result = tmp;
    Z_ADDREF_PP(result);
    return SUCCESS;
  }

  ALLOC_INIT_ZVAL(*result);
  return FAILURE;
}

ZEPHIR_ATTR_NONNULL static inline int zephir_read_property_this(zval **result, zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC)
{
#ifdef __GNUC__
  if (__builtin_constant_p(property_name) && __builtin_constant_p(property_length)) {
    return zephir_read_property_this_quick(result, object, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
  }
#endif

  return zephir_read_property_this_quick(result, object, property_name, property_length, zend_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
}

ZEPHIR_ATTR_NONNULL static inline zval* zephir_fetch_nproperty_this_quick(zval *object, const char *property_name, zend_uint property_length, ulong key, int silent TSRMLS_DC)
{
#ifdef __GNUC__
  if (__builtin_constant_p(property_name) && __builtin_constant_p(property_length)) {
    zval *result = zephir_fetch_property_this_quick(object, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
    return result ? result : EG(uninitialized_zval_ptr);
  }
#endif

  zval *result = zephir_fetch_property_this_quick(object, property_name, property_length, zend_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
  return result ? result : EG(uninitialized_zval_ptr);
}

ZEPHIR_ATTR_NONNULL static inline zval* zephir_fetch_nproperty_this(zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC)
{
#ifdef __GNUC__
  if (__builtin_constant_p(property_name) && __builtin_constant_p(property_length)) {
    return zephir_fetch_nproperty_this_quick(object, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
  }
#endif

  return zephir_fetch_nproperty_this_quick(object, property_name, property_length, zend_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
}

ZEPHIR_ATTR_NONNULL static inline zval* zephir_fetch_property_this(zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC)
{
#ifdef __GNUC__
  if (__builtin_constant_p(property_name) && __builtin_constant_p(property_length)) {
    return zephir_fetch_property_this_quick(object, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
  }
#endif

  return zephir_fetch_property_this_quick(object, property_name, property_length, zend_hash_func(property_name, property_length + 1), silent TSRMLS_CC);
}

#endif




#ifndef ZEPHIR_KERNEL_STRING_H
#define ZEPHIR_KERNEL_STRING_H

#include <Zend/zend.h>

#define ZEPHIR_TRIM_LEFT  1
#define ZEPHIR_TRIM_RIGHT 2
#define ZEPHIR_TRIM_BOTH  3

/** Fast char position */
int zephir_memnstr(const zval *haystack, const zval *needle ZEPHIR_DEBUG_PARAMS);
int zephir_memnstr_str(const zval *haystack, char *needle, unsigned int needle_length ZEPHIR_DEBUG_PARAMS);

/** Function replacement */
void zephir_fast_strlen(zval *return_value, zval *str);
int zephir_fast_strlen_ev(zval *str);
void zephir_fast_strtolower(zval *return_value, zval *str);
void zephir_strtolower_inplace(zval *s);
void zephir_fast_join(zval *result, zval *glue, zval *pieces TSRMLS_DC);
void zephir_fast_join_str(zval *result, char *glue, unsigned int glue_length, zval *pieces TSRMLS_DC);
void zephir_fast_explode(zval *result, zval *delimiter, zval *str, long limit TSRMLS_DC);
void zephir_fast_explode_str(zval *result, const char *delimiter, int delimiter_length, zval *str, long limit TSRMLS_DC);
void zephir_fast_strpos(zval *return_value, const zval *haystack, const zval *needle, unsigned int offset);
void zephir_fast_strpos_str(zval *return_value, const zval *haystack, char *needle, unsigned int needle_length);
void zephir_fast_stripos_str(zval *return_value, zval *haystack, char *needle, unsigned int needle_length);
void zephir_fast_str_replace(zval *return_value, zval *search, zval *replace, zval *subject);
void zephir_fast_trim(zval *return_value, zval *str, zval *charlist, int where TSRMLS_DC);
void zephir_fast_strip_tags(zval *return_value, zval *str);
void zephir_fast_strtoupper(zval *return_value, zval *str);

/** Camelize/Uncamelize */
void zephir_camelize(zval *return_value, const zval *str);
void zephir_uncamelize(zval *return_value, const zval *str);

/** Starts/Ends with */
int zephir_start_with(const zval *str, const zval *compared, zval *case_sensitive);
int zephir_start_with_str(const zval *str, char *compared, unsigned int compared_length);
int zephir_start_with_str_str(char *str, unsigned int str_length, char *compared, unsigned int compared_length);
int zephir_end_with(const zval *str, const zval *compared, zval *case_sensitive);
int zephir_end_with_str(const zval *str, char *compared, unsigned int compared_length);

/** Random string */
void zephir_random_string(zval *return_value, const zval *type, const zval *length TSRMLS_DC);

/* Strips extra slashes */
void zephir_remove_extra_slashes(zval *return_value, const zval *str);

/** Generates a unique key for an array/object */
void zephir_unique_key(zval *return_value, zval *prefix, zval *value TSRMLS_DC);

/** spprintf */
int zephir_spprintf(char **message, int max_len, char *format, ...);

/* Substr */
void zephir_substr(zval *return_value, zval *str, long from, long length);

/** EOL */
zval *zephir_eol(int eol TSRMLS_DC);

/** Preg-Match */
void zephir_preg_match(zval *return_value, zval **return_value_ptr, zval *regex, zval *subject, zval *matches, int global, long flags, long offset TSRMLS_DC);

/** Base64 */
void zephir_base64_encode(zval *return_value, zval *data);
void zephir_base64_decode(zval *return_value, zval *data);

/** Hash */
void zephir_md5(zval *return_value, zval *str);

/** JSON */
int zephir_json_encode(zval *return_value, zval **return_value_ptr, zval *v, int opts TSRMLS_DC);
int zephir_json_decode(zval *return_value, zval **return_value_ptr, zval *v, zend_bool assoc TSRMLS_DC);

/***/
void zephir_lcfirst(zval *return_value, zval *s);
void zephir_ucfirst(zval *return_value, zval *s);
int zephir_http_build_query(zval *return_value, zval *params, char *sep TSRMLS_DC);
void zephir_htmlspecialchars(zval *return_value, zval *string, zval *quoting, zval *charset TSRMLS_DC);
void zephir_htmlentities(zval *return_value, zval *string, zval *quoting, zval *charset TSRMLS_DC);
void zephir_strval(zval *return_value, zval *v);
void zephir_date(zval *return_value, zval *format, zval *timestamp TSRMLS_DC);
void zephir_addslashes(zval *return_value, zval *str TSRMLS_DC);
void zephir_stripslashes(zval *return_value, zval *str TSRMLS_DC);
void zephir_stripcslashes(zval *return_value, zval *str TSRMLS_DC);

#if PHP_VERSION_ID < 50400

const char* zend_new_interned_string(const char *arKey, int nKeyLength, int free_src TSRMLS_DC);
#define ZEPHIR_ZVAL_MAYBE_INTERNED_STRING(pz, string)  ZVAL_STRING(pz, string, 1);

#else

#define ZEPHIR_ZVAL_MAYBE_INTERNED_STRING(pz, string) \
	do { \
		if (IS_INTERNED(string)) { \
			ZVAL_STRINGL(pz, string, INTERNED_LEN(string)-1, 0); \
		} \
		else { \
			ZVAL_STRING(pz, string, 1); \
		} \
	} while (0)

#endif /* PHP_VERSION_ID < 50400 */

#endif /* ZEPHIR_KERNEL_STRING_H */




/** Low level filters */
void zephir_filter_alphanum(zval *return_value, zval *param);
void zephir_filter_identifier(zval *return_value, zval *param);

/** Encoding */
void zephir_is_basic_charset(zval *return_value, const zval *param);

/** Escaping */
void zephir_escape_css(zval *return_value, zval *param);
void zephir_escape_js(zval *return_value, zval *param);
void zephir_escape_htmlattr(zval *return_value, zval *param);
void zephir_escape_html(zval *return_value, zval *str, zval *quote_style, zval *charset TSRMLS_DC);



#ifndef ZEPHIR_KERNEL_FILE_H
#define ZEPHIR_KERNEL_FILE_H

int zephir_file_exists(zval *filename TSRMLS_DC);
int zephir_compare_mtime(zval *filename1, zval *filename2 TSRMLS_DC);
void zephir_fix_path(zval **return_value, zval *path, zval *directory_separator TSRMLS_DC);
void zephir_prepare_virtual_path(zval *return_value, zval *path, zval *virtual_separator TSRMLS_DC);
void zephir_unique_path_key(zval *return_value, zval *path TSRMLS_DC);
void zephir_realpath(zval *return_value, zval *filename TSRMLS_DC);
void zephir_file_get_contents(zval *return_value, zval *filename TSRMLS_DC);
void zephir_file_put_contents(zval *return_value, zval *filename, zval *data TSRMLS_DC);
void zephir_possible_autoload_filepath(zval *return_value, zval *prefix, zval *class_name, zval *virtual_separator, zval *separator TSRMLS_DC);

void zephir_is_dir(zval *return_value, zval *path TSRMLS_DC);
void zephir_unlink(zval *return_value, zval *path TSRMLS_DC);
void zephir_filemtime(zval *return_value, zval *path TSRMLS_DC);
void zephir_basename(zval *return_value, zval *path TSRMLS_DC);

void zephir_fwrite(zval *return_value, zval *stream_zval, zval *data TSRMLS_DC);
int zephir_feof(zval *stream_zval TSRMLS_DC);
int zephir_fclose(zval *stream_zval TSRMLS_DC);

#ifdef TSRM_WIN32
#define ZEPHIR_DIRECTORY_SEPARATOR "\\"
#else
#define ZEPHIR_DIRECTORY_SEPARATOR "/"
#endif

#endif /* ZEPHIR_KERNEL_FILE_H */




#ifndef ZEPHIR_KERNEL_OUTPUT_H
#define ZEPHIR_KERNEL_OPERATORS_H

void zephir_ob_start(TSRMLS_D);
void zephir_ob_get_contents(zval *result TSRMLS_DC);
int zephir_ob_end_flush(TSRMLS_D);
int zephir_ob_end_clean(TSRMLS_D);
int zephir_ob_flush(TSRMLS_D);
int zephir_ob_clean(TSRMLS_D);
int zephir_ob_get_level(TSRMLS_D);

#endif



#ifndef ZEPHIR_KERNEL_OPERATORS_H
#define ZEPHIR_KERNEL_OPERATORS_H

/** Strict comparing */
#define ZEPHIR_IS_LONG(op1, op2)   ((Z_TYPE_P(op1) == IS_LONG && Z_LVAL_P(op1) == op2) || zephir_compare_strict_long(op1, op2 TSRMLS_CC))
#define ZEPHIR_IS_DOUBLE(op1, op2) ((Z_TYPE_P(op1) == IS_DOUBLE && Z_DVAL_P(op1) == op2) || zephir_compare_strict_double(op1, op2 TSRMLS_CC))
#define ZEPHIR_IS_STRING(op1, op2) zephir_compare_strict_string(op1, op2, strlen(op2))

#define ZEPHIR_IS_LONG_IDENTICAL(op1, op2)   (Z_TYPE_P(op1) == IS_LONG && Z_LVAL_P(op1) == op2)
#define ZEPHIR_IS_DOUBLE_IDENTICAL(op1, op2) (Z_TYPE_P(op1) == IS_DOUBLE && Z_DVAL_P(op1) == op2)
#define ZEPHIR_IS_STRING_IDENTICAL(op1, op2) (Z_TYPE_P(op1) == IS_STRING && zephir_compare_strict_string(op1, op2, strlen(op2)))

/** strict boolean comparison */
#define ZEPHIR_IS_FALSE(var)       ((Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var)) || zephir_compare_strict_bool(var, 0 TSRMLS_CC))
#define ZEPHIR_IS_TRUE(var)        ((Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var)) || zephir_compare_strict_bool(var, 1 TSRMLS_CC))

#define ZEPHIR_IS_FALSE_IDENTICAL(var)       (Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var))
#define ZEPHIR_IS_TRUE_IDENTICAL(var)        (Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var))

#define ZEPHIR_IS_NOT_FALSE(var)   (Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && Z_BVAL_P(var)))
#define ZEPHIR_IS_NOT_TRUE(var)    (Z_TYPE_P(var) != IS_BOOL || (Z_TYPE_P(var) == IS_BOOL && !Z_BVAL_P(var)))
#define ZEPHIR_IS_BOOL(op1, op2)   ((Z_TYPE_P(op1) == IS_BOOL && Z_BVAL_P(op1) == op2) || zephir_compare_strict_bool(op1, op2 TSRMLS_CC))

/** SQL null empty **/
#define ZEPHIR_IS_EMPTY(var)       (Z_TYPE_P(var) == IS_NULL || ZEPHIR_IS_FALSE(var) || (Z_TYPE_P(var) == IS_STRING && !Z_STRLEN_P(var)) || (Z_TYPE_P(var) == IS_ARRAY && zend_hash_num_elements(Z_ARRVAL_P(var)) == 0))
#define ZEPHIR_IS_NOT_EMPTY(var)   (!ZEPHIR_IS_EMPTY(var))

/** Is scalar */
#define ZEPHIR_IS_SCALAR(var)      (!ZEPHIR_IS_NOT_SCALAR(var))
#define ZEPHIR_IS_NOT_SCALAR(var)  (Z_TYPE_P(var) == IS_NULL || Z_TYPE_P(var) == IS_ARRAY || Z_TYPE_P(var) == IS_OBJECT || Z_TYPE_P(var) == IS_RESOURCE)

/** Equals/Identical */
#define ZEPHIR_IS_EQUAL(op1, op2)      zephir_is_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_IS_IDENTICAL(op1, op2)  zephir_is_identical(op1, op2 TSRMLS_CC)

/** Greater/Smaller equals */
#define ZEPHIR_LE(op1, op2)       zephir_less_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_LE_LONG(op1, op2)  ((Z_TYPE_P(op1) == IS_LONG && Z_LVAL_P(op1) <= op2) || zephir_less_equal_long(op1, op2 TSRMLS_CC))
#define ZEPHIR_LE_DOUBLE(op1, op2)  ((Z_TYPE_P(op1) == IS_DOUBLE && Z_DVAL_P(op1) <= op2) || zephir_less_equal_double(op1, op2 TSRMLS_CC))
#define ZEPHIR_GE(op1, op2)       zephir_greater_equal(op1, op2 TSRMLS_CC)
#define ZEPHIR_GE_LONG(op1, op2)  zephir_greater_equal_long(op1, op2 TSRMLS_CC)
#define ZEPHIR_LT(op1, op2)       ((Z_TYPE_P(op1) == IS_LONG && Z_TYPE_P(op2) == IS_LONG) ? Z_LVAL_P(op1) < Z_LVAL_P(op2) : zephir_less(op1, op2 TSRMLS_CC))
#define ZEPHIR_LT_LONG(op1, op2)  ((Z_TYPE_P(op1) == IS_LONG && Z_LVAL_P(op1) < op2) || zephir_less_long(op1, op2 TSRMLS_CC))
#define ZEPHIR_LT_DOUBLE(op1, op2)  ((Z_TYPE_P(op1) == IS_DOUBLE && Z_DVAL_P(op1) < op2) || zephir_less_double(op1, op2 TSRMLS_CC))
#define ZEPHIR_GT(op1, op2)       zephir_greater(op1, op2 TSRMLS_CC)
#define ZEPHIR_GT_LONG(op1, op2)  ((Z_TYPE_P(op1) == IS_LONG && Z_LVAL_P(op1) > op2) || zephir_greater_long(op1, op2 TSRMLS_CC))
#define ZEPHIR_GT_DOUBLE(op1, op2)  ((Z_TYPE_P(op1) == IS_DOUBLE && Z_DVAL_P(op1) > op2) || zephir_greater_double(op1, op2 TSRMLS_CC))

#define ZEPHIR_STRING_OFFSET(op1, index) ((index >= 0 && index < Z_STRLEN_P(op1)) ? Z_STRVAL_P(op1)[index] : '\0')

#if PHP_VERSION_ID < 50400
#define zephir_increment(var) increment_function(var)
#else
#define zephir_increment(var) fast_increment_function(var)
#endif

#if PHP_VERSION_ID < 50400
#define zephir_decrement(var) decrement_function(var)
#else
#define zephir_decrement(var) fast_decrement_function(var)
#endif

void zephir_make_printable_zval(zval *expr, zval *expr_copy, int *use_copy);

#if PHP_VERSION_ID < 50400
#define zephir_sub_function(result, left, right) sub_function(result, left, right)
#else
#define zephir_sub_function(result, left, right) fast_sub_function(result, left, right)
#endif

/** Operator functions */
int zephir_add_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
int zephir_and_function(zval *result, zval *left, zval *right);
void zephir_negate(zval *z TSRMLS_DC);

/** Bitwise functions */
int zephir_bitwise_and_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
int zephir_bitwise_or_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
int zephir_bitwise_xor_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
int zephir_shift_left_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);
int zephir_shift_right_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);

void zephir_concat_self(zval **left, zval *right TSRMLS_DC);
void zephir_concat_self_str(zval **left, const char *right, int right_length TSRMLS_DC);
void zephir_concat_self_long(zval **left, const long right TSRMLS_DC);
void zephir_concat_self_char(zval **left, unsigned char right TSRMLS_DC);

/** Strict comparing */
int zephir_compare_strict_string(zval *op1, const char *op2, int op2_length);
int zephir_compare_strict_long(zval *op1, long op2 TSRMLS_DC);
int zephir_compare_strict_double(zval *op1, double op2 TSRMLS_DC);
int zephir_compare_strict_bool(zval *op1, zend_bool op2 TSRMLS_DC);

void zephir_cast(zval *result, zval *var, zend_uint type);
void zephir_convert_to_object(zval *op);
long zephir_get_intval_ex(const zval *op);
double zephir_get_doubleval_ex(const zval *op);
zend_bool zephir_get_boolval_ex(const zval *op);

int zephir_is_numeric_ex(const zval *op);

int zephir_is_equal(zval *op1, zval *op2 TSRMLS_DC);
int zephir_is_identical(zval *op1, zval *op2 TSRMLS_DC);

int zephir_less(zval *op1, zval *op2 TSRMLS_DC);
int zephir_less_long(zval *op1, long op2 TSRMLS_DC);

int zephir_greater(zval *op1, zval *op2 TSRMLS_DC);
int zephir_greater_long(zval *op1, long op2 TSRMLS_DC);

int zephir_less_equal(zval *op1, zval *op2 TSRMLS_DC);
int zephir_less_equal_long(zval *op1, long op2 TSRMLS_DC);

int zephir_greater_equal(zval *op1, zval *op2 TSRMLS_DC);
int zephir_greater_equal_long(zval *op1, long op2 TSRMLS_DC);

#define zephir_get_numberval(z) (Z_TYPE_P(z) == IS_LONG ? Z_LVAL_P(z) : zephir_get_doubleval(z))
#define zephir_get_intval(z) (Z_TYPE_P(z) == IS_LONG ? Z_LVAL_P(z) : zephir_get_intval_ex(z))
#define zephir_get_doubleval(z) (Z_TYPE_P(z) == IS_DOUBLE ? Z_DVAL_P(z) : zephir_get_doubleval_ex(z))
#define zephir_get_boolval(z) (Z_TYPE_P(z) == IS_BOOL ? Z_BVAL_P(z) : zephir_get_boolval_ex(z))

#define ZEPHIR_ADD_ASSIGN(z, v)  \
	{  \
		zval tmp;  \
		ZEPHIR_SEPARATE(z);  \
		if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_LONG) {  \
			Z_LVAL_P(z) += Z_LVAL_P(v);  \
		} else {  \
			if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_DOUBLE) {  \
				Z_LVAL_P(z) += Z_DVAL_P(v);  \
			} else {  \
				zephir_add_function(&tmp, z, v TSRMLS_CC);  \
				if (Z_TYPE(tmp) == IS_LONG) {  \
					Z_LVAL_P(z) = Z_LVAL(tmp);  \
				} else {  \
					if (Z_TYPE(tmp) == IS_DOUBLE) {  \
						Z_DVAL_P(z) = Z_DVAL(tmp);  \
					}  \
				}  \
			}  \
		}  \
	}

#define ZEPHIR_SUB_ASSIGN(z, v)  \
	{  \
		zval tmp;  \
		ZEPHIR_SEPARATE(z);  \
		if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_LONG) {  \
			Z_LVAL_P(z) -= Z_LVAL_P(v);  \
		} else {  \
			if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_DOUBLE) {  \
				Z_LVAL_P(z) -= Z_DVAL_P(v);  \
			} else {  \
				sub_function(&tmp, z, v TSRMLS_CC);  \
				if (Z_TYPE(tmp) == IS_LONG) {  \
					Z_LVAL_P(z) = Z_LVAL(tmp);  \
				} else {  \
					if (Z_TYPE(tmp) == IS_DOUBLE) {  \
						Z_DVAL_P(z) = Z_DVAL(tmp);  \
					}  \
				}  \
			}  \
		}  \
	}

#define ZEPHIR_MUL_ASSIGN(z, v)  \
	{  \
		zval tmp;  \
		ZEPHIR_SEPARATE(z);  \
		if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_LONG) {  \
			Z_LVAL_P(z) *= Z_LVAL_P(v);  \
		} else {  \
			if (Z_TYPE_P(z) == IS_LONG && Z_TYPE_P(v) == IS_DOUBLE) {  \
				Z_LVAL_P(z) *= Z_DVAL_P(v);  \
			} else {  \
				sub_function(&tmp, z, v TSRMLS_CC);  \
				if (Z_TYPE(tmp) == IS_LONG) {  \
					Z_LVAL_P(z) = Z_LVAL(tmp);  \
				} else {  \
					if (Z_TYPE(tmp) == IS_DOUBLE) {  \
						Z_DVAL_P(z) = Z_DVAL(tmp);  \
					}  \
				}  \
			}  \
		}  \
	}

#define zephir_get_strval(left, right) \
	{ \
		int use_copy_right; \
		zval right_tmp; \
		if (Z_TYPE_P(right) == IS_STRING) { \
			ZEPHIR_CPY_WRT(left, right); \
		} else { \
			INIT_ZVAL(right_tmp); \
			zephir_make_printable_zval(right, &right_tmp, &use_copy_right); \
			if (use_copy_right) { \
				ZEPHIR_INIT_NVAR(left); \
				ZVAL_STRINGL(left, Z_STRVAL_P(&right_tmp), Z_STRLEN_P(&right_tmp), 0); \
			} \
		} \
	}

#define zephir_get_arrval(returnValue, passValue) \
	{ \
		if (Z_TYPE_P(passValue) == IS_ARRAY) { \
			ZEPHIR_CPY_WRT(returnValue, passValue); \
		} else { \
			ZEPHIR_INIT_NVAR(returnValue); \
			array_init_size(returnValue, 0); \
		} \
	}

#define zephir_is_numeric(value) (Z_TYPE_P(value) == IS_LONG || Z_TYPE_P(value) == IS_DOUBLE || zephir_is_numeric_ex(value))

#define zephir_is_true(value) \
	(Z_TYPE_P(value) == IS_NULL ? 0 : \
		(Z_TYPE_P(value) == IS_BOOL ? Z_BVAL_P(value) : \
			(Z_TYPE_P(value) == IS_LONG ? Z_LVAL_P(value) : \
				zend_is_true(value) \
			) \
		) \
	)

#endif


#define ZEPHIR_CONCAT_SS(result, op1, op2) \
	 zephir_concat_ss(&result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SS(result, op1, op2) \
	 zephir_concat_ss(&result, op1, sizeof(op1)-1, op2, sizeof(op2)-1, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_SV(result, op1, op2) \
	 zephir_concat_sv(&result, op1, sizeof(op1)-1, op2, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SV(result, op1, op2) \
	 zephir_concat_sv(&result, op1, sizeof(op1)-1, op2, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_SVS(result, op1, op2, op3) \
	 zephir_concat_svs(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SVS(result, op1, op2, op3) \
	 zephir_concat_svs(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_SVSV(result, op1, op2, op3, op4) \
	 zephir_concat_svsv(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SVSV(result, op1, op2, op3, op4) \
	 zephir_concat_svsv(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 zephir_concat_svsvs(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SVSVS(result, op1, op2, op3, op4, op5) \
	 zephir_concat_svsvs(&result, op1, sizeof(op1)-1, op2, op3, sizeof(op3)-1, op4, op5, sizeof(op5)-1, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_SVV(result, op1, op2, op3) \
	 zephir_concat_svv(&result, op1, sizeof(op1)-1, op2, op3, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_SVV(result, op1, op2, op3) \
	 zephir_concat_svv(&result, op1, sizeof(op1)-1, op2, op3, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VS(result, op1, op2) \
	 zephir_concat_vs(&result, op1, op2, sizeof(op2)-1, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VS(result, op1, op2) \
	 zephir_concat_vs(&result, op1, op2, sizeof(op2)-1, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VSV(result, op1, op2, op3) \
	 zephir_concat_vsv(&result, op1, op2, sizeof(op2)-1, op3, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VSV(result, op1, op2, op3) \
	 zephir_concat_vsv(&result, op1, op2, sizeof(op2)-1, op3, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VSVS(result, op1, op2, op3, op4) \
	 zephir_concat_vsvs(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VSVS(result, op1, op2, op3, op4) \
	 zephir_concat_vsvs(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 zephir_concat_vsvsv(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VSVSV(result, op1, op2, op3, op4, op5) \
	 zephir_concat_vsvsv(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 zephir_concat_vsvsvsv(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, op7, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VSVSVSV(result, op1, op2, op3, op4, op5, op6, op7) \
	 zephir_concat_vsvsvsv(&result, op1, op2, sizeof(op2)-1, op3, op4, sizeof(op4)-1, op5, op6, sizeof(op6)-1, op7, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VSVV(result, op1, op2, op3, op4) \
	 zephir_concat_vsvv(&result, op1, op2, sizeof(op2)-1, op3, op4, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VSVV(result, op1, op2, op3, op4) \
	 zephir_concat_vsvv(&result, op1, op2, sizeof(op2)-1, op3, op4, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VV(result, op1, op2) \
	 zephir_concat_vv(&result, op1, op2, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VV(result, op1, op2) \
	 zephir_concat_vv(&result, op1, op2, 1 TSRMLS_CC);

#define ZEPHIR_CONCAT_VVV(result, op1, op2, op3) \
	 zephir_concat_vvv(&result, op1, op2, op3, 0 TSRMLS_CC);
#define ZEPHIR_SCONCAT_VVV(result, op1, op2, op3) \
	 zephir_concat_vvv(&result, op1, op2, op3, 1 TSRMLS_CC);


void zephir_concat_ss(zval **result, const char *op1, zend_uint op1_len, const char *op2, zend_uint op2_len, int self_var TSRMLS_DC);
void zephir_concat_sv(zval **result, const char *op1, zend_uint op1_len, zval *op2, int self_var TSRMLS_DC);
void zephir_concat_svs(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, int self_var TSRMLS_DC);
void zephir_concat_svsv(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, zval *op4, int self_var TSRMLS_DC);
void zephir_concat_svsvs(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, zval *op4, const char *op5, zend_uint op5_len, int self_var TSRMLS_DC);
void zephir_concat_svv(zval **result, const char *op1, zend_uint op1_len, zval *op2, zval *op3, int self_var TSRMLS_DC);
void zephir_concat_vs(zval **result, zval *op1, const char *op2, zend_uint op2_len, int self_var TSRMLS_DC);
void zephir_concat_vsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, int self_var TSRMLS_DC);
void zephir_concat_vsvs(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, int self_var TSRMLS_DC);
void zephir_concat_vsvsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, zval *op5, int self_var TSRMLS_DC);
void zephir_concat_vsvsvsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, zval *op5, const char *op6, zend_uint op6_len, zval *op7, int self_var TSRMLS_DC);
void zephir_concat_vsvv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, zval *op4, int self_var TSRMLS_DC);
void zephir_concat_vv(zval **result, zval *op1, zval *op2, int self_var TSRMLS_DC);
void zephir_concat_vvv(zval **result, zval *op1, zval *op2, zval *op3, int self_var TSRMLS_DC);
void zephir_concat_function(zval *result, zval *op1, zval *op2 TSRMLS_DC);



#ifndef ZEPHIR_KERNEL_EXCEPTIONS_H
#define ZEPHIR_KERNEL_EXCEPTIONS_H


/** Exceptions */
#define ZEPHIR_THROW_EXCEPTION_STR(class_entry, message) \
	do { \
		zephir_throw_exception_string(class_entry, message, strlen(message) TSRMLS_CC); \
		ZEPHIR_MM_RESTORE(); \
	} while (0)

#define ZEPHIR_THROW_EXCEPTION_DEBUG_STR(class_entry, message, file, line) \
  do { \
    zephir_throw_exception_string_debug(class_entry, message, strlen(message), file, line TSRMLS_CC); \
    ZEPHIR_MM_RESTORE(); \
  } while (0)

#define ZEPHIR_THROW_EXCEPTION_ZVAL(class_entry, message) \
	do { \
		zephir_throw_exception_zval(class_entry, message TSRMLS_CC); \
		ZEPHIR_MM_RESTORE(); \
	} while (0)

#define ZEPHIR_THROW_EXCEPTION_DEBUG_ZVAL(class_entry, message, file, line) \
  do { \
    zephir_throw_exception_zval(class_entry, message, file, line TSRMLS_CC); \
    ZEPHIR_MM_RESTORE(); \
  } while (0)


#define ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(class_entry, message, file, line) zephir_throw_exception_string_debug(class_entry, message, strlen(message), file, line TSRMLS_CC)
#define ZEPHIR_THROW_EXCEPTION_STRW(class_entry, message) zephir_throw_exception_string(class_entry, message, strlen(message) TSRMLS_CC)
#define ZEPHIR_THROW_EXCEPTION_ZVALW(class_entry, message) zephir_throw_exception_zval(class_entry, message TSRMLS_CC)
#define ZEPHIR_THROW_EXCEPTION_DEBUG_ZVALW(class_entry, message, file, line) zephir_throw_exception_zval_debug(class_entry, message, file, line TSRMLS_CC)

/** Throw Exceptions */
void zephir_throw_exception(zval *object TSRMLS_DC);
void zephir_throw_exception_debug(zval *object, const char *file, zend_uint line TSRMLS_DC);
void zephir_throw_exception_string_debug(zend_class_entry *ce, const char *message, zend_uint message_len, const char *file, zend_uint line TSRMLS_DC);
void zephir_throw_exception_string(zend_class_entry *ce, const char *message, zend_uint message_len TSRMLS_DC);
void zephir_throw_exception_zval(zend_class_entry *ce, zval *message TSRMLS_DC);
void zephir_throw_exception_zval_debug(zend_class_entry *ce, zval *message, const char *file, zend_uint line TSRMLS_DC);
void zephir_throw_exception_internal(zval *exception TSRMLS_DC);

#endif /* ZEPHIR_KERNEL_EXCEPTIONS_H */




#ifndef ZEPHIR_KERNEL_REQUIRE_H
#define ZEPHIR_KERNEL_REQUIRE_H


int zephir_require_ret(zval **return_value_ptr, const char *require_path TSRMLS_DC) ZEPHIR_ATTR_NONNULL1(2);

ZEPHIR_ATTR_NONNULL static inline int zephir_require(const char *require_path TSRMLS_DC)
{
    return zephir_require_ret(NULL, require_path TSRMLS_CC);
}

ZEPHIR_ATTR_NONNULL static inline int zephir_require_zval(const zval *require_path TSRMLS_DC)
{
    return zephir_require_ret(NULL, Z_TYPE_P(require_path) == IS_STRING ? Z_STRVAL_P(require_path) : "" TSRMLS_CC);
}

ZEPHIR_ATTR_NONNULL static inline int zephir_require_zval_ret(zval **return_value_ptr, const zval *require_path TSRMLS_DC)
{
    return zephir_require_ret(return_value_ptr, Z_TYPE_P(require_path) == IS_STRING ? Z_STRVAL_P(require_path) : "" TSRMLS_CC);
}

#endif /* ZEPHIR_KERNEL_REQUIRE_H */



void zephir_serialize(zval *return_value, zval **var  TSRMLS_DC);
void zephir_unserialize(zval *return_value, zval *var TSRMLS_DC);

void zephir_var_export(zval **var TSRMLS_DC);
void zephir_var_export_ex(zval *return_value, zval **var TSRMLS_DC);

void zephir_var_dump(zval **var TSRMLS_DC);




#ifndef ZEPHIR_KERNEL_SESSION_H
#define ZEPHIR_KERNEL_SESSION_H

void zephir_session_start(TSRMLS_D);
void zephir_session_destroy(TSRMLS_D);
void zephir_get_session_id(zval *return_value, zval **return_value_ptr TSRMLS_DC);
void zephir_set_session_id(zval *sid TSRMLS_DC);

#endif /* ZEPHIR_KERNEL_SESSION_H */




#ifdef HAVE_CONFIG_H
#endif




zend_class_entry *zephir_register_internal_interface_ex(zend_class_entry *orig_ce, zend_class_entry *parent_ce TSRMLS_DC) {

	zend_class_entry *ce;

	ce = zend_register_internal_interface(orig_ce TSRMLS_CC);
	if (parent_ce) {
		zend_do_inheritance(ce, parent_ce TSRMLS_CC);
	}

	return ce;
}

int zephir_init_global(char *global, unsigned int global_length TSRMLS_DC) {

	#if PHP_VERSION_ID < 50400
	zend_bool jit_initialization = (PG(auto_globals_jit) && !PG(register_globals) && !PG(register_long_arrays));
	if (jit_initialization) {
		return zend_is_auto_global(global, global_length - 1 TSRMLS_CC);
	}
	#else
	if (PG(auto_globals_jit)) {
		return zend_is_auto_global(global, global_length - 1 TSRMLS_CC);
	}
	#endif

	return SUCCESS;
}

int zephir_get_global(zval **arr, const char *global, unsigned int global_length TSRMLS_DC) {

	zval **gv;

	zend_bool jit_initialization = PG(auto_globals_jit);
	if (jit_initialization) {
		zend_is_auto_global(global, global_length - 1 TSRMLS_CC);
	}

	if (&EG(symbol_table)) {
		if (zend_hash_find(&EG(symbol_table), global, global_length, (void **) &gv) == SUCCESS) {
			if (Z_TYPE_PP(gv) == IS_ARRAY) {
				*arr = *gv;
				if (!*arr) {
					ZEPHIR_INIT_VAR(*arr);
					array_init(*arr);
				}
			} else {
				ZEPHIR_INIT_VAR(*arr);
				array_init(*arr);
			}
			return SUCCESS;
		}
	}

	ZEPHIR_INIT_VAR(*arr);
	array_init(*arr);

	return SUCCESS;
}

void zephir_fast_count(zval *result, zval *value TSRMLS_DC) {

	if (Z_TYPE_P(value) == IS_ARRAY) {
		ZVAL_LONG(result, zend_hash_num_elements(Z_ARRVAL_P(value)));
		return;
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval *retval = NULL;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			ZVAL_LONG(result, 1);
			if (SUCCESS == Z_OBJ_HT(*value)->count_elements(value, &Z_LVAL_P(result) TSRMLS_CC)) {
				return;
			}
		}

		#ifdef HAVE_SPL
		if (Z_OBJ_HT_P(value)->get_class_entry && instanceof_function(Z_OBJCE_P(value), spl_ce_Countable TSRMLS_CC)) {
			zend_call_method_with_0_params(&value, NULL, NULL, "count", &retval);
			if (retval) {
				convert_to_long_ex(&retval);
				ZVAL_LONG(result, Z_LVAL_P(retval));
				zval_ptr_dtor(&retval);
			}
			return;
		}
		#endif

		ZVAL_LONG(result, 0);
		return;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		ZVAL_LONG(result, 0);
		return;
	}

	ZVAL_LONG(result, 1);
}

int zephir_fast_count_ev(zval *value TSRMLS_DC) {

	long count = 0;

	if (Z_TYPE_P(value) == IS_ARRAY) {
		return zend_hash_num_elements(Z_ARRVAL_P(value)) > 0;
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval *retval = NULL;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			Z_OBJ_HT(*value)->count_elements(value, &count TSRMLS_CC);
			return (int) count > 0;
		}

		#ifdef HAVE_SPL
		if (Z_OBJ_HT_P(value)->get_class_entry && instanceof_function(Z_OBJCE_P(value), spl_ce_Countable TSRMLS_CC)) {
			zend_call_method_with_0_params(&value, NULL, NULL, "count", &retval);
			if (retval) {
				convert_to_long_ex(&retval);
				count = Z_LVAL_P(retval);
				zval_ptr_dtor(&retval);
				return (int) count > 0;
			}
			return 0;
		}
		#endif

		return 0;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		return 0;
	}

	return 1;
}

int zephir_fast_count_int(zval *value TSRMLS_DC) {

	long count = 0;

	if (Z_TYPE_P(value) == IS_ARRAY) {
		return zend_hash_num_elements(Z_ARRVAL_P(value));
	}

	if (Z_TYPE_P(value) == IS_OBJECT) {

		#ifdef HAVE_SPL
		zval *retval = NULL;
		#endif

		if (Z_OBJ_HT_P(value)->count_elements) {
			Z_OBJ_HT(*value)->count_elements(value, &count TSRMLS_CC);
			return (int) count;
		}

		#ifdef HAVE_SPL
		if (Z_OBJ_HT_P(value)->get_class_entry && instanceof_function(Z_OBJCE_P(value), spl_ce_Countable TSRMLS_CC)) {
			zend_call_method_with_0_params(&value, NULL, NULL, "count", &retval);
			if (retval) {
				convert_to_long_ex(&retval);
				count = Z_LVAL_P(retval);
				zval_ptr_dtor(&retval);
				return (int) count;
			}
			return 0;
		}
		#endif

		return 0;
	}

	if (Z_TYPE_P(value) == IS_NULL) {
		return 0;
	}

	return 1;
}

int zephir_function_exists(const zval *function_name TSRMLS_DC) {

	return zephir_function_quick_exists_ex(
		Z_STRVAL_P(function_name),
		Z_STRLEN_P(function_name) + 1,
		zend_inline_hash_func(Z_STRVAL_P(function_name), Z_STRLEN_P(function_name) + 1) TSRMLS_CC
	);
}

int zephir_function_exists_ex(const char *function_name, unsigned int function_len TSRMLS_DC) {

	return zephir_function_quick_exists_ex(function_name, function_len, zend_inline_hash_func(function_name, function_len) TSRMLS_CC);
}

int zephir_function_quick_exists_ex(const char *method_name, unsigned int method_len, unsigned long key TSRMLS_DC) {

	if (zend_hash_quick_exists(CG(function_table), method_name, method_len, key)) {
		return SUCCESS;
	}

	return FAILURE;
}

int zephir_is_callable(zval *var TSRMLS_DC) {

	char *error = NULL;
	zend_bool retval;

	retval = zend_is_callable_ex(var, NULL, 0, NULL, NULL, NULL, &error TSRMLS_CC);
	if (error) {
		efree(error);
	}

	return (int) retval;
}

int zephir_is_iterable_ex(zval *arr, HashTable **arr_hash, HashPosition *hash_position, int duplicate, int reverse) {

	if (unlikely(Z_TYPE_P(arr) != IS_ARRAY)) {
		return 0;
	}

	if (duplicate) {
		ALLOC_HASHTABLE(*arr_hash);
		zend_hash_init(*arr_hash, 0, NULL, NULL, 0);
		zend_hash_copy(*arr_hash, Z_ARRVAL_P(arr), NULL, NULL, sizeof(zval*));
	} else {
		*arr_hash = Z_ARRVAL_P(arr);
	}

	if (reverse) {
		if (hash_position) {
			*hash_position = (*arr_hash)->pListTail;
		} else {
			(*arr_hash)->pInternalPointer = (*arr_hash)->pListTail;
		}
	} else {
		if (hash_position) {
			*hash_position = (*arr_hash)->pListHead;
		} else {
			(*arr_hash)->pInternalPointer = (*arr_hash)->pListHead;
		}
	}

	return 1;
}

void zephir_safe_zval_ptr_dtor(zval *pzval)
{
	if (pzval) {
		zval_ptr_dtor(&pzval);
	}
}

int zephir_fetch_parameters(int num_args TSRMLS_DC, int required_args, int optional_args, ...)
{
	va_list va;
	int arg_count = (int) (zend_uintptr_t) *(zend_vm_stack_top(TSRMLS_C) - 1);
	zval **arg, **p;
	int i;

	if (num_args < required_args || (num_args > (required_args + optional_args))) {
		zephir_throw_exception_string(spl_ce_BadMethodCallException, SL("Wrong number of parameters") TSRMLS_CC);
		return FAILURE;
	}

	if (num_args > arg_count) {
		zephir_throw_exception_string(spl_ce_BadMethodCallException, SL("Could not obtain parameters for parsing") TSRMLS_CC);
		return FAILURE;
	}

	if (!num_args) {
		return SUCCESS;
	}

	va_start(va, optional_args);

	i = 0;
	while (num_args-- > 0) {

		arg = (zval **) (zend_vm_stack_top(TSRMLS_C) - 1 - (arg_count - i));

		p = va_arg(va, zval **);
		*p = *arg;

		i++;
	}

	va_end(va);

	return SUCCESS;
}

void zephir_gettype(zval *return_value, zval *arg TSRMLS_DC) {

	switch (Z_TYPE_P(arg)) {

		case IS_NULL:
			RETVAL_STRING("NULL", 1);
			break;

		case IS_BOOL:
			RETVAL_STRING("boolean", 1);
			break;

		case IS_LONG:
			RETVAL_STRING("integer", 1);
			break;

		case IS_DOUBLE:
			RETVAL_STRING("double", 1);
			break;

		case IS_STRING:
			RETVAL_STRING("string", 1);
			break;

		case IS_ARRAY:
			RETVAL_STRING("array", 1);
			break;

		case IS_OBJECT:
			RETVAL_STRING("object", 1);
			break;

		case IS_RESOURCE:
			{
				const char *type_name = zend_rsrc_list_get_rsrc_type(Z_LVAL_P(arg) TSRMLS_CC);

				if (type_name) {
					RETVAL_STRING("resource", 1);
					break;
				}
			}

		default:
			RETVAL_STRING("unknown type", 1);
	}
}

zend_class_entry* zephir_get_internal_ce(const char *class_name, unsigned int class_name_len TSRMLS_DC) {
    zend_class_entry** temp_ce;

    if (zend_hash_find(CG(class_table), class_name, class_name_len, (void **)&temp_ce) == FAILURE) {
        zend_error(E_ERROR, "Class '%s' not found", class_name);
        return NULL;
    }

    return *temp_ce;
}





#include <Zend/zend_alloc.h>



static zephir_memory_entry* zephir_memory_grow_stack_common(zend_zephir_globals_def *g)
{
	assert(g->start_memory != NULL);
	if (!g->active_memory) {
		g->active_memory = g->start_memory;
#ifndef ZEPHIR_RELEASE
		assert(g->active_memory->permanent == 1);
#endif
	}
	else if (!g->active_memory->next) {
		assert(g->active_memory >= g->end_memory - 1 || g->active_memory < g->start_memory);
		zephir_memory_entry *entry = (zephir_memory_entry *) ecalloc(1, sizeof(zephir_memory_entry));
	/* ecalloc() will take care of these members
		entry->pointer   = 0;
		entry->capacity  = 0;
		entry->addresses = NULL;
		entry->hash_pointer   = 0;
		entry->hash_capacity  = 0;
		entry->hash_addresses = NULL;
		entry->next = NULL;
	*/
#ifndef ZEPHIR_RELEASE
		entry->permanent  = 0;
		entry->func       = NULL;
#endif
		entry->prev       = g->active_memory;
		entry->prev->next = entry;
		g->active_memory  = entry;
	}
	else {
#ifndef ZEPHIR_RELEASE
		assert(g->active_memory->permanent == 1);
#endif
		assert(g->active_memory < g->end_memory && g->active_memory >= g->start_memory);
		g->active_memory = g->active_memory->next;
	}

	assert(g->active_memory->pointer == 0);
	assert(g->active_memory->hash_pointer == 0);

	return g->active_memory;
}

static void zephir_memory_restore_stack_common(zend_zephir_globals_def *g TSRMLS_DC)
{
	size_t i;
	zephir_memory_entry *prev, *active_memory;
	zephir_symbol_table *active_symbol_table;

	active_memory = g->active_memory;
	assert(active_memory != NULL);

	if (EXPECTED(!CG(unclean_shutdown))) {
		/* Clean active symbol table */
		if (g->active_symbol_table) {
			active_symbol_table = g->active_symbol_table;
			if (active_symbol_table->scope == active_memory) {
				zend_hash_destroy(EG(active_symbol_table));
				FREE_HASHTABLE(EG(active_symbol_table));
				EG(active_symbol_table) = active_symbol_table->symbol_table;
				g->active_symbol_table = active_symbol_table->prev;
				efree(active_symbol_table);
			}
		}

		/* Check for non freed hash key zvals, mark as null to avoid string freeing */
		for (i = 0; i < active_memory->hash_pointer; ++i) {
			assert(active_memory->hash_addresses[i] != NULL && *(active_memory->hash_addresses[i]) != NULL);
			if (Z_REFCOUNT_PP(active_memory->hash_addresses[i]) <= 1) {
				ZVAL_NULL(*active_memory->hash_addresses[i]);
			} else {
				zval_copy_ctor(*active_memory->hash_addresses[i]);
			}
		}

#ifndef ZEPHIR_RELEASE
		for (i = 0; i < active_memory->pointer; ++i) {
			if (active_memory->addresses[i] != NULL && *(active_memory->addresses[i]) != NULL) {
				zval **var = active_memory->addresses[i];
#if PHP_VERSION_ID < 50400
				if (Z_TYPE_PP(var) > IS_CONSTANT_ARRAY) {
					fprintf(stderr, "%s: observed variable #%d (%p) has invalid type %u [%s]\n", __func__, (int)i, *var, Z_TYPE_PP(var), active_memory->func);
				}
#else
				if (Z_TYPE_PP(var) > IS_CALLABLE) {
					fprintf(stderr, "%s: observed variable #%d (%p) has invalid type %u [%s]\n", __func__, (int)i, *var, Z_TYPE_PP(var), active_memory->func);
				}
#endif

				if (Z_REFCOUNT_PP(var) == 0) {
					fprintf(stderr, "%s: observed variable #%d (%p) has 0 references, type=%d [%s]\n", __func__, (int)i, *var, Z_TYPE_PP(var), active_memory->func);
				}
				else if (Z_REFCOUNT_PP(var) >= 1000000) {
					fprintf(stderr, "%s: observed variable #%d (%p) has too many references (%u), type=%d  [%s]\n", __func__, (int)i, *var, Z_REFCOUNT_PP(var), Z_TYPE_PP(var), active_memory->func);
				}
#if 0
				/* Skip this check, PDO does return variables with is_ref = 1 and refcount = 1*/
				else if (Z_REFCOUNT_PP(var) == 1 && Z_ISREF_PP(var)) {
					fprintf(stderr, "%s: observed variable #%d (%p) is a reference with reference count = 1, type=%d  [%s]\n", __func__, (int)i, *var, Z_TYPE_PP(var), active_memory->func);
				}
#endif
			}
		}
#endif

		/* Traverse all zvals allocated, reduce the reference counting or free them */
		for (i = 0; i < active_memory->pointer; ++i) {
			if (EXPECTED(active_memory->addresses[i] != NULL && *(active_memory->addresses[i]) != NULL)) {
				if (Z_REFCOUNT_PP(active_memory->addresses[i]) == 1) {
					zval_ptr_dtor(active_memory->addresses[i]);
				} else {
					Z_DELREF_PP(active_memory->addresses[i]);
				}
			}
		}
	}

#ifndef ZEPHIR_RELEASE
	active_memory->func = NULL;
#endif

	prev = active_memory->prev;

	if (active_memory >= g->end_memory || active_memory < g->start_memory) {
#ifndef ZEPHIR_RELEASE
		assert(g->active_memory->permanent == 0);
#endif
		assert(prev != NULL);

		if (active_memory->hash_addresses != NULL) {
			efree(active_memory->hash_addresses);
		}

		if (active_memory->addresses != NULL) {
			efree(active_memory->addresses);
		}

		efree(g->active_memory);
		g->active_memory = prev;
		prev->next = NULL;
	}
	else {
#ifndef ZEPHIR_RELEASE
		assert(g->active_memory->permanent == 1);
#endif

		active_memory->pointer      = 0;
		active_memory->hash_pointer = 0;
		g->active_memory = prev;
	}

#ifndef ZEPHIR_RELEASE
	if (g->active_memory) {
		zephir_memory_entry *f = g->active_memory;
		if (f >= g->start_memory && f < g->end_memory - 1) {
			assert(f->permanent == 1);
			assert(f->next != NULL);

			if (f > g->start_memory) {
				assert(f->prev != NULL);
			}
		}
	}
#endif
}

#ifndef ZEPHIR_RELEASE

void zephir_dump_memory_frame(zephir_memory_entry *active_memory TSRMLS_DC)
{
	size_t i;

	assert(active_memory != NULL);

	fprintf(stderr, "Dump of the memory frame %p (%s)\n", active_memory, active_memory->func);

	if (active_memory->hash_pointer) {
		for (i = 0; i < active_memory->hash_pointer; ++i) {
			assert(active_memory->hash_addresses[i] != NULL && *(active_memory->hash_addresses[i]) != NULL);
			fprintf(stderr, "Hash ptr %lu (%p => %p), type=%u, refcnt=%u\n", (ulong)i, active_memory->hash_addresses[i], *active_memory->hash_addresses[i], Z_TYPE_PP(active_memory->hash_addresses[i]), Z_REFCOUNT_PP(active_memory->hash_addresses[i]));
		}
	}

	for (i = 0; i < active_memory->pointer; ++i) {
		if (EXPECTED(active_memory->addresses[i] != NULL && *(active_memory->addresses[i]) != NULL)) {
			zval **var = active_memory->addresses[i];
			fprintf(stderr, "Obs var %lu (%p => %p), type=%u, refcnt=%u; ", (ulong)i, var, *var, Z_TYPE_PP(var), Z_REFCOUNT_PP(var));
			switch (Z_TYPE_PP(var)) {
				case IS_NULL:     fprintf(stderr, "value=NULL\n"); break;
				case IS_LONG:     fprintf(stderr, "value=%ld\n", Z_LVAL_PP(var)); break;
				case IS_DOUBLE:   fprintf(stderr, "value=%E\n", Z_DVAL_PP(var)); break;
				case IS_BOOL:     fprintf(stderr, "value=(bool)%d\n", Z_BVAL_PP(var)); break;
				case IS_ARRAY:    fprintf(stderr, "value=array(%p), %d elements\n", Z_ARRVAL_PP(var), zend_hash_num_elements(Z_ARRVAL_PP(var))); break;
				case IS_OBJECT:   fprintf(stderr, "value=object(%u), %s\n", Z_OBJ_HANDLE_PP(var), Z_OBJCE_PP(var)->name); break;
				case IS_STRING:   fprintf(stderr, "value=%*s (%p)\n", Z_STRLEN_PP(var), Z_STRVAL_PP(var), Z_STRVAL_PP(var)); break;
				case IS_RESOURCE: fprintf(stderr, "value=(resource)%ld\n", Z_LVAL_PP(var)); break;
				default:          fprintf(stderr, "\n"); break;
			}
		}
	}

	fprintf(stderr, "End of the dump of the memory frame %p\n", active_memory);
}

void zephir_dump_current_frame(TSRMLS_D)
{
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	if (UNEXPECTED(zephir_globals_ptr->active_memory == NULL)) {
		fprintf(stderr, "WARNING: calling %s() without an active memory frame!\n", __func__);
		zephir_print_backtrace();
		return;
	}

	zephir_dump_memory_frame(zephir_globals_ptr->active_memory TSRMLS_CC);
}

void zephir_dump_all_frames(TSRMLS_D)
{
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;
	zephir_memory_entry *active_memory       = zephir_globals_ptr->active_memory;

	fprintf(stderr, "*** DUMP START ***\n");
	while (active_memory != NULL) {
		zephir_dump_memory_frame(active_memory TSRMLS_CC);
		active_memory = active_memory->prev;
	}

	fprintf(stderr, "*** DUMP END ***\n");
}

int ZEND_FASTCALL zephir_memory_restore_stack(const char *func TSRMLS_DC)
{
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	if (UNEXPECTED(zephir_globals_ptr->active_memory == NULL)) {
		fprintf(stderr, "WARNING: calling zephir_memory_restore_stack() without an active memory frame!\n");
		zephir_print_backtrace();
		return FAILURE;
	}

	if (UNEXPECTED(zephir_globals_ptr->active_memory->func != func)) {
		fprintf(stderr, "Trying to free someone else's memory frame!\n");
		fprintf(stderr, "The frame was created by %s\n", zephir_globals_ptr->active_memory->func);
		fprintf(stderr, "Calling function: %s\n", func);
		zephir_print_backtrace();
	}

	zephir_memory_restore_stack_common(zephir_globals_ptr TSRMLS_CC);
	return SUCCESS;
}

void ZEND_FASTCALL zephir_memory_grow_stack(const char *func TSRMLS_DC)
{
	zephir_memory_entry *entry = zephir_memory_grow_stack_common(ZEPHIR_VGLOBAL);
	entry->func = func;
}
#else
void ZEND_FASTCALL zephir_memory_grow_stack(TSRMLS_D)
{
	zephir_memory_grow_stack_common(ZEPHIR_VGLOBAL);
}

int ZEND_FASTCALL zephir_memory_restore_stack(TSRMLS_D)
{
	zephir_memory_restore_stack_common(ZEPHIR_VGLOBAL TSRMLS_CC);
	return SUCCESS;
}
#endif

ZEPHIR_ATTR_NONNULL static void zephir_reallocate_memory(const zend_zephir_globals_def *g)
{
	zephir_memory_entry *frame = g->active_memory;
	int persistent = (frame >= g->start_memory && frame < g->end_memory);
	void *buf = perealloc(frame->addresses, sizeof(zval **) * (frame->capacity + 16), persistent);
	if (EXPECTED(buf != NULL)) {
		frame->capacity += 16;
		frame->addresses = buf;
	}
	else {
		zend_error(E_CORE_ERROR, "Memory allocation failed");
	}

#ifndef ZEPHIR_RELEASE
	assert(frame->permanent == persistent);
#endif
}

ZEPHIR_ATTR_NONNULL static void zephir_reallocate_hmemory(const zend_zephir_globals_def *g)
{
	zephir_memory_entry *frame = g->active_memory;
	int persistent = (frame >= g->start_memory && frame < g->end_memory);
	void *buf = perealloc(frame->hash_addresses, sizeof(zval **) * (frame->hash_capacity + 4), persistent);
	if (EXPECTED(buf != NULL)) {
		frame->hash_capacity += 4;
		frame->hash_addresses = buf;
	}
	else {
		zend_error(E_CORE_ERROR, "Memory allocation failed");
	}

#ifndef ZEPHIR_RELEASE
	assert(frame->permanent == persistent);
#endif
}

ZEPHIR_ATTR_NONNULL1(2) static inline void zephir_do_memory_observe(zval **var, const zend_zephir_globals_def *g)
{
	zephir_memory_entry *frame = g->active_memory;
#ifndef ZEPHIR_RELEASE
	if (UNEXPECTED(frame == NULL)) {
		fprintf(stderr, "ZEPHIR_MM_GROW() must be called before using any of MM functions or macros!");
		zephir_print_backtrace();
		abort();
	}
#endif

	if (UNEXPECTED(frame->pointer == frame->capacity)) {
		zephir_reallocate_memory(g);
	}

#ifndef ZEPHIR_RELEASE
	{
		size_t i;
		for (i = 0; i < frame->pointer; ++i) {
			if (frame->addresses[i] == var) {
				fprintf(stderr, "Variable %p is already observed", var);
				zephir_print_backtrace();
				abort();
			}
		}
	}
#endif

	frame->addresses[frame->pointer] = var;
	++frame->pointer;
}

void ZEND_FASTCALL zephir_memory_observe(zval **var TSRMLS_DC)
{
	zend_zephir_globals_def *g = ZEPHIR_VGLOBAL;
	zephir_do_memory_observe(var, g);
	*var = NULL; /* In case an exception or error happens BEFORE the observed variable gets initialized */
}

void ZEND_FASTCALL zephir_memory_alloc(zval **var TSRMLS_DC)
{
	zend_zephir_globals_def *g = ZEPHIR_VGLOBAL;
	zephir_do_memory_observe(var, g);
	ALLOC_INIT_ZVAL(*var);
}

void ZEND_FASTCALL zephir_memory_alloc_pnull(zval **var TSRMLS_DC)
{
	zend_zephir_globals_def *g = ZEPHIR_VGLOBAL;
	zephir_memory_entry *active_memory = g->active_memory;

#ifndef ZEPHIR_RELEASE
	if (UNEXPECTED(active_memory == NULL)) {
		fprintf(stderr, "ZEPHIR_MM_GROW() must be called before using any of MM functions or macros!");
		zephir_print_backtrace();
		abort();
	}
#endif

	zephir_do_memory_observe(var, g);
	ALLOC_INIT_ZVAL(*var);

	if (active_memory->hash_pointer == active_memory->hash_capacity) {
		zephir_reallocate_hmemory(g);
	}

	active_memory->hash_addresses[active_memory->hash_pointer] = var;
	++active_memory->hash_pointer;
}

void ZEND_FASTCALL zephir_memory_remove(zval **var TSRMLS_DC) {
	zval_ptr_dtor(var);
	*var = NULL;
}

int ZEND_FASTCALL zephir_clean_restore_stack(TSRMLS_D) {

	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	while (zephir_globals_ptr->active_memory != NULL) {
		zephir_memory_restore_stack_common(zephir_globals_ptr TSRMLS_CC);
	}

	return SUCCESS;
}

void ZEND_FASTCALL zephir_copy_ctor(zval *destination, zval *origin) {
	if (Z_REFCOUNT_P(origin) > 1) {
		zval_copy_ctor(destination);
	} else {
		ZVAL_NULL(origin);
	}
}

void zephir_create_symbol_table(TSRMLS_D) {

	zephir_symbol_table *entry;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;
	HashTable *symbol_table;

#ifndef ZEPHIR_RELEASE
	if (!zephir_globals_ptr->active_memory) {
		fprintf(stderr, "ERROR: Trying to create a virtual symbol table without a memory frame");
		zephir_print_backtrace();
		return;
	}
#endif

	entry = (zephir_symbol_table *) emalloc(sizeof(zephir_symbol_table));
	entry->scope = zephir_globals_ptr->active_memory;
	entry->symbol_table = EG(active_symbol_table);
	entry->prev = zephir_globals_ptr->active_symbol_table;
	zephir_globals_ptr->active_symbol_table = entry;

	ALLOC_HASHTABLE(symbol_table);
	zend_hash_init(symbol_table, 0, NULL, ZVAL_PTR_DTOR, 0);
	EG(active_symbol_table) = symbol_table;
}

void zephir_clean_symbol_tables(TSRMLS_D) {

	/*unsigned int i;

	if (ZEPHIR_GLOBAL(symbol_tables)) {
		for (i = ZEPHIR_GLOBAL(number_symbol_tables); i > 0; i--) {
			EG(active_symbol_table) = ZEPHIR_GLOBAL(symbol_tables)[i - 1];
		}
		efree(ZEPHIR_GLOBAL(symbol_tables));
		ZEPHIR_GLOBAL(symbol_tables) = NULL;
	}*/
}

int zephir_set_symbol(zval *key_name, zval *value TSRMLS_DC) {

	if (!EG(active_symbol_table)) {
		zend_rebuild_symbol_table(TSRMLS_C);
	}

	if (EG(active_symbol_table)) {
		if (Z_TYPE_P(key_name) == IS_STRING) {
			Z_ADDREF_P(value);
			zend_hash_update(EG(active_symbol_table), Z_STRVAL_P(key_name), Z_STRLEN_P(key_name) + 1, &value, sizeof(zval *), NULL);
			if (EG(exception)) {
				return FAILURE;
			}
		}
	}

	return SUCCESS;
}

int zephir_set_symbol_str(char *key_name, unsigned int key_length, zval *value TSRMLS_DC) {

	if (!EG(active_symbol_table)) {
		zend_rebuild_symbol_table(TSRMLS_C);
	}

	if (&EG(symbol_table)) {
		Z_ADDREF_P(value);
		zend_hash_update(&EG(symbol_table), key_name, key_length, &value, sizeof(zval *), NULL);
		if (EG(exception)) {
			return FAILURE;
		}
	}

	return SUCCESS;
}

static inline void zephir_dtor_func(zval *zvalue ZEND_FILE_LINE_DC)
{
	switch (Z_TYPE_P(zvalue) & IS_CONSTANT_TYPE_MASK) {
		case IS_STRING:
		case IS_CONSTANT:
			CHECK_ZVAL_STRING_REL(zvalue);
			STR_FREE_REL(zvalue->value.str.val);
			break;
#if PHP_VERSION_ID < 50600
		case IS_CONSTANT_ARRAY:
#endif
		case IS_ARRAY:  {
				TSRMLS_FETCH();
				if (zvalue->value.ht && (zvalue->value.ht != &EG(symbol_table))) {
					/* break possible cycles */
					Z_TYPE_P(zvalue) = IS_NULL;
					zend_hash_destroy(zvalue->value.ht);
					FREE_HASHTABLE(zvalue->value.ht);
				}
			}
			break;
		case IS_OBJECT:
			{
				TSRMLS_FETCH();
				Z_OBJ_HT_P(zvalue)->del_ref(zvalue TSRMLS_CC);
			}
			break;
		case IS_RESOURCE:
			{
				TSRMLS_FETCH();
				zend_list_delete(zvalue->value.lval);
			}
			break;
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_NULL:
		default:
			return;
			break;
	}
}

void zephir_value_dtor(zval *zvalue ZEND_FILE_LINE_DC)
{
	if (zvalue->type <= IS_BOOL) {
		return;
	}
	zephir_dtor_func(zvalue ZEND_FILE_LINE_RELAY_CC);
}




#ifndef ZEPHIR_RELEASE
#if defined(linux)

#include <execinfo.h>
#include <Zend/zend.h>
#include <ext/standard/php_smart_str.h>

void *backtrace_buf[4096];

void zephir_print_backtrace(void)
{
	int i;
	int stack_size       = backtrace(backtrace_buf, sizeof(backtrace_buf) / sizeof(void*));
	char **stack_symbols = backtrace_symbols(backtrace_buf, stack_size);
	char buf[50];
	smart_str s;

	s.c = NULL;

	for (i = 0; i < stack_size; ++i) {
		snprintf(buf, sizeof(buf), "#%d  %p [", i, backtrace_buf[i]);
		smart_str_appends(&s, buf);
		smart_str_appends(&s, stack_symbols[i]);
		smart_str_appends(&s, "]\n");
	}

	smart_str_0(&s);

	fprintf(stderr, "%s\n", s.c);
	smart_str_free(&s);
}

#else

#include <execinfo.h>
#include <Zend/zend.h>
#include <ext/standard/php_smart_str.h>

void *backtrace_buf[4096];

void zephir_print_backtrace(void)
{
	int i;
	int stack_size       = backtrace(backtrace_buf, sizeof(backtrace_buf) / sizeof(void*));
	char **stack_symbols = backtrace_symbols(backtrace_buf, stack_size);
	char buf[50];
	smart_str s;

	s.c = NULL;

	for (i = 0; i < stack_size; ++i) {
		snprintf(buf, sizeof(buf), "#%d  %p [", i, backtrace_buf[i]);
		smart_str_appends(&s, buf);
		smart_str_appends(&s, stack_symbols[i]);
		smart_str_appends(&s, "]\n");
	}

	smart_str_0(&s);

	fprintf(stdout, "%s\n", s.c);
	fprintf(stderr, "%s\n", s.c);
	smart_str_free(&s);
}

#endif
#endif /* ZEPHIR_RELEASE */




#include <php.h>

#include <Zend/zend_API.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_execute.h>


#if PHP_VERSION_ID >= 50500
static const unsigned char tolower_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};
#endif

int zephir_has_constructor_ce(const zend_class_entry *ce)
{
	while (ce) {
		if (ce->constructor) {
			return 1;
		}

		ce = ce->parent;
	}

	return 0;
}

#if 0
static inline ulong zephir_update_hash(const char *arKey, uint nKeyLength, ulong hash)
{
	for (; nKeyLength >= 8; nKeyLength -= 8) {
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
		hash = ((hash << 5) + hash) + *arKey++;
	}

	switch (nKeyLength) {
		case 7:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 6:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 5:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 4:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 3:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 2:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		case 1:  hash = ((hash << 5) + hash) + *arKey++;
		/* no break */
		default: break;
	}

	return hash;
}
#endif

static ulong zephir_make_fcall_key(char **result, size_t *length, const zend_class_entry *obj_ce, zephir_call_type type, zval *function_name TSRMLS_DC)
{
	const zend_class_entry *calling_scope = EG(scope);
	char *buf = NULL, *c;
	size_t l = 0, len = 0;
	const size_t ppzce_size = sizeof(zend_class_entry**);
	ulong hash = 5381;

	*result = NULL;
	*length = 0;

	if (calling_scope && type == zephir_fcall_parent) {
		calling_scope = calling_scope->parent;
		if (UNEXPECTED(!calling_scope)) {
			return 0;
		}
	}
	else if (type == zephir_fcall_static) {
		calling_scope = EG(called_scope);
		if (UNEXPECTED(!calling_scope)) {
			return 0;
		}
	}

	if (
		    calling_scope
		 && obj_ce
		 && calling_scope != obj_ce
		 && !instanceof_function(obj_ce, calling_scope TSRMLS_CC)
		 && !instanceof_function(calling_scope, obj_ce TSRMLS_CC)
	) {
		calling_scope = NULL;
	}

	if (Z_TYPE_P(function_name) == IS_STRING) {
		l   = (size_t)(Z_STRLEN_P(function_name)) + 1;
		c   = Z_STRVAL_P(function_name);
		len = 2 * ppzce_size + l;
		buf = ecalloc(1, len);

		memcpy(buf,                  c,               l);
		memcpy(buf + l,              &calling_scope,  ppzce_size);
		memcpy(buf + l + ppzce_size, &obj_ce,         ppzce_size);
	}
	else if (Z_TYPE_P(function_name) == IS_ARRAY) {
		zval **method;
		HashTable *function_hash = Z_ARRVAL_P(function_name);
		if (
			    function_hash->nNumOfElements == 2
			 && zend_hash_index_find(function_hash, 1, (void**)&method) == SUCCESS
			 && Z_TYPE_PP(method) == IS_STRING
		) {
			l   = (size_t)(Z_STRLEN_PP(method)) + 1;
			c   = Z_STRVAL_PP(method);
			len = 2 * ppzce_size + l;
			buf = ecalloc(1, len);

			memcpy(buf,                  c,               l);
			memcpy(buf + l,              &calling_scope,  ppzce_size);
			memcpy(buf + l + ppzce_size, &obj_ce,         ppzce_size);
		}
	}
	else if (Z_TYPE_P(function_name) == IS_OBJECT) {
		if (Z_OBJ_HANDLER_P(function_name, get_closure)) {
			l   = sizeof("__invoke");
			len = 2 * ppzce_size + l;
			buf = ecalloc(1, len);

			memcpy(buf,                  "__invoke",     l);
			memcpy(buf + l,              &calling_scope, ppzce_size);
			memcpy(buf + l + ppzce_size, &obj_ce,        ppzce_size);
		}
	}

	if (EXPECTED(buf != NULL)) {
		size_t i;

		for (i = 0; i < l; ++i) {
			char c = buf[i];
#if PHP_VERSION_ID >= 50500
			c = tolower_map[(unsigned char)c];
#else
			c = tolower(c);
#endif
			buf[i] = c;
			hash   = (hash << 5) + hash + c;
		}

		for (i = l; i < len; ++i) {
			char c = buf[i];
			hash = (hash << 5) + hash + c;
		}
	}

	*result = buf;
	*length = len;
	return hash;
}

ZEPHIR_ATTR_NONNULL static void zephir_fcall_populate_fci_cache(zend_fcall_info_cache *fcic, zend_fcall_info *fci, zephir_call_type type TSRMLS_DC)
{
	switch (type) {
		case zephir_fcall_parent:
			if (EG(scope) && EG(scope)->parent) {
				fcic->calling_scope = EG(scope)->parent;
				fcic->called_scope  = EG(called_scope);
				fcic->object_ptr    = fci->object_ptr ? fci->object_ptr : EG(This);
				fcic->initialized   = 1;
			}

			break;

		case zephir_fcall_self:
			if (EG(scope)) {
				fcic->calling_scope = EG(scope);
				fcic->called_scope  = EG(called_scope);
				fcic->object_ptr    = fci->object_ptr ? fci->object_ptr : EG(This);
				fcic->initialized   = 1;
			}

			break;

		case zephir_fcall_static:
			if (EG(called_scope)) {
				fcic->calling_scope = EG(called_scope);
				fcic->called_scope  = EG(called_scope);
				fcic->object_ptr    = fci->object_ptr ? fci->object_ptr : EG(This);
				fcic->initialized   = 1;
			}

			break;

		case zephir_fcall_function:
			fcic->calling_scope = NULL;
			fcic->called_scope  = NULL;
			fcic->object_ptr    = NULL;
			fcic->initialized   = 1;
			break;

		case zephir_fcall_ce: {
			zend_class_entry *scope = EG(active_op_array) ? EG(active_op_array)->scope : NULL;

			fcic->initialized      = 1;
			fcic->calling_scope    = EG(scope);
			fcic->object_ptr       = NULL;

			if (scope && EG(This) && instanceof_function(Z_OBJCE_P(EG(This)), scope TSRMLS_CC) && instanceof_function(scope, fcic->calling_scope TSRMLS_CC)) {
				fcic->object_ptr   = EG(This);
				fcic->called_scope = Z_OBJCE_P(fcic->object_ptr);
			}
			else {
				fcic->called_scope = fcic->calling_scope;
			}

			break;
		}

		case zephir_fcall_method:
			fcic->initialized      = 1;
			fcic->calling_scope    = EG(scope);
			fcic->object_ptr       = fci->object_ptr;
			if (fci->object_ptr) {
				fcic->called_scope = Z_OBJCE_P(fci->object_ptr);
			}
			else if (EG(scope) && !(EG(called_scope) && instanceof_function(EG(called_scope), EG(scope) TSRMLS_CC))) {
				fcic->called_scope = EG(scope);
			}
			else {
				fcic->called_scope = EG(called_scope);
			}

			break;

		default:
#ifndef ZEPHIR_RELEASE
			fprintf(stderr, "%s: unknown call type (%d)\n", __func__, (int)type);
			abort();
#endif
			fcic->initialized = 0; /* not strictly necessary but just to be safe */
			break;
	}

}

int zephir_call_user_function(zval **object_pp, zend_class_entry *obj_ce, zephir_call_type type,
	zval *function_name, zval **retval_ptr_ptr, zephir_fcall_cache_entry **cache_entry, zend_uint param_count,
	zval *params[] TSRMLS_DC)
{
	zval ***params_ptr, ***params_array = NULL;
	zval **static_params_array[10];
	zval *local_retval_ptr = NULL;
	int status;
	zend_fcall_info fci;
	zend_fcall_info_cache fcic /* , clone */;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;
	char *fcall_key = NULL;
	size_t fcall_key_len;
	ulong fcall_key_hash;
	zephir_fcall_cache_entry **temp_cache_entry = NULL;
	zend_class_entry *old_scope = EG(scope);

	assert(obj_ce || !object_pp);

	if (retval_ptr_ptr && *retval_ptr_ptr) {
		zval_ptr_dtor(retval_ptr_ptr);
		*retval_ptr_ptr = NULL;
	}

	++zephir_globals_ptr->recursive_lock;

	if (UNEXPECTED(zephir_globals_ptr->recursive_lock > 2048)) {
		
		PHPWRITE(Z_STRVAL_P(function_name), Z_STRLEN_P(function_name));
		//PHPWRITE(Z_STRVAL_P(params[0]), Z_STRLEN_P(params[0]));
		zend_error(E_ERROR, "Maximum recursion depth exceeded");
		return FAILURE;
	}

	if (param_count) {
		zend_uint i;

		if (UNEXPECTED(param_count > 10)) {
			params_array = (zval***)emalloc(param_count * sizeof(zval**));
			params_ptr   = params_array;
			for (i = 0; i < param_count; ++i) {
				params_array[i] = &params[i];
			}
		} else {
			params_ptr = static_params_array;
			for (i = 0; i < param_count; ++i) {
				static_params_array[i] = &params[i];
			}
		}
	}
	else {
		params_ptr = NULL;
	}

	if (type != zephir_fcall_function && !object_pp) {
		object_pp = EG(This) ? &EG(This) : NULL;
		if (!obj_ce && object_pp) {
			obj_ce = Z_OBJCE_PP(object_pp);
		}
	}

	if (obj_ce) {
		EG(scope) = obj_ce;
	}

	if (!cache_entry || !*cache_entry) {
		fcall_key_hash = zephir_make_fcall_key(&fcall_key, &fcall_key_len, (object_pp ? Z_OBJCE_PP(object_pp) : obj_ce), type, function_name TSRMLS_CC);
	}

	fci.size           = sizeof(fci);
	fci.function_table = obj_ce ? &obj_ce->function_table : EG(function_table);
	fci.object_ptr     = object_pp ? *object_pp : NULL;
	fci.function_name  = function_name;
	fci.retval_ptr_ptr = retval_ptr_ptr ? retval_ptr_ptr : &local_retval_ptr;
	fci.param_count    = param_count;
	fci.params         = params_ptr;
	fci.no_separation  = 1;
	fci.symbol_table   = NULL;

	fcic.initialized = 0;
	if (!cache_entry || !*cache_entry) {
		if (fcall_key && zend_hash_quick_find(zephir_globals_ptr->fcache, fcall_key, fcall_key_len, fcall_key_hash, (void**)&temp_cache_entry) != FAILURE) {
			zephir_fcall_populate_fci_cache(&fcic, &fci, type TSRMLS_CC);

#ifndef ZEPHIR_RELEASE
			fcic.function_handler = (*temp_cache_entry)->f;
			++(*temp_cache_entry)->times;
#else
			fcic.function_handler = *temp_cache_entry;
#endif
			/*memcpy(&clone, &fcic, sizeof(clone));*/
		}
	} else {
		zephir_fcall_populate_fci_cache(&fcic, &fci, type TSRMLS_CC);
#ifndef ZEPHIR_RELEASE
		fcic.function_handler = (*cache_entry)->f;
		++(*temp_cache_entry)->times;
#else
		fcic.function_handler = *cache_entry;
#endif
	}

	/* fcic.initialized = 0; */
	status = ZEPHIR_ZEND_CALL_FUNCTION_WRAPPER(&fci, &fcic TSRMLS_CC);

	EG(scope) = old_scope;

	if (!cache_entry || !*cache_entry) {
		if (EXPECTED(status != FAILURE) && fcall_key && !temp_cache_entry) {
	#ifndef ZEPHIR_RELEASE
			zephir_fcall_cache_entry *temp_cache_entry = malloc(sizeof(zephir_fcall_cache_entry));
			cache_entry->f     = fcic.function_handler;
			cache_entry->times = 0;
	#else
			zephir_fcall_cache_entry *temp_cache_entry = fcic.function_handler;
	#endif
			if (FAILURE == zend_hash_quick_add(zephir_globals_ptr->fcache, fcall_key, fcall_key_len, fcall_key_hash, &temp_cache_entry, sizeof(zephir_fcall_cache_entry*), NULL)) {
	#ifndef ZEPHIR_RELEASE
				free(temp_cache_entry);
	#endif
			} else {
				if (cache_entry) {
					*cache_entry = temp_cache_entry;
				}
			}
		}
	}

	//#ifndef ZEPHIR_RELEASE
	//if (Z_ISREF_P(zephir_globals_ptr->global_null)) {
	//	fprintf(stderr, "F=%s\n", fcall_key);
	//}
	//#endif

	if (fcall_key) {
		efree(fcall_key);
	}

	if (UNEXPECTED(params_array != NULL)) {
		efree(params_array);
	}

	if (!retval_ptr_ptr) {
		if (local_retval_ptr) {
			zval_ptr_dtor(&local_retval_ptr);
		}
	}

	--zephir_globals_ptr->recursive_lock;
	return status;
}

int zephir_call_func_aparams(zval **return_value_ptr, const char *func_name, uint func_length,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC)
{
	int status;
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;
	zval func = zval_used_for_init;

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr && *return_value_ptr) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	ZVAL_STRINGL(&func, func_name, func_length, 0);
	status = zephir_call_user_function(NULL, NULL, zephir_fcall_function, &func, rvp, cache_entry, param_count, params TSRMLS_CC);

	if (status == FAILURE && !EG(exception)) {
		zend_error(E_ERROR, "Call to undefined function %s()", func_name);
	} else {
		if (EG(exception)) {
			status = FAILURE;
			if (return_value_ptr) {
				*return_value_ptr = NULL;
			}
		}
	}

	if (rv) {
		zval_ptr_dtor(&rv);
	}

	return status;
}

int zephir_call_zval_func_aparams(zval **return_value_ptr, zval *func_name,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC)
{
	int status;
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr && *return_value_ptr) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	status = zephir_call_user_function(NULL, NULL, zephir_fcall_function, func_name, rvp, cache_entry, param_count, params TSRMLS_CC);

	if (status == FAILURE && !EG(exception)) {
		zend_error(E_ERROR, "Call to undefined function %s()", Z_TYPE_P(func_name) ? Z_STRVAL_P(func_name) : "undefined");
	} else {
		if (EG(exception)) {
			status = FAILURE;
			if (return_value_ptr) {
				*return_value_ptr = NULL;
			}
		}
	}

	if (rv) {
		zval_ptr_dtor(&rv);
	}

	return status;
}

int zephir_call_class_method_aparams(zval **return_value_ptr, zend_class_entry *ce, zephir_call_type type, zval *object,
	const char *method_name, uint method_len,
	zephir_fcall_cache_entry **cache_entry,
	uint param_count, zval **params TSRMLS_DC)
{
	zval *rv = NULL, **rvp = return_value_ptr ? return_value_ptr : &rv;
	zval fn = zval_used_for_init;
	zval *mn;
	int status;

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr && *return_value_ptr) {
		fprintf(stderr, "%s: *return_value_ptr must be NULL\n", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	if (object) {
		if (Z_TYPE_P(object) != IS_OBJECT) {
			zend_error(E_ERROR, "Trying to call method %s on a non-object", method_name);
		}
	}

	if (!cache_entry || !*cache_entry) {

		array_init_size(&fn, 2);
		switch (type) {
			case zephir_fcall_parent: add_next_index_stringl(&fn, ZEND_STRL("parent"), 1); break;
			case zephir_fcall_self:   assert(!ce); add_next_index_stringl(&fn, ZEND_STRL("self"), 1); break;
			case zephir_fcall_static: assert(!ce); add_next_index_stringl(&fn, ZEND_STRL("static"), 1); break;

			case zephir_fcall_ce:
				assert(ce != NULL);
				add_next_index_stringl(&fn, ce->name, ce->name_length, !IS_INTERNED(ce->name));
				break;

			case zephir_fcall_method:
			default:
				assert(object != NULL);
				Z_ADDREF_P(object);
				add_next_index_zval(&fn, object);
				break;
		}

		ALLOC_INIT_ZVAL(mn);
		ZVAL_STRINGL(mn, method_name, method_len, 0);
		add_next_index_zval(&fn, mn);

	} else {
		ZVAL_STRINGL(&fn, "undefined", sizeof("undefined")-1, 0);
	}

	status = zephir_call_user_function(object ? &object : NULL, ce, type, &fn, rvp, cache_entry, param_count, params TSRMLS_CC);

	if (Z_TYPE_P(&fn) == IS_ARRAY) {
		if (Z_REFCOUNT_P(mn) > 1) {
			zval_copy_ctor(mn);
		} else {
			ZVAL_NULL(mn);
		}
		zval_ptr_dtor(&mn);
	}

	if (status == FAILURE && !EG(exception)) {
		switch (type) {
			case zephir_fcall_parent: zend_error(E_ERROR, "Call to undefined function parent::%s()", method_name); break;
			case zephir_fcall_self:   zend_error(E_ERROR, "Call to undefined function self::%s()", method_name); break;
			case zephir_fcall_static: zend_error(E_ERROR, "Call to undefined function static::%s()", method_name); break;
			case zephir_fcall_ce:     zend_error(E_ERROR, "Call to undefined function %s::%s()", ce->name, method_name); break;
			case zephir_fcall_method: zend_error(E_ERROR, "Call to undefined function %s::%s()", Z_OBJCE_P(object)->name, method_name); break;
			default:                   zend_error(E_ERROR, "Call to undefined function ?::%s()", method_name);
		}
	}
	else if (EG(exception)) {
		status = FAILURE;
		if (return_value_ptr) {
			*return_value_ptr = NULL;
		}
	}

	if (rv) {
		zval_ptr_dtor(&rv);
	}

	if (Z_TYPE_P(&fn) == IS_ARRAY) {
		zval_dtor(&fn);
	}
	return status;
}

int zephir_call_user_func_array_noex(zval *return_value, zval *handler, zval *params TSRMLS_DC){

	zval *retval_ptr = NULL;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
	char *is_callable_error = NULL;
	int status = FAILURE;

	if (params && Z_TYPE_P(params) != IS_ARRAY) {
		ZVAL_NULL(return_value);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_call_user_func_array_noex()");
		return FAILURE;
	}

	if (zend_fcall_info_init(handler, 0, &fci, &fci_cache, NULL, &is_callable_error TSRMLS_CC) == SUCCESS) {
		if (is_callable_error) {
			zend_error(E_STRICT, "%s", is_callable_error);
			efree(is_callable_error);
		}
		status = SUCCESS;
	} else {
		if (is_callable_error) {
			zend_error(E_WARNING, "%s", is_callable_error);
			efree(is_callable_error);
		} else {
			status = SUCCESS;
		}
	}

	if (status == SUCCESS) {

		zend_fcall_info_args(&fci, params TSRMLS_CC);
		fci.retval_ptr_ptr = &retval_ptr;

		if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && fci.retval_ptr_ptr && *fci.retval_ptr_ptr) {
			COPY_PZVAL_TO_ZVAL(*return_value, *fci.retval_ptr_ptr);
		}

		if (fci.params) {
			efree(fci.params);
		}
	}

	if (EG(exception)) {
		status = SUCCESS;
	}

	return status;
}

#if PHP_VERSION_ID <= 50309

void zephir_throw_exception_internal(zval *exception TSRMLS_DC)
{
	if (exception != NULL) {
		zval *previous = EG(exception);
		zend_exception_set_previous(exception, EG(exception) TSRMLS_CC);
		EG(exception) = exception;
		if (previous) {
			return;
		}
	}

	if (!EG(current_execute_data)) {
		if (EG(exception)) {
			zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
		}
		zend_error(E_ERROR, "Exception thrown without a stack frame");
	}

	if (zend_throw_exception_hook) {
		zend_throw_exception_hook(exception TSRMLS_CC);
	}

	if (EG(current_execute_data)->opline == NULL ||
		(EG(current_execute_data)->opline + 1)->opcode == ZEND_HANDLE_EXCEPTION) {
		/* no need to rethrow the exception */
		return;
	}

	EG(opline_before_exception) = EG(current_execute_data)->opline;
	EG(current_execute_data)->opline = EG(exception_op);
}

int zephir_call_function(zend_fcall_info *fci, zend_fcall_info_cache *fci_cache TSRMLS_DC) {

	zend_uint i;
	zval **original_return_value;
	HashTable *calling_symbol_table;
	zend_op_array *original_op_array;
	zend_op **original_opline_ptr;
	zend_class_entry *current_scope;
	zend_class_entry *current_called_scope;
	zend_class_entry *calling_scope = NULL;
	zend_class_entry *called_scope = NULL;
	zval *current_this;
	zend_execute_data execute_data;

	*fci->retval_ptr_ptr = NULL;

	if (!EG(active)) {
		return FAILURE; /* executor is already inactive */
	}

	if (EG(exception)) {
		return FAILURE; /* we would result in an instable executor otherwise */
	}

	switch (fci->size) {
		case sizeof(zend_fcall_info):
			break; /* nothing to do currently */
		default:
			zend_error(E_ERROR, "Corrupted fcall_info provided to zephir_call_function()");
			break;
	}

	/* Initialize execute_data */
	if (EG(current_execute_data)) {
		execute_data = *EG(current_execute_data);
		EX(op_array) = NULL;
		EX(opline) = NULL;
		EX(object) = NULL;
	} else {
		/* This only happens when we're called outside any execute()'s
		 * It shouldn't be strictly necessary to NULL execute_data out,
		 * but it may make bugs easier to spot
		 */
		memset(&execute_data, 0, sizeof(zend_execute_data));
	}

	if (!fci_cache || !fci_cache->initialized) {

		zend_fcall_info_cache fci_cache_local;
		char *callable_name;
		char *error = NULL;

		if (!fci_cache) {
			fci_cache = &fci_cache_local;
		}

		if (!zend_is_callable_ex(fci->function_name, fci->object_ptr, IS_CALLABLE_CHECK_SILENT, &callable_name, NULL, fci_cache, &error TSRMLS_CC)) {
			if (error) {
				zend_error(E_WARNING, "Invalid callback %s, %s", callable_name, error);
				efree(error);
			}
			if (callable_name) {
				efree(callable_name);
			}
			return FAILURE;
		} else {
			if (error) {
				/* Capitalize the first latter of the error message */
				if (error[0] >= 'a' && error[0] <= 'z') {
					error[0] += ('A' - 'a');
				}
				zend_error(E_STRICT, "%s", error);
				efree(error);
			}
		}
		efree(callable_name);
	}

	EX(function_state).function = fci_cache->function_handler;
	calling_scope = fci_cache->calling_scope;
	called_scope = fci_cache->called_scope;
	fci->object_ptr = fci_cache->object_ptr;
	EX(object) = fci->object_ptr;
	if (fci->object_ptr && Z_TYPE_P(fci->object_ptr) == IS_OBJECT && (!EG(objects_store).object_buckets || !EG(objects_store).object_buckets[Z_OBJ_HANDLE_P(fci->object_ptr)].valid)) {
		return FAILURE;
	}

	#ifndef ZEPHIR_RELEASE
	if (EX(function_state).function->common.fn_flags & ZEND_ACC_ABSTRACT) {
		zend_error_noreturn(E_ERROR, "Cannot call abstract method %s::%s()", EX(function_state).function->common.scope->name, EX(function_state).function->common.function_name);
		return FAILURE;
	}
	#endif

	ZEND_VM_STACK_GROW_IF_NEEDED(fci->param_count + 1);

	for (i = 0; i < fci->param_count; i++) {
		zval *param;

		if (EX(function_state).function->type == ZEND_INTERNAL_FUNCTION
			&& (EX(function_state).function->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) == 0
			&& !ARG_SHOULD_BE_SENT_BY_REF(EX(function_state).function, i + 1)
			&& PZVAL_IS_REF(*fci->params[i])) {
			ALLOC_ZVAL(param);
			*param = **(fci->params[i]);
			INIT_PZVAL(param);
			zval_copy_ctor(param);
		} else if (ARG_SHOULD_BE_SENT_BY_REF(EX(function_state).function, i + 1)
			&& !PZVAL_IS_REF(*fci->params[i])) {

			if (Z_REFCOUNT_PP(fci->params[i]) > 1) {
				zval *new_zval;

				if (fci->no_separation &&
					!ARG_MAY_BE_SENT_BY_REF(EX(function_state).function, i + 1)) {
					if (i || UNEXPECTED(ZEND_VM_STACK_ELEMETS(EG(argument_stack)) == EG(argument_stack)->top)) {
						/* hack to clean up the stack */
						zend_vm_stack_push_nocheck((void *) (zend_uintptr_t)i TSRMLS_CC);
						#if PHP_VERSION_ID <= 50500
						zend_vm_stack_clear_multiple(TSRMLS_C);
						#else
						zend_vm_stack_clear_multiple(0 TSRMLS_C);
						#endif
					}

					zend_error(E_WARNING, "Parameter %d to %s%s%s() expected to be a reference, value given",
						i+1,
						EX(function_state).function->common.scope ? EX(function_state).function->common.scope->name : "",
						EX(function_state).function->common.scope ? "::" : "",
						EX(function_state).function->common.function_name);
					return FAILURE;
				}

				ALLOC_ZVAL(new_zval);
				*new_zval = **fci->params[i];
				zval_copy_ctor(new_zval);
				Z_SET_REFCOUNT_P(new_zval, 1);
				Z_DELREF_PP(fci->params[i]);
				*fci->params[i] = new_zval;
			}
			Z_ADDREF_PP(fci->params[i]);
			Z_SET_ISREF_PP(fci->params[i]);
			param = *fci->params[i];
		} else if (*fci->params[i] != &EG(uninitialized_zval)) {
			Z_ADDREF_PP(fci->params[i]);
			param = *fci->params[i];
		} else {
			ALLOC_ZVAL(param);
			*param = **(fci->params[i]);
			INIT_PZVAL(param);
		}
		zend_vm_stack_push_nocheck(param TSRMLS_CC);
	}

	EX(function_state).arguments = zend_vm_stack_top(TSRMLS_C);
	zend_vm_stack_push_nocheck((void*)(zend_uintptr_t)fci->param_count TSRMLS_CC);

	current_scope = EG(scope);
	EG(scope) = calling_scope;

	current_this = EG(This);

	current_called_scope = EG(called_scope);
	if (called_scope) {
		EG(called_scope) = called_scope;
	} else {
		if (EX(function_state).function->type != ZEND_INTERNAL_FUNCTION) {
			EG(called_scope) = NULL;
		}
	}

	if (fci->object_ptr) {
		if ((EX(function_state).function->common.fn_flags & ZEND_ACC_STATIC)) {
			EG(This) = NULL;
		} else {
			EG(This) = fci->object_ptr;

			if (!PZVAL_IS_REF(EG(This))) {
				Z_ADDREF_P(EG(This)); /* For $this pointer */
			} else {
				zval *this_ptr;

				ALLOC_ZVAL(this_ptr);
				*this_ptr = *EG(This);
				INIT_PZVAL(this_ptr);
				zval_copy_ctor(this_ptr);
				EG(This) = this_ptr;
			}
		}
	} else {
		EG(This) = NULL;
	}

	EX(prev_execute_data) = EG(current_execute_data);
	EG(current_execute_data) = &execute_data;

	if (EX(function_state).function->type == ZEND_USER_FUNCTION) {

		calling_symbol_table = EG(active_symbol_table);
		EG(scope) = EX(function_state).function->common.scope;
		if (fci->symbol_table) {
			EG(active_symbol_table) = fci->symbol_table;
		} else {
			EG(active_symbol_table) = NULL;
		}

		original_return_value = EG(return_value_ptr_ptr);
		original_op_array = EG(active_op_array);
		EG(return_value_ptr_ptr) = fci->retval_ptr_ptr;
		EG(active_op_array) = (zend_op_array *) EX(function_state).function;
		original_opline_ptr = EG(opline_ptr);
		zend_execute(EG(active_op_array) TSRMLS_CC);
		if (!fci->symbol_table && EG(active_symbol_table)) {
			if (EG(symtable_cache_ptr)>=EG(symtable_cache_limit)) {
				zend_hash_destroy(EG(active_symbol_table));
				FREE_HASHTABLE(EG(active_symbol_table));
			} else {
				/* clean before putting into the cache, since clean
				   could call dtors, which could use cached hash */
				zend_hash_clean(EG(active_symbol_table));
				*(++EG(symtable_cache_ptr)) = EG(active_symbol_table);
			}
		}
		EG(active_symbol_table) = calling_symbol_table;
		EG(active_op_array) = original_op_array;
		EG(return_value_ptr_ptr)=original_return_value;
		EG(opline_ptr) = original_opline_ptr;
	} else if (EX(function_state).function->type == ZEND_INTERNAL_FUNCTION) {
		int call_via_handler = (EX(function_state).function->common.fn_flags & ZEND_ACC_CALL_VIA_HANDLER) != 0;
		ALLOC_INIT_ZVAL(*fci->retval_ptr_ptr);
		if (EX(function_state).function->common.scope) {
			EG(scope) = EX(function_state).function->common.scope;
		}
		((zend_internal_function *) EX(function_state).function)->handler(fci->param_count, *fci->retval_ptr_ptr, fci->retval_ptr_ptr, fci->object_ptr, 1 TSRMLS_CC);
		/*  We shouldn't fix bad extensions here,
			because it can break proper ones (Bug #34045)
		if (!EX(function_state).function->common.return_reference)
		{
			INIT_PZVAL(*fci->retval_ptr_ptr);
		}*/
		if (EG(exception) && fci->retval_ptr_ptr) {
			zval_ptr_dtor(fci->retval_ptr_ptr);
			*fci->retval_ptr_ptr = NULL;
		}

		if (call_via_handler) {
			/* We must re-initialize function again */
			fci_cache->initialized = 0;
		}
	} else {
		ALLOC_INIT_ZVAL(*fci->retval_ptr_ptr);

		if (fci->object_ptr) {
			Z_OBJ_HT_P(fci->object_ptr)->call_method(EX(function_state).function->common.function_name, fci->param_count, *fci->retval_ptr_ptr, fci->retval_ptr_ptr, fci->object_ptr, 1 TSRMLS_CC);
		} else {
			zend_error_noreturn(E_ERROR, "Cannot call overloaded function for non-object");
			return FAILURE;
		}

		if (EX(function_state).function->type == ZEND_OVERLOADED_FUNCTION_TEMPORARY) {
			efree(EX(function_state).function->common.function_name);
		}
		efree(EX(function_state).function);

		if (EG(exception) && fci->retval_ptr_ptr) {
			zval_ptr_dtor(fci->retval_ptr_ptr);
			*fci->retval_ptr_ptr = NULL;
		}
	}
	#if PHP_VERSION_ID <= 50500
	zend_vm_stack_clear_multiple(TSRMLS_C);
	#else
	zend_vm_stack_clear_multiple(0 TSRMLS_C);
	#endif

	if (EG(This)) {
		zval_ptr_dtor(&EG(This));
	}
	EG(called_scope) = current_called_scope;
	EG(scope) = current_scope;
	EG(This) = current_this;
	EG(current_execute_data) = EX(prev_execute_data);

	if (EG(exception)) {
		zephir_throw_exception_internal(NULL TSRMLS_CC);
	}
	return SUCCESS;
}

#endif

inline void zephir_eval_php(zval *str, zval *retval_ptr, char *context TSRMLS_DC)
{
    zend_eval_string_ex(Z_STRVAL_P(str), retval_ptr, context, 1 TSRMLS_CC);
}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>
#include <ext/standard/php_array.h>
#include <Zend/zend_hash.h>


int zephir_array_isset_fetch(zval **fetched, const zval *arr, zval *index, int readonly TSRMLS_DC) {

	HashTable *h;
	zval **val;
	int result;

	if (Z_TYPE_P(arr) != IS_ARRAY) {
		*fetched = ZEPHIR_GLOBAL(global_null);
		if (!readonly) {
			Z_ADDREF_P(*fetched);
		}
		return 0;
	}

	h = Z_ARRVAL_P(arr);
	switch (Z_TYPE_P(index)) {
		case IS_NULL:
			result = zephir_hash_find(h, SS(""), (void**)&val);
			break;

		case IS_DOUBLE:
			result = zend_hash_index_find(h, (ulong)Z_DVAL_P(index), (void**)&val);
			break;

		case IS_LONG:
		case IS_BOOL:
		case IS_RESOURCE:
			result = zend_hash_index_find(h, Z_LVAL_P(index), (void**)&val);
			break;

		case IS_STRING:
			result = zend_symtable_find(h, (Z_STRLEN_P(index) ? Z_STRVAL_P(index) : ""), Z_STRLEN_P(index)+1, (void**)&val);
			break;

		default:
			zend_error(E_WARNING, "Illegal offset type");
			*fetched = ZEPHIR_GLOBAL(global_null);
			if (!readonly) {
				Z_ADDREF_P(*fetched);
			}
			return 0;
	}

	if (result == SUCCESS) {
		*fetched = *val;
		if (!readonly) {
			Z_ADDREF_P(*fetched);
		}
		return 1;
	}

	*fetched = ZEPHIR_GLOBAL(global_null);
	if (!readonly) {
		Z_ADDREF_P(*fetched);
	}
	return 0;
}

int zephir_array_isset_quick_string_fetch(zval **fetched, zval *arr, char *index, uint index_length, unsigned long key, int readonly TSRMLS_DC) {

	zval **zv;

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		if (zephir_hash_quick_find(Z_ARRVAL_P(arr), index, index_length, key, (void**) &zv) == SUCCESS) {
			*fetched = *zv;
			if (!readonly) {
				Z_ADDREF_P(*fetched);
			}
			return 1;
		}
	}

	*fetched = ZEPHIR_GLOBAL(global_null);
	if (!readonly) {
		Z_ADDREF_P(*fetched);
	}
	return 0;
}

int zephir_array_isset_string_fetch(zval **fetched, zval *arr, char *index, uint index_length, int readonly TSRMLS_DC) {

	return zephir_array_isset_quick_string_fetch(fetched, arr, index, index_length, zend_inline_hash_func(index, index_length), readonly TSRMLS_CC);
}

int zephir_array_isset_long_fetch(zval **fetched, zval *arr, unsigned long index, int readonly TSRMLS_DC) {

	zval **zv;

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		if (zend_hash_index_find(Z_ARRVAL_P(arr), index, (void**)&zv) == SUCCESS) {
			*fetched = *zv;
			if (!readonly) {
				Z_ADDREF_P(*fetched);
			}
			return 1;
		}
	}

	*fetched = ZEPHIR_GLOBAL(global_null);
	if (!readonly) {
		Z_ADDREF_P(*fetched);
	}
	return 0;
}

int ZEPHIR_FASTCALL zephir_array_isset(const zval *arr, zval *index) {

	HashTable *h;

	if (Z_TYPE_P(arr) != IS_ARRAY) {
		return 0;
	}

	h = Z_ARRVAL_P(arr);
	switch (Z_TYPE_P(index)) {
		case IS_NULL:
			return zephir_hash_exists(h, SS(""));

		case IS_DOUBLE:
			return zend_hash_index_exists(h, (ulong)Z_DVAL_P(index));

		case IS_BOOL:
		case IS_LONG:
		case IS_RESOURCE:
			return zend_hash_index_exists(h, Z_LVAL_P(index));

		case IS_STRING:
			return zend_symtable_exists(h, Z_STRVAL_P(index), Z_STRLEN_P(index)+1);

		default:
			zend_error(E_WARNING, "Illegal offset type");
			return 0;
	}
}

int ZEPHIR_FASTCALL zephir_array_isset_string(const zval *arr, const char *index, uint index_length) {

	return zephir_array_isset_quick_string(arr, index, index_length, zend_inline_hash_func(index, index_length));
}

int ZEPHIR_FASTCALL zephir_array_isset_quick_string(const zval *arr, const char *index, uint index_length, unsigned long key) {

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		return zend_hash_quick_exists(Z_ARRVAL_P(arr), index, index_length, key);
	}

	return 0;
}

int ZEPHIR_FASTCALL zephir_array_isset_long(const zval *arr, unsigned long index) {

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		return zend_hash_index_exists(Z_ARRVAL_P(arr), index);
	}

	return 0;
}

int ZEPHIR_FASTCALL zephir_array_unset(zval **arr, zval *index, int flags) {

	HashTable *ht;

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		return FAILURE;
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	ht = Z_ARRVAL_PP(arr);

	switch (Z_TYPE_P(index)) {
		case IS_NULL:
			return (zend_hash_del(ht, "", 1) == SUCCESS);

		case IS_DOUBLE:
			return (zend_hash_index_del(ht, (ulong)Z_DVAL_P(index)) == SUCCESS);

		case IS_LONG:
		case IS_BOOL:
		case IS_RESOURCE:
			return (zend_hash_index_del(ht, Z_LVAL_P(index)) == SUCCESS);

		case IS_STRING:
			return (zend_symtable_del(ht, Z_STRVAL_P(index), Z_STRLEN_P(index)+1) == SUCCESS);

		default:
			zend_error(E_WARNING, "Illegal offset type");
			return 0;
	}
}

int ZEPHIR_FASTCALL zephir_array_unset_string(zval **arr, const char *index, uint index_length, int flags) {

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		return 0;
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	return zend_hash_del(Z_ARRVAL_PP(arr), index, index_length);
}

int ZEPHIR_FASTCALL zephir_array_unset_long(zval **arr, unsigned long index, int flags) {

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		return 0;
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	return zend_hash_index_del(Z_ARRVAL_PP(arr), index);
}

int zephir_array_append(zval **arr, zval *value, int flags ZEPHIR_DEBUG_PARAMS) {

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		zend_error(E_WARNING, "Cannot use a scalar value as an array in %s on line %d", file, line);
		return FAILURE;
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	Z_ADDREF_P(value);
	return add_next_index_zval(*arr, value);
}

int zephir_array_append_long(zval **arr, long value, int separate) {

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	Z_SET_REFCOUNT_P(zvalue, 0);
	ZVAL_LONG(zvalue, value);

	return zephir_array_append(arr, zvalue, separate ZEPHIR_DEBUG_PARAMS_DUMMY);
}

int zephir_array_append_string(zval **arr, char *value, uint value_length, int separate) {

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	Z_SET_REFCOUNT_P(zvalue, 0);
	ZVAL_STRINGL(zvalue, value, value_length, 1);

	return zephir_array_append(arr, zvalue, separate ZEPHIR_DEBUG_PARAMS_DUMMY);
}

int zephir_array_update_zval(zval **arr, zval *index, zval **value, int flags) {

	HashTable *ht;

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		zend_error(E_WARNING, "Cannot use a scalar value as an array");
		return FAILURE;
	}

	if ((flags & PH_CTOR) == PH_CTOR) {
		zval *new_zv;
		Z_DELREF_PP(value);
		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, *value);
		*value = new_zv;
		zval_copy_ctor(new_zv);
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	if ((flags & PH_COPY) == PH_COPY) {
		Z_ADDREF_PP(value);
	}

	ht = Z_ARRVAL_PP(arr);

	switch (Z_TYPE_P(index)) {
		case IS_NULL:
			return zend_symtable_update(ht, "", 1, value, sizeof(zval*), NULL);

		case IS_DOUBLE:
			return zend_hash_index_update(ht, (ulong)Z_DVAL_P(index), value, sizeof(zval*), NULL);

		case IS_LONG:
		case IS_BOOL:
		case IS_RESOURCE:
			return zend_hash_index_update(ht, Z_LVAL_P(index), value, sizeof(zval*), NULL);

		case IS_STRING:
			return zend_symtable_update(ht, Z_STRVAL_P(index), Z_STRLEN_P(index)+1, value, sizeof(zval*), NULL);

		default:
			zend_error(E_WARNING, "Illegal offset type");
			return FAILURE;
	}
}

int zephir_array_update_zval_bool(zval **arr, zval *index, int value, int flags) {

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_BOOL(zvalue, value);

	return zephir_array_update_zval(arr, index, &zvalue, flags);
}

int zephir_array_update_zval_string(zval **arr, zval *index, char *value, uint value_length, int flags) {

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_STRINGL(zvalue, value, value_length, 1);

	return zephir_array_update_zval(arr, index, &zvalue, flags);
}

int zephir_array_update_zval_long(zval **arr, zval *index, long value, int flags) {

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_LONG(zvalue, value);

	return zephir_array_update_zval(arr, index, &zvalue, flags);
}

int zephir_array_update_quick_string(zval **arr, const char *index, uint index_length, unsigned long key, zval **value, int flags){

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		zend_error(E_WARNING, "Cannot use a scalar value as an array");
		return FAILURE;
	}

	if ((flags & PH_CTOR) == PH_CTOR) {
		zval *new_zv;
		Z_DELREF_PP(value);
		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, *value);
		*value = new_zv;
		zval_copy_ctor(new_zv);
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	if ((flags & PH_COPY) == PH_COPY) {
		Z_ADDREF_PP(value);
	}

	return zend_hash_quick_update(Z_ARRVAL_PP(arr), index, index_length, key, value, sizeof(zval *), NULL);
}

int zephir_array_update_string(zval **arr, const char *index, uint index_length, zval **value, int flags) {

	return zephir_array_update_quick_string(arr, index, index_length + 1, zend_inline_hash_func(index, index_length + 1), value, flags);
}

int zephir_array_update_string_bool(zval **arr, const char *index, uint index_length, int value, int flags){

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_BOOL(zvalue, value);

	return zephir_array_update_string(arr, index, index_length, &zvalue, flags);
}

int zephir_array_update_string_long(zval **arr, const char *index, uint index_length, long value, int flags){

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_LONG(zvalue, value);

	return zephir_array_update_string(arr, index, index_length, &zvalue, flags);
}

int zephir_array_update_string_string(zval **arr, const char *index, uint index_length, char *value, uint value_length, int flags){

	zval *zvalue;

	ALLOC_INIT_ZVAL(zvalue);
	ZVAL_STRINGL(zvalue, value, value_length, 1);

	return zephir_array_update_string(arr, index, index_length, &zvalue, flags);
}

int zephir_array_update_long(zval **arr, unsigned long index, zval **value, int flags ZEPHIR_DEBUG_PARAMS){

	if (Z_TYPE_PP(arr) != IS_ARRAY) {
		zend_error(E_WARNING, "Cannot use a scalar value as an array in %s on line %d", file, line);
		return FAILURE;
	}

	if ((flags & PH_CTOR) == PH_CTOR) {
		zval *new_zv;
		Z_DELREF_PP(value);
		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, *value);
		*value = new_zv;
		zval_copy_ctor(new_zv);
	}

	if ((flags & PH_SEPARATE) == PH_SEPARATE) {
		SEPARATE_ZVAL_IF_NOT_REF(arr);
	}

	if ((flags & PH_COPY) == PH_COPY) {
		Z_ADDREF_PP(value);
	}

	return zend_hash_index_update(Z_ARRVAL_PP(arr), index, value, sizeof(zval *), NULL);
}

int zephir_array_fetch(zval **return_value, zval *arr, zval *index, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC){

	zval **zv;
	HashTable *ht;
	int result;
	ulong uidx = 0;
	char *sidx = NULL;

	if (Z_TYPE_P(arr) == IS_ARRAY) {
		ht = Z_ARRVAL_P(arr);
		switch (Z_TYPE_P(index)) {
			case IS_NULL:
				result = zephir_hash_find(ht, SS(""), (void**) &zv);
				sidx   = "";
				break;

			case IS_DOUBLE:
				uidx   = (ulong)Z_DVAL_P(index);
				result = zend_hash_index_find(ht, uidx, (void**) &zv);
				break;

			case IS_LONG:
			case IS_BOOL:
			case IS_RESOURCE:
				uidx   = Z_LVAL_P(index);
				result = zend_hash_index_find(ht, uidx, (void**) &zv);
				break;

			case IS_STRING:
				sidx   = Z_STRLEN_P(index) ? Z_STRVAL_P(index) : "";
				result = zend_symtable_find(ht, Z_STRVAL_P(index), Z_STRLEN_P(index)+1, (void**) &zv);
				break;

			default:
				if ((flags & PH_NOISY) == PH_NOISY) {
					zend_error(E_WARNING, "Illegal offset type in %s on line %d", file, line);
				}
				result = FAILURE;
				break;
		}

		if (result != FAILURE) {
			*return_value = *zv;
			if ((flags & PH_READONLY) != PH_READONLY) {
				Z_ADDREF_PP(return_value);
			}
			return SUCCESS;
		}

		if ((flags & PH_NOISY) == PH_NOISY) {
			if (sidx == NULL) {
				zend_error(E_NOTICE, "Undefined index: %ld in %s on line %d", uidx, file, line);
			} else {
				zend_error(E_NOTICE, "Undefined index: %s in %s on line %d", sidx, file, line);
			}
		}
	}

	*return_value = ZEPHIR_GLOBAL(global_null);
	if ((flags & PH_READONLY) != PH_READONLY) {
		Z_ADDREF_PP(return_value);
	}
	return FAILURE;
}

int zephir_array_fetch_quick_string(zval **return_value, zval *arr, const char *index, uint index_length, unsigned long key, int flags TSRMLS_DC){

	zval **zv;

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		if (zephir_hash_quick_find(Z_ARRVAL_P(arr), index, index_length, key, (void**) &zv) == SUCCESS) {
			*return_value = *zv;
			if ((flags & PH_READONLY) != PH_READONLY) {
				Z_ADDREF_PP(return_value);
			}
			return SUCCESS;
		}
		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Undefined index: %s", index);
		}
	} else {
		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Cannot use a scalar value as an array");
		}
	}

	*return_value = ZEPHIR_GLOBAL(global_null);
	if ((flags & PH_READONLY) != PH_READONLY) {
		Z_ADDREF_PP(return_value);
	}
	return FAILURE;
}

int zephir_array_fetch_string(zval **return_value, zval *arr, const char *index, uint index_length, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC){

	return zephir_array_fetch_quick_string(return_value, arr, index, index_length + 1, zend_inline_hash_func(index, index_length + 1), flags TSRMLS_CC);
}

int zephir_array_fetch_long(zval **return_value, zval *arr, unsigned long index, int flags ZEPHIR_DEBUG_PARAMS TSRMLS_DC){

	zval **zv;

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		if (zend_hash_index_find(Z_ARRVAL_P(arr), index, (void**)&zv) == SUCCESS) {
			*return_value = *zv;
			if ((flags & PH_READONLY) != PH_READONLY) {
				Z_ADDREF_PP(return_value);
			}
			return SUCCESS;
		}

		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Undefined index: %lu in %s on line %d", index, file, line);
		}
	}
	else {
		if ((flags & PH_NOISY) == PH_NOISY) {
			zend_error(E_NOTICE, "Cannot use a scalar value as an array in %s on line %d", file, line);
		}
	}

	*return_value = ZEPHIR_GLOBAL(global_null);
	if ((flags & PH_READONLY) != PH_READONLY) {
		Z_ADDREF_PP(return_value);
	}
	return FAILURE;
}

void zephir_merge_append(zval *left, zval *values){

	zval         **tmp;
	HashTable      *arr_values;
	HashPosition   pos;

	if (Z_TYPE_P(left) != IS_ARRAY) {
		zend_error(E_NOTICE, "First parameter of zephir_merge_append must be an array");
		return;
	}

	if (Z_TYPE_P(values) == IS_ARRAY) {

		arr_values = Z_ARRVAL_P(values);
		zend_hash_internal_pointer_reset_ex(arr_values, &pos);

		while (zend_hash_get_current_data_ex(arr_values, (void **) &tmp, &pos) == SUCCESS) {

			Z_ADDREF_PP(tmp);
			add_next_index_zval(left, *tmp);

			zend_hash_move_forward_ex(arr_values, &pos);
		}

	} else {
		Z_ADDREF_P(values);
		add_next_index_zval(left, values);
	}
}

void zephir_array_get_current(zval *return_value, zval *array){

	zval **entry;

	if (Z_TYPE_P(array) == IS_ARRAY) {
		if (zend_hash_get_current_data(Z_ARRVAL_P(array), (void **) &entry) == FAILURE) {
			RETURN_FALSE;
		}
		RETURN_ZVAL(*entry, 1, 0);
	}

	RETURN_FALSE;
}

void zephir_array_next(zval *array){
	if (Z_TYPE_P(array) == IS_ARRAY) {
		zend_hash_move_forward(Z_ARRVAL_P(array));
	}
}

int zephir_fast_in_array(zval *needle, zval *haystack TSRMLS_DC) {

	zval         **tmp;
	HashTable      *arr;
	HashPosition   pos;
	unsigned int   numelems;

	if (Z_TYPE_P(haystack) != IS_ARRAY) {
		return 0;
	}

	arr = Z_ARRVAL_P(haystack);
	numelems = zend_hash_num_elements(arr);

	if (numelems == 0) {
		return 0;
	}

	zend_hash_internal_pointer_reset_ex(arr, &pos);

	while (zend_hash_get_current_data_ex(arr, (void **) &tmp, &pos) == SUCCESS) {
		if (ZEPHIR_IS_EQUAL(needle, *tmp)) {
			return 1;
		}
		zend_hash_move_forward_ex(arr, &pos);
	}

	return 0;
}

void zephir_fast_array_merge(zval *return_value, zval **array1, zval **array2 TSRMLS_DC) {

	int init_size, num;

	if (Z_TYPE_PP(array1) != IS_ARRAY) {
		zend_error(E_WARNING, "First argument is not an array");
		RETURN_NULL();
	}

	if (Z_TYPE_PP(array2) != IS_ARRAY) {
		zend_error(E_WARNING, "Second argument is not an array");
		RETURN_NULL();
	}

	init_size = zend_hash_num_elements(Z_ARRVAL_PP(array1));
	num = zend_hash_num_elements(Z_ARRVAL_PP(array2));
	if (num > init_size) {
		init_size = num;
	}

	array_init_size(return_value, init_size);

	php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL_PP(array1), 0 TSRMLS_CC);

	php_array_merge(Z_ARRVAL_P(return_value), Z_ARRVAL_PP(array2), 0 TSRMLS_CC);
}

void zephir_array_merge_recursive_n(zval **a1, zval *a2 TSRMLS_DC)
{
	HashPosition hp;
	zval **value, key, *tmp1, *tmp2;

	assert(Z_TYPE_PP(a1) == IS_ARRAY);
	assert(Z_TYPE_P(a2)  == IS_ARRAY);

	for (
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(a2), &hp);
		zend_hash_get_current_data_ex(Z_ARRVAL_P(a2), (void**) &value, &hp) == SUCCESS;
		zend_hash_move_forward_ex(Z_ARRVAL_P(a2), &hp)
	) {
		key = zephir_get_current_key_w(Z_ARRVAL_P(a2), &hp);

		if (!zephir_array_isset(*a1, &key) || Z_TYPE_PP(value) != IS_ARRAY) {
			zephir_array_update_zval(a1, &key, value, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_fetch(&tmp1, *a1, &key, PH_NOISY ZEPHIR_DEBUG_PARAMS_DUMMY TSRMLS_CC);
			zephir_array_fetch(&tmp2, a2, &key, PH_NOISY ZEPHIR_DEBUG_PARAMS_DUMMY TSRMLS_CC);
			zephir_array_merge_recursive_n(&tmp1, tmp2 TSRMLS_CC);
			zval_ptr_dtor(&tmp1);
			zval_ptr_dtor(&tmp2);
		}
	}
}

void zephir_array_unshift(zval *arr, zval *arg TSRMLS_DC)
{
	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		zval** args[1]      = { &arg };

		HashTable *newhash = Z_ARRVAL_P(arr);

		#if PHP_VERSION_ID < 50600
			newhash = php_splice(newhash, 0, 0, args, 1, NULL);
		#else
			php_splice(newhash, 0, 0, args, 1, NULL TSRMLS_CC);
		#endif

		HashTable  oldhash = *Z_ARRVAL_P(arr);
		*Z_ARRVAL_P(arr)   = *newhash;

		FREE_HASHTABLE(newhash);
		zend_hash_destroy(&oldhash);
	}
}

void zephir_array_keys(zval *return_value, zval *input TSRMLS_DC)
{

	zval *new_val, **entry;
	char  *string_key;
	uint   string_key_len;
	ulong  num_key;
	HashPosition pos;

	if (likely(Z_TYPE_P(input) == IS_ARRAY)) {

		array_init_size(return_value, zend_hash_num_elements(Z_ARRVAL_P(input)));

		/* Go through input array and add keys to the return array */
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&entry, &pos) == SUCCESS) {

			MAKE_STD_ZVAL(new_val);

			switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(input), &string_key, &string_key_len, &num_key, 1, &pos)) {
				case HASH_KEY_IS_STRING:
					ZVAL_STRINGL(new_val, string_key, string_key_len - 1, 0);
					zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &new_val, sizeof(zval *), NULL);
					break;

				case HASH_KEY_IS_LONG:
					Z_TYPE_P(new_val) = IS_LONG;
					Z_LVAL_P(new_val) = num_key;
					zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &new_val, sizeof(zval *), NULL);
					break;
			}

			zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos);
		}
	}
}

void zephir_array_values(zval *return_value, zval *arr)
{
	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		zval **entry;
		HashPosition pos;

		array_init_size(return_value, zend_hash_num_elements(Z_ARRVAL_P(arr)));
		for (
			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(arr), &pos);
			zend_hash_get_current_data_ex(Z_ARRVAL_P(arr), (void **)&entry, &pos) == SUCCESS;
			zend_hash_move_forward_ex(Z_ARRVAL_P(arr), &pos)
		) {
			Z_ADDREF_PP(entry);
			zend_hash_next_index_insert(Z_ARRVAL_P(return_value), entry, sizeof(zval*), NULL);
		}
	}
}

int zephir_array_key_exists(zval *arr, zval *key TSRMLS_DC)
{
	HashTable *h = HASH_OF(arr);
	if (h) {
		switch (Z_TYPE_P(key)) {
			case IS_STRING:
				return zend_symtable_exists(h, Z_STRVAL_P(key), Z_STRLEN_P(key) + 1);

			case IS_LONG:
				return zend_hash_index_exists(h, Z_LVAL_P(key));

			case IS_NULL:
				return zend_hash_exists(h, "", 1);

			default:
				zend_error(E_WARNING, "The key should be either a string or an integer");
				return 0;
		}
	}

	return 0;
}

int zephir_array_is_associative(zval *arr) {

	if (likely(Z_TYPE_P(arr) == IS_ARRAY)) {
		HashPosition pos;
		zval **entry;
		char *skey;
		uint skey_len;
		ulong nkey;
		ulong expected = 0;

		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(arr), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(arr), (void**) &entry, &pos) == SUCCESS) {

			if (HASH_KEY_IS_LONG == zend_hash_get_current_key_ex(Z_ARRVAL_P(arr), &skey, &skey_len, &nkey, 1, &pos)) {
				if (expected != nkey) {
					return 1;
				}
			} else {
				return 1;
			}

			++expected;
			zend_hash_move_forward_ex(Z_ARRVAL_P(arr), &pos);
		}
	}

	return 0;
}

int zephir_array_update_multi(zval **arr, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...) {

	int i, l, ll; char *s;
	va_list ap;
	zval *fetched, *tmp, *p, *item;

	va_start(ap, types_count);

	p = *arr;
	for (i = 0; i < types_length; ++i) {

		switch (types[i]) {

			case 's':
				s = va_arg(ap, char*);
				l = va_arg(ap, int);
				if (zephir_array_isset_string_fetch(&fetched, p, s, l + 1, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_string(&fetched, s, l, value, PH_COPY | PH_SEPARATE);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_string(&p, s, l, value, PH_COPY | PH_SEPARATE);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_string(&p, s, l, &tmp, PH_SEPARATE);
					p = tmp;
				}
				break;

			case 'l':
				ll = va_arg(ap, long);
				if (zephir_array_isset_long_fetch(&fetched, p, ll, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_long(&fetched, ll, value, PH_COPY | PH_SEPARATE, "", 0);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_long(&p, ll, value, PH_COPY | PH_SEPARATE, "", 0);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_long(&p, ll, &tmp, PH_SEPARATE, "", 0);
					p = tmp;
				}
				break;

			case 'z':
				item = va_arg(ap, zval*);
				if (zephir_array_isset_fetch(&fetched, p, item, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_zval(&fetched, item, value, PH_COPY | PH_SEPARATE);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_zval(&p, item, value, PH_COPY | PH_SEPARATE);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_zval(&p, item, &tmp, PH_SEPARATE);
					p = tmp;
				}
				break;

			case 'a':
				zephir_array_append(&p, *value, PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				break;
		}
	}

	va_end(ap);

	return 0;
}




#ifdef HAVE_CONFIG_H
#endif



int zephir_hash_exists(const HashTable *ht, const char *arKey, uint nKeyLength)
{
	ulong h;
	uint nIndex;
	Bucket *p;

	h = zend_inline_hash_func(arKey, nKeyLength);
	nIndex = h & ht->nTableMask;

	p = ht->arBuckets[nIndex];
	while (p != NULL) {
		if (p->arKey == arKey || ((p->h == h) && (p->nKeyLength == nKeyLength))) {
			if (!memcmp(p->arKey, arKey, nKeyLength)) {
				return 1;
			}
		}
		p = p->pNext;
	}
	return 0;
}

int zephir_hash_quick_exists(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h)
{
	uint nIndex;
	Bucket *p;

	if (nKeyLength == 0) {
		return zend_hash_index_exists(ht, h);
	}

	nIndex = h & ht->nTableMask;

	p = ht->arBuckets[nIndex];
	while (p != NULL) {
		if (p->arKey == arKey || ((p->h == h) && (p->nKeyLength == nKeyLength))) {
			if (!memcmp(p->arKey, arKey, nKeyLength)) {
				return 1;
			}
		}
		p = p->pNext;
	}
	return 0;
}

int zephir_hash_find(const HashTable *ht, const char *arKey, uint nKeyLength, void **pData)
{
	ulong h;
	uint nIndex;
	Bucket *p;

	h = zend_inline_hash_func(arKey, nKeyLength);
	nIndex = h & ht->nTableMask;

	p = ht->arBuckets[nIndex];
	while (p != NULL) {
		if (p->arKey == arKey || ((p->h == h) && (p->nKeyLength == nKeyLength))) {
			if (!memcmp(p->arKey, arKey, nKeyLength)) {
				*pData = p->pData;
				return SUCCESS;
			}
		}
		p = p->pNext;
	}
	return FAILURE;
}

int zephir_hash_quick_find(const HashTable *ht, const char *arKey, uint nKeyLength, ulong h, void **pData)
{
	uint nIndex;
	Bucket *p;

	if (nKeyLength == 0) {
		return zend_hash_index_find(ht, h, pData);
	}

	nIndex = h & ht->nTableMask;

	p = ht->arBuckets[nIndex];
	while (p != NULL) {
		if (p->arKey == arKey || ((p->h == h) && (p->nKeyLength == nKeyLength))) {
			if (!memcmp(p->arKey, arKey, nKeyLength)) {
				*pData = p->pData;
				return SUCCESS;
			}
		}
		p = p->pNext;
	}
	return FAILURE;
}

void zephir_get_current_key(zval **key, const HashTable *hash_table, HashPosition *hash_position TSRMLS_DC)
{
	Bucket *p;

	ZEPHIR_INIT_NVAR_PNULL(*key);

	p = hash_position ? (*hash_position) : hash_table->pInternalPointer;

	if (p) {
		if (p->nKeyLength) {
			ZVAL_STRINGL(*key, (char *) p->arKey, p->nKeyLength - 1, 0);
		} else {
			ZVAL_LONG(*key, p->h);
		}
	}

}

zval zephir_get_current_key_w(const HashTable *hash_table, HashPosition *hash_position)
{
	Bucket *p;
	zval result;

	INIT_ZVAL(result);
	p = hash_position ? (*hash_position) : hash_table->pInternalPointer;

	if (p) {
		if (p->nKeyLength) {
			ZVAL_STRINGL(&result, (char *) p->arKey, p->nKeyLength - 1, 0);
		} else {
			ZVAL_LONG(&result, p->h);
		}
	}

	return result;
}

int zephir_has_numeric_keys(const zval *data)
{
	HashTable *ht;

	if (Z_TYPE_P(data) == IS_ARRAY) {

		ht = Z_ARRVAL_P(data);

		ht->pInternalPointer = ht->pListHead;
		while (ht->pInternalPointer) {
			if (!ht->pInternalPointer->nKeyLength) {
				return 1;
			}
			ht->pInternalPointer = ht->pInternalPointer->pListNext;
		}

	}

	return 0;
}

void zephir_hash_update_or_insert(HashTable *ht, zval *key, zval *value)
{
	if (!key || Z_TYPE_P(key) == IS_NULL) {
		zend_hash_next_index_insert(ht, (void**)&value, sizeof(zval*), NULL);
		return;
	}

	switch (Z_TYPE_P(key)) {
		case IS_STRING:
			zend_symtable_update(ht, Z_STRVAL_P(key), Z_STRLEN_P(key)+1, (void**)&value, sizeof(zval*), NULL);
			return;

		case IS_RESOURCE:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_LONG:
			zend_hash_index_update(ht, ((Z_TYPE_P(key) == IS_DOUBLE) ? (ulong)Z_DVAL_P(key) : Z_LVAL_P(key)), (void*)&value, sizeof(zval*), NULL);
			return;

		default:
			zend_error(E_WARNING, "Illegal offset type");
			return;
	}
}

zval** zephir_hash_get(HashTable *ht, zval *key, int type)
{
	zval **ret = NULL;

	switch (Z_TYPE_P(key)) {
		case IS_RESOURCE:
			zend_error(E_STRICT, "Resource ID#%ld used as offset, casting to integer (%ld)", Z_LVAL_P(key), Z_LVAL_P(key));
			/* no break */
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL: {
			ulong index = (Z_TYPE_P(key) == IS_DOUBLE) ? ((long int)Z_DVAL_P(key)) : Z_LVAL_P(key);
			if (FAILURE == zend_hash_index_find(ht, index, (void**)&ret)) {
				switch (type) {
					case BP_VAR_R:
						zend_error(E_NOTICE, "Undefined offset: %ld", index);
						/* no break */
					case BP_VAR_UNSET:
					case BP_VAR_IS: {
						TSRMLS_FETCH();
						ret = &EG(uninitialized_zval_ptr);
						break;
					}

					case BP_VAR_RW:
						zend_error(E_NOTICE, "Undefined offset: %ld", index);
						/* no break */
					case BP_VAR_W: {
						zval *value;
						ALLOC_INIT_ZVAL(value);
						zend_hash_index_update(ht, index, (void**)&value, sizeof(void*), (void**)&ret);
						break;
					}
				}
			}

			return ret;
		}

		case IS_STRING:
			if (FAILURE == zend_symtable_find(ht, Z_STRVAL_P(key), Z_STRLEN_P(key)+1, (void**)&ret)) {
				switch (type) {
					case BP_VAR_R:
						zend_error(E_NOTICE, "Undefined offset: %s", Z_STRVAL_P(key));
						/* no break */
					case BP_VAR_UNSET:
					case BP_VAR_IS: {
						TSRMLS_FETCH();
						ret = &EG(uninitialized_zval_ptr);
						break;
					}

					case BP_VAR_RW:
						zend_error(E_NOTICE, "Undefined offset: %s", Z_STRVAL_P(key));
						/* no break */
					case BP_VAR_W: {
						zval *value;
						ALLOC_INIT_ZVAL(value);
						zend_symtable_update(ht, Z_STRVAL_P(key), Z_STRLEN_P(key)+1, (void**)&value, sizeof(void*), (void**)&ret);
						break;
					}
				}
			}

			return ret;

		default: {
			TSRMLS_FETCH();
			zend_error(E_WARNING, "Illegal offset type");
			return (type == BP_VAR_W || type == BP_VAR_RW) ? &EG(error_zval_ptr) : &EG(uninitialized_zval_ptr);
		}
	}
}

int zephir_hash_unset(HashTable *ht, zval *key)
{
	switch (Z_TYPE_P(key)) {
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_RESOURCE:
			return (zend_hash_index_del(ht, (Z_TYPE_P(key) == IS_DOUBLE) ? ((long int)Z_DVAL_P(key)) : Z_LVAL_P(key)) == SUCCESS);

		case IS_STRING:
			return (zend_symtable_del(ht, Z_STRVAL_P(key), Z_STRLEN_P(key) + 1) == SUCCESS);

		default:
			zend_error(E_WARNING, "Illegal offset type");
			return 0;
	}
}






#ifdef HAVE_CONFIG_H
#endif


#ifdef PHP_WIN32
#endif



int zephir_get_class_constant(zval *return_value, zend_class_entry *ce, char *constant_name, unsigned int constant_length TSRMLS_DC) {

	zval **result_ptr;

	if (zephir_hash_find(&ce->constants_table, constant_name, constant_length, (void **) &result_ptr) != SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Undefined class constant '%s::%s'", ce->name, constant_name);
		return FAILURE;
	}

	ZVAL_ZVAL(return_value, *result_ptr, 1, 0);
	return SUCCESS;
}

int zephir_instance_of(zval *result, const zval *object, const zend_class_entry *ce TSRMLS_DC) {

	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "instanceof expects an object instance");
		ZVAL_FALSE(result);
		return FAILURE;
	}

	ZVAL_BOOL(result, instanceof_function(Z_OBJCE_P(object), ce TSRMLS_CC));
	return SUCCESS;
}

int zephir_instance_of_ev(const zval *object, const zend_class_entry *ce TSRMLS_DC) {

	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "instanceof expects an object instance");
		return 0;
	}

	return instanceof_function(Z_OBJCE_P(object), ce TSRMLS_CC);
}

int zephir_is_instance_of(zval *object, const char *class_name, unsigned int class_length TSRMLS_DC) {

	zend_class_entry *ce, *temp_ce;

	if (Z_TYPE_P(object) == IS_OBJECT) {

		ce = Z_OBJCE_P(object);
		if (ce->name_length == class_length) {
			return !zend_binary_strcasecmp(ce->name, ce->name_length, class_name, class_length);
		}

		temp_ce = zend_fetch_class(class_name, class_length, ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
		if (temp_ce) {
			return instanceof_function(ce, temp_ce TSRMLS_CC);
		}
	}

	return 0;
}

void zephir_get_class(zval *result, zval *object, int lower TSRMLS_DC) {

	zend_class_entry *ce;

	if (Z_TYPE_P(object) == IS_OBJECT) {

		ce = Z_OBJCE_P(object);
		Z_STRLEN_P(result) = ce->name_length;
		Z_STRVAL_P(result) = (char *) emalloc(ce->name_length + 1);
		memcpy(Z_STRVAL_P(result), ce->name, ce->name_length);
		Z_STRVAL_P(result)[Z_STRLEN_P(result)] = 0;
		Z_TYPE_P(result) = IS_STRING;

		if (lower) {
			zend_str_tolower(Z_STRVAL_P(result), Z_STRLEN_P(result));
		}

	} else {
		ZVAL_NULL(result);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "zephir_get_class expects an object");
	}
}

void zephir_get_class_ns(zval *result, zval *object, int lower TSRMLS_DC) {

	int found = 0;
	zend_class_entry *ce;
	unsigned int i, class_length;
	const char *cursor, *class_name;

	if (Z_TYPE_P(object) != IS_OBJECT) {
		if (Z_TYPE_P(object) != IS_STRING) {
			ZVAL_NULL(result);
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "zephir_get_class_ns expects an object");
			return;
		}
	}

	if (Z_TYPE_P(object) == IS_OBJECT) {
		ce = Z_OBJCE_P(object);
		class_name = ce->name;
		class_length = ce->name_length;
	} else {
		class_name = Z_STRVAL_P(object);
		class_length = Z_STRLEN_P(object);
	}

	if (!class_length) {
		ZVAL_NULL(result);
		return;
	}

	i = class_length;
	cursor = (char *) (class_name + class_length - 1);

	while (i > 0) {
		if ((*cursor) == '\\') {
			found = 1;
			break;
		}
		cursor--;
		i--;
	}

	if (found) {
		Z_STRLEN_P(result) = class_length - i;
		Z_STRVAL_P(result) = (char *) emalloc(class_length - i + 1);
		memcpy(Z_STRVAL_P(result), class_name + i, class_length - i);
		Z_STRVAL_P(result)[Z_STRLEN_P(result)] = 0;
		Z_TYPE_P(result) = IS_STRING;
	} else {
		ZVAL_STRINGL(result, class_name, class_length, 1);
	}

	if (lower) {
		zend_str_tolower(Z_STRVAL_P(result), Z_STRLEN_P(result));
	}

}

void zephir_get_ns_class(zval *result, zval *object, int lower TSRMLS_DC) {

	zend_class_entry *ce;
	int found = 0;
	unsigned int i, j, class_length;
	const char *cursor, *class_name;

	if (Z_TYPE_P(object) != IS_OBJECT) {
		if (Z_TYPE_P(object) != IS_STRING) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "zephir_get_ns_class expects an object");
			ZVAL_NULL(result);
			return;
		}
	}

	if (Z_TYPE_P(object) == IS_OBJECT) {
		ce = Z_OBJCE_P(object);
		class_name = ce->name;
		class_length = ce->name_length;
	} else {
		class_name = Z_STRVAL_P(object);
		class_length = Z_STRLEN_P(object);
	}

	if (!class_length) {
		ZVAL_NULL(result);
		return;
	}

	j = 0;
	i = class_length;
	cursor = (char *) (class_name + class_length - 1);

	while (i > 0) {
		if ((*cursor) == '\\') {
			found = 1;
			break;
		}
		cursor--;
		i--;
		j++;
	}

	if (j > 0) {

		if (found) {
			Z_STRLEN_P(result) = class_length - j - 1;
			Z_STRVAL_P(result) = (char *) emalloc(class_length - j);
			memcpy(Z_STRVAL_P(result), class_name, class_length - j - 1);
			Z_STRVAL_P(result)[Z_STRLEN_P(result)] = 0;
			Z_TYPE_P(result) = IS_STRING;
		} else {
			ZVAL_EMPTY_STRING(result);
		}

		if (lower) {
			zend_str_tolower(Z_STRVAL_P(result), Z_STRLEN_P(result));
		}
	} else {
		ZVAL_NULL(result);
	}

}

void zephir_get_called_class(zval *return_value TSRMLS_DC) {

	if (EG(called_scope)) {
		RETURN_STRINGL(EG(called_scope)->name, EG(called_scope)->name_length, 1);
	}

	if (!EG(scope))  {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "zephir_get_called_class() called from outside a class");
	}
}

zend_class_entry *zephir_fetch_class(const zval *class_name TSRMLS_DC) {

	if (Z_TYPE_P(class_name) == IS_STRING) {
		return zend_fetch_class(Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
	}

	php_error_docref(NULL TSRMLS_CC, E_WARNING, "class name must be a string");
	return zend_fetch_class("stdclass", strlen("stdclass"), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
}

zend_class_entry* zephir_fetch_self_class(TSRMLS_D) {
	return zend_fetch_class(NULL, 0, ZEND_FETCH_CLASS_SELF TSRMLS_CC);
}

zend_class_entry* zephir_fetch_parent_class(TSRMLS_D) {
	return zend_fetch_class(NULL, 0, ZEND_FETCH_CLASS_PARENT TSRMLS_CC);
}

zend_class_entry* zephir_fetch_static_class(TSRMLS_D) {
	return zend_fetch_class(NULL, 0, ZEND_FETCH_CLASS_STATIC TSRMLS_CC);
}

int zephir_class_exists(const zval *class_name, int autoload TSRMLS_DC) {

	zend_class_entry **ce;

	if (Z_TYPE_P(class_name) == IS_STRING) {
		if (zend_lookup_class(Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), &ce TSRMLS_CC) == SUCCESS) {
#if PHP_VERSION_ID < 50400
			return (((*ce)->ce_flags & ZEND_ACC_INTERFACE) == 0);
#else
			return ((*ce)->ce_flags & (ZEND_ACC_INTERFACE | (ZEND_ACC_TRAIT - ZEND_ACC_EXPLICIT_ABSTRACT_CLASS))) == 0;
#endif
		}
		return 0;
	}

	php_error_docref(NULL TSRMLS_CC, E_WARNING, "class name must be a string");
	return 0;
}

int zephir_interface_exists(const zval *class_name, int autoload TSRMLS_DC) {

	zend_class_entry **ce;

	if (Z_TYPE_P(class_name) == IS_STRING) {
		if (zend_lookup_class(Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), &ce TSRMLS_CC) == SUCCESS) {
			return (((*ce)->ce_flags & ZEND_ACC_INTERFACE) > 0);
		}
		return 0;
	}

	php_error_docref(NULL TSRMLS_CC, E_WARNING, "interface name must be a string");
	return 0;
}

int zephir_clone(zval *destination, zval *obj TSRMLS_DC) {

	int status = SUCCESS;
	zend_class_entry *ce;
	zend_object_clone_obj_t clone_call;

	if (Z_TYPE_P(obj) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "__clone method called on non-object");
		status = FAILURE;
	} else {
		ce = Z_OBJCE_P(obj);
		clone_call =  Z_OBJ_HT_P(obj)->clone_obj;
		if (!clone_call) {
			if (ce) {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Trying to clone an uncloneable object of class %s", ce->name);
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Trying to clone an uncloneable object");
			}
			status = FAILURE;
		} else {
			if (!EG(exception)) {
				Z_OBJVAL_P(destination) = clone_call(obj TSRMLS_CC);
				Z_TYPE_P(destination) = IS_OBJECT;
				Z_SET_REFCOUNT_P(destination, 1);
				Z_UNSET_ISREF_P(destination);
				if (EG(exception)) {
					zval_ptr_dtor(&destination);
				}
			}
		}
	}

	return status;
}

int zephir_isset_property_quick(zval *object, const char *property_name, unsigned int property_length, unsigned long hash TSRMLS_DC) {

	if (Z_TYPE_P(object) == IS_OBJECT) {
		if (likely(zephir_hash_quick_exists(&Z_OBJCE_P(object)->properties_info, property_name, property_length, hash))) {
			return 1;
		} else {
			return zephir_hash_quick_exists(Z_OBJ_HT_P(object)->get_properties(object TSRMLS_CC), property_name, property_length, hash);
		}
	}

	return 0;
}

int zephir_isset_property(zval *object, const char *property_name, unsigned int property_length TSRMLS_DC) {

	return zephir_isset_property_quick(object, property_name, property_length, zend_inline_hash_func(property_name, property_length) TSRMLS_CC);
}

int zephir_isset_property_zval(zval *object, const zval *property TSRMLS_DC) {

	unsigned long hash;

	if (Z_TYPE_P(object) == IS_OBJECT) {
		if (Z_TYPE_P(property) == IS_STRING) {

			hash = zend_inline_hash_func(Z_STRVAL_P(property), Z_STRLEN_P(property) + 1);

			if (likely(zephir_hash_quick_exists(&Z_OBJCE_P(object)->properties_info, Z_STRVAL_P(property), Z_STRLEN_P(property) + 1, hash))) {
				return 1;
			} else {
				return zephir_hash_quick_exists(Z_OBJ_HT_P(object)->get_properties(object TSRMLS_CC), Z_STRVAL_P(property), Z_STRLEN_P(property) + 1, hash);
			}
		}
	}

	return 0;
}

static inline zend_class_entry *zephir_lookup_class_ce_quick(zend_class_entry *ce, const char *property_name, zend_uint property_length, ulong hash TSRMLS_DC) {

	zend_class_entry *original_ce = ce;

	while (ce) {
		if (zephir_hash_quick_exists(&ce->properties_info, property_name, property_length + 1, hash)) {
			return ce;
		}
		ce = ce->parent;
	}
	return original_ce;
}

static inline zend_class_entry *zephir_lookup_class_ce(zend_class_entry *ce, const char *property_name, unsigned int property_length TSRMLS_DC) {

	return zephir_lookup_class_ce_quick(ce, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1) TSRMLS_CC);
}

int zephir_read_property(zval **result, zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC) {

	zval *property;
	zend_class_entry *ce, *old_scope;

	if (Z_TYPE_P(object) != IS_OBJECT) {

		if (silent == PH_NOISY) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Trying to get property of non-object");
		}

		ALLOC_INIT_ZVAL(*result);
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length TSRMLS_CC);
	}

	old_scope = EG(scope);
	EG(scope) = ce;

	if (!Z_OBJ_HT_P(object)->read_property) {
#if PHP_VERSION_ID < 50400
		char *class_name;
#else
		const char *class_name;
#endif
		zend_uint class_name_len;

		zend_get_object_classname(object, &class_name, &class_name_len TSRMLS_CC);
		zend_error(E_CORE_ERROR, "Property %s of class %s cannot be read", property_name, class_name);
	}

	MAKE_STD_ZVAL(property);
	ZVAL_STRINGL(property, property_name, property_length, 0);

#if PHP_VERSION_ID < 50400
	*result = Z_OBJ_HT_P(object)->read_property(object, property, silent ? BP_VAR_IS : BP_VAR_R TSRMLS_CC);
#else
	*result = Z_OBJ_HT_P(object)->read_property(object, property, silent ? BP_VAR_IS : BP_VAR_R, 0 TSRMLS_CC);
#endif

	Z_ADDREF_PP(result);

	if (Z_REFCOUNT_P(property) > 1) {
		ZVAL_STRINGL(property, property_name, property_length, 1);
	} else {
		ZVAL_NULL(property);
	}

	zval_ptr_dtor(&property);

	EG(scope) = old_scope;
	return SUCCESS;
}

zval* zephir_fetch_property_this_quick(zval *object, const char *property_name, zend_uint property_length, ulong key, int silent TSRMLS_DC) {

	zval **zv = NULL;
	zend_object *zobj;
	zend_property_info *property_info;
	zend_class_entry *ce, *old_scope;

	if (likely(Z_TYPE_P(object) == IS_OBJECT)) {

		ce = Z_OBJCE_P(object);
		if (ce->parent) {
			ce = zephir_lookup_class_ce_quick(ce, property_name, property_length, key TSRMLS_CC);
		}

		old_scope = EG(scope);
		EG(scope) = ce;

		zobj = zend_objects_get_address(object TSRMLS_CC);

		if (zephir_hash_quick_find(&ce->properties_info, property_name, property_length + 1, key, (void **) &property_info) == SUCCESS) {

			#if PHP_VERSION_ID < 50400

			if (zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length + 1, property_info->h, (void **) &zv) == SUCCESS) {
				EG(scope) = old_scope;
				return *zv;
			}

			#else

			int flag;
			if (EXPECTED((property_info->flags & ZEND_ACC_STATIC) == 0) && property_info->offset >= 0) {
				if (zobj->properties) {
					zv   = (zval**) zobj->properties_table[property_info->offset];
					flag = (zv == NULL) ? 1 : 0;
				} else {
					zv   = &zobj->properties_table[property_info->offset];
					flag = (*zv == NULL) ? 1 : 0;
				}
			} else if (UNEXPECTED(!zobj->properties)) {
				flag = 1;
			} else if (UNEXPECTED(zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length+1, property_info->h, (void **) &zv) == FAILURE)) {
				flag = 2;
			} else {
				flag = 0;
			}

			if (unlikely(flag) && zobj->properties) {
				if (
					(flag == 2 || zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length+1, property_info->h, (void **) &zv) == FAILURE)
					&& zv && *zv
				) {
					flag = 0;
				}
			}

			if (likely(!flag)) {
				EG(scope) = old_scope;
				return *zv;
			}

			#endif

		}

		EG(scope) = old_scope;

	} else {
		if (silent == PH_NOISY) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Trying to get property of non-object");
		}
	}

	return NULL;
}

int zephir_return_property_quick(zval *return_value, zval **return_value_ptr, zval *object, char *property_name, unsigned int property_length, unsigned long key TSRMLS_DC) {

	zval **zv;
	zend_object *zobj;
	zend_property_info *property_info;
	zend_class_entry *ce, *old_scope;

	if (likely(Z_TYPE_P(object) == IS_OBJECT)) {

		ce = Z_OBJCE_P(object);
		if (ce->parent) {
			ce = zephir_lookup_class_ce_quick(ce, property_name, property_length, key TSRMLS_CC);
		}

		old_scope = EG(scope);
		EG(scope) = ce;

		zobj = zend_objects_get_address(object TSRMLS_CC);

		if (zephir_hash_quick_find(&ce->properties_info, property_name, property_length + 1, key, (void **) &property_info) == SUCCESS) {

			#if PHP_VERSION_ID < 50400

			if (zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length+1, property_info->h, (void **) &zv) == SUCCESS) {

				EG(scope) = old_scope;

				if (return_value_ptr) {
					zval_ptr_dtor(return_value_ptr);
					Z_ADDREF_PP(zv);
					*return_value_ptr = *zv;
				}
				else {
					ZVAL_ZVAL(return_value, *zv, 1, 0);
				}

				return SUCCESS;
			}

			#else

			int flag;
			if (EXPECTED((property_info->flags & ZEND_ACC_STATIC) == 0) && property_info->offset >= 0) {
				if (zobj->properties) {
					zv   = (zval**) zobj->properties_table[property_info->offset];
					flag = (zv == NULL) ? 1 : 0;
				} else {
					zv   = &zobj->properties_table[property_info->offset];
					flag = (*zv == NULL) ? 1 : 0;
				}
			} else if (UNEXPECTED(!zobj->properties)) {
				flag = 1;
			} else if (UNEXPECTED(zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length+1, property_info->h, (void **) &zv) == FAILURE)) {
				flag = 2;
			} else {
				flag = 0;
			}

			if (unlikely(flag) && zobj->properties) {
				if (
					(flag == 2 || zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length+1, property_info->h, (void **) &zv) == FAILURE)
					&& zv && *zv
				) {
					flag = 0;
				}
			}

			if (likely(!flag)) {
				EG(scope) = old_scope;

				if (return_value_ptr) {
					zval_ptr_dtor(return_value_ptr);
					Z_ADDREF_PP(zv);
					*return_value_ptr = *zv;
				}
				else {
					ZVAL_ZVAL(return_value, *zv, 1, 0);
				}

				return SUCCESS;
			}

			#endif

		}

		EG(scope) = old_scope;

	} else {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Trying to get property of non-object");
	}

	ZVAL_NULL(return_value);
	return FAILURE;
}

int zephir_return_property(zval *return_value, zval **return_value_ptr, zval *object, char *property_name, unsigned int property_length TSRMLS_DC) {

	return zephir_return_property_quick(return_value, return_value_ptr, object, property_name, property_length, zend_inline_hash_func(property_name, property_length + 1) TSRMLS_CC);
}

int zephir_read_property_zval(zval **result, zval *object, zval *property, int flags TSRMLS_DC) {

	if (unlikely(Z_TYPE_P(property) != IS_STRING)) {

		if ((flags & PH_NOISY) == PH_NOISY) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot access empty property %d", Z_TYPE_P(property));
		}

		*result = ZEPHIR_GLOBAL(global_null);
		Z_ADDREF_P(*result);
		return FAILURE;
	}

	return zephir_read_property(result, object, Z_STRVAL_P(property), Z_STRLEN_P(property), flags TSRMLS_CC);
}

int zephir_update_property_long(zval *object, char *property_name, unsigned int property_length, long value TSRMLS_DC) {

	zval *v;

	ALLOC_ZVAL(v);
	Z_UNSET_ISREF_P(v);
	Z_SET_REFCOUNT_P(v, 0);
	ZVAL_LONG(v, value);

	return zephir_update_property_zval(object, property_name, property_length, v TSRMLS_CC);
}

int zephir_update_property_string(zval *object, char *property_name, unsigned int property_length, char *str, unsigned int str_length TSRMLS_DC) {

	zval *value;
	int res;

	ALLOC_ZVAL(value);
	Z_UNSET_ISREF_P(value);
	Z_SET_REFCOUNT_P(value, 0);
	ZVAL_STRINGL(value, str, str_length, 1);

	res = zephir_update_property_zval(object, property_name, property_length, value TSRMLS_CC);
	if (res == SUCCESS) {
		return SUCCESS;
	}

	return FAILURE;
}

int zephir_update_property_bool(zval *object, char *property_name, unsigned int property_length, int value TSRMLS_DC) {
	return zephir_update_property_zval(object, property_name, property_length, value ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
}

int zephir_update_property_null(zval *object, char *property_name, unsigned int property_length TSRMLS_DC) {
	return zephir_update_property_zval(object, property_name, property_length, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
}

int zephir_update_property_zval(zval *object, const char *property_name, unsigned int property_length, zval *value TSRMLS_DC){

	zend_class_entry *ce, *old_scope;
	zval *property;

	old_scope = EG(scope);
	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Attempt to assign property of non-object");
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length TSRMLS_CC);
	}

	EG(scope) = ce;

	if (!Z_OBJ_HT_P(object)->write_property) {
#if PHP_VERSION_ID < 50400
		char *class_name;
#else
		const char *class_name;
#endif
		zend_uint class_name_len;

		zend_get_object_classname(object, &class_name, &class_name_len TSRMLS_CC);
		zend_error(E_CORE_ERROR, "Property %s of class %s cannot be updated", property_name, class_name);
	}

	MAKE_STD_ZVAL(property);
	ZVAL_STRINGL(property, property_name, property_length, 0);

#if PHP_VERSION_ID < 50400
	Z_OBJ_HT_P(object)->write_property(object, property, value TSRMLS_CC);
#else
	Z_OBJ_HT_P(object)->write_property(object, property, value, 0 TSRMLS_CC);
#endif

	if (Z_REFCOUNT_P(property) > 1) {
		ZVAL_STRINGL(property, property_name, property_length, 1);
	} else {
		ZVAL_NULL(property);
	}

	zval_ptr_dtor(&property);

	EG(scope) = old_scope;
	return SUCCESS;
}

int zephir_update_property_this_quick(zval *object, const char *property_name, zend_uint property_length, zval *value, ulong key TSRMLS_DC){

	zend_class_entry *ce, *old_scope;

	if (unlikely(Z_TYPE_P(object) != IS_OBJECT)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Attempt to assign property of non-object");
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce_quick(ce, property_name, property_length, key TSRMLS_CC);
	}

	old_scope = EG(scope);
	EG(scope) = ce;

	#if PHP_VERSION_ID < 50400

	{
		zval *property;

		if (!Z_OBJ_HT_P(object)->write_property) {
			EG(scope) = old_scope;
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Property %s of class %s cannot be updated", property_name, ce->name);
			return FAILURE;
		}

		MAKE_STD_ZVAL(property);
		ZVAL_STRINGL(property, property_name, property_length, 0);

		Z_OBJ_HT_P(object)->write_property(object, property, value TSRMLS_CC);

		if (Z_REFCOUNT_P(property) > 1) {
			ZVAL_STRINGL(property, property_name, property_length, 1);
		} else {
			ZVAL_NULL(property);
		}

		zval_ptr_dtor(&property);
	}

	#else

	{
		zend_object *zobj;
		zval **variable_ptr;
		zend_property_info *property_info;

		zobj = zend_objects_get_address(object TSRMLS_CC);

		if (zephir_hash_quick_find(&ce->properties_info, property_name, property_length + 1, key, (void **) &property_info) == SUCCESS) {
			assert(property_info != NULL);

			/** This is as zend_std_write_property, but we're not interesed in validate properties visibility */
			if (property_info->offset >= 0 ? (zobj->properties ? ((variable_ptr = (zval**) zobj->properties_table[property_info->offset]) != NULL) : (*(variable_ptr = &zobj->properties_table[property_info->offset]) != NULL)) : (EXPECTED(zobj->properties != NULL) && EXPECTED(zephir_hash_quick_find(zobj->properties, property_info->name, property_info->name_length + 1, property_info->h, (void **) &variable_ptr) == SUCCESS))) {

				if (EXPECTED(*variable_ptr != value)) {

					/* if we are assigning reference, we shouldn't move it, but instead assign variable to the same pointer */
					if (PZVAL_IS_REF(*variable_ptr)) {

						zval garbage = **variable_ptr; /* old value should be destroyed */

						/* To check: can't *variable_ptr be some system variable like error_zval here? */
						Z_TYPE_PP(variable_ptr) = Z_TYPE_P(value);
						(*variable_ptr)->value = value->value;
						if (Z_REFCOUNT_P(value) > 0) {
							zval_copy_ctor(*variable_ptr);
						} else {
							efree(value);
						}
						zval_dtor(&garbage);

					} else {
						zval *garbage = *variable_ptr;

						/* if we assign referenced variable, we should separate it */
						Z_ADDREF_P(value);
						if (PZVAL_IS_REF(value)) {
							SEPARATE_ZVAL(&value);
						}
						*variable_ptr = value;
						zval_ptr_dtor(&garbage);
					}
				}

			}
		}
	}

	#endif

	EG(scope) = old_scope;

	return SUCCESS;
}

int zephir_update_property_this(zval *object, char *property_name, unsigned int property_length, zval *value TSRMLS_DC){

	return zephir_update_property_this_quick(object, property_name, property_length, value, zend_inline_hash_func(property_name, property_length + 1) TSRMLS_CC);
}

int zephir_update_property_zval_zval(zval *object, zval *property, zval *value TSRMLS_DC){

	if (Z_TYPE_P(property) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Property should be string");
		return FAILURE;
	}

	return zephir_update_property_zval(object, Z_STRVAL_P(property), Z_STRLEN_P(property), value TSRMLS_CC);
}

int zephir_update_property_array(zval *object, const char *property, zend_uint property_length, const zval *index, zval *value TSRMLS_DC) {

	zval *tmp;
	int separated = 0;

	if (Z_TYPE_P(object) == IS_OBJECT) {

		zephir_read_property(&tmp, object, property, property_length, PH_NOISY TSRMLS_CC);

		Z_DELREF_P(tmp);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			separated = 1;
		}

		/** Convert the value to array if not is an array */
		if (Z_TYPE_P(tmp) != IS_ARRAY) {
			if (separated) {
				convert_to_array(tmp);
			} else {
				zval *new_zv;
				ALLOC_ZVAL(new_zv);
				INIT_PZVAL_COPY(new_zv, tmp);
				tmp = new_zv;
				zval_copy_ctor(new_zv);
				Z_SET_REFCOUNT_P(tmp, 0);
				array_init(tmp);
				separated = 1;
			}
		}

		Z_ADDREF_P(value);

		if (Z_TYPE_P(index) == IS_STRING) {
			zend_symtable_update(Z_ARRVAL_P(tmp), Z_STRVAL_P(index), Z_STRLEN_P(index) + 1, &value, sizeof(zval*), NULL);
		} else if (Z_TYPE_P(index) == IS_LONG) {
			zend_hash_index_update(Z_ARRVAL_P(tmp), Z_LVAL_P(index), &value, sizeof(zval *), NULL);
		} else if (Z_TYPE_P(index) == IS_NULL) {
			zend_hash_next_index_insert(Z_ARRVAL_P(tmp), (void**)&value, sizeof(zval*), NULL);
		}

		if (separated) {
			zephir_update_property_zval(object, property, property_length, tmp TSRMLS_CC);
		}
	}

	return SUCCESS;
}

int zephir_update_property_array_multi(zval *object, const char *property, zend_uint property_length, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...) {

	int i, l, ll; char *s;
	va_list ap;
	zval *fetched, *tmp_arr, *tmp, *p, *item, *old_p;
	int separated = 0;

	if (Z_TYPE_P(object) == IS_OBJECT) {

		zephir_read_property(&tmp_arr, object, property, property_length, PH_NOISY TSRMLS_CC);

		Z_DELREF_P(tmp_arr);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp_arr) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp_arr);
			tmp_arr = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp_arr, 0);
			separated = 1;
		}

		/** Convert the value to array if not is an array */
		if (Z_TYPE_P(tmp_arr) != IS_ARRAY) {
			if (separated) {
				convert_to_array(tmp_arr);
			} else {
				zval *new_zv;
				ALLOC_ZVAL(new_zv);
				INIT_PZVAL_COPY(new_zv, tmp_arr);
				tmp_arr = new_zv;
				zval_copy_ctor(new_zv);
				Z_SET_REFCOUNT_P(tmp_arr, 0);
				array_init(tmp_arr);
				separated = 1;
			}
		}

		va_start(ap, types_count);

		p = tmp_arr;
		old_p = NULL;
		for (i = 0; i < types_length; ++i) {
			switch (types[i]) {

				case 's':
					s = va_arg(ap, char*);
					l = va_arg(ap, int);
					if (zephir_array_isset_string_fetch(&fetched, p, s, l + 1, 1 TSRMLS_CC)) {
						if (Z_TYPE_P(fetched) == IS_ARRAY) {
							if (i == (types_length - 1)) {
								zephir_array_update_string(&fetched, s, l, value, PH_COPY | PH_SEPARATE);
							} else {
								old_p = p;
								p = fetched;
							}
							continue;
						}
					}
					if (i == (types_length - 1)) {
						zephir_array_update_string(&p, s, l, value, PH_COPY | PH_SEPARATE);
					} else {
						MAKE_STD_ZVAL(tmp);
						array_init(tmp);
						zephir_array_update_string(&p, s, l, &tmp, PH_SEPARATE);
						p = tmp;
					}
					break;

				case 'l':
					ll = va_arg(ap, long);
					if (zephir_array_isset_long_fetch(&fetched, p, ll, 1 TSRMLS_CC)) {
						if (Z_TYPE_P(fetched) == IS_ARRAY) {
							if (i == (types_length - 1)) {
								zephir_array_update_long(&fetched, ll, value, PH_COPY | PH_SEPARATE, "", 0);
							} else {
								old_p = p;
								p = fetched;
							}
							continue;
						}
					}
					if (i == (types_length - 1)) {
						zephir_array_update_long(&p, ll, value, PH_COPY | PH_SEPARATE, "", 0);
					} else {
						MAKE_STD_ZVAL(tmp);
						array_init(tmp);
						zephir_array_update_long(&p, ll, &tmp, PH_SEPARATE, "", 0);
						p = tmp;
					}
					break;

				case 'z':
					item = va_arg(ap, zval*);
					if (zephir_array_isset_fetch(&fetched, p, item, 1 TSRMLS_CC)) {
						if (Z_TYPE_P(fetched) == IS_ARRAY) {
							if (i == (types_length - 1)) {
								zephir_array_update_zval(&fetched, item, value, PH_COPY | PH_SEPARATE);
							} else {
								old_p = p;
								p = fetched;
							}
							continue;
						}
					}
					if (i == (types_length - 1)) {
						zephir_array_update_zval(&p, item, value, PH_COPY | PH_SEPARATE);
					} else {
						MAKE_STD_ZVAL(tmp);
						array_init(tmp);
						zephir_array_update_zval(&p, item, &tmp, PH_SEPARATE);
						p = tmp;
					}
					break;

				case 'a':
					if (Z_REFCOUNT_P(p) > 1) {
						SEPARATE_ZVAL_IF_NOT_REF(&p);
						switch (types[i - 1]) {
							case 'z':
								zephir_array_update_zval(&old_p, item, &p, PH_COPY);
								break;
						}
					}
					zephir_array_append(&p, *value, 0 ZEPHIR_DEBUG_PARAMS_DUMMY);
					break;
			}
		}

		va_end(ap);

		if (separated) {
			zephir_update_property_zval(object, property, property_length, tmp_arr TSRMLS_CC);
		}
	}

	return SUCCESS;
}

int zephir_update_property_array_string(zval *object, char *property, unsigned int property_length, char *index, unsigned int index_length, zval *value TSRMLS_DC) {

	zval *tmp;
	int separated = 0;

	if (likely(Z_TYPE_P(object) == IS_OBJECT)) {

		zephir_read_property(&tmp, object, property, property_length, PH_NOISY_CC);

		Z_DELREF_P(tmp);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			separated = 1;
		}

		/** Convert the value to array if not is an array */
		if (Z_TYPE_P(tmp) != IS_ARRAY) {
			if (separated) {
				convert_to_array(tmp);
			} else {
				zval *new_zv;
				ALLOC_ZVAL(new_zv);
				INIT_PZVAL_COPY(new_zv, tmp);
				tmp = new_zv;
				zval_copy_ctor(new_zv);
				Z_SET_REFCOUNT_P(tmp, 0);
				array_init(tmp);
				separated = 1;
			}
		}

		Z_ADDREF_P(value);

		zend_hash_update(Z_ARRVAL_P(tmp), index, index_length, &value, sizeof(zval *), NULL);

		if (separated) {
			zephir_update_property_zval(object, property, property_length, tmp TSRMLS_CC);
		}

	}

	return SUCCESS;
}

int zephir_update_property_array_append(zval *object, char *property, unsigned int property_length, zval *value TSRMLS_DC) {

	zval *tmp;
	int separated = 0;

	if (Z_TYPE_P(object) != IS_OBJECT) {
		return SUCCESS;
	}

	zephir_read_property(&tmp, object, property, property_length, PH_NOISY_CC);

	Z_DELREF_P(tmp);

	/** Separation only when refcount > 1 */
	if (Z_REFCOUNT_P(tmp) > 1) {
		zval *new_zv;
		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, tmp);
		tmp = new_zv;
		zval_copy_ctor(new_zv);
		Z_SET_REFCOUNT_P(tmp, 0);
		separated = 1;
	}

	/** Convert the value to array if not is an array */
	if (Z_TYPE_P(tmp) != IS_ARRAY) {
		if (separated) {
			convert_to_array(tmp);
		} else {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			array_init(tmp);
			separated = 1;
		}
	}

	Z_ADDREF_P(value);
	add_next_index_zval(tmp, value);

	if (separated) {
		zephir_update_property_zval(object, property, property_length, tmp TSRMLS_CC);
	}

	return SUCCESS;
}

int zephir_update_property_empty_array(zend_class_entry *ce, zval *object, char *property_name, unsigned int property_length TSRMLS_DC) {

	zval *empty_array;
	int res;

	ALLOC_INIT_ZVAL(empty_array);
	array_init(empty_array);

	res = zephir_update_property_zval(object, property_name, property_length, empty_array TSRMLS_CC);
	zval_ptr_dtor(&empty_array);
	return res;
}

int zephir_unset_property(zval* object, const char* name TSRMLS_DC)
{
	if (Z_TYPE_P(object) == IS_OBJECT) {
		zval member;
		zend_class_entry *old_scope;

		INIT_PZVAL(&member);
		ZVAL_STRING(&member, name, 0);
		old_scope = EG(scope);
		EG(scope) = Z_OBJCE_P(object);

		#if PHP_VERSION_ID < 50400
			Z_OBJ_HT_P(object)->unset_property(object, &member TSRMLS_CC);
		#else
			Z_OBJ_HT_P(object)->unset_property(object, &member, 0 TSRMLS_CC);
		#endif

		EG(scope) = old_scope;

		return SUCCESS;
	}

	return FAILURE;
}

int zephir_unset_property_array(zval *object, char *property, unsigned int property_length, zval *index TSRMLS_DC) {

	zval *tmp;
	int separated = 0;

	if (Z_TYPE_P(object) == IS_OBJECT) {

		zephir_read_property(&tmp, object, property, property_length, PH_NOISY_CC);
		Z_DELREF_P(tmp);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			separated = 1;
		}

		zephir_array_unset(&tmp, index, 0);

		if (separated) {
			zephir_update_property_zval(object, property, property_length, tmp TSRMLS_CC);
		}
	}

	return SUCCESS;
}

int zephir_method_exists(const zval *object, const zval *method_name TSRMLS_DC){

	char *lcname = zend_str_tolower_dup(Z_STRVAL_P(method_name), Z_STRLEN_P(method_name));
	int res = zephir_method_exists_ex(object, lcname, Z_STRLEN_P(method_name)+1 TSRMLS_CC);
	efree(lcname);
	return res;
}

int zephir_method_exists_ex(const zval *object, const char *method_name, unsigned int method_len TSRMLS_DC){

	return zephir_method_quick_exists_ex(object, method_name, method_len, zend_inline_hash_func(method_name, method_len) TSRMLS_CC);
}

int zephir_method_quick_exists_ex(const zval *object, const char *method_name, unsigned int method_len, unsigned long hash TSRMLS_DC){

	zend_class_entry *ce;

	if (likely(Z_TYPE_P(object) == IS_OBJECT)) {
		ce = Z_OBJCE_P(object);
	} else {
		if (Z_TYPE_P(object) == IS_STRING) {
			ce = zend_fetch_class(Z_STRVAL_P(object), Z_STRLEN_P(object), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
		} else {
			return FAILURE;
		}
	}

	while (ce) {
		if (zephir_hash_quick_exists(&ce->function_table, method_name, method_len, hash)) {
			return SUCCESS;
		}
		ce = ce->parent;
	}

	return FAILURE;
}

zval* zephir_fetch_static_property_ce(zend_class_entry *ce, const char *property, int len TSRMLS_DC) {
	assert(ce != NULL);
	return zend_read_static_property(ce, property, len, (zend_bool) ZEND_FETCH_CLASS_SILENT TSRMLS_CC);
}

int zephir_read_static_property_ce(zval **result, zend_class_entry *ce, const char *property, int len TSRMLS_DC) {
	assert(ce != NULL);

	*result = zephir_fetch_static_property_ce(ce, property, len TSRMLS_CC);
	if (*result) {
		Z_ADDREF_PP(result);
		return SUCCESS;
	}

	return FAILURE;
}

#if PHP_VERSION_ID >= 50400
static zval **zephir_std_get_static_property(zend_class_entry *ce, const char *property_name, int property_name_len, zend_bool silent, ulong hash_value, zend_property_info **
	property_info TSRMLS_DC)
{
	zend_property_info *temp_property_info;

	if (!hash_value) {
		hash_value = zend_hash_func(property_name, property_name_len + 1);
	}

	if (!property_info || !*property_info) {

		if (UNEXPECTED(zend_hash_quick_find(&ce->properties_info, property_name, property_name_len+1, hash_value, (void **) &temp_property_info)==FAILURE)) {
			if (!silent) {
				zend_error_noreturn(E_ERROR, "Access to undeclared static property: %s::$%s", ce->name, property_name);
			}
			return NULL;
		}

		#ifndef ZEPHIR_RELEASE
		if (UNEXPECTED(!zend_verify_property_access(temp_property_info, ce TSRMLS_CC))) {
			if (!silent) {
				zend_error_noreturn(E_ERROR, "Cannot access %s property %s::$%s", zend_visibility_string(temp_property_info->flags), ce->name, property_name);
			}
			return NULL;
		}

		if (UNEXPECTED((temp_property_info->flags & ZEND_ACC_STATIC) == 0)) {
			if (!silent) {
				zend_error_noreturn(E_ERROR, "Access to undeclared static property: %s::$%s", ce->name, property_name);
			}
			return NULL;
		}
		#endif

		zend_update_class_constants(ce TSRMLS_CC);

		if (property_info) {
			*property_info = temp_property_info;
		}

	} else {
		temp_property_info = *property_info;
	}

	if (UNEXPECTED(CE_STATIC_MEMBERS(ce) == NULL) || UNEXPECTED(CE_STATIC_MEMBERS(ce)[temp_property_info->offset] == NULL)) {
		if (!silent) {
			zend_error_noreturn(E_ERROR, "Access to undeclared static property: %s::$%s", ce->name, property_name);
		}
		return NULL;
	}

	return &CE_STATIC_MEMBERS(ce)[temp_property_info->offset];
}
#endif

static int zephir_update_static_property_ex(zend_class_entry *scope, const char *name, int name_length,
	zval *value, zend_property_info **property_info TSRMLS_DC)
{
	zval **property;
	zend_class_entry *old_scope = EG(scope);

	if (value == ZEPHIR_GLOBAL(global_null)) {
		ALLOC_ZVAL(value);
		Z_UNSET_ISREF_P(value);
		Z_SET_REFCOUNT_P(value, 0);
		ZVAL_NULL(value);
	}

	EG(scope) = scope;
#if PHP_VERSION_ID < 50400
	property = zend_std_get_static_property(scope, name, name_length, 0 TSRMLS_CC);
#else
	property = zephir_std_get_static_property(scope, name, name_length, zend_inline_hash_func(name, name_length), 0, property_info TSRMLS_CC);
#endif
	EG(scope) = old_scope;

	if (!property) {
		return FAILURE;
	} else {
		if (*property != value) {
			if (PZVAL_IS_REF(*property)) {
				zval_dtor(*property);
				Z_TYPE_PP(property) = Z_TYPE_P(value);
				(*property)->value = value->value;
				if (Z_REFCOUNT_P(value) > 0) {
					zval_copy_ctor(*property);
				} else {
					efree(value);
				}
			} else {
				zval *garbage = *property;

				Z_ADDREF_P(value);
				if (PZVAL_IS_REF(value)) {
					SEPARATE_ZVAL(&value);
				}
				*property = value;
				zval_ptr_dtor(&garbage);
			}
		}
		return SUCCESS;
	}
}

int zephir_read_static_property(zval **result, const char *class_name, unsigned int class_length, char *property_name,
	unsigned int property_length TSRMLS_DC){
	zend_class_entry **ce;
	if (zend_lookup_class(class_name, class_length, &ce TSRMLS_CC) == SUCCESS) {
		return zephir_read_static_property_ce(result, *ce, property_name, property_length TSRMLS_CC);
	}
	return FAILURE;
}

int zephir_update_static_property_ce(zend_class_entry *ce, const char *name, int len, zval *value TSRMLS_DC) {
	assert(ce != NULL);
	return zephir_update_static_property_ex(ce, name, len, value, NULL TSRMLS_CC);
}

int zephir_update_static_property_ce_cache(zend_class_entry *ce, const char *name, int len, zval *value, zend_property_info **property_info TSRMLS_DC) {
	assert(ce != NULL);
	return zephir_update_static_property_ex(ce, name, len, value, property_info TSRMLS_CC);
}

int zephir_update_static_property_array_multi_ce(zend_class_entry *ce, const char *property, zend_uint property_length, zval **value TSRMLS_DC, const char *types, int types_length, int types_count, ...) {

	int i, l, ll; char *s;
	va_list ap;
	zval *fetched, *tmp_arr, *tmp, *p, *item;
	int separated = 0;

	zephir_read_static_property_ce(&tmp_arr, ce, property, property_length TSRMLS_CC);

	Z_DELREF_P(tmp_arr);

	/** Separation only when refcount > 1 */
	if (Z_REFCOUNT_P(tmp_arr) > 1) {
		zval *new_zv;
		ALLOC_ZVAL(new_zv);
		INIT_PZVAL_COPY(new_zv, tmp_arr);
		tmp_arr = new_zv;
		zval_copy_ctor(new_zv);
		Z_SET_REFCOUNT_P(tmp_arr, 0);
		separated = 1;
	}

	/** Convert the value to array if not is an array */
	if (Z_TYPE_P(tmp_arr) != IS_ARRAY) {
		if (separated) {
			convert_to_array(tmp_arr);
		} else {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp_arr);
			tmp_arr = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp_arr, 0);
			array_init(tmp_arr);
			separated = 1;
		}
	}

	va_start(ap, types_count);

	p = tmp_arr;
	for (i = 0; i < types_length; ++i) {
		switch (types[i]) {

			case 's':
				s = va_arg(ap, char*);
				l = va_arg(ap, int);
				if (zephir_array_isset_string_fetch(&fetched, p, s, l + 1, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_string(&fetched, s, l, value, PH_COPY | PH_SEPARATE);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_string(&p, s, l, value, PH_COPY | PH_SEPARATE);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_string(&p, s, l, &tmp, PH_SEPARATE);
					p = tmp;
				}
				break;

			case 'l':
				ll = va_arg(ap, long);
				if (zephir_array_isset_long_fetch(&fetched, p, ll, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_long(&fetched, ll, value, PH_COPY | PH_SEPARATE, "", 0);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_long(&p, ll, value, PH_COPY | PH_SEPARATE, "", 0);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_long(&p, ll, &tmp, PH_SEPARATE, "", 0);
					p = tmp;
				}
				break;

			case 'z':
				item = va_arg(ap, zval*);
				if (zephir_array_isset_fetch(&fetched, p, item, 1 TSRMLS_CC)) {
					if (Z_TYPE_P(fetched) == IS_ARRAY) {
						if (i == (types_length - 1)) {
							zephir_array_update_zval(&fetched, item, value, PH_COPY | PH_SEPARATE);
						} else {
							p = fetched;
						}
						continue;
					}
				}
				if (i == (types_length - 1)) {
					zephir_array_update_zval(&p, item, value, PH_COPY | PH_SEPARATE);
				} else {
					MAKE_STD_ZVAL(tmp);
					array_init(tmp);
					zephir_array_update_zval(&p, item, &tmp, PH_SEPARATE);
					p = tmp;
				}
				break;

			case 'a':
				zephir_array_append(&p, *value, PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				break;
		}
	}

	va_end(ap);

	if (separated) {
		zephir_update_static_property_ce(ce, property, property_length, tmp_arr TSRMLS_CC);
	}

	return SUCCESS;
}

int zephir_update_static_property(const char *class_name, unsigned int class_length, char *name, unsigned int name_length, zval *value TSRMLS_DC){
	zend_class_entry **ce;
	if (zend_lookup_class(class_name, class_length, &ce TSRMLS_CC) == SUCCESS) {
		return zend_update_static_property(*ce, name, name_length, value TSRMLS_CC);
	}

	return FAILURE;
}

int zephir_read_class_property(zval **result, int type, const char *property, int len TSRMLS_DC) {
	zend_class_entry *ce;

	type |= (ZEND_FETCH_CLASS_SILENT | ZEND_FETCH_CLASS_NO_AUTOLOAD);
	type &= ZEND_FETCH_CLASS_MASK;
	ce    = zend_fetch_class(NULL, 0, type TSRMLS_CC);

	if (likely(ce != NULL)) {
		return zephir_read_static_property_ce(result, ce, property, len TSRMLS_CC);
	}

	return FAILURE;
}

int zephir_create_instance(zval *return_value, const zval *class_name TSRMLS_DC){

	zend_class_entry *ce;

	if (Z_TYPE_P(class_name) != IS_STRING) {
		zephir_throw_exception_string(spl_ce_RuntimeException, SL("Invalid class name") TSRMLS_CC);
		return FAILURE;
	}

	ce = zend_fetch_class(Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
	if (!ce) {
		return FAILURE;
	}

	object_init_ex(return_value, ce);
	if (zephir_has_constructor_ce(ce)) {
		return zephir_call_class_method_aparams(NULL, ce, zephir_fcall_method, return_value, SL("__construct"), NULL, 0, NULL TSRMLS_CC);
	}

	return SUCCESS;
}

int zephir_create_instance_params(zval *return_value, const zval *class_name, zval *params TSRMLS_DC){

	int outcome;
	zend_class_entry *ce;

	if (Z_TYPE_P(class_name) != IS_STRING) {
		zephir_throw_exception_string(spl_ce_RuntimeException, SL("Invalid class name") TSRMLS_CC);
		return FAILURE;
	}

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zephir_throw_exception_string(spl_ce_RuntimeException, SL("Instantiation parameters must be an array") TSRMLS_CC);
		return FAILURE;
	}

	ce = zend_fetch_class(Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), ZEND_FETCH_CLASS_DEFAULT TSRMLS_CC);
	if (!ce) {
		return FAILURE;
	}

	object_init_ex(return_value, ce);
	outcome = SUCCESS;

	if (zephir_has_constructor_ce(ce)) {
		int param_count = zend_hash_num_elements(Z_ARRVAL_P(params));
		zval *static_params[10];
		zval **params_ptr, **params_arr = NULL;

		if (param_count > 0) {
			HashPosition pos;
			zval **item;
			int i = 0;

			if (likely(param_count) <= 10) {
				params_ptr = static_params;
			}
			else {
				params_arr = emalloc(param_count * sizeof(zval*));
				params_ptr = &params;
			}

			for (
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(params), &pos);
				zend_hash_get_current_data_ex(Z_ARRVAL_P(params), (void**)&item, &pos) == SUCCESS;
				zend_hash_move_forward_ex(Z_ARRVAL_P(params), &pos), ++i
			) {
				params_ptr[i] = *item;
			}
		}
		else {
			params_ptr = NULL;
		}

		outcome = zephir_call_class_method_aparams(NULL, ce, zephir_fcall_method, return_value, SL("__construct"), NULL, param_count, params_ptr TSRMLS_CC);

		if (unlikely(params_arr != NULL)) {
			efree(params_arr);
		}
	}

	return outcome;
}

int zephir_property_incr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC){

	zval *tmp = NULL;
	zend_class_entry *ce;
	int separated = 0;

	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Attempt to assign property of non-object");
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length TSRMLS_CC);
	}

	zephir_read_property(&tmp, object, property_name, property_length, 0 TSRMLS_CC);
	if (tmp) {

		Z_DELREF_P(tmp);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			separated = 1;
		}

		zephir_increment(tmp);

		if (separated) {
			zephir_update_property_zval(object, property_name, property_length, tmp TSRMLS_CC);
		}
	}

	return SUCCESS;
}

int zephir_property_decr(zval *object, char *property_name, unsigned int property_length TSRMLS_DC){

	zval *tmp = NULL;
	zend_class_entry *ce;
	int separated = 0;

	if (Z_TYPE_P(object) != IS_OBJECT) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Attempt to assign property of non-object");
		return FAILURE;
	}

	ce = Z_OBJCE_P(object);
	if (ce->parent) {
		ce = zephir_lookup_class_ce(ce, property_name, property_length TSRMLS_CC);
	}

	zephir_read_property(&tmp, object, property_name, property_length, 0 TSRMLS_CC);
	if (tmp) {

		Z_DELREF_P(tmp);

		/** Separation only when refcount > 1 */
		if (Z_REFCOUNT_P(tmp) > 1) {
			zval *new_zv;
			ALLOC_ZVAL(new_zv);
			INIT_PZVAL_COPY(new_zv, tmp);
			tmp = new_zv;
			zval_copy_ctor(new_zv);
			Z_SET_REFCOUNT_P(tmp, 0);
			separated = 1;
		}

		zephir_decrement(tmp);

		if (separated) {
			zephir_update_property_zval(object, property_name, property_length, tmp TSRMLS_CC);
		}
	}

	return SUCCESS;
}

int zephir_fetch_property(zval **result, zval *object, const char *property_name, zend_uint property_length, int silent TSRMLS_DC) {

	if (zephir_isset_property(object, property_name, property_length + 1 TSRMLS_CC)) {
		zephir_read_property(result, object, property_name, property_length, 0 TSRMLS_CC);
		return 1;
	}

	*result = ZEPHIR_GLOBAL(global_null);
	Z_ADDREF_P(*result);
	return 0;
}

int zephir_fetch_property_zval(zval **result, zval *object, zval *property, int silent TSRMLS_DC) {

	if (unlikely(Z_TYPE_P(property) != IS_STRING)) {
		*result = ZEPHIR_GLOBAL(global_null);
		Z_ADDREF_P(*result);
		return 0;
	}

	if (zephir_isset_property(object, Z_STRVAL_P(property), Z_STRLEN_P(property) + 1 TSRMLS_CC)) {
		zephir_read_property(result, object, Z_STRVAL_P(property), Z_STRLEN_P(property), 0 TSRMLS_CC);
		return 1;
	}

	*result = ZEPHIR_GLOBAL(global_null);
	Z_ADDREF_P(*result);
	return 0;
}


#ifdef HAVE_CONFIG_H
#endif

#include <ctype.h>

#include <php.h>

#include <ext/standard/php_smart_str.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_rand.h>
#include <ext/standard/php_lcg.h>
#include <ext/standard/php_http.h>

#ifdef ZEPHIR_USE_PHP_PCRE
#include <ext/pcre/php_pcre.h>
#endif

#ifdef ZEPHIR_USE_PHP_JSON
#include <ext/json/php_json.h>
#endif


#define PH_RANDOM_ALNUM 0
#define PH_RANDOM_ALPHA 1
#define PH_RANDOM_HEXDEC 2
#define PH_RANDOM_NUMERIC 3
#define PH_RANDOM_NOZERO 4

void zephir_fast_strlen(zval *return_value, zval *str){

	zval copy;
	int use_copy = 0;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	ZVAL_LONG(return_value, Z_STRLEN_P(str));

	if (use_copy) {
		zval_dtor(str);
	}
}

int zephir_fast_strlen_ev(zval *str){

	zval copy;
	int use_copy = 0, length;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	if (use_copy) {
		zval_dtor(str);
	}

	return length;
}

void zephir_fast_strtolower(zval *return_value, zval *str){

	zval copy;
	int use_copy = 0;
	char *lower_str;
	unsigned int length;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	lower_str = estrndup(Z_STRVAL_P(str), length);
	php_strtolower(lower_str, length);

	if (use_copy) {
		zval_dtor(str);
	}

	ZVAL_STRINGL(return_value, lower_str, length, 0);
}

void zephir_strtolower_inplace(zval *s) {
	if (likely(Z_TYPE_P(s) == IS_STRING)) {
		php_strtolower(Z_STRVAL_P(s), Z_STRLEN_P(s));
	}
}

void zephir_fast_join(zval *result, zval *glue, zval *pieces TSRMLS_DC){

	if (Z_TYPE_P(glue) != IS_STRING || Z_TYPE_P(pieces) != IS_ARRAY) {
		ZVAL_NULL(result);
		zend_error(E_WARNING, "Invalid arguments supplied for join()");
		return;
	}

	php_implode(glue, pieces, result TSRMLS_CC);
}

void zephir_append_printable_zval(smart_str *implstr, zval **tmp TSRMLS_DC) {

	zval tmp_val;
	unsigned int str_len;

	switch (Z_TYPE_PP(tmp)) {
		case IS_STRING:
			smart_str_appendl(implstr, Z_STRVAL_PP(tmp), Z_STRLEN_PP(tmp));
			break;

		case IS_LONG:
			smart_str_append_long(implstr, Z_LVAL_PP(tmp));
			break;

		case IS_BOOL:
			if (Z_LVAL_PP(tmp) == 1) {
				smart_str_appendl(implstr, "1", sizeof("1") - 1);
			}
			break;

		case IS_NULL:
			break;

		case IS_DOUBLE: {
			char *stmp;
			str_len = spprintf(&stmp, 0, "%.*G", (int) EG(precision), Z_DVAL_PP(tmp));
			smart_str_appendl(implstr, stmp, str_len);
			efree(stmp);
		}
			break;

		case IS_OBJECT: {
			int copy;
			zval expr;
			zend_make_printable_zval(*tmp, &expr, &copy);
			smart_str_appendl(implstr, Z_STRVAL(expr), Z_STRLEN(expr));
			if (copy) {
				zval_dtor(&expr);
			}
		}
			break;

		default:
			tmp_val = **tmp;
			zval_copy_ctor(&tmp_val);
			convert_to_string(&tmp_val);
			smart_str_appendl(implstr, Z_STRVAL(tmp_val), Z_STRLEN(tmp_val));
			zval_dtor(&tmp_val);
			break;
	}
}

void zephir_fast_join_str(zval *return_value, char *glue, unsigned int glue_length, zval *pieces TSRMLS_DC){

	zval         **tmp;
	HashTable      *arr;
	HashPosition   pos;
	smart_str      implstr = {0};
	unsigned int   numelems, i = 0;

	if (Z_TYPE_P(pieces) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for fast_join()");
		RETURN_EMPTY_STRING();
	}

	arr = Z_ARRVAL_P(pieces);
	numelems = zend_hash_num_elements(arr);

	if (numelems == 0) {
		RETURN_EMPTY_STRING();
	}

	zend_hash_internal_pointer_reset_ex(arr, &pos);

	while (zend_hash_get_current_data_ex(arr, (void **) &tmp, &pos) == SUCCESS) {
		zephir_append_printable_zval(&implstr, tmp TSRMLS_CC);
		if (++i != numelems) {
			smart_str_appendl(&implstr, glue, glue_length);
		}
		zend_hash_move_forward_ex(arr, &pos);
	}
	smart_str_0(&implstr);

	if (implstr.len) {
		RETURN_STRINGL(implstr.c, implstr.len, 0);
	} else {
		smart_str_free(&implstr);
		RETURN_EMPTY_STRING();
	}
}

void zephir_camelize(zval *return_value, const zval *str){

	int i, len;
	smart_str camelize_str = {0};
	char *marker, ch;

	if (unlikely(Z_TYPE_P(str) != IS_STRING)) {
		zend_error(E_WARNING, "Invalid arguments supplied for camelize()");
		RETURN_EMPTY_STRING();
	}

	marker = Z_STRVAL_P(str);
	len    = Z_STRLEN_P(str);

	for (i = 0; i < len - 1; i++) {
		ch = *marker;
		if (i == 0 || ch == '-' || ch == '_') {
			if (ch == '-' || ch == '_') {
				i++;
				marker++;
			}

			smart_str_appendc(&camelize_str, toupper(*marker));
		}
		else {
			smart_str_appendc(&camelize_str, tolower(*marker));
		}

		marker++;
	}

	if (likely(i == len - 1)) {
		smart_str_appendc(&camelize_str, *marker);
	}

	smart_str_0(&camelize_str);

	if (camelize_str.c) {
		RETURN_STRINGL(camelize_str.c, camelize_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}

}

void zephir_uncamelize(zval *return_value, const zval *str){

	unsigned int i;
	smart_str uncamelize_str = {0};
	char *marker, ch;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_error(E_WARNING, "Invalid arguments supplied for camelize()");
		return;
	}

	marker = Z_STRVAL_P(str);
	for (i = 0; i < Z_STRLEN_P(str); i++) {
		ch = *marker;
		if (ch == '\0') {
			break;
		}
		if (ch >= 'A' && ch <= 'Z') {
			if (i > 0) {
				smart_str_appendc(&uncamelize_str, '_');
			}
			smart_str_appendc(&uncamelize_str, (*marker) + 32);
		} else {
			smart_str_appendc(&uncamelize_str, (*marker));
		}
		marker++;
	}
	smart_str_0(&uncamelize_str);

	if (uncamelize_str.c) {
		RETURN_STRINGL(uncamelize_str.c, uncamelize_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}
}

void zephir_fast_explode(zval *return_value, zval *delimiter, zval *str, long limit TSRMLS_DC){

	if (unlikely(Z_TYPE_P(str) != IS_STRING || Z_TYPE_P(delimiter) != IS_STRING)) {
		zend_error(E_WARNING, "Invalid arguments supplied for explode()");
		RETURN_EMPTY_STRING();
	}

	array_init(return_value);
	php_explode(delimiter, str, return_value, limit);
}

void zephir_fast_explode_str(zval *return_value, const char *delimiter, int delimiter_length, zval *str, long limit TSRMLS_DC){

	zval delimiter_zval;

	if (unlikely(Z_TYPE_P(str) != IS_STRING)) {
		zend_error(E_WARNING, "Invalid arguments supplied for explode()");
		RETURN_EMPTY_STRING();
	}

	ZVAL_STRINGL(&delimiter_zval, delimiter, delimiter_length, 0);

	array_init(return_value);
	php_explode(&delimiter_zval, str, return_value, limit);
}

int zephir_memnstr(const zval *haystack, const zval *needle ZEPHIR_DEBUG_PARAMS) {

	if (Z_TYPE_P(haystack) != IS_STRING || Z_TYPE_P(needle) != IS_STRING) {
		#ifndef ZEPHIR_RELEASE
		zend_error(E_WARNING, "Invalid arguments supplied for memnstr in %s on line %d", file, line);
		#else
		zend_error(E_WARNING, "Invalid arguments supplied for memnstr()");
		#endif
		return 0;
	}

	if (Z_STRLEN_P(haystack) >= Z_STRLEN_P(needle)) {
		return php_memnstr(Z_STRVAL_P(haystack), Z_STRVAL_P(needle), Z_STRLEN_P(needle), Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack)) ? 1 : 0;
	}

	return 0;
}

int zephir_memnstr_str(const zval *haystack, char *needle, unsigned int needle_length ZEPHIR_DEBUG_PARAMS) {

	if (Z_TYPE_P(haystack) != IS_STRING) {
		#ifndef ZEPHIR_RELEASE
		zend_error(E_WARNING, "Invalid arguments supplied for memnstr in %s on line %d", file, line);
		#else
		zend_error(E_WARNING, "Invalid arguments supplied for memnstr()");
		#endif
		return 0;
	}

	if (Z_STRLEN_P(haystack) >= needle_length) {
		return php_memnstr(Z_STRVAL_P(haystack), needle, needle_length, Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack)) ? 1 : 0;
	}

	return 0;
}

void zephir_fast_strpos(zval *return_value, const zval *haystack, const zval *needle, unsigned int offset) {

	const char *found = NULL;

	if (unlikely(Z_TYPE_P(haystack) != IS_STRING || Z_TYPE_P(needle) != IS_STRING)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for strpos()");
		return;
	}

	if (offset > Z_STRLEN_P(haystack)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Offset not contained in string");
		return;
	}

	if (!Z_STRLEN_P(needle)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Empty delimiter");
		return;
	}

	found = php_memnstr(Z_STRVAL_P(haystack)+offset, Z_STRVAL_P(needle), Z_STRLEN_P(needle), Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack));

	if (found) {
		ZVAL_LONG(return_value, found-Z_STRVAL_P(haystack));
	} else {
		ZVAL_BOOL(return_value, 0);
	}

}

void zephir_fast_strpos_str(zval *return_value, const zval *haystack, char *needle, unsigned int needle_length) {

	const char *found = NULL;

	if (unlikely(Z_TYPE_P(haystack) != IS_STRING)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for strpos()");
		return;
	}

	found = php_memnstr(Z_STRVAL_P(haystack), needle, needle_length, Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack));

	if (found) {
		ZVAL_LONG(return_value, found-Z_STRVAL_P(haystack));
	} else {
		ZVAL_BOOL(return_value, 0);
	}

}

void zephir_fast_stripos_str(zval *return_value, zval *haystack, char *needle, unsigned int needle_length) {

	const char *found = NULL;
	char *needle_dup, *haystack_dup;

	if (unlikely(Z_TYPE_P(haystack) != IS_STRING)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for stripos()");
		return;
	}

	haystack_dup = estrndup(Z_STRVAL_P(haystack), Z_STRLEN_P(haystack));
	php_strtolower(haystack_dup, Z_STRLEN_P(haystack));

	needle_dup = estrndup(needle, needle_length);
	php_strtolower(needle_dup, needle_length);

	found = php_memnstr(haystack_dup, needle, needle_length, haystack_dup + Z_STRLEN_P(haystack));

	efree(haystack_dup);
	efree(needle_dup);

	if (found) {
		ZVAL_LONG(return_value, found-Z_STRVAL_P(haystack));
	} else {
		ZVAL_BOOL(return_value, 0);
	}

}


void zephir_fast_str_replace(zval *return_value, zval *search, zval *replace, zval *subject) {

	zval replace_copy, search_copy;
	int copy_replace = 0, copy_search = 0;

	if (Z_TYPE_P(subject) != IS_STRING) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for str_replace()");
		return;
	}

	if (Z_TYPE_P(replace) != IS_STRING) {
		zend_make_printable_zval(replace, &replace_copy, &copy_replace);
		if (copy_replace) {
			replace = &replace_copy;
		}
	}

	if (Z_TYPE_P(search) != IS_STRING) {
		zend_make_printable_zval(search, &search_copy, &copy_search);
		if (copy_search) {
			search = &search_copy;
		}
	}

	Z_TYPE_P(return_value) = IS_STRING;
	if (Z_STRLEN_P(subject) == 0) {
		ZVAL_STRINGL(return_value, "", 0, 1);
		return;
	}

	if (Z_STRLEN_P(search) == 1) {
		php_char_to_str_ex(Z_STRVAL_P(subject),
			Z_STRLEN_P(subject),
			Z_STRVAL_P(search)[0],
			Z_STRVAL_P(replace),
			Z_STRLEN_P(replace),
			return_value,
			1,
			NULL);
	} else {
		if (Z_STRLEN_P(search) > 1) {
			Z_STRVAL_P(return_value) = php_str_to_str_ex(Z_STRVAL_P(subject), Z_STRLEN_P(subject),
				Z_STRVAL_P(search), Z_STRLEN_P(search),
				Z_STRVAL_P(replace), Z_STRLEN_P(replace), &Z_STRLEN_P(return_value), 1, NULL);
		} else {
			MAKE_COPY_ZVAL(&subject, return_value);
		}
	}

	if (copy_replace) {
		zval_dtor(replace);
	}

	if (copy_search) {
		zval_dtor(search);
	}

}

void zephir_fast_trim(zval *return_value, zval *str, zval *charlist, int where TSRMLS_DC) {

	zval copy;
	int use_copy = 0;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}
		if (charlist && Z_TYPE_P(charlist) == IS_STRING) {
			php_trim(Z_STRVAL_P(str), Z_STRLEN_P(str), Z_STRVAL_P(charlist), Z_STRLEN_P(charlist), return_value, where TSRMLS_CC);
		} else {
			php_trim(Z_STRVAL_P(str), Z_STRLEN_P(str), NULL, 0, return_value, where TSRMLS_CC);
		}

	if (use_copy) {
		zval_dtor(&copy);
	}
}

void zephir_fast_strip_tags(zval *return_value, zval *str) {

	zval copy;
	int use_copy = 0;
	char *stripped;
	size_t len;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	stripped = estrndup(Z_STRVAL_P(str), Z_STRLEN_P(str));
	len = php_strip_tags(stripped, Z_STRLEN_P(str), NULL, NULL, 0);

	if (use_copy) {
		zval_dtor(&copy);
	}

	ZVAL_STRINGL(return_value, stripped, len, 0);
}

void zephir_fast_strtoupper(zval *return_value, zval *str) {

	zval copy;
	int use_copy = 0;
	char *lower_str;
	unsigned int length;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	lower_str = estrndup(Z_STRVAL_P(str), length);
	php_strtoupper(lower_str, length);

	if (use_copy) {
		zval_dtor(str);
	}

	ZVAL_STRINGL(return_value, lower_str, length, 0);
}

int zephir_start_with(const zval *str, const zval *compared, zval *case_sensitive){

	int sensitive = 0;
	int i;
	char *op1_cursor, *op2_cursor;

	if (Z_TYPE_P(str) != IS_STRING || Z_TYPE_P(compared) != IS_STRING) {
		return 0;
	}

	if (!Z_STRLEN_P(compared) || !Z_STRLEN_P(str) || Z_STRLEN_P(compared) > Z_STRLEN_P(str)) {
		return 0;
	}

	if (case_sensitive) {
		sensitive = zend_is_true(case_sensitive);
	}

	if (!sensitive) {
		return !memcmp(Z_STRVAL_P(str), Z_STRVAL_P(compared), Z_STRLEN_P(compared));
	}

	op1_cursor = Z_STRVAL_P(str);
	op2_cursor = Z_STRVAL_P(compared);
	for (i = 0; i < Z_STRLEN_P(compared); i++) {
		if (tolower(*op1_cursor) != tolower(*op2_cursor)) {
			return 0;
		}

		op1_cursor++;
		op2_cursor++;
	}

	return 1;
}

int zephir_start_with_str(const zval *str, char *compared, unsigned int compared_length){

	if (Z_TYPE_P(str) != IS_STRING || compared_length > Z_STRLEN_P(str)) {
		return 0;
	}

	return !memcmp(Z_STRVAL_P(str), compared, compared_length);
}

int zephir_start_with_str_str(char *str, unsigned int str_length, char *compared, unsigned int compared_length){

	if (compared_length > str_length) {
		return 0;
	}

	return !memcmp(str, compared, compared_length);
}

int zephir_end_with(const zval *str, const zval *compared, zval *case_sensitive){

	int sensitive = 0;
	int i;
	char *op1_cursor, *op2_cursor;

	if (Z_TYPE_P(str) != IS_STRING || Z_TYPE_P(compared) != IS_STRING) {
		return 0;
	}

	if (!Z_STRLEN_P(compared) || !Z_STRLEN_P(str) || Z_STRLEN_P(compared) > Z_STRLEN_P(str)) {
		return 0;
	}

	if (case_sensitive) {
		sensitive = zend_is_true(case_sensitive);
	}

	if (!sensitive) {
		return !memcmp(Z_STRVAL_P(str) + Z_STRLEN_P(str) - Z_STRLEN_P(compared), Z_STRVAL_P(compared), Z_STRLEN_P(compared));
	}

	op1_cursor = Z_STRVAL_P(str) + Z_STRLEN_P(str) - Z_STRLEN_P(compared);
	op2_cursor = Z_STRVAL_P(compared);

	for (i = 0; i < Z_STRLEN_P(compared); ++i) {
		if (tolower(*op1_cursor) != tolower(*op2_cursor)) {
			return 0;
		}

		++op1_cursor;
		++op2_cursor;
	}

	return 1;
}

int zephir_end_with_str(const zval *str, char *compared, unsigned int compared_length){

	if (Z_TYPE_P(str) != IS_STRING) {
		return 0;
	}

	if (!compared_length || !Z_STRLEN_P(str) || compared_length > Z_STRLEN_P(str)) {
		return 0;
	}

	return !memcmp(Z_STRVAL_P(str) + Z_STRLEN_P(str) - compared_length, compared, compared_length);
}

void zephir_random_string(zval *return_value, const zval *type, const zval *length TSRMLS_DC) {

	long i, rand_type, ch;
	smart_str random_str = {0};

	if (Z_TYPE_P(type) != IS_LONG) {
		return;
	}

	if (Z_LVAL_P(type) > PH_RANDOM_NOZERO) {
		return;
	}

	if (Z_TYPE_P(length) != IS_LONG) {
		return;
	}

	/** Generate seed */
	if (!BG(mt_rand_is_seeded)) {
		php_mt_srand(GENERATE_SEED() TSRMLS_CC);
	}

	for (i = 0; i < Z_LVAL_P(length); i++) {

		switch (Z_LVAL_P(type)) {
			case PH_RANDOM_ALNUM:
				rand_type = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(rand_type, 0, 3, PHP_MT_RAND_MAX);
				break;
			case PH_RANDOM_ALPHA:
				rand_type = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(rand_type, 1, 2, PHP_MT_RAND_MAX);
				break;
			case PH_RANDOM_HEXDEC:
				rand_type = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(rand_type, 0, 1, PHP_MT_RAND_MAX);
				break;
			case PH_RANDOM_NUMERIC:
				rand_type = 0;
				break;
			case PH_RANDOM_NOZERO:
				rand_type = 5;
				break;
			default:
				continue;
		}

		switch (rand_type) {
			case 0:
				ch = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(ch, '0', '9', PHP_MT_RAND_MAX);
				break;
			case 1:
				ch = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(ch, 'a', 'f', PHP_MT_RAND_MAX);
				break;
			case 2:
				ch = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(ch, 'a', 'z', PHP_MT_RAND_MAX);
				break;
			case 3:
				ch = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(ch, 'A', 'Z', PHP_MT_RAND_MAX);
				break;
			case 5:
				ch = (long) (php_mt_rand(TSRMLS_C) >> 1);
				RAND_RANGE(ch, '1', '9', PHP_MT_RAND_MAX);
				break;
			default:
				continue;
		}

		smart_str_appendc(&random_str, (unsigned int) ch);
	}


	smart_str_0(&random_str);

	if (random_str.len) {
		RETURN_STRINGL(random_str.c, random_str.len, 0);
	} else {
		smart_str_free(&random_str);
		RETURN_EMPTY_STRING();
	}

}

void zephir_remove_extra_slashes(zval *return_value, const zval *str) {

	char *cursor, *removed_str;
	unsigned int i;

	if (Z_TYPE_P(str) != IS_STRING) {
		RETURN_EMPTY_STRING();
	}

	if (Z_STRLEN_P(str) > 1) {
		cursor = Z_STRVAL_P(str);
		cursor += (Z_STRLEN_P(str) - 1);
		for (i = Z_STRLEN_P(str); i > 0; i--) {
			if ((*cursor) == '/') {
				cursor--;
				continue;
			}
			break;
		}
	} else {
		i = Z_STRLEN_P(str);
	}

	removed_str = emalloc(i + 1);
	memcpy(removed_str, Z_STRVAL_P(str), i);
	removed_str[i] = '\0';

	RETURN_STRINGL(removed_str, i, 0);

}

int zephir_spprintf(char **message, int max_len, char *format, ...)
{
	va_list arg;
	int len;

	va_start(arg, format);
	len = vspprintf(message, max_len, format, arg);
	va_end(arg);
	return len;
}

void zephir_substr(zval *return_value, zval *str, long f, long l) {

	if (Z_TYPE_P(str) != IS_STRING) {

		if (Z_TYPE_P(str) == IS_NULL || Z_TYPE_P(str) == IS_BOOL) {
			RETURN_FALSE;
		}

		if (Z_TYPE_P(str) == IS_LONG) {
			RETURN_EMPTY_STRING();
		}

		zend_error(E_WARNING, "Invalid arguments supplied for zephir_substr()");
		RETURN_FALSE;
	}

		long str_len = Z_STRLEN_P(str);

		if ((l < 0 && -l > str_len)) {
			RETURN_FALSE;
		} else if (l > str_len) {
			l = str_len;
		} else if (l == 0) {
			l = str_len;
		}

		if (f > str_len) {
			RETURN_FALSE;
		} else if (f < 0 && -f > str_len) {
			f = 0;
		}

		if (l < 0 && (l + str_len - f) < 0) {
			RETURN_FALSE;
		}


		/* if "from" position is negative, count start position from the end
		 * of the string
		 */
		if (f < 0) {
			f = str_len + f;
			if (f < 0) {
				f = 0;
			}
		}

		/* if "length" position is negative, set it to the length
		 * needed to stop that many chars from the end of the string
		 */
		if (l < 0) {
			l = (str_len - f) + l;
			if (l < 0) {
				l = 0;
			}
		}

		if (f >= str_len) {
			RETURN_FALSE;
		}

		if ((f + l) > str_len) {
			l = str_len - f;
		}

	if (l <= 0){
		RETURN_EMPTY_STRING();
	}

	RETURN_STRINGL(Z_STRVAL_P(str) + f, l, 1);
}

void zephir_append_printable_array(smart_str *implstr, zval *value TSRMLS_DC) {

	zval         **tmp;
	HashTable      *arr;
	HashPosition   pos;
	unsigned int numelems, i = 0, str_len;

	arr = Z_ARRVAL_P(value);
	numelems = zend_hash_num_elements(arr);

	smart_str_appendc(implstr, '[');

	if (numelems > 0) {
		zend_hash_internal_pointer_reset_ex(arr, &pos);
		while (zend_hash_get_current_data_ex(arr, (void **) &tmp, &pos) == SUCCESS) {

			if (Z_TYPE_PP(tmp) == IS_OBJECT) {
				smart_str_appendc(implstr, 'O');
				{
					char stmp[MAX_LENGTH_OF_LONG + 1];
					str_len = slprintf(stmp, sizeof(stmp), "%ld", Z_OBJVAL_PP(tmp).handle);
					smart_str_appendl(implstr, stmp, str_len);
				}
			} else {
				if (Z_TYPE_PP(tmp) == IS_ARRAY) {
					zephir_append_printable_array(implstr, *tmp TSRMLS_CC);
				} else {
					zephir_append_printable_zval(implstr, tmp TSRMLS_CC);
				}
			}

			if (++i != numelems) {
				smart_str_appendc(implstr, ',');
			}

			zend_hash_move_forward_ex(arr, &pos);
		}
	}

	smart_str_appendc(implstr, ']');
}

void zephir_unique_key(zval *return_value, zval *prefix, zval *value TSRMLS_DC) {

	smart_str implstr = {0};

	if (Z_TYPE_P(prefix) == IS_STRING) {
		smart_str_appendl(&implstr, Z_STRVAL_P(prefix), Z_STRLEN_P(prefix));
	}

	if (Z_TYPE_P(value) == IS_ARRAY) {
		zephir_append_printable_array(&implstr, value TSRMLS_CC);
	} else {
		zephir_append_printable_zval(&implstr, &value TSRMLS_CC);
	}

	smart_str_0(&implstr);

	if (implstr.len) {
		RETURN_STRINGL(implstr.c, implstr.len, 0);
	} else {
		smart_str_free(&implstr);
		RETURN_NULL();
	}

}

zval *zephir_eol(int eol TSRMLS_DC) {

	zval *local_eol;

	ZEPHIR_INIT_VAR(local_eol);

	if (eol) {
		ZVAL_STRING(local_eol, PHP_EOL, 1);
	} else {
		ZVAL_EMPTY_STRING(local_eol);
	}

	return local_eol;
}

void zephir_base64_encode(zval *return_value, zval *data) {

	zval copy;
	char *encoded;
	int use_copy = 0, length;

	if (Z_TYPE_P(data) != IS_STRING) {
		zend_make_printable_zval(data, &copy, &use_copy);
		if (use_copy) {
			data = &copy;
		}
	}

	encoded = (char *) php_base64_encode((unsigned char *)(Z_STRVAL_P(data)), Z_STRLEN_P(data), &length);

	if (use_copy) {
		zval_dtor(data);
	}

	if (encoded) {
		RETURN_STRINGL(encoded, length, 0);
	} else {
		RETURN_NULL();
	}
}

void zephir_base64_decode(zval *return_value, zval *data) {

	zval copy;
	char *decoded;
	int use_copy = 0, length;

	if (Z_TYPE_P(data) != IS_STRING) {
		zend_make_printable_zval(data, &copy, &use_copy);
		if (use_copy) {
			data = &copy;
		}
	}

	decoded = (char *) php_base64_decode((unsigned char *)(Z_STRVAL_P(data)), Z_STRLEN_P(data), &length);

	if (use_copy) {
		zval_dtor(data);
	}

	if (decoded) {
		RETURN_STRINGL(decoded, length, 0);
	} else {
		RETURN_NULL();
	}
}

void zephir_md5(zval *return_value, zval *str) {

	PHP_MD5_CTX ctx;
	unsigned char digest[16];
	char hexdigest[33];
	zval copy;
	int use_copy = 0;

	if (Z_TYPE_P(str) != IS_STRING) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	PHP_MD5Init(&ctx);
	PHP_MD5Update(&ctx, Z_STRVAL_P(str), Z_STRLEN_P(str));
	PHP_MD5Final(digest, &ctx);

	make_digest(hexdigest, digest);

	ZVAL_STRINGL(return_value, hexdigest, 32, 1);
}

#if ZEPHIR_USE_PHP_PCRE

void zephir_preg_match(zval *return_value, zval **return_value_ptr, zval *regex, zval *subject, zval *matches, int global, long flags, long offset TSRMLS_DC)
{
	zval copy;
	int use_copy = 0;
	pcre_cache_entry *pce;

	if (Z_TYPE_P(regex) != IS_STRING) {
		zend_error(E_WARNING, "Invalid arguments supplied for zephir_preg_match()");
		RETURN_FALSE;
	}

	if (Z_TYPE_P(subject) != IS_STRING) {
		zend_make_printable_zval(subject, &copy, &use_copy);
		if (use_copy) {
			subject = &copy;
		}
	}

	/* Compile regex or get it from cache. */
	if ((pce = pcre_get_compiled_regex_cache(Z_STRVAL_P(regex), Z_STRLEN_P(regex) TSRMLS_CC)) == NULL) {

		if (use_copy) {
			zval_dtor(subject);
		}

		RETURN_FALSE;
	}

		if (flags != 0 || offset != 0) {
			php_pcre_match_impl(pce, Z_STRVAL_P(subject), Z_STRLEN_P(subject), return_value, matches, global, 1, flags, offset TSRMLS_CC);
		} else {
			php_pcre_match_impl(pce, Z_STRVAL_P(subject), Z_STRLEN_P(subject), return_value, matches, global, 0, 0, 0 TSRMLS_CC);
		}

	if (use_copy) {
		zval_dtor(&copy);
	}
}

#else

void zephir_preg_match(zval *return_value, zval **return_value_ptr, zval *regex, zval *subject, zval *matches, int global, long flags, long offset TSRMLS_DC)
{
	if (matches) {
		Z_SET_ISREF_P(matches);
	}

	if (global) {
		if (flags != 0 || offset != 0) {
			//zephir_call_func_params(return_value, return_value_ptr, SL("preg_match_all") TSRMLS_CC, (matches ? 3 : 2), regex, subject, matches, flags, offset);
		} else {
			//zephir_call_func_params(return_value, return_value_ptr, SL("preg_match_all") TSRMLS_CC, (matches ? 3 : 2), regex, subject, matches);
		}
	} else {
		if (flags != 0 || offset != 0) {
			//zephir_call_func_params(return_value, return_value_ptr, SL("preg_match") TSRMLS_CC, (matches ? 3 : 2), regex, subject, matches, flags, offset);
		} else {
			//zephir_call_func_params(return_value, return_value_ptr, SL("preg_match") TSRMLS_CC, (matches ? 3 : 2), regex, subject, matches);
		}
	}

	if (matches) {
		Z_UNSET_ISREF_P(matches);
	}
}

#endif /* ZEPHIR_USE_PHP_PCRE */

#ifdef ZEPHIR_USE_PHP_JSON

int zephir_json_encode(zval *return_value, zval **return_value_ptr, zval *v, int opts TSRMLS_DC)
{
	smart_str buf = { NULL, 0, 0 };

	php_json_encode(&buf, v, opts TSRMLS_CC);
	smart_str_0(&buf);
	ZVAL_STRINGL(return_value, buf.c, buf.len, 0);

	return SUCCESS;
}

int zephir_json_decode(zval *return_value, zval **return_value_ptr, zval *v, zend_bool assoc TSRMLS_DC)
{
	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(v) != IS_STRING)) {
		zend_make_printable_zval(v, &copy, &use_copy);
		if (use_copy) {
			v = &copy;
		}
	}

	php_json_decode(return_value, Z_STRVAL_P(v), Z_STRLEN_P(v), assoc, 512 /* JSON_PARSER_DEFAULT_DEPTH */ TSRMLS_CC);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}

	return SUCCESS;
}

#else

int zephir_json_encode(zval *return_value, zval **return_value_ptr, zval *v, int opts TSRMLS_DC)
{
	zval zopts;
	zval *params[2];

	ZEPHIR_SINIT_VAR(zopts);
	ZVAL_LONG(&zopts, opts);

	params[0] = v;
	params[1] = &zopts;

	return zephir_return_call_function(return_value, NULL, ZEND_STRL("json_encode"), NULL, 2, params TSRMLS_CC);
}

int zephir_json_decode(zval *return_value, zval **return_value_ptr, zval *v, zend_bool assoc TSRMLS_DC)
{
	zval zassoc;
	zval *params[2];

	ZEPHIR_SINIT_VAR(zassoc);
	ZVAL_BOOL(&zassoc, assoc);

	params[0] = v;
	params[1] = &zassoc;

	return zephir_return_call_function(return_value, NULL, ZEND_STRL("json_decode"), NULL, 2, params TSRMLS_CC);
}

#endif /* ZEPHIR_USE_PHP_JSON */

void zephir_lcfirst(zval *return_value, zval *s)
{
	zval copy;
	char *c;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(s) != IS_STRING)) {
		zend_make_printable_zval(s, &copy, &use_copy);
		if (use_copy) {
			s = &copy;
		}
	}

	if (!Z_STRLEN_P(s)) {
		ZVAL_EMPTY_STRING(return_value);
	}
	else {
		ZVAL_STRINGL(return_value, Z_STRVAL_P(s), Z_STRLEN_P(s), 1);
		c = Z_STRVAL_P(return_value);
		*c = tolower((unsigned char)*c);
	}

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_ucfirst(zval *return_value, zval *s)
{
	zval copy;
	char *c;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(s) != IS_STRING)) {
		zend_make_printable_zval(s, &copy, &use_copy);
		if (use_copy) {
			s = &copy;
		}
	}

	if (!Z_STRLEN_P(s)) {
		ZVAL_EMPTY_STRING(return_value);
	}
	else {
		ZVAL_STRINGL(return_value, Z_STRVAL_P(s), Z_STRLEN_P(s), 1);
		c = Z_STRVAL_P(return_value);
		*c = toupper((unsigned char)*c);
	}

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

int zephir_http_build_query(zval *return_value, zval *params, char *sep TSRMLS_DC)
{
	if (Z_TYPE_P(params) == IS_ARRAY || Z_TYPE_P(params) == IS_OBJECT) {
		smart_str formstr = { NULL, 0, 0 };
		int res;

#if PHP_VERSION_ID < 50400
		res = php_url_encode_hash_ex(HASH_OF(params), &formstr, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(params) == IS_OBJECT ? params : NULL), sep TSRMLS_CC);
#else
		res = php_url_encode_hash_ex(HASH_OF(params), &formstr, NULL, 0, NULL, 0, NULL, 0, (Z_TYPE_P(params) == IS_OBJECT ? params : NULL), sep, PHP_QUERY_RFC1738 TSRMLS_CC);
#endif

		if (res == SUCCESS) {
			if (!formstr.c) {
				ZVAL_EMPTY_STRING(return_value);
			}
			else {
				smart_str_0(&formstr);
				ZVAL_STRINGL(return_value, formstr.c, formstr.len, 0);
			}

			return SUCCESS;
		}

		smart_str_free(&formstr);
		ZVAL_FALSE(return_value);
	}
	else {
		ZVAL_NULL(return_value);
	}

	return FAILURE;
}

void zephir_htmlspecialchars(zval *return_value, zval *string, zval *quoting, zval *charset TSRMLS_DC)
{
	zval copy;
	char *escaped, *cs;
	int qs, use_copy = 0;
#if PHP_VERSION_ID < 50400
	int escaped_len;
#else
	size_t escaped_len;
#endif

	if (unlikely(Z_TYPE_P(string) != IS_STRING)) {
		zend_make_printable_zval(string, &copy, &use_copy);
		if (use_copy) {
			string = &copy;
		}
	}

	cs = (charset && Z_TYPE_P(charset) == IS_STRING) ? Z_STRVAL_P(charset) : NULL;
	qs = (quoting && Z_TYPE_P(quoting) == IS_LONG)   ? Z_LVAL_P(quoting)   : ENT_COMPAT;

	escaped = php_escape_html_entities_ex((unsigned char *)(Z_STRVAL_P(string)), Z_STRLEN_P(string), &escaped_len, 0, qs, cs, 1 TSRMLS_CC);
	ZVAL_STRINGL(return_value, escaped, escaped_len, 0);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_htmlentities(zval *return_value, zval *string, zval *quoting, zval *charset TSRMLS_DC)
{
	zval copy;
	char *escaped, *cs;
	int qs, use_copy = 0;
#if PHP_VERSION_ID < 50400
	int escaped_len;
#else
	size_t escaped_len;
#endif

	if (unlikely(Z_TYPE_P(string) != IS_STRING)) {
		zend_make_printable_zval(string, &copy, &use_copy);
		if (use_copy) {
			string = &copy;
		}
	}

	cs = (charset && Z_TYPE_P(charset) == IS_STRING) ? Z_STRVAL_P(charset) : NULL;
	qs = (quoting && Z_TYPE_P(quoting) == IS_LONG)   ? Z_LVAL_P(quoting)   : ENT_COMPAT;

	escaped = php_escape_html_entities_ex((unsigned char *)(Z_STRVAL_P(string)), Z_STRLEN_P(string), &escaped_len, 1, qs, cs, 1 TSRMLS_CC);
	ZVAL_STRINGL(return_value, escaped, escaped_len, 0);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_strval(zval *return_value, zval *v)
{
	zval copy;
	int use_copy = 0;

	zend_make_printable_zval(v, &copy, &use_copy);
	if (use_copy) {
		zval *tmp = &copy;
		ZVAL_ZVAL(return_value, tmp, 0, 0);
	}
	else {
		ZVAL_ZVAL(return_value, v, 1, 0);
	}
}

void zephir_date(zval *return_value, zval *format, zval *timestamp TSRMLS_DC)
{
	long int ts;
	zval copy;
	int use_copy = 0;
	char *formatted;

	if (unlikely(Z_TYPE_P(format) != IS_STRING)) {
		zend_make_printable_zval(format, &copy, &use_copy);
		if (use_copy) {
			format = &copy;
		}
	}

	ts = (timestamp) ? zephir_get_intval(timestamp) : time(NULL);

	formatted = php_format_date(Z_STRVAL_P(format), Z_STRLEN_P(format), ts, 1 TSRMLS_CC);
	ZVAL_STRING(return_value, formatted, 0);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_addslashes(zval *return_value, zval *str TSRMLS_DC)
{
	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(str) != IS_STRING)) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	ZVAL_STRING(return_value, php_addslashes(Z_STRVAL_P(str), Z_STRLEN_P(str), &Z_STRLEN_P(return_value), 0 TSRMLS_CC), 0);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_stripslashes(zval *return_value, zval *str TSRMLS_DC)
{
	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(str) != IS_STRING)) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	ZVAL_STRINGL(return_value, Z_STRVAL_P(str), Z_STRLEN_P(str), 1);
	php_stripslashes(Z_STRVAL_P(return_value), &Z_STRLEN_P(return_value) TSRMLS_CC);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

void zephir_stripcslashes(zval *return_value, zval *str TSRMLS_DC)
{

	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(str) != IS_STRING)) {
		zend_make_printable_zval(str, &copy, &use_copy);
		if (use_copy) {
			str = &copy;
		}
	}

	ZVAL_STRINGL(return_value, Z_STRVAL_P(str), Z_STRLEN_P(str), 1);
	php_stripcslashes(Z_STRVAL_P(return_value), &Z_STRLEN_P(return_value));

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}

#if PHP_VERSION_ID < 50400

const char* zend_new_interned_string(const char *arKey, int nKeyLength, int free_src TSRMLS_DC)
{
	return arKey;
}

#endif




#ifdef HAVE_CONFIG_H
#endif

#include <ctype.h>




void zephir_filter_alphanum(zval *return_value, zval *param) {

	unsigned int i;
	unsigned char ch;
	smart_str filtered_str = {0};
	zval copy;
	int use_copy = 0;

	if (Z_TYPE_P(param) != IS_STRING) {
		zend_make_printable_zval(param, &copy, &use_copy);
		if (use_copy) {
			param = &copy;
		}
	}

	for (i = 0; i < Z_STRLEN_P(param); i++) {
		ch = Z_STRVAL_P(param)[i];
		if (ch == '\0') {
			break;
		}
		if (isalnum(ch)) {
			smart_str_appendc(&filtered_str, ch);
		}
	}

	if (use_copy) {
		zval_dtor(param);
	}

	smart_str_0(&filtered_str);

	if (filtered_str.c) {
		RETURN_STRINGL(filtered_str.c, filtered_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}
}

void zephir_filter_identifier(zval *return_value, zval *param){

	unsigned int i;
	unsigned char ch;
	zval copy;
	smart_str filtered_str = {0};
	int use_copy = 0;

	if (Z_TYPE_P(param) != IS_STRING) {
		zend_make_printable_zval(param, &copy, &use_copy);
		if (use_copy) {
			param = &copy;
		}
	}

	for (i = 0; i < Z_STRLEN_P(param); i++) {
		ch = Z_STRVAL_P(param)[i];
		if (ch == '\0') {
			break;
		}
		if (isalnum(ch) || ch == '_') {
			smart_str_appendc(&filtered_str, ch);
		}
	}

	if (use_copy) {
		zval_dtor(param);
	}

	smart_str_0(&filtered_str);

	if (filtered_str.c) {
		RETURN_STRINGL(filtered_str.c, filtered_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}

}

void zephir_is_basic_charset(zval *return_value, const zval *param){

	unsigned int i;
	unsigned int ch;
	int iso88591 = 0;

	for (i = 0; i < Z_STRLEN_P(param); i++) {
		ch = Z_STRVAL_P(param)[i];
		if (ch != '\0') {
			if (ch == 172 || (ch >= 128 && ch <= 159)) {
				continue;
			}
			if (ch >= 160 && ch <= 255) {
				iso88591 = 1;
				continue;
			}
		}
		RETURN_FALSE;
	}

	if (!iso88591) {
		RETURN_STRING("ASCII", 1);
	}

	RETURN_STRING("ISO-8859-1", 1);
}

static long zephir_unpack(char *data, int size, int issigned, int *map)
{
	long result;
	char *cresult = (char *) &result;
	int i;

	result = issigned ? -1 : 0;

	for (i = 0; i < size; i++) {
		cresult[map[i]] = *data++;
	}

	return result;
}

static inline char *zephir_longtohex(unsigned long value) {

	static char digits[] = "0123456789abcdef";
	char buf[(sizeof(unsigned long) << 3) + 1];
	char *ptr, *end;

	end = ptr = buf + sizeof(buf) - 1;
	*ptr = '\0';
	do {
		*--ptr = digits[value & 0x0F];
		value >>= 4;
	} while (ptr > buf && value);

	return estrndup(ptr, end - ptr);
}

void zephir_escape_multi(zval *return_value, zval *param, const char *escape_char, unsigned int escape_length, char escape_extra, int use_whitelist) {

	unsigned int i;
	zval copy;
	smart_str escaped_str = {0};
	char machine_little_endian, *hex;
	int big_endian_long_map[4];
	int use_copy = 0, machine_endian_check = 1;
	int issigned = 0;
	long value;

	if (Z_TYPE_P(param) != IS_STRING) {
		zend_make_printable_zval(param, &copy, &use_copy);
		if (use_copy) {
			param = &copy;
		}
	}

	if (Z_STRLEN_P(param) <= 0) {
		RETURN_FALSE;
	}

	machine_little_endian = ((char *) &machine_endian_check)[0];
	if (machine_little_endian) {
		big_endian_long_map[0] = 3;
		big_endian_long_map[1] = 2;
		big_endian_long_map[2] = 1;
		big_endian_long_map[3] = 0;
	} else {
		int size = sizeof(Z_LVAL_P(param));
		big_endian_long_map[0] = size - 4;
		big_endian_long_map[1] = size - 3;
		big_endian_long_map[2] = size - 2;
		big_endian_long_map[3] = size - 1;
	}

	if ((Z_STRLEN_P(param) % 4) != 0) {
		RETURN_FALSE;
	}

	for (i = 0; i < Z_STRLEN_P(param); i += 4) {

		issigned = Z_STRVAL_P(param)[i] & 0x80;

		value = 0;
		if (sizeof(long) > 4 && issigned) {
			value = ~INT_MAX;
		}

		value |= zephir_unpack(&Z_STRVAL_P(param)[i], 4, issigned, big_endian_long_map);
		if (sizeof(long) > 4) {
			value = (unsigned int) value;
		}

		if (value == '\0') {
			RETURN_FALSE;
		}

		if (value < 256 && isalnum(value)) {
			smart_str_appendc(&escaped_str, (unsigned char) value);
			continue;
		}

		if (use_whitelist) {
			switch (value) {
				case ' ':
				case '/':
				case '*':
				case '+':
				case '-':
				case '\t':
				case '\n':
				case '^':
				case '$':
				case '!':
				case '?':
				case '\\':
				case '#':
				case '}':
				case '{':
				case ')':
				case '(':
				case ']':
				case '[':
				case '.':
				case ',':
				case ':':
				case ';':
				case '_':
				case '|':
					smart_str_appendc(&escaped_str, (unsigned char) value);
					continue;
			}
		}

		hex = zephir_longtohex(value);

		smart_str_appendl(&escaped_str, escape_char, escape_length);
		smart_str_appendl(&escaped_str, hex, strlen(hex));
		if (escape_extra != '\0') {
			smart_str_appendc(&escaped_str, escape_extra);
		}

		efree(hex);
	}

	if (use_copy) {
		zval_dtor(param);
	}

	smart_str_0(&escaped_str);

	if (escaped_str.c) {
		RETURN_STRINGL(escaped_str.c, escaped_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}

}

void zephir_escape_css(zval *return_value, zval *param) {
	zephir_escape_multi(return_value, param, "\\", sizeof("\\")-1, ' ', 0);
}

void zephir_escape_js(zval *return_value, zval *param) {
	zephir_escape_multi(return_value, param, "\\x", sizeof("\\x")-1, '\0', 1);
}

void zephir_escape_htmlattr(zval *return_value, zval *param) {
	zephir_escape_multi(return_value, param, "&#x", sizeof("&#x")-1, ';', 1);
}

void zephir_escape_html(zval *return_value, zval *str, zval *quote_style, zval *charset TSRMLS_DC) {

	#if PHP_VERSION_ID < 50400
	int length;
	#else
	size_t length;
	#endif

	char *escaped;

	if (Z_TYPE_P(str) != IS_STRING) {
		/* Nothing to escape */
		RETURN_ZVAL(str, 1, 0);
	}

	if (Z_TYPE_P(quote_style) != IS_LONG) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid quote_style supplied for zephir_escape_html()");
		RETURN_ZVAL(str, 1, 0);
	}

	if (Z_TYPE_P(charset) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid charset supplied for zephir_escape_html()");
		RETURN_ZVAL(str, 1, 0);
	}

	escaped = php_escape_html_entities((unsigned char*) Z_STRVAL_P(str), Z_STRLEN_P(str), &length, 0, Z_LVAL_P(quote_style), Z_STRVAL_P(charset) TSRMLS_CC);

	RETURN_STRINGL(escaped, length, 0);
}




#ifdef HAVE_CONFIG_H
#endif



void zephir_make_printable_zval(zval *expr, zval *expr_copy, int *use_copy){
	zend_make_printable_zval(expr, expr_copy, use_copy);
	if (use_copy) {
		Z_SET_REFCOUNT_P(expr_copy, 1);
		Z_UNSET_ISREF_P(expr_copy);
	}
}

int zephir_and_function(zval *result, zval *left, zval *right){
	int istrue = zend_is_true(left) && zend_is_true(right);
	ZVAL_BOOL(result, istrue);
	return SUCCESS;
}

void zephir_concat_self(zval **left, zval *right TSRMLS_DC){

	zval left_copy, right_copy;
	uint length;
	int use_copy_left = 0, use_copy_right = 0;

	if (Z_TYPE_P(right) != IS_STRING) {
		zephir_make_printable_zval(right, &right_copy, &use_copy_right);
		if (use_copy_right) {
			right = &right_copy;
		}
	}

	if (Z_TYPE_PP(left) == IS_NULL) {

		Z_STRVAL_PP(left) = emalloc(Z_STRLEN_P(right) + 1);
		memcpy(Z_STRVAL_PP(left), Z_STRVAL_P(right), Z_STRLEN_P(right));
		Z_STRVAL_PP(left)[Z_STRLEN_P(right)] = 0;
		Z_STRLEN_PP(left) = Z_STRLEN_P(right);
		Z_TYPE_PP(left) = IS_STRING;

		if (use_copy_right) {
			zval_dtor(&right_copy);
		}

		return;
	}

	if (Z_TYPE_PP(left) != IS_STRING) {
		zephir_make_printable_zval(*left, &left_copy, &use_copy_left);
		if (use_copy_left) {
			ZEPHIR_CPY_WRT_CTOR(*left, (&left_copy));
		}
	}

	length = Z_STRLEN_PP(left) + Z_STRLEN_P(right);
	Z_STRVAL_PP(left) = erealloc(Z_STRVAL_PP(left), length + 1);

	memcpy(Z_STRVAL_PP(left) + Z_STRLEN_PP(left), Z_STRVAL_P(right), Z_STRLEN_P(right));
	Z_STRVAL_PP(left)[length] = 0;
	Z_STRLEN_PP(left) = length;
	Z_TYPE_PP(left) = IS_STRING;

	if (use_copy_left) {
		zval_dtor(&left_copy);
	}

	if (use_copy_right) {
		zval_dtor(&right_copy);
	}
}

void zephir_concat_self_str(zval **left, const char *right, int right_length TSRMLS_DC){

	zval left_copy;
	uint length;
	int use_copy = 0;

	if (Z_TYPE_PP(left) == IS_NULL) {

		Z_STRVAL_PP(left) = emalloc(right_length + 1);
		memcpy(Z_STRVAL_PP(left), right, right_length);
		Z_STRVAL_PP(left)[right_length] = 0;
		Z_STRLEN_PP(left) = right_length;
		Z_TYPE_PP(left) = IS_STRING;

		return;
	}

	if (Z_TYPE_PP(left) != IS_STRING) {
		zephir_make_printable_zval(*left, &left_copy, &use_copy);
		if (use_copy) {
			ZEPHIR_CPY_WRT_CTOR(*left, (&left_copy));
		}
	}

	length = Z_STRLEN_PP(left) + right_length;
	Z_STRVAL_PP(left) = erealloc(Z_STRVAL_PP(left), length + 1);

	memcpy(Z_STRVAL_PP(left) + Z_STRLEN_PP(left), right, right_length);
	Z_STRVAL_PP(left)[length] = 0;
	Z_STRLEN_PP(left) = length;
	Z_TYPE_PP(left) = IS_STRING;

	if (use_copy) {
		zval_dtor(&left_copy);
	}
}

void zephir_concat_self_long(zval **left, const long right TSRMLS_DC) {

	zval left_copy;
	uint length;
	char *right_char;
	int use_copy = 0, right_length = 0;

	right_length = zephir_spprintf(&right_char, 0, "%ld", right);

	if (Z_TYPE_PP(left) == IS_NULL) {
		Z_STRVAL_PP(left) = emalloc(right_length + 1);
		if (right_length > 0) {
			memcpy(Z_STRVAL_PP(left), right_char, right_length);
		} else {
			memcpy(Z_STRVAL_PP(left), "", 0);
		}
		Z_STRVAL_PP(left)[right_length] = 0;
		Z_STRLEN_PP(left) = right_length;
		Z_TYPE_PP(left) = IS_STRING;
		return;
	}

	if (Z_TYPE_PP(left) != IS_STRING) {
		zephir_make_printable_zval(*left, &left_copy, &use_copy);
		if (use_copy) {
			ZEPHIR_CPY_WRT_CTOR(*left, (&left_copy));
		}
	}

	if (right_length > 0) {
		length = Z_STRLEN_PP(left) + right_length;
		Z_STRVAL_PP(left) = erealloc(Z_STRVAL_PP(left), length + 1);
		memcpy(Z_STRVAL_PP(left) + Z_STRLEN_PP(left), right_char, right_length);
		Z_STRVAL_PP(left)[length] = 0;
		Z_STRLEN_PP(left) = length;
		Z_TYPE_PP(left) = IS_STRING;
	}

	if (use_copy) {
		zval_dtor(&left_copy);
	}
}

void zephir_concat_self_char(zval **left, unsigned char right TSRMLS_DC) {

	zval left_copy;
	int use_copy = 0;

	if (Z_TYPE_PP(left) == IS_NULL) {
		Z_STRVAL_PP(left) = emalloc(2);
		Z_STRVAL_PP(left)[0] = right;
		Z_STRVAL_PP(left)[1] = 0;
		Z_STRLEN_PP(left) = 1;
		Z_TYPE_PP(left) = IS_STRING;
		return;
	}

	if (Z_TYPE_PP(left) != IS_STRING) {
		zephir_make_printable_zval(*left, &left_copy, &use_copy);
		if (use_copy) {
			ZEPHIR_CPY_WRT_CTOR(*left, (&left_copy));
		}
	}

	Z_STRLEN_PP(left)++;
	Z_STRVAL_PP(left) = erealloc(Z_STRVAL_PP(left), Z_STRLEN_PP(left) + 1);
	Z_STRVAL_PP(left)[Z_STRLEN_PP(left) - 1] = right;
	Z_STRVAL_PP(left)[Z_STRLEN_PP(left)] = 0;
	Z_TYPE_PP(left) = IS_STRING;

	if (use_copy) {
		zval_dtor(&left_copy);
	}
}

int zephir_compare_strict_string(zval *op1, const char *op2, int op2_length) {

	switch (Z_TYPE_P(op1)) {
		case IS_STRING:
			if (!Z_STRLEN_P(op1) && !op2_length) {
				return 1;
			}
			if (Z_STRLEN_P(op1) != op2_length) {
				return 0;
			}
			return !zend_binary_strcmp(Z_STRVAL_P(op1), Z_STRLEN_P(op1), op2, op2_length);
		case IS_NULL:
			return !zend_binary_strcmp("", 0, op2, op2_length);
		case IS_BOOL:
			if (!Z_BVAL_P(op1)) {
				return !zend_binary_strcmp("0", strlen("0"), op2, op2_length);
			} else {
				return !zend_binary_strcmp("1", strlen("1"), op2, op2_length);
			}
	}

	return 0;
}

int zephir_compare_strict_long(zval *op1, long op2 TSRMLS_DC) {

	int bool_result;

	switch (Z_TYPE_P(op1)) {
		case IS_LONG:
			return Z_LVAL_P(op1) == op2;
		case IS_DOUBLE:
			return Z_DVAL_P(op1) == (double) op2;
		case IS_NULL:
			return 0 == op2;
		case IS_BOOL:
			if (Z_BVAL_P(op1)) {
				return 1 == op2;
			} else {
				return 0 == op2;
			}
		default:
			{
				zval result, op2_tmp;
				ZVAL_LONG(&op2_tmp, op2);
				is_equal_function(&result, op1, &op2_tmp TSRMLS_CC);
				bool_result = Z_BVAL(result);
				return bool_result;
			}
	}

	return 0;
}

int zephir_compare_strict_double(zval *op1, double op2 TSRMLS_DC) {

	int bool_result;

	switch (Z_TYPE_P(op1)) {
		case IS_LONG:
			return Z_LVAL_P(op1) == (long) op2;
		case IS_DOUBLE:
			return Z_DVAL_P(op1) == op2;
		case IS_NULL:
			return 0 == op2;
		case IS_BOOL:
			if (Z_BVAL_P(op1)) {
				return 1 == op2;
			} else {
				return 0 == op2;
			}
		default:
			{
				zval result, op2_tmp;
				ZVAL_DOUBLE(&op2_tmp, op2);
				is_equal_function(&result, op1, &op2_tmp TSRMLS_CC);
				bool_result = Z_BVAL(result);
				return bool_result;
			}
	}

	return 0;
}

int zephir_compare_strict_bool(zval *op1, zend_bool op2 TSRMLS_DC) {

	int bool_result;

	switch (Z_TYPE_P(op1)) {
		case IS_LONG:
			return (Z_LVAL_P(op1) ? 1 : 0) == op2;
		case IS_DOUBLE:
			return (Z_DVAL_P(op1) ? 1 : 0) == op2;
		case IS_NULL:
			return 0 == op2;
		case IS_BOOL:
			if (Z_BVAL_P(op1)) {
				return 1 == op2;
			} else {
				return 0 == op2;
			}
		default:
			{
				zval result, op2_tmp;
				ZVAL_BOOL(&op2_tmp, op2);
				is_equal_function(&result, op1, &op2_tmp TSRMLS_CC);
				bool_result = Z_BVAL(result);
				return bool_result;
			}
	}

	return 0;
}

int zephir_add_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) {
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = add_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}

void zephir_negate(zval *z TSRMLS_DC) {
	while (1) {
		switch (Z_TYPE_P(z)) {
			case IS_LONG:
			case IS_BOOL:
				ZVAL_LONG(z, -Z_LVAL_P(z));
				return;

			case IS_DOUBLE:
				ZVAL_DOUBLE(z, -Z_DVAL_P(z));
				return;

			case IS_NULL:
				ZVAL_LONG(z, 0);
				return;

			default:
				convert_scalar_to_number(z TSRMLS_CC);
				assert(Z_TYPE_P(z) == IS_LONG || Z_TYPE_P(z) == IS_DOUBLE);
		}
	}
}

void zephir_convert_to_object(zval *op) {
    convert_to_object(op);
}

void zephir_cast(zval *result, zval *var, zend_uint type){

	ZVAL_ZVAL(result, var, 1, 0);

	switch (type) {
		case IS_STRING:
			convert_to_string(result);
			break;
		case IS_LONG:
			convert_to_long(result);
			break;
		case IS_DOUBLE:
			convert_to_double(result);
			break;
		/*case IS_BOOL:
			convert_to_bool(result);
			break;*/
		case IS_ARRAY:
			if (Z_TYPE_P(result) != IS_ARRAY) {
				convert_to_array(result);
			}
			break;
	}

}

long zephir_get_intval_ex(const zval *op) {

	switch (Z_TYPE_P(op)) {
        case IS_ARRAY:
            return zend_hash_num_elements(Z_ARRVAL_P(op)) ? 1 : 0;
            break;

#if PHP_VERSION_ID > 50400
	    case IS_CALLABLE:
#endif
	    case IS_RESOURCE:
	    case IS_OBJECT:
	        return 1;

		case IS_LONG:
			return Z_LVAL_P(op);

		case IS_BOOL:
			return Z_BVAL_P(op);

		case IS_DOUBLE:
			return (long) Z_DVAL_P(op);

		case IS_STRING: {
			long long_value = 0;
			double double_value = 0;

			ASSUME(Z_STRVAL_P(op) != NULL);
			zend_uchar type = is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op), &long_value, &double_value, 0);
			if (type == IS_LONG) {
				return long_value;
			}
			if (type == IS_DOUBLE) {
				return (long) double_value;
			}
			return 0;
		}
	}

	return 0;
}

double zephir_get_doubleval_ex(const zval *op) {

	int type;
	long long_value = 0;
	double double_value = 0;

	switch (Z_TYPE_P(op)) {
        case IS_ARRAY:
            return zend_hash_num_elements(Z_ARRVAL_P(op)) ? (double) 1 : 0;
            break;
#if PHP_VERSION_ID > 50400
	    case IS_CALLABLE:
#endif
	    case IS_RESOURCE:
	    case IS_OBJECT:
	        return (double) 1;
		case IS_LONG:
			return (double) Z_LVAL_P(op);
		case IS_BOOL:
			return (double) Z_BVAL_P(op);
		case IS_DOUBLE:
			return Z_DVAL_P(op);
		case IS_STRING:
			if ((type = is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op), &long_value, &double_value, 0))) {
				if (type == IS_LONG) {
					return (double) long_value;
				} else {
					if (type == IS_DOUBLE) {
						return double_value;
					} else {
						return 0;
					}
				}
			}
	}

	return 0;
}

zend_bool zephir_get_boolval_ex(const zval *op) {

	int type;
	long long_value = 0;
	double double_value = 0;

	switch (Z_TYPE_P(op)) {
        case IS_ARRAY:
            return zend_hash_num_elements(Z_ARRVAL_P(op)) ? (zend_bool) 1 : 0;
            break;
#if PHP_VERSION_ID > 50400
	    case IS_CALLABLE:
#endif
	    case IS_RESOURCE:
	    case IS_OBJECT:
	        return (zend_bool) 1;
		case IS_LONG:
			return (Z_LVAL_P(op) ? (zend_bool) 1 : 0);
		case IS_BOOL:
			return Z_BVAL_P(op);
		case IS_DOUBLE:
			return (Z_DVAL_P(op) ? (zend_bool) 1 : 0);
		case IS_STRING:
			if ((type = is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op), &long_value, &double_value, 0))) {
				if (type == IS_LONG) {
					return (long_value ? (zend_bool) 1 : 0);
				} else {
					if (type == IS_DOUBLE) {
						return (double_value ? (zend_bool) 1 : 0);
					} else {
						return 0;
					}
				}
			}
	}

	return 0;
}

int zephir_is_numeric_ex(const zval *op) {

	int type;

	switch (Z_TYPE_P(op)) {
		case IS_LONG:
			return 1;
		case IS_BOOL:
			return 0;
		case IS_DOUBLE:
			return 1;
		case IS_STRING:
			if ((type = is_numeric_string(Z_STRVAL_P(op), Z_STRLEN_P(op), NULL, NULL, 0))) {
				if (type == IS_LONG || type == IS_DOUBLE) {
					return 1;
				}
			}
	}

	return 0;
}

int zephir_is_equal(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	#if PHP_VERSION_ID < 50400
	is_equal_function(&result, op1, op2 TSRMLS_CC);
	return Z_BVAL(result);
	#else
	return fast_equal_function(&result, op1, op2 TSRMLS_CC);
	#endif
}

int zephir_less(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	#if PHP_VERSION_ID < 50400
	is_smaller_function(&result, op1, op2 TSRMLS_CC);
	return Z_BVAL(result);
	#else
	return fast_is_smaller_function(&result, op1, op2 TSRMLS_CC);
	#endif
}

int zephir_less_equal(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	is_smaller_or_equal_function(&result, op1, op2 TSRMLS_CC);
	return Z_BVAL(result);
}

int zephir_less_long(zval *op1, long op2 TSRMLS_DC) {
	zval result, op2_zval;
	ZVAL_LONG(&op2_zval, op2);
	is_smaller_function(&result, op1, &op2_zval TSRMLS_CC);
	return Z_BVAL(result);
}

int zephir_less_equal_long(zval *op1, long op2 TSRMLS_DC) {
	zval result, op2_zval;
	ZVAL_LONG(&op2_zval, op2);
	is_smaller_or_equal_function(&result, op1, &op2_zval TSRMLS_CC);
	return Z_BVAL(result);
}

int zephir_greater(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	is_smaller_or_equal_function(&result, op1, op2 TSRMLS_CC);
	return !Z_BVAL(result);
}

int zephir_greater_long(zval *op1, long op2 TSRMLS_DC) {
	zval result, op2_zval;
	ZVAL_LONG(&op2_zval, op2);
	is_smaller_or_equal_function(&result, op1, &op2_zval TSRMLS_CC);
	return !Z_BVAL(result);
}

int zephir_greater_equal(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	is_smaller_function(&result, op1, op2 TSRMLS_CC);
	return !Z_BVAL(result);
}

int zephir_greater_equal_long(zval *op1, long op2 TSRMLS_DC) {
	zval result, op2_zval;
	ZVAL_LONG(&op2_zval, op2);
	is_smaller_function(&result, op1, &op2_zval TSRMLS_CC);
	return !Z_BVAL(result);
}

int zephir_is_identical(zval *op1, zval *op2 TSRMLS_DC) {
	zval result;
	is_identical_function(&result, op1, op2 TSRMLS_CC);
	return Z_BVAL(result);
}

int zephir_bitwise_and_function(zval *result, zval *op1, zval *op2 TSRMLS_DC){
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = bitwise_and_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}

int zephir_bitwise_or_function(zval *result, zval *op1, zval *op2 TSRMLS_DC){
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = bitwise_or_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}

int zephir_bitwise_xor_function(zval *result, zval *op1, zval *op2 TSRMLS_DC){
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = bitwise_xor_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}

int zephir_shift_left_function(zval *result, zval *op1, zval *op2 TSRMLS_DC){
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = shift_left_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}

int zephir_shift_right_function(zval *result, zval *op1, zval *op2 TSRMLS_DC){
	int status;
	int ref_count = Z_REFCOUNT_P(result);
	int is_ref = Z_ISREF_P(result);
	status = shift_right_function(result, op1, op2 TSRMLS_CC);
	Z_SET_REFCOUNT_P(result, ref_count);
	Z_SET_ISREF_TO_P(result, is_ref);
	return status;
}




#ifdef HAVE_CONFIG_H
#endif



void zephir_concat_ss(zval **result, const char *op1, zend_uint op1_len, const char *op2, zend_uint op2_len, int self_var TSRMLS_DC){

	zval result_copy;
	int use_copy = 0;
	uint offset = 0, length;

	length = op1_len + op2_len;
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, op2, op2_len);
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_sv(zval **result, const char *op1, zend_uint op1_len, zval *op2, int self_var TSRMLS_DC){

	zval result_copy, op2_copy;
	int use_copy = 0, use_copy2 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_svs(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, int self_var TSRMLS_DC){

	zval result_copy, op2_copy;
	int use_copy = 0, use_copy2 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + op3_len;
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2), op3, op3_len);
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_svsv(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, zval *op4, int self_var TSRMLS_DC){

	zval result_copy, op2_copy, op4_copy;
	int use_copy = 0, use_copy2 = 0, use_copy4 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	if (Z_TYPE_P(op4) != IS_STRING) {
	   zend_make_printable_zval(op4, &op4_copy, &use_copy4);
	   if (use_copy4) {
	       op4 = &op4_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + op3_len + Z_STRLEN_P(op4);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2), op3, op3_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2) + op3_len, Z_STRVAL_P(op4), Z_STRLEN_P(op4));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy4) {
	   zval_dtor(op4);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_svsvs(zval **result, const char *op1, zend_uint op1_len, zval *op2, const char *op3, zend_uint op3_len, zval *op4, const char *op5, zend_uint op5_len, int self_var TSRMLS_DC){

	zval result_copy, op2_copy, op4_copy;
	int use_copy = 0, use_copy2 = 0, use_copy4 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	if (Z_TYPE_P(op4) != IS_STRING) {
	   zend_make_printable_zval(op4, &op4_copy, &use_copy4);
	   if (use_copy4) {
	       op4 = &op4_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + op3_len + Z_STRLEN_P(op4) + op5_len;
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2), op3, op3_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2) + op3_len, Z_STRVAL_P(op4), Z_STRLEN_P(op4));
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2) + op3_len + Z_STRLEN_P(op4), op5, op5_len);
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy4) {
	   zval_dtor(op4);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_svv(zval **result, const char *op1, zend_uint op1_len, zval *op2, zval *op3, int self_var TSRMLS_DC){

	zval result_copy, op2_copy, op3_copy;
	int use_copy = 0, use_copy2 = 0, use_copy3 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = op1_len + Z_STRLEN_P(op2) + Z_STRLEN_P(op3);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, op1, op1_len);
	memcpy(Z_STRVAL_PP(result) + offset + op1_len, Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	memcpy(Z_STRVAL_PP(result) + offset + op1_len + Z_STRLEN_P(op2), Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vs(zval **result, zval *op1, const char *op2, zend_uint op2_len, int self_var TSRMLS_DC){

	zval result_copy, op1_copy;
	int use_copy = 0, use_copy1 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len;
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op3_copy;
	int use_copy = 0, use_copy1 = 0, use_copy3 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vsvs(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op3_copy;
	int use_copy = 0, use_copy1 = 0, use_copy3 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len;
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3), op4, op4_len);
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vsvsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, zval *op5, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op3_copy, op5_copy;
	int use_copy = 0, use_copy1 = 0, use_copy3 = 0, use_copy5 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	if (Z_TYPE_P(op5) != IS_STRING) {
	   zend_make_printable_zval(op5, &op5_copy, &use_copy5);
	   if (use_copy5) {
	       op5 = &op5_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3), op4, op4_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len, Z_STRVAL_P(op5), Z_STRLEN_P(op5));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy5) {
	   zval_dtor(op5);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vsvsvsv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, const char *op4, zend_uint op4_len, zval *op5, const char *op6, zend_uint op6_len, zval *op7, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op3_copy, op5_copy, op7_copy;
	int use_copy = 0, use_copy1 = 0, use_copy3 = 0, use_copy5 = 0, use_copy7 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	if (Z_TYPE_P(op5) != IS_STRING) {
	   zend_make_printable_zval(op5, &op5_copy, &use_copy5);
	   if (use_copy5) {
	       op5 = &op5_copy;
	   }
	}

	if (Z_TYPE_P(op7) != IS_STRING) {
	   zend_make_printable_zval(op7, &op7_copy, &use_copy7);
	   if (use_copy7) {
	       op7 = &op7_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len + Z_STRLEN_P(op7);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3), op4, op4_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len, Z_STRVAL_P(op5), Z_STRLEN_P(op5));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5), op6, op6_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + op4_len + Z_STRLEN_P(op5) + op6_len, Z_STRVAL_P(op7), Z_STRLEN_P(op7));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy5) {
	   zval_dtor(op5);
	}

	if (use_copy7) {
	   zval_dtor(op7);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vsvv(zval **result, zval *op1, const char *op2, zend_uint op2_len, zval *op3, zval *op4, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op3_copy, op4_copy;
	int use_copy = 0, use_copy1 = 0, use_copy3 = 0, use_copy4 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	if (Z_TYPE_P(op4) != IS_STRING) {
	   zend_make_printable_zval(op4, &op4_copy, &use_copy4);
	   if (use_copy4) {
	       op4 = &op4_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3) + Z_STRLEN_P(op4);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), op2, op2_len);
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len, Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + op2_len + Z_STRLEN_P(op3), Z_STRVAL_P(op4), Z_STRLEN_P(op4));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy4) {
	   zval_dtor(op4);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vv(zval **result, zval *op1, zval *op2, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op2_copy;
	int use_copy = 0, use_copy1 = 0, use_copy2 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_vvv(zval **result, zval *op1, zval *op2, zval *op3, int self_var TSRMLS_DC){

	zval result_copy, op1_copy, op2_copy, op3_copy;
	int use_copy = 0, use_copy1 = 0, use_copy2 = 0, use_copy3 = 0;
	uint offset = 0, length;

	if (Z_TYPE_P(op1) != IS_STRING) {
	   zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	   if (use_copy1) {
	       op1 = &op1_copy;
	   }
	}

	if (Z_TYPE_P(op2) != IS_STRING) {
	   zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	   if (use_copy2) {
	       op2 = &op2_copy;
	   }
	}

	if (Z_TYPE_P(op3) != IS_STRING) {
	   zend_make_printable_zval(op3, &op3_copy, &use_copy3);
	   if (use_copy3) {
	       op3 = &op3_copy;
	   }
	}

	length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2) + Z_STRLEN_P(op3);
	if (self_var) {

		if (Z_TYPE_PP(result) != IS_STRING) {
			zend_make_printable_zval(*result, &result_copy, &use_copy);
			if (use_copy) {
				ZEPHIR_CPY_WRT_CTOR(*result, (&result_copy));
			}
		}

		offset = Z_STRLEN_PP(result);
		length += offset;
		Z_STRVAL_PP(result) = (char *) erealloc(Z_STRVAL_PP(result), length + 1);

	} else {
		Z_STRVAL_PP(result) = (char *) emalloc(length + 1);
	}

	memcpy(Z_STRVAL_PP(result) + offset, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
	memcpy(Z_STRVAL_PP(result) + offset + Z_STRLEN_P(op1) + Z_STRLEN_P(op2), Z_STRVAL_P(op3), Z_STRLEN_P(op3));
	Z_STRVAL_PP(result)[length] = 0;
	Z_TYPE_PP(result) = IS_STRING;
	Z_STRLEN_PP(result) = length;

	if (use_copy1) {
	   zval_dtor(op1);
	}

	if (use_copy2) {
	   zval_dtor(op2);
	}

	if (use_copy3) {
	   zval_dtor(op3);
	}

	if (use_copy) {
	   zval_dtor(&result_copy);
	}

}

void zephir_concat_function(zval *result, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
#if PHP_VERSION_ID < 50400
	zval op1_copy, op2_copy;
	int use_copy1 = 0, use_copy2 = 0;

	if (Z_TYPE_P(op1) != IS_STRING) {
		zend_make_printable_zval(op1, &op1_copy, &use_copy1);
	}
	if (Z_TYPE_P(op2) != IS_STRING) {
		zend_make_printable_zval(op2, &op2_copy, &use_copy2);
	}

	if (use_copy1) {
		/* We have created a converted copy of op1. Therefore, op1 won't become the result so
		 * we have to free it.
		 */
		if (result == op1) {
			zval_dtor(op1);
		}
		op1 = &op1_copy;
	}
	if (use_copy2) {
		op2 = &op2_copy;
	}
	if (result==op1 && !IS_INTERNED(Z_STRVAL_P(op1))) {	/* special case, perform operations on result */
		uint res_len = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);

		if (Z_STRLEN_P(result) < 0 || (int) (Z_STRLEN_P(op1) + Z_STRLEN_P(op2)) < 0) {
			efree(Z_STRVAL_P(result));
			ZVAL_EMPTY_STRING(result);
			zend_error(E_ERROR, "String size overflow");
		}

		Z_STRVAL_P(result) = erealloc(Z_STRVAL_P(result), res_len+1);

		memcpy(Z_STRVAL_P(result)+Z_STRLEN_P(result), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
		Z_STRVAL_P(result)[res_len]=0;
		Z_STRLEN_P(result) = res_len;
	} else {
		int length = Z_STRLEN_P(op1) + Z_STRLEN_P(op2);
		char *buf = (char *) emalloc(length + 1);

		memcpy(buf, Z_STRVAL_P(op1), Z_STRLEN_P(op1));
		memcpy(buf + Z_STRLEN_P(op1), Z_STRVAL_P(op2), Z_STRLEN_P(op2));
		buf[length] = 0;
		ZVAL_STRINGL(result, buf, length, 0);
	}
	if (use_copy1) {
		zval_dtor(op1);
	}
	if (use_copy2) {
		zval_dtor(op2);
	}
#else
    concat_function(result, op1, op2 TSRMLS_CC);
#endif
}



#ifdef HAVE_CONFIG_H
#endif




#define PHP_STREAM_TO_ZVAL(stream, arg) \
	php_stream_from_zval_no_verify(stream, arg); \
	if (stream == NULL) {   \
		if (return_value) { \
			RETURN_FALSE;   \
		} else { \
			return; \
		} \
	}

int zephir_file_exists(zval *filename TSRMLS_DC){

	zval return_value;

	if (Z_TYPE_P(filename) != IS_STRING) {
		return FAILURE;
	}

	php_stat(Z_STRVAL_P(filename), (php_stat_len) Z_STRLEN_P(filename), FS_EXISTS, &return_value TSRMLS_CC);

	if (ZEPHIR_IS_FALSE((&return_value))) {
		return FAILURE;
	}

	if (ZEPHIR_IS_EMPTY((&return_value))) {
		return FAILURE;
	}

	return SUCCESS;
}

int zephir_compare_mtime(zval *filename1, zval *filename2 TSRMLS_DC){

	php_stream_statbuf statbuffer1, statbuffer2;

	if (Z_TYPE_P(filename1) != IS_STRING || Z_TYPE_P(filename2) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for compare_mtime()");
		return 0;
	}

	if (php_stream_stat_path_ex(Z_STRVAL_P(filename1), 0, &statbuffer1, NULL)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "mstat failed for %s", Z_STRVAL_P(filename1));
		return 0;
	}

	if (php_stream_stat_path_ex(Z_STRVAL_P(filename2), 0, &statbuffer2, NULL)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "mstat failed for %s", Z_STRVAL_P(filename2));
		return 0;
	}

	return (int) (statbuffer1.sb.st_mtime >= statbuffer2.sb.st_mtime);
}

void zephir_fast_filemtime(zval *return_value, zval *filename TSRMLS_DC){

	if (Z_TYPE_P(filename) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for fast_filemtime()");
		return;
	}

	php_stat(Z_STRVAL_P(filename), (php_stat_len) Z_STRLEN_P(filename), FS_MTIME, return_value TSRMLS_CC);
}

void zephir_fix_path(zval **return_value, zval *path, zval *directory_separator TSRMLS_DC) {

	if (Z_TYPE_P(path) != IS_STRING || Z_TYPE_P(directory_separator) != IS_STRING) {
		return;
	}

	if (Z_STRLEN_P(path) > 0 && Z_STRLEN_P(directory_separator) > 0) {
		if (Z_STRVAL_P(path)[Z_STRLEN_P(path) - 1] != Z_STRVAL_P(directory_separator)[0]) {
			ZEPHIR_CONCAT_VV(*return_value, path, directory_separator);
			return;
		}
	}

	zval_ptr_dtor(return_value);
	*return_value = path;
	Z_ADDREF_P(path);
}

void zephir_prepare_virtual_path(zval *return_value, zval *path, zval *virtual_separator TSRMLS_DC) {

	unsigned int i;
	unsigned char ch;
	smart_str virtual_str = {0};

	if (Z_TYPE_P(path) != IS_STRING || Z_TYPE_P(virtual_separator) != IS_STRING) {
		if (Z_TYPE_P(path) == IS_STRING) {
			RETURN_STRINGL(Z_STRVAL_P(path), Z_STRLEN_P(path), 1);
		} else {
			RETURN_EMPTY_STRING();
		}
		return;
	}

	for (i = 0; i < Z_STRLEN_P(path); i++) {
		ch = Z_STRVAL_P(path)[i];
		if (ch == '\0') {
			break;
		}
		if (ch == '/' || ch == '\\' || ch == ':') {
			smart_str_appendl(&virtual_str, Z_STRVAL_P(virtual_separator), Z_STRLEN_P(virtual_separator));
		}
		else {
			smart_str_appendc(&virtual_str, tolower(ch));
		}
	}

	smart_str_0(&virtual_str);

	if (virtual_str.c) {
		RETURN_STRINGL(virtual_str.c, virtual_str.len, 0);
	} else {
		RETURN_EMPTY_STRING();
	}
}

void zephir_unique_path_key(zval *return_value, zval *path TSRMLS_DC) {

	unsigned long h;
	char *strKey;

	if (Z_TYPE_P(path) != IS_STRING) {
		return;
	}

	h = zend_hash_func(Z_STRVAL_P(path), Z_STRLEN_P(path) + 1);

	strKey = emalloc(24);
	sprintf(strKey, "v%lu", h);

	RETURN_STRING(strKey, 0);
}

void zephir_realpath(zval *return_value, zval *filename TSRMLS_DC) {

	char resolved_path_buff[MAXPATHLEN];

	if (Z_TYPE_P(filename) != IS_STRING) {
		RETURN_FALSE;
	}

	if (strlen(Z_STRVAL_P(filename)) != Z_STRLEN_P(filename)) {
		RETURN_FALSE;
	}

	if (VCWD_REALPATH(Z_STRVAL_P(filename), resolved_path_buff)) {
		RETURN_STRING(resolved_path_buff, 1);
	}

	RETURN_FALSE;
}

void zephir_possible_autoload_filepath(zval *return_value, zval *prefix, zval *class_name, zval *virtual_separator, zval *separator TSRMLS_DC) {

	unsigned int i, length;
	unsigned char ch;
	smart_str virtual_str = {0};

	if (Z_TYPE_P(prefix) != IS_STRING || Z_TYPE_P(class_name) != IS_STRING || Z_TYPE_P(virtual_separator) != IS_STRING) {
		RETURN_FALSE;
	}

	length = Z_STRLEN_P(prefix);
	if (!length) {
		RETURN_FALSE;
	}

	if (length > Z_STRLEN_P(class_name)) {
		RETURN_FALSE;
	}

	if (separator) {
		if (Z_STRVAL_P(prefix)[Z_STRLEN_P(prefix) - 1] == Z_STRVAL_P(separator)[0]) {
			length--;
		}
	}

	for (i = length + 1; i < Z_STRLEN_P(class_name); i++) {

		ch = Z_STRVAL_P(class_name)[i];

		if (ch == '\0') {
			break;
		}

		if (ch == '\\') {
			smart_str_appendl(&virtual_str, Z_STRVAL_P(virtual_separator), Z_STRLEN_P(virtual_separator));
			continue;
		}

		if (separator) {
			if (ch == Z_STRVAL_P(separator)[0]) {
				smart_str_appendl(&virtual_str, Z_STRVAL_P(virtual_separator), Z_STRLEN_P(virtual_separator));
				continue;
			}
		}

		if ((ch == '_') || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
			smart_str_appendc(&virtual_str, ch);
			continue;
		}

		if (ch > 127) {
			smart_str_appendc(&virtual_str, ch);
			continue;
		}

	}

	smart_str_0(&virtual_str);

	if (virtual_str.len) {
		RETURN_STRINGL(virtual_str.c, virtual_str.len, 0);
	} else {
		smart_str_free(&virtual_str);
		RETURN_FALSE;
	}

}

void zephir_file_get_contents(zval *return_value, zval *filename TSRMLS_DC)
{

	char *contents;
	php_stream *stream;
	int len;
	long maxlen = PHP_STREAM_COPY_ALL;
	zval *zcontext = NULL;
	php_stream_context *context = NULL;

	if (Z_TYPE_P(filename) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_file_get_contents()");
		RETVAL_FALSE;
		return;
	}

	context = php_stream_context_from_zval(zcontext, 0);

	stream = php_stream_open_wrapper_ex(Z_STRVAL_P(filename), "rb", 0 | REPORT_ERRORS, NULL, context);
	if (!stream) {
		RETURN_FALSE;
	}

	if ((len = php_stream_copy_to_mem(stream, &contents, maxlen, 0)) > 0) {
		RETVAL_STRINGL(contents, len, 0);
	} else {
		if (len == 0) {
			RETVAL_EMPTY_STRING();
		} else {
			RETVAL_FALSE;
		}
	}

	php_stream_close(stream);
}

void zephir_file_put_contents(zval *return_value, zval *filename, zval *data TSRMLS_DC)
{
	php_stream *stream;
	int numbytes = 0, use_copy = 0;
	zval *zcontext = NULL;
	zval copy;
	php_stream_context *context = NULL;

	if (Z_TYPE_P(filename) != IS_STRING) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_file_put_contents()");
		if (return_value) {
			RETVAL_FALSE;
		}
		return;
	}

	context = php_stream_context_from_zval(zcontext, 0 & PHP_FILE_NO_DEFAULT_CONTEXT);

	stream = php_stream_open_wrapper_ex(Z_STRVAL_P(filename), "wb", ((0 & PHP_FILE_USE_INCLUDE_PATH) ? USE_PATH : 0) | REPORT_ERRORS, NULL, context);
	if (stream == NULL) {
		if (return_value) {
			RETURN_FALSE;
		}
		return;
	}

	switch (Z_TYPE_P(data)) {

		case IS_NULL:
		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL:
		case IS_CONSTANT:
			zend_make_printable_zval(data, &copy, &use_copy);
			if (use_copy) {
				data = &copy;
			}
			/* no break */

		case IS_STRING:
			if (Z_STRLEN_P(data)) {
				numbytes = php_stream_write(stream, Z_STRVAL_P(data), Z_STRLEN_P(data));
				if (numbytes != Z_STRLEN_P(data)) {
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "Only %d of %d bytes written, possibly out of free disk space", numbytes, Z_STRLEN_P(data));
					numbytes = -1;
				}
			}
			break;
		default:
			numbytes = -1;
			break;
	}

	php_stream_close(stream);

	if (use_copy) {
		zval_dtor(data);
	}

	if (numbytes < 0) {
		if (return_value) {
			RETURN_FALSE;
		} else {
			return;
		}
	}

	if (return_value) {
		RETURN_LONG(numbytes);
	}
	return;
}

void zephir_is_dir(zval *return_value, zval *path TSRMLS_DC)
{
	if (likely(Z_TYPE_P(path) == IS_STRING)) {
		php_stat(Z_STRVAL_P(path), (php_stat_len)(Z_STRLEN_P(path)), FS_IS_DIR, return_value TSRMLS_CC);
	} else {
		ZVAL_FALSE(return_value);
	}
}

void zephir_unlink(zval *return_value, zval *path TSRMLS_DC)
{
	if (likely(Z_TYPE_P(path) == IS_STRING)) {
		php_stream_context *context;
		php_stream_wrapper *wrapper;
		zval *zctx = NULL;

		if (unlikely(strlen(Z_STRVAL_P(path)) != Z_STRLEN_P(path))) {
			ZVAL_FALSE(return_value);
			return;
		}

		context = php_stream_context_from_zval(zctx, 0);
		wrapper = php_stream_locate_url_wrapper(Z_STRVAL_P(path), NULL, 0 TSRMLS_CC);

		if (!wrapper || !wrapper->wops || !wrapper->wops->unlink) {
			ZVAL_FALSE(return_value);
			return;
		}

		ZVAL_BOOL(return_value, wrapper->wops->unlink(wrapper, Z_STRVAL_P(path), REPORT_ERRORS, context TSRMLS_CC));
		return;
	}

	ZVAL_FALSE(return_value);
	return;
}

void zephir_filemtime(zval *return_value, zval *path TSRMLS_DC)
{
	if (likely(Z_TYPE_P(path) == IS_STRING)) {
		php_stat(Z_STRVAL_P(path), (php_stat_len)(Z_STRLEN_P(path)), FS_MTIME, return_value TSRMLS_CC);
	} else {
		ZVAL_FALSE(return_value);
	}
}

void zephir_basename(zval *return_value, zval *path TSRMLS_DC)
{
	if (likely(Z_TYPE_P(path) == IS_STRING)) {
		char *ret;
		size_t ret_len;

		php_basename(Z_STRVAL_P(path), Z_STRLEN_P(path), NULL, 0, &ret, &ret_len TSRMLS_CC);
		ZVAL_STRINGL(return_value, ret, (int)ret_len, 0);
	} else {
		ZVAL_FALSE(return_value);
	}
}

void zephir_fwrite(zval *return_value, zval *stream_zval, zval *data TSRMLS_DC)
{

	int num_bytes;
	php_stream *stream;

	if (Z_TYPE_P(stream_zval) != IS_RESOURCE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_fwrite()");
		if (return_value) {
			RETVAL_FALSE;
		} else {
			return;
		}
	}

	if (Z_TYPE_P(data) != IS_STRING) {
		/* @todo convert data to string */
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_fwrite()");
		if (return_value) {
			RETVAL_FALSE;
		} else {
			return;
		}
	}

	if (!Z_STRLEN_P(data)) {
		if (return_value) {
			RETURN_LONG(0);
		} else {
			return;
		}
	}

	PHP_STREAM_TO_ZVAL(stream, &stream_zval);

	num_bytes = php_stream_write(stream, Z_STRVAL_P(data), Z_STRLEN_P(data));
	if (return_value) {
		RETURN_LONG(num_bytes);
	}
}

int zephir_feof(zval *stream_zval TSRMLS_DC)
{

	php_stream *stream;

	if (Z_TYPE_P(stream_zval) != IS_RESOURCE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_feof()");
		return 0;
	}

	php_stream_from_zval_no_verify(stream, &stream_zval);
	if (stream == NULL) {
		return 0;
	}

	return php_stream_eof(stream);
}

int zephir_fclose(zval *stream_zval TSRMLS_DC)
{
	php_stream *stream;

	if (Z_TYPE_P(stream_zval) != IS_RESOURCE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid arguments supplied for zephir_fwrite()");
		return 0;
	}

	php_stream_from_zval_no_verify(stream, &stream_zval);
	if (stream == NULL) {
		return 0;
	}

	if ((stream->flags & PHP_STREAM_FLAG_NO_FCLOSE) != 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%d is not a valid stream resource", stream->rsrc_id);
		return 0;
	}

	if (!stream->is_persistent) {
		php_stream_close(stream);
	} else {
		php_stream_pclose(stream);
	}

	return 1;
}




#ifdef HAVE_CONFIG_H
#endif


#include <Zend/zend_API.h>
#include <main/php_output.h>

void zephir_ob_start(TSRMLS_D)
{
#if PHP_VERSION_ID < 50400
	php_start_ob_buffer(NULL, 0, 1 TSRMLS_CC);
#else
	php_output_start_default(TSRMLS_C);
#endif
}

void zephir_ob_get_contents(zval *result TSRMLS_DC)
{
#if PHP_VERSION_ID < 50400
	php_ob_get_buffer(result TSRMLS_CC);
#else
	php_output_get_contents(result TSRMLS_CC);
#endif
}

int zephir_ob_end_flush(TSRMLS_D)
{
	if (zephir_ob_get_level(TSRMLS_C) < 1) {
		php_error_docref("ref.outcontrol" TSRMLS_CC, E_NOTICE, "failed to delete and flush buffer. No buffer to flush");
		return FAILURE;
	}

#if PHP_VERSION_ID < 50400
	php_end_ob_buffer(1, 0 TSRMLS_CC);
	return SUCCESS;
#else
	return php_output_end(TSRMLS_C);
#endif
}

int zephir_ob_end_clean(TSRMLS_D)
{
	if (zephir_ob_get_level(TSRMLS_C) < 1) {
		php_error_docref("ref.outcontrol" TSRMLS_CC, E_NOTICE, "failed to delete buffer. No buffer to delete");
		return FAILURE;
	}

#if PHP_VERSION_ID < 50400
	php_end_ob_buffer(0, 0 TSRMLS_CC);
	return SUCCESS;
#else
	return php_output_discard(TSRMLS_C);
#endif
}

int zephir_ob_flush(TSRMLS_D)
{
	if (zephir_ob_get_level(TSRMLS_C) < 1) {
		php_error_docref("ref.outcontrol" TSRMLS_CC, E_NOTICE, "failed to flush buffer. No buffer to flush");
		return FAILURE;
	}

#if PHP_VERSION_ID < 50400
	php_end_ob_buffer(1, 1 TSRMLS_CC);
	return SUCCESS;
#else
	return php_output_flush(TSRMLS_C);
#endif
}

int zephir_ob_clean(TSRMLS_D)
{
	if (zephir_ob_get_level(TSRMLS_C) < 1) {
		php_error_docref("ref.outcontrol" TSRMLS_CC, E_NOTICE, "failed to delete buffer. No buffer to delete");
		return FAILURE;
	}

#if PHP_VERSION_ID < 50400
	php_end_ob_buffer(0, 1 TSRMLS_CC);
	return SUCCESS;
#else
	return php_output_clean(TSRMLS_C);
#endif
}

int zephir_ob_get_level(TSRMLS_D)
{
#if PHP_VERSION_ID < 50400
	return OG(ob_nesting_level);
#else
	return php_output_get_level(TSRMLS_C);
#endif
}




#ifdef HAVE_CONFIG_H
#endif




void zephir_throw_exception(zval *object TSRMLS_DC){
	Z_ADDREF_P(object);
	zend_throw_exception_object(object TSRMLS_CC);
}

void zephir_throw_exception_debug(zval *object, const char *file, zend_uint line TSRMLS_DC){

	zend_class_entry *default_exception_ce;

	Z_ADDREF_P(object);

	if (line > 0) {
		default_exception_ce = zend_exception_get_default(TSRMLS_C);
		zend_update_property_string(default_exception_ce, object, "file", sizeof("file")-1, file TSRMLS_CC);
		zend_update_property_long(default_exception_ce, object, "line", sizeof("line")-1, line TSRMLS_CC);
	}

	zend_throw_exception_object(object TSRMLS_CC);
}

void zephir_throw_exception_string_debug(zend_class_entry *ce, const char *message, zend_uint message_len, const char *file, zend_uint line TSRMLS_DC) {

	zval *object, *msg;
	int ZEPHIR_LAST_CALL_STATUS = 0;
	zend_class_entry *default_exception_ce;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	ALLOC_INIT_ZVAL(msg);
	ZVAL_STRINGL(msg, message, message_len, 1);

	ZEPHIR_CALL_METHOD(NULL, object, "__construct", NULL, msg);
	zephir_check_call_status();

	if (line > 0) {
		default_exception_ce = zend_exception_get_default(TSRMLS_C);
		zend_update_property_string(default_exception_ce, object, "file", sizeof("file")-1, file TSRMLS_CC);
		zend_update_property_long(default_exception_ce, object, "line", sizeof("line")-1, line TSRMLS_CC);
	}

	zend_throw_exception_object(object TSRMLS_CC);

	zval_ptr_dtor(&msg);
}

void zephir_throw_exception_string(zend_class_entry *ce, const char *message, zend_uint message_len TSRMLS_DC){

	zval *object, *msg;
	int ZEPHIR_LAST_CALL_STATUS = 0;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	ALLOC_INIT_ZVAL(msg);
	ZVAL_STRINGL(msg, message, message_len, 1);

	ZEPHIR_CALL_METHOD(NULL, object, "__construct", NULL, msg);
	zephir_check_call_status();

	zend_throw_exception_object(object TSRMLS_CC);

	zval_ptr_dtor(&msg);
}

void zephir_throw_exception_zval_debug(zend_class_entry *ce, zval *message, const char *file, zend_uint line TSRMLS_DC){

	zval *object;
	int ZEPHIR_LAST_CALL_STATUS = 0;
	zend_class_entry *default_exception_ce;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	ZEPHIR_CALL_METHOD(NULL, object, "__construct", NULL, message);
	zephir_check_call_status();

	if (line > 0) {
		default_exception_ce = zend_exception_get_default(TSRMLS_C);
		zend_update_property_string(default_exception_ce, object, "file", sizeof("file")-1, file TSRMLS_CC);
		zend_update_property_long(default_exception_ce, object, "line", sizeof("line")-1, line TSRMLS_CC);
	}

	zend_throw_exception_object(object TSRMLS_CC);
}

void zephir_throw_exception_zval(zend_class_entry *ce, zval *message TSRMLS_DC){

	zval *object;
	int ZEPHIR_LAST_CALL_STATUS = 0;

	ALLOC_INIT_ZVAL(object);
	object_init_ex(object, ce);

	ZEPHIR_CALL_METHOD(NULL, object, "__construct", NULL, message);
	zephir_check_call_status();

	zend_throw_exception_object(object TSRMLS_CC);
}





#ifdef HAVE_CONFIG_H
#endif


#include <main/php_main.h>
#include <Zend/zend_exceptions.h>

#ifndef ENFORCE_SAFE_MODE
#define ENFORCE_SAFE_MODE    0
#endif

int zephir_require_ret(zval **return_value_ptr, const char *require_path TSRMLS_DC)
{
	zend_file_handle file_handle;
	int ret;

#ifndef ZEPHIR_RELEASE
	if (return_value_ptr && *return_value_ptr) {
		fprintf(stderr, "%s: *return_value_ptr is expected to be NULL", __func__);
		zephir_print_backtrace();
		abort();
	}
#endif

	if (!require_path) {
		/* @TODO, throw an exception here */
		return FAILURE;
	}

	ret = php_stream_open_for_zend_ex(require_path, &file_handle, ENFORCE_SAFE_MODE | USE_PATH | STREAM_OPEN_FOR_INCLUDE | IGNORE_URL TSRMLS_CC);
	if (ret == SUCCESS) {
		int dummy = 1;
		zend_op_array *new_op_array;

		if (!file_handle.opened_path) {
			file_handle.opened_path = estrdup(require_path);
		}

		zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
		new_op_array = zend_compile_file(&file_handle, ZEND_REQUIRE TSRMLS_CC);
		zend_destroy_file_handle(&file_handle TSRMLS_CC);

		if (new_op_array) {
			zval **original_return_value            = EG(return_value_ptr_ptr);
			zend_op_array *original_active_op_array = EG(active_op_array);
			zend_op **original_opline_ptr           = EG(opline_ptr);

			EG(return_value_ptr_ptr) = return_value_ptr;
			EG(active_op_array)      = new_op_array;

			zend_execute(new_op_array TSRMLS_CC);
			zend_exception_restore(TSRMLS_C);
			destroy_op_array(new_op_array TSRMLS_CC);
			efree(new_op_array);

			if (EG(exception)) {
				assert(!return_value_ptr || !*return_value_ptr);
				ret = FAILURE;
			}
			else {
				ret = SUCCESS;
			}

			EG(return_value_ptr_ptr) = original_return_value;
			EG(active_op_array)      = original_active_op_array;
			EG(opline_ptr)           = original_opline_ptr;
			return ret;
		}
	}
	else {
		zend_destroy_file_handle(&file_handle TSRMLS_CC);
	}

	return FAILURE;
}




#ifdef HAVE_CONFIG_H
#endif



void zephir_serialize(zval *return_value, zval **var TSRMLS_DC) {

	php_serialize_data_t var_hash;
	smart_str buf = {0};

	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, var, &var_hash TSRMLS_CC);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	if (EG(exception)) {
		smart_str_free(&buf);
		RETURN_FALSE;
	}

	if (buf.c) {
		RETURN_STRINGL(buf.c, buf.len, 0);
	} else {
		RETURN_NULL();
	}
}

void zephir_unserialize(zval *return_value, zval *var TSRMLS_DC) {

	const unsigned char *p;
	php_unserialize_data_t var_hash;

	if (Z_TYPE_P(var) != IS_STRING) {
		RETURN_FALSE;
	}

	if (Z_STRLEN_P(var) == 0) {
		RETURN_FALSE;
	}

	p = (const unsigned char*) Z_STRVAL_P(var);
	PHP_VAR_UNSERIALIZE_INIT(var_hash);
	if (!php_var_unserialize(&return_value, &p, p + Z_STRLEN_P(var), &var_hash TSRMLS_CC)) {
		PHP_VAR_UNSERIALIZE_DESTROY(var_hash);
		zval_dtor(return_value);
		ZVAL_NULL(return_value);
		if (!EG(exception)) {
			php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Error at offset %ld of %d bytes", (long)((char*)p - Z_STRVAL_P(var)), Z_STRLEN_P(var));
		}
		RETURN_FALSE;
	}
	PHP_VAR_UNSERIALIZE_DESTROY(var_hash);

}

void zephir_var_export(zval **var TSRMLS_DC) {
    php_var_export(var, 1 TSRMLS_CC);
}

void zephir_var_export_ex(zval *return_value, zval **var TSRMLS_DC) {

    smart_str buf = { NULL, 0, 0 };

    php_var_export_ex(var, 1, &buf TSRMLS_CC);
    smart_str_0(&buf);
    ZVAL_STRINGL(return_value, buf.c, buf.len, 0);
}


void zephir_var_dump(zval **var TSRMLS_DC) {
    php_var_dump(var, 1 TSRMLS_CC);
}





#ifdef HAVE_CONFIG_H
#endif



#ifdef ZEPHIR_USE_PHP_SESSION
#include <ext/session/php_session.h>
#endif

void zephir_session_start(TSRMLS_D)
{
#ifdef ZEPHIR_USE_PHP_SESSION
	php_session_start(TSRMLS_C);
#else
	//zephir_call_func_params(NULL, NULL, SL("session_start") TSRMLS_CC, 0);
#endif
}

void zephir_session_destroy(TSRMLS_D)
{
	//zephir_call_func_params(NULL, NULL, SL("session_destroy") TSRMLS_CC, 0);
}

void zephir_get_session_id(zval *return_value, zval **return_value_ptr TSRMLS_DC)
{
#ifdef ZEPHIR_USE_PHP_SESSION
	if (PS(id)) {
		RETURN_STRING(PS(id), 1);
	}

	RETURN_EMPTY_STRING();
#else
	//zephir_call_func_params(return_value, return_value_ptr, SL("session_id") TSRMLS_CC, 0);
#endif
}

void zephir_set_session_id(zval *sid TSRMLS_DC)
{
#ifdef ZEPHIR_USE_PHP_SESSION
	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(sid) != IS_STRING)) {
		zend_make_printable_zval(sid, &copy, &use_copy);
		if (use_copy) {
			sid = &copy;
		}
	}

	if (PS(id)) {
		efree(PS(id));
	}

	PS(id) = estrndup(Z_STRVAL_P(sid), Z_STRLEN_P(sid));

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
#else
	//zephir_call_func_params(NULL, NULL, SL("session_id") TSRMLS_CC, 1, sid);
#endif
}















#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_ConnectionManager) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ConnectionManager, fastorm, connectionmanager, fastorm_connectionmanager_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(fastorm_connectionmanager_ce, SL("connections"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_connectionmanager_ce, SL("defaultConnectionName"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(fastorm_connectionmanager_ce TSRMLS_CC, 1, fastorm_iconnectionmanager_ce);
	return SUCCESS;

}

static PHP_METHOD(Fastorm_ConnectionManager, __construct) {

	zval *defaultConnectionName_param = NULL, *_0;
	zval *defaultConnectionName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &defaultConnectionName_param);

	zephir_get_strval(defaultConnectionName, defaultConnectionName_param);


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("connections"), _0 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("defaultConnectionName"), defaultConnectionName TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_ConnectionManager, getConnection) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *name_param = NULL, *_0, *_2, *_3 = NULL, *_4, *_5;
	zval *name = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &name_param);

	if (!name_param) {
		ZEPHIR_INIT_VAR(name);
		ZVAL_EMPTY_STRING(name);
	} else {
		zephir_get_strval(name, name_param);
	}


	if (ZEPHIR_IS_STRING_IDENTICAL(name, "")) {
		ZEPHIR_OBS_VAR(_0);
		zephir_read_property_this(&_0, this_ptr, SL("defaultConnectionName"), PH_NOISY_CC);
		zephir_get_strval(_1, _0);
		ZEPHIR_CPY_WRT(name, _1);
	}
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("connections"), PH_NOISY_CC);
	if (!(zephir_array_isset(_2, name))) {
		ZEPHIR_CALL_METHOD(&_3, this_ptr, "createconnection", NULL, name);
		zephir_check_call_status();
		zephir_update_property_array(this_ptr, SL("connections"), name, _3 TSRMLS_CC);
	}
	_4 = zephir_fetch_nproperty_this(this_ptr, SL("connections"), PH_NOISY_CC);
	zephir_array_fetch(&_5, _4, name, PH_NOISY | PH_READONLY, "fastorm/ConnectionManager.zep", 23 TSRMLS_CC);
	RETURN_CTOR(_5);

}

static PHP_METHOD(Fastorm_ConnectionManager, createConnection) {

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_DataObject) {

	ZEPHIR_REGISTER_CLASS(Fastorm, DataObject, fastorm, dataobject, fastorm_dataobject_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(fastorm_dataobject_ce, SL("_data"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_dataobject_ce, SL("_idFieldCache"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_dataobject_ce, SL("_metadataCache"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_dataobject_ce, SL("_propCache"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_class_implements(fastorm_dataobject_ce TSRMLS_CC, 1, zend_ce_arrayaccess);
	zend_class_implements(fastorm_dataobject_ce TSRMLS_CC, 1, zend_ce_serializable);
	return SUCCESS;

}

static PHP_METHOD(Fastorm_DataObject, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *id = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &id);

	if (!id) {
		id = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("_data"), _0 TSRMLS_CC);
	ZEPHIR_CALL_SELF(NULL, "_initialize", NULL);
	zephir_check_call_status();
	if (Z_TYPE_P(id) == IS_ARRAY) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "setdata", NULL, id);
		zephir_check_call_status();
	} else {
		if (Z_TYPE_P(id) != IS_NULL) {
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "setid", NULL, id);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, getId) {

	zval *className, *key, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_idFieldCache") TSRMLS_CC);
	zephir_array_fetch(&key, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 29 TSRMLS_CC);
	if (Z_TYPE_P(key) == IS_NULL) {
		RETURN_MM_NULL();
	} else {
		ZEPHIR_OBS_VAR(_1);
		zephir_read_property_zval(&_1, this_ptr, key, PH_NOISY_CC);
		RETURN_CCTOR(_1);
	}

}

static PHP_METHOD(Fastorm_DataObject, setId) {

	zval *id, *className, *key, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &id);



	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_idFieldCache") TSRMLS_CC);
	zephir_array_fetch(&key, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 40 TSRMLS_CC);
	if (Z_TYPE_P(key) != IS_NULL) {
		zephir_update_property_zval_zval(this_ptr, key, id TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, setData) {

	HashTable *_1;
	HashPosition _0;
	zval *data_param = NULL, *className, *key = NULL, *value = NULL, **_2, *_3, *_4;
	zval *data = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &data_param);

	zephir_get_arrval(data, data_param);


	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	zephir_is_iterable(data, &_1, &_0, 0, 0, "fastorm/DataObject.zep", 57);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		_3 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
		zephir_array_fetch(&_4, _3, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 51 TSRMLS_CC);
		if (zephir_array_isset(_4, key)) {
			zephir_update_property_zval_zval(this_ptr, key, value TSRMLS_CC);
		} else {
			zephir_update_property_array(this_ptr, SL("_data"), key, value TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, getData) {

	HashTable *_3;
	HashPosition _2;
	zval *data, *propertyName = NULL, *propertyFlags = NULL, *className, *_0, *_1, **_4, *_5 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_OBS_VAR(data);
	zephir_read_property_this(&data, this_ptr, SL("_data"), PH_NOISY_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	zephir_array_fetch(&_1, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 63 TSRMLS_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/DataObject.zep", 66);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(propertyName, _3, _2);
		ZEPHIR_GET_HVALUE(propertyFlags, _4);
		ZEPHIR_OBS_NVAR(_5);
		zephir_read_property_zval(&_5, this_ptr, propertyName, PH_NOISY_CC);
		zephir_array_update_zval(&data, propertyName, &_5, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(data);

}

static PHP_METHOD(Fastorm_DataObject, offsetExists) {

	zend_bool _0;
	zval *offset, *_1;

	zephir_fetch_params(0, 1, 0, &offset);



	_0 = zephir_isset_property_zval(this_ptr, offset TSRMLS_CC);
	if (!(_0)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
		_0 = zephir_array_isset(_1, offset);
	}
	RETURN_BOOL(_0);

}

static PHP_METHOD(Fastorm_DataObject, offsetGet) {

	zval *offset, *className, *_0, *_1, *_2, *_3, *_4, *_5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);



	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	zephir_array_fetch(&_1, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 76 TSRMLS_CC);
	if (zephir_array_isset(_1, offset)) {
		ZEPHIR_OBS_VAR(_2);
		zephir_read_property_zval(&_2, this_ptr, offset, PH_NOISY_CC);
		RETURN_CCTOR(_2);
	} else {
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
		if (zephir_array_isset(_3, offset)) {
			_4 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
			zephir_array_fetch(&_5, _4, offset, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 80 TSRMLS_CC);
			RETURN_CTOR(_5);
		} else {
			RETURN_MM_NULL();
		}
	}

}

static PHP_METHOD(Fastorm_DataObject, offsetSet) {

	zval *offset, *value, *className, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &offset, &value);



	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	zephir_array_fetch(&_1, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 90 TSRMLS_CC);
	if (zephir_array_isset(_1, offset)) {
		zephir_update_property_zval_zval(this_ptr, offset, value TSRMLS_CC);
	} else {
		zephir_update_property_array(this_ptr, SL("_data"), offset, value TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, offsetUnset) {

	zval *offset, *className, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);



	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	zephir_array_fetch(&_1, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 100 TSRMLS_CC);
	if (zephir_array_isset(_1, offset)) {
		zephir_update_property_this(this_ptr, Z_STRVAL_P(offset), Z_STRLEN_P(offset), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	} else {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
		zephir_array_unset(&_2, offset, PH_SEPARATE);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, _initialize) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *className, *_0, *metadata = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(className);
	zephir_get_called_class(className TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	if (!(zephir_array_isset(_0, className))) {
		ZEPHIR_CALL_SELF(&metadata, "_processinitialization", NULL, className);
		zephir_check_call_status();
		ZEPHIR_CALL_SELF(NULL, "initialize", NULL, metadata);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, initialize) {

	zval *metadata;

	zephir_fetch_params(0, 1, 0, &metadata);



	if (!(zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}

}

static PHP_METHOD(Fastorm_DataObject, _processInitialization) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *className_param = NULL, *metadata = NULL, *_0 = NULL;
	zval *className = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &className_param);

	zephir_get_strval(className, className_param);


	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL, className);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_update_static_property_array_multi_ce(fastorm_dataobject_ce, SL("_propCache"), &_0 TSRMLS_CC, SL("z"), 1, className);
	ZEPHIR_CALL_METHOD(&_0, metadata, "getidfield",  NULL);
	zephir_check_call_status();
	zephir_update_static_property_array_multi_ce(fastorm_dataobject_ce, SL("_idFieldCache"), &_0 TSRMLS_CC, SL("z"), 1, className);
	RETURN_CCTOR(metadata);

}

static PHP_METHOD(Fastorm_DataObject, getMetadata) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *className_param = NULL;
	zval *className = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &className_param);

	if (!className_param) {
		ZEPHIR_INIT_VAR(className);
		ZVAL_EMPTY_STRING(className);
	} else {
		zephir_get_strval(className, className_param);
	}


	if (ZEPHIR_IS_STRING_IDENTICAL(className, "")) {
		ZEPHIR_INIT_NVAR(className);
		zephir_get_called_class(className TSRMLS_CC);
	}
	ZEPHIR_RETURN_CALL_CE_STATIC(fastorm_objectmetadata_ce, "getmetadata", &_0, className);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DataObject, serialize) {

	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getdataforserialization", NULL, ZEPHIR_GLOBAL(global_false));
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_FUNCTION("serialize", &_1, _0);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DataObject, unserialize) {

	zephir_fcall_cache_entry *_8 = NULL;
	zend_class_entry *_7;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_6 = NULL;
	zval *serialized, *unserialized = NULL, *key = NULL, *value = NULL, *className, **_3, _4 = zval_used_for_init, *_5 = NULL, *_9, *_10;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &serialized);



	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&unserialized, "unserialize", &_0, serialized);
	zephir_check_call_status();
	zephir_is_iterable(unserialized, &_2, &_1, 0, 0, "fastorm/DataObject.zep", 160);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(key, _2, _1);
		ZEPHIR_GET_HVALUE(value, _3);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_STRING(&_4, "/[1-2][0-9]{3}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}/", 0);
		ZEPHIR_CALL_FUNCTION(&_5, "preg_match", &_6, &_4, value);
		zephir_check_call_status();
		if (zephir_is_true(_5)) {
			ZEPHIR_INIT_NVAR(value);
			_7 = zend_fetch_class(SL("DateTime"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
			object_init_ex(value, _7);
			ZEPHIR_CALL_METHOD(NULL, value, "__construct", &_8, value);
			zephir_check_call_status();
		}
		_9 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
		zephir_array_fetch(&_10, _9, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 152 TSRMLS_CC);
		if (zephir_array_isset(_10, key)) {
			zephir_update_property_zval_zval(this_ptr, key, value TSRMLS_CC);
		} else {
			zephir_update_property_array(this_ptr, SL("_data"), key, value TSRMLS_CC);
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DataObject, getDataForSerialization) {

	HashTable *_1;
	HashPosition _0;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *data;
	zval *justClass_param = NULL, *key = NULL, *value = NULL, *iterate = NULL, **_2, *_3 = NULL, *_4 = NULL;
	zend_bool justClass;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &justClass_param);

	if (!justClass_param) {
		justClass = 0;
	} else {
		justClass = zephir_get_boolval(justClass_param);
	}


	ZEPHIR_INIT_VAR(data);
	array_init(data);
	if (justClass) {
		ZEPHIR_CALL_METHOD(&iterate, this_ptr, "getdbformateddata",  NULL);
		zephir_check_call_status();
	} else {
		ZEPHIR_CALL_METHOD(&iterate, this_ptr, "getdata",  NULL);
		zephir_check_call_status();
	}
	zephir_is_iterable(iterate, &_1, &_0, 0, 0, "fastorm/DataObject.zep", 179);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(key, _1, _0);
		ZEPHIR_GET_HVALUE(value, _2);
		if (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC)) {
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_STRING(_4, "Y-m-d H:i:s", 0);
			ZEPHIR_CALL_METHOD(&_3, value, "format", NULL, _4);
			zephir_check_temp_parameter(_4);
			zephir_check_call_status();
			zephir_array_update_zval(&data, key, &_3, PH_COPY | PH_SEPARATE);
		} else {
			zephir_array_update_zval(&data, key, &value, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CTOR(data);

}

static PHP_METHOD(Fastorm_DataObject, getDbFormatedData) {

	HashTable *_3;
	HashPosition _2;
	zval *data;
	zval *withoutAutoincrementKeys_param = NULL, *propName = NULL, *propFlag = NULL, *className, *_0, *_1, **_4, *_6 = NULL;
	zend_bool withoutAutoincrementKeys, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &withoutAutoincrementKeys_param);

	if (!withoutAutoincrementKeys_param) {
		withoutAutoincrementKeys = 0;
	} else {
		withoutAutoincrementKeys = zephir_get_boolval(withoutAutoincrementKeys_param);
	}


	ZEPHIR_INIT_VAR(className);
	zephir_get_class(className, this_ptr, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	_0 = zephir_fetch_static_property_ce(fastorm_dataobject_ce, SL("_propCache") TSRMLS_CC);
	zephir_array_fetch(&_1, _0, className, PH_NOISY | PH_READONLY, "fastorm/DataObject.zep", 188 TSRMLS_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/DataObject.zep", 194);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(propName, _3, _2);
		ZEPHIR_GET_HVALUE(propFlag, _4);
		_5 = !withoutAutoincrementKeys;
		if (!(_5)) {
			_5 = !(((int) (zephir_get_numberval(propFlag)) & 4));
		}
		if (_5) {
			ZEPHIR_OBS_NVAR(_6);
			zephir_read_property_zval(&_6, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&data, propName, &_6, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CTOR(data);

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_ColumnInfo) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ColumnInfo, fastorm, db_columninfo, fastorm_db_columninfo_method_entry, 0);

	zend_declare_property_null(fastorm_db_columninfo_ce, SL("patterns"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_ColumnInfo, detectType) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL;
	HashTable *_4;
	HashPosition _3;
	zval *_1;
	zval *type, *_0, *key = NULL, *val = NULL, *_2, **_5, *_6 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type);



	_0 = zephir_fetch_static_property_ce(fastorm_db_columninfo_ce, SL("patterns") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_INIT_VAR(_1);
		array_init_size(_1, 12);
		add_assoc_stringl_ex(_1, SS("^_"), SL("s"), 1);
		add_assoc_stringl_ex(_1, SS("BYTEA|BLOB|BIN"), SL("bin"), 1);
		add_assoc_stringl_ex(_1, SS("TEXT|CHAR|POINT|INTERVAL"), SL("s"), 1);
		add_assoc_stringl_ex(_1, SS("YEAR|BYTE|COUNTER|SERIAL|INT|LONG|SHORT"), SL("i"), 1);
		add_assoc_stringl_ex(_1, SS("CURRENCY|REAL|MONEY|FLOAT|DOUBLE|DECIMAL|NUMERIC|NUMBER"), SL("f"), 1);
		add_assoc_stringl_ex(_1, SS("^TIME$"), SL("t"), 1);
		add_assoc_stringl_ex(_1, SS("TIME"), SL("t"), 1);
		add_assoc_stringl_ex(_1, SS("DATE"), SL("d"), 1);
		add_assoc_stringl_ex(_1, SS("BOOL"), SL("b"), 1);
		zephir_update_static_property_ce(fastorm_db_columninfo_ce, SL("patterns"), _1 TSRMLS_CC);
	}
	_2 = zephir_fetch_static_property_ce(fastorm_db_columninfo_ce, SL("patterns") TSRMLS_CC);
	zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/db/ColumnInfo.zep", 36);
	for (
	  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
	  ; zephir_hash_move_forward_ex(_4, &_3)
	) {
		ZEPHIR_GET_HMKEY(key, _4, _3);
		ZEPHIR_GET_HVALUE(val, _5);
		ZEPHIR_INIT_LNVAR(_6);
		ZEPHIR_CONCAT_SVS(_6, "#", key, "#i");
		ZEPHIR_CALL_FUNCTION(&_7, "preg_match", &_8, _6, type);
		zephir_check_call_status();
		if (zephir_is_true(_7)) {
			RETURN_CCTOR(val);
		}
	}
	RETURN_MM_STRING("s", 1);

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Connection) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Connection, fastorm, db_connection, fastorm_db_connection_method_entry, 0);

	zend_declare_property_null(fastorm_db_connection_ce, SL("events"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("config"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("driver"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("translator"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("connected"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Connection, __construct) {

	zend_class_entry *_14;
	zephir_fcall_cache_entry *_5 = NULL;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_10 = NULL;
	zval *driver = NULL, *driverClass = NULL, *_11 = NULL, *_13;
	zval *config = NULL, *name = NULL, *key = NULL, *val = NULL, *tmp, **_3, *_4 = NULL, *_6 = NULL, *_7, _8, _9, *_12 = NULL, *_15;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &config, &name);

	ZEPHIR_SEPARATE_PARAM(config);
	if (!name) {
		name = ZEPHIR_GLOBAL(global_null);
	}


	zephir_update_property_this(this_ptr, SL("connected"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	if (Z_TYPE_P(config) == IS_STRING) {
		Z_SET_ISREF_P(config);
		ZEPHIR_CALL_FUNCTION(NULL, "parse_str", &_0, config, config);
		Z_UNSET_ISREF_P(config);
		zephir_check_call_status();
	} else {
		if (Z_TYPE_P(config) == IS_ARRAY == 0) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Configuration must be array, string or object.", "fastorm/db/Connection.zep", 44);
			return;
		}
		ZEPHIR_INIT_VAR(tmp);
		array_init(tmp);
		zephir_is_iterable(config, &_2, &_1, 0, 0, "fastorm/db/Connection.zep", 55);
		for (
		  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		  ; zephir_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HMKEY(key, _2, _1);
			ZEPHIR_GET_HVALUE(val, _3);
			if (zephir_is_instance_of(val, SL("Traversable") TSRMLS_CC)) {
				ZEPHIR_CALL_FUNCTION(&_4, "iterator_to_array", &_5, val);
				zephir_check_call_status();
				zephir_array_update_zval(&tmp, key, &_4, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_zval(&tmp, key, &val, PH_COPY | PH_SEPARATE);
			}
		}
		ZEPHIR_CPY_WRT(config, tmp);
	}
	if (!(zephir_array_isset_string(config, SS("driver")))) {
		ZEPHIR_INIT_VAR(_6);
		ZVAL_STRING(_6, "mysql", 1);
		zephir_array_update_string(&config, SL("driver"), &_6, PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_NVAR(_6);
	zephir_array_fetch_string(&_7, config, SL("driver"), PH_NOISY | PH_READONLY, "fastorm/db/Connection.zep", 64 TSRMLS_CC);
	zephir_fast_strtolower(_6, _7);
	ZEPHIR_SINIT_VAR(_8);
	ZVAL_STRING(&_8, "#[^a-z0-9_]#", 0);
	ZEPHIR_SINIT_VAR(_9);
	ZVAL_STRING(&_9, "_", 0);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_10, &_8, &_9, _6);
	zephir_check_call_status();
	zephir_get_strval(_11, _4);
	ZEPHIR_CPY_WRT(driver, _11);
	ZEPHIR_INIT_VAR(driverClass);
	ZEPHIR_CONCAT_SVS(driverClass, "\\Fastorm\\Db\\Drivers\\", driver, "Driver");
	if (zephir_class_exists(driverClass, zephir_is_true(ZEPHIR_GLOBAL(global_false))  TSRMLS_CC) == 0) {
		ZEPHIR_INIT_VAR(_12);
		object_init_ex(_12, fastorm_db_dbexception_ce);
		ZEPHIR_INIT_VAR(_13);
		ZEPHIR_CONCAT_SVS(_13, "Unable to create instance of dibi driver ", driverClass, ".");
		ZEPHIR_CALL_METHOD(NULL, _12, "__construct", NULL, _13);
		zephir_check_call_status();
		zephir_throw_exception_debug(_12, "fastorm/db/Connection.zep", 67 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_update_string(&config, SL("name"), &name, PH_COPY | PH_SEPARATE);
	zephir_update_property_this(this_ptr, SL("config"), config TSRMLS_CC);
	ZEPHIR_INIT_LNVAR(_12);
	_14 = zend_fetch_class(Z_STRVAL_P(driverClass), Z_STRLEN_P(driverClass), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(_12, _14);
	if (zephir_has_constructor(_12 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _12, "__construct", NULL);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("driver"), _12 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_15);
	object_init_ex(_15, fastorm_db_translator_ce);
	ZEPHIR_CALL_METHOD(NULL, _15, "__construct", NULL, this_ptr);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("translator"), _15 TSRMLS_CC);
	if (!(zephir_array_isset_string(config, SS("lazy")))) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, __destruct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _1, _3;
	zval *_0, *_2, *_4, *_5 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	_1 = zephir_is_true(_0);
	if (_1) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_1 = Z_TYPE_P(_2) != IS_NULL;
	}
	_3 = _1;
	if (_3) {
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_5, _4, "getresource",  NULL);
		zephir_check_call_status();
		_3 = Z_TYPE_P(_5) != IS_NULL;
	}
	if (_3) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "disconnect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, connect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *e = NULL, *event = NULL, *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	ZEPHIR_CALL_METHOD(&event, this_ptr, "event", NULL, _0);
	zephir_check_call_status();

	/* try_start_1: */

		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "connect", NULL, _2);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_update_property_this(this_ptr, SL("connected"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
		ZEPHIR_CALL_METHOD(&_3, event, "done",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CALL_METHOD(&_3, event, "done", NULL, e);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
			zephir_check_call_status();
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 134 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, disconnect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, _0, "disconnect", NULL);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, isConnected) {


	RETURN_MEMBER_QUICK(this_ptr, "connected", 3529665016UL);

}

static PHP_METHOD(Fastorm_Db_Connection, getConfig) {

	zval *key = NULL, *defaultValue = NULL, *_0, *_1, *_2;

	zephir_fetch_params(0, 0, 2, &key, &defaultValue);

	if (!key) {
		key = ZEPHIR_GLOBAL(global_null);
	}
	if (!defaultValue) {
		defaultValue = ZEPHIR_GLOBAL(global_null);
	}


	if (Z_TYPE_P(key) == IS_NULL) {
		RETURN_MEMBER_QUICK(this_ptr, "config", 3547462619UL);
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
		if (zephir_array_isset(_0, key)) {
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
			zephir_array_fetch(&_2, _1, key, PH_NOISY | PH_READONLY, "fastorm/db/Connection.zep", 176 TSRMLS_CC);
			RETURN_CTORW(_2);
		} else {
			RETURN_CCTORW(defaultValue);
		}
	}

}

static PHP_METHOD(Fastorm_Db_Connection, getDriver) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	RETURN_MM_MEMBER(this_ptr, "driver");

}

static PHP_METHOD(Fastorm_Db_Connection, queryArray) {

	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *funcArgs, *metadata = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &funcArgs, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "translateargs", &_2, funcArgs);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nativequery", NULL, _1, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, __call) {

	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *funcName, *arguments, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &funcName, &arguments);



	if (ZEPHIR_IS_STRING_IDENTICAL(funcName, "query")) {
		ZEPHIR_CALL_METHOD(&_0, this_ptr, "translateargs", &_1, arguments);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "nativequery", NULL, _0);
		zephir_check_call_status();
	} else {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_exception_ce, "Method not exists", "fastorm/db/Connection.zep", 218);
		return;
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, translate) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
	zval *args = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "translateargs", &_1, args);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, translateArgs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *args, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("translator"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_METHOD(_1, "translate", NULL, args);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, nativeQuery) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *sql, *metadata = NULL, *res = NULL, *e = NULL, *event = NULL, *_1, *_2, *_3, *_4 = NULL, *_5, *_6 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &sql, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_1)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(res);
	ZVAL_NULL(res);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 60);
	ZEPHIR_CALL_METHOD(&event, this_ptr, "event", NULL, _2, sql);
	zephir_check_call_status();

	/* try_start_1: */

		_3 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&res, _3, "query", NULL, sql);
		zephir_check_call_status_or_jump(try_end_1);
		if (zephir_is_true(res)) {
			ZEPHIR_CALL_METHOD(&_4, this_ptr, "createresultset", NULL, res, metadata);
			zephir_check_call_status_or_jump(try_end_1);
			ZEPHIR_CPY_WRT(res, _4);
		} else {
			_5 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
			ZEPHIR_CALL_METHOD(&res, _5, "getaffectedrows",  NULL);
			zephir_check_call_status_or_jump(try_end_1);
		}
		ZEPHIR_CALL_METHOD(&_4, event, "done", NULL, res);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _4);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_CCTOR(res);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CALL_METHOD(&_6, event, "done", NULL, e);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _6);
			zephir_check_call_status();
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 283 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, getAffectedRows) {

	zend_bool _4;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *rows = NULL, *_0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&rows, _1, "getaffectedrows",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_2, "is_int", &_3, rows);
	zephir_check_call_status();
	_4 = ZEPHIR_IS_FALSE_IDENTICAL(_2);
	if (!(_4)) {
		_4 = ZEPHIR_LT_LONG(rows, 0);
	}
	if (_4) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Cannot retrieve number of affected rows.", "fastorm/db/Connection.zep", 302);
		return;
	}
	RETURN_CCTOR(rows);

}

static PHP_METHOD(Fastorm_Db_Connection, getInsertId) {

	int id, ZEPHIR_LAST_CALL_STATUS;
	zval *sequence = NULL, *_0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &sequence);

	if (!sequence) {
		sequence = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_2, _1, "getinsertid", NULL, sequence);
	zephir_check_call_status();
	id = zephir_get_intval(_2);
	if (id < 1) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Cannot retrieve last generated ID.", "fastorm/db/Connection.zep", 323);
		return;
	}
	RETURN_MM_LONG(id);

}

static PHP_METHOD(Fastorm_Db_Connection, begin) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *event = NULL, *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 64);
	ZEPHIR_CALL_METHOD(&event, this_ptr, "event", NULL, _1, savepoint);
	zephir_check_call_status();

	/* try_start_1: */

		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _2, "begin", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(&_3, event, "done",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CALL_METHOD(&_3, event, "done", NULL, e);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
			zephir_check_call_status();
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 349 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, commit) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *event = NULL, *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 128);
	ZEPHIR_CALL_METHOD(&event, this_ptr, "event", NULL, _1, savepoint);
	zephir_check_call_status();

	/* try_start_1: */

		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _2, "commit", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(&_3, event, "done",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CALL_METHOD(&_3, event, "done", NULL, e);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
			zephir_check_call_status();
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 374 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, rollback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *event = NULL, *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 256);
	ZEPHIR_CALL_METHOD(&event, this_ptr, "event", NULL, _1, savepoint);
	zephir_check_call_status();

	/* try_start_1: */

		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _2, "rollback", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(&_3, event, "done",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CALL_METHOD(&_3, event, "done", NULL, e);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "onevent", NULL, _3);
			zephir_check_call_status();
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 399 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, createResultSet) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *resultDriver, *metadata = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &resultDriver, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_instance_of_ev(resultDriver, fastorm_db_iresultdriver_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'resultDriver' must be an instance of 'Fastorm\\Db\\IResultDriver'", "", 0);
		return;
	}
	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	object_init_ex(return_value, fastorm_db_result_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, resultDriver, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, command) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, fastorm_db_query_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, this_ptr);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, select) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *args = NULL, *_1 = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "select", 0);
	ZEPHIR_RETURN_CALL_METHOD(_1, "__call", NULL, _2, args);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, update) {

	zval *_3;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *table, *args, *_1 = NULL, *_2 = NULL, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &table, &args);



	_0 = Z_TYPE_P(args) == IS_ARRAY;
	if (!(_0)) {
		_0 = (zephir_is_instance_of(args, SL("Traversable") TSRMLS_CC));
	}
	if (!(_0)) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Arguments must be array or Traversable.", "fastorm/db/Connection.zep", 449);
		return;
	}
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_3);
	array_init_size(_3, 3);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "%n", 1);
	zephir_array_fast_append(_3, _4);
	zephir_array_fast_append(_3, table);
	ZEPHIR_INIT_BNVAR(_4);
	ZVAL_STRING(_4, "update", 0);
	ZEPHIR_CALL_METHOD(&_2, _1, "__call", NULL, _4, _3);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_INIT_BNVAR(_4);
	ZVAL_STRING(_4, "set", 0);
	ZEPHIR_RETURN_CALL_METHOD(_2, "__call", NULL, _4, args);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, insert) {

	zval *_5, *_6;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *table, *args = NULL, *_0 = NULL, *_1 = NULL, *_2, *_3, *_4 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &table, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	if (zephir_is_instance_of(args, SL("Traversable") TSRMLS_CC)) {
		ZEPHIR_CALL_FUNCTION(&_0, "iterator_to_array", NULL, args);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(args, _0);
	} else {
		if (Z_TYPE_P(args) == IS_ARRAY == 0) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Arguments must be array or Traversable.", "fastorm/db/Connection.zep", 467);
			return;
		}
	}
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "insert", 0);
	ZEPHIR_CALL_METHOD(&_1, _0, "__call", NULL, _3, _2);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_5);
	array_init_size(_5, 6);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "%n", 1);
	zephir_array_fast_append(_5, _3);
	zephir_array_fast_append(_5, table);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "(%n)", 1);
	zephir_array_fast_append(_5, _3);
	ZEPHIR_INIT_BNVAR(_3);
	zephir_array_keys(_3, args TSRMLS_CC);
	zephir_array_fast_append(_5, _3);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "into", 0);
	ZEPHIR_CALL_METHOD(&_4, _1, "__call", NULL, _3, _5);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_6);
	array_init_size(_6, 3);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "%l", 1);
	zephir_array_fast_append(_6, _3);
	zephir_array_fast_append(_6, args);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "values", 0);
	ZEPHIR_RETURN_CALL_METHOD(_4, "__call", NULL, _3, _6);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, delete) {

	zval *_4;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *table, *_0 = NULL, *_1 = NULL, *_2, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &table);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "delete", 0);
	ZEPHIR_CALL_METHOD(&_1, _0, "__call", NULL, _3, _2);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_4);
	array_init_size(_4, 3);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "%n", 1);
	zephir_array_fast_append(_4, _3);
	zephir_array_fast_append(_4, table);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "from", 0);
	ZEPHIR_RETURN_CALL_METHOD(_1, "__call", NULL, _3, _4);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Connection, addEvent) {

	zval *eventHandler, *evArray = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &eventHandler);



	_0 = zephir_fetch_static_property_ce(fastorm_db_connection_ce, SL("events") TSRMLS_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_INIT_VAR(evArray);
		array_init(evArray);
	} else {
		ZEPHIR_OBS_NVAR(evArray);
		zephir_read_static_property_ce(&evArray, fastorm_db_connection_ce, SL("events") TSRMLS_CC);
	}
	zephir_array_append(&evArray, eventHandler, PH_SEPARATE, "fastorm/db/Connection.zep", 494);
	zephir_update_static_property_ce(fastorm_db_connection_ce, SL("events"), evArray TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, onEvent) {

	int ZEPHIR_LAST_CALL_STATUS;
	HashTable *_5;
	HashPosition _4;
	zend_bool _0, _2;
	zval *event = NULL, *_1, *callback = NULL, *_3, **_6;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &event);

	if (!event) {
		event = ZEPHIR_GLOBAL(global_null);
	}


	_0 = Z_TYPE_P(event) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(event, fastorm_db_event_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'event' must be an instance of 'Fastorm\\Db\\Event'", "", 0);
		return;
	}
	_1 = zephir_fetch_static_property_ce(fastorm_db_connection_ce, SL("events") TSRMLS_CC);
	_2 = Z_TYPE_P(_1) != IS_NULL;
	if (_2) {
		_2 = Z_TYPE_P(event) != IS_NULL;
	}
	if (_2) {
		_3 = zephir_fetch_static_property_ce(fastorm_db_connection_ce, SL("events") TSRMLS_CC);
		zephir_is_iterable(_3, &_5, &_4, 0, 0, "fastorm/db/Connection.zep", 504);
		for (
		  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		  ; zephir_hash_move_forward_ex(_5, &_4)
		) {
			ZEPHIR_GET_HVALUE(callback, _6);
			ZEPHIR_CALL_ZVAL_FUNCTION(NULL, callback, NULL, event);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Connection, event) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *type, *sql = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &type, &sql);

	if (!sql) {
		sql = ZEPHIR_GLOBAL(global_null);
	}


	object_init_ex(return_value, fastorm_db_event_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, this_ptr, type, sql);
	zephir_check_call_status();
	RETURN_MM();

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_DbException) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm\\Db, DbException, fastorm, db_dbexception, fastorm_exception_ce, fastorm_db_dbexception_method_entry, 0);

	zend_declare_property_null(fastorm_db_dbexception_ce, SL("sql"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_dbexception_ce, SL("args"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_DbException, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *exception, *code = NULL, *previous = NULL, *sql = NULL, *args = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 4, &exception, &code, &previous, &sql, &args);

	if (!code) {
		code = ZEPHIR_GLOBAL(global_null);
	}
	if (!previous) {
		previous = ZEPHIR_GLOBAL(global_null);
	}
	if (!sql) {
		sql = ZEPHIR_GLOBAL(global_null);
	}
	if (!args) {
		args = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_PARENT(NULL, fastorm_db_dbexception_ce, this_ptr, "__construct", NULL, exception, code, previous);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("sql"), sql TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("args"), args TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_DbException, getSql) {


	RETURN_MEMBER_QUICK(this_ptr, "sql", 2090732981UL);

}

static PHP_METHOD(Fastorm_Db_DbException, getArgs) {


	RETURN_MEMBER_QUICK(this_ptr, "args", 253399346UL);

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Drivers_MysqliDriver) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm\\Db\\Drivers, MysqliDriver, fastorm, db_drivers_mysqlidriver, fastorm_dbobject_ce, fastorm_db_drivers_mysqlidriver_method_entry, 0);

	/** @var mysqli  Connection resource */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("connection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var mysqli_result  Resultset resource */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("resultSet"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_bool(fastorm_db_drivers_mysqlidriver_ce, SL("autoFree"), 1, ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool  Is buffered (seekable and countable)? */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("buffered"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("types"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_ACCESS_DENIED"), 1045 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_DUPLICATE_ENTRY"), 1062 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_DATA_TRUNCATED"), 1265 TSRMLS_CC);

	zend_class_implements(fastorm_db_drivers_mysqlidriver_ce TSRMLS_CC, 1, fastorm_db_iresultdriver_ce);
	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval _0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "mysqli", 0);
	ZEPHIR_CALL_FUNCTION(&_1, "extension_loaded", &_2, &_0);
	zephir_check_call_status();
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: PHP extension 'mysqli' is not loaded.", "fastorm/db/Drivers/MysqliDriver.zep", 30);
		return;
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, connect) {

	zephir_fcall_cache_entry *_14 = NULL, *_23 = NULL;
	HashTable *_11;
	HashPosition _10;
	zval *_8;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_7 = NULL, *_9 = NULL, *_26 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *config = NULL, _0 = zval_used_for_init, *_1, *defaults, *_2 = NULL, *_4 = NULL, *host = NULL, *foo, *_5 = NULL, *_6 = NULL, *key = NULL, *value = NULL, *iterate, **_12, *_13, *_15, *_16 = NULL, *_17, *_18, *_19, *_20, *_21, *_22, *_24, *ok = NULL, _25, *_27;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config);

	ZEPHIR_SEPARATE_PARAM(config);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(NULL, "mysqli_report", NULL, &_0);
	zephir_check_call_status();
	if (zephir_array_isset_string(config, SS("resource"))) {
		zephir_array_fetch_string(&_1, config, SL("resource"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 43 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("connection"), _1 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_VAR(defaults);
		array_init_size(defaults, 9);
		add_assoc_stringl_ex(defaults, SS("charset"), SL("utf8"), 1);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "P", 0);
		ZEPHIR_CALL_FUNCTION(&_2, "date", NULL, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&defaults, SL("timezone"), &_2, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_user", 0);
		ZEPHIR_CALL_FUNCTION(&_2, "ini_get", &_3, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&defaults, SL("username"), &_2, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_pw", 0);
		ZEPHIR_CALL_FUNCTION(&_2, "ini_get", &_3, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&defaults, SL("password"), &_2, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_socket", 0);
		ZEPHIR_CALL_FUNCTION(&_2, "ini_get", &_3, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&defaults, SL("socket"), &_2, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&defaults, SL("port"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&defaults, SL("flags"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_VAR(_4);
		zephir_fast_array_merge(_4, &(defaults), &(config) TSRMLS_CC);
		ZEPHIR_CPY_WRT(config, _4);
		if (!(zephir_array_isset_string(config, SS("host")))) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_STRING(&_0, "mysqli.default_host", 0);
			ZEPHIR_CALL_FUNCTION(&host, "ini_get", &_3, &_0);
			zephir_check_call_status();
			if (zephir_is_true(host)) {
				zephir_array_update_string(&config, SL("host"), &host, PH_COPY | PH_SEPARATE);
				ZEPHIR_SINIT_NVAR(_0);
				ZVAL_STRING(&_0, "mysqli.default_port", 0);
				ZEPHIR_CALL_FUNCTION(&_2, "ini_get", &_3, &_0);
				zephir_check_call_status();
				zephir_array_update_string(&config, SL("port"), &_2, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_string(&config, SL("host"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&config, SL("port"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
			}
		}
		ZEPHIR_OBS_VAR(foo);
		zephir_array_fetch_string(&foo, config, SL("database"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 71 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&_5, "mysqli_init", NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("connection"), _5 TSRMLS_CC);
		if (zephir_array_isset_string(config, SS("options"))) {
			zephir_array_fetch_string(&_1, config, SL("options"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 75 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(&_6, "is_scalar", &_7, _1);
			zephir_check_call_status();
			if (zephir_is_true(_6)) {
				ZEPHIR_INIT_VAR(_8);
				ZEPHIR_CONCAT_SS(_8, "MysqliDriver", ": configuration item 'options' must be array; for constants MYSQLI_CLIENT_* use 'flags'.");
				ZEPHIR_SINIT_NVAR(_0);
				ZVAL_LONG(&_0, 1024);
				ZEPHIR_CALL_FUNCTION(NULL, "trigger_error", &_9, _8, &_0);
				zephir_check_call_status();
			} else {
				ZEPHIR_OBS_VAR(iterate);
				zephir_array_fetch_string(&iterate, config, SL("options"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 79 TSRMLS_CC);
				zephir_is_iterable(iterate, &_11, &_10, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 83);
				for (
				  ; zephir_hash_get_current_data_ex(_11, (void**) &_12, &_10) == SUCCESS
				  ; zephir_hash_move_forward_ex(_11, &_10)
				) {
					ZEPHIR_GET_HMKEY(key, _11, _10);
					ZEPHIR_GET_HVALUE(value, _12);
					_13 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
					ZEPHIR_CALL_FUNCTION(NULL, "mysqli_options", &_14, _13, key, value);
					zephir_check_call_status();
				}
			}
		}
		if (!(zephir_array_isset_string(config, SS("persistent")))) {
			_13 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_15, config, SL("host"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_16);
			ZEPHIR_CONCAT_SV(_16, "", _15);
			zephir_array_fetch_string(&_17, config, SL("username"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			zephir_array_fetch_string(&_18, config, SL("password"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			zephir_array_fetch_string(&_19, config, SL("database"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			zephir_array_fetch_string(&_20, config, SL("port"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			zephir_array_fetch_string(&_21, config, SL("socket"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			zephir_array_fetch_string(&_22, config, SL("flags"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 86 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(NULL, "mysqli_real_connect", &_23, _13, _16, _17, _18, _19, _20, _21, _22);
			zephir_check_call_status();
		} else {
			_24 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_15, config, SL("host"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			ZEPHIR_INIT_LNVAR(_16);
			ZEPHIR_CONCAT_SV(_16, "p:", _15);
			zephir_array_fetch_string(&_17, config, SL("username"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			zephir_array_fetch_string(&_18, config, SL("password"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			zephir_array_fetch_string(&_19, config, SL("database"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			zephir_array_fetch_string(&_20, config, SL("port"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			zephir_array_fetch_string(&_21, config, SL("socket"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			zephir_array_fetch_string(&_22, config, SL("flags"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 88 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(NULL, "mysqli_real_connect", &_23, _24, _16, _17, _18, _19, _20, _21, _22);
			zephir_check_call_status();
		}
	}
	if (zephir_array_isset_string(config, SS("charset"))) {
		ZEPHIR_INIT_VAR(ok);
		ZVAL_BOOL(ok, 0);
		ZEPHIR_INIT_NVAR(_4);
		ZEPHIR_GET_CONSTANT(_4, "PHP_VERSION");
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "5.1.5", 0);
		ZEPHIR_SINIT_VAR(_25);
		ZVAL_STRING(&_25, ">=", 0);
		ZEPHIR_CALL_FUNCTION(&_2, "version_compare", &_26, _4, &_0, &_25);
		zephir_check_call_status();
		if (zephir_is_true(_2)) {
			_24 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_1, config, SL("charset"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 102 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(&ok, "mysqli_set_charset", NULL, _24, _1);
			zephir_check_call_status();
		}
		if (!(zephir_is_true(ok))) {
			zephir_array_fetch_string(&_15, config, SL("charset"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 105 TSRMLS_CC);
			ZEPHIR_INIT_LNVAR(_16);
			ZEPHIR_CONCAT_SVS(_16, "SET NAMES '", _15, "'");
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _16);
			zephir_check_call_status();
		}
	}
	if (zephir_array_isset_string(config, SS("sqlmode"))) {
		zephir_array_fetch_string(&_1, config, SL("sqlmode"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 110 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_16);
		ZEPHIR_CONCAT_SVS(_16, "SET sql_mode='", _1, "'");
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _16);
		zephir_check_call_status();
	}
	if (zephir_array_isset_string(config, SS("timezone"))) {
		zephir_array_fetch_string(&_1, config, SL("timezone"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 114 TSRMLS_CC);
		ZEPHIR_INIT_VAR(_27);
		ZEPHIR_CONCAT_SVS(_27, "SET time_zone='", _1, "'");
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _27);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("buffered"), (!zephir_array_isset_string(config, SS("unbuffered"))) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, disconnect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) != IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(NULL, "mysqli_close", NULL, _1);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("connection"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, query) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL, *_5 = NULL;
	zval *sql, *res = NULL, *_0, *_1, _2 = zval_used_for_init, *_4 = NULL, *_6, *_7, *_8 = NULL, *_9, *_10, *_11 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &sql);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_2);
		ZVAL_LONG(&_2, 0);
		ZEPHIR_CALL_FUNCTION(&res, "mysqli_query", &_3, _1, sql, &_2);
		zephir_check_call_status();
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_2);
		ZVAL_LONG(&_2, 1);
		ZEPHIR_CALL_FUNCTION(&res, "mysqli_query", &_3, _1, sql, &_2);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_4, "mysqli_errno", &_5, _1);
	zephir_check_call_status();
	if (zephir_is_true(_4)) {
		ZEPHIR_INIT_VAR(_6);
		object_init_ex(_6, fastorm_db_dbexception_ce);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(&_8, "mysqli_error", NULL, _7);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_9);
		ZEPHIR_CONCAT_SV(_9, "Db not supported exception:", _8);
		_10 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(&_11, "mysqli_errno", &_5, _10);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(NULL, _6, "__construct", NULL, _9, _11, ZEPHIR_GLOBAL(global_null), ZEPHIR_GLOBAL(global_null), sql);
		zephir_check_call_status();
		zephir_throw_exception_debug(_6, "fastorm/db/Drivers/MysqliDriver.zep", 151 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	} else {
		if (Z_TYPE_P(res) == IS_OBJECT) {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "createresultdriver", NULL, res);
			zephir_check_call_status();
			RETURN_MM();
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInfo) {

	HashTable *_8;
	HashPosition _7;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL, *_6 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *res, *matches, *m = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, *_5 = NULL, **_9, *hod, *_10 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(res);
	array_init(res);

	ZEPHIR_INIT_VAR(matches);
	array_init(matches);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "mysqli_info", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "#(.+?): +(\\d+) *#", 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 2);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match_all", &_4, _2, _1, matches, _3);
	zephir_check_temp_parameter(_2);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_5, "preg_last_error", &_6);
	zephir_check_call_status();
	if (zephir_is_true(_5)) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db Pcre Exception", "fastorm/db/Drivers/MysqliDriver.zep", 170);
		return;
	}
	zephir_is_iterable(matches, &_8, &_7, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 178);
	for (
	  ; zephir_hash_get_current_data_ex(_8, (void**) &_9, &_7) == SUCCESS
	  ; zephir_hash_move_forward_ex(_8, &_7)
	) {
		ZEPHIR_GET_HVALUE(m, _9);
		zephir_array_fetch_long(&hod, m, 1, PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 175 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(_10);
		zephir_array_fetch_long(&_10, m, 2, PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 176 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_LONG(_2, zephir_get_intval(_10));
		zephir_array_update_zval(&res, hod, &_2, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(res);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *_0, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "mysqli_affected_rows", &_2, _0);
	zephir_check_call_status();
	if (ZEPHIR_IS_LONG_IDENTICAL(_1, -1)) {
		RETURN_MM_BOOL(0);
	} else {
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_affected_rows", &_2, _3);
		zephir_check_call_status();
		RETURN_MM();
	}

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInsertId) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *sequence, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &sequence);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_insert_id", NULL, _0);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, begin) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "START TRANSACTION", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, commit) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "RELEASE SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "COMMIT", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, rollback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "ROLLBACK TO SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "ROLLBACK", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResource) {

	zval *_1, *_3, *_4;
	zend_bool _0, _2;

	ZEPHIR_MM_GROW();

	_0 = zephir_isset_property(this_ptr, SS("connection") TSRMLS_CC);
	if (_0) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		_0 = zephir_isset_property(_1, SS("thread_id") TSRMLS_CC);
	}
	_2 = _0;
	if (_2) {
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_OBS_VAR(_4);
		zephir_read_property(&_4, _3, SL("thread_id"), PH_NOISY_CC);
		_2 = zephir_is_true(_4);
	}
	if (_2) {
		RETURN_MM_MEMBER(this_ptr, "connection");
	} else {
		RETURN_MM_NULL();
	}

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, createResultDriver) {

	zval *resourc, *res;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &resourc);



	if (!(zephir_is_instance_of(resourc, SL("mysqli_result") TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'resourc' must be an instance of 'mysqli_result'", "", 0);
		return;
	}
	ZEPHIR_INIT_VAR(res);
	if (zephir_clone(res, this_ptr TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	zephir_update_property_zval(res, SL("resultSet"), resourc TSRMLS_CC);
	RETURN_CCTOR(res);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escape) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_6 = NULL;
	zval *value, *type, *_0, *_1 = NULL, *_3 = NULL, _4 = zval_used_for_init, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &type);



	do {
		if (ZEPHIR_IS_STRING(type, "s")) {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&_1, "mysqli_real_escape_string", &_2, _0, value);
			zephir_check_call_status();
			ZEPHIR_CONCAT_SVS(return_value, "'", _1, "'");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "bin")) {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&_1, "mysqli_real_escape_string", &_2, _0, value);
			zephir_check_call_status();
			ZEPHIR_CONCAT_SVS(return_value, "_binary'", _1, "'");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "n")) {
			ZEPHIR_INIT_VAR(_3);
			ZEPHIR_SINIT_VAR(_4);
			ZVAL_STRING(&_4, "`", 0);
			ZEPHIR_SINIT_VAR(_5);
			ZVAL_STRING(&_5, "``", 0);
			zephir_fast_str_replace(_3, &_4, &_5, value);
			ZEPHIR_CONCAT_SVS(return_value, "`", _3, "`");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "b")) {
			if (zephir_is_true(value)) {
				ZEPHIR_CONCAT_SS(return_value, "1", "");
				RETURN_MM();
			} else {
				ZEPHIR_CONCAT_SS(return_value, "0", "");
				RETURN_MM();
			}
		}
		if (ZEPHIR_IS_STRING(type, "d")) {
			if (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "'Y-m-d'", 0);
				ZEPHIR_RETURN_CALL_METHOD(value, "format", NULL, _3);
				zephir_check_temp_parameter(_3);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				ZEPHIR_SINIT_NVAR(_4);
				ZVAL_STRING(&_4, "'Y-m-d'", 0);
				ZEPHIR_RETURN_CALL_FUNCTION("date", &_6, &_4, value);
				zephir_check_call_status();
				RETURN_MM();
			}
		}
		if (ZEPHIR_IS_STRING(type, "t")) {
			if (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "'Y-m-d H:i:s'", 0);
				ZEPHIR_RETURN_CALL_METHOD(value, "format", NULL, _3);
				zephir_check_temp_parameter(_3);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				ZEPHIR_SINIT_NVAR(_4);
				ZVAL_STRING(&_4, "'Y-m-d H:i:s'", 0);
				ZEPHIR_RETURN_CALL_FUNCTION("date", &_6, &_4, value);
				zephir_check_call_status();
				RETURN_MM();
			}
		}
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Unsupported type.", "fastorm/db/Drivers/MysqliDriver.zep", 323);
		return;
	} while(0);

	RETURN_MM_STRING("", 1);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escapeLike) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zval *value = NULL, *pos, *_0, _1, _2, _3, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &pos);

	ZEPHIR_SEPARATE_PARAM(value);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "\\", 0);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "\\\\", 0);
	zephir_fast_str_replace(_0, &_1, &_2, value);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_STRING(&_3, "\x00\n\r\\'%_", 0);
	ZEPHIR_CALL_FUNCTION(&value, "addcslashes", &_4, _0, &_3);
	zephir_check_call_status();
	if (ZEPHIR_LE_LONG(pos, 0)) {
		ZEPHIR_INIT_VAR(_5);
		if (ZEPHIR_GE_LONG(pos, 0)) {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "%'", 1);
		} else {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "'", 1);
		}
		ZEPHIR_CONCAT_SVV(return_value, "'%", value, _5);
		RETURN_MM();
	} else {
		ZEPHIR_INIT_LNVAR(_5);
		if (ZEPHIR_GE_LONG(pos, 0)) {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "%'", 1);
		} else {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "'", 1);
		}
		ZEPHIR_CONCAT_SVV(return_value, "'", value, _5);
		RETURN_MM();
	}

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, unescape) {

	zval *value, *type;

	zephir_fetch_params(0, 2, 0, &value, &type);



	if (ZEPHIR_IS_STRING_IDENTICAL(type, "bin")) {
		RETURN_CCTORW(value);
	}
	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Unsupported type.", "fastorm/db/Drivers/MysqliDriver.zep", 358);
	return;

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, applyLimit) {

	zend_bool _0;
	int limit, offset;
	zval *sql_param = NULL, *limit_param = NULL, *offset_param = NULL, _1 = zval_used_for_init;
	zval *sql = NULL, *lmtString = NULL, *ofsString = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &sql_param, &limit_param, &offset_param);

	zephir_get_strval(sql, sql_param);
	limit = zephir_get_intval(limit_param);
	offset = zephir_get_intval(offset_param);


	_0 = limit >= 0;
	if (!(_0)) {
		_0 = offset > 0;
	}
	if (_0) {
		limit = limit;
		offset = offset;
		if (limit < 0) {
			ZEPHIR_INIT_VAR(lmtString);
			ZEPHIR_CONCAT_SS(lmtString, "18446744073709551615", "");
		} else {
			ZEPHIR_SINIT_VAR(_1);
			ZVAL_LONG(&_1, limit);
			ZEPHIR_INIT_NVAR(lmtString);
			ZEPHIR_CONCAT_VS(lmtString, &_1, "");
		}
		if (offset > 0) {
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, offset);
			ZEPHIR_INIT_VAR(ofsString);
			ZEPHIR_CONCAT_SV(ofsString, " OFFSET ", &_1);
		} else {
			ZEPHIR_INIT_NVAR(ofsString);
			ZVAL_EMPTY_STRING(ofsString);
		}
		ZEPHIR_CONCAT_VSVV(return_value, sql, " LIMIT ", lmtString, ofsString);
		RETURN_MM();
	}
	RETURN_MM_STRING("", 1);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __destruct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _1, _3;
	zval *_0, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("autoFree"), PH_NOISY_CC);
	_1 = zephir_is_true(_0);
	if (_1) {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "getresultresource",  NULL);
		zephir_check_call_status();
		_1 = zephir_is_true(_2);
	}
	_3 = _1;
	if (_3) {
		ZEPHIR_CALL_METHOD(&_4, this_ptr, "free",  NULL);
		zephir_check_call_status();
		_3 = zephir_is_true(_4);
	}
	RETURN_MM_BOOL(_3);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getRowCount) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (!(zephir_is_true(_0))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: Row count is not available for unbuffered queries.", "fastorm/db/Drivers/MysqliDriver.zep", 411);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_num_rows", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, fetchRow) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *associative, *_0, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &associative);



	if (zephir_is_true(associative)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_1);
		ZVAL_LONG(&_1, 1);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_fetch_array", &_2, _0, &_1);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_LONG(&_1, 2);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_fetch_array", &_2, _0, &_1);
		zephir_check_call_status();
		RETURN_MM();
	}

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, seek) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *row, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (!(zephir_is_true(_0))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: Cannot seek an unbuffered result set.", "fastorm/db/Drivers/MysqliDriver.zep", 441);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_data_seek", NULL, _1, row);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, free) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) != IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(NULL, "mysqli_free_result", NULL, _1);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("resultSet"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultColumns) {

	zval *_23 = NULL;
	zephir_fcall_cache_entry *_18 = NULL;
	zend_bool _14;
	HashTable *_5;
	HashPosition _4;
	int ZEPHIR_LAST_CALL_STATUS, _15;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_10 = NULL, *_12 = NULL;
	zval *_0, *consts = NULL, *key = NULL, *value = NULL, *_2 = NULL, *_3, **_6, _7 = zval_used_for_init, _8 = zval_used_for_init, *_9 = NULL, *_11 = NULL, *row = NULL, *count, *i = NULL, *table = NULL, *_13, *columns, *_16 = NULL, *_17, *_19 = NULL, *_20 = NULL, *_21 = NULL, *_22 = NULL, *_24;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(columns);
	array_init(columns);

	zephir_read_static_property_ce(&_0, fastorm_db_drivers_mysqlidriver_ce, SL("types") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_CALL_FUNCTION(&consts, "get_defined_constants", &_1, ZEPHIR_GLOBAL(global_true));
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_2);
		array_init(_2);
		zephir_update_static_property_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), _2 TSRMLS_CC);
		zephir_array_fetch_string(&_3, consts, SL("mysqli"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 471 TSRMLS_CC);
		zephir_is_iterable(_3, &_5, &_4, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 476);
		for (
		  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
		  ; zephir_hash_move_forward_ex(_5, &_4)
		) {
			ZEPHIR_GET_HMKEY(key, _5, _4);
			ZEPHIR_GET_HVALUE(value, _6);
			ZEPHIR_SINIT_NVAR(_7);
			ZVAL_STRING(&_7, "MYSQLI_TYPE_", 0);
			ZEPHIR_SINIT_NVAR(_8);
			ZVAL_LONG(&_8, 12);
			ZEPHIR_CALL_FUNCTION(&_9, "strncmp", &_10, key, &_7, &_8);
			zephir_check_call_status();
			if (ZEPHIR_IS_LONG_IDENTICAL(_9, 0)) {
				ZEPHIR_SINIT_NVAR(_7);
				ZVAL_LONG(&_7, 12);
				ZEPHIR_CALL_FUNCTION(&_11, "substr", &_12, key, &_7);
				zephir_check_call_status();
				zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_11 TSRMLS_CC, SL("z"), 1, value);
			}
		}
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_2 TSRMLS_CC, SL("l"), 1, 1);
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_2 TSRMLS_CC, SL("l"), 1, 2);
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_2 TSRMLS_CC, SL("l"), 1, 3);
	}
	_13 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_9, "mysqli_num_fields", NULL, _13);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(count);
	ZVAL_LONG(count, (zephir_get_numberval(_9) - 1));
	ZEPHIR_CPY_WRT(_16, count);
	_15 = 0;
	_14 = 0;
	if (ZEPHIR_GE_LONG(_16, _15)) {
		while (1) {
			if (_14) {
				_15++;
				if (!(ZEPHIR_GE_LONG(_16, _15))) {
					break;
				}
			} else {
				_14 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _15);
			_17 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&row, "mysqli_fetch_field_direct", &_18, _17, i);
			zephir_check_call_status();
			ZEPHIR_OBS_NVAR(_19);
			zephir_read_property(&_19, row, SL("table"), PH_NOISY_CC);
			if (zephir_is_true(_19)) {
				ZEPHIR_OBS_NVAR(_20);
				zephir_read_property(&_20, row, SL("table"), PH_NOISY_CC);
				ZEPHIR_OBS_NVAR(_21);
				zephir_read_property(&_21, row, SL("name"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_22);
				ZEPHIR_CONCAT_VSV(_22, _20, ".", _21);
				ZEPHIR_CPY_WRT(table, _22);
			} else {
				ZEPHIR_OBS_NVAR(table);
				zephir_read_property(&table, row, SL("name"), PH_NOISY_CC);
			}
			ZEPHIR_INIT_NVAR(_23);
			array_init_size(_23, 7);
			ZEPHIR_OBS_NVAR(_20);
			zephir_read_property(&_20, row, SL("name"), PH_NOISY_CC);
			zephir_array_update_string(&_23, SL("name"), &_20, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_20);
			zephir_read_property(&_20, row, SL("orgtable"), PH_NOISY_CC);
			zephir_array_update_string(&_23, SL("table"), &_20, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_23, SL("fullname"), &table, PH_COPY | PH_SEPARATE);
			_24 = zephir_fetch_static_property_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types") TSRMLS_CC);
			ZEPHIR_OBS_NVAR(_20);
			ZEPHIR_OBS_NVAR(_21);
			zephir_read_property(&_21, row, SL("type"), PH_NOISY_CC);
			zephir_array_fetch(&_20, _24, _21, PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 497 TSRMLS_CC);
			zephir_array_update_string(&_23, SL("nativetype"), &_20, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_23, SL("vendor"), &row, PH_COPY | PH_SEPARATE);
			zephir_array_append(&columns, _23, PH_SEPARATE, "fastorm/db/Drivers/MysqliDriver.zep", 498);
		}
	}
	RETURN_CCTOR(columns);

}

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultResource) {

	zend_bool _1;
	zval *_0, *_2, *_3;

	ZEPHIR_MM_GROW();

	zephir_update_property_this(this_ptr, SL("autoFree"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	_1 = Z_TYPE_P(_0) == IS_NULL;
	if (!(_1)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_OBS_VAR(_3);
		zephir_read_property(&_3, _2, SL("type"), PH_NOISY_CC);
		_1 = Z_TYPE_P(_3) == IS_NULL;
	}
	if (_1) {
		RETURN_MM_NULL();
	} else {
		RETURN_MM_MEMBER(this_ptr, "resultSet");
	}

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Event) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Event, fastorm, db_event, fastorm_db_event_method_entry, 0);

	/** @var DibiConnection */
	zend_declare_property_null(fastorm_db_event_ce, SL("connection"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_event_ce, SL("type"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var string */
	zend_declare_property_null(fastorm_db_event_ce, SL("sql"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var DibiResult|DibiDriverException|null */
	zend_declare_property_null(fastorm_db_event_ce, SL("result"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var float */
	zend_declare_property_null(fastorm_db_event_ce, SL("time"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_event_ce, SL("count"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("types"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("elapsedTime"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("numOfQueries"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("totalTime"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("lastSql"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	/** event type */
	zend_declare_class_constant_long(fastorm_db_event_ce, SL("CONNECT"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("SELECT"), 4 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("INSERT"), 8 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("DELETE"), 16 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("UPDATE"), 32 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("QUERY"), 60 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("BEGIN"), 64 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("COMMIT"), 128 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("ROLLBACK"), 256 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("TRANSACTION"), 448 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("ALL"), 1023 TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Event, __construct) {

	zend_bool _6;
	zval *_4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_9 = NULL;
	zval *connection, *type, *sqlCommand = NULL, *matches, *_0, *_1 = NULL, *_3, *_5, *_7, *_8 = NULL, *_10, *_11, *_12;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &connection, &type, &sqlCommand);

	if (!sqlCommand) {
		sqlCommand = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("type"), type TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_fast_trim(_0, sqlCommand, NULL , ZEPHIR_TRIM_BOTH TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("sql"), _0 TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "microtime", &_2, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	zephir_negate(_1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("time"), _1 TSRMLS_CC);
	_3 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("types") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_3)) {
		ZEPHIR_INIT_VAR(_4);
		array_init_size(_4, 6);
		add_assoc_long_ex(_4, SS("SELECT"), 4);
		add_assoc_long_ex(_4, SS("UPDATE"), 32);
		add_assoc_long_ex(_4, SS("INSERT"), 8);
		add_assoc_long_ex(_4, SS("DELETE"), 16);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("types"), _4 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), _5 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("totalTime"), _5 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("numOfQueries"), _5 TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);
	_6 = ZEPHIR_IS_LONG_IDENTICAL(type, 60);
	if (_6) {
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("sql"), PH_NOISY_CC);
		ZEPHIR_INIT_VAR(_7);
		ZVAL_STRING(_7, "#\\(?\\s*(SELECT|UPDATE|INSERT|DELETE)#iA", 0);
		Z_SET_ISREF_P(matches);
		ZEPHIR_CALL_FUNCTION(&_8, "preg_match", &_9, _7, _5, matches);
		zephir_check_temp_parameter(_7);
		Z_UNSET_ISREF_P(matches);
		zephir_check_call_status();
		_6 = zephir_is_true(_8);
	}
	if (_6) {
		_10 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("types") TSRMLS_CC);
		ZEPHIR_INIT_BNVAR(_7);
		zephir_array_fetch_long(&_12, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Event.zep", 69 TSRMLS_CC);
		zephir_fast_strtoupper(_7, _12);
		zephir_array_fetch(&_11, _10, _7, PH_NOISY | PH_READONLY, "fastorm/db/Event.zep", 69 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("type"), _11 TSRMLS_CC);
	}
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	_5 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("numOfQueries") TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_10);
	ZVAL_LONG(_10, (zephir_get_numberval(_5) + 1));
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("numOfQueries"), _10 TSRMLS_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("lastSql"), sqlCommand TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Event, done) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *result = NULL, *e = NULL, *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &result);

	if (!result) {
		result = ZEPHIR_GLOBAL(global_null);
	}


	zephir_update_property_this(this_ptr, SL("result"), result TSRMLS_CC);
	if (Z_TYPE_P(result) != IS_NULL) {

		/* try_start_1: */

			if (zephir_instance_of_ev(result, fastorm_db_result_ce TSRMLS_CC)) {
				ZEPHIR_INIT_ZVAL_NREF(_0);
				ZVAL_LONG(_0, zephir_fast_count_int(result TSRMLS_CC));
				zephir_update_property_this(this_ptr, SL("count"), _0 TSRMLS_CC);
			} else {
				zephir_update_property_this(this_ptr, SL("count"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			}

		try_end_1:

		if (EG(exception)) {
			ZEPHIR_CPY_WRT(e, EG(exception));
			if (zephir_is_instance_of(e, SL("Fastorm\\Db\\DbException") TSRMLS_CC)) {
				zend_clear_exception(TSRMLS_C);
				zephir_update_property_this(this_ptr, SL("count"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			}
		}
	}
	ZEPHIR_CALL_FUNCTION(&_1, "microtime", &_2, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("time"), _1 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("time"), PH_NOISY_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("time"), PH_NOISY_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("totalTime"), _0 TSRMLS_CC);
	RETURN_THIS();

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_exceptions.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_IResultDriver) {

	ZEPHIR_REGISTER_INTERFACE(Fastorm\\Db, IResultDriver, fastorm, db_iresultdriver, fastorm_db_iresultdriver_method_entry);

	return SUCCESS;

}

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getRowCount);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, seek);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, fetchRow);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, free);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getResultColumns);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getResultResource);

ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, unescape);




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Query) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Query, fastorm, db_query, fastorm_db_query_method_entry, 0);

	zend_declare_property_null(fastorm_db_query_ce, SL("masks"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("modifiers"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("separators"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("swithes"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("connection"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("command"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("setups"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("cursor"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("clauses"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("flags"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_class_constant_bool(fastorm_db_query_ce, SL("REMOVE"), 0 TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_TEXT"), "s" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_BINARY"), "bin" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_BOOL"), "b" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_INTEGER"), "i" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_FLOAT"), "f" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_DATE"), "d" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_DATETIME"), "t" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_TIME"), "t" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("IDENTIFIER"), "n" TSRMLS_CC);

	/** sorting order */
	zend_declare_class_constant_string(fastorm_db_query_ce, SL("ASC"), "ASC" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("DESC"), "DESC" TSRMLS_CC);

	zend_class_implements(fastorm_db_query_ce TSRMLS_CC, 1, zend_ce_aggregate);
	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Query, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *connection, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &connection);



	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "staticinitialize", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	zephir_update_property_this(this_ptr, SL("flags"), _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	zephir_update_property_this(this_ptr, SL("setups"), _2 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Query, staticInitialize) {

	zval *_2;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 6);
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 12);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "SELECT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "DISTINCT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "FROM", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "GROUP BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "HAVING", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "OFFSET", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("SELECT"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 7);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "UPDATE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "SET", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("UPDATE"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 6);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "INSERT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "INTO", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "VALUES", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "SELECT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("INSERT"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 8);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "DELETE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "FROM", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "USING", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("DELETE"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("masks"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 12);
	add_assoc_stringl_ex(_0, SS("SELECT"), SL("%n"), 1);
	add_assoc_stringl_ex(_0, SS("FROM"), SL("%n"), 1);
	add_assoc_stringl_ex(_0, SS("IN"), SL("%in"), 1);
	add_assoc_stringl_ex(_0, SS("VALUES"), SL("%l"), 1);
	add_assoc_stringl_ex(_0, SS("SET"), SL("%a"), 1);
	add_assoc_stringl_ex(_0, SS("WHERE"), SL("%and"), 1);
	add_assoc_stringl_ex(_0, SS("HAVING"), SL("%and"), 1);
	add_assoc_stringl_ex(_0, SS("ORDER BY"), SL("%by"), 1);
	add_assoc_stringl_ex(_0, SS("GROUP BY"), SL("%by"), 1);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("modifiers"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 14);
	add_assoc_stringl_ex(_0, SS("SELECT"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("FROM"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("WHERE"), SL("AND"), 1);
	add_assoc_stringl_ex(_0, SS("GROUP BY"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("HAVING"), SL("AND"), 1);
	add_assoc_stringl_ex(_0, SS("ORDER BY"), SL(","), 1);
	zephir_array_update_string(&_0, SL("LIMIT"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_0, SL("OFFSET"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	add_assoc_stringl_ex(_0, SS("SET"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("VALUES"), SL(","), 1);
	zephir_array_update_string(&_0, SL("INTO"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("separators"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 6);
	add_assoc_stringl_ex(_0, SS("JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("INNER JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("LEFT JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("RIGHT JOIN"), SL("FROM"), 1);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("swithes"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Query, __call) {

	HashTable *_20;
	HashPosition _19;
	zephir_nts_static zephir_fcall_cache_entry *_13 = NULL, *_18 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool removeArray, _0, _11, _14, _15;
	zval *clause = NULL, *clauseArgs = NULL, *sep, *argument = NULL, *cursor = NULL, *_1, *_2 = NULL, *_3, *_4, *_5, *_6, *_7, *_8, *_9 = NULL, *_10, _12, *_16 = NULL, *_17 = NULL, **_21;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &clause, &clauseArgs);

	ZEPHIR_SEPARATE_PARAM(clause);
	ZEPHIR_SEPARATE_PARAM(clauseArgs);


	_0 = zephir_fast_count_int(clauseArgs TSRMLS_CC) == 1;
	if (_0) {
		zephir_array_fetch_long(&_1, clauseArgs, 0, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 107 TSRMLS_CC);
		_0 = ZEPHIR_IS_FALSE_IDENTICAL(_1);
	}
	removeArray = _0;
	ZEPHIR_CALL_SELF(&_2, "_formatclause", NULL, clause);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(clause, _2);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_EMPTY(_3)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "initialize", NULL, clause);
		zephir_check_call_status();
	}
	_4 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("swithes") TSRMLS_CC);
	if (zephir_array_isset(_4, clause)) {
		_5 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("swithes") TSRMLS_CC);
		zephir_array_fetch(&_6, _5, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 117 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("cursor"), _6 TSRMLS_CC);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _7);
	}
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
	if (zephir_array_key_exists(_5, clause TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _7);
		if (removeArray) {
			zephir_update_property_array(this_ptr, SL("clauses"), cursor, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			RETURN_THIS();
		}
		_7 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("separators") TSRMLS_CC);
		if (zephir_array_isset(_7, clause)) {
			_8 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("separators") TSRMLS_CC);
			ZEPHIR_OBS_VAR(sep);
			zephir_array_fetch(&sep, _8, clause, PH_NOISY, "fastorm/db/Query.zep", 132 TSRMLS_CC);
			if (ZEPHIR_IS_FALSE_IDENTICAL(sep)) {
				ZEPHIR_INIT_VAR(_9);
				array_init(_9);
				zephir_update_property_array(this_ptr, SL("clauses"), clause, _9 TSRMLS_CC);
			} else {
				_10 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
				zephir_array_fetch(&_6, _10, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 136 TSRMLS_CC);
				if (!(ZEPHIR_IS_EMPTY(_6))) {
					zephir_update_property_array_multi(this_ptr, SL("clauses"), &sep TSRMLS_CC, SL("za"), 1, clause);
				}
			}
		}
	} else {
		if (removeArray) {
			RETURN_THIS();
		}
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _5);
		zephir_update_property_array_multi(this_ptr, SL("clauses"), &clause TSRMLS_CC, SL("za"), 1, cursor);
	}
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
	if (Z_TYPE_P(_5) == IS_NULL) {
		zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _7);
	}
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
	if (!(zephir_array_isset(_5, cursor))) {
		ZEPHIR_INIT_NVAR(_9);
		array_init(_9);
		zephir_update_property_array(this_ptr, SL("clauses"), cursor, _9 TSRMLS_CC);
	}
	if (zephir_fast_count_int(clauseArgs TSRMLS_CC) == 1) {
		ZEPHIR_OBS_VAR(argument);
		zephir_array_fetch_long(&argument, clauseArgs, 0, PH_NOISY, "fastorm/db/Query.zep", 164 TSRMLS_CC);
		if (ZEPHIR_IS_TRUE_IDENTICAL(argument)) {
			RETURN_THIS();
		}
		_11 = Z_TYPE_P(argument) == IS_STRING;
		if (_11) {
			ZEPHIR_SINIT_VAR(_12);
			ZVAL_STRING(&_12, "#^[a-z:_][a-z0-9_.:]*$#i", 0);
			ZEPHIR_CALL_FUNCTION(&_2, "preg_match", &_13, &_12, argument);
			zephir_check_call_status();
			_11 = zephir_is_true(_2);
		}
		if (_11) {
			ZEPHIR_INIT_NVAR(clauseArgs);
			array_init_size(clauseArgs, 3);
			ZEPHIR_INIT_NVAR(_9);
			ZVAL_STRING(_9, "%n", 1);
			zephir_array_fast_append(clauseArgs, _9);
			zephir_array_fast_append(clauseArgs, argument);
		} else {
			_14 = Z_TYPE_P(argument) == IS_ARRAY;
			if (!(_14)) {
				_15 = (zephir_is_instance_of(argument, SL("Traversable") TSRMLS_CC));
				if (_15) {
					_15 = !(zephir_is_instance_of(argument, SL("Fastorm\\Db\\self") TSRMLS_CC));
				}
				_14 = _15;
			}
			if (_14) {
				_5 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("modifiers") TSRMLS_CC);
				if (zephir_array_isset(_5, clause)) {
					ZEPHIR_INIT_NVAR(clauseArgs);
					array_init_size(clauseArgs, 3);
					_7 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("modifiers") TSRMLS_CC);
					ZEPHIR_OBS_VAR(_16);
					zephir_array_fetch(&_16, _7, clause, PH_NOISY, "fastorm/db/Query.zep", 177 TSRMLS_CC);
					zephir_array_fast_append(clauseArgs, _16);
					zephir_array_fast_append(clauseArgs, argument);
				} else {
					Z_SET_ISREF_P(argument);
					ZEPHIR_CALL_FUNCTION(&_17, "key", &_18, argument);
					Z_UNSET_ISREF_P(argument);
					zephir_check_call_status();
					if (Z_TYPE_P(_17) == IS_STRING) {
						ZEPHIR_INIT_NVAR(clauseArgs);
						array_init_size(clauseArgs, 3);
						ZEPHIR_INIT_NVAR(_9);
						ZVAL_STRING(_9, "%a", 1);
						zephir_array_fast_append(clauseArgs, _9);
						zephir_array_fast_append(clauseArgs, argument);
					}
				}
			}
		}
	}
	zephir_is_iterable(clauseArgs, &_20, &_19, 0, 0, "fastorm/db/Query.zep", 195);
	for (
	  ; zephir_hash_get_current_data_ex(_20, (void**) &_21, &_19) == SUCCESS
	  ; zephir_hash_move_forward_ex(_20, &_19)
	) {
		ZEPHIR_GET_HVALUE(argument, _21);
		if (zephir_is_instance_of(argument, SL("Fastorm\\Db\\self") TSRMLS_CC)) {
			ZEPHIR_CALL_METHOD(&_17, argument, "__tostring",  NULL);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(argument);
			ZEPHIR_CONCAT_SVS(argument, "(", _17, ")");
		}
		ZEPHIR_OBS_NVAR(_16);
		zephir_read_property_this(&_16, this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_update_property_array_multi(this_ptr, SL("clauses"), &argument TSRMLS_CC, SL("za"), 1, _16);
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_Db_Query, initialize) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zval *clause_param = NULL, *_0, *_1, *_2, *_3 = NULL, *_5;
	zval *clause = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &clause_param);

	zephir_get_strval(clause, clause_param);


	_0 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
	if (zephir_array_isset(_0, clause)) {
		_1 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
		zephir_array_fetch(&_2, _1, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 200 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&_3, "array_fill_keys", &_4, _2, ZEPHIR_GLOBAL(global_null));
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("clauses"), _3 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_VAR(_5);
		array_init(_5);
		zephir_update_property_this(this_ptr, SL("clauses"), _5 TSRMLS_CC);
	}
	zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("command"), clause TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Query, _formatClause) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	zend_bool _0;
	zval *s_param = NULL, *_1, _2, _3, *_4 = NULL;
	zval *s = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &s_param);

	zephir_get_strval(s, s_param);
	ZEPHIR_SEPARATE_PARAM(s);


	_0 = ZEPHIR_IS_STRING_IDENTICAL(s, "order");
	if (!(_0)) {
		_0 = ZEPHIR_IS_STRING_IDENTICAL(s, "group");
	}
	if (_0) {
		zephir_concat_self_str(&s, "By", sizeof("By")-1 TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "#[a-z](?=[A-Z])#", 0);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_STRING(&_3, "$0 ", 0);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_5, &_2, &_3, s);
	zephir_check_call_status();
	zephir_fast_strtoupper(_1, _4);
	zephir_get_strval(_6, _1);
	RETURN_CTOR(_6);

}

static PHP_METHOD(Fastorm_Db_Query, getCommand) {


	RETURN_MEMBER_QUICK(this_ptr, "command", 1071038916UL);

}

static PHP_METHOD(Fastorm_Db_Query, getConnection) {


	RETURN_MEMBER_QUICK(this_ptr, "connection", 514987893UL);

}

static PHP_METHOD(Fastorm_Db_Query, setupResult) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	zval *method, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &method);



	ZEPHIR_CALL_FUNCTION(&_0, "func_get_args", &_1);
	zephir_check_call_status();
	zephir_update_property_array_append(this_ptr, SL("setups"), _0 TSRMLS_CC);
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_Db_Query, execute) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *ret = NULL, *res = NULL, *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &ret);

	if (!ret) {
		ret = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(&_0, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&res, this_ptr, "query", NULL, _0);
	zephir_check_call_status();
	if (ZEPHIR_IS_STRING_IDENTICAL(ret, "n")) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_METHOD(_1, "getinsertid", NULL);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		RETURN_CCTOR(res);
	}

}

static PHP_METHOD(Fastorm_Db_Query, fetchFirst) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_3;
	zval *ret = NULL, *_0, *_1 = NULL, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_STRING_IDENTICAL(_0, "SELECT")) {
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "%lmt", 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_INIT_NVAR(_4);
		ZVAL_LONG(_4, 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _3);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&ret, _1, "fetchrow",  NULL);
		zephir_check_call_status();
	} else {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&ret, _1, "fetchrow",  NULL);
		zephir_check_call_status();
	}
	if (ZEPHIR_IS_FALSE_IDENTICAL(ret)) {
		RETURN_MM_NULL();
	} else {
		RETURN_CCTOR(ret);
	}

}

static PHP_METHOD(Fastorm_Db_Query, fetchSingle) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_3;
	zval *_0, *_1 = NULL, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_STRING_IDENTICAL(_0, "SELECT")) {
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "%lmt", 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_INIT_NVAR(_4);
		ZVAL_LONG(_4, 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _3);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_RETURN_CALL_METHOD(_1, "fetchsingle", NULL);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_RETURN_CALL_METHOD(_1, "fetchsingle", NULL);
		zephir_check_call_status();
		RETURN_MM();
	}

}

static PHP_METHOD(Fastorm_Db_Query, fetchAssoc) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *assoc, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &assoc);



	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "fetchassoc", NULL, assoc);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Query, fetchPairs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *key_param = NULL, *value_param = NULL, *_0 = NULL, *_1 = NULL;
	zval *key = NULL, *value = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &key_param, &value_param);

	if (!key_param) {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	} else {
		zephir_get_strval(key, key_param);
	}
	if (!value_param) {
		ZEPHIR_INIT_VAR(value);
		ZVAL_EMPTY_STRING(value);
	} else {
		zephir_get_strval(value, value_param);
	}


	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "fetchpairs", NULL, key, value);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Query, getIterator) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_2;
	zval *offset = NULL, *limit = NULL, *_0 = NULL, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &offset, &limit);

	if (!offset) {
		offset = ZEPHIR_GLOBAL(global_null);
	}
	if (!limit) {
		limit = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 4);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "%ofs %lmt", 1);
	zephir_array_fast_append(_2, _3);
	zephir_array_fast_append(_2, offset);
	zephir_array_fast_append(_2, limit);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _2);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getiterator", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Query, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1;
	zval *_0 = NULL, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 4);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "SELECT COUNT(*) FROM (%ex", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	zephir_array_fast_append(_1, _3);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, ") AS [data]", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_3, _0, "fetchsingle",  NULL);
	zephir_check_call_status();
	RETURN_MM_LONG(zephir_get_intval(_3));

}

static PHP_METHOD(Fastorm_Db_Query, query) {

	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL;
	zval *_6 = NULL;
	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *queryArgs, *res = NULL, *setup = NULL, *_0, *_1, **_4, *_5 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &queryArgs);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&res, _0, "queryarray", NULL, queryArgs);
	zephir_check_call_status();
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("setups"), PH_NOISY_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/db/Query.zep", 328);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(setup, _4);
		ZEPHIR_INIT_NVAR(_5);
		ZEPHIR_INIT_NVAR(_6);
		array_init_size(_6, 3);
		zephir_array_fast_append(_6, res);
		Z_SET_ISREF_P(setup);
		ZEPHIR_CALL_FUNCTION(&_7, "array_shift", &_8, setup);
		Z_UNSET_ISREF_P(setup);
		zephir_check_call_status();
		zephir_array_fast_append(_6, _7);
		ZEPHIR_CALL_USER_FUNC_ARRAY(_5, _6, setup);
		zephir_check_call_status();
	}
	RETURN_CCTOR(res);

}

static PHP_METHOD(Fastorm_Db_Query, __toString) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "translate", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Query, setFlag) {

	zval *flag = NULL, *value = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &flag, &value);

	ZEPHIR_SEPARATE_PARAM(flag);
	if (!value) {
		value = ZEPHIR_GLOBAL(global_true);
	}


	ZEPHIR_INIT_VAR(_0);
	zephir_fast_strtoupper(_0, flag);
	ZEPHIR_CPY_WRT(flag, _0);
	if (zephir_is_true(value)) {
		zephir_update_property_array(this_ptr, SL("flags"), flag, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
		zephir_array_unset(&_1, flag, PH_SEPARATE);
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_Db_Query, getFlag) {

	zval *flag, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &flag);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_fast_strtoupper(_1, flag);
	RETURN_MM_BOOL(zephir_array_isset(_0, _1));

}

static PHP_METHOD(Fastorm_Db_Query, _export) {

	zend_bool _7;
	HashTable *_5, *_12, *_15;
	HashPosition _4, _11, _14;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *clause = NULL, *additionalArgs = NULL, *data = NULL, *cls = NULL, *statement = NULL, *arg = NULL, *ret, *_0 = NULL, *_1, *_2, *_3, **_6, *_8 = NULL, *_9 = NULL, *_10, **_13, **_16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &clause, &additionalArgs);

	if (!clause) {
		ZEPHIR_CPY_WRT(clause, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(clause);
	}
	if (!additionalArgs) {
		additionalArgs = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(ret);
	array_init(ret);
	if (Z_TYPE_P(clause) == IS_NULL) {
		ZEPHIR_OBS_VAR(data);
		zephir_read_property_this(&data, this_ptr, SL("clauses"), PH_NOISY_CC);
	} else {
		ZEPHIR_CALL_SELF(&_0, "_formatclause", NULL, clause);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(clause, _0);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
		if (zephir_array_key_exists(_1, clause TSRMLS_CC)) {
			ZEPHIR_INIT_NVAR(data);
			array_init_size(data, 2);
			_2 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
			ZEPHIR_OBS_VAR(_3);
			zephir_array_fetch(&_3, _2, clause, PH_NOISY, "fastorm/db/Query.zep", 379 TSRMLS_CC);
			zephir_array_update_string(&data, SL("clause"), &_3, PH_COPY | PH_SEPARATE);
		} else {
			array_init(return_value);
			RETURN_MM();
		}
	}
	zephir_is_iterable(data, &_5, &_4, 0, 0, "fastorm/db/Query.zep", 397);
	for (
	  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
	  ; zephir_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HMKEY(cls, _5, _4);
		ZEPHIR_GET_HVALUE(statement, _6);
		if (Z_TYPE_P(statement) != IS_NULL) {
			zephir_array_append(&ret, cls, PH_SEPARATE, "fastorm/db/Query.zep", 387);
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
			_7 = ZEPHIR_IS_IDENTICAL(cls, _1);
			if (_7) {
				_2 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
				_7 = zephir_fast_count_int(_2 TSRMLS_CC) > 0;
			}
			if (_7) {
				ZEPHIR_INIT_NVAR(_8);
				ZEPHIR_INIT_NVAR(_9);
				_10 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
				zephir_array_keys(_9, _10 TSRMLS_CC);
				zephir_fast_join_str(_8, SL(" "), _9 TSRMLS_CC);
				zephir_array_append(&ret, _8, PH_SEPARATE, "fastorm/db/Query.zep", 389);
			}
			zephir_is_iterable(statement, &_12, &_11, 0, 0, "fastorm/db/Query.zep", 394);
			for (
			  ; zephir_hash_get_current_data_ex(_12, (void**) &_13, &_11) == SUCCESS
			  ; zephir_hash_move_forward_ex(_12, &_11)
			) {
				ZEPHIR_GET_HVALUE(arg, _13);
				zephir_array_append(&ret, arg, PH_SEPARATE, "fastorm/db/Query.zep", 392);
			}
		}
	}
	if (Z_TYPE_P(additionalArgs) != IS_NULL) {
		zephir_is_iterable(additionalArgs, &_15, &_14, 0, 0, "fastorm/db/Query.zep", 401);
		for (
		  ; zephir_hash_get_current_data_ex(_15, (void**) &_16, &_14) == SUCCESS
		  ; zephir_hash_move_forward_ex(_15, &_14)
		) {
			ZEPHIR_GET_HVALUE(statement, _16);
			zephir_array_append(&ret, statement, PH_SEPARATE, "fastorm/db/Query.zep", 399);
		}
	}
	RETURN_CCTOR(ret);

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Result) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Result, fastorm, db_result, fastorm_db_result_method_entry, 0);

	/** @var array  IResultDriver */
	zend_declare_property_null(fastorm_db_result_ce, SL("driver"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_result_ce, SL("_types"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool  Already fetched? Used for allowance for first seek(0) */
	zend_declare_property_bool(fastorm_db_result_ce, SL("fetched"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_result_ce, SL("rowClass"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_result_ce, SL("metadata"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Result, __construct) {

	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *driver, *metadata = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &driver, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_instance_of_ev(driver, fastorm_db_iresultdriver_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'driver' must be an instance of 'Fastorm\\Db\\IResultDriver'", "", 0);
		return;
	}
	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("driver"), driver TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("metadata"), metadata TSRMLS_CC);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "detecttypes", &_1);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Result, getResource) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getresultresource", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Result, free) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) != IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "free", NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("metadata"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Result, getResultDriver) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_RuntimeException, "Result-set was released from memory.", "fastorm/db/Result.zep", 56);
		return;
	}
	RETURN_MEMBER_QUICK(this_ptr, "driver", 656010865UL);

}

static PHP_METHOD(Fastorm_Db_Result, seek) {

	zend_bool _0;
	zval *row_param = NULL, *_1, *_2 = NULL, *_3 = NULL, *_4;
	int row, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row_param);

	row = zephir_get_intval(row_param);


	_0 = row != 0;
	if (_0) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("fetched"), PH_NOISY_CC);
		_0 = zephir_is_true(_1);
	}
	if (_0) {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "getresultdriver",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_4);
		ZVAL_LONG(_4, row);
		ZEPHIR_CALL_METHOD(&_3, _2, "seek", NULL, _4);
		zephir_check_call_status();
		RETURN_MM_BOOL(zephir_get_boolval(_3));
	} else {
		RETURN_MM_BOOL(1);
	}

}

static PHP_METHOD(Fastorm_Db_Result, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Result, getRowCount) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Result, getIterator) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, fastorm_db_resultiterator_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, this_ptr);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Result, fetchRow) {

	zend_class_entry *_5;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *row = NULL, *_0 = NULL, *_1 = NULL, *_3, *_4, *className;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, _0, "fetchrow", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	if (!(Z_TYPE_P(row) == IS_ARRAY)) {
		RETURN_MM_BOOL(0);
	}
	zephir_update_property_this(this_ptr, SL("fetched"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "normalize", &_2, row);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(row, _1);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	if (Z_TYPE_P(_3) != IS_NULL) {
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_METHOD(_4, "newinstance", NULL, row);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("rowClass"), PH_NOISY_CC);
		if (Z_TYPE_P(_4) != IS_NULL) {
			className = zephir_fetch_nproperty_this(this_ptr, SL("rowClass"), PH_NOISY_CC);
			_5 = zend_fetch_class(Z_STRVAL_P(className), Z_STRLEN_P(className), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
			object_init_ex(return_value, _5);
			if (zephir_has_constructor(return_value TSRMLS_CC)) {
				ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, row);
				zephir_check_call_status();
			}
			RETURN_MM();
		} else {
			RETURN_CCTOR(row);
		}
	}

}

static PHP_METHOD(Fastorm_Db_Result, fetchSingle) {

	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *row = NULL, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, _0, "fetchrow", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	if (!(Z_TYPE_P(row) == IS_ARRAY)) {
		RETURN_MM_BOOL(0);
	}
	zephir_update_property_this(this_ptr, SL("fetched"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "normalize", &_2, row);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(row, _1);
	Z_SET_ISREF_P(row);
	ZEPHIR_RETURN_CALL_FUNCTION("reset", &_3, row);
	Z_UNSET_ISREF_P(row);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Result, fetchPairs) {

	zval *_9 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zephir_fcall_cache_entry *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *key = NULL, *value = NULL, *row = NULL, *tmp, *data, *_0, *_1 = NULL, *_2, *_5 = NULL, *_6 = NULL, *_7 = NULL, *_8 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &key, &value);

	if (!key) {
		ZEPHIR_CPY_WRT(key, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(key);
	}
	if (!value) {
		ZEPHIR_CPY_WRT(value, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(value);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "seek", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  NULL);
	zephir_check_call_status();
	if (!(zephir_is_true(row))) {
		array_init(return_value);
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	if (Z_TYPE_P(value) == IS_NULL) {
		if (Z_TYPE_P(key) != IS_NULL) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Either none or both columns must be specified.", "fastorm/db/Result.zep", 182);
			return;
		}
		ZEPHIR_INIT_VAR(tmp);
		ZEPHIR_CALL_METHOD(&_1, row, "toarray",  NULL);
		zephir_check_call_status();
		zephir_array_keys(tmp, _1 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(key);
		zephir_array_fetch_long(&key, tmp, 0, PH_NOISY, "fastorm/db/Result.zep", 187 TSRMLS_CC);
		if (zephir_fast_count_int(row TSRMLS_CC) < 2) {
			while (1) {
				zephir_array_fetch(&_2, row, key, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 190 TSRMLS_CC);
				zephir_array_append(&data, _2, PH_SEPARATE, "fastorm/db/Result.zep", 190);
				ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
				zephir_check_call_status();
				if (!(zephir_is_true(row))) {
					break;
				}
			}
			//missing empty
			RETURN_CCTOR(data);
		}
		ZEPHIR_OBS_NVAR(value);
		zephir_array_fetch_long(&value, tmp, 1, PH_NOISY, "fastorm/db/Result.zep", 199 TSRMLS_CC);
	} else {
		ZEPHIR_CALL_FUNCTION(&_1, "property_exists", &_4, row, value);
		zephir_check_call_status();
		if (!(zephir_is_true(_1))) {
			ZEPHIR_INIT_VAR(_5);
			object_init_ex(_5, spl_ce_InvalidArgumentException);
			ZEPHIR_INIT_VAR(_6);
			ZEPHIR_CONCAT_SVS(_6, "Unknown value column '", value, "'.");
			ZEPHIR_CALL_METHOD(NULL, _5, "__construct", NULL, _6);
			zephir_check_call_status();
			zephir_throw_exception_debug(_5, "fastorm/db/Result.zep", 203 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		if (Z_TYPE_P(key) == IS_NULL) {
			while (1) {
				zephir_array_fetch(&_2, row, value, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 208 TSRMLS_CC);
				zephir_array_append(&data, _2, PH_SEPARATE, "fastorm/db/Result.zep", 208);
				ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
				zephir_check_call_status();
				if (!(zephir_is_true(row))) {
					break;
				}
			}
			RETURN_CCTOR(data);
		}
		ZEPHIR_CALL_FUNCTION(&_7, "property_exists", &_4, row, key);
		zephir_check_call_status();
		if (!(zephir_is_true(_7))) {
			ZEPHIR_INIT_LNVAR(_5);
			object_init_ex(_5, spl_ce_InvalidArgumentException);
			ZEPHIR_INIT_LNVAR(_6);
			ZEPHIR_CONCAT_SVS(_6, "Unknown key column '", key, "'.");
			ZEPHIR_CALL_METHOD(NULL, _5, "__construct", NULL, _6);
			zephir_check_call_status();
			zephir_throw_exception_debug(_5, "fastorm/db/Result.zep", 218 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	while (1) {
		zephir_array_fetch(&_2, row, value, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 223 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(_8);
		zephir_array_fetch(&_8, row, key, PH_NOISY, "fastorm/db/Result.zep", 223 TSRMLS_CC);
		zephir_get_strval(_9, _8);
		zephir_array_update_zval(&data, _9, &_2, PH_COPY | PH_SEPARATE);
		ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
		zephir_check_call_status();
		if (!(zephir_is_true(row))) {
			break;
		}
	}
	//missing empty
	RETURN_CCTOR(data);

}

static PHP_METHOD(Fastorm_Db_Result, detectTypes) {

	zephir_fcall_cache_entry *_6 = NULL;
	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *columns = NULL, *col = NULL, *nativetype = NULL, *e = NULL, *_0, *_1 = NULL, **_4, *_5 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(_0);
		array_init(_0);
		zephir_update_property_this(this_ptr, SL("_types"), _0 TSRMLS_CC);
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "getresultdriver",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(&columns, _1, "getresultcolumns",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_is_iterable(columns, &_3, &_2, 0, 0, "fastorm/db/Result.zep", 251);
		for (
		  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
		  ; zephir_hash_move_forward_ex(_3, &_2)
		) {
			ZEPHIR_GET_HVALUE(col, _4);
			ZEPHIR_OBS_NVAR(nativetype);
			zephir_array_fetch_string(&nativetype, col, SL("nativetype"), PH_NOISY, "fastorm/db/Result.zep", 248 TSRMLS_CC);
			ZEPHIR_CALL_CE_STATIC(&_5, fastorm_db_columninfo_ce, "detecttype", &_6, nativetype);
			zephir_check_call_status_or_jump(try_end_1);
			ZEPHIR_OBS_NVAR(_7);
			zephir_array_fetch_string(&_7, col, SL("name"), PH_NOISY, "fastorm/db/Result.zep", 249 TSRMLS_CC);
			zephir_update_property_array(this_ptr, SL("_types"), _7, _5 TSRMLS_CC);
		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_NULL();
		}
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_Result, normalize) {

	zephir_fcall_cache_entry *_21 = NULL, *_23 = NULL;
	zend_class_entry *_20;
	zval *_8 = NULL, *_17 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL, *_18 = NULL;
	zend_bool _4, _5, _15, _16;
	HashTable *_2;
	HashPosition _1;
	zval *row = NULL, *key = NULL, *type = NULL, *value = NULL, *tmp = NULL, *left = NULL, *right = NULL, *_0, **_3, *_6 = NULL, _9 = zval_used_for_init, *_10 = NULL, *_11 = NULL, _12 = zval_used_for_init, _13 = zval_used_for_init, _14 = zval_used_for_init, *_19 = NULL, *_22 = NULL, *_24 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row);

	ZEPHIR_SEPARATE_PARAM(row);


	if (Z_TYPE_P(row) != IS_ARRAY) {
		ZEPHIR_INIT_NVAR(row);
		array_init(row);
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_types"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/db/Result.zep", 316);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(key, _2, _1);
		ZEPHIR_GET_HVALUE(type, _3);
		if (!(zephir_array_isset(row, key))) {
			continue;
		}
		ZEPHIR_OBS_NVAR(value);
		zephir_array_fetch(&value, row, key, PH_NOISY, "fastorm/db/Result.zep", 278 TSRMLS_CC);
		_4 = ZEPHIR_IS_FALSE_IDENTICAL(value);
		if (!(_4)) {
			_4 = Z_TYPE_P(value) == IS_NULL;
		}
		_5 = _4;
		if (!(_5)) {
			_5 = ZEPHIR_IS_STRING_IDENTICAL(type, "s");
		}
		if (_5) {
			continue;
		}
		if (ZEPHIR_IS_STRING_IDENTICAL(type, "i")) {
			ZEPHIR_INIT_NVAR(tmp);
			ZVAL_LONG(tmp, (zephir_get_numberval(value) * 1));
			ZEPHIR_CALL_FUNCTION(&_6, "is_float", &_7, tmp);
			zephir_check_call_status();
			if (zephir_is_true(_6)) {
				zephir_array_update_zval(&row, key, &value, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_zval(&row, key, &tmp, PH_COPY | PH_SEPARATE);
			}
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(type, "f")) {
				ZEPHIR_INIT_NVAR(tmp);
				ZVAL_DOUBLE(tmp, zephir_get_doubleval(value));
				ZEPHIR_INIT_NVAR(left);
				zephir_get_strval(_8, tmp);
				ZEPHIR_SINIT_NVAR(_9);
				ZVAL_LONG(&_9, 0);
				zephir_fast_trim(left, _8, &_9, ZEPHIR_TRIM_LEFT TSRMLS_CC);
				ZEPHIR_INIT_NVAR(right);
				ZEPHIR_INIT_NVAR(_10);
				ZEPHIR_INIT_NVAR(_11);
				ZEPHIR_SINIT_NVAR(_12);
				ZVAL_LONG(&_12, 0);
				zephir_fast_trim(_11, value, &_12, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_13);
				ZVAL_STRING(&_13, ".", 0);
				zephir_fast_trim(_10, _11, &_13, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_14);
				ZVAL_LONG(&_14, 0);
				zephir_fast_trim(right, _10, &_14, ZEPHIR_TRIM_LEFT TSRMLS_CC);
				if (ZEPHIR_IS_IDENTICAL(left, right)) {
					zephir_array_update_zval(&row, key, &tmp, PH_COPY | PH_SEPARATE);
				} else {
					zephir_array_update_zval(&row, key, &value, PH_COPY | PH_SEPARATE);
				}
			} else {
				if (ZEPHIR_IS_STRING_IDENTICAL(type, "b")) {
					_15 = zephir_get_boolval(value);
					if (_15) {
						_15 = !ZEPHIR_IS_STRING_IDENTICAL(value, "f");
					}
					_16 = _15;
					if (_16) {
						_16 = !ZEPHIR_IS_STRING_IDENTICAL(value, "F");
					}
					ZEPHIR_INIT_NVAR(_10);
					ZVAL_BOOL(_10, _16);
					zephir_array_update_zval(&row, key, &_10, PH_COPY | PH_SEPARATE);
				} else {
					_15 = ZEPHIR_IS_STRING_IDENTICAL(type, "d");
					if (!(_15)) {
						_15 = ZEPHIR_IS_STRING_IDENTICAL(type, "t");
					}
					if (_15) {
						_16 = zephir_get_intval(value) != 0;
						if (!(_16)) {
							zephir_get_strval(_17, value);
							ZEPHIR_SINIT_NVAR(_9);
							ZVAL_LONG(&_9, 0);
							ZEPHIR_SINIT_NVAR(_12);
							ZVAL_LONG(&_12, 3);
							ZEPHIR_CALL_FUNCTION(&_6, "substr", &_18, _17, &_9, &_12);
							zephir_check_call_status();
							_16 = ZEPHIR_IS_STRING_IDENTICAL(_6, "00:");
						}
						if (_16) {
							ZEPHIR_INIT_LNVAR(_19);
							_20 = zend_fetch_class(SL("DateTime"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
							object_init_ex(_19, _20);
							ZEPHIR_CALL_METHOD(NULL, _19, "__construct", &_21, value);
							zephir_check_call_status();
							zephir_array_update_zval(&row, key, &_19, PH_COPY | PH_SEPARATE);
						}
					} else {
						if (ZEPHIR_IS_STRING_IDENTICAL(type, "bin")) {
							ZEPHIR_CALL_METHOD(&_22, this_ptr, "getresultdriver",  &_23);
							zephir_check_call_status();
							ZEPHIR_CALL_METHOD(&_24, _22, "unescape", NULL, value, type);
							zephir_check_call_status();
							zephir_array_update_zval(&row, key, &_24, PH_COPY | PH_SEPARATE);
						}
					}
				}
			}
		}
	}
	RETURN_CCTOR(row);

}

static PHP_METHOD(Fastorm_Db_Result, setType) {

	zval *col, *type;

	zephir_fetch_params(0, 2, 0, &col, &type);



	zephir_update_property_array(this_ptr, SL("_types"), col, type TSRMLS_CC);
	RETURN_THISW();

}

static PHP_METHOD(Fastorm_Db_Result, getType) {

	zval *col, *_0, *_1, *_2;

	zephir_fetch_params(0, 1, 0, &col);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_types"), PH_NOISY_CC);
	if (zephir_array_isset(_0, col)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_types"), PH_NOISY_CC);
		zephir_array_fetch(&_2, _1, col, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 340 TSRMLS_CC);
		RETURN_CTORW(_2);
	} else {
		RETURN_NULL();
	}

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_ResultIterator) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ResultIterator, fastorm, db_resultiterator, fastorm_db_resultiterator_method_entry, 0);

	/** @var DibiResult */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("result"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("row"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("pointer"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_class_implements(fastorm_db_resultiterator_ce TSRMLS_CC, 1, zend_ce_iterator);
	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_ResultIterator, __construct) {

	zval *result;

	zephir_fetch_params(0, 1, 0, &result);



	if (!(zephir_instance_of_ev(result, fastorm_db_result_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'result' must be an instance of 'Fastorm\\Db\\Result'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("result"), result TSRMLS_CC);

}

static PHP_METHOD(Fastorm_Db_ResultIterator, rewind) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("pointer"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 0);
	ZEPHIR_CALL_METHOD(NULL, _0, "seek", NULL, _1);
	zephir_check_call_status();
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_3, _2, "fetchrow",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("row"), _3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_ResultIterator, key) {


	RETURN_MEMBER_QUICK(this_ptr, "pointer", 905334246UL);

}

static PHP_METHOD(Fastorm_Db_ResultIterator, current) {


	RETURN_MEMBER_QUICK(this_ptr, "row", 2090695229UL);

}

static PHP_METHOD(Fastorm_Db_ResultIterator, next) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, _0, "fetchrow",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("row"), _1 TSRMLS_CC);
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("pointer") TSRMLS_CC));
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_Db_ResultIterator, valid) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property_this(&_0, this_ptr, SL("row"), PH_NOISY_CC);
	RETURN_MM_BOOL(!ZEPHIR_IS_EMPTY(_0));

}

static PHP_METHOD(Fastorm_Db_ResultIterator, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Db_Translator) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Translator, fastorm, db_translator, fastorm_db_translator_method_entry, 0);

	/** @var Connection */
	zend_declare_property_null(fastorm_db_translator_ce, SL("connection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var IDibiDriver */
	zend_declare_property_null(fastorm_db_translator_ce, SL("driver"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("cursor"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var array */
	zend_declare_property_null(fastorm_db_translator_ce, SL("args"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_null(fastorm_db_translator_ce, SL("hasError"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_null(fastorm_db_translator_ce, SL("comment"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("ifLevel"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("ifLevelStart"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("limit"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("offset"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_Db_Translator, __construct) {

	zval *connection;

	zephir_fetch_params(0, 1, 0, &connection);



	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);

}

static PHP_METHOD(Fastorm_Db_Translator, translate) {

	double _30;
	zephir_fcall_cache_entry *_24 = NULL, *_26 = NULL, *_31 = NULL, *_36 = NULL;
	zval *_15 = NULL;
	zend_bool _4, _29;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_11 = NULL, *_14 = NULL, *_19 = NULL, *_22 = NULL, *_27 = NULL;
	zval *ret = NULL, *_34 = NULL;
	int toSkip, lastArr, ZEPHIR_LAST_CALL_STATUS;
	zval *args = NULL, *commandIns = NULL, *arg = NULL, *sql, *_0, *_1, *_2 = NULL, *_5, *_6, *_7, *_8, *_9 = NULL, _10 = zval_used_for_init, _12 = zval_used_for_init, *_13 = NULL, *_16 = NULL, *_17 = NULL, *_18 = NULL, *_20 = NULL, *_21 = NULL, *_23 = NULL, *_25, *_28, *_32 = NULL, *_33 = NULL, *_35, *_37;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_INIT_VAR(sql);
	array_init(sql);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_2, _1, "getdriver",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), _2 TSRMLS_CC);
	}
	if (Z_TYPE_P(args) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Input of translate function must be array", "fastorm/db/Translator.zep", 63);
		return;
	}
	ZEPHIR_CALL_FUNCTION(&_2, "array_values", &_3, args);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(args, _2);
	while (1) {
		_4 = zephir_fast_count_int(args TSRMLS_CC) == 1;
		if (_4) {
			zephir_array_fetch_long(&_5, args, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 68 TSRMLS_CC);
			_4 = Z_TYPE_P(_5) == IS_ARRAY;
		}
		if (!(_4)) {
			break;
		}
		zephir_array_fetch_long(&_6, args, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 69 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&args, "array_values", &_3, _6);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("args"), args TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, -1);
	zephir_update_property_this(this_ptr, SL("limit"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("offset"), _1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("hasError"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_VAR(commandIns);
	ZVAL_NULL(commandIns);
	lastArr = 0;
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("cursor"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("ifLevel"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("ifLevelStart"), _1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	while (1) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		if (!(ZEPHIR_LT_LONG(_1, zephir_fast_count_int(_7 TSRMLS_CC)))) {
			break;
		}
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(arg);
		ZEPHIR_OBS_NVAR(_9);
		zephir_read_property_this(&_9, this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_array_fetch(&arg, _8, _9, PH_NOISY, "fastorm/db/Translator.zep", 87 TSRMLS_CC);
		RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
		if (Z_TYPE_P(arg) == IS_STRING) {
			ZEPHIR_SINIT_NVAR(_10);
			ZVAL_STRING(&_10, "`['\":%?", 0);
			ZEPHIR_CALL_FUNCTION(&_2, "strcspn", &_11, arg, &_10);
			zephir_check_call_status();
			toSkip = zephir_get_intval(_2);
			if (zephir_fast_strlen_ev(arg) == toSkip) {
				zephir_array_append(&sql, arg, PH_SEPARATE, "fastorm/db/Translator.zep", 96);
			} else {
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_LONG(&_10, 0);
				ZEPHIR_SINIT_NVAR(_12);
				ZVAL_LONG(&_12, toSkip);
				ZEPHIR_CALL_FUNCTION(&_13, "substr", &_14, arg, &_10, &_12);
				zephir_check_call_status();
				ZEPHIR_INIT_NVAR(_15);
				array_init_size(_15, 3);
				zephir_array_fast_append(_15, this_ptr);
				ZEPHIR_INIT_NVAR(_16);
				ZVAL_STRING(_16, "cb", 1);
				zephir_array_fast_append(_15, _16);
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_LONG(&_10, toSkip);
				ZEPHIR_CALL_FUNCTION(&_17, "substr", &_14, arg, &_10);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_STRING(&_10, "/(?=[`['\":%?])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?)|%([a-zA-Z~][a-zA-Z0-9~]{0,5})|(\\?))/s", 0);
				ZEPHIR_CALL_FUNCTION(&_18, "preg_replace_callback", &_19, &_10, _15, _17);
				zephir_check_call_status();
				ZEPHIR_INIT_LNVAR(_20);
				ZEPHIR_CONCAT_VV(_20, _13, _18);
				zephir_array_append(&sql, _20, PH_SEPARATE, "fastorm/db/Translator.zep", 117);
				ZEPHIR_CALL_FUNCTION(&_21, "preg_last_error", &_22);
				zephir_check_call_status();
				if (zephir_is_true(_21)) {
					ZEPHIR_INIT_LNVAR(_23);
					object_init_ex(_23, fastorm_exception_ce);
					ZEPHIR_CALL_METHOD(NULL, _23, "__construct", &_24);
					zephir_check_call_status();
					zephir_throw_exception_debug(_23, "fastorm/db/Translator.zep", 119 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			}
			continue;
		}
		_25 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
		if (zephir_is_true(_25)) {
			ZEPHIR_INIT_NVAR(_16);
			ZVAL_STRING(_16, "...", 1);
			zephir_array_append(&sql, _16, PH_SEPARATE, "fastorm/db/Translator.zep", 126);
			continue;
		}
		if (zephir_is_instance_of(arg, SL("Traversable") TSRMLS_CC)) {
			ZEPHIR_CALL_FUNCTION(&_13, "iterator_to_array", &_26, arg);
			zephir_check_call_status();
			ZEPHIR_CPY_WRT(arg, _13);
		}
		if (Z_TYPE_P(arg) == IS_ARRAY) {
			Z_SET_ISREF_P(arg);
			ZEPHIR_CALL_FUNCTION(&_13, "key", &_27, arg);
			Z_UNSET_ISREF_P(arg);
			zephir_check_call_status();
			if (Z_TYPE_P(_13) == IS_STRING) {
				if (Z_TYPE_P(commandIns) == IS_NULL) {
					ZEPHIR_INIT_NVAR(commandIns);
					ZEPHIR_INIT_NVAR(_16);
					_28 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
					zephir_array_fetch_long(&_6, _28, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 138 TSRMLS_CC);
					zephir_fast_trim(_16, _6, NULL , ZEPHIR_TRIM_LEFT TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_10);
					ZVAL_LONG(&_10, 0);
					ZEPHIR_SINIT_NVAR(_12);
					ZVAL_LONG(&_12, 6);
					ZEPHIR_CALL_FUNCTION(&_17, "substr", &_14, _16, &_10, &_12);
					zephir_check_call_status();
					zephir_fast_strtoupper(commandIns, _17);
					_29 = ZEPHIR_IS_STRING_IDENTICAL(commandIns, "INSERT");
					if (!(_29)) {
						_29 = ZEPHIR_IS_STRING_IDENTICAL(commandIns, "REPLAC");
					}
					_30 = _29;
					ZEPHIR_INIT_NVAR(commandIns);
					ZVAL_BOOL(commandIns, _30);
					ZEPHIR_INIT_LNVAR(_20);
					if (zephir_is_true(commandIns)) {
						ZEPHIR_INIT_NVAR(_20);
						ZVAL_STRING(_20, "v", 1);
					} else {
						ZEPHIR_INIT_NVAR(_20);
						ZVAL_STRING(_20, "a", 1);
					}
					ZEPHIR_CALL_METHOD(&_18, this_ptr, "formatvalue", &_31, arg, _20);
					zephir_check_call_status();
					zephir_array_append(&sql, _18, PH_SEPARATE, "fastorm/db/Translator.zep", 140);
				} else {
					_28 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
					if (lastArr == (zephir_get_numberval(_28) - 1)) {
						ZEPHIR_INIT_NVAR(_32);
						ZVAL_STRING(_32, ",", 1);
						zephir_array_append(&sql, _32, PH_SEPARATE, "fastorm/db/Translator.zep", 143);
					}
					ZEPHIR_INIT_LNVAR(_23);
					if (zephir_is_true(commandIns)) {
						ZEPHIR_INIT_NVAR(_23);
						ZVAL_STRING(_23, "l", 1);
					} else {
						ZEPHIR_INIT_NVAR(_23);
						ZVAL_STRING(_23, "a", 1);
					}
					ZEPHIR_CALL_METHOD(&_21, this_ptr, "formatvalue", &_31, arg, _23);
					zephir_check_call_status();
					zephir_array_append(&sql, _21, PH_SEPARATE, "fastorm/db/Translator.zep", 145);
				}
				ZEPHIR_OBS_NVAR(_33);
				zephir_read_property_this(&_33, this_ptr, SL("cursor"), PH_NOISY_CC);
				lastArr = zephir_get_intval(_33);
				continue;
			}
		}
		ZEPHIR_CALL_METHOD(&_13, this_ptr, "formatvalue", &_31, arg, ZEPHIR_GLOBAL(global_false));
		zephir_check_call_status();
		zephir_array_append(&sql, _13, PH_SEPARATE, "fastorm/db/Translator.zep", 153);
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_1)) {
		ZEPHIR_INIT_NVAR(_16);
		ZVAL_STRING(_16, "*/", 1);
		zephir_array_append(&sql, _16, PH_SEPARATE, "fastorm/db/Translator.zep", 158);
	}
	ZEPHIR_INIT_NVAR(_16);
	zephir_fast_join_str(_16, SL(" "), sql TSRMLS_CC);
	zephir_get_strval(_34, _16);
	ZEPHIR_CPY_WRT(ret, _34);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("hasError"), PH_NOISY_CC);
	if (zephir_is_true(_1)) {
		ZEPHIR_INIT_LNVAR(_20);
		object_init_ex(_20, fastorm_db_dbexception_ce);
		ZEPHIR_INIT_NVAR(_32);
		ZVAL_STRING(_32, "SQL translate error", 0);
		ZEPHIR_INIT_VAR(_35);
		ZVAL_LONG(_35, 0);
		ZEPHIR_CALL_METHOD(NULL, _20, "__construct", &_36, _32, _35, ZEPHIR_GLOBAL(global_null), ret, args);
		zephir_check_temp_parameter(_32);
		zephir_check_call_status();
		zephir_throw_exception_debug(_20, "fastorm/db/Translator.zep", 164 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("limit"), PH_NOISY_CC);
	_29 = ZEPHIR_GT_LONG(_1, -1);
	if (!(_29)) {
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("offset"), PH_NOISY_CC);
		_29 = ZEPHIR_GT_LONG(_8, 0);
	}
	if (_29) {
		_25 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_28 = zephir_fetch_nproperty_this(this_ptr, SL("limit"), PH_NOISY_CC);
		_37 = zephir_fetch_nproperty_this(this_ptr, SL("offset"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _25, "applylimit", NULL, ret, _28, _37);
		zephir_check_call_status();
	}
	RETURN_CTOR(ret);

}

static PHP_METHOD(Fastorm_Db_Translator, formatValue) {

	zval *_77;
	zend_class_entry *_73;
	zephir_nts_static zephir_fcall_cache_entry *_31 = NULL, *_57 = NULL, *_62 = NULL, *_63 = NULL, *_65 = NULL, *_71 = NULL, *_75 = NULL, *_76 = NULL, *_79 = NULL, *_81 = NULL, *_84 = NULL, *_86 = NULL;
	zend_bool _17, declared, _55, _58, _64, _67;
	zephir_fcall_cache_entry *_8 = NULL, *_10 = NULL;
	HashTable *_5, *_23, *_27, *_34, *_37, *_41, *_44, *_48, *_53, *_60;
	HashPosition _4, _22, _26, _33, _36, _40, _43, _47, _52, _59;
	int ZEPHIR_LAST_CALL_STATUS, _72, toSkip;
	zval *modifier = NULL, *op = NULL, *_66 = NULL, *_74 = NULL, *_82 = NULL, *_85 = NULL;
	zval *value = NULL, *modifier_param = NULL, *k = NULL, *v = NULL, *pair = NULL, *vx, *kx, *proto = NULL, *_0, *_1, *_2, *_3 = NULL, **_6, *_7, *_9 = NULL, *_11 = NULL, *_12, *_13 = NULL, *_14 = NULL, *_15, *_16, *_18, *_19, _20 = zval_used_for_init, *_21 = NULL, **_24, *_25 = NULL, **_28, *_29 = NULL, *_30 = NULL, *_32 = NULL, **_35, **_38, *_39 = NULL, **_42, *k2 = NULL, *v2 = NULL, **_45, *_46 = NULL, **_49, *_50 = NULL, *_51, **_54, _56 = zval_used_for_init, *translator, **_61, _68 = zval_used_for_init, _69 = zval_used_for_init, _70, *_78 = NULL, *_80 = NULL, *_83;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &value, &modifier_param);

	ZEPHIR_SEPARATE_PARAM(value);
	if (!modifier_param) {
		ZEPHIR_INIT_VAR(modifier);
		ZVAL_EMPTY_STRING(modifier);
	} else {
		zephir_get_strval(modifier, modifier_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		RETURN_MM_STRING("...", 1);
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_1) == IS_NULL) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_3, _2, "getdriver",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), _3 TSRMLS_CC);
	}
	if (zephir_is_instance_of(value, SL("Traversable") TSRMLS_CC)) {
		ZEPHIR_CALL_FUNCTION(&_3, "iterator_to_array", NULL, value);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(value, _3);
	}
	if (Z_TYPE_P(value) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(vx);
		array_init(vx);
		ZEPHIR_INIT_VAR(kx);
		array_init(kx);
		do {
			if (ZEPHIR_IS_STRING(modifier, "and") || ZEPHIR_IS_STRING(modifier, "or")) {
				if (ZEPHIR_IS_EMPTY(value)) {
					RETURN_MM_STRING("1=1", 1);
				}
				zephir_is_iterable(value, &_5, &_4, 0, 0, "fastorm/db/Translator.zep", 246);
				for (
				  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
				  ; zephir_hash_move_forward_ex(_5, &_4)
				) {
					ZEPHIR_GET_HMKEY(k, _5, _4);
					ZEPHIR_GET_HVALUE(v, _6);
					if (Z_TYPE_P(k) == IS_STRING) {
						ZEPHIR_INIT_NVAR(pair);
						zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
						zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 216 TSRMLS_CC);
						ZEPHIR_CALL_METHOD(&_3, this_ptr, "delimite", &_8, _7);
						zephir_check_call_status();
						ZEPHIR_INIT_NVAR(k);
						ZEPHIR_CONCAT_VS(k, _3, " ");
						if (!(zephir_array_isset_long(pair, 1))) {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v);
							zephir_check_call_status();
							ZEPHIR_CPY_WRT(v, _9);
							if (ZEPHIR_IS_STRING_IDENTICAL(v, "null")) {
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VSV(_11, k, "IS ", v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 220);
							} else {
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VSV(_11, k, "= ", v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 222);
							}
						} else {
							zephir_array_fetch_long(&_12, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 225 TSRMLS_CC);
							if (ZEPHIR_IS_STRING_IDENTICAL(_12, "ex")) {
								ZEPHIR_INIT_NVAR(_13);
								ZVAL_STRING(_13, "ex", 0);
								ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, _13);
								zephir_check_temp_parameter(_13);
								zephir_check_call_status();
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VV(_11, k, _9);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 226);
							} else {
								zephir_array_fetch_long(&_15, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 229 TSRMLS_CC);
								ZEPHIR_CALL_METHOD(&_14, this_ptr, "formatvalue", &_10, v, _15);
								zephir_check_call_status();
								ZEPHIR_CPY_WRT(v, _14);
								zephir_array_fetch_long(&_16, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 230 TSRMLS_CC);
								_17 = ZEPHIR_IS_STRING_IDENTICAL(_16, "l");
								if (!(_17)) {
									zephir_array_fetch_long(&_18, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 230 TSRMLS_CC);
									_17 = ZEPHIR_IS_STRING_IDENTICAL(_18, "in");
								}
								if (_17) {
									ZEPHIR_INIT_NVAR(op);
									ZVAL_STRING(op, "IN ", 1);
								} else {
									zephir_array_fetch_long(&_19, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 232 TSRMLS_CC);
									ZEPHIR_SINIT_NVAR(_20);
									ZVAL_STRING(&_20, "like", 0);
									ZEPHIR_INIT_NVAR(_13);
									zephir_fast_strpos(_13, _19, &_20, 0 );
									if (!ZEPHIR_IS_FALSE_IDENTICAL(_13)) {
										ZEPHIR_INIT_NVAR(op);
										ZVAL_STRING(op, "LIKE ", 1);
									} else {
										if (ZEPHIR_IS_STRING_IDENTICAL(v, "null")) {
											ZEPHIR_INIT_NVAR(op);
											ZVAL_STRING(op, "IS ", 1);
										} else {
											ZEPHIR_INIT_NVAR(op);
											ZVAL_STRING(op, "= ", 1);
										}
									}
								}
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VVV(_11, k, op, v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 239);
							}
						}
					} else {
						ZEPHIR_INIT_NVAR(_13);
						ZVAL_STRING(_13, "ex", 0);
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, _13);
						zephir_check_temp_parameter(_13);
						zephir_check_call_status();
						zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 243);
					}
				}
				ZEPHIR_INIT_NVAR(_13);
				ZEPHIR_INIT_VAR(_21);
				zephir_fast_strtoupper(_21, modifier);
				ZEPHIR_INIT_LNVAR(_11);
				ZEPHIR_CONCAT_SVS(_11, ") ", _21, " (");
				zephir_fast_join(_13, _11, vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVS(return_value, "(", _13, ")");
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "n")) {
				zephir_is_iterable(value, &_23, &_22, 0, 0, "fastorm/db/Translator.zep", 263);
				for (
				  ; zephir_hash_get_current_data_ex(_23, (void**) &_24, &_22) == SUCCESS
				  ; zephir_hash_move_forward_ex(_23, &_22)
				) {
					ZEPHIR_GET_HMKEY(k, _23, _22);
					ZEPHIR_GET_HVALUE(v, _24);
					if (Z_TYPE_P(k) == IS_STRING) {
						if (ZEPHIR_IS_EMPTY(v)) {
							ZEPHIR_INIT_NVAR(op);
							ZVAL_EMPTY_STRING(op);
						} else {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, v);
							zephir_check_call_status();
							ZEPHIR_INIT_LNVAR(_25);
							ZEPHIR_CONCAT_SV(_25, " AS ", _9);
							zephir_get_strval(op, _25);
						}
						ZEPHIR_INIT_LNVAR(_25);
						ZEPHIR_CONCAT_VV(_25, k, op);
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, _25);
						zephir_check_call_status();
						zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 257);
					} else {
						ZEPHIR_INIT_NVAR(pair);
						zephir_fast_explode_str(pair, SL("%"), v, 2  TSRMLS_CC);
						zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 260 TSRMLS_CC);
						ZEPHIR_CALL_METHOD(&_14, this_ptr, "delimite", &_8, _7);
						zephir_check_call_status();
						zephir_array_append(&vx, _14, PH_SEPARATE, "fastorm/db/Translator.zep", 260);
					}
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "a")) {
				zephir_is_iterable(value, &_27, &_26, 0, 0, "fastorm/db/Translator.zep", 271);
				for (
				  ; zephir_hash_get_current_data_ex(_27, (void**) &_28, &_26) == SUCCESS
				  ; zephir_hash_move_forward_ex(_27, &_26)
				) {
					ZEPHIR_GET_HMKEY(k, _27, _26);
					ZEPHIR_GET_HVALUE(v, _28);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 269 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "delimite", &_8, _7);
					zephir_check_call_status();
					ZEPHIR_CALL_METHOD(&_30, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_32);
					ZEPHIR_CONCAT_VSV(_32, _29, "=", _30);
					zephir_array_append(&vx, _32, PH_SEPARATE, "fastorm/db/Translator.zep", 269);
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "in") || ZEPHIR_IS_STRING(modifier, "l")) {
				zephir_is_iterable(value, &_34, &_33, 0, 0, "fastorm/db/Translator.zep", 280);
				for (
				  ; zephir_hash_get_current_data_ex(_34, (void**) &_35, &_33) == SUCCESS
				  ; zephir_hash_move_forward_ex(_34, &_33)
				) {
					ZEPHIR_GET_HMKEY(k, _34, _33);
					ZEPHIR_GET_HVALUE(v, _35);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					zephir_array_append(&vx, _29, PH_SEPARATE, "fastorm/db/Translator.zep", 278);
				}
				_17 = zephir_is_true(vx);
				if (!(_17)) {
					_17 = ZEPHIR_IS_STRING_IDENTICAL(modifier, "l");
				}
				if (_17) {
					ZEPHIR_INIT_NVAR(_21);
					zephir_fast_join_str(_21, SL(", "), vx TSRMLS_CC);
					ZEPHIR_CONCAT_SVS(return_value, "(", _21, ")");
					RETURN_MM();
				} else {
					RETURN_MM_STRING("(null)", 1);
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "v")) {
				zephir_is_iterable(value, &_37, &_36, 0, 0, "fastorm/db/Translator.zep", 292);
				for (
				  ; zephir_hash_get_current_data_ex(_37, (void**) &_38, &_36) == SUCCESS
				  ; zephir_hash_move_forward_ex(_37, &_36)
				) {
					ZEPHIR_GET_HMKEY(k, _37, _36);
					ZEPHIR_GET_HVALUE(v, _38);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_12, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 289 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "delimite", &_8, _12);
					zephir_check_call_status();
					zephir_array_append(&kx, _29, PH_SEPARATE, "fastorm/db/Translator.zep", 289);
					ZEPHIR_CALL_METHOD(&_30, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					zephir_array_append(&vx, _30, PH_SEPARATE, "fastorm/db/Translator.zep", 290);
				}
				ZEPHIR_INIT_NVAR(_21);
				zephir_fast_join_str(_21, SL(", "), kx TSRMLS_CC);
				ZEPHIR_INIT_VAR(_39);
				zephir_fast_join_str(_39, SL(", "), vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVSVS(return_value, "(", _21, ") VALUES (", _39, ")");
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "m")) {
				declared = 0;
				ZEPHIR_INIT_VAR(proto);
				ZVAL_NULL(proto);
				zephir_is_iterable(value, &_41, &_40, 0, 0, "fastorm/db/Translator.zep", 326);
				for (
				  ; zephir_hash_get_current_data_ex(_41, (void**) &_42, &_40) == SUCCESS
				  ; zephir_hash_move_forward_ex(_41, &_40)
				) {
					ZEPHIR_GET_HMKEY(k, _41, _40);
					ZEPHIR_GET_HVALUE(v, _42);
					if (Z_TYPE_P(v) == IS_ARRAY) {
						if (declared) {
							ZEPHIR_INIT_NVAR(_21);
							zephir_array_keys(_21, v TSRMLS_CC);
							if (!ZEPHIR_IS_IDENTICAL(proto, _21)) {
								zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
								ZEPHIR_CONCAT_SVS(return_value, "**Multi-insert array '", k, "' is different.**");
								RETURN_MM();
							}
						} else {
							ZEPHIR_INIT_NVAR(proto);
							zephir_array_keys(proto, v TSRMLS_CC);
							declared = 1;
						}
					} else {
						zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						ZEPHIR_INIT_NVAR(_13);
						zephir_gettype(_13, v TSRMLS_CC);
						ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _13, "**");
						RETURN_MM();
					}
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 315 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, _7);
					zephir_check_call_status();
					zephir_array_append(&kx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 315);
					zephir_is_iterable(v, &_44, &_43, 0, 0, "fastorm/db/Translator.zep", 325);
					for (
					  ; zephir_hash_get_current_data_ex(_44, (void**) &_45, &_43) == SUCCESS
					  ; zephir_hash_move_forward_ex(_44, &_43)
					) {
						ZEPHIR_GET_HMKEY(k2, _44, _43);
						ZEPHIR_GET_HVALUE(v2, _45);
						_17 = !zephir_array_isset(vx, k2);
						if (!(_17)) {
							ZEPHIR_OBS_NVAR(_46);
							zephir_array_fetch(&_46, vx, k2, PH_NOISY, "fastorm/db/Translator.zep", 320 TSRMLS_CC);
							_17 = Z_TYPE_P(_46) != IS_ARRAY;
						}
						if (_17) {
							ZEPHIR_INIT_NVAR(_39);
							array_init(_39);
							zephir_array_update_zval(&vx, k2, &_39, PH_COPY | PH_SEPARATE);
						}
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "fomattedpairvalue", &_31, pair, v);
						zephir_check_call_status();
						zephir_array_update_multi(&vx, &_9 TSRMLS_CC, SL("za"), 2, k2);
					}
				}
				zephir_is_iterable(vx, &_48, &_47, 0, 0, "fastorm/db/Translator.zep", 329);
				for (
				  ; zephir_hash_get_current_data_ex(_48, (void**) &_49, &_47) == SUCCESS
				  ; zephir_hash_move_forward_ex(_48, &_47)
				) {
					ZEPHIR_GET_HMKEY(k, _48, _47);
					ZEPHIR_GET_HVALUE(v, _49);
					ZEPHIR_INIT_NVAR(_50);
					zephir_fast_join_str(_50, SL(", "), v TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_11);
					ZEPHIR_CONCAT_SVS(_11, "(", _50, ")");
					zephir_array_update_zval(&vx, k, &_11, PH_COPY | PH_SEPARATE);
				}
				ZEPHIR_INIT_NVAR(_50);
				zephir_fast_join_str(_50, SL(", "), kx TSRMLS_CC);
				ZEPHIR_INIT_VAR(_51);
				zephir_fast_join_str(_51, SL(", "), vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVSV(return_value, "(", _50, ") VALUES ", _51);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "by")) {
				zephir_is_iterable(value, &_53, &_52, 0, 0, "fastorm/db/Translator.zep", 346);
				for (
				  ; zephir_hash_get_current_data_ex(_53, (void**) &_54, &_52) == SUCCESS
				  ; zephir_hash_move_forward_ex(_53, &_52)
				) {
					ZEPHIR_GET_HMKEY(k, _53, _52);
					ZEPHIR_GET_HVALUE(v, _54);
					if (Z_TYPE_P(v) == IS_ARRAY) {
						ZEPHIR_INIT_NVAR(_21);
						ZVAL_STRING(_21, "ex", 0);
						ZEPHIR_CALL_METHOD(&_14, this_ptr, "formatvalue", &_10, v, _21);
						zephir_check_temp_parameter(_21);
						zephir_check_call_status();
						zephir_array_append(&vx, _14, PH_SEPARATE, "fastorm/db/Translator.zep", 334);
					} else {
						if (Z_TYPE_P(k) == IS_STRING) {
							_55 = Z_TYPE_P(v) == IS_STRING;
							if (_55) {
								ZEPHIR_SINIT_NVAR(_20);
								ZVAL_STRING(&_20, "d", 0);
								ZEPHIR_SINIT_NVAR(_56);
								ZVAL_LONG(&_56, 1);
								ZEPHIR_CALL_FUNCTION(&_29, "strncasecmp", &_57, v, &_20, &_56);
								zephir_check_call_status();
								_55 = zephir_is_true(_29);
							}
							_58 = _55;
							if (!(_58)) {
								_58 = ZEPHIR_GT_LONG(v, 0);
							}
							ZEPHIR_INIT_NVAR(v);
							if (_58) {
								ZVAL_STRING(v, "ASC", 1);
							} else {
								ZVAL_STRING(v, "DESC", 1);
							}
							ZEPHIR_CALL_METHOD(&_14, this_ptr, "delimite", &_8, k);
							zephir_check_call_status();
							ZEPHIR_INIT_LNVAR(_11);
							ZEPHIR_CONCAT_VSV(_11, _14, " ", v);
							zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 341);
						} else {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, v);
							zephir_check_call_status();
							zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 343);
						}
					}
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "ex") || ZEPHIR_IS_STRING(modifier, "sql")) {
				ZEPHIR_INIT_VAR(translator);
				object_init_ex(translator, fastorm_db_translator_ce);
				_2 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
				ZEPHIR_CALL_METHOD(NULL, translator, "__construct", NULL, _2);
				zephir_check_call_status();
				ZEPHIR_RETURN_CALL_METHOD(translator, "translate", NULL, value);
				zephir_check_call_status();
				RETURN_MM();
			}
			zephir_is_iterable(value, &_60, &_59, 0, 0, "fastorm/db/Translator.zep", 358);
			for (
			  ; zephir_hash_get_current_data_ex(_60, (void**) &_61, &_59) == SUCCESS
			  ; zephir_hash_move_forward_ex(_60, &_59)
			) {
				ZEPHIR_GET_HVALUE(v, _61);
				ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, modifier);
				zephir_check_call_status();
				zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 356);
			}
			zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
			RETURN_MM();
		} while(0);

	}
	if (!ZEPHIR_IS_STRING_IDENTICAL(modifier, "")) {
		_17 = Z_TYPE_P(value) != IS_NULL;
		if (_17) {
			ZEPHIR_CALL_FUNCTION(&_3, "is_scalar", &_62, value);
			zephir_check_call_status();
			_17 = !zephir_is_true(_3);
		}
		_55 = _17;
		if (_55) {
			_55 = !(zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC));
		}
		_58 = _55;
		if (_58) {
			_58 = !(zephir_is_instance_of(value, SL("DateTimeInterface") TSRMLS_CC));
		}
		if (_58) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_13);
			zephir_gettype(_13, value TSRMLS_CC);
			ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _13, "**");
			RETURN_MM();
		}
		do {
			if (ZEPHIR_IS_STRING(modifier, "s") || ZEPHIR_IS_STRING(modifier, "bin") || ZEPHIR_IS_STRING(modifier, "b")) {
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nullescape", &_63, value, modifier);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "sN") || ZEPHIR_IS_STRING(modifier, "sn")) {
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_STRING(_21, "s", 0);
				ZEPHIR_INIT_NVAR(_39);
				ZVAL_STRING(_39, "", 0);
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nullescape", &_63, value, _21, _39);
				zephir_check_temp_parameter(_21);
				zephir_check_temp_parameter(_39);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "iN") || ZEPHIR_IS_STRING(modifier, "in")) {
				if (ZEPHIR_IS_STRING(value, "")) {
					ZEPHIR_INIT_NVAR(value);
					ZVAL_NULL(value);
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "i") || ZEPHIR_IS_STRING(modifier, "u")) {
				_64 = Z_TYPE_P(value) == IS_STRING;
				if (_64) {
					ZEPHIR_SINIT_NVAR(_20);
					ZVAL_STRING(&_20, "#[+-]?\\d++(e\\d+)?\\z#A", 0);
					ZEPHIR_CALL_FUNCTION(&_29, "preg_match", &_65, &_20, value);
					zephir_check_call_status();
					_64 = zephir_is_true(_29);
				}
				if (_64) {
					RETURN_CCTOR(value);
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						ZEPHIR_INIT_NVAR(proto);
						ZVAL_LONG(proto, ((zephir_get_numberval(value) + 0)));
						zephir_get_strval(_66, proto);
						ZEPHIR_CONCAT_VS(return_value, _66, "");
						RETURN_MM();
					}
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "f")) {
				_64 = Z_TYPE_P(value) == IS_STRING;
				if (_64) {
					_64 = zephir_is_numeric(value);
				}
				_67 = _64;
				if (_67) {
					ZEPHIR_SINIT_NVAR(_20);
					ZVAL_STRING(&_20, "x", 0);
					ZEPHIR_INIT_NVAR(_21);
					zephir_fast_strpos(_21, value, &_20, 0 );
					_67 = ZEPHIR_IS_FALSE_IDENTICAL(_21);
				}
				if (_67) {
					RETURN_CCTOR(value);
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						ZEPHIR_INIT_NVAR(_39);
						ZEPHIR_SINIT_NVAR(_56);
						ZVAL_LONG(&_56, (zephir_get_numberval(value) + 0));
						ZEPHIR_SINIT_VAR(_68);
						ZVAL_LONG(&_68, 10);
						ZEPHIR_SINIT_VAR(_69);
						ZVAL_STRING(&_69, ".", 0);
						ZEPHIR_SINIT_VAR(_70);
						ZVAL_STRING(&_70, "", 0);
						ZEPHIR_CALL_FUNCTION(&_29, "number_format", &_71, &_56, &_68, &_69, &_70);
						zephir_check_call_status();
						ZEPHIR_SINIT_NVAR(_56);
						ZVAL_LONG(&_56, 0);
						zephir_fast_trim(_39, _29, &_56, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
						ZEPHIR_SINIT_NVAR(_68);
						ZVAL_STRING(&_68, ".", 0);
						zephir_fast_trim(return_value, _39, &_68, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
						RETURN_MM();
					}
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "d") || ZEPHIR_IS_STRING(modifier, "t")) {
				if (Z_TYPE_P(value) == IS_NULL) {
					RETURN_MM_STRING("null", 1);
				} else {
					if (zephir_is_numeric(value)) {
						_72 = zephir_get_intval(value);
						ZEPHIR_INIT_NVAR(value);
						ZVAL_LONG(value, _72);
					} else {
						if (Z_TYPE_P(value) == IS_STRING) {
							ZEPHIR_INIT_NVAR(value);
							_73 = zend_fetch_class(SL("DateTime"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
							object_init_ex(value, _73);
							ZEPHIR_CALL_METHOD(NULL, value, "__construct", NULL, value);
							zephir_check_call_status();
						}
					}
					_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, modifier);
					zephir_check_call_status();
					RETURN_MM();
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "by") || ZEPHIR_IS_STRING(modifier, "n")) {
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", &_8, value);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "ex") || ZEPHIR_IS_STRING(modifier, "sql")) {
				zephir_get_strval(_74, value);
				ZEPHIR_CPY_WRT(value, _74);
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_STRING(&_56, "`['\":", 0);
				ZEPHIR_CALL_FUNCTION(&_9, "strcspn", &_75, value, &_56);
				zephir_check_call_status();
				toSkip = zephir_get_intval(_9);
				if (zephir_fast_strlen_ev(value) != toSkip) {
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_LONG(&_56, 0);
					ZEPHIR_SINIT_NVAR(_68);
					ZVAL_LONG(&_68, toSkip);
					ZEPHIR_CALL_FUNCTION(&_29, "substr", &_76, value, &_56, &_68);
					zephir_check_call_status();
					ZEPHIR_INIT_VAR(_77);
					array_init_size(_77, 3);
					zephir_array_fast_append(_77, this_ptr);
					ZEPHIR_INIT_NVAR(_21);
					ZVAL_STRING(_21, "cb", 1);
					zephir_array_fast_append(_77, _21);
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_LONG(&_56, toSkip);
					ZEPHIR_CALL_FUNCTION(&_30, "substr", &_76, value, &_56);
					zephir_check_call_status();
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_STRING(&_56, "/(?=[`['\":])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?))/s", 0);
					ZEPHIR_CALL_FUNCTION(&_78, "preg_replace_callback", &_79, &_56, _77, _30);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_25);
					ZEPHIR_CONCAT_VV(_25, _29, _78);
					ZEPHIR_CPY_WRT(value, _25);
					ZEPHIR_CALL_FUNCTION(&_80, "preg_last_error", &_81);
					zephir_check_call_status();
					if (zephir_is_true(_80)) {
						ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "PCRE exception", "fastorm/db/Translator.zep", 446);
						return;
					}
				}
				RETURN_CCTOR(value);
			}
			if (ZEPHIR_IS_STRING(modifier, "SQL")) {
				zephir_get_strval(_82, value);
				RETURN_CTOR(_82);
			}
			if (ZEPHIR_IS_STRING(modifier, "like~")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, 1);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "~like")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, -1);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "~like~")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, 0);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "and") || ZEPHIR_IS_STRING(modifier, "or") || ZEPHIR_IS_STRING(modifier, "a") || ZEPHIR_IS_STRING(modifier, "l") || ZEPHIR_IS_STRING(modifier, "v")) {
				zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
				ZEPHIR_INIT_NVAR(_21);
				zephir_gettype(_21, value TSRMLS_CC);
				ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _21, "**");
				RETURN_MM();
			}
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Unknown or invalid modifier %modifier**", 1);
		} while(0);

	}
	if (Z_TYPE_P(value) == IS_STRING) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_INIT_NVAR(_21);
		ZVAL_STRING(_21, "s", 0);
		ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, _21);
		zephir_check_temp_parameter(_21);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		ZEPHIR_CALL_FUNCTION(&_3, "is_int", &_84, value);
		zephir_check_call_status();
		if (zephir_is_true(_3)) {
			zephir_get_strval(_85, value);
			RETURN_CTOR(_85);
		} else {
			ZEPHIR_CALL_FUNCTION(&_29, "is_float", &_86, value);
			zephir_check_call_status();
			if (zephir_is_true(_29)) {
				ZEPHIR_INIT_NVAR(_21);
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_LONG(&_56, 10);
				ZEPHIR_SINIT_NVAR(_68);
				ZVAL_STRING(&_68, ".", 0);
				ZEPHIR_SINIT_NVAR(_69);
				ZVAL_STRING(&_69, "", 0);
				ZEPHIR_CALL_FUNCTION(&_30, "number_format", &_71, value, &_56, &_68, &_69);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_LONG(&_56, 0);
				zephir_fast_trim(_21, _30, &_56, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_68);
				ZVAL_STRING(&_68, ".", 0);
				zephir_fast_trim(return_value, _21, &_68, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				RETURN_MM();
			} else {
				if (Z_TYPE_P(value) == IS_BOOL) {
					_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_INIT_NVAR(_39);
					ZVAL_STRING(_39, "b", 0);
					ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, _39);
					zephir_check_temp_parameter(_39);
					zephir_check_call_status();
					RETURN_MM();
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						_55 = (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC));
						if (!(_55)) {
							_55 = (zephir_is_instance_of(value, SL("DateTimeInterface") TSRMLS_CC));
						}
						if (_55) {
							_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
							ZEPHIR_INIT_NVAR(_39);
							ZVAL_STRING(_39, "t", 0);
							ZEPHIR_RETURN_CALL_METHOD(_83, "escape", NULL, value, _39);
							zephir_check_temp_parameter(_39);
							zephir_check_call_status();
							RETURN_MM();
						}
					}
				}
			}
		}
	}
	zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_13);
	zephir_gettype(_13, value TSRMLS_CC);
	ZEPHIR_CONCAT_SVS(return_value, "**Unexpected ", _13, "**");
	RETURN_MM();

}

static PHP_METHOD(Fastorm_Db_Translator, fomattedPairValue) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pairArray, *value, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &pairArray, &value);



	if (zephir_array_isset_long(pairArray, 1)) {
		zephir_array_fetch_long(&_0, pairArray, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 507 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value, _0);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		if (Z_TYPE_P(value) == IS_ARRAY) {
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "ex", 0);
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value);
			zephir_check_call_status();
			RETURN_MM();
		}
	}

}

static PHP_METHOD(Fastorm_Db_Translator, nullEscape) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *value, *modifier, *compareWith = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &value, &modifier, &compareWith);

	if (!compareWith) {
		compareWith = ZEPHIR_GLOBAL(global_null);
	}


	if (ZEPHIR_IS_IDENTICAL(value, compareWith)) {
		RETURN_MM_STRING("null", 1);
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_METHOD(_0, "escape", NULL, value, modifier);
		zephir_check_call_status();
		RETURN_MM();
	}

}

static PHP_METHOD(Fastorm_Db_Translator, cb) {

	zephir_nts_static zephir_fcall_cache_entry *_16 = NULL, *_31 = NULL;
	zend_bool _5, _6, _7;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *matches, *_0, *_1, *_2, *_3, *_4, *mod, *_8, *_9, *_10, *_11, *_12, *_13, *_14 = NULL, *_15 = NULL, *_17 = NULL, *_18 = NULL, *_19, *_20, *_21, *_22, *_23, *_24, *_25, _26 = zval_used_for_init, _27 = zval_used_for_init, *_28 = NULL, *_29, *_30, *m = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &matches);



	if (zephir_array_isset_long(matches, 11)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		if (ZEPHIR_GE_LONG(_0, zephir_fast_count_int(_1 TSRMLS_CC))) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Extra placeholder**", 1);
		}
		RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_array_fetch_long(&_3, _2, (zephir_get_numberval(_4) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 554 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, _3, ZEPHIR_GLOBAL(global_false));
		zephir_check_call_status();
		RETURN_MM();
	}
	if (zephir_array_isset_long(matches, 10)) {
		ZEPHIR_OBS_VAR(mod);
		zephir_array_fetch_long(&mod, matches, 10, PH_NOISY, "fastorm/db/Translator.zep", 560 TSRMLS_CC);
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		_5 = ZEPHIR_GE_LONG(_0, zephir_fast_count_int(_1 TSRMLS_CC));
		if (_5) {
			_5 = !ZEPHIR_IS_STRING_IDENTICAL(mod, "else");
		}
		_6 = _5;
		if (_6) {
			_6 = !ZEPHIR_IS_STRING_IDENTICAL(mod, "end");
		}
		if (_6) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Extra modifier %mod**", 1);
		}
		if (ZEPHIR_IS_STRING_IDENTICAL(mod, "if")) {
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("ifLevel") TSRMLS_CC));
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
			_2 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
			_7 = !zephir_is_true(_2);
			if (_7) {
				_4 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
				_8 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
				zephir_array_fetch_long(&_3, _4, (zephir_get_numberval(_8) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 571 TSRMLS_CC);
				_7 = !zephir_is_true(_3);
			}
			if (_7) {
				_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
				zephir_update_property_this(this_ptr, SL("ifLevelStart"), _9 TSRMLS_CC);
				zephir_update_property_this(this_ptr, SL("comment"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
				RETURN_MM_STRING("/*", 1);
			}
			RETURN_MM_STRING("", 1);
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(mod, "else")) {
				_8 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevelStart"), PH_NOISY_CC);
				_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
				if (ZEPHIR_IS_IDENTICAL(_8, _9)) {
					ZEPHIR_INIT_ZVAL_NREF(_10);
					ZVAL_LONG(_10, 0);
					zephir_update_property_this(this_ptr, SL("ifLevelStart"), _10 TSRMLS_CC);
					zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
					RETURN_MM_STRING("*/", 1);
				} else {
					_10 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
					if (!zephir_is_true(_10)) {
						_11 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
						zephir_update_property_this(this_ptr, SL("ifLevelStart"), _11 TSRMLS_CC);
						zephir_update_property_this(this_ptr, SL("comment"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("/*", 1);
					}
				}
			} else {
				if (ZEPHIR_IS_STRING_IDENTICAL(mod, "end")) {
					RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("ifLevel") TSRMLS_CC));
					_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevelStart"), PH_NOISY_CC);
					_10 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
					if (ZEPHIR_IS_LONG_IDENTICAL(_9, (zephir_get_numberval(_10) + 1))) {
						ZEPHIR_INIT_ZVAL_NREF(_11);
						ZVAL_LONG(_11, 0);
						zephir_update_property_this(this_ptr, SL("ifLevelStart"), _11 TSRMLS_CC);
						zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("*/", 1);
					}
					RETURN_MM_STRING("", 1);
				} else {
					if (ZEPHIR_IS_STRING_IDENTICAL(mod, "ex")) {
						_10 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
						_11 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
						_12 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
						ZEPHIR_OBS_VAR(_14);
						zephir_read_property_this(&_14, this_ptr, SL("cursor"), PH_NOISY_CC);
						zephir_array_fetch(&_13, _12, _14, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 601 TSRMLS_CC);
						ZEPHIR_INIT_VAR(_15);
						ZVAL_LONG(_15, 1);
						Z_SET_ISREF_P(_10);
						ZEPHIR_CALL_FUNCTION(NULL, "array_splice", &_16, _10, _11, _15, _13);
						Z_UNSET_ISREF_P(_10);
						zephir_check_call_status();
						RETURN_MM_STRING("", 1);
					} else {
						if (ZEPHIR_IS_STRING_IDENTICAL(mod, "lmt")) {
							_11 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
							ZEPHIR_OBS_NVAR(_14);
							zephir_read_property_this(&_14, this_ptr, SL("cursor"), PH_NOISY_CC);
							zephir_array_fetch(&_13, _11, _14, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 605 TSRMLS_CC);
							if (Z_TYPE_P(_13) != IS_NULL) {
								_12 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								ZEPHIR_OBS_VAR(_17);
								ZEPHIR_OBS_VAR(_18);
								zephir_read_property_this(&_18, this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch(&_17, _12, _18, PH_NOISY, "fastorm/db/Translator.zep", 606 TSRMLS_CC);
								ZEPHIR_INIT_ZVAL_NREF(_19);
								ZVAL_LONG(_19, zephir_get_intval(_17));
								zephir_update_property_this(this_ptr, SL("limit"), _19 TSRMLS_CC);
							}
							RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
							RETURN_MM_STRING("", 1);
						} else {
							if (ZEPHIR_IS_STRING_IDENTICAL(mod, "ofs")) {
								_19 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								ZEPHIR_OBS_NVAR(_17);
								zephir_read_property_this(&_17, this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch(&_20, _19, _17, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 612 TSRMLS_CC);
								if (Z_TYPE_P(_20) != IS_NULL) {
									_21 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
									ZEPHIR_OBS_NVAR(_18);
									ZEPHIR_OBS_VAR(_22);
									zephir_read_property_this(&_22, this_ptr, SL("cursor"), PH_NOISY_CC);
									zephir_array_fetch(&_18, _21, _22, PH_NOISY, "fastorm/db/Translator.zep", 613 TSRMLS_CC);
									ZEPHIR_INIT_ZVAL_NREF(_23);
									ZVAL_LONG(_23, zephir_get_intval(_18));
									zephir_update_property_this(this_ptr, SL("offset"), _23 TSRMLS_CC);
								}
								RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
								RETURN_MM_STRING("", 1);
							} else {
								RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
								_19 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								_21 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch_long(&_20, _19, (zephir_get_numberval(_21) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 620 TSRMLS_CC);
								ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, _20, mod);
								zephir_check_call_status();
								RETURN_MM();
							}
						}
					}
				}
			}
		}
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		RETURN_MM_STRING("...", 1);
	}
	zephir_array_fetch_long(&_3, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 628 TSRMLS_CC);
	if (zephir_is_true(_3)) {
		zephir_array_fetch_long(&_13, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 629 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", NULL, _13);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		zephir_array_fetch_long(&_3, matches, 2, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 631 TSRMLS_CC);
		if (zephir_is_true(_3)) {
			zephir_array_fetch_long(&_20, matches, 2, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 632 TSRMLS_CC);
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", NULL, _20);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			zephir_array_fetch_long(&_24, matches, 3, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 634 TSRMLS_CC);
			if (zephir_is_true(_24)) {
				_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_15);
				zephir_array_fetch_long(&_25, matches, 4, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 635 TSRMLS_CC);
				ZEPHIR_SINIT_VAR(_26);
				ZVAL_STRING(&_26, "''", 0);
				ZEPHIR_SINIT_VAR(_27);
				ZVAL_STRING(&_27, "'", 0);
				zephir_fast_str_replace(_15, &_26, &_27, _25);
				ZEPHIR_INIT_VAR(_28);
				ZVAL_STRING(_28, "s", 0);
				ZEPHIR_RETURN_CALL_METHOD(_0, "escape", NULL, _15, _28);
				zephir_check_temp_parameter(_28);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				zephir_array_fetch_long(&_25, matches, 5, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 637 TSRMLS_CC);
				if (zephir_is_true(_25)) {
					_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_INIT_NVAR(_28);
					zephir_array_fetch_long(&_29, matches, 6, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 638 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26);
					ZVAL_STRING(&_26, "\"\"", 0);
					ZEPHIR_SINIT_NVAR(_27);
					ZVAL_STRING(&_27, "\"", 0);
					zephir_fast_str_replace(_28, &_26, &_27, _29);
					ZEPHIR_INIT_VAR(_30);
					ZVAL_STRING(_30, "s", 0);
					ZEPHIR_RETURN_CALL_METHOD(_1, "escape", NULL, _28, _30);
					zephir_check_temp_parameter(_30);
					zephir_check_call_status();
					RETURN_MM();
				} else {
					zephir_array_fetch_long(&_29, matches, 7, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 640 TSRMLS_CC);
					if (zephir_is_true(_29)) {
						zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("**Alone quote**", 1);
					}
				}
			}
		}
	}
	zephir_array_fetch_long(&_3, matches, 8, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 645 TSRMLS_CC);
	if (zephir_is_true(_3)) {
		zephir_array_fetch_long(&_13, matches, 8, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 647 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_26);
		ZVAL_LONG(&_26, 0);
		ZEPHIR_SINIT_NVAR(_27);
		ZVAL_LONG(&_27, -1);
		ZEPHIR_CALL_FUNCTION(&m, "substr", &_31, _13, &_26, &_27);
		zephir_check_call_status();
		zephir_array_fetch_long(&_24, matches, 9, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 648 TSRMLS_CC);
		if (ZEPHIR_IS_STRING(_24, "")) {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, m);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			zephir_array_fetch_long(&_24, matches, 9, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 651 TSRMLS_CC);
			ZEPHIR_CONCAT_VV(return_value, m, _24);
			RETURN_MM();
		}
	}
	RETURN_MM_STRING("this should be never executed", 1);

}

static PHP_METHOD(Fastorm_Db_Translator, delimite) {

	int ZEPHIR_LAST_CALL_STATUS;
	HashTable *_1;
	HashPosition _0;
	zval *value, *parts, *k = NULL, *v = NULL, **_2, *_3, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_INIT_VAR(parts);
	zephir_fast_explode_str(parts, SL("."), value, LONG_MAX TSRMLS_CC);
	zephir_is_iterable(parts, &_1, &_0, 0, 0, "fastorm/db/Translator.zep", 675);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(k, _1, _0);
		ZEPHIR_GET_HVALUE(v, _2);
		if (!ZEPHIR_IS_STRING_IDENTICAL(v, "*")) {
			_3 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "n", 0);
			ZEPHIR_CALL_METHOD(&_4, _3, "escape", NULL, v, _5);
			zephir_check_temp_parameter(_5);
			zephir_check_call_status();
			zephir_array_update_zval(&parts, k, &_4, PH_COPY | PH_SEPARATE);
		}
	}
	zephir_fast_join_str(return_value, SL("."), parts TSRMLS_CC);
	RETURN_MM();

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_DbObject) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm, DbObject, fastorm, dbobject, fastorm_dataobject_ce, fastorm_dbobject_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(fastorm_dbobject_ce, SL("_joinCache"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_dbobject_ce, SL("_affectedRows"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_DbObject, getDbContext) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_SELF(&_0, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_SELF("getdbcontextwithmetadata", NULL, _0);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DbObject, getSelect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *metadata = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	object_init_ex(return_value, fastorm_objectquery_ce);
	ZEPHIR_CALL_SELF(&_0, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, _0, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DbObject, getDbContextWithMetadata) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	zval *metadata, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &metadata);



	if (!(zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	ZEPHIR_CALL_CE_STATIC(&_0, fastorm_modelmanager_ce, "getshared", &_1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getconnection", NULL, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DbObject, getById) {

	zval *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool joinAll, _0;
	zval *id, *joinAll_param = NULL, *metadata = NULL, *select, *_1 = NULL, *_3 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &id, &joinAll_param, &metadata);

	if (!joinAll_param) {
		joinAll = 0;
	} else {
		joinAll = zephir_get_boolval(joinAll_param);
	}
	if (!metadata) {
		ZEPHIR_CPY_WRT(metadata, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(metadata);
	}


	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	if (Z_TYPE_P(metadata) == IS_NULL) {
		ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(select);
	object_init_ex(select, fastorm_objectquery_ce);
	ZEPHIR_CALL_SELF(&_1, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, select, "__construct", NULL, _1, metadata);
	zephir_check_call_status();
	if (Z_TYPE_P(id) == IS_STRING) {
		ZEPHIR_INIT_VAR(_2);
		array_init_size(_2, 3);
		ZEPHIR_CALL_METHOD(&_3, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_4, metadata, "getidfield",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_5);
		ZEPHIR_CONCAT_VSVS(_5, _3, ".", _4, " = %s");
		zephir_array_fast_append(_2, _5);
		zephir_array_fast_append(_2, id);
		ZEPHIR_INIT_VAR(_6);
		ZVAL_STRING(_6, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, select, "__call", NULL, _6, _2);
		zephir_check_temp_parameter(_6);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_2);
		array_init_size(_2, 3);
		ZEPHIR_CALL_METHOD(&_3, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_4, metadata, "getidfield",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_5);
		ZEPHIR_CONCAT_VSVS(_5, _3, ".", _4, " = %i");
		zephir_array_fast_append(_2, _5);
		zephir_array_fast_append(_2, id);
		ZEPHIR_INIT_NVAR(_6);
		ZVAL_STRING(_6, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, select, "__call", NULL, _6, _2);
		zephir_check_temp_parameter(_6);
		zephir_check_call_status();
	}
	ZEPHIR_RETURN_CALL_METHOD(select, "fetchfirst", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_DbObject, __call) {

	zend_class_entry *_23;
	HashTable *_17;
	HashPosition _16;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_10 = NULL, *_13 = NULL;
	zend_bool isSetter, _3, _5, _6;
	zval *method, *args, *m, *propertyName = NULL, *metadata = NULL, *joinedMetadata = NULL, *_0, *_1 = NULL, *_4 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL, *_11, *_12, *ret = NULL, *alias = NULL, *_14, *key = NULL, *value = NULL, *_15 = NULL, **_18, *_19, *_20, *_21, *className = NULL, *_22 = NULL, *_24, *idField = NULL, *_25, *_26;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &method, &args);



	ZEPHIR_INIT_VAR(m);
	ZVAL_NULL(m);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "/^set(.+)$/", 0);
	Z_SET_ISREF_P(m);
	ZEPHIR_CALL_FUNCTION(&_1, "preg_match", &_2, _0, method, m);
	zephir_check_temp_parameter(_0);
	Z_UNSET_ISREF_P(m);
	zephir_check_call_status();
	_3 = zephir_is_true(_1);
	if (_3) {
		_3 = zephir_fast_count_int(args TSRMLS_CC) == 1;
	}
	if (_3) {
		isSetter = 1;
	} else {
		ZEPHIR_INIT_BNVAR(_0);
		ZVAL_STRING(_0, "/^get(.+)$/", 0);
		Z_SET_ISREF_P(m);
		ZEPHIR_CALL_FUNCTION(&_4, "preg_match", &_2, _0, method, m);
		zephir_check_temp_parameter(_0);
		Z_UNSET_ISREF_P(m);
		zephir_check_call_status();
		_5 = zephir_is_true(_4);
		if (_5) {
			_5 = zephir_fast_count_int(args TSRMLS_CC) == 0;
		}
		_6 = _5;
		if (!(_6)) {
			ZEPHIR_OBS_VAR(_7);
			zephir_array_fetch_long(&_7, args, 0, PH_NOISY, "fastorm/DbObject.zep", 51 TSRMLS_CC);
			_6 = Z_TYPE_P(_7) == IS_BOOL;
		}
		if (_6) {
			isSetter = 0;
		} else {
			ZEPHIR_INIT_VAR(_8);
			object_init_ex(_8, fastorm_exception_ce);
			ZEPHIR_INIT_VAR(_9);
			ZEPHIR_CONCAT_SVS(_9, "Method '", method, "' not implemented");
			ZEPHIR_CALL_METHOD(NULL, _8, "__construct", NULL, _9);
			zephir_check_call_status();
			zephir_throw_exception_debug(_8, "fastorm/DbObject.zep", 54 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	zephir_array_fetch_long(&_11, m, 1, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 58 TSRMLS_CC);
	ZEPHIR_CALL_CE_STATIC(&propertyName, fastorm_objectmetadata_ce, "topropertyname", &_10, _11);
	zephir_check_call_status();
	ZEPHIR_INIT_LNVAR(_8);
	ZEPHIR_CONCAT_VS(_8, propertyName, "_id");
	ZEPHIR_CALL_METHOD(&_4, metadata, "hasjoin", NULL, _8);
	zephir_check_call_status();
	if (zephir_is_true(_4)) {
		ZEPHIR_INIT_LNVAR(_9);
		ZEPHIR_CONCAT_VS(_9, propertyName, "_id");
		ZEPHIR_CPY_WRT(propertyName, _9);
	}
	ZEPHIR_CALL_METHOD(&joinedMetadata, metadata, "getjoin", NULL, propertyName);
	zephir_check_call_status();
	if (Z_TYPE_P(joinedMetadata) == IS_NULL) {
		ZEPHIR_INIT_LNVAR(_9);
		object_init_ex(_9, fastorm_exception_ce);
		ZEPHIR_INIT_VAR(_12);
		ZEPHIR_CONCAT_SVS(_12, "Method '", method, "' not implemented or target object not defined");
		ZEPHIR_CALL_METHOD(NULL, _9, "__construct", NULL, _12);
		zephir_check_call_status();
		zephir_throw_exception_debug(_9, "fastorm/DbObject.zep", 67 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	if (isSetter == 0) {
		ZEPHIR_OBS_NVAR(_7);
		zephir_read_property_zval(&_7, this_ptr, propertyName, PH_NOISY_CC);
		if (Z_TYPE_P(_7) == IS_NULL) {
			RETURN_MM_NULL();
		} else {
			ZEPHIR_CALL_METHOD(&_4, joinedMetadata, "getidfield",  NULL);
			zephir_check_call_status();
			ZEPHIR_CALL_CE_STATIC(&alias, fastorm_objectmetadata_ce, "makealias", &_13, propertyName, _4);
			zephir_check_call_status();
			_14 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
			if (zephir_array_isset(_14, alias)) {
				ZEPHIR_CALL_METHOD(&ret, joinedMetadata, "newinstance",  NULL);
				zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&_15, joinedMetadata, "getfields",  NULL);
				zephir_check_call_status();
				zephir_is_iterable(_15, &_17, &_16, 0, 0, "fastorm/DbObject.zep", 85);
				for (
				  ; zephir_hash_get_current_data_ex(_17, (void**) &_18, &_16) == SUCCESS
				  ; zephir_hash_move_forward_ex(_17, &_16)
				) {
					ZEPHIR_GET_HMKEY(key, _17, _16);
					ZEPHIR_GET_HVALUE(value, _18);
					ZEPHIR_CALL_CE_STATIC(&alias, fastorm_objectmetadata_ce, "makealias", &_13, propertyName, key);
					zephir_check_call_status();
					_19 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
					if (zephir_array_isset(_19, alias)) {
						_20 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
						zephir_array_fetch(&_21, _20, alias, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 82 TSRMLS_CC);
						zephir_update_property_zval_zval(ret, key, _21 TSRMLS_CC);
					}
				}
			} else {
				ZEPHIR_CALL_METHOD(&className, joinedMetadata, "getclassname",  NULL);
				zephir_check_call_status();
				ZEPHIR_OBS_VAR(_22);
				zephir_read_property_zval(&_22, this_ptr, propertyName, PH_NOISY_CC);
				_23 = zend_fetch_class(Z_STRVAL_P(className), Z_STRLEN_P(className), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				ZEPHIR_CALL_CE_STATIC(&ret, _23, "getbyid", NULL, _22, ZEPHIR_GLOBAL(global_false), joinedMetadata);
				zephir_check_call_status();
			}
			_5 = zephir_array_isset_long(args, 0);
			if (_5) {
				zephir_array_fetch_long(&_21, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 90 TSRMLS_CC);
				_5 = ZEPHIR_IS_TRUE_IDENTICAL(_21);
			}
			if (_5) {
				zephir_update_property_array(this_ptr, SL("_joinCache"), propertyName, ret TSRMLS_CC);
			}
			RETURN_CCTOR(ret);
		}
	} else {
		ZEPHIR_OBS_NVAR(_7);
		zephir_array_fetch_long(&_7, args, 0, PH_NOISY, "fastorm/DbObject.zep", 96 TSRMLS_CC);
		_5 = zephir_instance_of_ev(_7, fastorm_dataobject_ce TSRMLS_CC);
		if (!(_5)) {
			zephir_array_fetch_long(&_24, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 96 TSRMLS_CC);
			_5 = Z_TYPE_P(_24) == IS_NULL;
		}
		if (_5) {
			ZEPHIR_CALL_METHOD(&idField, joinedMetadata, "getidfield",  NULL);
			zephir_check_call_status();
			ZEPHIR_OBS_NVAR(_22);
			zephir_array_fetch_long(&_22, args, 0, PH_NOISY, "fastorm/DbObject.zep", 99 TSRMLS_CC);
			ZEPHIR_OBS_VAR(_25);
			zephir_read_property_zval(&_25, _22, idField, PH_NOISY_CC);
			zephir_update_property_zval_zval(this_ptr, propertyName, _25 TSRMLS_CC);
			_14 = zephir_fetch_nproperty_this(this_ptr, SL("_joinCache"), PH_NOISY_CC);
			if (Z_TYPE_P(_14) == IS_NULL) {
				ZEPHIR_INIT_BNVAR(_0);
				array_init(_0);
				zephir_update_property_this(this_ptr, SL("_joinCache"), _0 TSRMLS_CC);
			}
			zephir_array_fetch_long(&_26, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 103 TSRMLS_CC);
			zephir_update_property_array(this_ptr, SL("_joinCache"), propertyName, _26 TSRMLS_CC);
		} else {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_exception_ce, "Object in setter must be Dataobject type", "fastorm/DbObject.zep", 105);
			return;
		}
		RETURN_THIS();
	}
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_DbObject, onBeforeCreate) {

	zval *calledFromSave = NULL;

	zephir_fetch_params(0, 0, 1, &calledFromSave);

	if (!calledFromSave) {
		calledFromSave = ZEPHIR_GLOBAL(global_false);
	}



}

static PHP_METHOD(Fastorm_DbObject, onBeforeUpdate) {

	zval *remove = NULL;

	zephir_fetch_params(0, 0, 1, &remove);

	if (!remove) {
		remove = ZEPHIR_GLOBAL(global_false);
	}



}

static PHP_METHOD(Fastorm_DbObject, onAfterUpdate) {

	zval *removed = NULL;

	zephir_fetch_params(0, 0, 1, &removed);

	if (!removed) {
		removed = ZEPHIR_GLOBAL(global_false);
	}



}

static PHP_METHOD(Fastorm_DbObject, create) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *insertIgnore_param = NULL, *db = NULL, *metadata = NULL, *autoincrement = NULL, *query = NULL, *_0 = NULL, *_1 = NULL, *_2, *_3 = NULL, *_4, *_6 = NULL;
	zend_bool insertIgnore, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &insertIgnore_param);

	if (!insertIgnore_param) {
		insertIgnore = 0;
	} else {
		insertIgnore = zephir_get_boolval(insertIgnore_param);
	}


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforecreate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, metadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "getdbformateddata", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&query, db, "insert", NULL, _0, _1);
	zephir_check_call_status();
	if (insertIgnore) {
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "IGNORE", 0);
		ZEPHIR_CALL_METHOD(NULL, query, "setflag", NULL, _2);
		zephir_check_temp_parameter(_2);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_METHOD(&_3, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _3 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&autoincrement, metadata, "getautoincrementkey",  NULL);
	zephir_check_call_status();
	_4 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	_5 = zephir_is_true(_4);
	if (_5) {
		_5 = Z_TYPE_P(autoincrement) != IS_NULL;
	}
	if (_5) {
		ZEPHIR_CALL_METHOD(&_6, db, "getinsertid",  NULL);
		zephir_check_call_status();
		zephir_update_property_zval_zval(this_ptr, autoincrement, _6 TSRMLS_CC);
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_DbObject, save) {

	zend_bool _9;
	zval *_5;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *db = NULL, *metadata = NULL, *autoincrement = NULL, *data, *lastInsert = NULL, *query = NULL, *propName = NULL, *propFlag = NULL, *e = NULL, *_0 = NULL, **_3, *_4 = NULL, *_6, *_7 = NULL, *_8, *_10 = NULL, *_11;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforecreate", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_INIT_VAR(lastInsert);
	ZVAL_STRING(lastInsert, "", 1);
	ZEPHIR_INIT_VAR(autoincrement);
	ZVAL_NULL(autoincrement);
	ZEPHIR_CALL_METHOD(&_0, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/DbObject.zep", 167);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(propName, _2, _1);
		ZEPHIR_GET_HVALUE(propFlag, _3);
		if (!(((int) (zephir_get_numberval(propFlag)) & 4))) {
			ZEPHIR_OBS_NVAR(_4);
			zephir_read_property_zval(&_4, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&data, propName, &_4, PH_COPY | PH_SEPARATE);
		} else {
			ZEPHIR_INIT_NVAR(lastInsert);
			ZEPHIR_CONCAT_SVSVS(lastInsert, "`", propName, "` = LAST_INSERT_ID(`", propName, "`)");
			ZEPHIR_CPY_WRT(autoincrement, propName);
		}
	}
	ZEPHIR_INIT_VAR(_5);
	array_init_size(_5, 7);
	ZEPHIR_INIT_VAR(_6);
	ZVAL_STRING(_6, "INSERT INTO %n %v ON DUPLICATE KEY UPDATE %sql %a", 1);
	zephir_array_fast_append(_5, _6);
	ZEPHIR_CALL_METHOD(&_7, metadata, "gettable",  NULL);
	zephir_check_call_status();
	zephir_array_fast_append(_5, _7);
	zephir_array_fast_append(_5, data);
	zephir_array_fast_append(_5, lastInsert);
	zephir_array_fast_append(_5, data);
	ZEPHIR_CALL_METHOD(&query, db, "queryarray", NULL, _5);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_7, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _7 TSRMLS_CC);
	_8 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	_9 = zephir_is_true(_8);
	if (_9) {
		_9 = Z_TYPE_P(autoincrement) != IS_NULL;
	}
	if (_9) {

		/* try_start_1: */

			ZEPHIR_CALL_METHOD(&_10, db, "getinsertid",  NULL);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_update_property_zval_zval(this_ptr, autoincrement, _10 TSRMLS_CC);

		try_end_1:

		if (EG(exception)) {
			ZEPHIR_CPY_WRT(e, EG(exception));
			if (zephir_is_instance_of(e, SL("Db\\DbException") TSRMLS_CC)) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CALL_METHOD(NULL, e, "getmessage", NULL);
				zephir_check_call_status();
			}
		}
	}
	_11 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_11)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_DbObject, update) {

	zval *_8;
	zend_bool _5;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *onlyColumnsArray = NULL, *db = NULL, *metadata = NULL, autoincrement, *data, *query = NULL, *propName = NULL, *propFlag = NULL, *keys, *_0 = NULL, **_3, *_4 = NULL, *_6, *_7 = NULL, *_9 = NULL, *_10 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &onlyColumnsArray);

	if (!onlyColumnsArray) {
		onlyColumnsArray = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_SINIT_VAR(autoincrement);
	ZVAL_NULL(&autoincrement);
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_CALL_METHOD(&_0, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/DbObject.zep", 210);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(propName, _2, _1);
		ZEPHIR_GET_HVALUE(propFlag, _3);
		if (((int) (zephir_get_numberval(propFlag)) & 2)) {
			ZEPHIR_OBS_NVAR(_4);
			zephir_read_property_zval(&_4, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&keys, propName, &_4, PH_COPY | PH_SEPARATE);
		} else {
			_5 = Z_TYPE_P(onlyColumnsArray) == IS_NULL;
			if (!(_5)) {
				_5 = zephir_fast_in_array(propName, onlyColumnsArray TSRMLS_CC);
			}
			if (_5) {
				ZEPHIR_OBS_NVAR(_4);
				zephir_read_property_zval(&_4, this_ptr, propName, PH_NOISY_CC);
				zephir_array_update_zval(&data, propName, &_4, PH_COPY | PH_SEPARATE);
			}
		}
	}
	if (zephir_fast_count_int(data TSRMLS_CC) == 0) {
		ZEPHIR_INIT_ZVAL_NREF(_6);
		ZVAL_LONG(_6, 0);
		zephir_update_property_this(this_ptr, SL("_affectedRows"), _6 TSRMLS_CC);
	} else {
		ZEPHIR_CALL_METHOD(&_7, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&query, db, "update", NULL, _7, data);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_8);
		array_init_size(_8, 3);
		ZEPHIR_INIT_VAR(_9);
		ZVAL_STRING(_9, "%and", 1);
		zephir_array_fast_append(_8, _9);
		zephir_array_fast_append(_8, keys);
		ZEPHIR_INIT_NVAR(_9);
		ZVAL_STRING(_9, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, query, "__call", NULL, _9, _8);
		zephir_check_temp_parameter(_9);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_10, query, "execute",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("_affectedRows"), _10 TSRMLS_CC);
	}
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_6)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_DbObject, delete) {

	zval *_6;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *db = NULL, *metadata = NULL, autoincrement, *query = NULL, *propName = NULL, *propFlag = NULL, *keys, *_0 = NULL, **_3, *_4 = NULL, *_5 = NULL, *_7, *_8 = NULL, *_9;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", NULL, metadata);
	zephir_check_call_status();
	ZEPHIR_SINIT_VAR(autoincrement);
	ZVAL_NULL(&autoincrement);
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_CALL_METHOD(&_0, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/DbObject.zep", 243);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(propName, _2, _1);
		ZEPHIR_GET_HVALUE(propFlag, _3);
		if (((int) (zephir_get_numberval(propFlag)) & 2)) {
			ZEPHIR_OBS_NVAR(_4);
			zephir_read_property_zval(&_4, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&keys, propName, &_4, PH_COPY | PH_SEPARATE);
		}
	}
	ZEPHIR_CALL_METHOD(&_5, metadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&query, db, "delete", NULL, _5);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_6);
	array_init_size(_6, 3);
	ZEPHIR_INIT_VAR(_7);
	ZVAL_STRING(_7, "%and", 1);
	zephir_array_fast_append(_6, _7);
	zephir_array_fast_append(_6, keys);
	ZEPHIR_INIT_BNVAR(_7);
	ZVAL_STRING(_7, "where", 0);
	ZEPHIR_CALL_METHOD(NULL, query, "__call", NULL, _7, _6);
	zephir_check_temp_parameter(_7);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_8, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _8 TSRMLS_CC);
	_9 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_9)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_DbObject, hasMany) {



}

static PHP_METHOD(Fastorm_DbObject, getAffectedRows) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "You can use getAffectedRows() method only after insert, update or remove", "fastorm/DbObject.zep", 261);
		return;
	} else {
		RETURN_MEMBER_QUICK(this_ptr, "_affectedRows", 3226297793UL);
	}

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_Exception) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm, Exception, fastorm, exception, zend_exception_get_default(TSRMLS_C), NULL, 0);

	return SUCCESS;

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_exceptions.h>



ZEPHIR_INIT_CLASS(Fastorm_IConnectionManager) {

	ZEPHIR_REGISTER_INTERFACE(Fastorm, IConnectionManager, fastorm, iconnectionmanager, fastorm_iconnectionmanager_method_entry);

	return SUCCESS;

}

ZEPHIR_DOC_METHOD(Fastorm_IConnectionManager, getConnection);




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_ModelManager) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ModelManager, fastorm, modelmanager, fastorm_modelmanager_method_entry, 0);

	zend_declare_property_null(fastorm_modelmanager_ce, SL("_sharedInstance"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_modelmanager_ce, SL("connectionManager"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_ModelManager, getShared) {

	zval *_0, *_1;


	_0 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "Model Manager not initialized", "fastorm/ModelManager.zep", 14);
		return;
	}
	_1 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	RETURN_CTORW(_1);

}

static PHP_METHOD(Fastorm_ModelManager, __construct) {

	zval *connectionManager, *_0;

	zephir_fetch_params(0, 1, 0, &connectionManager);



	if (!(zephir_instance_of_ev(connectionManager, fastorm_iconnectionmanager_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'connectionManager' must be an instance of 'Fastorm\\IConnectionManager'", "", 0);
		return;
	}
	_0 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	if (Z_TYPE_P(_0) != IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "Model Manager can be initialized only once", "fastorm/ModelManager.zep", 22);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connectionManager"), connectionManager TSRMLS_CC);
	zephir_update_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance"), this_ptr TSRMLS_CC);

}

static PHP_METHOD(Fastorm_ModelManager, getConnection) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *metadata, *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &metadata);



	if (!(zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connectionManager"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, metadata, "getstorage",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getconnection", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_ObjectMetadata) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ObjectMetadata, fastorm, objectmetadata, fastorm_objectmetadata_method_entry, 0);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("_metadataCache"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("table"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("storage"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("idField"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("reflection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("className"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("fields"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("joins"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("aliases"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("PROPERTY"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("KEY"), 2 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("AUTOINCREMENT"), 4 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("JOINED"), 8 TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_ObjectMetadata, __construct) {

	zval *fields = NULL, *joins = NULL;
	zval *className = NULL, *idField = NULL, *table = NULL, *storage = NULL;
	zval *reflection, *className_param = NULL, *fields_param = NULL, *joins_param = NULL, *idField_param = NULL, *table_param = NULL, *storage_param = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 3, &reflection, &className_param, &fields_param, &joins_param, &idField_param, &table_param, &storage_param);

	zephir_get_strval(className, className_param);
	zephir_get_arrval(fields, fields_param);
	zephir_get_arrval(joins, joins_param);
	if (!idField_param) {
		ZEPHIR_INIT_VAR(idField);
		ZVAL_EMPTY_STRING(idField);
	} else {
		zephir_get_strval(idField, idField_param);
	}
	if (!table_param) {
		ZEPHIR_INIT_VAR(table);
		ZVAL_EMPTY_STRING(table);
	} else {
		zephir_get_strval(table, table_param);
	}
	if (!storage_param) {
		ZEPHIR_INIT_VAR(storage);
		ZVAL_EMPTY_STRING(storage);
	} else {
		zephir_get_strval(storage, storage_param);
	}


	if (!(zephir_is_instance_of(reflection, SL("ReflectionClass") TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'reflection' must be an instance of 'ReflectionClass'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("reflection"), reflection TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("idField"), idField TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("table"), table TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("storage"), storage TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("fields"), fields TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("joins"), joins TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("className"), className TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("aliases"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_ObjectMetadata, getTable) {


	RETURN_MEMBER_QUICK(this_ptr, "table", 495471661UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getStorage) {


	RETURN_MEMBER_QUICK(this_ptr, "storage", 2312891834UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getIdField) {


	RETURN_MEMBER_QUICK(this_ptr, "idField", 526498102UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getFields) {


	RETURN_MEMBER_QUICK(this_ptr, "fields", 2881624156UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getClassName) {


	RETURN_MEMBER_QUICK(this_ptr, "className", 362439804UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, newInstance) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_class_entry *_1;
	zval *data_param = NULL, *className = NULL, *_0;
	zval *data = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &data_param);

	if (!data_param) {
	ZEPHIR_INIT_VAR(data);
	ZVAL_NULL(data);
	} else {
		zephir_get_arrval(data, data_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("className"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(className, _0);
	_1 = zend_fetch_class(Z_STRVAL_P(className), Z_STRLEN_P(className), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(return_value, _1);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, data);
		zephir_check_call_status();
	}
	RETURN_MM();

}

static PHP_METHOD(Fastorm_ObjectMetadata, getReflection) {


	RETURN_MEMBER_QUICK(this_ptr, "reflection", 1246746928UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getMetadata) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *className_param = NULL, *_0, *_1 = NULL, *_3, *_4;
	zval *className = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &className_param);

	zephir_get_strval(className, className_param);


	_0 = zephir_fetch_static_property_ce(fastorm_objectmetadata_ce, SL("_metadataCache") TSRMLS_CC);
	if (!(zephir_array_isset(_0, className))) {
		ZEPHIR_CALL_SELF(&_1, "createmetadata", &_2, className);
		zephir_check_call_status();
		zephir_update_static_property_array_multi_ce(fastorm_objectmetadata_ce, SL("_metadataCache"), &_1 TSRMLS_CC, SL("z"), 1, className);
	}
	_3 = zephir_fetch_static_property_ce(fastorm_objectmetadata_ce, SL("_metadataCache") TSRMLS_CC);
	zephir_array_fetch(&_4, _3, className, PH_NOISY | PH_READONLY, "fastorm/ObjectMetadata.zep", 85 TSRMLS_CC);
	RETURN_CTOR(_4);

}

static PHP_METHOD(Fastorm_ObjectMetadata, createMetadata) {

	zend_bool _12, _13;
	HashTable *_7;
	HashPosition _6;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zend_class_entry *_0;
	zval *propertiesArray, *joins;
	int prop, ZEPHIR_LAST_CALL_STATUS;
	zval *className_param = NULL, *reflection, *properties = NULL, *docs = NULL, *matches, *property = NULL, *propName = NULL, *idField = NULL, *table = NULL, *storage = NULL, *_1 = NULL, *_2 = NULL, *_4 = NULL, *_5 = NULL, **_8, *_9 = NULL, _10 = zval_used_for_init, *_11 = NULL, *_14 = NULL, *_15 = NULL, *_16;
	zval *className = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &className_param);

	zephir_get_strval(className, className_param);


	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);
	ZEPHIR_INIT_VAR(reflection);
	_0 = zend_fetch_class(SL("ReflectionClass"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(reflection, _0);
	ZEPHIR_CALL_METHOD(NULL, reflection, "__construct", NULL, className);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 256);
	ZEPHIR_CALL_METHOD(&properties, reflection, "getproperties", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&docs, reflection, "getdoccomment",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "/@id ([A-Za-z0-9_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_2, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		ZEPHIR_OBS_VAR(idField);
		zephir_array_fetch_long(&idField, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 100 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_NVAR(idField);
		ZVAL_NULL(idField);
	}
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "/@table ([A-Za-z0-9_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_4)) {
		ZEPHIR_OBS_VAR(table);
		zephir_array_fetch_long(&table, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 106 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_NVAR(table);
		ZVAL_NULL(table);
	}
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "/@storage ([A-Za-z0-9_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_5, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_5)) {
		ZEPHIR_OBS_VAR(storage);
		zephir_array_fetch_long(&storage, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 112 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_NVAR(storage);
		if (Z_TYPE_P(table) != IS_NULL) {
			ZVAL_STRING(storage, "db", 1);
		} else {
			ZVAL_NULL(storage);
		}
	}
	ZEPHIR_INIT_VAR(propertiesArray);
	array_init(propertiesArray);
	ZEPHIR_INIT_VAR(joins);
	array_init(joins);
	zephir_is_iterable(properties, &_7, &_6, 0, 0, "fastorm/ObjectMetadata.zep", 146);
	for (
	  ; zephir_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
	  ; zephir_hash_move_forward_ex(_7, &_6)
	) {
		ZEPHIR_GET_HVALUE(property, _8);
		ZEPHIR_CALL_METHOD(&_9, property, "isstatic",  NULL);
		zephir_check_call_status();
		if (!(zephir_is_true(_9))) {
			ZEPHIR_CALL_METHOD(&propName, property, "getname",  NULL);
			zephir_check_call_status();
			prop = 1;
			ZEPHIR_CALL_METHOD(&docs, property, "getdoccomment",  NULL);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_10);
			ZVAL_STRING(&_10, "/@autoincrement/i", 0);
			ZEPHIR_CALL_FUNCTION(&_11, "preg_match", &_3, &_10, docs);
			zephir_check_call_status();
			if (zephir_is_true(_11)) {
				prop = (prop | 4);
			}
			_12 = ((prop & 4)) ? 1 : 0;
			if (!(_12)) {
				_12 = ZEPHIR_IS_IDENTICAL(propName, idField);
			}
			_13 = _12;
			if (!(_13)) {
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_STRING(&_10, "/@key/i", 0);
				ZEPHIR_CALL_FUNCTION(&_14, "preg_match", &_3, &_10, docs);
				zephir_check_call_status();
				_13 = zephir_is_true(_14);
			}
			if (_13) {
				prop = (prop | 2);
			}
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "/@join ([A-Za-z0-9_\\\\]+)/i", 0);
			Z_SET_ISREF_P(matches);
			ZEPHIR_CALL_FUNCTION(&_15, "preg_match", &_3, _1, docs, matches);
			zephir_check_temp_parameter(_1);
			Z_UNSET_ISREF_P(matches);
			zephir_check_call_status();
			if (zephir_is_true(_15)) {
				zephir_array_fetch_long(&_16, matches, 1, PH_NOISY | PH_READONLY, "fastorm/ObjectMetadata.zep", 139 TSRMLS_CC);
				zephir_array_update_zval(&joins, propName, &_16, PH_COPY | PH_SEPARATE);
				prop = (prop | 8);
			}
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, prop);
			zephir_array_update_zval(&propertiesArray, propName, &_1, PH_COPY | PH_SEPARATE);
		}
	}
	object_init_ex(return_value, fastorm_objectmetadata_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, reflection, className, propertiesArray, joins, idField, table, storage);
	zephir_check_call_status();
	RETURN_MM();

}

static PHP_METHOD(Fastorm_ObjectMetadata, getJoin) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *propName_param = NULL, *_0, *_1, *_2;
	zval *propName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &propName_param);

	zephir_get_strval(propName, propName_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("joins"), PH_NOISY_CC);
	if (zephir_array_isset(_0, propName)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("joins"), PH_NOISY_CC);
		zephir_array_fetch(&_2, _1, propName, PH_NOISY | PH_READONLY, "fastorm/ObjectMetadata.zep", 151 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_SELF("getmetadata", NULL, _2);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		RETURN_MM_NULL();
	}

}

static PHP_METHOD(Fastorm_ObjectMetadata, hasJoin) {

	zval *propName_param = NULL, *_0;
	zval *propName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &propName_param);

	zephir_get_strval(propName, propName_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("joins"), PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_array_isset(_0, propName));

}

static PHP_METHOD(Fastorm_ObjectMetadata, getJoins) {


	RETURN_MEMBER_QUICK(this_ptr, "joins", 120974824UL);

}

static PHP_METHOD(Fastorm_ObjectMetadata, getAliases) {

	zephir_fcall_cache_entry *_6 = NULL;
	HashTable *_4;
	HashPosition _3;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *aliasTable;
	zend_bool _0;
	zval *propName_param = NULL, *joinedTable = NULL, *_1, *key = NULL, *value = NULL, *aliasName = NULL, *_2 = NULL, **_5, *_7, *_8;
	zval *propName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &propName_param, &joinedTable);

	zephir_get_strval(propName, propName_param);
	if (!joinedTable) {
		ZEPHIR_CPY_WRT(joinedTable, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(joinedTable);
	}


	_0 = Z_TYPE_P(joinedTable) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(joinedTable, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'joinedTable' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("aliases"), PH_NOISY_CC);
	if (!(zephir_array_isset(_1, propName))) {
		if (Z_TYPE_P(joinedTable) == IS_NULL) {
			ZEPHIR_CALL_METHOD(&joinedTable, this_ptr, "getjoin", NULL, propName);
			zephir_check_call_status();
		}
		ZEPHIR_INIT_VAR(aliasTable);
		array_init(aliasTable);
		ZEPHIR_CALL_METHOD(&_2, joinedTable, "getfields",  NULL);
		zephir_check_call_status();
		zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/ObjectMetadata.zep", 178);
		for (
		  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
		  ; zephir_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HMKEY(key, _4, _3);
			ZEPHIR_GET_HVALUE(value, _5);
			ZEPHIR_CALL_SELF(&aliasName, "makealias", &_6, propName, key);
			zephir_check_call_status();
			zephir_array_update_zval(&aliasTable, aliasName, &key, PH_COPY | PH_SEPARATE);
		}
		zephir_update_property_array(this_ptr, SL("aliases"), propName, aliasTable TSRMLS_CC);
	}
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("aliases"), PH_NOISY_CC);
	zephir_array_fetch(&_8, _7, propName, PH_NOISY | PH_READONLY, "fastorm/ObjectMetadata.zep", 180 TSRMLS_CC);
	RETURN_CTOR(_8);

}

static PHP_METHOD(Fastorm_ObjectMetadata, toPropertyName) {

	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_7 = NULL;
	zval *camelcase_param = NULL, *matches, *ret, *match = NULL, *zeroMatches, *_0 = NULL, **_4, *_5 = NULL, *_6 = NULL;
	zval *camelcase = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &camelcase_param);

	zephir_get_strval(camelcase, camelcase_param);


	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "!([A-Z][A-Z0-9]*(?=$|[A-Z][a-z0-9])|[A-Za-z][a-z0-9]+)!", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match_all", &_1, _0, camelcase, matches);
	zephir_check_temp_parameter(_0);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	ZEPHIR_OBS_VAR(zeroMatches);
	zephir_array_fetch_long(&zeroMatches, matches, 0, PH_NOISY, "fastorm/ObjectMetadata.zep", 187 TSRMLS_CC);
	ZEPHIR_INIT_VAR(ret);
	array_init(ret);
	zephir_is_iterable(zeroMatches, &_3, &_2, 0, 0, "fastorm/ObjectMetadata.zep", 198);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(match, _4);
		ZEPHIR_INIT_NVAR(_0);
		zephir_fast_strtoupper(_0, match);
		if (ZEPHIR_IS_IDENTICAL(match, _0)) {
			ZEPHIR_INIT_NVAR(_5);
			zephir_fast_strtolower(_5, match);
			zephir_array_append(&ret, _5, PH_SEPARATE, "fastorm/ObjectMetadata.zep", 192);
		} else {
			ZEPHIR_CALL_FUNCTION(&_6, "lcfirst", &_7, match);
			zephir_check_call_status();
			zephir_array_append(&ret, _6, PH_SEPARATE, "fastorm/ObjectMetadata.zep", 194);
		}
	}
	zephir_fast_join_str(return_value, SL("_"), ret TSRMLS_CC);
	RETURN_MM();

}

static PHP_METHOD(Fastorm_ObjectMetadata, makeAlias) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zval *sourceProperty_param = NULL, *targetProperty_param = NULL, _0, _1, *_2 = NULL;
	zval *sourceProperty = NULL, *targetProperty = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &sourceProperty_param, &targetProperty_param);

	zephir_get_strval(sourceProperty, sourceProperty_param);
	zephir_get_strval(targetProperty, targetProperty_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "/_id$/", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "", 0);
	ZEPHIR_CALL_FUNCTION(&_2, "preg_replace", &_3, &_0, &_1, sourceProperty);
	zephir_check_call_status();
	ZEPHIR_CONCAT_SVSV(return_value, "r_", _2, "_", targetProperty);
	RETURN_MM();

}

static PHP_METHOD(Fastorm_ObjectMetadata, getAutoincrementKey) {

	HashTable *_2;
	HashPosition _1;
	zval *found = NULL, *key = NULL, *flag = NULL, *_0, **_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(found);
	ZVAL_NULL(found);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("fields"), PH_NOISY_CC);
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/ObjectMetadata.zep", 214);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(key, _2, _1);
		ZEPHIR_GET_HVALUE(flag, _3);
		if (((int) (zephir_get_numberval(flag)) & 4)) {
			ZEPHIR_CPY_WRT(found, key);
			break;
		}
	}
	RETURN_CCTOR(found);

}




#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>



ZEPHIR_INIT_CLASS(Fastorm_ObjectQuery) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm, ObjectQuery, fastorm, objectquery, fastorm_db_query_ce, fastorm_objectquery_method_entry, 0);

	zend_declare_property_null(fastorm_objectquery_ce, SL("metadata"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

static PHP_METHOD(Fastorm_ObjectQuery, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *connection, *metadata;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &connection, &metadata);



	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	if (!(zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("metadata"), metadata TSRMLS_CC);
	ZEPHIR_CALL_PARENT(NULL, fastorm_objectquery_ce, this_ptr, "__construct", &_0, connection);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "initiateselect", NULL);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_ObjectQuery, initiateSelect) {

	zval *_7, *_9;
	HashTable *_4;
	HashPosition _3;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *key = NULL, *flags = NULL, *select, *table = NULL, *_0, *_1, *_2 = NULL, **_5, *_6 = NULL, *_8;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(select);
	array_init(select);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&table, _0, "gettable",  NULL);
	zephir_check_call_status();
	if (Z_TYPE_P(table) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_exception_ce, "Object has not set table annotation", "fastorm/ObjectQuery.zep", 22);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_2, _1, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/ObjectQuery.zep", 30);
	for (
	  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
	  ; zephir_hash_move_forward_ex(_4, &_3)
	) {
		ZEPHIR_GET_HMKEY(key, _4, _3);
		ZEPHIR_GET_HVALUE(flags, _5);
		ZEPHIR_INIT_LNVAR(_6);
		ZEPHIR_CONCAT_VSV(_6, table, ".", key);
		zephir_array_append(&select, _6, PH_SEPARATE, "fastorm/ObjectQuery.zep", 27);
	}
	ZEPHIR_INIT_VAR(_7);
	array_init_size(_7, 2);
	ZEPHIR_INIT_VAR(_8);
	zephir_fast_join_str(_8, SL(", "), select TSRMLS_CC);
	zephir_array_fast_append(_7, _8);
	ZEPHIR_INIT_BNVAR(_8);
	ZVAL_STRING(_8, "select", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "__call", NULL, _8, _7);
	zephir_check_temp_parameter(_8);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_9);
	array_init_size(_9, 2);
	zephir_array_fast_append(_9, table);
	ZEPHIR_INIT_BNVAR(_8);
	ZVAL_STRING(_8, "from", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "__call", NULL, _8, _9);
	zephir_check_temp_parameter(_8);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_ObjectQuery, getMetadata) {


	RETURN_MEMBER_QUICK(this_ptr, "metadata", 2348961382UL);

}

static PHP_METHOD(Fastorm_ObjectQuery, joinObject) {

	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL, *_9 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *justColumns = NULL;
	zval *onProperty_param = NULL, *justColumns_param = NULL, *property = NULL, *joinMetadata = NULL, *joinAlias = NULL, *_0, *_1 = NULL, *_4, _5, _6, *_8;
	zval *onProperty = NULL, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &onProperty_param, &justColumns_param);

	zephir_get_strval(onProperty, onProperty_param);
	if (!justColumns_param) {
	ZEPHIR_INIT_VAR(justColumns);
	ZVAL_NULL(justColumns);
	} else {
		zephir_get_arrval(justColumns, justColumns_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_CONCAT_VS(_2, onProperty, "_id");
	ZEPHIR_CALL_METHOD(&_1, _0, "hasjoin", NULL, _2);
	zephir_check_call_status();
	if (zephir_is_true(_1)) {
		ZEPHIR_INIT_VAR(_3);
		ZEPHIR_CONCAT_VS(_3, onProperty, "_id");
		ZEPHIR_CPY_WRT(property, _3);
		ZEPHIR_CPY_WRT(joinAlias, onProperty);
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&joinMetadata, _4, "getjoin", NULL, property);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(property, onProperty);
		ZEPHIR_SINIT_VAR(_5);
		ZVAL_STRING(&_5, "/_id$/", 0);
		ZEPHIR_SINIT_VAR(_6);
		ZVAL_STRING(&_6, "", 0);
		ZEPHIR_CALL_FUNCTION(&joinAlias, "preg_replace", &_7, &_5, &_6, onProperty);
		zephir_check_call_status();
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&joinMetadata, _4, "getjoin", NULL, property);
		zephir_check_call_status();
	}
	if (Z_TYPE_P(joinMetadata) == IS_NULL) {
		ZEPHIR_INIT_VAR(_8);
		object_init_ex(_8, fastorm_exception_ce);
		ZEPHIR_INIT_LNVAR(_3);
		ZEPHIR_CONCAT_SVS(_3, "Property '", onProperty, "' hasn't defined join.");
		ZEPHIR_CALL_METHOD(NULL, _8, "__construct", NULL, _3);
		zephir_check_call_status();
		zephir_throw_exception_debug(_8, "fastorm/ObjectQuery.zep", 53 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "joinobjectwith", &_9, property, joinMetadata, joinAlias, justColumns);
	zephir_check_call_status();
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_ObjectQuery, joinAll) {

	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL, *_9 = NULL;
	zephir_fcall_cache_entry *_5 = NULL;
	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *property = NULL, *className = NULL, *joinMetadata = NULL, *joinAlias = NULL, *_0, *_1 = NULL, **_4, _6 = zval_used_for_init, _7 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, _0, "getjoins",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/ObjectQuery.zep", 68);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(property, _3, _2);
		ZEPHIR_GET_HVALUE(className, _4);
		ZEPHIR_CALL_CE_STATIC(&joinMetadata, fastorm_objectmetadata_ce, "getmetadata", &_5, className);
		zephir_check_call_status();
		ZEPHIR_SINIT_NVAR(_6);
		ZVAL_STRING(&_6, "/_id$/", 0);
		ZEPHIR_SINIT_NVAR(_7);
		ZVAL_STRING(&_7, "", 0);
		ZEPHIR_CALL_FUNCTION(&joinAlias, "preg_replace", &_8, &_6, &_7, property);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "joinobjectwith", &_9, property, joinMetadata, joinAlias);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

static PHP_METHOD(Fastorm_ObjectQuery, joinObjectWith) {

	zval *_8, *_10, *_11;
	zend_bool _4, _5;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *property, *joinMetadata, *joinAlias, *justColumns = NULL, *alias = NULL, *realName = NULL, *aliases = NULL, *select, *_0, **_3, *_6 = NULL, *_7 = NULL, *_9, *_12 = NULL, *_13, *_14 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 1, &property, &joinMetadata, &joinAlias, &justColumns);

	if (!justColumns) {
		justColumns = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(select);
	array_init(select);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&aliases, _0, "getaliases", NULL, property, joinMetadata);
	zephir_check_call_status();
	zephir_is_iterable(aliases, &_2, &_1, 0, 0, "fastorm/ObjectQuery.zep", 80);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(alias, _2, _1);
		ZEPHIR_GET_HVALUE(realName, _3);
		_4 = Z_TYPE_P(justColumns) == IS_NULL;
		if (!(_4)) {
			_4 = zephir_fast_in_array(realName, justColumns TSRMLS_CC);
		}
		_5 = _4;
		if (!(_5)) {
			ZEPHIR_CALL_METHOD(&_6, joinMetadata, "getidfield",  NULL);
			zephir_check_call_status();
			_5 = ZEPHIR_IS_IDENTICAL(realName, _6);
		}
		if (_5) {
			ZEPHIR_INIT_LNVAR(_7);
			ZEPHIR_CONCAT_VSVSV(_7, joinAlias, ".", realName, " AS ", alias);
			zephir_array_append(&select, _7, PH_SEPARATE, "fastorm/ObjectQuery.zep", 77);
		}
	}
	ZEPHIR_INIT_VAR(_8);
	array_init_size(_8, 2);
	ZEPHIR_INIT_VAR(_9);
	zephir_fast_join_str(_9, SL(", "), select TSRMLS_CC);
	zephir_array_fast_append(_8, _9);
	ZEPHIR_INIT_BNVAR(_9);
	ZVAL_STRING(_9, "select", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "__call", NULL, _9, _8);
	zephir_check_temp_parameter(_9);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_10);
	array_init_size(_10, 2);
	ZEPHIR_CALL_METHOD(&_6, joinMetadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_LNVAR(_7);
	ZEPHIR_CONCAT_VSV(_7, _6, " ", joinAlias);
	zephir_array_fast_append(_10, _7);
	ZEPHIR_INIT_BNVAR(_9);
	ZVAL_STRING(_9, "leftJoin", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "__call", NULL, _9, _10);
	zephir_check_temp_parameter(_9);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_11);
	array_init_size(_11, 2);
	ZEPHIR_CALL_METHOD(&_12, joinMetadata, "getidfield",  NULL);
	zephir_check_call_status();
	_13 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_14, _13, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_LNVAR(_7);
	ZEPHIR_CONCAT_VSVSVSV(_7, joinAlias, ".", _12, " = ", _14, ".", property);
	zephir_array_fast_append(_11, _7);
	ZEPHIR_INIT_BNVAR(_9);
	ZVAL_STRING(_9, "on", 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "__call", NULL, _9, _11);
	zephir_check_temp_parameter(_9);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

static PHP_METHOD(Fastorm_ObjectQuery, query) {

	zephir_nts_static zephir_fcall_cache_entry *_9 = NULL;
	zval *_7 = NULL;
	HashTable *_4;
	HashPosition _3;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *args, *res = NULL, *setup = NULL, *_0, *_1, *_2, **_5, *_6 = NULL, *_8 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("metadata"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&res, _0, "queryarray", NULL, args, _1);
	zephir_check_call_status();
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("setups"), PH_NOISY_CC);
	zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/ObjectQuery.zep", 92);
	for (
	  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
	  ; zephir_hash_move_forward_ex(_4, &_3)
	) {
		ZEPHIR_GET_HVALUE(setup, _5);
		ZEPHIR_INIT_NVAR(_6);
		ZEPHIR_INIT_NVAR(_7);
		array_init_size(_7, 3);
		zephir_array_fast_append(_7, res);
		Z_SET_ISREF_P(setup);
		ZEPHIR_CALL_FUNCTION(&_8, "array_shift", &_9, setup);
		Z_UNSET_ISREF_P(setup);
		zephir_check_call_status();
		zephir_array_fast_append(_7, _8);
		ZEPHIR_CALL_USER_FUNC_ARRAY(_6, _7, setup);
		zephir_check_call_status();
	}
	RETURN_CCTOR(res);

}




/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#endif

#include <php.h>

#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif


#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>


zend_class_entry *fastorm_db_iresultdriver_ce;
zend_class_entry *fastorm_iconnectionmanager_ce;
zend_class_entry *fastorm_dataobject_ce;
zend_class_entry *fastorm_db_query_ce;
zend_class_entry *fastorm_dbobject_ce;
zend_class_entry *fastorm_exception_ce;
zend_class_entry *fastorm_connectionmanager_ce;
zend_class_entry *fastorm_db_columninfo_ce;
zend_class_entry *fastorm_db_connection_ce;
zend_class_entry *fastorm_db_dbexception_ce;
zend_class_entry *fastorm_db_drivers_mysqlidriver_ce;
zend_class_entry *fastorm_db_event_ce;
zend_class_entry *fastorm_db_result_ce;
zend_class_entry *fastorm_db_resultiterator_ce;
zend_class_entry *fastorm_db_translator_ce;
zend_class_entry *fastorm_modelmanager_ce;
zend_class_entry *fastorm_objectmetadata_ce;
zend_class_entry *fastorm_objectquery_ce;

ZEND_DECLARE_MODULE_GLOBALS(fastorm)

#define ZEPHIR_NUM_PREALLOCATED_FRAMES 25

void zephir_initialize_memory(zend_zephir_globals_def *zephir_globals_ptr TSRMLS_DC)
{
	zephir_memory_entry *start;
	size_t i;

	start = (zephir_memory_entry *) pecalloc(ZEPHIR_NUM_PREALLOCATED_FRAMES, sizeof(zephir_memory_entry), 1);
/* pecalloc() will take care of these members for every frame
	start->pointer      = 0;
	start->hash_pointer = 0;
	start->prev = NULL;
	start->next = NULL;
*/
	for (i = 0; i < ZEPHIR_NUM_PREALLOCATED_FRAMES; ++i) {
		start[i].addresses       = pecalloc(24, sizeof(zval*), 1);
		start[i].capacity        = 24;
		start[i].hash_addresses  = pecalloc(8, sizeof(zval*), 1);
		start[i].hash_capacity   = 8;

#ifndef ZEPHIR_RELEASE
		start[i].permanent = 1;
#endif
	}

	start[0].next = &start[1];
	start[ZEPHIR_NUM_PREALLOCATED_FRAMES - 1].prev = &start[ZEPHIR_NUM_PREALLOCATED_FRAMES - 2];

	for (i = 1; i < ZEPHIR_NUM_PREALLOCATED_FRAMES - 1; ++i) {
		start[i].next = &start[i + 1];
		start[i].prev = &start[i - 1];
	}

	zephir_globals_ptr->start_memory = start;
	zephir_globals_ptr->end_memory   = start + ZEPHIR_NUM_PREALLOCATED_FRAMES;

	zephir_globals_ptr->fcache = pemalloc(sizeof(HashTable), 1);
	zend_hash_init(zephir_globals_ptr->fcache, 128, NULL, NULL, 1); // zephir_fcall_cache_dtor

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_null);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_null, 2);

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_false);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_false, 2);
	ZVAL_FALSE(zephir_globals_ptr->global_false);

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_true);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_true, 2);
	ZVAL_TRUE(zephir_globals_ptr->global_true);

	//zephir_globals_ptr->initialized = 1;
}

int zephir_cleanup_fcache(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pDest;
	zend_class_entry *scope;
	uint len = hash_key->nKeyLength;

	assert(hash_key->arKey != NULL);
	assert(hash_key->nKeyLength > 2 * sizeof(zend_class_entry**));

	memcpy(&scope, &hash_key->arKey[len - 2 * sizeof(zend_class_entry**)], sizeof(zend_class_entry*));


#ifndef ZEPHIR_RELEASE
	if ((*entry)->f->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
		return ZEND_HASH_APPLY_REMOVE;
	}
#else
	if ((*entry)->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
		return ZEND_HASH_APPLY_REMOVE;
	}
#endif

#if PHP_VERSION_ID >= 50400
	if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->info.internal.module->type != MODULE_PERSISTENT) {
		return ZEND_HASH_APPLY_REMOVE;
	}
#else
	if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->module->type != MODULE_PERSISTENT) {
		return ZEND_HASH_APPLY_REMOVE;
	}
#endif

	return ZEND_HASH_APPLY_KEEP;
}

void zephir_deinitialize_memory(TSRMLS_D)
{
	size_t i;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	//if (zephir_globals_ptr->initialized != 1) {
	//	zephir_globals_ptr->initialized = 0;
	//	return;
	//}

	if (zephir_globals_ptr->start_memory != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	//zephir_orm_destroy_cache(TSRMLS_C);

	zend_hash_apply_with_arguments(zephir_globals_ptr->fcache TSRMLS_CC, zephir_cleanup_fcache, 0);

#ifndef ZEPHIR_RELEASE
	assert(zephir_globals_ptr->start_memory != NULL);
#endif

	for (i = 0; i < ZEPHIR_NUM_PREALLOCATED_FRAMES; ++i) {
		pefree(zephir_globals_ptr->start_memory[i].hash_addresses, 1);
		pefree(zephir_globals_ptr->start_memory[i].addresses, 1);
	}

	pefree(zephir_globals_ptr->start_memory, 1);
	zephir_globals_ptr->start_memory = NULL;

	zend_hash_destroy(zephir_globals_ptr->fcache);
	pefree(zephir_globals_ptr->fcache, 1);
	zephir_globals_ptr->fcache = NULL;

	for (i = 0; i < 2; i++) {
		zval_ptr_dtor(&zephir_globals_ptr->global_null);
		zval_ptr_dtor(&zephir_globals_ptr->global_false);
		zval_ptr_dtor(&zephir_globals_ptr->global_true);
	}

	//zephir_globals_ptr->initialized = 0;
}

static PHP_MINIT_FUNCTION(fastorm)
{
#if PHP_VERSION_ID < 50500
	char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		size_t len = strlen(old_lc_all);
		char *tmp  = calloc(len+1, 1);
		if (UNEXPECTED(!tmp)) {
			return FAILURE;
		}

		memcpy(tmp, old_lc_all, len);
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif

	ZEPHIR_INIT(Fastorm_Db_IResultDriver);
	ZEPHIR_INIT(Fastorm_IConnectionManager);
	ZEPHIR_INIT(Fastorm_DataObject);
	ZEPHIR_INIT(Fastorm_DbObject);
	ZEPHIR_INIT(Fastorm_Db_Query);
	ZEPHIR_INIT(Fastorm_Exception);
	ZEPHIR_INIT(Fastorm_ConnectionManager);
	ZEPHIR_INIT(Fastorm_Db_ColumnInfo);
	ZEPHIR_INIT(Fastorm_Db_Connection);
	ZEPHIR_INIT(Fastorm_Db_DbException);
	ZEPHIR_INIT(Fastorm_Db_Drivers_MysqliDriver);
	ZEPHIR_INIT(Fastorm_Db_Event);
	ZEPHIR_INIT(Fastorm_Db_Result);
	ZEPHIR_INIT(Fastorm_Db_ResultIterator);
	ZEPHIR_INIT(Fastorm_Db_Translator);
	ZEPHIR_INIT(Fastorm_ModelManager);
	ZEPHIR_INIT(Fastorm_ObjectMetadata);
	ZEPHIR_INIT(Fastorm_ObjectQuery);

#if PHP_VERSION_ID < 50500
	setlocale(LC_ALL, old_lc_all);
	free(old_lc_all);
#endif
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(fastorm)
{

	zephir_deinitialize_memory(TSRMLS_C);

	//assert(ZEPHIR_GLOBAL(orm).parser_cache == NULL);
	//assert(ZEPHIR_GLOBAL(orm).ast_cache == NULL);

	return SUCCESS;
}
#endif

static void php_zephir_init_globals(zend_fastorm_globals *zephir_globals TSRMLS_DC)
{

	/* Memory options */
	zephir_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	zephir_globals->active_symbol_table = NULL;

	/* Recursive Lock */
	zephir_globals->recursive_lock = 0;


}

static PHP_RINIT_FUNCTION(fastorm)
{

	zend_fastorm_globals *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(zephir_globals_ptr TSRMLS_CC);
	//zephir_init_interned_strings(TSRMLS_C);

	zephir_initialize_memory(zephir_globals_ptr TSRMLS_CC);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(fastorm)
{

	zephir_deinitialize_memory(TSRMLS_C);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(fastorm)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_FASTORM_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_FASTORM_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_FASTORM_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_FASTORM_VERSION);
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_FASTORM_ZEPVERSION);
	php_info_print_table_end();


}

static PHP_GINIT_FUNCTION(fastorm)
{
	php_zephir_init_globals(fastorm_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(fastorm)
{

}

zend_module_entry fastorm_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_FASTORM_EXTNAME,
	NULL,
	PHP_MINIT(fastorm),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(fastorm),
#else
	NULL,
#endif
	PHP_RINIT(fastorm),
	PHP_RSHUTDOWN(fastorm),
	PHP_MINFO(fastorm),
	PHP_FASTORM_VERSION,
	ZEND_MODULE_GLOBALS(fastorm),
	PHP_GINIT(fastorm),
	PHP_GSHUTDOWN(fastorm),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_FASTORM
ZEND_GET_MODULE(fastorm)
#endif


