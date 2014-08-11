
extern zend_class_entry *src_testclass_ce;

ZEPHIR_INIT_CLASS(Src_TestClass);

PHP_METHOD(Src_TestClass, __construct);
PHP_METHOD(Src_TestClass, foo);

ZEND_BEGIN_ARG_INFO_EX(arginfo_src_testclass___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, some)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(src_testclass_method_entry) {
	PHP_ME(Src_TestClass, __construct, arginfo_src_testclass___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Src_TestClass, foo, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
