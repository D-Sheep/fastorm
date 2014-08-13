
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
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/string.h"
#include "kernel/concat.h"


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

PHP_METHOD(Fastorm_ObjectMetadata, __construct) {

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

PHP_METHOD(Fastorm_ObjectMetadata, getTable) {


	RETURN_MEMBER(this_ptr, "table");

}

PHP_METHOD(Fastorm_ObjectMetadata, getStorage) {


	RETURN_MEMBER(this_ptr, "storage");

}

PHP_METHOD(Fastorm_ObjectMetadata, getIdField) {


	RETURN_MEMBER(this_ptr, "idField");

}

PHP_METHOD(Fastorm_ObjectMetadata, getFields) {


	RETURN_MEMBER(this_ptr, "fields");

}

PHP_METHOD(Fastorm_ObjectMetadata, getClassName) {


	RETURN_MEMBER(this_ptr, "className");

}

PHP_METHOD(Fastorm_ObjectMetadata, newInstance) {

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

PHP_METHOD(Fastorm_ObjectMetadata, getReflection) {


	RETURN_MEMBER(this_ptr, "reflection");

}

PHP_METHOD(Fastorm_ObjectMetadata, getMetadata) {

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

PHP_METHOD(Fastorm_ObjectMetadata, createMetadata) {

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

PHP_METHOD(Fastorm_ObjectMetadata, getJoin) {

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

PHP_METHOD(Fastorm_ObjectMetadata, hasJoin) {

	zval *propName_param = NULL, *_0;
	zval *propName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &propName_param);

	zephir_get_strval(propName, propName_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("joins"), PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_array_isset(_0, propName));

}

PHP_METHOD(Fastorm_ObjectMetadata, getJoins) {


	RETURN_MEMBER(this_ptr, "joins");

}

PHP_METHOD(Fastorm_ObjectMetadata, getAliases) {

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

PHP_METHOD(Fastorm_ObjectMetadata, toPropertyName) {

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

PHP_METHOD(Fastorm_ObjectMetadata, makeAlias) {

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

PHP_METHOD(Fastorm_ObjectMetadata, getAutoincrementKey) {

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

