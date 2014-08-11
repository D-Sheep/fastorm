
extern zend_class_entry *fastorm_db_columninfo_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_ColumnInfo);

PHP_METHOD(Fastorm_Db_ColumnInfo, getTypeCache);

ZEPHIR_INIT_FUNCS(fastorm_db_columninfo_method_entry) {
	PHP_ME(Fastorm_Db_ColumnInfo, getTypeCache, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};
