
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/hash.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_Drivers_MysqliDriver) {

	ZEPHIR_REGISTER_CLASS_EX(Fastorm\\Db\\Drivers, MysqliDriver, fastorm, db_drivers_mysqlidriver, fastorm_dbobject_ce, fastorm_db_drivers_mysqlidriver_method_entry, 0);

	/** @var mysqli  Connection resource */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("connection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var mysqli_result  Resultset resource */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("resultSet"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_bool(fastorm_db_drivers_mysqlidriver_ce, SL("autoFree"), 1, ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool  Is buffered (seekable and countable)? */
	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("buffered"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_drivers_mysqlidriver_ce, SL("types"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_ACCESS_DENIED"), 1045 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_DUPLICATE_ENTRY"), 1062 TSRMLS_CC);

	zend_declare_class_constant_long(fastorm_db_drivers_mysqlidriver_ce, SL("ERROR_DATA_TRUNCATED"), 1265 TSRMLS_CC);

	zend_class_implements(fastorm_db_drivers_mysqlidriver_ce TSRMLS_CC, 1, fastorm_db_iresultdriver_ce);
	return SUCCESS;

}

/**
 * @throws DibiNotSupportedException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __construct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval _0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "mysqli", 0);
	ZEPHIR_CALL_FUNCTION(&_1, "extension_loaded", &_2, &_0);
	zephir_check_call_status();
	if (!(zephir_is_true(_1))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: PHP extension 'mysqli' is not loaded.", "fastorm/db/Drivers/MysqliDriver.zep", 30);
		return;
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Connects to a database.
 * @return void
 * @throws DbException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, connect) {

	zephir_fcall_cache_entry *_16 = NULL, *_25 = NULL;
	HashTable *_13;
	HashPosition _12;
	zval *_10;
	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL, *_8 = NULL, *_11 = NULL, *_28 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *config_param = NULL, _0 = zval_used_for_init, *_1, *_2 = NULL, *_4 = NULL, *host = NULL, *foo, *_6 = NULL, *_7 = NULL, *_9, *key = NULL, *value = NULL, *iterate, **_14, *_15, *_17, *_18 = NULL, *_19, *_20, *_21, *_22, *_23, *_24, *_26, *ok = NULL, _27, *_29, *_30;
	zval *config = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &config_param);

	zephir_get_arrval(config, config_param);
	ZEPHIR_SEPARATE_PARAM(config);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(NULL, "mysqli_report", NULL, &_0);
	zephir_check_call_status();
	if (zephir_array_isset_string(config, SS("resource"))) {
		zephir_array_fetch_string(&_1, config, SL("resource"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 43 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("connection"), _1 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_VAR(_2);
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 11);
		add_assoc_stringl_ex(_3, SS("charset"), SL("utf8"), 1);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_LONG(&_0, 'P');
		ZEPHIR_CALL_FUNCTION(&_4, "date", NULL, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&_3, SL("timezone"), &_4, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_user", 0);
		ZEPHIR_CALL_FUNCTION(&_4, "ini_get", &_5, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&_3, SL("username"), &_4, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_pw", 0);
		ZEPHIR_CALL_FUNCTION(&_4, "ini_get", &_5, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&_3, SL("password"), &_4, PH_COPY | PH_SEPARATE);
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "mysqli.default_socket", 0);
		ZEPHIR_CALL_FUNCTION(&_4, "ini_get", &_5, &_0);
		zephir_check_call_status();
		zephir_array_update_string(&_3, SL("socket"), &_4, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_3, SL("port"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
		zephir_fast_array_merge(_2, &(config), &(_3) TSRMLS_CC);
		if (!(zephir_array_isset_string(config, SS("host")))) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_STRING(&_0, "mysqli.default_host", 0);
			ZEPHIR_CALL_FUNCTION(&host, "ini_get", &_5, &_0);
			zephir_check_call_status();
			if (zephir_is_true(host)) {
				zephir_array_update_string(&config, SL("host"), &host, PH_COPY | PH_SEPARATE);
				ZEPHIR_SINIT_NVAR(_0);
				ZVAL_STRING(&_0, "mysqli.default_port", 0);
				ZEPHIR_CALL_FUNCTION(&_4, "ini_get", &_5, &_0);
				zephir_check_call_status();
				zephir_array_update_string(&config, SL("port"), &_4, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_string(&config, SL("host"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&config, SL("port"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
			}
		}
		ZEPHIR_OBS_VAR(foo);
		zephir_array_fetch_string(&foo, config, SL("database"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 67 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&_6, "mysqli_init", NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("connection"), _6 TSRMLS_CC);
		if (zephir_array_isset_string(config, SS("options"))) {
			zephir_array_fetch_string(&_1, config, SL("options"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 71 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(&_7, "is_scalar", &_8, _1);
			zephir_check_call_status();
			if (zephir_is_true(_7)) {
				zephir_array_fetch_string(&_9, config, SL("options"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 72 TSRMLS_CC);
				zephir_array_update_string(&config, SL("flags"), &_9, PH_COPY | PH_SEPARATE);
				ZEPHIR_INIT_VAR(_10);
				ZEPHIR_CONCAT_SS(_10, "MysqliDriver", ": configuration item 'options' must be array; for constants MYSQLI_CLIENT_* use 'flags'.");
				ZEPHIR_SINIT_NVAR(_0);
				ZVAL_LONG(&_0, 1024);
				ZEPHIR_CALL_FUNCTION(NULL, "trigger_error", &_11, _10, &_0);
				zephir_check_call_status();
			} else {
				ZEPHIR_OBS_VAR(iterate);
				zephir_array_fetch_string(&iterate, config, SL("options"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 76 TSRMLS_CC);
				zephir_is_iterable(iterate, &_13, &_12, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 80);
				for (
				  ; zephir_hash_get_current_data_ex(_13, (void**) &_14, &_12) == SUCCESS
				  ; zephir_hash_move_forward_ex(_13, &_12)
				) {
					ZEPHIR_GET_HMKEY(key, _13, _12);
					ZEPHIR_GET_HVALUE(value, _14);
					_15 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
					ZEPHIR_CALL_FUNCTION(NULL, "mysqli_options", &_16, _15, key, value);
					zephir_check_call_status();
				}
			}
		}
		zephir_array_fetch_string(&_9, config, SL("persistent"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 82 TSRMLS_CC);
		if (ZEPHIR_IS_EMPTY(_9)) {
			_15 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_17, config, SL("host"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_18);
			ZEPHIR_CONCAT_SV(_18, "", _17);
			zephir_array_fetch_string(&_19, config, SL("username"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			zephir_array_fetch_string(&_20, config, SL("password"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			zephir_array_fetch_string(&_21, config, SL("database"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			zephir_array_fetch_string(&_22, config, SL("port"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			zephir_array_fetch_string(&_23, config, SL("socket"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			zephir_array_fetch_string(&_24, config, SL("flags"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 83 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(NULL, "mysqli_real_connect", &_25, _15, _18, _19, _20, _21, _22, _23, _24);
			zephir_check_call_status();
		} else {
			_26 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_17, config, SL("host"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			ZEPHIR_INIT_LNVAR(_18);
			ZEPHIR_CONCAT_SV(_18, "p:", _17);
			zephir_array_fetch_string(&_19, config, SL("username"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			zephir_array_fetch_string(&_20, config, SL("password"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			zephir_array_fetch_string(&_21, config, SL("database"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			zephir_array_fetch_string(&_22, config, SL("port"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			zephir_array_fetch_string(&_23, config, SL("socket"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			zephir_array_fetch_string(&_24, config, SL("flags"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 85 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(NULL, "mysqli_real_connect", &_25, _26, _18, _19, _20, _21, _22, _23, _24);
			zephir_check_call_status();
		}
	}
	if (zephir_array_isset_string(config, SS("charset"))) {
		ZEPHIR_INIT_VAR(ok);
		ZVAL_BOOL(ok, 0);
		ZEPHIR_INIT_NVAR(_2);
		ZEPHIR_GET_CONSTANT(_2, "PHP_VERSION");
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_STRING(&_0, "5.1.5", 0);
		ZEPHIR_SINIT_VAR(_27);
		ZVAL_STRING(&_27, ">=", 0);
		ZEPHIR_CALL_FUNCTION(&_4, "version_compare", &_28, _2, &_0, &_27);
		zephir_check_call_status();
		if (zephir_is_true(_4)) {
			_26 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			zephir_array_fetch_string(&_1, config, SL("charset"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 99 TSRMLS_CC);
			ZEPHIR_CALL_FUNCTION(&ok, "mysqli_set_charset", NULL, _26, _1);
			zephir_check_call_status();
		}
		if (!(zephir_is_true(ok))) {
			ZEPHIR_INIT_VAR(_29);
			ZVAL_STRING(_29, "SET NAMES 'config[charset]'", 0);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _29);
			zephir_check_temp_parameter(_29);
			zephir_check_call_status();
		}
	}
	if (zephir_array_isset_string(config, SS("sqlmode"))) {
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "SET sql_mode='config[sqlmode]'", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _2);
		zephir_check_temp_parameter(_2);
		zephir_check_call_status();
	}
	if (zephir_array_isset_string(config, SS("timezone"))) {
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_STRING(_2, "SET time_zone='config[timezone]'", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _2);
		zephir_check_temp_parameter(_2);
		zephir_check_call_status();
	}
	ZEPHIR_OBS_VAR(_30);
	zephir_array_fetch_string(&_30, config, SL("unbuffered"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 114 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("buffered"), (ZEPHIR_IS_EMPTY(_30)) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Disconnects from a database.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, disconnect) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(NULL, "mysqli_close", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

/**
 * Executes the SQL query.
 * @param  string      SQL statement.
 * @return IDibiResultDriver|NULL
 * @throws DibiDriverException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, query) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL, *_5 = NULL;
	zval *sql, *res = NULL, *_0, *_1, _2 = zval_used_for_init, *_4 = NULL, *_6, *_7, *_8 = NULL, *_9, *_10, *_11 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &sql);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_2);
		ZVAL_LONG(&_2, 0);
		ZEPHIR_CALL_FUNCTION(&res, "mysqli_query", &_3, _1, sql, &_2);
		zephir_check_call_status();
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_2);
		ZVAL_LONG(&_2, 1);
		ZEPHIR_CALL_FUNCTION(&res, "mysqli_query", &_3, _1, sql, &_2);
		zephir_check_call_status();
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_4, "mysqli_errno", &_5, _1);
	zephir_check_call_status();
	if (zephir_is_true(_4)) {
		ZEPHIR_INIT_VAR(_6);
		object_init_ex(_6, fastorm_db_dbexception_ce);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(&_8, "mysqli_error", NULL, _7);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_9);
		ZEPHIR_CONCAT_SV(_9, "Db not supported exception:", _8);
		_10 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(&_11, "mysqli_errno", &_5, _10);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(NULL, _6, "__construct", NULL, _9, _11, sql);
		zephir_check_call_status();
		zephir_throw_exception_debug(_6, "fastorm/db/Drivers/MysqliDriver.zep", 143 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	} else {
		if (Z_TYPE_P(res) == IS_OBJECT) {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "createresultdriver", NULL, res);
			zephir_check_call_status();
			RETURN_MM();
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Retrieves information about the most recently executed query.
 * @return array
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInfo) {

	HashTable *_8;
	HashPosition _7;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL, *_6 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *res, *matches, *m = NULL, *_0, *_1 = NULL, *_2 = NULL, *_3, *_5 = NULL, **_9, *hod, *_10 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(res);
	array_init(res);

	ZEPHIR_INIT_VAR(matches);
	array_init(matches);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "mysqli_info", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "#(.+?): +(\d+) *#", 0);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 2);
	Z_SET_ISREF_P(matches);
	ZEPHIR_CALL_FUNCTION(NULL, "preg_match_all", &_4, _2, _1, matches, _3);
	zephir_check_temp_parameter(_2);
	Z_UNSET_ISREF_P(matches);
	zephir_check_call_status();
	ZEPHIR_CALL_FUNCTION(&_5, "preg_last_error", &_6);
	zephir_check_call_status();
	if (zephir_is_true(_5)) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db Pcre Exception", "fastorm/db/Drivers/MysqliDriver.zep", 162);
		return;
	}
	zephir_is_iterable(matches, &_8, &_7, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 170);
	for (
	  ; zephir_hash_get_current_data_ex(_8, (void**) &_9, &_7) == SUCCESS
	  ; zephir_hash_move_forward_ex(_8, &_7)
	) {
		ZEPHIR_GET_HVALUE(m, _9);
		zephir_array_fetch_long(&hod, m, 1, PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 167 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(_10);
		zephir_array_fetch_long(&_10, m, 2, PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 168 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_LONG(_2, zephir_get_intval(_10));
		zephir_array_update_zval(&res, hod, &_2, PH_COPY | PH_SEPARATE);
	}
	RETURN_CCTOR(res);

}

/**
 * Gets the number of affected rows by the last INSERT, UPDATE or DELETE query.
 * @return int|false  number of rows or false on error
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *_0, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&_1, "mysqli_affected_rows", &_2, _0);
	zephir_check_call_status();
	if (ZEPHIR_IS_LONG_IDENTICAL(_1, -1)) {
		RETURN_MM_BOOL(0);
	} else {
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_affected_rows", &_2, _3);
		zephir_check_call_status();
		RETURN_MM();
	}

}

/**
 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous INSERT query.
 * @return int|false  int on success or false on failure
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInsertId) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *sequence, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &sequence);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_insert_id", NULL, _0);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Begins a transaction (if supported).
 * @param  string  optional savepoint name
 * @return void
 * @throws DibiDriverException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, begin) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "START TRANSACTION", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Commits statements in a transaction.
 * @param  string  optional savepoint name
 * @return void
 * @throws DibiDriverException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, commit) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "RELEASE SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "COMMIT", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Rollback changes in a transaction.
 * @param  string  optional savepoint name
 * @return void
 * @throws DibiDriverException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, rollback) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *savepoint = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &savepoint);

	if (!savepoint) {
		savepoint = ZEPHIR_GLOBAL(global_null);
	}


	if (zephir_is_true(savepoint)) {
		ZEPHIR_INIT_VAR(_0);
		ZVAL_STRING(_0, "ROLLBACK TO SAVEPOINT savepoint", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	} else {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_STRING(_0, "ROLLBACK", 0);
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "query", NULL, _0);
		zephir_check_temp_parameter(_0);
		zephir_check_call_status();
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the connection resource.
 * @return mysqli
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResource) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(_1);
	zephir_read_property(&_1, _0, SL("thread_id"), PH_NOISY_CC);
	if (zephir_is_true(_1)) {
		RETURN_MM_MEMBER(this_ptr, "connection");
	} else {
		RETURN_MM_NULL();
	}

}

/**
 * Result set driver factory.
 * @param  mysqli_result
 * @return IDibiResultDriver
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, createResultDriver) {

	zval *resourc, *res;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &resourc);



	if (!(zephir_is_instance_of(resourc, SL("mysqli_result") TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'resourc' must be an instance of 'mysqli_result'", "", 0);
		return;
	}
	ZEPHIR_INIT_VAR(res);
	if (zephir_clone(res, this_ptr TSRMLS_CC) == FAILURE) {
		RETURN_MM();
	}
	zephir_update_property_zval(res, SL("resultSet"), resourc TSRMLS_CC);
	RETURN_CCTOR(res);

}

/**
 * Encodes data for use in a SQL statement.
 * @param  mixed     value
 * @param  string    type (dibi::TEXT, dibi::BOOL, ...)
 * @return string    encoded value
 * @throws InvalidArgumentException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escape) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_6 = NULL;
	zval *value, *type, *_0, *_1 = NULL, *_3 = NULL, _4 = zval_used_for_init, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &type);



	do {
		if (ZEPHIR_IS_STRING(type, "s")) {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&_1, "mysqli_real_escape_string", &_2, _0, value);
			zephir_check_call_status();
			ZEPHIR_CONCAT_SVS(return_value, "'", _1, "'");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "bin")) {
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&_1, "mysqli_real_escape_string", &_2, _0, value);
			zephir_check_call_status();
			ZEPHIR_CONCAT_SVS(return_value, "_binary'", _1, "'");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "n")) {
			ZEPHIR_INIT_VAR(_3);
			ZEPHIR_SINIT_VAR(_4);
			ZVAL_LONG(&_4, '`');
			ZEPHIR_SINIT_VAR(_5);
			ZVAL_LONG(&_5, '``');
			zephir_fast_str_replace(_3, &_4, &_5, value);
			ZEPHIR_CONCAT_SVS(return_value, "`", _3, "`");
			RETURN_MM();
		}
		if (ZEPHIR_IS_STRING(type, "b")) {
			if (zephir_is_true(value)) {
				ZEPHIR_CONCAT_SS(return_value, "1", "");
				RETURN_MM();
			} else {
				ZEPHIR_CONCAT_SS(return_value, "0", "");
				RETURN_MM();
			}
		}
		if (ZEPHIR_IS_STRING(type, "d")) {
			if (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "'Y-m-d'", 0);
				ZEPHIR_RETURN_CALL_METHOD(value, "format", NULL, _3);
				zephir_check_temp_parameter(_3);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				ZEPHIR_SINIT_NVAR(_4);
				ZVAL_STRING(&_4, "'Y-m-d'", 0);
				ZEPHIR_RETURN_CALL_FUNCTION("date", &_6, &_4, value);
				zephir_check_call_status();
				RETURN_MM();
			}
		}
		if (ZEPHIR_IS_STRING(type, "t")) {
			if (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC)) {
				ZEPHIR_INIT_NVAR(_3);
				ZVAL_STRING(_3, "'Y-m-d H:i:s'", 0);
				ZEPHIR_RETURN_CALL_METHOD(value, "format", NULL, _3);
				zephir_check_temp_parameter(_3);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				ZEPHIR_SINIT_NVAR(_4);
				ZVAL_STRING(&_4, "'Y-m-d H:i:s'", 0);
				ZEPHIR_RETURN_CALL_FUNCTION("date", &_6, &_4, value);
				zephir_check_call_status();
				RETURN_MM();
			}
		}
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Unsupported type.", "fastorm/db/Drivers/MysqliDriver.zep", 315);
		return;
	} while(0);

	RETURN_MM_STRING("", 1);

}

/**
 * Encodes string for use in a LIKE statement.
 * @param  string
 * @param  int
 * @return string
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escapeLike) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zval *value = NULL, *pos, *_0, _1, _2, _3, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &value, &pos);

	ZEPHIR_SEPARATE_PARAM(value);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "\\", 0);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "\\\\", 0);
	zephir_fast_str_replace(_0, &_1, &_2, value);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_STRING(&_3, "\x00\n\r\\'%_", 0);
	ZEPHIR_CALL_FUNCTION(&value, "addcslashes", &_4, _0, &_3);
	zephir_check_call_status();
	if (ZEPHIR_LE_LONG(pos, 0)) {
		ZEPHIR_INIT_VAR(_5);
		if (ZEPHIR_GE_LONG(pos, 0)) {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "%'", 1);
		} else {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "'", 1);
		}
		ZEPHIR_CONCAT_SVV(return_value, "'%", value, _5);
		RETURN_MM();
	} else {
		ZEPHIR_INIT_LNVAR(_5);
		if (ZEPHIR_GE_LONG(pos, 0)) {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "%'", 1);
		} else {
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "'", 1);
		}
		ZEPHIR_CONCAT_SVV(return_value, "'", value, _5);
		RETURN_MM();
	}

}

/**
 * Decodes data from result set.
 * @param  string    value
 * @param  string    type (dibi::BINARY)
 * @return string    decoded value
 * @throws InvalidArgumentException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, unescape) {

	zval *value, *type;

	zephir_fetch_params(0, 2, 0, &value, &type);



	if (ZEPHIR_IS_STRING_IDENTICAL(type, "bin")) {
		RETURN_CCTORW(value);
	}
	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Unsupported type.", "fastorm/db/Drivers/MysqliDriver.zep", 350);
	return;

}

/**
 * Injects LIMIT/OFFSET to the SQL query.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, applyLimit) {

	zend_bool _0;
	zval *sql, *limit = NULL, *offset = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &sql, &limit, &offset);

	ZEPHIR_SEPARATE_PARAM(limit);
	ZEPHIR_SEPARATE_PARAM(offset);


	_0 = ZEPHIR_GE_LONG(limit, 0);
	if (!(_0)) {
		_0 = ZEPHIR_GT_LONG(offset, 0);
	}
	if (_0) {
		if (ZEPHIR_LT_LONG(limit, 0)) {
			ZEPHIR_INIT_NVAR(limit);
			ZVAL_LONG(limit, 18446744073709551615);
		} else {
		}
		ZEPHIR_INIT_NVAR(offset);
		if (ZEPHIR_GT_LONG(offset, 0)) {
			ZEPHIR_INIT_VAR(_1);
			ZEPHIR_CONCAT_SV(_1, " OFFSET ", offset);
			ZEPHIR_CPY_WRT(offset, _1);
		} else {
			ZVAL_STRING(offset, "", 1);
		}
		ZEPHIR_CONCAT_VSVV(return_value, sql, " LIMIT ", limit, offset);
		RETURN_MM();
	}
	RETURN_MM_STRING("", 1);

}

/**
 * Automatically frees the resources allocated for this result set.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __destruct) {

	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool _1, _3;
	zval *_0, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("autoFree"), PH_NOISY_CC);
	_1 = zephir_is_true(_0);
	if (_1) {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "getresultresource",  NULL);
		zephir_check_call_status();
		_1 = zephir_is_true(_2);
	}
	_3 = _1;
	if (_3) {
		ZEPHIR_CALL_METHOD(&_4, this_ptr, "free",  NULL);
		zephir_check_call_status();
		_3 = zephir_is_true(_4);
	}
	RETURN_MM_BOOL(_3);

}

/**
 * Returns the number of rows in a result set.
 * @return int
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getRowCount) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (!(zephir_is_true(_0))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: Row count is not available for unbuffered queries.", "fastorm/db/Drivers/MysqliDriver.zep", 399);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_num_rows", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Fetches the row at current position and moves the internal cursor to the next position.
 * @param  bool     true for associative array, false for numeric
 * @return array    array on success, nonarray if no next record
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, fetchRow) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *associative, *_0, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &associative);



	if (zephir_is_true(associative)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_1);
		ZVAL_LONG(&_1, 1);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_fetch_array", &_2, _0, &_1);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_LONG(&_1, 2);
		ZEPHIR_RETURN_CALL_FUNCTION("mysqli_fetch_array", &_2, _0, &_1);
		zephir_check_call_status();
		RETURN_MM();
	}

}

/**
 * Moves cursor position without fetching row.
 * @param  int      the 0-based cursor pos to seek to
 * @return boolean  true on success, false if unable to seek to specified record
 * @throws DbException
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, seek) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *row, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("buffered"), PH_NOISY_CC);
	if (!(zephir_is_true(_0))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Db not supported exception: Cannot seek an unbuffered result set.", "fastorm/db/Drivers/MysqliDriver.zep", 429);
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_RETURN_CALL_FUNCTION("mysqli_data_seek", NULL, _1, row);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Frees the resources allocated for this result set.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, free) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(NULL, "mysqli_free_result", NULL, _0);
	zephir_check_call_status();
	zephir_update_property_this(this_ptr, SL("resultSet"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns metadata for all columns in a result set.
 * @return array
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultColumns) {

	zval *_21 = NULL;
	zephir_fcall_cache_entry *_18 = NULL, *_23 = NULL;
	zend_bool _14;
	HashTable *_4;
	HashPosition _3;
	int ZEPHIR_LAST_CALL_STATUS, _15;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL, *_9 = NULL, *_11 = NULL;
	zval *_0, *consts = NULL, *key = NULL, *value = NULL, *_2, **_5, _6 = zval_used_for_init, _7 = zval_used_for_init, *_8 = NULL, *_10 = NULL, *_12 = NULL, *row = NULL, *count = NULL, *i = NULL, *table = NULL, *_13, *columns, *_16 = NULL, *_17, *_19, *_20, *_22 = NULL, *_24;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(columns);
	array_init(columns);

	zephir_read_static_property_ce(&_0, fastorm_db_drivers_mysqlidriver_ce, SL("types") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_CALL_FUNCTION(&consts, "get_defined_constants", &_1, ZEPHIR_GLOBAL(global_true));
		zephir_check_call_status();
		zephir_array_fetch_string(&_2, consts, SL("mysqli"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 455 TSRMLS_CC);
		zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/db/Drivers/MysqliDriver.zep", 460);
		for (
		  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
		  ; zephir_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HMKEY(key, _4, _3);
			ZEPHIR_GET_HVALUE(value, _5);
			ZEPHIR_SINIT_NVAR(_6);
			ZVAL_STRING(&_6, "MYSQLI_TYPE_", 0);
			ZEPHIR_SINIT_NVAR(_7);
			ZVAL_LONG(&_7, 12);
			ZEPHIR_CALL_FUNCTION(&_8, "strncmp", &_9, key, &_6, &_7);
			zephir_check_call_status();
			if (ZEPHIR_IS_LONG_IDENTICAL(_8, 0)) {
				ZEPHIR_SINIT_NVAR(_6);
				ZVAL_LONG(&_6, 12);
				ZEPHIR_CALL_FUNCTION(&_10, "substr", &_11, key, &_6);
				zephir_check_call_status();
				zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_10 TSRMLS_CC, SL("z"), 1, value);
			}
		}
		ZEPHIR_INIT_VAR(_12);
		ZVAL_STRING(_12, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_12 TSRMLS_CC, SL("l"), 1, 1);
		ZEPHIR_INIT_NVAR(_12);
		ZVAL_STRING(_12, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_12 TSRMLS_CC, SL("l"), 1, 2);
		ZEPHIR_INIT_NVAR(_12);
		ZVAL_STRING(_12, "INT", 1);
		zephir_update_static_property_array_multi_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types"), &_12 TSRMLS_CC, SL("l"), 1, 3);
	}
	_13 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_CALL_FUNCTION(&count, "mysqli_num_fields", NULL, _13);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(_16, count);
	_15 = 1;
	_14 = 0;
	if (ZEPHIR_GE_LONG(_16, _15)) {
		while (1) {
			if (_14) {
				_15++;
				if (!(ZEPHIR_GE_LONG(_16, _15))) {
					break;
				}
			} else {
				_14 = 1;
			}
			ZEPHIR_INIT_NVAR(i);
			ZVAL_LONG(i, _15);
			_17 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
			ZEPHIR_CALL_FUNCTION(&row, "mysqli_fetch_field_direct", &_18, _17, i);
			zephir_check_call_status();
			zephir_array_fetch_string(&_2, row, SL("table"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 470 TSRMLS_CC);
			if (zephir_is_true(_2)) {
				zephir_array_fetch_string(&_19, row, SL("table"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 471 TSRMLS_CC);
				zephir_array_fetch_string(&_20, row, SL("name"), PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 471 TSRMLS_CC);
				ZEPHIR_INIT_NVAR(table);
				ZEPHIR_CONCAT_VSV(table, _19, ".", _20);
			} else {
				ZEPHIR_OBS_NVAR(table);
				zephir_array_fetch_string(&table, row, SL("name"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 473 TSRMLS_CC);
			}
			ZEPHIR_INIT_NVAR(_21);
			array_init_size(_21, 7);
			ZEPHIR_OBS_NVAR(_22);
			zephir_array_fetch_string(&_22, row, SL("name"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 477 TSRMLS_CC);
			zephir_array_update_string(&_21, SL("name"), &_22, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_22);
			zephir_array_fetch_string(&_22, row, SL("orgtable"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 478 TSRMLS_CC);
			zephir_array_update_string(&_21, SL("table"), &_22, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_21, SL("fullname"), &table, PH_COPY | PH_SEPARATE);
			_24 = zephir_fetch_static_property_ce(fastorm_db_drivers_mysqlidriver_ce, SL("types") TSRMLS_CC);
			ZEPHIR_OBS_NVAR(_22);
			zephir_array_fetch_string(&_22, row, SL("type"), PH_NOISY, "fastorm/db/Drivers/MysqliDriver.zep", 480 TSRMLS_CC);
			zephir_array_fetch(&_19, _24, _22, PH_NOISY | PH_READONLY, "fastorm/db/Drivers/MysqliDriver.zep", 480 TSRMLS_CC);
			ZEPHIR_CALL_CE_STATIC(&_8, fastorm_db_dibicolumninfo_ce, "detecttype", &_23, _19);
			zephir_check_call_status();
			zephir_array_update_string(&_21, SL("nativetype"), &_8, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_21, SL("vendor"), &row, PH_COPY | PH_SEPARATE);
			zephir_array_append(&columns, _21, PH_SEPARATE, "fastorm/db/Drivers/MysqliDriver.zep", 481);
		}
	}
	RETURN_CCTOR(columns);

}

/**
 * Returns the result set resource.
 * @return mysqli_result
 */
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultResource) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	zephir_update_property_this(this_ptr, SL("autoFree"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("resultSet"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(_1);
	zephir_read_property(&_1, _0, SL("type"), PH_NOISY_CC);
	if (Z_TYPE_P(_1) == IS_NULL) {
		RETURN_MM_NULL();
	} else {
		RETURN_MM_MEMBER(this_ptr, "resultSet");
	}

}

