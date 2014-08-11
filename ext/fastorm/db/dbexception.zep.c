
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_DbException) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm\\Db, DbException, fastorm, db_dbexception, fastorm_exception_ce, fastorm_db_dbexception_method_entry, 0);

	zend_declare_property_null(fastorm_db_dbexception_ce, SL("sql"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_DbException, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *exception, *code = NULL, *previous = NULL, *sql = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 3, &exception, &code, &previous, &sql);

	if (!code) {
		code = ZEPHIR_GLOBAL(global_null);
	}
	if (!previous) {
		previous = ZEPHIR_GLOBAL(global_null);
	}
	if (!sql) {
		sql = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_PARENT(NULL, fastorm_db_dbexception_ce, this_ptr, "__construct", NULL, exception, code, previous);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("sql"), sql TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_DbException, getSql) {


	RETURN_MEMBER(this_ptr, "sql");

}

