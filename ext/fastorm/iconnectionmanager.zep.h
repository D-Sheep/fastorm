
extern zend_class_entry *fastorm_iconnectionmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_IConnectionManager);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_iconnectionmanager_getconnection, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_iconnectionmanager_method_entry) {
	PHP_ABSTRACT_ME(Fastorm_IConnectionManager, getConnection, arginfo_fastorm_iconnectionmanager_getconnection)
  PHP_FE_END
};
