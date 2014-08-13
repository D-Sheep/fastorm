
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
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Fastorm_ConnectionManager) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ConnectionManager, fastorm, connectionmanager, fastorm_connectionmanager_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(fastorm_connectionmanager_ce, SL("connections"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_connectionmanager_ce, SL("defaultConnectionName"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(fastorm_connectionmanager_ce TSRMLS_CC, 1, fastorm_iconnectionmanager_ce);
	return SUCCESS;

}

PHP_METHOD(Fastorm_ConnectionManager, __construct) {

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

PHP_METHOD(Fastorm_ConnectionManager, getConnection) {

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

PHP_METHOD(Fastorm_ConnectionManager, createConnection) {

}

