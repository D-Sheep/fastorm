
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Fastorm_Db_IResultDriver) {

	ZEPHIR_REGISTER_INTERFACE(Fastorm\\Db, IResultDriver, fastorm, db_iresultdriver, fastorm_db_iresultdriver_method_entry);

	return SUCCESS;

}

/**
 * Returns the number of rows in a result set.
 * @return int
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getRowCount);

/**
 * Moves cursor position without fetching row.
 * @param  int      the 0-based cursor pos to seek to
 * @return boolean  TRUE on success, FALSE if unable to seek to specified record
 * @throws DibiException
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, seek);

/**
 * Fetches the row at current position and moves the internal cursor to the next position.
 * @param  bool     TRUE for associative array, FALSE for numeric
 * @return array    array on success, nonarray if no next record
 * @internal
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, fetchRow);

/**
 * Frees the resources allocated for this result set.
 * @param  resource  result set resource
 * @return void
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, free);

/**
 * Returns metadata for all columns in a result set.
 * @return array of {name, nativetype [, table, fullname, (int) size, (bool) nullable, (mixed) default, (bool) autoincrement, (array) vendor ]}
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getResultColumns);

/**
 * Returns the result set resource.
 * @return mixed
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, getResultResource);

/**
 * Decodes data from result set.
 * @param  string    value
 * @param  string    type (dibi::BINARY)
 * @return string    decoded value
 * @throws InvalidArgumentException
 */
ZEPHIR_DOC_METHOD(Fastorm_Db_IResultDriver, unescape);

