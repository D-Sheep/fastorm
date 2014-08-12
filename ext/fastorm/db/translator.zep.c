
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/hash.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_Translator) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Translator, fastorm, db_translator, fastorm_db_translator_method_entry, 0);

	/** @var Connection */
	zend_declare_property_null(fastorm_db_translator_ce, SL("connection"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var IDibiDriver */
	zend_declare_property_null(fastorm_db_translator_ce, SL("driver"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("cursor"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var array */
	zend_declare_property_null(fastorm_db_translator_ce, SL("args"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_null(fastorm_db_translator_ce, SL("hasError"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var bool */
	zend_declare_property_null(fastorm_db_translator_ce, SL("comment"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("ifLevel"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("ifLevelStart"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("limit"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/** @var int */
	zend_declare_property_null(fastorm_db_translator_ce, SL("offset"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_Translator, __construct) {

	zval *connection;

	zephir_fetch_params(0, 1, 0, &connection);



	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);

}

/**
 * Generates SQL.
 * @param  array
 * @return string
 * @throws DbException
 */
PHP_METHOD(Fastorm_Db_Translator, translate) {

	double _30;
	zephir_fcall_cache_entry *_24 = NULL, *_26 = NULL, *_31 = NULL, *_36 = NULL;
	zval *_15 = NULL;
	zend_bool _4, _29;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_11 = NULL, *_14 = NULL, *_19 = NULL, *_22 = NULL, *_27 = NULL;
	zval *ret = NULL, *_34 = NULL;
	int toSkip, lastArr, ZEPHIR_LAST_CALL_STATUS;
	zval *args = NULL, *commandIns = NULL, *arg = NULL, *sql, *_0, *_1, *_2 = NULL, *_5, *_6, *_7, *_8, *_9 = NULL, _10 = zval_used_for_init, _12 = zval_used_for_init, *_13 = NULL, *_16 = NULL, *_17 = NULL, *_18 = NULL, *_20 = NULL, *_21 = NULL, *_23 = NULL, *_25, *_28, *_32 = NULL, *_33 = NULL, *_35, *_37;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &args);

	ZEPHIR_SEPARATE_PARAM(args);


	ZEPHIR_INIT_VAR(sql);
	array_init(sql);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_0) == IS_NULL) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_2, _1, "getdriver",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), _2 TSRMLS_CC);
	}
	if (Z_TYPE_P(args) != IS_ARRAY) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "Input of translate function must be array", "fastorm/db/Translator.zep", 63);
		return;
	}
	ZEPHIR_CALL_FUNCTION(&_2, "array_values", &_3, args);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(args, _2);
	while (1) {
		_4 = zephir_fast_count_int(args TSRMLS_CC) == 1;
		if (_4) {
			zephir_array_fetch_long(&_5, args, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 68 TSRMLS_CC);
			_4 = Z_TYPE_P(_5) == IS_ARRAY;
		}
		if (!(_4)) {
			break;
		}
		zephir_array_fetch_long(&_6, args, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 69 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&args, "array_values", &_3, _6);
		zephir_check_call_status();
	}
	zephir_update_property_this(this_ptr, SL("args"), args TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, -1);
	zephir_update_property_this(this_ptr, SL("limit"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("offset"), _1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("hasError"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_VAR(commandIns);
	ZVAL_NULL(commandIns);
	lastArr = 0;
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("cursor"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("ifLevel"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 0);
	zephir_update_property_this(this_ptr, SL("ifLevelStart"), _1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	while (1) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		if (!(ZEPHIR_LT_LONG(_1, zephir_fast_count_int(_7 TSRMLS_CC)))) {
			break;
		}
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(arg);
		ZEPHIR_OBS_NVAR(_9);
		zephir_read_property_this(&_9, this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_array_fetch(&arg, _8, _9, PH_NOISY, "fastorm/db/Translator.zep", 87 TSRMLS_CC);
		RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
		if (Z_TYPE_P(arg) == IS_STRING) {
			ZEPHIR_SINIT_NVAR(_10);
			ZVAL_STRING(&_10, "`['\":%?", 0);
			ZEPHIR_CALL_FUNCTION(&_2, "strcspn", &_11, arg, &_10);
			zephir_check_call_status();
			toSkip = zephir_get_intval(_2);
			if (zephir_fast_strlen_ev(arg) == toSkip) {
				zephir_array_append(&sql, arg, PH_SEPARATE, "fastorm/db/Translator.zep", 96);
			} else {
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_LONG(&_10, 0);
				ZEPHIR_SINIT_NVAR(_12);
				ZVAL_LONG(&_12, toSkip);
				ZEPHIR_CALL_FUNCTION(&_13, "substr", &_14, arg, &_10, &_12);
				zephir_check_call_status();
				ZEPHIR_INIT_NVAR(_15);
				array_init_size(_15, 3);
				zephir_array_fast_append(_15, this_ptr);
				ZEPHIR_INIT_NVAR(_16);
				ZVAL_STRING(_16, "cb", 1);
				zephir_array_fast_append(_15, _16);
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_LONG(&_10, toSkip);
				ZEPHIR_CALL_FUNCTION(&_17, "substr", &_14, arg, &_10);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_10);
				ZVAL_STRING(&_10, "/(?=[`['\":%?])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?)|%([a-zA-Z~][a-zA-Z0-9~]{0,5})|(\\?))/s", 0);
				ZEPHIR_CALL_FUNCTION(&_18, "preg_replace_callback", &_19, &_10, _15, _17);
				zephir_check_call_status();
				ZEPHIR_INIT_LNVAR(_20);
				ZEPHIR_CONCAT_VV(_20, _13, _18);
				zephir_array_append(&sql, _20, PH_SEPARATE, "fastorm/db/Translator.zep", 117);
				ZEPHIR_CALL_FUNCTION(&_21, "preg_last_error", &_22);
				zephir_check_call_status();
				if (zephir_is_true(_21)) {
					ZEPHIR_INIT_LNVAR(_23);
					object_init_ex(_23, fastorm_exception_ce);
					ZEPHIR_CALL_METHOD(NULL, _23, "__construct", &_24);
					zephir_check_call_status();
					zephir_throw_exception_debug(_23, "fastorm/db/Translator.zep", 119 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			}
			continue;
		}
		_25 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
		if (zephir_is_true(_25)) {
			ZEPHIR_INIT_NVAR(_16);
			ZVAL_STRING(_16, "...", 1);
			zephir_array_append(&sql, _16, PH_SEPARATE, "fastorm/db/Translator.zep", 126);
			continue;
		}
		if (zephir_is_instance_of(arg, SL("Traversable") TSRMLS_CC)) {
			ZEPHIR_CALL_FUNCTION(&_13, "iterator_to_array", &_26, arg);
			zephir_check_call_status();
			ZEPHIR_CPY_WRT(arg, _13);
		}
		if (Z_TYPE_P(arg) == IS_ARRAY) {
			Z_SET_ISREF_P(arg);
			ZEPHIR_CALL_FUNCTION(&_13, "key", &_27, arg);
			Z_UNSET_ISREF_P(arg);
			zephir_check_call_status();
			if (Z_TYPE_P(_13) == IS_STRING) {
				if (Z_TYPE_P(commandIns) == IS_NULL) {
					ZEPHIR_INIT_NVAR(commandIns);
					ZEPHIR_INIT_NVAR(_16);
					_28 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
					zephir_array_fetch_long(&_6, _28, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 138 TSRMLS_CC);
					zephir_fast_trim(_16, _6, NULL , ZEPHIR_TRIM_LEFT TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_10);
					ZVAL_LONG(&_10, 0);
					ZEPHIR_SINIT_NVAR(_12);
					ZVAL_LONG(&_12, 6);
					ZEPHIR_CALL_FUNCTION(&_17, "substr", &_14, _16, &_10, &_12);
					zephir_check_call_status();
					zephir_fast_strtoupper(commandIns, _17);
					_29 = ZEPHIR_IS_STRING_IDENTICAL(commandIns, "INSERT");
					if (!(_29)) {
						_29 = ZEPHIR_IS_STRING_IDENTICAL(commandIns, "REPLAC");
					}
					_30 = _29;
					ZEPHIR_INIT_NVAR(commandIns);
					ZVAL_BOOL(commandIns, _30);
					ZEPHIR_INIT_LNVAR(_20);
					if (zephir_is_true(commandIns)) {
						ZEPHIR_INIT_NVAR(_20);
						ZVAL_STRING(_20, "v", 1);
					} else {
						ZEPHIR_INIT_NVAR(_20);
						ZVAL_STRING(_20, "a", 1);
					}
					ZEPHIR_CALL_METHOD(&_18, this_ptr, "formatvalue", &_31, arg, _20);
					zephir_check_call_status();
					zephir_array_append(&sql, _18, PH_SEPARATE, "fastorm/db/Translator.zep", 140);
				} else {
					_28 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
					if (lastArr == (zephir_get_numberval(_28) - 1)) {
						ZEPHIR_INIT_NVAR(_32);
						ZVAL_STRING(_32, ",", 1);
						zephir_array_append(&sql, _32, PH_SEPARATE, "fastorm/db/Translator.zep", 143);
					}
					ZEPHIR_INIT_LNVAR(_23);
					if (zephir_is_true(commandIns)) {
						ZEPHIR_INIT_NVAR(_23);
						ZVAL_STRING(_23, "l", 1);
					} else {
						ZEPHIR_INIT_NVAR(_23);
						ZVAL_STRING(_23, "a", 1);
					}
					ZEPHIR_CALL_METHOD(&_21, this_ptr, "formatvalue", &_31, arg, _23);
					zephir_check_call_status();
					zephir_array_append(&sql, _21, PH_SEPARATE, "fastorm/db/Translator.zep", 145);
				}
				ZEPHIR_OBS_NVAR(_33);
				zephir_read_property_this(&_33, this_ptr, SL("cursor"), PH_NOISY_CC);
				lastArr = zephir_get_intval(_33);
				continue;
			}
		}
		ZEPHIR_CALL_METHOD(&_13, this_ptr, "formatvalue", &_31, arg, ZEPHIR_GLOBAL(global_false));
		zephir_check_call_status();
		zephir_array_append(&sql, _13, PH_SEPARATE, "fastorm/db/Translator.zep", 153);
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_1)) {
		ZEPHIR_INIT_NVAR(_16);
		ZVAL_STRING(_16, "*/", 1);
		zephir_array_append(&sql, _16, PH_SEPARATE, "fastorm/db/Translator.zep", 158);
	}
	ZEPHIR_INIT_NVAR(_16);
	zephir_fast_join_str(_16, SL(" "), sql TSRMLS_CC);
	zephir_get_strval(_34, _16);
	ZEPHIR_CPY_WRT(ret, _34);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("hasError"), PH_NOISY_CC);
	if (zephir_is_true(_1)) {
		ZEPHIR_INIT_LNVAR(_20);
		object_init_ex(_20, fastorm_db_dbexception_ce);
		ZEPHIR_INIT_NVAR(_32);
		ZVAL_STRING(_32, "SQL translate error", 0);
		ZEPHIR_INIT_VAR(_35);
		ZVAL_LONG(_35, 0);
		ZEPHIR_CALL_METHOD(NULL, _20, "__construct", &_36, _32, _35, ZEPHIR_GLOBAL(global_null), ret, args);
		zephir_check_temp_parameter(_32);
		zephir_check_call_status();
		zephir_throw_exception_debug(_20, "fastorm/db/Translator.zep", 164 TSRMLS_CC);
		ZEPHIR_MM_RESTORE();
		return;
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("limit"), PH_NOISY_CC);
	_29 = ZEPHIR_GT_LONG(_1, -1);
	if (!(_29)) {
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("offset"), PH_NOISY_CC);
		_29 = ZEPHIR_GT_LONG(_8, 0);
	}
	if (_29) {
		_25 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		_28 = zephir_fetch_nproperty_this(this_ptr, SL("limit"), PH_NOISY_CC);
		_37 = zephir_fetch_nproperty_this(this_ptr, SL("offset"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(NULL, _25, "applylimit", NULL, ret, _28, _37);
		zephir_check_call_status();
	}
	RETURN_CTOR(ret);

}

/**
 * Apply modifier to single value.
 * @param  mixed
 * @param  string
 * @return string
 */
PHP_METHOD(Fastorm_Db_Translator, formatValue) {

	zval *_77;
	zend_class_entry *_73;
	zephir_nts_static zephir_fcall_cache_entry *_31 = NULL, *_57 = NULL, *_62 = NULL, *_63 = NULL, *_65 = NULL, *_71 = NULL, *_75 = NULL, *_76 = NULL, *_79 = NULL, *_81 = NULL, *_84 = NULL, *_86 = NULL;
	zend_bool _17, declared, _55, _58, _64, _67;
	zephir_fcall_cache_entry *_8 = NULL, *_10 = NULL;
	HashTable *_5, *_23, *_27, *_34, *_37, *_41, *_44, *_48, *_53, *_60;
	HashPosition _4, _22, _26, _33, _36, _40, _43, _47, _52, _59;
	int ZEPHIR_LAST_CALL_STATUS, _72, toSkip;
	zval *modifier = NULL, *op = NULL, *_66 = NULL, *_74 = NULL, *_82 = NULL, *_85 = NULL;
	zval *value = NULL, *modifier_param = NULL, *k = NULL, *v = NULL, *pair = NULL, *vx, *kx, *proto = NULL, *_0, *_1, *_2, *_3 = NULL, **_6, *_7, *_9 = NULL, *_11 = NULL, *_12, *_13 = NULL, *_14 = NULL, *_15, *_16, *_18, *_19, _20 = zval_used_for_init, *_21 = NULL, **_24, *_25 = NULL, **_28, *_29 = NULL, *_30 = NULL, *_32 = NULL, **_35, **_38, *_39 = NULL, **_42, *k2 = NULL, *v2 = NULL, **_45, *_46 = NULL, **_49, *_50 = NULL, *_51, **_54, _56 = zval_used_for_init, *translator, **_61, _68 = zval_used_for_init, _69 = zval_used_for_init, _70, *_78 = NULL, *_80 = NULL, *_83;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &value, &modifier_param);

	ZEPHIR_SEPARATE_PARAM(value);
	if (!modifier_param) {
		ZEPHIR_INIT_VAR(modifier);
		ZVAL_EMPTY_STRING(modifier);
	} else {
		zephir_get_strval(modifier, modifier_param);
	}


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		RETURN_MM_STRING("...", 1);
	}
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
	if (Z_TYPE_P(_1) == IS_NULL) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_CALL_METHOD(&_3, _2, "getdriver",  NULL);
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("driver"), _3 TSRMLS_CC);
	}
	if (zephir_is_instance_of(value, SL("Traversable") TSRMLS_CC)) {
		ZEPHIR_CALL_FUNCTION(&_3, "iterator_to_array", NULL, value);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(value, _3);
	}
	if (Z_TYPE_P(value) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(vx);
		array_init(vx);
		ZEPHIR_INIT_VAR(kx);
		array_init(kx);
		do {
			if (ZEPHIR_IS_STRING(modifier, "and") || ZEPHIR_IS_STRING(modifier, "or")) {
				if (ZEPHIR_IS_EMPTY(value)) {
					RETURN_MM_STRING("1=1", 1);
				}
				zephir_is_iterable(value, &_5, &_4, 0, 0, "fastorm/db/Translator.zep", 246);
				for (
				  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
				  ; zephir_hash_move_forward_ex(_5, &_4)
				) {
					ZEPHIR_GET_HMKEY(k, _5, _4);
					ZEPHIR_GET_HVALUE(v, _6);
					if (Z_TYPE_P(k) == IS_STRING) {
						ZEPHIR_INIT_NVAR(pair);
						zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
						zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 216 TSRMLS_CC);
						ZEPHIR_CALL_METHOD(&_3, this_ptr, "delimite", &_8, _7);
						zephir_check_call_status();
						ZEPHIR_INIT_NVAR(k);
						ZEPHIR_CONCAT_VS(k, _3, " ");
						if (!(zephir_array_isset_long(pair, 1))) {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v);
							zephir_check_call_status();
							ZEPHIR_CPY_WRT(v, _9);
							if (ZEPHIR_IS_STRING_IDENTICAL(v, "null")) {
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VSV(_11, k, "IS ", v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 220);
							} else {
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VSV(_11, k, "= ", v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 222);
							}
						} else {
							zephir_array_fetch_long(&_12, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 225 TSRMLS_CC);
							if (ZEPHIR_IS_STRING_IDENTICAL(_12, "ex")) {
								ZEPHIR_INIT_NVAR(_13);
								ZVAL_STRING(_13, "ex", 0);
								ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, _13);
								zephir_check_temp_parameter(_13);
								zephir_check_call_status();
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VV(_11, k, _9);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 226);
							} else {
								zephir_array_fetch_long(&_15, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 229 TSRMLS_CC);
								ZEPHIR_CALL_METHOD(&_14, this_ptr, "formatvalue", &_10, v, _15);
								zephir_check_call_status();
								ZEPHIR_CPY_WRT(v, _14);
								zephir_array_fetch_long(&_16, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 230 TSRMLS_CC);
								_17 = ZEPHIR_IS_STRING_IDENTICAL(_16, "l");
								if (!(_17)) {
									zephir_array_fetch_long(&_18, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 230 TSRMLS_CC);
									_17 = ZEPHIR_IS_STRING_IDENTICAL(_18, "in");
								}
								if (_17) {
									ZEPHIR_INIT_NVAR(op);
									ZVAL_STRING(op, "IN ", 1);
								} else {
									zephir_array_fetch_long(&_19, pair, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 232 TSRMLS_CC);
									ZEPHIR_SINIT_NVAR(_20);
									ZVAL_STRING(&_20, "like", 0);
									ZEPHIR_INIT_NVAR(_13);
									zephir_fast_strpos(_13, _19, &_20, 0 );
									if (!ZEPHIR_IS_FALSE_IDENTICAL(_13)) {
										ZEPHIR_INIT_NVAR(op);
										ZVAL_STRING(op, "LIKE ", 1);
									} else {
										if (ZEPHIR_IS_STRING_IDENTICAL(v, "null")) {
											ZEPHIR_INIT_NVAR(op);
											ZVAL_STRING(op, "IS ", 1);
										} else {
											ZEPHIR_INIT_NVAR(op);
											ZVAL_STRING(op, "= ", 1);
										}
									}
								}
								ZEPHIR_INIT_LNVAR(_11);
								ZEPHIR_CONCAT_VVV(_11, k, op, v);
								zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 239);
							}
						}
					} else {
						ZEPHIR_INIT_NVAR(_13);
						ZVAL_STRING(_13, "ex", 0);
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, _13);
						zephir_check_temp_parameter(_13);
						zephir_check_call_status();
						zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 243);
					}
				}
				ZEPHIR_INIT_NVAR(_13);
				ZEPHIR_INIT_VAR(_21);
				zephir_fast_strtoupper(_21, modifier);
				ZEPHIR_INIT_LNVAR(_11);
				ZEPHIR_CONCAT_SVS(_11, ") ", _21, " (");
				zephir_fast_join(_13, _11, vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVS(return_value, "(", _13, ")");
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "n")) {
				zephir_is_iterable(value, &_23, &_22, 0, 0, "fastorm/db/Translator.zep", 263);
				for (
				  ; zephir_hash_get_current_data_ex(_23, (void**) &_24, &_22) == SUCCESS
				  ; zephir_hash_move_forward_ex(_23, &_22)
				) {
					ZEPHIR_GET_HMKEY(k, _23, _22);
					ZEPHIR_GET_HVALUE(v, _24);
					if (Z_TYPE_P(k) == IS_STRING) {
						if (ZEPHIR_IS_EMPTY(v)) {
							ZEPHIR_INIT_NVAR(op);
							ZVAL_EMPTY_STRING(op);
						} else {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, v);
							zephir_check_call_status();
							ZEPHIR_INIT_LNVAR(_25);
							ZEPHIR_CONCAT_SV(_25, " AS ", _9);
							zephir_get_strval(op, _25);
						}
						ZEPHIR_INIT_LNVAR(_25);
						ZEPHIR_CONCAT_VV(_25, k, op);
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, _25);
						zephir_check_call_status();
						zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 257);
					} else {
						ZEPHIR_INIT_NVAR(pair);
						zephir_fast_explode_str(pair, SL("%"), v, 2  TSRMLS_CC);
						zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 260 TSRMLS_CC);
						ZEPHIR_CALL_METHOD(&_14, this_ptr, "delimite", &_8, _7);
						zephir_check_call_status();
						zephir_array_append(&vx, _14, PH_SEPARATE, "fastorm/db/Translator.zep", 260);
					}
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "a")) {
				zephir_is_iterable(value, &_27, &_26, 0, 0, "fastorm/db/Translator.zep", 271);
				for (
				  ; zephir_hash_get_current_data_ex(_27, (void**) &_28, &_26) == SUCCESS
				  ; zephir_hash_move_forward_ex(_27, &_26)
				) {
					ZEPHIR_GET_HMKEY(k, _27, _26);
					ZEPHIR_GET_HVALUE(v, _28);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 269 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "delimite", &_8, _7);
					zephir_check_call_status();
					ZEPHIR_CALL_METHOD(&_30, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_32);
					ZEPHIR_CONCAT_VSV(_32, _29, "=", _30);
					zephir_array_append(&vx, _32, PH_SEPARATE, "fastorm/db/Translator.zep", 269);
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "in") || ZEPHIR_IS_STRING(modifier, "l")) {
				zephir_is_iterable(value, &_34, &_33, 0, 0, "fastorm/db/Translator.zep", 280);
				for (
				  ; zephir_hash_get_current_data_ex(_34, (void**) &_35, &_33) == SUCCESS
				  ; zephir_hash_move_forward_ex(_34, &_33)
				) {
					ZEPHIR_GET_HMKEY(k, _34, _33);
					ZEPHIR_GET_HVALUE(v, _35);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					zephir_array_append(&vx, _29, PH_SEPARATE, "fastorm/db/Translator.zep", 278);
				}
				_17 = zephir_is_true(vx);
				if (!(_17)) {
					_17 = ZEPHIR_IS_STRING_IDENTICAL(modifier, "l");
				}
				if (_17) {
					ZEPHIR_INIT_NVAR(_21);
					zephir_fast_join_str(_21, SL(", "), vx TSRMLS_CC);
					ZEPHIR_CONCAT_SVS(return_value, "(", _21, ")");
					RETURN_MM();
				} else {
					RETURN_MM_STRING("(null)", 1);
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "v")) {
				zephir_is_iterable(value, &_37, &_36, 0, 0, "fastorm/db/Translator.zep", 292);
				for (
				  ; zephir_hash_get_current_data_ex(_37, (void**) &_38, &_36) == SUCCESS
				  ; zephir_hash_move_forward_ex(_37, &_36)
				) {
					ZEPHIR_GET_HMKEY(k, _37, _36);
					ZEPHIR_GET_HVALUE(v, _38);
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_12, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 289 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_29, this_ptr, "delimite", &_8, _12);
					zephir_check_call_status();
					zephir_array_append(&kx, _29, PH_SEPARATE, "fastorm/db/Translator.zep", 289);
					ZEPHIR_CALL_METHOD(&_30, this_ptr, "fomattedpairvalue", &_31, pair, v);
					zephir_check_call_status();
					zephir_array_append(&vx, _30, PH_SEPARATE, "fastorm/db/Translator.zep", 290);
				}
				ZEPHIR_INIT_NVAR(_21);
				zephir_fast_join_str(_21, SL(", "), kx TSRMLS_CC);
				ZEPHIR_INIT_VAR(_39);
				zephir_fast_join_str(_39, SL(", "), vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVSVS(return_value, "(", _21, ") VALUES (", _39, ")");
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "m")) {
				declared = 0;
				ZEPHIR_INIT_VAR(proto);
				ZVAL_NULL(proto);
				zephir_is_iterable(value, &_41, &_40, 0, 0, "fastorm/db/Translator.zep", 326);
				for (
				  ; zephir_hash_get_current_data_ex(_41, (void**) &_42, &_40) == SUCCESS
				  ; zephir_hash_move_forward_ex(_41, &_40)
				) {
					ZEPHIR_GET_HMKEY(k, _41, _40);
					ZEPHIR_GET_HVALUE(v, _42);
					if (Z_TYPE_P(v) == IS_ARRAY) {
						if (declared) {
							ZEPHIR_INIT_NVAR(_21);
							zephir_array_keys(_21, v TSRMLS_CC);
							if (!ZEPHIR_IS_IDENTICAL(proto, _21)) {
								zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
								ZEPHIR_CONCAT_SVS(return_value, "**Multi-insert array '", k, "' is different.**");
								RETURN_MM();
							}
						} else {
							ZEPHIR_INIT_NVAR(proto);
							zephir_array_keys(proto, v TSRMLS_CC);
							declared = 1;
						}
					} else {
						zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						ZEPHIR_INIT_NVAR(_13);
						zephir_gettype(_13, v TSRMLS_CC);
						ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _13, "**");
						RETURN_MM();
					}
					ZEPHIR_INIT_NVAR(pair);
					zephir_fast_explode_str(pair, SL("%"), k, 2  TSRMLS_CC);
					zephir_array_fetch_long(&_7, pair, 0, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 315 TSRMLS_CC);
					ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, _7);
					zephir_check_call_status();
					zephir_array_append(&kx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 315);
					zephir_is_iterable(v, &_44, &_43, 0, 0, "fastorm/db/Translator.zep", 325);
					for (
					  ; zephir_hash_get_current_data_ex(_44, (void**) &_45, &_43) == SUCCESS
					  ; zephir_hash_move_forward_ex(_44, &_43)
					) {
						ZEPHIR_GET_HMKEY(k2, _44, _43);
						ZEPHIR_GET_HVALUE(v2, _45);
						_17 = !zephir_array_isset(vx, k2);
						if (!(_17)) {
							ZEPHIR_OBS_NVAR(_46);
							zephir_array_fetch(&_46, vx, k2, PH_NOISY, "fastorm/db/Translator.zep", 320 TSRMLS_CC);
							_17 = Z_TYPE_P(_46) != IS_ARRAY;
						}
						if (_17) {
							ZEPHIR_INIT_NVAR(_39);
							array_init(_39);
							zephir_array_update_zval(&vx, k2, &_39, PH_COPY | PH_SEPARATE);
						}
						ZEPHIR_CALL_METHOD(&_9, this_ptr, "fomattedpairvalue", &_31, pair, v);
						zephir_check_call_status();
						zephir_array_update_multi(&vx, &_9 TSRMLS_CC, SL("za"), 2, k2);
					}
				}
				zephir_is_iterable(vx, &_48, &_47, 0, 0, "fastorm/db/Translator.zep", 329);
				for (
				  ; zephir_hash_get_current_data_ex(_48, (void**) &_49, &_47) == SUCCESS
				  ; zephir_hash_move_forward_ex(_48, &_47)
				) {
					ZEPHIR_GET_HMKEY(k, _48, _47);
					ZEPHIR_GET_HVALUE(v, _49);
					ZEPHIR_INIT_NVAR(_50);
					zephir_fast_join_str(_50, SL(", "), v TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_11);
					ZEPHIR_CONCAT_SVS(_11, "(", _50, ")");
					zephir_array_update_zval(&vx, k, &_11, PH_COPY | PH_SEPARATE);
				}
				ZEPHIR_INIT_NVAR(_50);
				zephir_fast_join_str(_50, SL(", "), kx TSRMLS_CC);
				ZEPHIR_INIT_VAR(_51);
				zephir_fast_join_str(_51, SL(", "), vx TSRMLS_CC);
				ZEPHIR_CONCAT_SVSV(return_value, "(", _50, ") VALUES ", _51);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "by")) {
				zephir_is_iterable(value, &_53, &_52, 0, 0, "fastorm/db/Translator.zep", 346);
				for (
				  ; zephir_hash_get_current_data_ex(_53, (void**) &_54, &_52) == SUCCESS
				  ; zephir_hash_move_forward_ex(_53, &_52)
				) {
					ZEPHIR_GET_HMKEY(k, _53, _52);
					ZEPHIR_GET_HVALUE(v, _54);
					if (Z_TYPE_P(v) == IS_ARRAY) {
						ZEPHIR_INIT_NVAR(_21);
						ZVAL_STRING(_21, "ex", 0);
						ZEPHIR_CALL_METHOD(&_14, this_ptr, "formatvalue", &_10, v, _21);
						zephir_check_temp_parameter(_21);
						zephir_check_call_status();
						zephir_array_append(&vx, _14, PH_SEPARATE, "fastorm/db/Translator.zep", 334);
					} else {
						if (Z_TYPE_P(k) == IS_STRING) {
							_55 = Z_TYPE_P(v) == IS_STRING;
							if (_55) {
								ZEPHIR_SINIT_NVAR(_20);
								ZVAL_STRING(&_20, "d", 0);
								ZEPHIR_SINIT_NVAR(_56);
								ZVAL_LONG(&_56, 1);
								ZEPHIR_CALL_FUNCTION(&_29, "strncasecmp", &_57, v, &_20, &_56);
								zephir_check_call_status();
								_55 = zephir_is_true(_29);
							}
							_58 = _55;
							if (!(_58)) {
								_58 = ZEPHIR_GT_LONG(v, 0);
							}
							ZEPHIR_INIT_NVAR(v);
							if (_58) {
								ZVAL_STRING(v, "ASC", 1);
							} else {
								ZVAL_STRING(v, "DESC", 1);
							}
							ZEPHIR_CALL_METHOD(&_14, this_ptr, "delimite", &_8, k);
							zephir_check_call_status();
							ZEPHIR_INIT_LNVAR(_11);
							ZEPHIR_CONCAT_VSV(_11, _14, " ", v);
							zephir_array_append(&vx, _11, PH_SEPARATE, "fastorm/db/Translator.zep", 341);
						} else {
							ZEPHIR_CALL_METHOD(&_9, this_ptr, "delimite", &_8, v);
							zephir_check_call_status();
							zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 343);
						}
					}
				}
				zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "ex") || ZEPHIR_IS_STRING(modifier, "sql")) {
				ZEPHIR_INIT_VAR(translator);
				object_init_ex(translator, fastorm_db_translator_ce);
				_2 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
				ZEPHIR_CALL_METHOD(NULL, translator, "__construct", NULL, _2);
				zephir_check_call_status();
				ZEPHIR_RETURN_CALL_METHOD(translator, "translate", NULL, value);
				zephir_check_call_status();
				RETURN_MM();
			}
			zephir_is_iterable(value, &_60, &_59, 0, 0, "fastorm/db/Translator.zep", 358);
			for (
			  ; zephir_hash_get_current_data_ex(_60, (void**) &_61, &_59) == SUCCESS
			  ; zephir_hash_move_forward_ex(_60, &_59)
			) {
				ZEPHIR_GET_HVALUE(v, _61);
				ZEPHIR_CALL_METHOD(&_9, this_ptr, "formatvalue", &_10, v, modifier);
				zephir_check_call_status();
				zephir_array_append(&vx, _9, PH_SEPARATE, "fastorm/db/Translator.zep", 356);
			}
			zephir_fast_join_str(return_value, SL(", "), vx TSRMLS_CC);
			RETURN_MM();
		} while(0);

	}
	if (!ZEPHIR_IS_STRING_IDENTICAL(modifier, "")) {
		_17 = Z_TYPE_P(value) != IS_NULL;
		if (_17) {
			ZEPHIR_CALL_FUNCTION(&_3, "is_scalar", &_62, value);
			zephir_check_call_status();
			_17 = !zephir_is_true(_3);
		}
		_55 = _17;
		if (_55) {
			_55 = !(zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC));
		}
		_58 = _55;
		if (_58) {
			_58 = !(zephir_is_instance_of(value, SL("DateTimeInterface") TSRMLS_CC));
		}
		if (_58) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_13);
			zephir_gettype(_13, value TSRMLS_CC);
			ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _13, "**");
			RETURN_MM();
		}
		do {
			if (ZEPHIR_IS_STRING(modifier, "s") || ZEPHIR_IS_STRING(modifier, "bin") || ZEPHIR_IS_STRING(modifier, "b")) {
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nullescape", &_63, value, modifier);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "sN") || ZEPHIR_IS_STRING(modifier, "sn")) {
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_STRING(_21, "s", 0);
				ZEPHIR_INIT_NVAR(_39);
				ZVAL_STRING(_39, "", 0);
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "nullescape", &_63, value, _21, _39);
				zephir_check_temp_parameter(_21);
				zephir_check_temp_parameter(_39);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "iN") || ZEPHIR_IS_STRING(modifier, "in")) {
				if (ZEPHIR_IS_STRING(value, "")) {
					ZEPHIR_INIT_NVAR(value);
					ZVAL_NULL(value);
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "i") || ZEPHIR_IS_STRING(modifier, "u")) {
				_64 = Z_TYPE_P(value) == IS_STRING;
				if (_64) {
					ZEPHIR_SINIT_NVAR(_20);
					ZVAL_STRING(&_20, "#[+-]?\\d++(e\\d+)?\\z#A", 0);
					ZEPHIR_CALL_FUNCTION(&_29, "preg_match", &_65, &_20, value);
					zephir_check_call_status();
					_64 = zephir_is_true(_29);
				}
				if (_64) {
					RETURN_CCTOR(value);
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						ZEPHIR_INIT_NVAR(proto);
						ZVAL_LONG(proto, ((zephir_get_numberval(value) + 0)));
						zephir_get_strval(_66, proto);
						ZEPHIR_CONCAT_VS(return_value, _66, "");
						RETURN_MM();
					}
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "f")) {
				_64 = Z_TYPE_P(value) == IS_STRING;
				if (_64) {
					_64 = zephir_is_numeric(value);
				}
				_67 = _64;
				if (_67) {
					ZEPHIR_SINIT_NVAR(_20);
					ZVAL_STRING(&_20, "x", 0);
					ZEPHIR_INIT_NVAR(_21);
					zephir_fast_strpos(_21, value, &_20, 0 );
					_67 = ZEPHIR_IS_FALSE_IDENTICAL(_21);
				}
				if (_67) {
					RETURN_CCTOR(value);
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						ZEPHIR_INIT_NVAR(_39);
						ZEPHIR_SINIT_NVAR(_56);
						ZVAL_LONG(&_56, (zephir_get_numberval(value) + 0));
						ZEPHIR_SINIT_VAR(_68);
						ZVAL_LONG(&_68, 10);
						ZEPHIR_SINIT_VAR(_69);
						ZVAL_STRING(&_69, ".", 0);
						ZEPHIR_SINIT_VAR(_70);
						ZVAL_STRING(&_70, "", 0);
						ZEPHIR_CALL_FUNCTION(&_29, "number_format", &_71, &_56, &_68, &_69, &_70);
						zephir_check_call_status();
						ZEPHIR_SINIT_NVAR(_56);
						ZVAL_LONG(&_56, 0);
						zephir_fast_trim(_39, _29, &_56, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
						ZEPHIR_SINIT_NVAR(_68);
						ZVAL_STRING(&_68, ".", 0);
						zephir_fast_trim(return_value, _39, &_68, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
						RETURN_MM();
					}
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "d") || ZEPHIR_IS_STRING(modifier, "t")) {
				if (Z_TYPE_P(value) == IS_NULL) {
					RETURN_MM_STRING("null", 1);
				} else {
					if (zephir_is_numeric(value)) {
						_72 = zephir_get_intval(value);
						ZEPHIR_INIT_NVAR(value);
						ZVAL_LONG(value, _72);
					} else {
						if (Z_TYPE_P(value) == IS_STRING) {
							ZEPHIR_INIT_NVAR(value);
							_73 = zend_fetch_class(SL("DateTime"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
							object_init_ex(value, _73);
							ZEPHIR_CALL_METHOD(NULL, value, "__construct", NULL, value);
							zephir_check_call_status();
						}
					}
					_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, modifier);
					zephir_check_call_status();
					RETURN_MM();
				}
			}
			if (ZEPHIR_IS_STRING(modifier, "by") || ZEPHIR_IS_STRING(modifier, "n")) {
				ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", &_8, value);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "ex") || ZEPHIR_IS_STRING(modifier, "sql")) {
				zephir_get_strval(_74, value);
				ZEPHIR_CPY_WRT(value, _74);
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_STRING(&_56, "`['\":", 0);
				ZEPHIR_CALL_FUNCTION(&_9, "strcspn", &_75, value, &_56);
				zephir_check_call_status();
				toSkip = zephir_get_intval(_9);
				if (zephir_fast_strlen_ev(value) != toSkip) {
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_LONG(&_56, 0);
					ZEPHIR_SINIT_NVAR(_68);
					ZVAL_LONG(&_68, toSkip);
					ZEPHIR_CALL_FUNCTION(&_29, "substr", &_76, value, &_56, &_68);
					zephir_check_call_status();
					ZEPHIR_INIT_VAR(_77);
					array_init_size(_77, 3);
					zephir_array_fast_append(_77, this_ptr);
					ZEPHIR_INIT_NVAR(_21);
					ZVAL_STRING(_21, "cb", 1);
					zephir_array_fast_append(_77, _21);
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_LONG(&_56, toSkip);
					ZEPHIR_CALL_FUNCTION(&_30, "substr", &_76, value, &_56);
					zephir_check_call_status();
					ZEPHIR_SINIT_NVAR(_56);
					ZVAL_STRING(&_56, "/(?=[`['\":])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?))/s", 0);
					ZEPHIR_CALL_FUNCTION(&_78, "preg_replace_callback", &_79, &_56, _77, _30);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_25);
					ZEPHIR_CONCAT_VV(_25, _29, _78);
					ZEPHIR_CPY_WRT(value, _25);
					ZEPHIR_CALL_FUNCTION(&_80, "preg_last_error", &_81);
					zephir_check_call_status();
					if (zephir_is_true(_80)) {
						ZEPHIR_THROW_EXCEPTION_DEBUG_STR(fastorm_db_dbexception_ce, "PCRE exception", "fastorm/db/Translator.zep", 446);
						return;
					}
				}
				RETURN_CCTOR(value);
			}
			if (ZEPHIR_IS_STRING(modifier, "SQL")) {
				zephir_get_strval(_82, value);
				RETURN_CTOR(_82);
			}
			if (ZEPHIR_IS_STRING(modifier, "like~")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, 1);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "~like")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, -1);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "~like~")) {
				_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_21);
				ZVAL_LONG(_21, 0);
				ZEPHIR_RETURN_CALL_METHOD(_83, "escapelike", NULL, value, _21);
				zephir_check_call_status();
				RETURN_MM();
			}
			if (ZEPHIR_IS_STRING(modifier, "and") || ZEPHIR_IS_STRING(modifier, "or") || ZEPHIR_IS_STRING(modifier, "a") || ZEPHIR_IS_STRING(modifier, "l") || ZEPHIR_IS_STRING(modifier, "v")) {
				zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
				ZEPHIR_INIT_NVAR(_21);
				zephir_gettype(_21, value TSRMLS_CC);
				ZEPHIR_CONCAT_SVS(return_value, "**Unexpected type ", _21, "**");
				RETURN_MM();
			}
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Unknown or invalid modifier %modifier**", 1);
		} while(0);

	}
	if (Z_TYPE_P(value) == IS_STRING) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_INIT_NVAR(_21);
		ZVAL_STRING(_21, "s", 0);
		ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, _21);
		zephir_check_temp_parameter(_21);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		ZEPHIR_CALL_FUNCTION(&_3, "is_int", &_84, value);
		zephir_check_call_status();
		if (zephir_is_true(_3)) {
			zephir_get_strval(_85, value);
			RETURN_CTOR(_85);
		} else {
			ZEPHIR_CALL_FUNCTION(&_29, "is_float", &_86, value);
			zephir_check_call_status();
			if (zephir_is_true(_29)) {
				ZEPHIR_INIT_NVAR(_21);
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_LONG(&_56, 10);
				ZEPHIR_SINIT_NVAR(_68);
				ZVAL_STRING(&_68, ".", 0);
				ZEPHIR_SINIT_NVAR(_69);
				ZVAL_STRING(&_69, "", 0);
				ZEPHIR_CALL_FUNCTION(&_30, "number_format", &_71, value, &_56, &_68, &_69);
				zephir_check_call_status();
				ZEPHIR_SINIT_NVAR(_56);
				ZVAL_LONG(&_56, 0);
				zephir_fast_trim(_21, _30, &_56, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				ZEPHIR_SINIT_NVAR(_68);
				ZVAL_STRING(&_68, ".", 0);
				zephir_fast_trim(return_value, _21, &_68, ZEPHIR_TRIM_RIGHT TSRMLS_CC);
				RETURN_MM();
			} else {
				if (Z_TYPE_P(value) == IS_BOOL) {
					_2 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_INIT_NVAR(_39);
					ZVAL_STRING(_39, "b", 0);
					ZEPHIR_RETURN_CALL_METHOD(_2, "escape", NULL, value, _39);
					zephir_check_temp_parameter(_39);
					zephir_check_call_status();
					RETURN_MM();
				} else {
					if (Z_TYPE_P(value) == IS_NULL) {
						RETURN_MM_STRING("null", 1);
					} else {
						_55 = (zephir_is_instance_of(value, SL("DateTime") TSRMLS_CC));
						if (!(_55)) {
							_55 = (zephir_is_instance_of(value, SL("DateTimeInterface") TSRMLS_CC));
						}
						if (_55) {
							_83 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
							ZEPHIR_INIT_NVAR(_39);
							ZVAL_STRING(_39, "t", 0);
							ZEPHIR_RETURN_CALL_METHOD(_83, "escape", NULL, value, _39);
							zephir_check_temp_parameter(_39);
							zephir_check_call_status();
							RETURN_MM();
						}
					}
				}
			}
		}
	}
	zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_13);
	zephir_gettype(_13, value TSRMLS_CC);
	ZEPHIR_CONCAT_SVS(return_value, "**Unexpected ", _13, "**");
	RETURN_MM();

}

PHP_METHOD(Fastorm_Db_Translator, fomattedPairValue) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *pairArray, *value, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &pairArray, &value);



	if (zephir_array_isset_long(pairArray, 1)) {
		zephir_array_fetch_long(&_0, pairArray, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 507 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value, _0);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		if (Z_TYPE_P(value) == IS_ARRAY) {
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "ex", 0);
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, value);
			zephir_check_call_status();
			RETURN_MM();
		}
	}

}

PHP_METHOD(Fastorm_Db_Translator, nullEscape) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *value, *modifier, *compareWith = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 1, &value, &modifier, &compareWith);

	if (!compareWith) {
		compareWith = ZEPHIR_GLOBAL(global_null);
	}


	if (ZEPHIR_IS_IDENTICAL(value, compareWith)) {
		RETURN_MM_STRING("null", 1);
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_METHOD(_0, "escape", NULL, value, modifier);
		zephir_check_call_status();
		RETURN_MM();
	}

}

/**
 * PREG callback from translate() or formatValue().
 * @param  array
 * @return string
 */
PHP_METHOD(Fastorm_Db_Translator, cb) {

	zephir_nts_static zephir_fcall_cache_entry *_16 = NULL, *_31 = NULL;
	zend_bool _5, _6, _7;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *matches, *_0, *_1, *_2, *_3, *_4, *mod, *_8, *_9, *_10, *_11, *_12, *_13, *_14 = NULL, *_15 = NULL, *_17 = NULL, *_18 = NULL, *_19, *_20, *_21, *_22, *_23, *_24, *_25, _26 = zval_used_for_init, _27 = zval_used_for_init, *_28 = NULL, *_29, *_30, *m = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &matches);



	if (zephir_array_isset_long(matches, 11)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		if (ZEPHIR_GE_LONG(_0, zephir_fast_count_int(_1 TSRMLS_CC))) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Extra placeholder**", 1);
		}
		RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_array_fetch_long(&_3, _2, (zephir_get_numberval(_4) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 554 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, _3, ZEPHIR_GLOBAL(global_false));
		zephir_check_call_status();
		RETURN_MM();
	}
	if (zephir_array_isset_long(matches, 10)) {
		ZEPHIR_OBS_VAR(mod);
		zephir_array_fetch_long(&mod, matches, 10, PH_NOISY, "fastorm/db/Translator.zep", 560 TSRMLS_CC);
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
		_5 = ZEPHIR_GE_LONG(_0, zephir_fast_count_int(_1 TSRMLS_CC));
		if (_5) {
			_5 = !ZEPHIR_IS_STRING_IDENTICAL(mod, "else");
		}
		_6 = _5;
		if (_6) {
			_6 = !ZEPHIR_IS_STRING_IDENTICAL(mod, "end");
		}
		if (_6) {
			zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
			RETURN_MM_STRING("**Extra modifier %mod**", 1);
		}
		if (ZEPHIR_IS_STRING_IDENTICAL(mod, "if")) {
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("ifLevel") TSRMLS_CC));
			RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
			_2 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
			_7 = !zephir_is_true(_2);
			if (_7) {
				_4 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
				_8 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
				zephir_array_fetch_long(&_3, _4, (zephir_get_numberval(_8) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 571 TSRMLS_CC);
				_7 = !zephir_is_true(_3);
			}
			if (_7) {
				_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
				zephir_update_property_this(this_ptr, SL("ifLevelStart"), _9 TSRMLS_CC);
				zephir_update_property_this(this_ptr, SL("comment"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
				RETURN_MM_STRING("/*", 1);
			}
			RETURN_MM_STRING("", 1);
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(mod, "else")) {
				_8 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevelStart"), PH_NOISY_CC);
				_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
				if (ZEPHIR_IS_IDENTICAL(_8, _9)) {
					ZEPHIR_INIT_ZVAL_NREF(_10);
					ZVAL_LONG(_10, 0);
					zephir_update_property_this(this_ptr, SL("ifLevelStart"), _10 TSRMLS_CC);
					zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
					RETURN_MM_STRING("*/", 1);
				} else {
					_10 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
					if (!zephir_is_true(_10)) {
						_11 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
						zephir_update_property_this(this_ptr, SL("ifLevelStart"), _11 TSRMLS_CC);
						zephir_update_property_this(this_ptr, SL("comment"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("/*", 1);
					}
				}
			} else {
				if (ZEPHIR_IS_STRING_IDENTICAL(mod, "end")) {
					RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("ifLevel") TSRMLS_CC));
					_9 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevelStart"), PH_NOISY_CC);
					_10 = zephir_fetch_nproperty_this(this_ptr, SL("ifLevel"), PH_NOISY_CC);
					if (ZEPHIR_IS_LONG_IDENTICAL(_9, (zephir_get_numberval(_10) + 1))) {
						ZEPHIR_INIT_ZVAL_NREF(_11);
						ZVAL_LONG(_11, 0);
						zephir_update_property_this(this_ptr, SL("ifLevelStart"), _11 TSRMLS_CC);
						zephir_update_property_this(this_ptr, SL("comment"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("*/", 1);
					}
					RETURN_MM_STRING("", 1);
				} else {
					if (ZEPHIR_IS_STRING_IDENTICAL(mod, "ex")) {
						_10 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
						_11 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
						_12 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
						ZEPHIR_OBS_VAR(_14);
						zephir_read_property_this(&_14, this_ptr, SL("cursor"), PH_NOISY_CC);
						zephir_array_fetch(&_13, _12, _14, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 601 TSRMLS_CC);
						ZEPHIR_INIT_VAR(_15);
						ZVAL_LONG(_15, 1);
						Z_SET_ISREF_P(_10);
						ZEPHIR_CALL_FUNCTION(NULL, "array_splice", &_16, _10, _11, _15, _13);
						Z_UNSET_ISREF_P(_10);
						zephir_check_call_status();
						RETURN_MM_STRING("", 1);
					} else {
						if (ZEPHIR_IS_STRING_IDENTICAL(mod, "lmt")) {
							_11 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
							ZEPHIR_OBS_NVAR(_14);
							zephir_read_property_this(&_14, this_ptr, SL("cursor"), PH_NOISY_CC);
							zephir_array_fetch(&_13, _11, _14, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 605 TSRMLS_CC);
							if (Z_TYPE_P(_13) != IS_NULL) {
								_12 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								ZEPHIR_OBS_VAR(_17);
								ZEPHIR_OBS_VAR(_18);
								zephir_read_property_this(&_18, this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch(&_17, _12, _18, PH_NOISY, "fastorm/db/Translator.zep", 606 TSRMLS_CC);
								ZEPHIR_INIT_ZVAL_NREF(_19);
								ZVAL_LONG(_19, zephir_get_intval(_17));
								zephir_update_property_this(this_ptr, SL("limit"), _19 TSRMLS_CC);
							}
							RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
							RETURN_MM_STRING("", 1);
						} else {
							if (ZEPHIR_IS_STRING_IDENTICAL(mod, "ofs")) {
								_19 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								ZEPHIR_OBS_NVAR(_17);
								zephir_read_property_this(&_17, this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch(&_20, _19, _17, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 612 TSRMLS_CC);
								if (Z_TYPE_P(_20) != IS_NULL) {
									_21 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
									ZEPHIR_OBS_NVAR(_18);
									ZEPHIR_OBS_VAR(_22);
									zephir_read_property_this(&_22, this_ptr, SL("cursor"), PH_NOISY_CC);
									zephir_array_fetch(&_18, _21, _22, PH_NOISY, "fastorm/db/Translator.zep", 613 TSRMLS_CC);
									ZEPHIR_INIT_ZVAL_NREF(_23);
									ZVAL_LONG(_23, zephir_get_intval(_18));
									zephir_update_property_this(this_ptr, SL("offset"), _23 TSRMLS_CC);
								}
								RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
								RETURN_MM_STRING("", 1);
							} else {
								RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("cursor") TSRMLS_CC));
								_19 = zephir_fetch_nproperty_this(this_ptr, SL("args"), PH_NOISY_CC);
								_21 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
								zephir_array_fetch_long(&_20, _19, (zephir_get_numberval(_21) - 1), PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 620 TSRMLS_CC);
								ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, _20, mod);
								zephir_check_call_status();
								RETURN_MM();
							}
						}
					}
				}
			}
		}
	}
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("comment"), PH_NOISY_CC);
	if (zephir_is_true(_0)) {
		RETURN_MM_STRING("...", 1);
	}
	zephir_array_fetch_long(&_3, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 628 TSRMLS_CC);
	if (zephir_is_true(_3)) {
		zephir_array_fetch_long(&_13, matches, 1, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 629 TSRMLS_CC);
		ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", NULL, _13);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		zephir_array_fetch_long(&_3, matches, 2, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 631 TSRMLS_CC);
		if (zephir_is_true(_3)) {
			zephir_array_fetch_long(&_20, matches, 2, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 632 TSRMLS_CC);
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "delimite", NULL, _20);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			zephir_array_fetch_long(&_24, matches, 3, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 634 TSRMLS_CC);
			if (zephir_is_true(_24)) {
				_0 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
				ZEPHIR_INIT_NVAR(_15);
				zephir_array_fetch_long(&_25, matches, 4, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 635 TSRMLS_CC);
				ZEPHIR_SINIT_VAR(_26);
				ZVAL_STRING(&_26, "''", 0);
				ZEPHIR_SINIT_VAR(_27);
				ZVAL_STRING(&_27, "'", 0);
				zephir_fast_str_replace(_15, &_26, &_27, _25);
				ZEPHIR_INIT_VAR(_28);
				ZVAL_STRING(_28, "s", 0);
				ZEPHIR_RETURN_CALL_METHOD(_0, "escape", NULL, _15, _28);
				zephir_check_temp_parameter(_28);
				zephir_check_call_status();
				RETURN_MM();
			} else {
				zephir_array_fetch_long(&_25, matches, 5, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 637 TSRMLS_CC);
				if (zephir_is_true(_25)) {
					_1 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
					ZEPHIR_INIT_NVAR(_28);
					zephir_array_fetch_long(&_29, matches, 6, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 638 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26);
					ZVAL_STRING(&_26, "\"\"", 0);
					ZEPHIR_SINIT_NVAR(_27);
					ZVAL_STRING(&_27, "\"", 0);
					zephir_fast_str_replace(_28, &_26, &_27, _29);
					ZEPHIR_INIT_VAR(_30);
					ZVAL_STRING(_30, "s", 0);
					ZEPHIR_RETURN_CALL_METHOD(_1, "escape", NULL, _28, _30);
					zephir_check_temp_parameter(_30);
					zephir_check_call_status();
					RETURN_MM();
				} else {
					zephir_array_fetch_long(&_29, matches, 7, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 640 TSRMLS_CC);
					if (zephir_is_true(_29)) {
						zephir_update_property_this(this_ptr, SL("hasError"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
						RETURN_MM_STRING("**Alone quote**", 1);
					}
				}
			}
		}
	}
	zephir_array_fetch_long(&_3, matches, 8, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 645 TSRMLS_CC);
	if (zephir_is_true(_3)) {
		zephir_array_fetch_long(&_13, matches, 8, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 647 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_26);
		ZVAL_LONG(&_26, 0);
		ZEPHIR_SINIT_NVAR(_27);
		ZVAL_LONG(&_27, -1);
		ZEPHIR_CALL_FUNCTION(&m, "substr", &_31, _13, &_26, &_27);
		zephir_check_call_status();
		zephir_array_fetch_long(&_24, matches, 9, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 648 TSRMLS_CC);
		if (ZEPHIR_IS_STRING(_24, "")) {
			ZEPHIR_RETURN_CALL_METHOD(this_ptr, "formatvalue", NULL, m);
			zephir_check_call_status();
			RETURN_MM();
		} else {
			zephir_array_fetch_long(&_24, matches, 9, PH_NOISY | PH_READONLY, "fastorm/db/Translator.zep", 651 TSRMLS_CC);
			ZEPHIR_CONCAT_VV(return_value, m, _24);
			RETURN_MM();
		}
	}
	RETURN_MM_STRING("this should be never executed", 1);

}

/**
 * Apply substitutions to indentifier and delimites it.
 * @param  string indentifier
 * @return string
 * @internal
 */
PHP_METHOD(Fastorm_Db_Translator, delimite) {

	int ZEPHIR_LAST_CALL_STATUS;
	HashTable *_1;
	HashPosition _0;
	zval *value, *parts, *k = NULL, *v = NULL, **_2, *_3, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_INIT_VAR(parts);
	zephir_fast_explode_str(parts, SL("."), value, LONG_MAX TSRMLS_CC);
	zephir_is_iterable(parts, &_1, &_0, 0, 0, "fastorm/db/Translator.zep", 675);
	for (
	  ; zephir_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
	  ; zephir_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HMKEY(k, _1, _0);
		ZEPHIR_GET_HVALUE(v, _2);
		if (!ZEPHIR_IS_STRING_IDENTICAL(v, "*")) {
			_3 = zephir_fetch_nproperty_this(this_ptr, SL("driver"), PH_NOISY_CC);
			ZEPHIR_INIT_NVAR(_5);
			ZVAL_STRING(_5, "n", 0);
			ZEPHIR_CALL_METHOD(&_4, _3, "escape", NULL, v, _5);
			zephir_check_temp_parameter(_5);
			zephir_check_call_status();
			zephir_array_update_zval(&parts, k, &_4, PH_COPY | PH_SEPARATE);
		}
	}
	zephir_fast_join_str(return_value, SL("."), parts TSRMLS_CC);
	RETURN_MM();

}

