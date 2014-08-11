
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
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_ResultIterator) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ResultIterator, fastorm, db_resultiterator, fastorm_db_resultiterator_method_entry, 0);

	/** @var DibiResult */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("result"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("row"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_resultiterator_ce, SL("pointer"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_class_implements(fastorm_db_resultiterator_ce TSRMLS_CC, 1, zend_ce_iterator);
	return SUCCESS;

}

/**
 * @param  DibiResult
 */
PHP_METHOD(Fastorm_Db_ResultIterator, __construct) {

	zval *result;

	zephir_fetch_params(0, 1, 0, &result);



	if (!(zephir_instance_of_ev(result, fastorm_db_result_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'result' must be an instance of 'Fastorm\\Db\\Result'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("result"), result TSRMLS_CC);

}

/**
 * Rewinds the iterator to the first element.
 * @return void
 */
PHP_METHOD(Fastorm_Db_ResultIterator, rewind) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("pointer"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 0);
	ZEPHIR_CALL_METHOD(NULL, _0, "seek", NULL, _1);
	zephir_check_call_status();
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_3, _2, "fetchrow",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("row"), _3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the key of the current element.
 * @return mixed
 */
PHP_METHOD(Fastorm_Db_ResultIterator, key) {


	RETURN_MEMBER(this_ptr, "pointer");

}

/**
 * Returns the current element.
 * @return mixed
 */
PHP_METHOD(Fastorm_Db_ResultIterator, current) {


	RETURN_MEMBER(this_ptr, "row");

}

/**
 * Moves forward to next element.
 * @return void
 */
PHP_METHOD(Fastorm_Db_ResultIterator, next) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, _0, "fetchrow",  NULL);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("row"), _1 TSRMLS_CC);
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("pointer") TSRMLS_CC));
	ZEPHIR_MM_RESTORE();

}

/**
 * Checks if there is a current element after calls to rewind() or next().
 * @return bool
 */
PHP_METHOD(Fastorm_Db_ResultIterator, valid) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property_this(&_0, this_ptr, SL("row"), PH_NOISY_CC);
	RETURN_MM_BOOL(!ZEPHIR_IS_EMPTY(_0));

}

/**
 * Required by the Countable interface.
 * @return int
 */
PHP_METHOD(Fastorm_Db_ResultIterator, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("result"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

