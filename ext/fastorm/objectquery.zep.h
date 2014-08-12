
extern zend_class_entry *fastorm_objectquery_ce;

ZEPHIR_INIT_CLASS(Fastorm_ObjectQuery);

PHP_METHOD(Fastorm_ObjectQuery, __construct);
PHP_METHOD(Fastorm_ObjectQuery, initiate);
PHP_METHOD(Fastorm_ObjectQuery, getMetadata);
PHP_METHOD(Fastorm_ObjectQuery, joinObject);
PHP_METHOD(Fastorm_ObjectQuery, joinAll);
PHP_METHOD(Fastorm_ObjectQuery, joinObjectWith);
PHP_METHOD(Fastorm_ObjectQuery, query);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectquery___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, connection)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectquery_joinobject, 0, 0, 1)
	ZEND_ARG_INFO(0, onProperty)
	ZEND_ARG_ARRAY_INFO(0, justColumns, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectquery_joinobjectwith, 0, 0, 3)
	ZEND_ARG_INFO(0, property)
	ZEND_ARG_INFO(0, joinMetadata)
	ZEND_ARG_INFO(0, joinAlias)
	ZEND_ARG_INFO(0, justColumns)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_objectquery_query, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_objectquery_method_entry) {
	PHP_ME(Fastorm_ObjectQuery, __construct, arginfo_fastorm_objectquery___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ObjectQuery, initiate, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_ObjectQuery, getMetadata, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinObject, arginfo_fastorm_objectquery_joinobject, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinAll, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinObjectWith, arginfo_fastorm_objectquery_joinobjectwith, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_ObjectQuery, query, arginfo_fastorm_objectquery_query, ZEND_ACC_PROTECTED)
  PHP_FE_END
};
