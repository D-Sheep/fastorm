
extern zend_class_entry *fastorm_db_translator_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Translator);

PHP_METHOD(Fastorm_Db_Translator, __construct);
PHP_METHOD(Fastorm_Db_Translator, translate);
PHP_METHOD(Fastorm_Db_Translator, formatValue);
PHP_METHOD(Fastorm_Db_Translator, fomattedPairValue);
PHP_METHOD(Fastorm_Db_Translator, nullEscape);
PHP_METHOD(Fastorm_Db_Translator, cb);
PHP_METHOD(Fastorm_Db_Translator, delimite);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, connection)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_translate, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_formatvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, modifier)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_fomattedpairvalue, 0, 0, 2)
	ZEND_ARG_INFO(0, pairArray)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_nullescape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, modifier)
	ZEND_ARG_INFO(0, compareWith)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_cb, 0, 0, 1)
	ZEND_ARG_INFO(0, matches)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_translator_delimite, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_translator_method_entry) {
	PHP_ME(Fastorm_Db_Translator, __construct, arginfo_fastorm_db_translator___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Translator, translate, arginfo_fastorm_db_translator_translate, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Translator, formatValue, arginfo_fastorm_db_translator_formatvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Translator, fomattedPairValue, arginfo_fastorm_db_translator_fomattedpairvalue, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Translator, nullEscape, arginfo_fastorm_db_translator_nullescape, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Translator, cb, arginfo_fastorm_db_translator_cb, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Translator, delimite, arginfo_fastorm_db_translator_delimite, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
