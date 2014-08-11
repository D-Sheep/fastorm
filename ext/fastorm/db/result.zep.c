
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/hash.h"
#include "kernel/string.h"

ZEPHIR_INIT_CLASS(Fastorm_Db_Result) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Result, fastorm, db_result, fastorm_db_result_method_entry, 0);

	/** @var array  IResultDriver */
	zend_declare_property_null(fastorm_db_result_ce, SL("driver"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var array  Translate table */
	zend_declare_property_null(fastorm_db_result_ce, SL("types"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var DibiResultInfo */
	zend_declare_property_null(fastorm_db_result_ce, SL("meta"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool  Already fetched? Used for allowance for first seek(0) */
	zend_declare_property_bool(fastorm_db_result_ce, SL("fetched"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var array  format */
	zend_declare_property_null(fastorm_db_result_ce, SL("formats"), ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_null(fastorm_db_result_ce, SL("rowClass"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_Result, __construct) {

	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *driver, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &driver);



	if (!(zephir_instance_of_ev(driver, fastorm_db_iresultdriver_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'driver' must be an instance of 'Fastorm\\Db\\IResultDriver'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("driver"), driver TSRMLS_CC);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "detecttypes", &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	zephir_update_property_this(this_ptr, SL("types"), _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	zephir_update_property_this(this_ptr, SL("formats"), _2 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_Result, getResource) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getresultresource", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Frees the resources allocated for this result set.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Result, free) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) != IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _1, "free", NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("meta"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Safe access to property driver.
 * @return IDibiResultDriver
 * @throws RuntimeException
 */
PHP_METHOD(Fastorm_Db_Result, getResultDriver) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_RuntimeException, "Result-set was released from memory.", "fastorm/db/Result.zep", 59);
		return;
	}
	RETURN_MEMBER(this_ptr, "driver");

}

/**
 * Moves cursor position without fetching row.
 * @param  int      the 0-based cursor pos to seek to
 * @return boolean  true on success, false if unable to seek to specified record
 * @throws DbException
 */
PHP_METHOD(Fastorm_Db_Result, seek) {

	zend_bool _0;
	zval *row_param = NULL, *_1, *_2 = NULL, *_3 = NULL, *_4;
	int row, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row_param);

	row = zephir_get_intval(row_param);


	_0 = row != 0;
	if (_0) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("fetched"), PH_NOISY_CC);
		_0 = zephir_is_true(_1);
	}
	if (_0) {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "getresultdriver",  NULL);
		zephir_check_call_status();
		ZEPHIR_INIT_VAR(_4);
		ZVAL_LONG(_4, row);
		ZEPHIR_CALL_METHOD(&_3, _2, "seek", NULL, _4);
		zephir_check_call_status();
		RETURN_MM_BOOL(zephir_get_boolval(_3));
	} else {
		RETURN_MM_BOOL(1);
	}

}

/**
 * Required by the Countable interface.
 * @return int
 */
PHP_METHOD(Fastorm_Db_Result, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Returns the number of rows in a result set.
 * @return int
 */
PHP_METHOD(Fastorm_Db_Result, getRowCount) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getrowcount", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Required by the IteratorAggregate interface.
 * @return ResultIterator
 */
PHP_METHOD(Fastorm_Db_Result, getIterator) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, fastorm_db_resultiterator_ce);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, this_ptr);
		zephir_check_call_status();
	}
	RETURN_MM();

}

/**
 * Fetches the row at current position, process optional type conversion.
 * and moves the internal cursor to the next position
 * @return DibiRow|false  array on success, false if no next record
 */
PHP_METHOD(Fastorm_Db_Result, fetchRow) {

	zend_class_entry *_4;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *row = NULL, *className, *_0 = NULL, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, _0, "fetchrow", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	if (!(Z_TYPE_P(row) == IS_ARRAY)) {
		RETURN_MM_BOOL(0);
	}
	zephir_update_property_this(this_ptr, SL("fetched"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "normalize", &_2, row);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(row, _1);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("rowClass"), PH_NOISY_CC);
	if (Z_TYPE_P(_3) != IS_NULL) {
		className = zephir_fetch_nproperty_this(this_ptr, SL("rowClass"), PH_NOISY_CC);
		ZEPHIR_INIT_BNVAR(row);
		_4 = zend_fetch_class(Z_STRVAL_P(className), Z_STRLEN_P(className), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
		object_init_ex(row, _4);
		if (zephir_has_constructor(row TSRMLS_CC)) {
			ZEPHIR_CALL_METHOD(NULL, row, "__construct", NULL, row);
			zephir_check_call_status();
		}
	}
	RETURN_CCTOR(row);

}

/**
 * Like fetchRow(), but returns only first field.
 * @return mixed  value on success, false if no next record
 */
PHP_METHOD(Fastorm_Db_Result, fetchSingle) {

	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *row = NULL, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getresultdriver",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, _0, "fetchrow", NULL, ZEPHIR_GLOBAL(global_true));
	zephir_check_call_status();
	if (!(Z_TYPE_P(row) == IS_ARRAY)) {
		RETURN_MM_BOOL(0);
	}
	zephir_update_property_this(this_ptr, SL("fetched"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "normalize", &_2, row);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(row, _1);
	Z_SET_ISREF_P(row);
	ZEPHIR_RETURN_CALL_FUNCTION("reset", &_3, row);
	Z_UNSET_ISREF_P(row);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Fetches all records from table like key => value pairs.
 * @param  string  associative key
 * @param  string  value
 * @return array
 * @throws InvalidArgumentException
 */
PHP_METHOD(Fastorm_Db_Result, fetchPairs) {

	zval *_9 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zephir_fcall_cache_entry *_3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *key = NULL, *value = NULL, *row = NULL, *tmp, *data, *_0, *_1 = NULL, *_2, *_5 = NULL, *_6 = NULL, *_7 = NULL, *_8 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &key, &value);

	if (!key) {
		ZEPHIR_CPY_WRT(key, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(key);
	}
	if (!value) {
		ZEPHIR_CPY_WRT(value, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(value);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "seek", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  NULL);
	zephir_check_call_status();
	if (!(zephir_is_true(row))) {
		array_init(return_value);
		RETURN_MM();
	}
	ZEPHIR_INIT_VAR(data);
	array_init(data);
	if (Z_TYPE_P(value) == IS_NULL) {
		if (Z_TYPE_P(key) != IS_NULL) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Either none or both columns must be specified.", "fastorm/db/Result.zep", 181);
			return;
		}
		ZEPHIR_INIT_VAR(tmp);
		ZEPHIR_CALL_METHOD(&_1, row, "toarray",  NULL);
		zephir_check_call_status();
		zephir_array_keys(tmp, _1 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(key);
		zephir_array_fetch_long(&key, tmp, 0, PH_NOISY, "fastorm/db/Result.zep", 186 TSRMLS_CC);
		if (zephir_fast_count_int(row TSRMLS_CC) < 2) {
			while (1) {
				zephir_array_fetch(&_2, row, key, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 189 TSRMLS_CC);
				zephir_array_append(&data, _2, PH_SEPARATE, "fastorm/db/Result.zep", 189);
				ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
				zephir_check_call_status();
				if (!(zephir_is_true(row))) {
					break;
				}
			}
			//missing empty
			RETURN_CCTOR(data);
		}
		ZEPHIR_OBS_NVAR(value);
		zephir_array_fetch_long(&value, tmp, 1, PH_NOISY, "fastorm/db/Result.zep", 198 TSRMLS_CC);
	} else {
		ZEPHIR_CALL_FUNCTION(&_1, "property_exists", &_4, row, value);
		zephir_check_call_status();
		if (!(zephir_is_true(_1))) {
			ZEPHIR_INIT_VAR(_5);
			object_init_ex(_5, spl_ce_InvalidArgumentException);
			ZEPHIR_INIT_VAR(_6);
			ZEPHIR_CONCAT_SVS(_6, "Unknown value column '", value, "'.");
			ZEPHIR_CALL_METHOD(NULL, _5, "__construct", NULL, _6);
			zephir_check_call_status();
			zephir_throw_exception_debug(_5, "fastorm/db/Result.zep", 202 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		if (Z_TYPE_P(key) == IS_NULL) {
			while (1) {
				zephir_array_fetch(&_2, row, value, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 207 TSRMLS_CC);
				zephir_array_append(&data, _2, PH_SEPARATE, "fastorm/db/Result.zep", 207);
				ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
				zephir_check_call_status();
				if (!(zephir_is_true(row))) {
					break;
				}
			}
			RETURN_CCTOR(data);
		}
		ZEPHIR_CALL_FUNCTION(&_7, "property_exists", &_4, row, key);
		zephir_check_call_status();
		if (!(zephir_is_true(_7))) {
			ZEPHIR_INIT_LNVAR(_5);
			object_init_ex(_5, spl_ce_InvalidArgumentException);
			ZEPHIR_INIT_LNVAR(_6);
			ZEPHIR_CONCAT_SVS(_6, "Unknown key column '", key, "'.");
			ZEPHIR_CALL_METHOD(NULL, _5, "__construct", NULL, _6);
			zephir_check_call_status();
			zephir_throw_exception_debug(_5, "fastorm/db/Result.zep", 217 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	while (1) {
		zephir_array_fetch(&_2, row, value, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 222 TSRMLS_CC);
		ZEPHIR_OBS_NVAR(_8);
		zephir_array_fetch(&_8, row, key, PH_NOISY, "fastorm/db/Result.zep", 222 TSRMLS_CC);
		zephir_get_strval(_9, _8);
		zephir_array_update_zval(&data, _9, &_2, PH_COPY | PH_SEPARATE);
		ZEPHIR_CALL_METHOD(&row, this_ptr, "fetchrow",  &_3);
		zephir_check_call_status();
		if (!(zephir_is_true(row))) {
			break;
		}
	}
	//missing empty
	RETURN_CCTOR(data);

}

/**
 * Autodetect column types.
 * @return void
 */
PHP_METHOD(Fastorm_Db_Result, detectTypes) {

	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *cache = NULL, *columns = NULL, *col = NULL, *nativetype, *e = NULL, *_1 = NULL, **_4, *_5 = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_CE_STATIC(&cache, fastorm_db_columninfo_ce, "gettypecache", &_0);
	zephir_check_call_status();

	/* try_start_1: */

		ZEPHIR_CALL_METHOD(&_1, this_ptr, "getresultdriver",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		ZEPHIR_CALL_METHOD(&columns, _1, "getresultcolumns",  NULL);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_is_iterable(columns, &_3, &_2, 0, 0, "fastorm/db/Result.zep", 249);
		for (
		  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
		  ; zephir_hash_move_forward_ex(_3, &_2)
		) {
			ZEPHIR_GET_HVALUE(col, _4);
			zephir_array_fetch_string(&nativetype, col, SL("nativetype"), PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 246 TSRMLS_CC);
			ZEPHIR_OBS_NVAR(_5);
			zephir_read_property_zval(&_5, cache, nativetype, PH_NOISY_CC);
			ZEPHIR_OBS_NVAR(_6);
			zephir_array_fetch_string(&_6, col, SL("name"), PH_NOISY, "fastorm/db/Result.zep", 247 TSRMLS_CC);
			zephir_update_property_array(this_ptr, SL("types"), _6, _5 TSRMLS_CC);
		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_is_instance_of(e, SL("DbException") TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_NULL();
		}
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * Converts values to specified type and format.
 * @param  array
 * @return void
 */
PHP_METHOD(Fastorm_Db_Result, normalize) {

	zephir_fcall_cache_entry *_20 = NULL, *_26 = NULL, *_27 = NULL;
	zend_class_entry *_19;
	zval *_8 = NULL, *_17 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL, *_18 = NULL;
	zend_bool _5, _15, _16;
	HashTable *_3;
	HashPosition _2;
	zval *row_param = NULL, *key = NULL, *type = NULL, *value = NULL, *tmp = NULL, *left = NULL, *right = NULL, *_0, *_1, **_4, *_6 = NULL, _9 = zval_used_for_init, *_10 = NULL, *_11 = NULL, _12 = zval_used_for_init, _13 = zval_used_for_init, _14 = zval_used_for_init, *_21, *_22, *_23 = NULL, *_24, *_25, *_28 = NULL;
	zval *row = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &row_param);

	zephir_get_arrval(row, row_param);
	ZEPHIR_SEPARATE_PARAM(row);


	ZEPHIR_INIT_VAR(_0);
	zephir_gettype(_0, row TSRMLS_CC);
	if (!ZEPHIR_IS_STRING_IDENTICAL(_0, "array")) {
		ZEPHIR_INIT_NVAR(row);
		array_init(row);
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("types"), PH_NOISY_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/db/Result.zep", 315);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(key, _3, _2);
		ZEPHIR_GET_HVALUE(type, _4);
		if (!(zephir_array_isset(row, key))) {
			continue;
		}
		ZEPHIR_OBS_NVAR(value);
		zephir_array_fetch(&value, row, key, PH_NOISY, "fastorm/db/Result.zep", 273 TSRMLS_CC);
		_5 = ZEPHIR_IS_FALSE_IDENTICAL(value);
		if (!(_5)) {
			_5 = ZEPHIR_IS_STRING_IDENTICAL(type, "s");
		}
		if (_5) {
			continue;
		}
		if (ZEPHIR_IS_STRING_IDENTICAL(type, "i")) {
			ZEPHIR_INIT_NVAR(tmp);
			ZVAL_LONG(tmp, (zephir_get_numberval(value) * 1));
			ZEPHIR_CALL_FUNCTION(&_6, "is_float", &_7, tmp);
			zephir_check_call_status();
			if (zephir_is_true(_6)) {
				zephir_array_update_zval(&row, key, &value, PH_COPY | PH_SEPARATE);
			} else {
				zephir_array_update_zval(&row, key, &tmp, PH_COPY | PH_SEPARATE);
			}
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(type, "f")) {
				ZEPHIR_INIT_NVAR(tmp);
				ZVAL_DOUBLE(tmp, zephir_get_doubleval(value));
				ZEPHIR_INIT_NVAR(left);
				zephir_get_strval(_8, tmp);
				ZEPHIR_SINIT_NVAR(_9);
				ZVAL_LONG(&_9, 0);
				zephir_fast_trim(left, _8, &_9, ZEPHIR_TRIM_LEFT TSRMLS_CC);
				ZEPHIR_INIT_NVAR(right);
				ZEPHIR_INIT_NVAR(_10);
				ZEPHIR_INIT_NVAR(_11);
				ZEPHIR_SINIT_NVAR(_12);
				ZVAL_LONG(&_12, 0);
				zephir_fast_trim(_11, value, &_12, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_13);
				ZVAL_STRING(&_13, ".", 0);
				zephir_fast_trim(_10, _11, &_13, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_14);
				ZVAL_LONG(&_14, 0);
				zephir_fast_trim(right, _10, &_14, ZEPHIR_TRIM_LEFT TSRMLS_CC);
				if (ZEPHIR_IS_IDENTICAL(left, right)) {
					zephir_array_update_zval(&row, key, &tmp, PH_COPY | PH_SEPARATE);
				} else {
					zephir_array_update_zval(&row, key, &value, PH_COPY | PH_SEPARATE);
				}
			} else {
				if (ZEPHIR_IS_STRING_IDENTICAL(type, "b")) {
					_15 = zephir_get_boolval(value);
					if (_15) {
						_15 = !ZEPHIR_IS_STRING_IDENTICAL(value, "f");
					}
					_16 = _15;
					if (_16) {
						_16 = !ZEPHIR_IS_STRING_IDENTICAL(value, "F");
					}
					ZEPHIR_INIT_NVAR(_10);
					ZVAL_BOOL(_10, _16);
					zephir_array_update_zval(&row, key, &_10, PH_COPY | PH_SEPARATE);
				} else {
					_15 = ZEPHIR_IS_STRING_IDENTICAL(type, "d");
					if (!(_15)) {
						_15 = ZEPHIR_IS_STRING_IDENTICAL(type, "t");
					}
					if (_15) {
						_16 = zephir_get_intval(value) != 0;
						if (!(_16)) {
							zephir_get_strval(_17, value);
							ZEPHIR_SINIT_NVAR(_9);
							ZVAL_LONG(&_9, 0);
							ZEPHIR_SINIT_NVAR(_12);
							ZVAL_LONG(&_12, 3);
							ZEPHIR_CALL_FUNCTION(&_6, "substr", &_18, _17, &_9, &_12);
							zephir_check_call_status();
							_16 = ZEPHIR_IS_STRING_IDENTICAL(_6, "00:");
						}
						if (_16) {
							ZEPHIR_INIT_NVAR(value);
							_19 = zend_fetch_class(SL("DateTime"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
							object_init_ex(value, _19);
							ZEPHIR_CALL_METHOD(NULL, value, "__construct", &_20, value);
							zephir_check_call_status();
							_21 = zephir_fetch_nproperty_this(this_ptr, SL("formats"), PH_NOISY_CC);
							zephir_array_fetch(&_22, _21, type, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 304 TSRMLS_CC);
							if (ZEPHIR_IS_EMPTY(_22)) {
								zephir_array_update_zval(&row, key, &value, PH_COPY | PH_SEPARATE);
							} else {
								_24 = zephir_fetch_nproperty_this(this_ptr, SL("formats"), PH_NOISY_CC);
								zephir_array_fetch(&_25, _24, type, PH_NOISY | PH_READONLY, "fastorm/db/Result.zep", 307 TSRMLS_CC);
								ZEPHIR_CALL_METHOD(&_23, value, "format", &_26, _25);
								zephir_check_call_status();
								zephir_array_update_zval(&row, key, &_23, PH_COPY | PH_SEPARATE);
							}
						}
					} else {
						if (ZEPHIR_IS_STRING_IDENTICAL(type, "bin")) {
							ZEPHIR_CALL_METHOD(&_23, this_ptr, "getresultdriver",  &_27);
							zephir_check_call_status();
							ZEPHIR_CALL_METHOD(&_28, _23, "unescape", NULL, value, type);
							zephir_check_call_status();
							zephir_array_update_zval(&row, key, &_28, PH_COPY | PH_SEPARATE);
						}
					}
				}
			}
		}
	}
	RETURN_CTOR(row);

}

/**
 * Define column type.
 * @param  string  column
 * @param  string  type (use constant Dibi::*)
 * @return self
 */
PHP_METHOD(Fastorm_Db_Result, setType) {

	zval *col, *type;

	zephir_fetch_params(0, 2, 0, &col, &type);



	zephir_update_property_array(this_ptr, SL("types"), col, type TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns column type.
 * @return string
 */
PHP_METHOD(Fastorm_Db_Result, getType) {

	zval *col, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &col);



	ZEPHIR_INIT_VAR(_0);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("types"), PH_NOISY_CC);
	if (zephir_array_isset(_1, col)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("types"), PH_NOISY_CC);
		zephir_array_fetch(&_0, _2, col, PH_NOISY, "fastorm/db/Result.zep", 338 TSRMLS_CC);
	} else {
		ZVAL_NULL(_0);
	}
	RETURN_CCTOR(_0);

}

/**
 * Sets data format.
 * @param  string  type (use constant Dibi::*)
 * @param  string  format
 * @return self
 */
PHP_METHOD(Fastorm_Db_Result, setFormat) {

	zval *type, *format;

	zephir_fetch_params(0, 2, 0, &type, &format);



	zephir_update_property_array(this_ptr, SL("formats"), type, format TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns data format.
 * @return string
 */
PHP_METHOD(Fastorm_Db_Result, getFormat) {

	zval *type, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type);



	ZEPHIR_INIT_VAR(_0);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("formats"), PH_NOISY_CC);
	if (zephir_array_isset(_1, type)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("formats"), PH_NOISY_CC);
		zephir_array_fetch(&_0, _2, type, PH_NOISY, "fastorm/db/Result.zep", 361 TSRMLS_CC);
	} else {
		ZVAL_NULL(_0);
	}
	RETURN_CCTOR(_0);

}

/**
 * Returns a meta information about the current result set.
 * @return DibiResultInfo
 */
PHP_METHOD(Fastorm_Db_Result, getInfo) {

	zval *_0;


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("meta"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
	}
	RETURN_MEMBER(this_ptr, "meta");

}

