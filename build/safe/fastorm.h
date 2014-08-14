
/* This file was generated automatically by Zephir do not modify it! */

#ifndef ZEPHIR_CLASS_ENTRIES_H
#define ZEPHIR_CLASS_ENTRIES_H


zend_class_entry *fastorm_connectionmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_ConnectionManager);

static PHP_METHOD(Fastorm_ConnectionManager, __construct);
static PHP_METHOD(Fastorm_ConnectionManager, getConnection);
static PHP_METHOD(Fastorm_ConnectionManager, createConnection);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, defaultConnectionName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager_getconnection, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_connectionmanager_createconnection, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_connectionmanager_method_entry) {
	PHP_ME(Fastorm_ConnectionManager, __construct, arginfo_fastorm_connectionmanager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ConnectionManager, getConnection, arginfo_fastorm_connectionmanager_getconnection, ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ConnectionManager, createConnection, arginfo_fastorm_connectionmanager_createconnection, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
  PHP_FE_END
};

zend_class_entry *fastorm_dataobject_ce;

ZEPHIR_INIT_CLASS(Fastorm_DataObject);

static PHP_METHOD(Fastorm_DataObject, __construct);
static PHP_METHOD(Fastorm_DataObject, getId);
static PHP_METHOD(Fastorm_DataObject, setId);
static PHP_METHOD(Fastorm_DataObject, setData);
static PHP_METHOD(Fastorm_DataObject, getData);
static PHP_METHOD(Fastorm_DataObject, offsetExists);
static PHP_METHOD(Fastorm_DataObject, offsetGet);
static PHP_METHOD(Fastorm_DataObject, offsetSet);
static PHP_METHOD(Fastorm_DataObject, offsetUnset);
static PHP_METHOD(Fastorm_DataObject, _initialize);
static PHP_METHOD(Fastorm_DataObject, initialize);
static PHP_METHOD(Fastorm_DataObject, _processInitialization);
static PHP_METHOD(Fastorm_DataObject, getMetadata);
static PHP_METHOD(Fastorm_DataObject, serialize);
static PHP_METHOD(Fastorm_DataObject, unserialize);
static PHP_METHOD(Fastorm_DataObject, getDataForSerialization);
static PHP_METHOD(Fastorm_DataObject, getDbFormatedData);

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_dataobject_initialize, 0, 0, 1)
	ZEND_ARG_INFO(0, metadata)
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
	PHP_ME(Fastorm_DataObject, initialize, arginfo_fastorm_dataobject_initialize, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, _processInitialization, arginfo_fastorm_dataobject__processinitialization, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, getMetadata, arginfo_fastorm_dataobject_getmetadata, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_DataObject, serialize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, unserialize, arginfo_fastorm_dataobject_unserialize, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, getDataForSerialization, arginfo_fastorm_dataobject_getdataforserialization, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_DataObject, getDbFormatedData, arginfo_fastorm_dataobject_getdbformateddata, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_dbobject_ce;

ZEPHIR_INIT_CLASS(Fastorm_DbObject);

static PHP_METHOD(Fastorm_DbObject, getDbContext);
static PHP_METHOD(Fastorm_DbObject, getSelect);
static PHP_METHOD(Fastorm_DbObject, getDbContextWithMetadata);
static PHP_METHOD(Fastorm_DbObject, getById);
static PHP_METHOD(Fastorm_DbObject, __call);
static PHP_METHOD(Fastorm_DbObject, onBeforeCreate);
static PHP_METHOD(Fastorm_DbObject, onBeforeUpdate);
static PHP_METHOD(Fastorm_DbObject, onAfterUpdate);
static PHP_METHOD(Fastorm_DbObject, create);
static PHP_METHOD(Fastorm_DbObject, save);
static PHP_METHOD(Fastorm_DbObject, update);
static PHP_METHOD(Fastorm_DbObject, delete);
static PHP_METHOD(Fastorm_DbObject, hasMany);
static PHP_METHOD(Fastorm_DbObject, getAffectedRows);

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
	PHP_ME(Fastorm_DbObject, getDbContextWithMetadata, arginfo_fastorm_dbobject_getdbcontextwithmetadata, ZEND_ACC_PRIVATE|ZEND_ACC_STATIC)
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

zend_class_entry *fastorm_exception_ce;

ZEPHIR_INIT_CLASS(Fastorm_Exception);


zend_class_entry *fastorm_iconnectionmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_IConnectionManager);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_iconnectionmanager_getconnection, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_iconnectionmanager_method_entry) {
	PHP_ABSTRACT_ME(Fastorm_IConnectionManager, getConnection, arginfo_fastorm_iconnectionmanager_getconnection)
  PHP_FE_END
};

zend_class_entry *fastorm_modelmanager_ce;

ZEPHIR_INIT_CLASS(Fastorm_ModelManager);

static PHP_METHOD(Fastorm_ModelManager, getShared);
static PHP_METHOD(Fastorm_ModelManager, __construct);
static PHP_METHOD(Fastorm_ModelManager, getConnection);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_modelmanager___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, connectionManager)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_modelmanager_getconnection, 0, 0, 1)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_modelmanager_method_entry) {
	PHP_ME(Fastorm_ModelManager, getShared, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_ModelManager, __construct, arginfo_fastorm_modelmanager___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_ModelManager, getConnection, arginfo_fastorm_modelmanager_getconnection, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_objectmetadata_ce;

ZEPHIR_INIT_CLASS(Fastorm_ObjectMetadata);

static PHP_METHOD(Fastorm_ObjectMetadata, __construct);
static PHP_METHOD(Fastorm_ObjectMetadata, getTable);
static PHP_METHOD(Fastorm_ObjectMetadata, getStorage);
static PHP_METHOD(Fastorm_ObjectMetadata, getIdField);
static PHP_METHOD(Fastorm_ObjectMetadata, getFields);
static PHP_METHOD(Fastorm_ObjectMetadata, getClassName);
static PHP_METHOD(Fastorm_ObjectMetadata, newInstance);
static PHP_METHOD(Fastorm_ObjectMetadata, getReflection);
static PHP_METHOD(Fastorm_ObjectMetadata, getMetadata);
static PHP_METHOD(Fastorm_ObjectMetadata, createMetadata);
static PHP_METHOD(Fastorm_ObjectMetadata, getJoin);
static PHP_METHOD(Fastorm_ObjectMetadata, hasJoin);
static PHP_METHOD(Fastorm_ObjectMetadata, getJoins);
static PHP_METHOD(Fastorm_ObjectMetadata, getAliases);
static PHP_METHOD(Fastorm_ObjectMetadata, toPropertyName);
static PHP_METHOD(Fastorm_ObjectMetadata, makeAlias);
static PHP_METHOD(Fastorm_ObjectMetadata, getAutoincrementKey);

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
	PHP_ME(Fastorm_ObjectMetadata, getAutoincrementKey, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_objectquery_ce;

ZEPHIR_INIT_CLASS(Fastorm_ObjectQuery);

static PHP_METHOD(Fastorm_ObjectQuery, __construct);
static PHP_METHOD(Fastorm_ObjectQuery, initiateSelect);
static PHP_METHOD(Fastorm_ObjectQuery, getMetadata);
static PHP_METHOD(Fastorm_ObjectQuery, joinObject);
static PHP_METHOD(Fastorm_ObjectQuery, joinAll);
static PHP_METHOD(Fastorm_ObjectQuery, joinObjectWith);
static PHP_METHOD(Fastorm_ObjectQuery, query);

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
	PHP_ME(Fastorm_ObjectQuery, initiateSelect, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_ObjectQuery, getMetadata, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinObject, arginfo_fastorm_objectquery_joinobject, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinAll, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_ObjectQuery, joinObjectWith, arginfo_fastorm_objectquery_joinobjectwith, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_ObjectQuery, query, arginfo_fastorm_objectquery_query, ZEND_ACC_PROTECTED)
  PHP_FE_END
};

zend_class_entry *fastorm_db_columninfo_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_ColumnInfo);

static PHP_METHOD(Fastorm_Db_ColumnInfo, detectType);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_columninfo_detecttype, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_columninfo_method_entry) {
	PHP_ME(Fastorm_Db_ColumnInfo, detectType, arginfo_fastorm_db_columninfo_detecttype, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
  PHP_FE_END
};

zend_class_entry *fastorm_db_connection_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Connection);

static PHP_METHOD(Fastorm_Db_Connection, __construct);
static PHP_METHOD(Fastorm_Db_Connection, __destruct);
static PHP_METHOD(Fastorm_Db_Connection, connect);
static PHP_METHOD(Fastorm_Db_Connection, disconnect);
static PHP_METHOD(Fastorm_Db_Connection, isConnected);
static PHP_METHOD(Fastorm_Db_Connection, getConfig);
static PHP_METHOD(Fastorm_Db_Connection, getDriver);
static PHP_METHOD(Fastorm_Db_Connection, query);
static PHP_METHOD(Fastorm_Db_Connection, translate);
static PHP_METHOD(Fastorm_Db_Connection, translateArgs);
static PHP_METHOD(Fastorm_Db_Connection, nativeQuery);
static PHP_METHOD(Fastorm_Db_Connection, getAffectedRows);
static PHP_METHOD(Fastorm_Db_Connection, getInsertId);
static PHP_METHOD(Fastorm_Db_Connection, begin);
static PHP_METHOD(Fastorm_Db_Connection, commit);
static PHP_METHOD(Fastorm_Db_Connection, rollback);
static PHP_METHOD(Fastorm_Db_Connection, createResultSet);
static PHP_METHOD(Fastorm_Db_Connection, command);
static PHP_METHOD(Fastorm_Db_Connection, select);
static PHP_METHOD(Fastorm_Db_Connection, update);
static PHP_METHOD(Fastorm_Db_Connection, insert);
static PHP_METHOD(Fastorm_Db_Connection, delete);
static PHP_METHOD(Fastorm_Db_Connection, addEvent);
static PHP_METHOD(Fastorm_Db_Connection, onEvent);
static PHP_METHOD(Fastorm_Db_Connection, event);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_getconfig, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_query, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_translate, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_translateargs, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_nativequery, 0, 0, 1)
	ZEND_ARG_INFO(0, sql)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_getinsertid, 0, 0, 0)
	ZEND_ARG_INFO(0, sequence)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_begin, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_commit, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_rollback, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_createresultset, 0, 0, 1)
	ZEND_ARG_INFO(0, resultDriver)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_select, 0, 0, 1)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_update, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_insert, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_addevent, 0, 0, 1)
	ZEND_ARG_INFO(0, eventHandler)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_onevent, 0, 0, 0)
	ZEND_ARG_INFO(0, event)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_connection_event, 0, 0, 1)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_connection_method_entry) {
	PHP_ME(Fastorm_Db_Connection, __construct, arginfo_fastorm_db_connection___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Connection, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(Fastorm_Db_Connection, connect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, disconnect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, isConnected, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getConfig, arginfo_fastorm_db_connection_getconfig, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getDriver, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, query, arginfo_fastorm_db_connection_query, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, translate, arginfo_fastorm_db_connection_translate, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, translateArgs, arginfo_fastorm_db_connection_translateargs, ZEND_ACC_PRIVATE)
	PHP_ME(Fastorm_Db_Connection, nativeQuery, arginfo_fastorm_db_connection_nativequery, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getAffectedRows, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, getInsertId, arginfo_fastorm_db_connection_getinsertid, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, begin, arginfo_fastorm_db_connection_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, commit, arginfo_fastorm_db_connection_commit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, rollback, arginfo_fastorm_db_connection_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, createResultSet, arginfo_fastorm_db_connection_createresultset, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, command, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, select, arginfo_fastorm_db_connection_select, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, update, arginfo_fastorm_db_connection_update, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, insert, arginfo_fastorm_db_connection_insert, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, delete, arginfo_fastorm_db_connection_delete, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, addEvent, arginfo_fastorm_db_connection_addevent, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Connection, onEvent, arginfo_fastorm_db_connection_onevent, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_Db_Connection, event, arginfo_fastorm_db_connection_event, ZEND_ACC_PROTECTED)
  PHP_FE_END
};

zend_class_entry *fastorm_db_dbexception_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_DbException);

static PHP_METHOD(Fastorm_Db_DbException, __construct);
static PHP_METHOD(Fastorm_Db_DbException, getSql);
static PHP_METHOD(Fastorm_Db_DbException, getArgs);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_dbexception___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, exception)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, previous)
	ZEND_ARG_INFO(0, sql)
	ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_dbexception_method_entry) {
	PHP_ME(Fastorm_Db_DbException, __construct, arginfo_fastorm_db_dbexception___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_DbException, getSql, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_DbException, getArgs, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_db_drivers_mysqlidriver_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Drivers_MysqliDriver);

static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __construct);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, connect);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, disconnect);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, query);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInfo);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getInsertId);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, begin);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, commit);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, rollback);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResource);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, createResultDriver);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escape);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, escapeLike);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, unescape);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, applyLimit);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, __destruct);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getRowCount);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, fetchRow);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, seek);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, free);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultColumns);
static PHP_METHOD(Fastorm_Db_Drivers_MysqliDriver, getResultResource);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_connect, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_query, 0, 0, 1)
	ZEND_ARG_INFO(0, sql)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_getinsertid, 0, 0, 1)
	ZEND_ARG_INFO(0, sequence)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_begin, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_commit, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_rollback, 0, 0, 0)
	ZEND_ARG_INFO(0, savepoint)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_createresultdriver, 0, 0, 1)
	ZEND_ARG_INFO(0, resourc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_escape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_escapelike, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, pos)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_unescape, 0, 0, 2)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_applylimit, 0, 0, 3)
	ZEND_ARG_INFO(0, sql)
	ZEND_ARG_INFO(0, limit)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_fetchrow, 0, 0, 1)
	ZEND_ARG_INFO(0, associative)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_drivers_mysqlidriver_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_drivers_mysqlidriver_method_entry) {
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, connect, arginfo_fastorm_db_drivers_mysqlidriver_connect, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, disconnect, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, query, arginfo_fastorm_db_drivers_mysqlidriver_query, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getInfo, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getAffectedRows, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getInsertId, arginfo_fastorm_db_drivers_mysqlidriver_getinsertid, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, begin, arginfo_fastorm_db_drivers_mysqlidriver_begin, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, commit, arginfo_fastorm_db_drivers_mysqlidriver_commit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, rollback, arginfo_fastorm_db_drivers_mysqlidriver_rollback, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, createResultDriver, arginfo_fastorm_db_drivers_mysqlidriver_createresultdriver, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, escape, arginfo_fastorm_db_drivers_mysqlidriver_escape, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, escapeLike, arginfo_fastorm_db_drivers_mysqlidriver_escapelike, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, unescape, arginfo_fastorm_db_drivers_mysqlidriver_unescape, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, applyLimit, arginfo_fastorm_db_drivers_mysqlidriver_applylimit, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, __destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getRowCount, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, fetchRow, arginfo_fastorm_db_drivers_mysqlidriver_fetchrow, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, seek, arginfo_fastorm_db_drivers_mysqlidriver_seek, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, free, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResultColumns, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Drivers_MysqliDriver, getResultResource, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_db_event_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Event);

static PHP_METHOD(Fastorm_Db_Event, __construct);
static PHP_METHOD(Fastorm_Db_Event, done);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_event___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, connection)
	ZEND_ARG_INFO(0, type)
	ZEND_ARG_INFO(0, sqlCommand)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_event_done, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_event_method_entry) {
	PHP_ME(Fastorm_Db_Event, __construct, arginfo_fastorm_db_event___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Event, done, arginfo_fastorm_db_event_done, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_db_iresultdriver_ce;

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

zend_class_entry *fastorm_db_query_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Query);

static PHP_METHOD(Fastorm_Db_Query, __construct);
static PHP_METHOD(Fastorm_Db_Query, staticInitialize);
static PHP_METHOD(Fastorm_Db_Query, __call);
static PHP_METHOD(Fastorm_Db_Query, initialize);
static PHP_METHOD(Fastorm_Db_Query, _formatClause);
static PHP_METHOD(Fastorm_Db_Query, getCommand);
static PHP_METHOD(Fastorm_Db_Query, getConnection);
static PHP_METHOD(Fastorm_Db_Query, setupResult);
static PHP_METHOD(Fastorm_Db_Query, execute);
static PHP_METHOD(Fastorm_Db_Query, fetchFirst);
static PHP_METHOD(Fastorm_Db_Query, fetchSingle);
static PHP_METHOD(Fastorm_Db_Query, fetchAssoc);
static PHP_METHOD(Fastorm_Db_Query, fetchPairs);
static PHP_METHOD(Fastorm_Db_Query, getIterator);
static PHP_METHOD(Fastorm_Db_Query, count);
static PHP_METHOD(Fastorm_Db_Query, query);
static PHP_METHOD(Fastorm_Db_Query, __toString);
static PHP_METHOD(Fastorm_Db_Query, setFlag);
static PHP_METHOD(Fastorm_Db_Query, getFlag);
static PHP_METHOD(Fastorm_Db_Query, _export);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, connection)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query___call, 0, 0, 2)
	ZEND_ARG_INFO(0, clause)
	ZEND_ARG_INFO(0, clauseArgs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_initialize, 0, 0, 1)
	ZEND_ARG_INFO(0, clause)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query__formatclause, 0, 0, 1)
	ZEND_ARG_INFO(0, s)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_setupresult, 0, 0, 1)
	ZEND_ARG_INFO(0, method)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_execute, 0, 0, 0)
	ZEND_ARG_INFO(0, ret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_fetchassoc, 0, 0, 1)
	ZEND_ARG_INFO(0, assoc)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_fetchpairs, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_getiterator, 0, 0, 0)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_query, 0, 0, 1)
	ZEND_ARG_INFO(0, queryArgs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_setflag, 0, 0, 1)
	ZEND_ARG_INFO(0, flag)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query_getflag, 0, 0, 1)
	ZEND_ARG_INFO(0, flag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_query__export, 0, 0, 0)
	ZEND_ARG_INFO(0, clause)
	ZEND_ARG_INFO(0, additionalArgs)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_query_method_entry) {
	PHP_ME(Fastorm_Db_Query, __construct, arginfo_fastorm_db_query___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_Query, staticInitialize, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_Db_Query, __call, arginfo_fastorm_db_query___call, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, initialize, arginfo_fastorm_db_query_initialize, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_Db_Query, _formatClause, arginfo_fastorm_db_query__formatclause, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Fastorm_Db_Query, getCommand, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, getConnection, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, setupResult, arginfo_fastorm_db_query_setupresult, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, execute, arginfo_fastorm_db_query_execute, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchFirst, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchSingle, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchAssoc, arginfo_fastorm_db_query_fetchassoc, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, fetchPairs, arginfo_fastorm_db_query_fetchpairs, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, getIterator, arginfo_fastorm_db_query_getiterator, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, query, arginfo_fastorm_db_query_query, ZEND_ACC_PROTECTED)
	PHP_ME(Fastorm_Db_Query, __toString, NULL, ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, setFlag, arginfo_fastorm_db_query_setflag, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, getFlag, arginfo_fastorm_db_query_getflag, ZEND_ACC_FINAL|ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_Query, _export, arginfo_fastorm_db_query__export, ZEND_ACC_PROTECTED)
  PHP_FE_END
};

zend_class_entry *fastorm_db_result_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Result);

static PHP_METHOD(Fastorm_Db_Result, __construct);
static PHP_METHOD(Fastorm_Db_Result, getResource);
static PHP_METHOD(Fastorm_Db_Result, free);
static PHP_METHOD(Fastorm_Db_Result, getResultDriver);
static PHP_METHOD(Fastorm_Db_Result, seek);
static PHP_METHOD(Fastorm_Db_Result, count);
static PHP_METHOD(Fastorm_Db_Result, getRowCount);
static PHP_METHOD(Fastorm_Db_Result, getIterator);
static PHP_METHOD(Fastorm_Db_Result, fetchRow);
static PHP_METHOD(Fastorm_Db_Result, fetchSingle);
static PHP_METHOD(Fastorm_Db_Result, fetchPairs);
static PHP_METHOD(Fastorm_Db_Result, detectTypes);
static PHP_METHOD(Fastorm_Db_Result, normalize);
static PHP_METHOD(Fastorm_Db_Result, setType);
static PHP_METHOD(Fastorm_Db_Result, getType);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, driver)
	ZEND_ARG_INFO(0, metadata)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_fetchpairs, 0, 0, 0)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_normalize, 0, 0, 1)
	ZEND_ARG_INFO(0, row)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_settype, 0, 0, 2)
	ZEND_ARG_INFO(0, col)
	ZEND_ARG_INFO(0, type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_result_gettype, 0, 0, 1)
	ZEND_ARG_INFO(0, col)
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
  PHP_FE_END
};

zend_class_entry *fastorm_db_resultiterator_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_ResultIterator);

static PHP_METHOD(Fastorm_Db_ResultIterator, __construct);
static PHP_METHOD(Fastorm_Db_ResultIterator, rewind);
static PHP_METHOD(Fastorm_Db_ResultIterator, key);
static PHP_METHOD(Fastorm_Db_ResultIterator, current);
static PHP_METHOD(Fastorm_Db_ResultIterator, next);
static PHP_METHOD(Fastorm_Db_ResultIterator, valid);
static PHP_METHOD(Fastorm_Db_ResultIterator, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_fastorm_db_resultiterator___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(fastorm_db_resultiterator_method_entry) {
	PHP_ME(Fastorm_Db_ResultIterator, __construct, arginfo_fastorm_db_resultiterator___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Fastorm_Db_ResultIterator, rewind, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, key, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, current, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, next, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, valid, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Fastorm_Db_ResultIterator, count, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

zend_class_entry *fastorm_db_translator_ce;

ZEPHIR_INIT_CLASS(Fastorm_Db_Translator);

static PHP_METHOD(Fastorm_Db_Translator, __construct);
static PHP_METHOD(Fastorm_Db_Translator, translate);
static PHP_METHOD(Fastorm_Db_Translator, formatValue);
static PHP_METHOD(Fastorm_Db_Translator, fomattedPairValue);
static PHP_METHOD(Fastorm_Db_Translator, nullEscape);
static PHP_METHOD(Fastorm_Db_Translator, cb);
static PHP_METHOD(Fastorm_Db_Translator, delimite);

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

#endif