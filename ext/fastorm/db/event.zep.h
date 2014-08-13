
extern zend_class_entry *fastorm_db_event_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Event);

PHP_METHOD(Fastorm_Db_Event, __construct);
PHP_METHOD(Fastorm_Db_Event, done);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_event___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, connection)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, sqlCommand)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_event_done, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_event_method_entry) {
	PHP_ME(Fastorm_Db_Event, __construct, arginfo_fastorm_db_event___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Event, done, arginfo_fastorm_db_event_done, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
