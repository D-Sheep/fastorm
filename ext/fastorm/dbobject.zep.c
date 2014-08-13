
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

	zend_declare_property_null(fastorm_dbobject_ce, SL("_affectedRows"), ZEND_ACC_PRIVATE TSRMLS_CC);

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

PHP_METHOD(Fastorm_DbObject, onBeforeCreate) {

	zval *calledFromSave = NULL;

	zephir_fetch_params(0, 0, 1, &calledFromSave);

	if (!calledFromSave) {
		calledFromSave = ZEPHIR_GLOBAL(global_false);
	}



}

PHP_METHOD(Fastorm_DbObject, onBeforeUpdate) {

	zval *remove = NULL;

	zephir_fetch_params(0, 0, 1, &remove);

	if (!remove) {
		remove = ZEPHIR_GLOBAL(global_false);
	}



}

PHP_METHOD(Fastorm_DbObject, onAfterUpdate) {

	zval *removed = NULL;

	zephir_fetch_params(0, 0, 1, &removed);

	if (!removed) {
		removed = ZEPHIR_GLOBAL(global_false);
	}



}

PHP_METHOD(Fastorm_DbObject, create) {

	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *insertIgnore_param = NULL, *db = NULL, *metadata = NULL, *autoincrement = NULL, *query = NULL, *_1 = NULL, *_2 = NULL, *_3, *_4 = NULL, *_5, *_7 = NULL;
	zend_bool insertIgnore, _6;

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
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", &_0, metadata);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, metadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "getdbformateddata", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&query, db, "insert", NULL, _1, _2);
	zephir_check_call_status();
	if (insertIgnore) {
		ZEPHIR_INIT_VAR(_3);
		ZVAL_STRING(_3, "IGNORE", 0);
		ZEPHIR_CALL_METHOD(NULL, query, "setflag", NULL, _3);
		zephir_check_temp_parameter(_3);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_METHOD(&_4, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _4 TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&autoincrement, metadata, "getautoincrementkey",  NULL);
	zephir_check_call_status();
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	_6 = zephir_is_true(_5);
	if (_6) {
		_6 = Z_TYPE_P(autoincrement) != IS_NULL;
	}
	if (_6) {
		ZEPHIR_CALL_METHOD(&_7, db, "getinsertid",  NULL);
		zephir_check_call_status();
		zephir_update_property_zval_zval(this_ptr, autoincrement, _7 TSRMLS_CC);
	}
	RETURN_THIS();

}

PHP_METHOD(Fastorm_DbObject, save) {

	zend_bool _10;
	HashTable *_3;
	HashPosition _2;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *db = NULL, *metadata = NULL, *autoincrement = NULL, *data, *lastInsert = NULL, *query = NULL, *propName = NULL, *propFlag = NULL, *e = NULL, *_1 = NULL, **_4, *_5 = NULL, *_6 = NULL, *_7, *_8 = NULL, *_9, *_11 = NULL, *_12;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforecreate", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", &_0, metadata);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_INIT_VAR(lastInsert);
	ZVAL_STRING(lastInsert, "", 1);
	ZEPHIR_INIT_VAR(autoincrement);
	ZVAL_NULL(autoincrement);
	ZEPHIR_CALL_METHOD(&_1, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/DbObject.zep", 167);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(propName, _3, _2);
		ZEPHIR_GET_HVALUE(propFlag, _4);
		if (!(((int) (zephir_get_numberval(propFlag)) & 4))) {
			ZEPHIR_OBS_NVAR(_5);
			zephir_read_property_zval(&_5, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&data, propName, &_5, PH_COPY | PH_SEPARATE);
		} else {
			ZEPHIR_INIT_NVAR(lastInsert);
			ZEPHIR_CONCAT_SVSVS(lastInsert, "`", propName, "` = LAST_INSERT_ID(`", propName, "`)");
			ZEPHIR_CPY_WRT(autoincrement, propName);
		}
	}
	ZEPHIR_CALL_METHOD(&_6, metadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_7);
	ZVAL_STRING(_7, "INSERT INTO %n %v ON DUPLICATE KEY UPDATE %sql %a", 0);
	ZEPHIR_CALL_METHOD(&query, db, "query", NULL, _7, _6, data, lastInsert, data);
	zephir_check_temp_parameter(_7);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_8, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _8 TSRMLS_CC);
	_9 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	_10 = zephir_is_true(_9);
	if (_10) {
		_10 = Z_TYPE_P(autoincrement) != IS_NULL;
	}
	if (_10) {

		/* try_start_1: */

			ZEPHIR_CALL_METHOD(&_11, db, "getinsertid",  NULL);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_update_property_zval_zval(this_ptr, autoincrement, _11 TSRMLS_CC);

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
	_12 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_12)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

PHP_METHOD(Fastorm_DbObject, update) {

	zval *_9;
	zend_bool _6;
	HashTable *_3;
	HashPosition _2;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *onlyColumnsArray = NULL, *db = NULL, *metadata = NULL, autoincrement, *data, *query = NULL, *propName = NULL, *propFlag = NULL, *keys, *_1 = NULL, **_4, *_5 = NULL, *_7, *_8 = NULL, *_10 = NULL, *_11 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &onlyColumnsArray);

	if (!onlyColumnsArray) {
		onlyColumnsArray = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", &_0, metadata);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	ZEPHIR_SINIT_VAR(autoincrement);
	ZVAL_NULL(&autoincrement);
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_CALL_METHOD(&_1, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/DbObject.zep", 211);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(propName, _3, _2);
		ZEPHIR_GET_HVALUE(propFlag, _4);
		if (((int) (zephir_get_numberval(propFlag)) & 2)) {
			ZEPHIR_OBS_NVAR(_5);
			zephir_read_property_zval(&_5, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&keys, propName, &_5, PH_COPY | PH_SEPARATE);
		} else {
			_6 = Z_TYPE_P(onlyColumnsArray) == IS_NULL;
			if (!(_6)) {
				_6 = zephir_fast_in_array(propName, onlyColumnsArray TSRMLS_CC);
			}
			if (_6) {
				ZEPHIR_OBS_NVAR(_5);
				zephir_read_property_zval(&_5, this_ptr, propName, PH_NOISY_CC);
				zephir_array_update_zval(&data, propName, &_5, PH_COPY | PH_SEPARATE);
			}
		}
	}
	if (zephir_fast_count_int(data TSRMLS_CC) == 0) {
		ZEPHIR_INIT_ZVAL_NREF(_7);
		ZVAL_LONG(_7, 0);
		zephir_update_property_this(this_ptr, SL("_affectedRows"), _7 TSRMLS_CC);
	} else {
		ZEPHIR_CALL_METHOD(&_8, metadata, "gettable",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&query, db, "update", NULL, _8, data);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_9);
		array_init_size(_9, 3);
		ZEPHIR_INIT_VAR(_10);
		ZVAL_STRING(_10, "%and", 1);
		zephir_array_fast_append(_9, _10);
		zephir_array_fast_append(_9, keys);
		ZEPHIR_INIT_NVAR(_10);
		ZVAL_STRING(_10, "where", 0);
		ZEPHIR_CALL_METHOD(NULL, query, "__call", NULL, _10, _9);
		zephir_check_temp_parameter(_10);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_11, query, "execute",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("_affectedRows"), _11 TSRMLS_CC);
	}
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_7)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

PHP_METHOD(Fastorm_DbObject, delete) {

	zval *_7;
	HashTable *_3;
	HashPosition _2;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *db = NULL, *metadata = NULL, autoincrement, *query = NULL, *propName = NULL, *propFlag = NULL, *keys, *_1 = NULL, **_4, *_5 = NULL, *_6 = NULL, *_8, *_9 = NULL, *_10;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "onbeforeupdate", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&metadata, "getmetadata", NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_SELF(&db, "getdbcontextwithmetadata", &_0, metadata);
	zephir_check_call_status();
	ZEPHIR_SINIT_VAR(autoincrement);
	ZVAL_NULL(&autoincrement);
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_CALL_METHOD(&_1, metadata, "getfields",  NULL);
	zephir_check_call_status();
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/DbObject.zep", 244);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(propName, _3, _2);
		ZEPHIR_GET_HVALUE(propFlag, _4);
		if (((int) (zephir_get_numberval(propFlag)) & 2)) {
			ZEPHIR_OBS_NVAR(_5);
			zephir_read_property_zval(&_5, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&keys, propName, &_5, PH_COPY | PH_SEPARATE);
		}
	}
	ZEPHIR_CALL_METHOD(&_6, metadata, "gettable",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&query, db, "delete", NULL, _6);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_7);
	array_init_size(_7, 3);
	ZEPHIR_INIT_VAR(_8);
	ZVAL_STRING(_8, "%and", 1);
	zephir_array_fast_append(_7, _8);
	zephir_array_fast_append(_7, keys);
	ZEPHIR_INIT_BNVAR(_8);
	ZVAL_STRING(_8, "where", 0);
	ZEPHIR_CALL_METHOD(NULL, query, "__call", NULL, _8, _7);
	zephir_check_temp_parameter(_8);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_9, query, "execute",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("_affectedRows"), _9 TSRMLS_CC);
	_10 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (zephir_is_true(_10)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "onafterupdate", NULL);
		zephir_check_call_status();
	}
	RETURN_THIS();

}

PHP_METHOD(Fastorm_DbObject, hasMany) {



}

PHP_METHOD(Fastorm_DbObject, getAffectedRows) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_affectedRows"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "You can use getAffectedRows() method only after insert, update or remove", "fastorm/DbObject.zep", 262);
		return;
	} else {
		RETURN_MEMBER(this_ptr, "_affectedRows");
	}

}

