
extern zend_class_entry *fastorm_db_resultiterator_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_ResultIterator);

PHP_METHOD(Fastorm_Db_ResultIterator, __construct);
PHP_METHOD(Fastorm_Db_ResultIterator, rewind);
PHP_METHOD(Fastorm_Db_ResultIterator, key);
PHP_METHOD(Fastorm_Db_ResultIterator, current);
PHP_METHOD(Fastorm_Db_ResultIterator, next);
PHP_METHOD(Fastorm_Db_ResultIterator, valid);
PHP_METHOD(Fastorm_Db_ResultIterator, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_resultiterator___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_resultiterator_method_entry) {
	PHP_ME(Fastorm_Db_ResultIterator, __construct, arginfo_fastorm_db_resultiterator___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_ResultIterator, rewind, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, key, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, current, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, next, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, valid, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, count, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
