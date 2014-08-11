
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
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Src_TestClass) {

	ZEPHIR_REGISTER_CLASS(Src, TestClass, src, testclass, src_testclass_method_entry, 0);

	zend_declare_property_null(src_testclass_ce, SL("_test"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Src_TestClass, __construct) {

	zval *some;

	zephir_fetch_params(0, 1, 0, &some);



	zephir_update_property_this(this_ptr, SL("_test"), some TSRMLS_CC);

}

PHP_METHOD(Src_TestClass, foo) {


	RETURN_MEMBER(this_ptr, "_test");

}

