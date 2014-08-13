
extern zend_class_entry *fastorm_db_connection_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Connection);

PHP_METHOD(Fastorm_Db_Connection, __construct);
PHP_METHOD(Fastorm_Db_Connection, __destruct);
PHP_METHOD(Fastorm_Db_Connection, connect);
PHP_METHOD(Fastorm_Db_Connection, disconnect);
PHP_METHOD(Fastorm_Db_Connection, isConnected);
PHP_METHOD(Fastorm_Db_Connection, getConfig);
PHP_METHOD(Fastorm_Db_Connection, getDriver);
PHP_METHOD(Fastorm_Db_Connection, query);
PHP_METHOD(Fastorm_Db_Connection, translate);
PHP_METHOD(Fastorm_Db_Connection, translateArgs);
PHP_METHOD(Fastorm_Db_Connection, nativeQuery);
PHP_METHOD(Fastorm_Db_Connection, getAffectedRows);
PHP_METHOD(Fastorm_Db_Connection, getInsertId);
PHP_METHOD(Fastorm_Db_Connection, begin);
PHP_METHOD(Fastorm_Db_Connection, commit);
PHP_METHOD(Fastorm_Db_Connection, rollback);
PHP_METHOD(Fastorm_Db_Connection, createResultSet);
PHP_METHOD(Fastorm_Db_Connection, command);
PHP_METHOD(Fastorm_Db_Connection, select);
PHP_METHOD(Fastorm_Db_Connection, update);
PHP_METHOD(Fastorm_Db_Connection, insert);
PHP_METHOD(Fastorm_Db_Connection, delete);
PHP_METHOD(Fastorm_Db_Connection, addEvent);
PHP_METHOD(Fastorm_Db_Connection, onEvent);
PHP_METHOD(Fastorm_Db_Connection, event);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_getconfig, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_query, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_translate, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_translateargs, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_nativequery, 0, 0, 1)
	ZEND_ARG_INFO(0, sql)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_getinsertid, 0, 0, 0)
	ZEND_ARG_INFO(0, sequence)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_begin, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_commit, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_rollback, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_createresultset, 0, 0, 1)
	ZEND_ARG_INFO(0, resultDriver)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_select, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_update, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_insert, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_addevent, 0, 0, 1)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_onevent, 0, 0, 0)
	ZEND_ARG_INFO(0, event)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_event, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_connection_method_entry) {
	PHP_ME(Fastorm_Db_Connection, __construct, arginfo_fastorm_db_connection___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Connection, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(Fastorm_Db_Connection, connect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, disconnect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, isConnected, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getConfig, arginfo_fastorm_db_connection_getconfig, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getDriver, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, query, arginfo_fastorm_db_connection_query, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, translate, arginfo_fastorm_db_connection_translate, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, translateArgs, arginfo_fastorm_db_connection_translateargs, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Connection, nativeQuery, arginfo_fastorm_db_connection_nativequery, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getAffectedRows, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getInsertId, arginfo_fastorm_db_connection_getinsertid, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, begin, arginfo_fastorm_db_connection_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, commit, arginfo_fastorm_db_connection_commit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, rollback, arginfo_fastorm_db_connection_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, createResultSet, arginfo_fastorm_db_connection_createresultset, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, command, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, select, arginfo_fastorm_db_connection_select, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, update, arginfo_fastorm_db_connection_update, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, insert, arginfo_fastorm_db_connection_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, delete, arginfo_fastorm_db_connection_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, addEvent, arginfo_fastorm_db_connection_addevent, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_Db_Connection, onEvent, arginfo_fastorm_db_connection_onevent, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_Db_Connection, event, arginfo_fastorm_db_connection_event, ZEND_ACC_PROTECTED)
  PHP_FE_END
};
