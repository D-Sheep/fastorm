
extern zend_class_entry *fastorm_db_query_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Query);

PHP_METHOD(Fastorm_Db_Query, __construct);
PHP_METHOD(Fastorm_Db_Query, staticInitialize);
PHP_METHOD(Fastorm_Db_Query, __call);
PHP_METHOD(Fastorm_Db_Query, initialize);
PHP_METHOD(Fastorm_Db_Query, _formatClause);
PHP_METHOD(Fastorm_Db_Query, getCommand);
PHP_METHOD(Fastorm_Db_Query, getConnection);
PHP_METHOD(Fastorm_Db_Query, setupResult);
PHP_METHOD(Fastorm_Db_Query, execute);
PHP_METHOD(Fastorm_Db_Query, fetchRow);
PHP_METHOD(Fastorm_Db_Query, fetchSingle);
PHP_METHOD(Fastorm_Db_Query, fetchAssoc);
PHP_METHOD(Fastorm_Db_Query, fetchPairs);
PHP_METHOD(Fastorm_Db_Query, getIterator);
PHP_METHOD(Fastorm_Db_Query, count);
PHP_METHOD(Fastorm_Db_Query, query);
PHP_METHOD(Fastorm_Db_Query, __toString);
PHP_METHOD(Fastorm_Db_Query, _export);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, connection)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query___call, 0, 0, 2)
	ZEND_ARG_INFO(0, clause)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_initialize, 0, 0, 1)
	ZEND_ARG_INFO(0, clause)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query__formatclause, 0, 0, 1)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_setupresult, 0, 0, 1)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_execute, 0, 0, 0)
	ZEND_ARG_INFO(0, ret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_fetchassoc, 0, 0, 1)
	ZEND_ARG_INFO(0, assoc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_fetchpairs, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_getiterator, 0, 0, 0)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_query, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query__export, 0, 0, 0)
	ZEND_ARG_INFO(0, clause)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_query_method_entry) {
	PHP_ME(Fastorm_Db_Query, __construct, arginfo_fastorm_db_query___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Query, staticInitialize, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Query, __call, arginfo_fastorm_db_query___call, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, initialize, arginfo_fastorm_db_query_initialize, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Query, _formatClause, arginfo_fastorm_db_query__formatclause, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_Db_Query, getCommand, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, getConnection, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, setupResult, arginfo_fastorm_db_query_setupresult, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, execute, arginfo_fastorm_db_query_execute, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchSingle, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchAssoc, arginfo_fastorm_db_query_fetchassoc, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchPairs, arginfo_fastorm_db_query_fetchpairs, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, getIterator, arginfo_fastorm_db_query_getiterator, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, query, arginfo_fastorm_db_query_query, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Query, __toString, NULL, ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, _export, arginfo_fastorm_db_query__export, ZEND_ACC_PROTECTED)
  PHP_FE_END
};
