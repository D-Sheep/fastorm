
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Fastorm_IConnectionManager) {

	ZEPHIR_REGISTER_INTERFACE(Fastorm, IConnectionManager, fastorm, iconnectionmanager, fastorm_iconnectionmanager_method_entry);

	return SUCCESS;

}

ZEPHIR_DOC_METHOD(Fastorm_IConnectionManager, getConnection);

