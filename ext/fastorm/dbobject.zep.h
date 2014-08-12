
extern zend_class_entry *fastorm_dbobject_ce;

ZEPHIR_INIT_CLASS(Fastorm_DbObject);

PHP_METHOD(Fastorm_DbObject, getDbContext);
PHP_METHOD(Fastorm_DbObject, getSelect);
PHP_METHOD(Fastorm_DbObject, getDbContextWithMetadata);
PHP_METHOD(Fastorm_DbObject, getById);
PHP_METHOD(Fastorm_DbObject, __call);
PHP_METHOD(Fastorm_DbObject, create);
PHP_METHOD(Fastorm_DbObject, save);
PHP_METHOD(Fastorm_DbObject, update);
PHP_METHOD(Fastorm_DbObject, hasMany);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_getdbcontextwithmetadata, 0, 0, 1)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_getbyid, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
	ZEND_ARG_INFO(0, joinAll)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject___call, 0, 0, 2)
	ZEND_ARG_INFO(0, method)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_create, 0, 0, 0)
	ZEND_ARG_INFO(0, insertIgnore)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_update, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, onlyColumns, 1)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_dbobject_method_entry) {
	PHP_ME(Fastorm_DbObject, getDbContext, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getSelect, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getDbContextWithMetadata, arginfo_fastorm_dbobject_getdbcontextwithmetadata, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getById, arginfo_fastorm_dbobject_getbyid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, __call, arginfo_fastorm_dbobject___call, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, create, arginfo_fastorm_dbobject_create, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, save, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, update, arginfo_fastorm_dbobject_update, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, hasMany, NULL, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
  PHP_FE_END
};
