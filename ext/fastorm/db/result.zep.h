
extern zend_class_entry *fastorm_db_result_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Result);

PHP_METHOD(Fastorm_Db_Result, __construct);
PHP_METHOD(Fastorm_Db_Result, getResource);
PHP_METHOD(Fastorm_Db_Result, free);
PHP_METHOD(Fastorm_Db_Result, getResultDriver);
PHP_METHOD(Fastorm_Db_Result, seek);
PHP_METHOD(Fastorm_Db_Result, count);
PHP_METHOD(Fastorm_Db_Result, getRowCount);
PHP_METHOD(Fastorm_Db_Result, getIterator);
PHP_METHOD(Fastorm_Db_Result, fetchRow);
PHP_METHOD(Fastorm_Db_Result, fetchSingle);
PHP_METHOD(Fastorm_Db_Result, fetchPairs);
PHP_METHOD(Fastorm_Db_Result, detectTypes);
PHP_METHOD(Fastorm_Db_Result, normalize);
PHP_METHOD(Fastorm_Db_Result, setType);
PHP_METHOD(Fastorm_Db_Result, getType);
PHP_METHOD(Fastorm_Db_Result, setFormat);
PHP_METHOD(Fastorm_Db_Result, getFormat);
PHP_METHOD(Fastorm_Db_Result, getInfo);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, driver)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_fetchpairs, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_normalize, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, row, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_settype, 0, 0, 2)
	ZEND_ARG_INFO(0, col)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_gettype, 0, 0, 1)
	ZEND_ARG_INFO(0, col)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_setformat, 0, 0, 2)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, format)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_getformat, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_result_method_entry) {
	PHP_ME(Fastorm_Db_Result, __construct, arginfo_fastorm_db_result___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Result, getResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, free, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getResultDriver, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, seek, arginfo_fastorm_db_result_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getRowCount, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getIterator, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, fetchRow, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, fetchSingle, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, fetchPairs, arginfo_fastorm_db_result_fetchpairs, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, detectTypes, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Result, normalize, arginfo_fastorm_db_result_normalize, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Result, setType, arginfo_fastorm_db_result_settype, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getType, arginfo_fastorm_db_result_gettype, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, setFormat, arginfo_fastorm_db_result_setformat, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getFormat, arginfo_fastorm_db_result_getformat, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Result, getInfo, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
