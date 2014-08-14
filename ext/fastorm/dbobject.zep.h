
extern zend_class_entry *fastorm_dbobject_ce;

ZEPHIR_INIT_CLASS(Fastorm_DbObject);

PHP_METHOD(Fastorm_DbObject, getDbContext);
PHP_METHOD(Fastorm_DbObject, getSelect);
PHP_METHOD(Fastorm_DbObject, getDbContextWithMetadata);
PHP_METHOD(Fastorm_DbObject, getById);
PHP_METHOD(Fastorm_DbObject, __call);
PHP_METHOD(Fastorm_DbObject, onBeforeCreate);
PHP_METHOD(Fastorm_DbObject, onBeforeUpdate);
PHP_METHOD(Fastorm_DbObject, onAfterUpdate);
PHP_METHOD(Fastorm_DbObject, create);
PHP_METHOD(Fastorm_DbObject, save);
PHP_METHOD(Fastorm_DbObject, update);
PHP_METHOD(Fastorm_DbObject, delete);
PHP_METHOD(Fastorm_DbObject, hasMany);
PHP_METHOD(Fastorm_DbObject, getAffectedRows);

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_onbeforecreate, 0, 0, 0)
	ZEND_ARG_INFO(0, calledFromSave)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_onbeforeupdate, 0, 0, 0)
	ZEND_ARG_INFO(0, remove)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_onafterupdate, 0, 0, 0)
	ZEND_ARG_INFO(0, removed)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_create, 0, 0, 0)
	ZEND_ARG_INFO(0, insertIgnore)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dbobject_update, 0, 0, 0)
	ZEND_ARG_INFO(0, onlyColumnsArray)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_dbobject_method_entry) {
	PHP_ME(Fastorm_DbObject, getDbContext, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getSelect, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getDbContextWithMetadata, arginfo_fastorm_dbobject_getdbcontextwithmetadata, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getById, arginfo_fastorm_dbobject_getbyid, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, __call, arginfo_fastorm_dbobject___call, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, onBeforeCreate, arginfo_fastorm_dbobject_onbeforecreate, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_DbObject, onBeforeUpdate, arginfo_fastorm_dbobject_onbeforeupdate, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_DbObject, onAfterUpdate, arginfo_fastorm_dbobject_onafterupdate, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_DbObject, create, arginfo_fastorm_dbobject_create, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, save, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, update, arginfo_fastorm_dbobject_update, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, delete, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DbObject, hasMany, NULL, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DbObject, getAffectedRows, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
