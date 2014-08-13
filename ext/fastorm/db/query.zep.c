
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/string.h"


/**
 *
 *
 * @author (dg)
 * Inspired by David Grudl"s dibi - DibiFLuent
 */
ZEPHIR_INIT_CLASS(Fastorm_Db_Query) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, Query, fastorm, db_query, fastorm_db_query_method_entry, 0);

	zend_declare_property_null(fastorm_db_query_ce, SL("masks"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("modifiers"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("separators"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("swithes"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("connection"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("command"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("setups"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("cursor"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("clauses"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(fastorm_db_query_ce, SL("flags"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_class_constant_bool(fastorm_db_query_ce, SL("REMOVE"), 0 TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_TEXT"), "s" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_BINARY"), "bin" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_BOOL"), "b" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_INTEGER"), "i" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_FLOAT"), "f" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_DATE"), "d" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_DATETIME"), "t" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("TYPE_TIME"), "t" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("IDENTIFIER"), "n" TSRMLS_CC);

	/** sorting order */
	zend_declare_class_constant_string(fastorm_db_query_ce, SL("ASC"), "ASC" TSRMLS_CC);

	zend_declare_class_constant_string(fastorm_db_query_ce, SL("DESC"), "DESC" TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_Query, __construct) {

	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *connection, *_0, *_2, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &connection);



	if (!(zephir_instance_of_ev(connection, fastorm_db_connection_ce TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(spl_ce_InvalidArgumentException, "Parameter 'connection' must be an instance of 'Fastorm\\Db\\Connection'", "", 0);
		return;
	}
	zephir_update_property_this(this_ptr, SL("connection"), connection TSRMLS_CC);
	_0 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "staticinitialize", &_1);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	zephir_update_property_this(this_ptr, SL("flags"), _2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	array_init(_3);
	zephir_update_property_this(this_ptr, SL("setups"), _3 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_Query, staticInitialize) {

	zval *_2;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 7);
	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 13);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "SELECT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "DISTINCT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "FROM", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "GROUP BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "HAVING", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "OFFSET", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("SELECT"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 7);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "UPDATE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "SET", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("UPDATE"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 7);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "INSERT", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "INTO", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "VALUES", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "SELECT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("INSERT"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_BNVAR(_1);
	array_init_size(_1, 11);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "DELETE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "FROM", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "USING", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "WHERE", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "ORDER BY", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "LIMIT", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_update_string(&_0, SL("DELETE"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("masks"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 13);
	add_assoc_stringl_ex(_0, SS("SELECT"), SL("%n"), 1);
	add_assoc_stringl_ex(_0, SS("FROM"), SL("%n"), 1);
	add_assoc_stringl_ex(_0, SS("IN"), SL("%in"), 1);
	add_assoc_stringl_ex(_0, SS("VALUES"), SL("%l"), 1);
	add_assoc_stringl_ex(_0, SS("SET"), SL("%a"), 1);
	add_assoc_stringl_ex(_0, SS("WHERE"), SL("%and"), 1);
	add_assoc_stringl_ex(_0, SS("HAVING"), SL("%and"), 1);
	add_assoc_stringl_ex(_0, SS("ORDER BY"), SL("%by"), 1);
	add_assoc_stringl_ex(_0, SS("GROUP BY"), SL("%by"), 1);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("modifiers"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 17);
	add_assoc_stringl_ex(_0, SS("SELECT"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("FROM"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("WHERE"), SL("AND"), 1);
	add_assoc_stringl_ex(_0, SS("GROUP BY"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("HAVING"), SL("AND"), 1);
	add_assoc_stringl_ex(_0, SS("ORDER BY"), SL(","), 1);
	zephir_array_update_string(&_0, SL("LIMIT"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&_0, SL("OFFSET"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	add_assoc_stringl_ex(_0, SS("SET"), SL(","), 1);
	add_assoc_stringl_ex(_0, SS("VALUES"), SL(","), 1);
	zephir_array_update_string(&_0, SL("INTO"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("separators"), _0 TSRMLS_CC);
	ZEPHIR_INIT_BNVAR(_0);
	array_init_size(_0, 7);
	add_assoc_stringl_ex(_0, SS("JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("INNER JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("LEFT JOIN"), SL("FROM"), 1);
	add_assoc_stringl_ex(_0, SS("RIGHT JOIN"), SL("FROM"), 1);
	zephir_update_static_property_ce(fastorm_db_query_ce, SL("swithes"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_Query, __call) {

	HashTable *_21;
	HashPosition _20;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL, *_14 = NULL, *_19 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_bool removeArray, _0, _12, _15, _16;
	zval *clause = NULL, *clauseArgs = NULL, *sep, *argument = NULL, *cursor = NULL, *_1, *_2 = NULL, *_3, *_5, *_6, *_7, *_8, *_9, *_10 = NULL, *_11, _13, *_17 = NULL, *_18 = NULL, **_22;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &clause, &clauseArgs);

	ZEPHIR_SEPARATE_PARAM(clause);
	ZEPHIR_SEPARATE_PARAM(clauseArgs);


	_0 = zephir_fast_count_int(clauseArgs TSRMLS_CC) == 1;
	if (_0) {
		zephir_array_fetch_long(&_1, clauseArgs, 0, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 107 TSRMLS_CC);
		_0 = ZEPHIR_IS_FALSE_IDENTICAL(_1);
	}
	removeArray = _0;
	ZEPHIR_CALL_SELF(&_2, "_formatclause", NULL, clause);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(clause, _2);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_EMPTY(_3)) {
		ZEPHIR_CALL_METHOD(NULL, this_ptr, "initialize", &_4, clause);
		zephir_check_call_status();
	}
	_5 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("swithes") TSRMLS_CC);
	if (zephir_array_isset(_5, clause)) {
		_6 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("swithes") TSRMLS_CC);
		zephir_array_fetch(&_7, _6, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 117 TSRMLS_CC);
		zephir_update_property_this(this_ptr, SL("cursor"), _7 TSRMLS_CC);
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _8);
	}
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
	if (zephir_array_key_exists(_6, clause TSRMLS_CC)) {
		zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _8);
		if (removeArray) {
			zephir_update_property_array(this_ptr, SL("clauses"), cursor, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
			RETURN_THIS();
		}
		_8 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("separators") TSRMLS_CC);
		if (zephir_array_isset(_8, clause)) {
			_9 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("separators") TSRMLS_CC);
			ZEPHIR_OBS_VAR(sep);
			zephir_array_fetch(&sep, _9, clause, PH_NOISY, "fastorm/db/Query.zep", 132 TSRMLS_CC);
			if (ZEPHIR_IS_FALSE_IDENTICAL(sep)) {
				ZEPHIR_INIT_VAR(_10);
				array_init(_10);
				zephir_update_property_array(this_ptr, SL("clauses"), clause, _10 TSRMLS_CC);
			} else {
				_11 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
				zephir_array_fetch(&_7, _11, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 136 TSRMLS_CC);
				if (!(ZEPHIR_IS_EMPTY(_7))) {
					zephir_update_property_array_multi(this_ptr, SL("clauses"), &sep TSRMLS_CC, SL("za"), 1, clause);
				}
			}
		}
	} else {
		if (removeArray) {
			RETURN_THIS();
		}
		_6 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _6);
		zephir_update_property_array_multi(this_ptr, SL("clauses"), &clause TSRMLS_CC, SL("za"), 1, cursor);
	}
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
	if (Z_TYPE_P(_6) == IS_NULL) {
		zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("cursor"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(cursor, _8);
	}
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
	if (!(zephir_array_isset(_6, cursor))) {
		ZEPHIR_INIT_NVAR(_10);
		array_init(_10);
		zephir_update_property_array(this_ptr, SL("clauses"), cursor, _10 TSRMLS_CC);
	}
	if (zephir_fast_count_int(clauseArgs TSRMLS_CC) == 1) {
		ZEPHIR_OBS_VAR(argument);
		zephir_array_fetch_long(&argument, clauseArgs, 0, PH_NOISY, "fastorm/db/Query.zep", 164 TSRMLS_CC);
		if (ZEPHIR_IS_TRUE_IDENTICAL(argument)) {
			RETURN_THIS();
		}
		_12 = Z_TYPE_P(argument) == IS_STRING;
		if (_12) {
			ZEPHIR_SINIT_VAR(_13);
			ZVAL_STRING(&_13, "#^[a-z:_][a-z0-9_.:]*$#i", 0);
			ZEPHIR_CALL_FUNCTION(&_2, "preg_match", &_14, &_13, argument);
			zephir_check_call_status();
			_12 = zephir_is_true(_2);
		}
		if (_12) {
			ZEPHIR_INIT_NVAR(clauseArgs);
			array_init_size(clauseArgs, 3);
			ZEPHIR_INIT_NVAR(_10);
			ZVAL_STRING(_10, "%n", 1);
			zephir_array_fast_append(clauseArgs, _10);
			zephir_array_fast_append(clauseArgs, argument);
		} else {
			_15 = Z_TYPE_P(argument) == IS_ARRAY;
			if (!(_15)) {
				_16 = (zephir_is_instance_of(argument, SL("Traversable") TSRMLS_CC));
				if (_16) {
					_16 = !(zephir_is_instance_of(argument, SL("Fastorm\\Db\\self") TSRMLS_CC));
				}
				_15 = _16;
			}
			if (_15) {
				_6 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("modifiers") TSRMLS_CC);
				if (zephir_array_isset(_6, clause)) {
					ZEPHIR_INIT_NVAR(clauseArgs);
					array_init_size(clauseArgs, 3);
					_8 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("modifiers") TSRMLS_CC);
					ZEPHIR_OBS_VAR(_17);
					zephir_array_fetch(&_17, _8, clause, PH_NOISY, "fastorm/db/Query.zep", 177 TSRMLS_CC);
					zephir_array_fast_append(clauseArgs, _17);
					zephir_array_fast_append(clauseArgs, argument);
				} else {
					Z_SET_ISREF_P(argument);
					ZEPHIR_CALL_FUNCTION(&_18, "key", &_19, argument);
					Z_UNSET_ISREF_P(argument);
					zephir_check_call_status();
					if (Z_TYPE_P(_18) == IS_STRING) {
						ZEPHIR_INIT_NVAR(clauseArgs);
						array_init_size(clauseArgs, 3);
						ZEPHIR_INIT_NVAR(_10);
						ZVAL_STRING(_10, "%a", 1);
						zephir_array_fast_append(clauseArgs, _10);
						zephir_array_fast_append(clauseArgs, argument);
					}
				}
			}
		}
	}
	zephir_is_iterable(clauseArgs, &_21, &_20, 0, 0, "fastorm/db/Query.zep", 195);
	for (
	  ; zephir_hash_get_current_data_ex(_21, (void**) &_22, &_20) == SUCCESS
	  ; zephir_hash_move_forward_ex(_21, &_20)
	) {
		ZEPHIR_GET_HVALUE(argument, _22);
		if (zephir_is_instance_of(argument, SL("Fastorm\\Db\\self") TSRMLS_CC)) {
			ZEPHIR_CALL_METHOD(&_18, argument, "__tostring",  NULL);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(argument);
			ZEPHIR_CONCAT_SVS(argument, "(", _18, ")");
		}
		ZEPHIR_OBS_NVAR(_17);
		zephir_read_property_this(&_17, this_ptr, SL("cursor"), PH_NOISY_CC);
		zephir_update_property_array_multi(this_ptr, SL("clauses"), &argument TSRMLS_CC, SL("za"), 1, _17);
	}
	RETURN_THIS();

}

PHP_METHOD(Fastorm_Db_Query, initialize) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_4 = NULL;
	zval *clause_param = NULL, *_0, *_1, *_2, *_3 = NULL, *_5;
	zval *clause = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &clause_param);

	zephir_get_strval(clause, clause_param);


	_0 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
	if (zephir_array_isset(_0, clause)) {
		_1 = zephir_fetch_static_property_ce(fastorm_db_query_ce, SL("masks") TSRMLS_CC);
		zephir_array_fetch(&_2, _1, clause, PH_NOISY | PH_READONLY, "fastorm/db/Query.zep", 200 TSRMLS_CC);
		ZEPHIR_CALL_FUNCTION(&_3, "array_fill_keys", &_4, _2, ZEPHIR_GLOBAL(global_null));
		zephir_check_call_status();
		zephir_update_property_this(this_ptr, SL("clauses"), _3 TSRMLS_CC);
	} else {
		ZEPHIR_INIT_VAR(_5);
		array_init(_5);
		zephir_update_property_this(this_ptr, SL("clauses"), _5 TSRMLS_CC);
	}
	zephir_update_property_this(this_ptr, SL("cursor"), clause TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("command"), clause TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Fastorm_Db_Query, _formatClause) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_5 = NULL;
	zend_bool _0;
	zval *s_param = NULL, *_1, _2, _3, *_4 = NULL;
	zval *s = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &s_param);

	zephir_get_strval(s, s_param);
	ZEPHIR_SEPARATE_PARAM(s);


	_0 = ZEPHIR_IS_STRING_IDENTICAL(s, "order");
	if (!(_0)) {
		_0 = ZEPHIR_IS_STRING_IDENTICAL(s, "group");
	}
	if (_0) {
		zephir_concat_self_str(&s, "By", sizeof("By")-1 TSRMLS_CC);
	}
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "#[a-z](?=[A-Z])#", 0);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_STRING(&_3, "$0 ", 0);
	ZEPHIR_CALL_FUNCTION(&_4, "preg_replace", &_5, &_2, &_3, s);
	zephir_check_call_status();
	zephir_fast_strtoupper(_1, _4);
	zephir_get_strval(_6, _1);
	RETURN_CTOR(_6);

}

/**
 * Returns SQL command.
 * @return string
 */
PHP_METHOD(Fastorm_Db_Query, getCommand) {


	RETURN_MEMBER(this_ptr, "command");

}

PHP_METHOD(Fastorm_Db_Query, getConnection) {


	RETURN_MEMBER(this_ptr, "connection");

}

PHP_METHOD(Fastorm_Db_Query, setupResult) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	zval *method, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &method);



	ZEPHIR_CALL_FUNCTION(&_0, "func_get_args", &_1);
	zephir_check_call_status();
	zephir_update_property_array_append(this_ptr, SL("setups"), _0 TSRMLS_CC);
	RETURN_THIS();

}

PHP_METHOD(Fastorm_Db_Query, execute) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *ret = NULL, *res = NULL, *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &ret);

	if (!ret) {
		ret = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_METHOD(&_0, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&res, this_ptr, "query", NULL, _0);
	zephir_check_call_status();
	if (ZEPHIR_IS_STRING_IDENTICAL(ret, "n")) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_METHOD(_1, "getinsertid", NULL);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		RETURN_CCTOR(res);
	}

}

PHP_METHOD(Fastorm_Db_Query, fetchFirst) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_3;
	zval *ret = NULL, *_0, *_1 = NULL, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_STRING_IDENTICAL(_0, "SELECT")) {
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "%lmt", 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_INIT_NVAR(_4);
		ZVAL_LONG(_4, 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _3);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&ret, _1, "fetchrow",  NULL);
		zephir_check_call_status();
	} else {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&ret, _1, "fetchrow",  NULL);
		zephir_check_call_status();
	}
	if (ZEPHIR_IS_FALSE_IDENTICAL(ret)) {
		RETURN_MM_NULL();
	} else {
		RETURN_CCTOR(ret);
	}

}

PHP_METHOD(Fastorm_Db_Query, fetchSingle) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_3;
	zval *_0, *_1 = NULL, *_2 = NULL, *_4 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
	if (ZEPHIR_IS_STRING_IDENTICAL(_0, "SELECT")) {
		ZEPHIR_INIT_VAR(_3);
		array_init_size(_3, 3);
		ZEPHIR_INIT_VAR(_4);
		ZVAL_STRING(_4, "%lmt", 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_INIT_NVAR(_4);
		ZVAL_LONG(_4, 1);
		zephir_array_fast_append(_3, _4);
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _3);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_RETURN_CALL_METHOD(_1, "fetchsingle", NULL);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "_export",  NULL);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "query", NULL, _2);
		zephir_check_call_status();
		ZEPHIR_RETURN_CALL_METHOD(_1, "fetchsingle", NULL);
		zephir_check_call_status();
		RETURN_MM();
	}

}

PHP_METHOD(Fastorm_Db_Query, fetchAssoc) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *assoc, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &assoc);



	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "fetchassoc", NULL, assoc);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_Db_Query, fetchPairs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *key_param = NULL, *value_param = NULL, *_0 = NULL, *_1 = NULL;
	zval *key = NULL, *value = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &key_param, &value_param);

	if (!key_param) {
		ZEPHIR_INIT_VAR(key);
		ZVAL_EMPTY_STRING(key);
	} else {
		zephir_get_strval(key, key_param);
	}
	if (!value_param) {
		ZEPHIR_INIT_VAR(value);
		ZVAL_EMPTY_STRING(value);
	} else {
		zephir_get_strval(value, value_param);
	}


	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "fetchpairs", NULL, key, value);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_Db_Query, getIterator) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_2;
	zval *offset = NULL, *limit = NULL, *_0 = NULL, *_1 = NULL, *_3;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &offset, &limit);

	if (!offset) {
		offset = ZEPHIR_GLOBAL(global_null);
	}
	if (!limit) {
		limit = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(_2);
	array_init_size(_2, 5);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "%ofs %lmt", 1);
	zephir_array_fast_append(_2, _3);
	zephir_array_fast_append(_2, offset);
	zephir_array_fast_append(_2, limit);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export", NULL, ZEPHIR_GLOBAL(global_null), _2);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "getiterator", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Fastorm_Db_Query, count) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1;
	zval *_0 = NULL, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_1);
	array_init_size(_1, 5);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "SELECT COUNT(*) FROM (%ex", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	zephir_array_fast_append(_1, _3);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, ") AS [data]", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "query", NULL, _1);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_3, _0, "fetchsingle",  NULL);
	zephir_check_call_status();
	RETURN_MM_LONG(zephir_get_intval(_3));

}

PHP_METHOD(Fastorm_Db_Query, query) {

	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL;
	zval *_6 = NULL;
	HashTable *_3;
	HashPosition _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *queryArgs, *res = NULL, *setup = NULL, *_0, *_1, **_4, *_5 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &queryArgs);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&res, _0, "query", NULL, queryArgs);
	zephir_check_call_status();
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("setups"), PH_NOISY_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/db/Query.zep", 328);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HVALUE(setup, _4);
		ZEPHIR_INIT_NVAR(_5);
		ZEPHIR_INIT_NVAR(_6);
		array_init_size(_6, 3);
		zephir_array_fast_append(_6, res);
		Z_SET_ISREF_P(setup);
		ZEPHIR_CALL_FUNCTION(&_7, "array_shift", &_8, setup);
		Z_UNSET_ISREF_P(setup);
		zephir_check_call_status();
		zephir_array_fast_append(_6, _7);
		ZEPHIR_CALL_USER_FUNC_ARRAY(_5, _6, setup);
		zephir_check_call_status();
	}
	RETURN_CCTOR(res);

}

PHP_METHOD(Fastorm_Db_Query, __toString) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("connection"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "_export",  NULL);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "translate", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Change a SQL flag.
 * @param  string  flag name
 * @param  bool  value
 * @return DibiFluent  provides a fluent interface
 */
PHP_METHOD(Fastorm_Db_Query, setFlag) {

	zval *flag = NULL, *value = NULL, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &flag, &value);

	ZEPHIR_SEPARATE_PARAM(flag);
	if (!value) {
		value = ZEPHIR_GLOBAL(global_true);
	}


	ZEPHIR_INIT_VAR(_0);
	zephir_fast_strtoupper(_0, flag);
	ZEPHIR_CPY_WRT(flag, _0);
	if (zephir_is_true(value)) {
		zephir_update_property_array(this_ptr, SL("flags"), flag, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
		zephir_array_unset(&_1, flag, PH_SEPARATE);
	}
	RETURN_THIS();

}

/**
 * Is a flag set?
 * @param  string  flag name
 * @return bool
 */
PHP_METHOD(Fastorm_Db_Query, getFlag) {

	zval *flag, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &flag);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_fast_strtoupper(_1, flag);
	RETURN_MM_BOOL(zephir_array_isset(_0, _1));

}

PHP_METHOD(Fastorm_Db_Query, _export) {

	zend_bool _7;
	HashTable *_5, *_12, *_15;
	HashPosition _4, _11, _14;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *clause = NULL, *additionalArgs = NULL, *data = NULL, *cls = NULL, *statement = NULL, *arg = NULL, *ret, *_0 = NULL, *_1, *_2, *_3, **_6, *_8 = NULL, *_9 = NULL, *_10, **_13, **_16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &clause, &additionalArgs);

	if (!clause) {
		ZEPHIR_CPY_WRT(clause, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(clause);
	}
	if (!additionalArgs) {
		additionalArgs = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(ret);
	array_init(ret);
	if (Z_TYPE_P(clause) == IS_NULL) {
		ZEPHIR_OBS_VAR(data);
		zephir_read_property_this(&data, this_ptr, SL("clauses"), PH_NOISY_CC);
	} else {
		ZEPHIR_CALL_SELF(&_0, "_formatclause", NULL, clause);
		zephir_check_call_status();
		ZEPHIR_CPY_WRT(clause, _0);
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
		if (zephir_array_key_exists(_1, clause TSRMLS_CC)) {
			ZEPHIR_INIT_NVAR(data);
			array_init_size(data, 2);
			_2 = zephir_fetch_nproperty_this(this_ptr, SL("clauses"), PH_NOISY_CC);
			ZEPHIR_OBS_VAR(_3);
			zephir_array_fetch(&_3, _2, clause, PH_NOISY, "fastorm/db/Query.zep", 379 TSRMLS_CC);
			zephir_array_update_string(&data, SL("clause"), &_3, PH_COPY | PH_SEPARATE);
		} else {
			array_init(return_value);
			RETURN_MM();
		}
	}
	zephir_is_iterable(data, &_5, &_4, 0, 0, "fastorm/db/Query.zep", 397);
	for (
	  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
	  ; zephir_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HMKEY(cls, _5, _4);
		ZEPHIR_GET_HVALUE(statement, _6);
		if (Z_TYPE_P(statement) != IS_NULL) {
			zephir_array_append(&ret, cls, PH_SEPARATE, "fastorm/db/Query.zep", 387);
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("command"), PH_NOISY_CC);
			_7 = ZEPHIR_IS_IDENTICAL(cls, _1);
			if (_7) {
				_2 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
				_7 = zephir_fast_count_int(_2 TSRMLS_CC) > 0;
			}
			if (_7) {
				ZEPHIR_INIT_NVAR(_8);
				ZEPHIR_INIT_NVAR(_9);
				_10 = zephir_fetch_nproperty_this(this_ptr, SL("flags"), PH_NOISY_CC);
				zephir_array_keys(_9, _10 TSRMLS_CC);
				zephir_fast_join_str(_8, SL(" "), _9 TSRMLS_CC);
				zephir_array_append(&ret, _8, PH_SEPARATE, "fastorm/db/Query.zep", 389);
			}
			zephir_is_iterable(statement, &_12, &_11, 0, 0, "fastorm/db/Query.zep", 394);
			for (
			  ; zephir_hash_get_current_data_ex(_12, (void**) &_13, &_11) == SUCCESS
			  ; zephir_hash_move_forward_ex(_12, &_11)
			) {
				ZEPHIR_GET_HVALUE(arg, _13);
				zephir_array_append(&ret, arg, PH_SEPARATE, "fastorm/db/Query.zep", 392);
			}
		}
	}
	if (Z_TYPE_P(additionalArgs) != IS_NULL) {
		zephir_is_iterable(additionalArgs, &_15, &_14, 0, 0, "fastorm/db/Query.zep", 401);
		for (
		  ; zephir_hash_get_current_data_ex(_15, (void**) &_16, &_14) == SUCCESS
		  ; zephir_hash_move_forward_ex(_15, &_14)
		) {
			ZEPHIR_GET_HVALUE(statement, _16);
			zephir_array_append(&ret, statement, PH_SEPARATE, "fastorm/db/Query.zep", 399);
		}
	}
	RETURN_CCTOR(ret);

}

