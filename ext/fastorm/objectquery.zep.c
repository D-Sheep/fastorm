
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Fastorm_ObjectQuery) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm, ObjectQuery, fastorm, objectquery, fastorm_db_query_ce, fastorm_objectquery_method_entry, 0);

	zend_declare_property_null(fastorm_objectquery_ce, SL("metadata"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_ObjectQuery, __construct) {

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

PHP_METHOD(Fastorm_ObjectQuery, initiateSelect) {

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

PHP_METHOD(Fastorm_ObjectQuery, getMetadata) {


	RETURN_MEMBER(this_ptr, "metadata");

}

PHP_METHOD(Fastorm_ObjectQuery, joinObject) {

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

PHP_METHOD(Fastorm_ObjectQuery, joinAll) {

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

PHP_METHOD(Fastorm_ObjectQuery, joinObjectWith) {

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

PHP_METHOD(Fastorm_ObjectQuery, query) {

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
	ZEPHIR_CALL_METHOD(&res, _0, "query", NULL, args, _1);
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

