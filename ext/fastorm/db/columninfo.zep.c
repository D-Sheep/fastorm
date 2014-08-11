
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_ColumnInfo) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ColumnInfo, fastorm, db_columninfo, fastorm_db_columninfo_method_entry, 0);

	zend_declare_property_null(fastorm_db_columninfo_ce, SL("patterns"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

/**
 * Heuristic type detection.
 * @param  string
 * @return string
 * @internal
 */
PHP_METHOD(Fastorm_Db_ColumnInfo, detectType) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_8 = NULL;
	HashTable *_4;
	HashPosition _3;
	zval *_1;
	zval *type, *_0, *key = NULL, *val = NULL, *_2, **_5, *_6 = NULL, *_7 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type);



	_0 = zephir_fetch_static_property_ce(fastorm_db_columninfo_ce, SL("patterns") TSRMLS_CC);
	if (ZEPHIR_IS_EMPTY(_0)) {
		ZEPHIR_INIT_VAR(_1);
		array_init_size(_1, 12);
		add_assoc_stringl_ex(_1, SS("^_"), SL("s"), 1);
		add_assoc_stringl_ex(_1, SS("BYTEA|BLOB|BIN"), SL("bin"), 1);
		add_assoc_stringl_ex(_1, SS("TEXT|CHAR|POINT|INTERVAL"), SL("s"), 1);
		add_assoc_stringl_ex(_1, SS("YEAR|BYTE|COUNTER|SERIAL|INT|LONG|SHORT"), SL("i"), 1);
		add_assoc_stringl_ex(_1, SS("CURRENCY|REAL|MONEY|FLOAT|DOUBLE|DECIMAL|NUMERIC|NUMBER"), SL("f"), 1);
		add_assoc_stringl_ex(_1, SS("^TIME$"), SL("t"), 1);
		add_assoc_stringl_ex(_1, SS("TIME"), SL("t"), 1);
		add_assoc_stringl_ex(_1, SS("DATE"), SL("d"), 1);
		add_assoc_stringl_ex(_1, SS("BOOL"), SL("b"), 1);
		zephir_update_static_property_ce(fastorm_db_columninfo_ce, SL("patterns"), _1 TSRMLS_CC);
	}
	_2 = zephir_fetch_static_property_ce(fastorm_db_columninfo_ce, SL("patterns") TSRMLS_CC);
	zephir_is_iterable(_2, &_4, &_3, 0, 0, "fastorm/db/ColumnInfo.zep", 36);
	for (
	  ; zephir_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
	  ; zephir_hash_move_forward_ex(_4, &_3)
	) {
		ZEPHIR_GET_HMKEY(key, _4, _3);
		ZEPHIR_GET_HVALUE(val, _5);
		ZEPHIR_INIT_LNVAR(_6);
		ZEPHIR_CONCAT_SVS(_6, "#", key, "#i");
		ZEPHIR_CALL_FUNCTION(&_7, "preg_match", &_8, _6, type);
		zephir_check_call_status();
		if (zephir_is_true(_7)) {
			RETURN_CCTOR(val);
		}
	}
	RETURN_MM_STRING("s", 1);

}

