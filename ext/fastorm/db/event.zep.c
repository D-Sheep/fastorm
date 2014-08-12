
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
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_Event) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Event, fastorm, db_event, fastorm_db_event_method_entry, 0);

	/** @var DibiConnection */
	zend_declare_property_null(fastorm_db_event_ce, SL("connection"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_event_ce, SL("type"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var string */
	zend_declare_property_null(fastorm_db_event_ce, SL("sql"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var DibiResult|DibiDriverException|null */
	zend_declare_property_null(fastorm_db_event_ce, SL("result"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var float */
	zend_declare_property_null(fastorm_db_event_ce, SL("time"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_event_ce, SL("count"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("types"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("elapsedTime"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("numOfQueries"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("totalTime"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_event_ce, SL("lastSql"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	/** event type */
	zend_declare_class_constant_long(fastorm_db_event_ce, SL("CONNECT"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("SELECT"), 4 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("INSERT"), 8 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("DELETE"), 16 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("UPDATE"), 32 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("QUERY"), 60 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("BEGIN"), 64 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("COMMIT"), 128 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("ROLLBACK"), 256 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("TRANSACTION"), 448 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_event_ce, SL("ALL"), 1023 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_Event, __construct) {

	zend_bool _6;
	zval *_4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_9 = NULL;
	zval *connection, *type, *sqlCommand = NULL, *matches, *_0, *_1 = NULL, *_3, *_5, *_7, *_8 = NULL, *_10, *_11, *_12;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &connection, &type, &sqlCommand);

	if (!sqlCommand) {
		sqlCommand = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("type"), type TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_fast_trim(_0, sqlCommand, NULL , ZEPHIR_TRIM_BOTH TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("sql"), _0 TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "microtime", &_2, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	zephir_negate(_1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("time"), _1 TSRMLS_CC);
	_3 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("types") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_3)) {
		ZEPHIR_INIT_VAR(_4);
		array_init_size(_4, 6);
		add_assoc_long_ex(_4, SS("SELECT"), 4);
		add_assoc_long_ex(_4, SS("UPDATE"), 32);
		add_assoc_long_ex(_4, SS("INSERT"), 8);
		add_assoc_long_ex(_4, SS("DELETE"), 16);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("types"), _4 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), _5 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("totalTime"), _5 TSRMLS_CC);
		ZEPHIR_INIT_ZVAL_NREF(_5);
		ZVAL_LONG(_5, 0);
		zephir_update_static_property_ce(fastorm_db_event_ce, SL("numOfQueries"), _5 TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(matches);
	ZVAL_NULL(matches);
	_6 = ZEPHIR_IS_LONG_IDENTICAL(type, 60);
	if (_6) {
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("sql"), PH_NOISY_CC);
		ZEPHIR_INIT_VAR(_7);
		ZVAL_STRING(_7, "#\\(?\\s*(SELECT|UPDATE|INSERT|DELETE)#iA", 0);
		Z_SET_ISREF_P(matches);
		ZEPHIR_CALL_FUNCTION(&_8, "preg_match", &_9, _7, _5, matches);
		zephir_check_temp_parameter(_7);
		Z_UNSET_ISREF_P(matches);
		zephir_check_call_status();
		_6 = zephir_is_true(_8);
	}
	if (_6) {
		_10 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("types") TSRMLS_CC);
		ZEPHIR_INIT_BNVAR(_7);
		zephir_array_fetch_long(&_12, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Event.zep", 69 TSRMLS_CC);
		zephir_fast_strtoupper(_7, _12);
		zephir_array_fetch(&_11, _10, _7, PH_NOISY | PH_READONLY, "fastorm/db/Event.zep", 69 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("type"), _11 TSRMLS_CC);
	}
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	_5 = zephir_fetch_static_property_ce(fastorm_db_event_ce, SL("numOfQueries") TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_10);
	ZVAL_LONG(_10, (zephir_get_numberval(_5) + 1));
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("numOfQueries"), _10 TSRMLS_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("lastSql"), sqlCommand TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_Event, done) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval *result = NULL, *e = NULL, *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &result);

	if (!result) {
		result = ZEPHIR_GLOBAL(global_null);
	}


	zephir_update_property_this(this_ptr, SL("result"), result TSRMLS_CC);
	if (Z_TYPE_P(result) != IS_NULL) {

		/* try_start_1: */

			if (zephir_instance_of_ev(result, fastorm_db_result_ce TSRMLS_CC)) {
				ZEPHIR_INIT_ZVAL_NREF(_0);
				ZVAL_LONG(_0, zephir_fast_count_int(result TSRMLS_CC));
				zephir_update_property_this(this_ptr, SL("count"), _0 TSRMLS_CC);
			} else {
				zephir_update_property_this(this_ptr, SL("count"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			}

		try_end_1:

		if (EG(exception)) {
			ZEPHIR_CPY_WRT(e, EG(exception));
			if (zephir_is_instance_of(e, SL("Fastorm\\Db\\DbException") TSRMLS_CC)) {
				zend_clear_exception(TSRMLS_C);
				zephir_update_property_this(this_ptr, SL("count"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			}
		}
	}
	ZEPHIR_CALL_FUNCTION(&_1, "microtime", &_2, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("time"), _1 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("time"), PH_NOISY_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("elapsedTime"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("time"), PH_NOISY_CC);
	zephir_update_static_property_ce(fastorm_db_event_ce, SL("totalTime"), _0 TSRMLS_CC);
	RETURN_THIS();

}

