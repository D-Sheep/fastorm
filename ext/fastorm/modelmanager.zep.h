
extern zend_class_entry *fastorm_modelmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_ModelManager);

PHP_METHOD(Fastorm_ModelManager, getShared);

ZEPHIR_INIT_FUNCS(fastorm_modelmanager_method_entry) {
	PHP_ME(Fastorm_ModelManager, getShared, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};
