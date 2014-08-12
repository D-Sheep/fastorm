
extern zend_class_entry *fastorm_connectionmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_ConnectionManager);

PHP_METHOD(Fastorm_ConnectionManager, __construct);
PHP_METHOD(Fastorm_ConnectionManager, getConnection);
PHP_METHOD(Fastorm_ConnectionManager, createConnection);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, defaultConnectionName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager_getconnection, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager_createconnection, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_connectionmanager_method_entry) {
	PHP_ME(Fastorm_ConnectionManager, __construct, arginfo_fastorm_connectionmanager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ConnectionManager, getConnection, arginfo_fastorm_connectionmanager_getconnection, ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ConnectionManager, createConnection, arginfo_fastorm_connectionmanager_createconnection, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
  PHP_FE_END
};
