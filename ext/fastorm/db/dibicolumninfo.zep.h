
extern zend_class_entry *fastorm_db_dibicolumninfo_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_DibiColumnInfo);

PHP_METHOD(Fastorm_Db_DibiColumnInfo, detectType);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_dibicolumninfo_detecttype, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_dibicolumninfo_method_entry) {
	PHP_ME(Fastorm_Db_DibiColumnInfo, detectType, arginfo_fastorm_db_dibicolumninfo_detecttype, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};
