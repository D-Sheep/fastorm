
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_ColumnInfo) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ColumnInfo, fastorm, db_columninfo, fastorm_db_columninfo_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Fastorm_Db_ColumnInfo, getTypeCache) {



}

