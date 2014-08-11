
extern zend_class_entry *fastorm_dataobject_ce;

ZEPHIR_INIT_CLASS(Fastorm_DataObject);

PHP_METHOD(Fastorm_DataObject, __construct);
PHP_METHOD(Fastorm_DataObject, getId);
PHP_METHOD(Fastorm_DataObject, setId);
PHP_METHOD(Fastorm_DataObject, setData);
PHP_METHOD(Fastorm_DataObject, getData);
PHP_METHOD(Fastorm_DataObject, offsetExists);
PHP_METHOD(Fastorm_DataObject, offsetGet);
PHP_METHOD(Fastorm_DataObject, offsetSet);
PHP_METHOD(Fastorm_DataObject, offsetUnset);
PHP_METHOD(Fastorm_DataObject, _initialize);
PHP_METHOD(Fastorm_DataObject, _processInitialization);
PHP_METHOD(Fastorm_DataObject, getMetadata);
PHP_METHOD(Fastorm_DataObject, serialize);
PHP_METHOD(Fastorm_DataObject, unserialize);
PHP_METHOD(Fastorm_DataObject, getDataForSerialization);
PHP_METHOD(Fastorm_DataObject, getDbFormatedData);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_setid, 0, 0, 1)
	ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_setdata, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, data, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_offsetexists, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_offsetget, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_offsetset, 0, 0, 2)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_offsetunset, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject__processinitialization, 0, 0, 1)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_getmetadata, 0, 0, 0)
	ZEND_ARG_INFO(0, className)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_unserialize, 0, 0, 1)
	ZEND_ARG_INFO(0, serialized)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_getdataforserialization, 0, 0, 0)
	ZEND_ARG_INFO(0, justClass)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_getdbformateddata, 0, 0, 0)
	ZEND_ARG_INFO(0, withoutAutoincrementKeys)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_dataobject_method_entry) {
	PHP_ME(Fastorm_DataObject, __construct, arginfo_fastorm_dataobject___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_DataObject, getId, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, setId, arginfo_fastorm_dataobject_setid, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, setData, arginfo_fastorm_dataobject_setdata, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, getData, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, offsetExists, arginfo_fastorm_dataobject_offsetexists, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, offsetGet, arginfo_fastorm_dataobject_offsetget, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, offsetSet, arginfo_fastorm_dataobject_offsetset, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, offsetUnset, arginfo_fastorm_dataobject_offsetunset, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, _initialize, NULL, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, _processInitialization, arginfo_fastorm_dataobject__processinitialization, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, getMetadata, arginfo_fastorm_dataobject_getmetadata, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, serialize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, unserialize, arginfo_fastorm_dataobject_unserialize, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, getDataForSerialization, arginfo_fastorm_dataobject_getdataforserialization, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, getDbFormatedData, arginfo_fastorm_dataobject_getdbformateddata, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
