
extern zend_class_entry *fastorm_modelmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_ModelManager);

PHP_METHOD(Fastorm_ModelManager, getShared);
PHP_METHOD(Fastorm_ModelManager, __construct);
PHP_METHOD(Fastorm_ModelManager, getConnection);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_modelmanager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, connectionManager)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_modelmanager_getconnection, 0, 0, 1)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_modelmanager_method_entry) {
	PHP_ME(Fastorm_ModelManager, getShared, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_ModelManager, __construct, arginfo_fastorm_modelmanager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ModelManager, getConnection, arginfo_fastorm_modelmanager_getconnection, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
