
extern zend_class_entry *fastorm_db_iresultdriver_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_IResultDriver);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_iresultdriver_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_iresultdriver_fetchrow, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_iresultdriver_unescape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, ype)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_iresultdriver_method_entry) {
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, getRowCount, NULL)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, seek, arginfo_fastorm_db_iresultdriver_seek)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, fetchRow, arginfo_fastorm_db_iresultdriver_fetchrow)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, free, NULL)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, getResultColumns, NULL)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, getResultResource, NULL)
	PHP_ABSTRACT_ME(Fastorm_Db_IResultDriver, unescape, arginfo_fastorm_db_iresultdriver_unescape)
  PHP_FE_END
};
