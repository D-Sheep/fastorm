
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/array.h"
#include "kernel/hash.h"


ZEPHIR_INIT_CLASS(Fastorm_DbObject) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm, DbObject, fastorm, dbobject, fastorm_dataobject_ce, fastorm_dbobject_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(fastorm_dbobject_ce, SL("_joinCache"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_DbObject, getDbContext) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_SELF(&_1, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_SELF("getdbcontextwithmetadata", &_0, _1);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_DbObject, getSelect) {

	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *metadata = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	object_init_ex(return_value, fastorm_objectquery_ce);
	ZEPHIR_CALL_SELF(&_0, "getdbcontextwithmetadata", &_1, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, _0, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_DbObject, getDbContextWithMetadata) {

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

/**
 *	@metadata only for powerup (private)
 */
PHP_METHOD(Fastorm_DbObject, getById) {

	zval *_3 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool joinAll, _0;
	zval *id, *joinAll_param = NULL, *metadata = NULL, *select, *_1 = NULL, *_4 = NULL, *_5 = NULL, *_6 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 2, &id, &joinAll_param, &metadata);

	if (!joinAll_param) {
		joinAll = 1;
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
	ZEPHIR_CALL_SELF(&_1, "getdbcontextwithmetadata", &_2, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, select, "__construct", NULL, _1, metadata);
	zephir_check_call_status();
	if (Z_TYPE_P(id) == IS_STRING) {
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_CALL_METHOD(&_4, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_5, metadata, "getidfield",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_6);
		ZEPHIR_CONCAT_VSVS(_6, _4, ".", _5, " = %s");
		zephir_array_fast_append(_3, _6);
		zephir_array_fast_append(_3, id);
		ZEPHIR_INIT_VAR(_7);
		ZVAL_STRING(_7, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, select, "__call", NULL, _7, _3);
		zephir_check_temp_parameter(_7);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_CALL_METHOD(&_4, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_5, metadata, "getidfield",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_6);
		ZEPHIR_CONCAT_VSVS(_6, _4, ".", _5, " = %i");
		zephir_array_fast_append(_3, _6);
		zephir_array_fast_append(_3, id);
		ZEPHIR_INIT_NVAR(_7);
		ZVAL_STRING(_7, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, select, "__call", NULL, _7, _3);
		zephir_check_temp_parameter(_7);
		zephir_check_call_status();
	}
	ZEPHIR_RETURN_CALL_METHOD(select, "fetchfirst", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_DbObject, __call) {

	zend_class_entry *_21;
	HashTable *_15;
	HashPosition _14;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_10 = NULL, *_12 = NULL;
	zend_bool isSetter, _3, _5, _6;
	zval *method, *args, *m, *propertyName = NULL, *metadata = NULL, *joinedMetadata = NULL, *_0, *_1 = NULL, *_4 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL, *_11, *ret = NULL, *alias = NULL, *_13, *key = NULL, *value = NULL, **_16, *_17, *_18, *_19, *className = NULL, *_20 = NULL, *_22, *idField = NULL, *_23, *_24;

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
			zephir_array_fetch_long(&_7, args, 0, PH_NOISY, "fastorm/DbObject.zep", 49 TSRMLS_CC);
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
			zephir_throw_exception_debug(_8, "fastorm/DbObject.zep", 52 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	zephir_array_fetch_long(&_11, m, 1, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 56 TSRMLS_CC);
	ZEPHIR_CALL_CE_STATIC(&propertyName, fastorm_objectmetadata_ce, "topropertyname", &_10, _11);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&joinedMetadata, metadata, "getjoin", NULL, propertyName);
	zephir_check_call_status();
	if (Z_TYPE_P(joinedMetadata) == IS_NULL) {
		ZEPHIR_INIT_LNVAR(_8);
		object_init_ex(_8, fastorm_exception_ce);
		ZEPHIR_INIT_LNVAR(_9);
		ZEPHIR_CONCAT_SVS(_9, "Method '", method, "' not implemented or target object not defined");
		ZEPHIR_CALL_METHOD(NULL, _8, "__construct", NULL, _9);
		zephir_check_call_status();
		zephir_throw_exception_debug(_8, "fastorm/DbObject.zep", 60 TSRMLS_CC);
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
			ZEPHIR_CALL_CE_STATIC(&alias, fastorm_objectmetadata_ce, "makealias", &_12, propertyName, _4);
			zephir_check_call_status();
			_13 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
			if (zephir_array_isset(_13, alias)) {
				ZEPHIR_CALL_METHOD(&ret, joinedMetadata, "newinstance",  NULL);
				zephir_check_call_status();
				zephir_is_iterable(joinedMetadata, &_15, &_14, 0, 0, "fastorm/DbObject.zep", 78);
				for (
				  ; zephir_hash_get_current_data_ex(_15, (void**) &_16, &_14) == SUCCESS
				  ; zephir_hash_move_forward_ex(_15, &_14)
				) {
					ZEPHIR_GET_HMKEY(key, _15, _14);
					ZEPHIR_GET_HVALUE(value, _16);
					ZEPHIR_CALL_CE_STATIC(&alias, fastorm_objectmetadata_ce, "makealias", &_12, propertyName, key);
					zephir_check_call_status();
					_17 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
					if (zephir_array_isset(_17, alias)) {
						_18 = zephir_fetch_nproperty_this(this_ptr, SL("_data"), PH_NOISY_CC);
						zephir_array_fetch(&_19, _18, alias, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 75 TSRMLS_CC);
						zephir_update_property_zval_zval(ret, key, _19 TSRMLS_CC);
					}
				}
			} else {
				ZEPHIR_CALL_METHOD(&className, joinedMetadata, "getclassname",  NULL);
				zephir_check_call_status();
				ZEPHIR_OBS_VAR(_20);
				zephir_read_property_zval(&_20, this_ptr, propertyName, PH_NOISY_CC);
				_21 = zend_fetch_class(Z_STRVAL_P(className), Z_STRLEN_P(className), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				ZEPHIR_CALL_CE_STATIC(&ret, _21, "getbyid", NULL, _20, ZEPHIR_GLOBAL(global_false), joinedMetadata);
				zephir_check_call_status();
			}
			_5 = zephir_array_isset_long(args, 0);
			if (_5) {
				zephir_array_fetch_long(&_19, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 83 TSRMLS_CC);
				_5 = ZEPHIR_IS_TRUE_IDENTICAL(_19);
			}
			if (_5) {
				zephir_update_property_array(this_ptr, SL("_joinCache"), propertyName, ret TSRMLS_CC);
			}
			RETURN_CCTOR(ret);
		}
	} else {
		ZEPHIR_OBS_NVAR(_7);
		zephir_array_fetch_long(&_7, args, 0, PH_NOISY, "fastorm/DbObject.zep", 89 TSRMLS_CC);
		_5 = zephir_instance_of_ev(_7, fastorm_dataobject_ce TSRMLS_CC);
		if (!(_5)) {
			zephir_array_fetch_long(&_22, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 89 TSRMLS_CC);
			_5 = Z_TYPE_P(_22) == IS_NULL;
		}
		if (_5) {
			ZEPHIR_CALL_METHOD(&idField, joinedMetadata, "getidfield",  NULL);
			zephir_check_call_status();
			ZEPHIR_OBS_NVAR(_20);
			zephir_array_fetch_long(&_20, args, 0, PH_NOISY, "fastorm/DbObject.zep", 92 TSRMLS_CC);
			ZEPHIR_OBS_VAR(_23);
			zephir_read_property_zval(&_23, _20, idField, PH_NOISY_CC);
			zephir_update_property_zval_zval(this_ptr, propertyName, _23 TSRMLS_CC);
			_13 = zephir_fetch_nproperty_this(this_ptr, SL("_joinCache"), PH_NOISY_CC);
			if (Z_TYPE_P(_13) == IS_NULL) {
				ZEPHIR_INIT_BNVAR(_0);
				array_init(_0);
				zephir_update_property_this(this_ptr, SL("_joinCache"), _0 TSRMLS_CC);
			}
			zephir_array_fetch_long(&_24, args, 0, PH_NOISY | PH_READONLY, "fastorm/DbObject.zep", 96 TSRMLS_CC);
			zephir_update_property_array(this_ptr, SL("_joinCache"), propertyName, _24 TSRMLS_CC);
		} else {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_exception_ce, "Object in setter must be Dataobject type", "fastorm/DbObject.zep", 98);
			return;
		}
		RETURN_THIS();
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_DbObject, create) {

	zval *insertIgnore_param = NULL;
	zend_bool insertIgnore;

	zephir_fetch_params(0, 0, 1, &insertIgnore_param);

	if (!insertIgnore_param) {
		insertIgnore = 0;
	} else {
		insertIgnore = zephir_get_boolval(insertIgnore_param);
	}



}

PHP_METHOD(Fastorm_DbObject, save) {



}

PHP_METHOD(Fastorm_DbObject, update) {

	zval *onlyColumns_param = NULL;
	zval *onlyColumns = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &onlyColumns_param);

	if (!onlyColumns_param) {
	ZEPHIR_INIT_VAR(onlyColumns);
	ZVAL_NULL(onlyColumns);
	} else {
		zephir_get_arrval(onlyColumns, onlyColumns_param);
	}



}

PHP_METHOD(Fastorm_DbObject, hasMany) {



}

