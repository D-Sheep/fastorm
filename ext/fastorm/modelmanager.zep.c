
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


ZEPHIR_INIT_CLASS(Fastorm_ModelManager) {

	ZEPHIR_REGISTER_CLASS(Fastorm, ModelManager, fastorm, modelmanager, fastorm_modelmanager_method_entry, 0);

	zend_declare_property_null(fastorm_modelmanager_ce, SL("_sharedInstance"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_ModelManager, getShared) {

	zval *_0, *_1;


	_0 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(fastorm_exception_ce, "M", "fastorm/ModelManager.zep", 12);
		return;
	}
	_1 = zephir_fetch_static_property_ce(fastorm_modelmanager_ce, SL("_sharedInstance") TSRMLS_CC);
	RETURN_CTORW(_1);

}

