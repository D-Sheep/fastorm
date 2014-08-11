
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_DibiColumnInfo) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, DibiColumnInfo, fastorm, db_dibicolumninfo, fastorm_db_dibicolumninfo_method_entry, 0);

	zend_declare_property_null(fastorm_db_dibicolumninfo_ce, SL("patterns"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

/**
 * Heuristic type detection.
 * @param  string
 * @return string
 * @internal
 */
PHP_METHOD(Fastorm_Db_DibiColumnInfo, detectType) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL;
	HashTable *_3;
	HashPosition _2;
	zval *_0;
	zval *type, *key = NULL, *val = NULL, *_1, **_4, *_5 = NULL, *_6 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type);



	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 13);
	add_assoc_stringl_ex(_0, SS("^_"), SL("s"), 1);
	add_assoc_stringl_ex(_0, SS("BYTEA|BLOB|BIN"), SL("bin"), 1);
	add_assoc_stringl_ex(_0, SS("TEXT|CHAR|POINT|INTERVAL"), SL("s"), 1);
	add_assoc_stringl_ex(_0, SS("YEAR|BYTE|COUNTER|SERIAL|INT|LONG|SHORT"), SL("i"), 1);
	add_assoc_stringl_ex(_0, SS("CURRENCY|REAL|MONEY|FLOAT|DOUBLE|DECIMAL|NUMERIC|NUMBER"), SL("f"), 1);
	add_assoc_stringl_ex(_0, SS("^TIME$"), SL("t"), 1);
	add_assoc_stringl_ex(_0, SS("TIME"), SL("t"), 1);
	add_assoc_stringl_ex(_0, SS("DATE"), SL("d"), 1);
	add_assoc_stringl_ex(_0, SS("BOOL"), SL("b"), 1);
	zephir_update_static_property_ce(fastorm_db_dibicolumninfo_ce, SL("patterns"), _0 TSRMLS_CC);
	_1 = zephir_fetch_static_property_ce(fastorm_db_dibicolumninfo_ce, SL("patterns") TSRMLS_CC);
	zephir_is_iterable(_1, &_3, &_2, 0, 0, "fastorm/db/DibiColumnInfo.zep", 31);
	for (
	  ; zephir_hash_get_current_data_ex(_3, (void**) &_4, &_2) == SUCCESS
	  ; zephir_hash_move_forward_ex(_3, &_2)
	) {
		ZEPHIR_GET_HMKEY(key, _3, _2);
		ZEPHIR_GET_HVALUE(val, _4);
		ZEPHIR_INIT_LNVAR(_5);
		ZEPHIR_CONCAT_SVS(_5, "#", key, "#i");
		ZEPHIR_CALL_FUNCTION(&_6, "preg_match", &_7, _5, type);
		zephir_check_call_status();
		if (zephir_is_true(_6)) {
			RETURN_CCTOR(val);
		}
	}
	RETURN_MM_STRING("s", 1);

}

