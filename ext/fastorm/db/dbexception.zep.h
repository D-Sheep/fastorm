
extern zend_class_entry *fastorm_db_dbexception_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_DbException);

PHP_METHOD(Fastorm_Db_DbException, __construct);
PHP_METHOD(Fastorm_Db_DbException, getSql);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_dbexception___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, exception)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, previous)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_dbexception_method_entry) {
	PHP_ME(Fastorm_Db_DbException, __construct, arginfo_fastorm_db_dbexception___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_DbException, getSql, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
