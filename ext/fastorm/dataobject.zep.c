
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/exception.h"


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

PHP_METHOD(Fastorm_DataObject, __construct) {

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

PHP_METHOD(Fastorm_DataObject, getId) {

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

PHP_METHOD(Fastorm_DataObject, setId) {

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

PHP_METHOD(Fastorm_DataObject, setData) {

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

PHP_METHOD(Fastorm_DataObject, getData) {

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

PHP_METHOD(Fastorm_DataObject, offsetExists) {

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

PHP_METHOD(Fastorm_DataObject, offsetGet) {

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

PHP_METHOD(Fastorm_DataObject, offsetSet) {

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

PHP_METHOD(Fastorm_DataObject, offsetUnset) {

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

PHP_METHOD(Fastorm_DataObject, _initialize) {

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

PHP_METHOD(Fastorm_DataObject, initialize) {

	zval *metadata;

	zephir_fetch_params(0, 1, 0, &metadata);



	if (!(zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}

}

PHP_METHOD(Fastorm_DataObject, _processInitialization) {

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

PHP_METHOD(Fastorm_DataObject, getMetadata) {

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

PHP_METHOD(Fastorm_DataObject, serialize) {

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

PHP_METHOD(Fastorm_DataObject, unserialize) {

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

PHP_METHOD(Fastorm_DataObject, getDataForSerialization) {

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

PHP_METHOD(Fastorm_DataObject, getDbFormatedData) {

	HashTable *_2;
	HashPosition _1;
	zval *data;
	zval *withoutAutoincrementKeys_param = NULL, *propName = NULL, *propFlag = NULL, *className, *_0, **_3, *_5 = NULL;
	zend_bool withoutAutoincrementKeys, _4;

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
	zephir_is_iterable(_0, &_2, &_1, 0, 0, "fastorm/DataObject.zep", 194);
	for (
	  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
	  ; zephir_hash_move_forward_ex(_2, &_1)
	) {
		ZEPHIR_GET_HMKEY(propName, _2, _1);
		ZEPHIR_GET_HVALUE(propFlag, _3);
		_4 = !withoutAutoincrementKeys;
		if (!(_4)) {
			_4 = !(((int) (zephir_get_numberval(propFlag)) & 4));
		}
		if (_4) {
			ZEPHIR_OBS_NVAR(_5);
			zephir_read_property_zval(&_5, this_ptr, propName, PH_NOISY_CC);
			zephir_array_update_zval(&data, propName, &_5, PH_COPY | PH_SEPARATE);
		}
	}
	RETURN_CTOR(data);

}

