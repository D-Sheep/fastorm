
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
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"

ZEPHIR_INIT_CLASS(Fastorm_ObjectMetadata) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ObjectMetadata, fastorm, objectmetadata, fastorm_objectmetadata_method_entry, 0);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("_metadataCache"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("table"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("storage"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("idField"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("reflection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_objectmetadata_ce, SL("fields"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("PROPERTY"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("KEY"), 2 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_objectmetadata_ce, SL("AUTOINCREMENT"), 4 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_ObjectMetadata, __construct) {

	zval *idField = NULL, *table = NULL, *storage = NULL;
	zval *fields = NULL;
	zval *reflection, *fields_param = NULL, *idField_param = NULL, *table_param = NULL, *storage_param = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 3, &reflection, &fields_param, &idField_param, &table_param, &storage_param);

	zephir_get_arrval(fields, fields_param);
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
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("fields"), _0 TSRMLS_CC);
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

PHP_METHOD(Fastorm_ObjectMetadata, getReflection) {


	RETURN_MEMBER(this_ptr, "reflection");

}

PHP_METHOD(Fastorm_ObjectMetadata, getMetadata) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *className_param = NULL, *_0, *_1 = NULL, *_3;
	zval *className = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &className_param);

	zephir_get_strval(className, className_param);


	_0 = zephir_fetch_static_property_ce(fastorm_objectmetadata_ce, SL("_metadataCache") TSRMLS_CC);
	if (!(zephir_array_isset(_0, className))) {
		ZEPHIR_CALL_SELF(&_1, "createmetadata", &_2, className);
		zephir_check_call_status();
		zephir_update_static_property_ce(fastorm_objectmetadata_ce, SL("_metadataCache"), _1 TSRMLS_CC);
	}
	_3 = zephir_fetch_static_property_ce(fastorm_objectmetadata_ce, SL("_metadataCache") TSRMLS_CC);
	RETURN_CTOR(_3);

}

PHP_METHOD(Fastorm_ObjectMetadata, createMetadata) {

	zend_bool _12, _13;
	HashTable *_7;
	HashPosition _6;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	zend_class_entry *_0;
	zval *propertiesArray;
	int prop, ZEPHIR_LAST_CALL_STATUS;
	zval *className_param = NULL, *reflection, *properties = NULL, *docs = NULL, *matches, *property = NULL, *propName = NULL, *idField = NULL, *table = NULL, *storage = NULL, *_1 = NULL, *_2 = NULL, *_4 = NULL, *_5 = NULL, **_8, *_9 = NULL, _10 = zval_used_for_init, *_11 = NULL, *_14 = NULL;
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
	ZVAL_STRING(_1, "/@id ([a-z_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_2, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_2)) {
		ZEPHIR_OBS_VAR(idField);
		zephir_array_fetch_long(&idField, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 78 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_NVAR(idField);
		ZVAL_NULL(idField);
	}
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "/@table ([a-z_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_4)) {
		ZEPHIR_OBS_VAR(table);
		zephir_array_fetch_long(&table, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 84 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_NVAR(table);
		ZVAL_NULL(table);
	}
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "/@storage ([a-z_]+)/i", 0);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(&_5, "preg_match", &_3, _1, docs, matches);
	zephir_check_temp_parameter(_1);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	if (zephir_is_true(_5)) {
		ZEPHIR_OBS_VAR(storage);
		zephir_array_fetch_long(&storage, matches, 1, PH_NOISY, "fastorm/ObjectMetadata.zep", 90 TSRMLS_CC);
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
	zephir_is_iterable(properties, &_7, &_6, 0, 0, "fastorm/ObjectMetadata.zep", 119);
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
			ZVAL_LONG(_1, prop);
			zephir_array_update_zval(&propertiesArray, propName, &_1, PH_COPY | PH_SEPARATE);
		}
	}
	object_init_ex(return_value, fastorm_objectmetadata_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, reflection, propertiesArray, idField, table, storage);
	zephir_check_call_status();
	RETURN_MM();

}

