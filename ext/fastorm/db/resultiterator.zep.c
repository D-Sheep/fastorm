
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


ZEPHIR_INIT_CLASS(Fastorm_Db_ResultIterator) {

	ZEPHIR_REGISTER_CLASS(Fastorm\\Db, ResultIterator, fastorm, db_resultiterator, NULL, 0);

	return SUCCESS;

}

