
extern zend_class_entry *fastorm_objectmetadata_ce;

ZEPHIR_INIT_CLASS(Fastorm_ObjectMetadata);

PHP_METHOD(Fastorm_ObjectMetadata, __construct);
PHP_METHOD(Fastorm_ObjectMetadata, getTable);
PHP_METHOD(Fastorm_ObjectMetadata, getStorage);
PHP_METHOD(Fastorm_ObjectMetadata, getIdField);
PHP_METHOD(Fastorm_ObjectMetadata, getFields);
PHP_METHOD(Fastorm_ObjectMetadata, getClassName);
PHP_METHOD(Fastorm_ObjectMetadata, newInstance);
PHP_METHOD(Fastorm_ObjectMetadata, getReflection);
PHP_METHOD(Fastorm_ObjectMetadata, getMetadata);
PHP_METHOD(Fastorm_ObjectMetadata, createMetadata);
PHP_METHOD(Fastorm_ObjectMetadata, getJoin);
PHP_METHOD(Fastorm_ObjectMetadata, hasJoin);
PHP_METHOD(Fastorm_ObjectMetadata, getJoins);
PHP_METHOD(Fastorm_ObjectMetadata, getAliases);
PHP_METHOD(Fastorm_ObjectMetadata, toPropertyName);
PHP_METHOD(Fastorm_ObjectMetadata, makeAlias);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata___construct, 0, 0, 4)
	ZEND_ARG_INFO(0, reflection)
	ZEND_ARG_INFO(0, className)
	ZEND_ARG_ARRAY_INFO(0, fields, 0)
	ZEND_ARG_ARRAY_INFO(0, joins, 0)
	ZEND_ARG_INFO(0, idField)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, storage)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_newinstance, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, data, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_getmetadata, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_createmetadata, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_getjoin, 0, 0, 1)
	ZEND_ARG_INFO(0, propName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_hasjoin, 0, 0, 1)
	ZEND_ARG_INFO(0, propName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_getaliases, 0, 0, 1)
	ZEND_ARG_INFO(0, propName)
	ZEND_ARG_INFO(0, joinedTable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_topropertyname, 0, 0, 1)
	ZEND_ARG_INFO(0, camelcase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectmetadata_makealias, 0, 0, 2)
	ZEND_ARG_INFO(0, sourceProperty)
	ZEND_ARG_INFO(0, targetProperty)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_objectmetadata_method_entry) {
	PHP_ME(Fastorm_ObjectMetadata, __construct, arginfo_fastorm_objectmetadata___construct, ZEND_ACC_PROTECTED|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ObjectMetadata, getTable, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getStorage, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getIdField, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getFields, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getClassName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, newInstance, arginfo_fastorm_objectmetadata_newinstance, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getReflection, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getMetadata, arginfo_fastorm_objectmetadata_getmetadata, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_ObjectMetadata, createMetadata, arginfo_fastorm_objectmetadata_createmetadata, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_ObjectMetadata, getJoin, arginfo_fastorm_objectmetadata_getjoin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, hasJoin, arginfo_fastorm_objectmetadata_hasjoin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getJoins, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, getAliases, arginfo_fastorm_objectmetadata_getaliases, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectMetadata, toPropertyName, arginfo_fastorm_objectmetadata_topropertyname, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_ObjectMetadata, makeAlias, arginfo_fastorm_objectmetadata_makealias, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};
