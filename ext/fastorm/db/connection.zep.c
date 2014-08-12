
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
#include "kernel/fcall.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/hash.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/concat.h"

ZEPHIR_INIT_CLASS(Fastorm_Db_Connection) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Connection, fastorm, db_connection, fastorm_db_connection_method_entry, 0);

	zend_declare_property_null(fastorm_db_connection_ce, SL("onEvent"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("config"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("driver"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("translator"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_connection_ce, SL("connected"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

/**
 * Connection options: (see driver-specific options too)
 *   - lazy (bool) => if TRUE, connection will be established only when required
 *   - result (array) => result set options
 *       - formatDateTime => date-time format (if empty, DateTime objects will be returned)
 *   - profiler (array or bool)
 *       - run (bool) => enable profiler?
 *       - file => file to log
 *   - substitutes (array) => map of driver specific substitutes (under development)

 * @param  mixed   connection parameters
 * @param  string  connection name
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, __construct) {

	zend_class_entry *_14;
	zephir_fcall_cache_entry *_5 = NULL;
	HashTable *_2;
	HashPosition _1;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_10 = NULL;
	zval *driver = NULL, *driverClass = NULL, *_11 = NULL, *_13;
	zval *config = NULL, *name = NULL, *key = NULL, *val = NULL, *tmp, **_3, *_4 = NULL, *_6 = NULL, *_7, _8, _9, *_12 = NULL, *_15, *_16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &config, &name);

	ZEPHIR_SEPARATE_PARAM(config);
	if (!name) {
		name = ZEPHIR_GLOBAL(global_null);
	}


	zephir_update_property_this(this_ptr, SL("connected"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	if (Z_TYPE_P(config) == IS_STRING) {
		Z_SET_ISREF_P(config);
		ZEPHIR_CALL_FUNCTION(NULL, "parse_str", &_0, config, config);
		Z_UNSET_ISREF_P(config);
		zephir_check_call_status();
	} else {
		if (Z_TYPE_P(config) == IS_ARRAY == 0) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Configuration must be array, string or object.", "fastorm/db/Connection.zep", 44);
			return;
		}
		ZEPHIR_INIT_VAR(tmp);
		array_init(tmp);
		zephir_is_iterable(config, &_2, &_1, 0, 0, "fastorm/db/Connection.zep", 55);
		for (
		  ; zephir_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
		  ; zephir_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HMKEY(key, _2, _1);
			ZEPHIR_GET_HVALUE(val, _3);
			if (zephir_is_instance_of(val, SL("Traversable") TSRMLS_CC)) {
				ZEPHIR_CALL_FUNCTION(&_4, "iterator_to_array", &_5, val);
				zephir_check_call_status();
				zephir_array_update_zval(&tmp, key, &_4, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_zval(&tmp, key, &val, PH_COPY | PH_SEPARATE);
			}
		}
		ZEPHIR_CPY_WRT(config, tmp);
	}
	if (!(zephir_array_isset_string(config, SS("driver")))) {
		ZEPHIR_INIT_VAR(_6);
		ZVAL_STRING(_6, "mysql", 1);
		zephir_array_update_string(&config, SL("driver"), &_6, PH_COPY | PH_SEPARATE);
	}
	ZEPHIR_INIT_NVAR(_6);
	zephir_array_fetch_string(&_7, config, SL("driver"), PH_NOISY | PH_READONLY, "fastorm/db/Connection.zep", 64 TSRMLS_CC);
	zephir_fast_strtolower(_6, _7);
	ZEPHIR_SINIT_VAR(_8);
	ZVAL_STRING(&_8, "#[^a-z0-9_]#", 0);
	ZEPHIR_SINIT_VAR(_9);
	ZVAL_STRING(&_9, "_", 0);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_10, &_8, &_9, _6);
	zephir_check_call_status();
	zephir_get_strval(_11, _4);
	ZEPHIR_CPY_WRT(driver, _11);
	ZEPHIR_INIT_VAR(driverClass);
	ZEPHIR_CONCAT_SVS(driverClass, "\\Fastorm\\Db\\Drivers\\", driver, "Driver");
	if (zephir_class_exists(driverClass, zephir_is_true(ZEPHIR_GLOBAL(global_false))  TSRMLS_CC) == 0) {
		ZEPHIR_INIT_VAR(_12);
		object_init_ex(_12, fastorm_db_dbexception_ce);
		ZEPHIR_INIT_VAR(_13);
		ZEPHIR_CONCAT_SVS(_13, "Unable to create instance of dibi driver ", driverClass, ".");
		ZEPHIR_CALL_METHOD(NULL, _12, "__construct", NULL, _13);
		zephir_check_call_status();
		zephir_throw_exception_debug(_12, "fastorm/db/Connection.zep", 67 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_array_update_string(&config, SL("name"), &name, PH_COPY | PH_SEPARATE);
	zephir_update_property_this(this_ptr, SL("config"), config TSRMLS_CC);
	ZEPHIR_INIT_LNVAR(_12);
	_14 = zend_fetch_class(Z_STRVAL_P(driverClass), Z_STRLEN_P(driverClass), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(_12, _14);
	if (zephir_has_constructor(_12 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _12, "__construct", NULL);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("driver"), _12 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_15);
	object_init_ex(_15, fastorm_db_translator_ce);
	ZEPHIR_CALL_METHOD(NULL, _15, "__construct", NULL, this_ptr);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("translator"), _15 TSRMLS_CC);
	if (!(zephir_array_isset_string(config, SS("lazy")))) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_NVAR(_6);
	array_init(_6);
	zephir_update_property_this(this_ptr, SL("onEvent"), _6 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_16);
	array_init(_16);
	zephir_update_property_this(this_ptr, SL("config"), _16 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Automatically frees the resources allocated for this result set.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, __destruct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _1, _3;
	zval *_0, *_2, *_4, *_5 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	_1 = zephir_is_true(_0);
	if (_1) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_1 = Z_TYPE_P(_2) != IS_NULL;
	}
	_3 = _1;
	if (_3) {
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_5, _4, "getresource",  NULL);
		zephir_check_call_status();
		_3 = Z_TYPE_P(_5) != IS_NULL;
	}
	if (_3) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "disconnect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Connects to a database.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, connect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *e = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _0, "connect", NULL, _1);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_update_property_this(this_ptr, SL("connected"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 134 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Disconnects from a database.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, disconnect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(NULL, _0, "disconnect", NULL);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns TRUE when connection was established.
 * @return bool
 */
PHP_METHOD(Fastorm_Db_Connection, isConnected) {


	RETURN_MEMBER(this_ptr, "connected");

}

/**
 * Returns configuration variable. If no key is passed, returns the entire array.
 * @see self::__construct
 * @param  string
 * @param  mixed  default value to use if key not found
 * @return mixed
 */
PHP_METHOD(Fastorm_Db_Connection, getConfig) {

	zval *key = NULL, *defaultValue = NULL, *_0, *_1, *_2;

	zephir_fetch_params(0, 0, 2, &key, &defaultValue);

	if (!key) {
		key = ZEPHIR_GLOBAL(global_null);
	}
	if (!defaultValue) {
		defaultValue = ZEPHIR_GLOBAL(global_null);
	}


	if (Z_TYPE_P(key) == IS_NULL) {
		RETURN_MEMBER(this_ptr, "config");
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
		if (zephir_array_isset(_0, key)) {
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("config"), PH_NOISY_CC);
			zephir_array_fetch(&_2, _1, key, PH_NOISY | PH_READONLY, "fastorm/db/Connection.zep", 176 TSRMLS_CC);
			RETURN_CTORW(_2);
		} else {
			RETURN_CCTORW(defaultValue);
		}
	}

}

/**
 * Returns the driver and connects to a database in lazy mode.
 * @return IDibiDriver
 */
PHP_METHOD(Fastorm_Db_Connection, getDriver) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	RETURN_MM_MEMBER(this_ptr, "driver");

}

/**
 * Generates (translates) and executes SQL query.
 * @param  array|mixed      one or more arguments
 * @return DibiResult|int   result set object (if any)
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, query) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_3 = NULL;
	zend_bool _0;
	zval *args = NULL, *metadata = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &args, &metadata);

	ZEPHIR_SEPARATE_PARAM(args);
	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_1);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "translateargs", &_3, args);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nativequery", NULL, _2, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Generates SQL query.
 * @param  array|mixed      one or more arguments
 * @return string
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, translate) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL, *_1 = NULL;
	zval *args = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "translateargs", &_1, args);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Generates SQL query.
 * @param  array
 * @return string
 */
PHP_METHOD(Fastorm_Db_Connection, translateArgs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *args, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("translator"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_METHOD(_1, "translate", NULL, args);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Executes the SQL query.
 * @param  string           SQL statement.
 * @return DibiResult|int   result set object (if any)
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, nativeQuery) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *sql, *metadata = NULL, *res = NULL, *e = NULL, *_1, *_2, *_3 = NULL, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &sql, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_1)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(res);
	ZVAL_NULL(res);

	/* try_start_1: */

		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&res, _2, "query", NULL, sql);
		zephir_check_call_status_or_jump(try_end_1);
		if (zephir_is_true(res)) {
			ZEPHIR_CALL_METHOD(&_3, this_ptr, "createresultset", NULL, res, metadata);
			zephir_check_call_status_or_jump(try_end_1);
			ZEPHIR_CPY_WRT(res, _3);
		} else {
			_4 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
			ZEPHIR_CALL_METHOD(&res, _4, "getaffectedrows",  NULL);
			zephir_check_call_status_or_jump(try_end_1);
		}
		RETURN_CCTOR(res);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 275 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Gets the number of affected rows by the last INSERT, UPDATE or DELETE query.
 * @return int  number of rows
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, getAffectedRows) {

	zend_bool _4;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *rows = NULL, *_0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&rows, _1, "getaffectedrows",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_2, "is_int", &_3, rows);
	zephir_check_call_status();
	_4 = ZEPHIR_IS_FALSE_IDENTICAL(_2);
	if (!(_4)) {
		_4 = ZEPHIR_LT_LONG(rows, 0);
	}
	if (_4) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Cannot retrieve number of affected rows.", "fastorm/db/Connection.zep", 294);
		return;
	}
	RETURN_CCTOR(rows);

}

/**
 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous INSERT query.
 * @param  string     optional sequence name
 * @return int
 * @throws DibiException
 */
PHP_METHOD(Fastorm_Db_Connection, getInsertId) {

	int id, ZEPHIR_LAST_CALL_STATUS;
	zval *sequence = NULL, *_0, *_1, *_2 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &sequence);

	if (!sequence) {
		sequence = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_2, _1, "getinsertid", NULL, sequence);
	zephir_check_call_status();
	id = zephir_get_intval(_2);
	if (id < 1) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Cannot retrieve last generated ID.", "fastorm/db/Connection.zep", 315);
		return;
	}
	RETURN_MM_LONG(id);

}

/**
 * Begins a transaction (if supported).
 * @param  string  optional savepoint name
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, begin) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}

	/* try_start_1: */

		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "begin", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 340 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Commits statements in a transaction.
 * @param  string  optional savepoint name
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, commit) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}

	/* try_start_1: */

		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "commit", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 364 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Rollback changes in a transaction.
 * @param  string  optional savepoint name
 * @return void
 */
PHP_METHOD(Fastorm_Db_Connection, rollback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *e = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connected"), PH_NOISY_CC);
	if (ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "connect", NULL);
		zephir_check_call_status();
	}

	/* try_start_1: */

		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "rollback", NULL, savepoint);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(e, "fastorm/db/Connection.zep", 388 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Result set factory.
 * @param  IDibiResultDriver
 * @return DibiResult
 */
PHP_METHOD(Fastorm_Db_Connection, createResultSet) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *resultDriver, *metadata = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &resultDriver, &metadata);

	if (!metadata) {
		metadata = ZEPHIR_GLOBAL(global_null);
	}


	if (!(zephir_instance_of_ev(resultDriver, fastorm_db_iresultdriver_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'resultDriver' must be an instance of 'Fastorm\\Db\\IResultDriver'", "", 0);
		return;
	}
	_0 = Z_TYPE_P(metadata) != IS_NULL;
	if (_0) {
		_0 = !zephir_instance_of_ev(metadata, fastorm_objectmetadata_ce TSRMLS_CC);
	}
	if (_0) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'metadata' must be an instance of 'Fastorm\\ObjectMetadata'", "", 0);
		return;
	}
	object_init_ex(return_value, fastorm_db_result_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, resultDriver, metadata);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @return DibiFluent
 */
PHP_METHOD(Fastorm_Db_Connection, command) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, fastorm_db_query_ce);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, this_ptr);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @param  string    column name
 * @return DibiFluent
 */
PHP_METHOD(Fastorm_Db_Connection, select) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *args = NULL, *_1 = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_CALL_FUNCTION(&args, "func_get_args", &_0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "select", 0);
	ZEPHIR_RETURN_CALL_METHOD(_1, "__call", NULL, _2, args);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @param  string   table
 * @param  array
 * @return DibiFluent
 */
PHP_METHOD(Fastorm_Db_Connection, update) {

	zval *_3;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _0;
	zval *table, *args, *_1 = NULL, *_2 = NULL, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &table, &args);



	_0 = Z_TYPE_P(args) == IS_ARRAY;
	if (!(_0)) {
		_0 = (zephir_is_instance_of(args, SL("Traversable") TSRMLS_CC));
	}
	if (!(_0)) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Arguments must be array or Traversable.", "fastorm/db/Connection.zep", 438);
		return;
	}
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_3);
	array_init_size(_3, 3);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_STRING(_4, "%n", 1);
	zephir_array_fast_append(_3, _4);
	zephir_array_fast_append(_3, table);
	ZEPHIR_INIT_BNVAR(_4);
	ZVAL_STRING(_4, "update", 0);
	ZEPHIR_CALL_METHOD(&_2, _1, "__call", NULL, _4, _3);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	ZEPHIR_INIT_BNVAR(_4);
	ZVAL_STRING(_4, "set", 0);
	ZEPHIR_RETURN_CALL_METHOD(_2, "__call", NULL, _4, args);
	zephir_check_temp_parameter(_4);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @param  string   table
 * @param  array
 * @return DibiFluent
 */
PHP_METHOD(Fastorm_Db_Connection, insert) {

	zval *_2, *_4;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *table, *args = NULL, *_0 = NULL, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &table, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	if (zephir_is_instance_of(args, SL("Traversable") TSRMLS_CC)) {
		ZEPHIR_CALL_FUNCTION(&_0, "iterator_to_array", NULL, args);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(args, _0);
	} else {
		if (Z_TYPE_P(args) == IS_ARRAY == 0) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Arguments must be array or Traversable.", "fastorm/db/Connection.zep", 456);
			return;
		}
	}
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 6);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "%n", 1);
	zephir_array_fast_append(_2, _3);
	zephir_array_fast_append(_2, table);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "(%n)", 1);
	zephir_array_fast_append(_2, _3);
	ZEPHIR_INIT_BNVAR(_3);
	zephir_array_keys(_3, args TSRMLS_CC);
	zephir_array_fast_append(_2, _3);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "insert", 0);
	ZEPHIR_CALL_METHOD(&_1, _0, "__call", NULL, _3, _2);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_4);
	array_init_size(_4, 3);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "%l", 1);
	zephir_array_fast_append(_4, _3);
	zephir_array_fast_append(_4, args);
	ZEPHIR_INIT_BNVAR(_3);
	ZVAL_STRING(_3, "values", 0);
	ZEPHIR_RETURN_CALL_METHOD(_1, "__call", NULL, _3, _4);
	zephir_check_temp_parameter(_3);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * @param  string   table
 * @return DibiFluent
 */
PHP_METHOD(Fastorm_Db_Connection, delete) {

	zval *_1;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *table, *_0 = NULL, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &table);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "command",  NULL);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 3);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "%n", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_fast_append(_1, table);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "delete", 0);
	ZEPHIR_RETURN_CALL_METHOD(_0, "__call", NULL, _2, _1);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	RETURN_MM();

}

