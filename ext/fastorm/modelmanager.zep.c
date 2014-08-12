
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
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Fastorm_ModelManager) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ModelManager, fastorm, modelmanager, fastorm_modelmanager_method_entry, 0);

	zend_declare_property_null(fastorm_modelmanager_ce, SL("_sharedInstance"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_modelmanager_ce, SL("connectionManager"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_ModelManager, getShared) {

	zval *_0, *_1;


	_0 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "Model Manager not initialized", "fastorm/ModelManager.zep", 14);
		return;
	}
	_1 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	RETURN_CTORW(_1);

}

PHP_METHOD(Fastorm_ModelManager, __construct) {

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

PHP_METHOD(Fastorm_ModelManager, getConnection) {

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

