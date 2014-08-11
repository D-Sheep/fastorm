
extern zend_class_entry *fastorm_db_drivers_mysqlidriver_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Drivers_MysqliDriver);

PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __construct);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, connect);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, disconnect);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, query);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInfo);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInsertId);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, begin);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, commit);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, rollback);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResource);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, createResultDriver);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escape);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escapeLike);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, unescape);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, applyLimit);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __destruct);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getRowCount);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, fetchRow);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, seek);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, free);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultColumns);
PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultResource);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_connect, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, config, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_query, 0, 0, 1)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_getinsertid, 0, 0, 1)
	ZEND_ARG_INFO(0, sequence)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_begin, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_commit, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_rollback, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_createresultdriver, 0, 0, 1)
	ZEND_ARG_INFO(0, resourc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_escape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_escapelike, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, pos)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_unescape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_applylimit, 0, 0, 3)
	ZEND_ARG_INFO(0, sql)
	ZEND_ARG_INFO(0, limit)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_fetchrow, 0, 0, 1)
	ZEND_ARG_INFO(0, associative)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_drivers_mysqlidriver_method_entry) {
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, connect, arginfo_fastorm_db_drivers_mysqlidriver_connect, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, disconnect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, query, arginfo_fastorm_db_drivers_mysqlidriver_query, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getInfo, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getInsertId, arginfo_fastorm_db_drivers_mysqlidriver_getinsertid, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, begin, arginfo_fastorm_db_drivers_mysqlidriver_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, commit, arginfo_fastorm_db_drivers_mysqlidriver_commit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, rollback, arginfo_fastorm_db_drivers_mysqlidriver_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, createResultDriver, arginfo_fastorm_db_drivers_mysqlidriver_createresultdriver, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, escape, arginfo_fastorm_db_drivers_mysqlidriver_escape, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, escapeLike, arginfo_fastorm_db_drivers_mysqlidriver_escapelike, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, unescape, arginfo_fastorm_db_drivers_mysqlidriver_unescape, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, applyLimit, arginfo_fastorm_db_drivers_mysqlidriver_applylimit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getRowCount, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, fetchRow, arginfo_fastorm_db_drivers_mysqlidriver_fetchrow, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, seek, arginfo_fastorm_db_drivers_mysqlidriver_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, free, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResultColumns, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResultResource, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
