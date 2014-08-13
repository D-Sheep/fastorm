<?php

namespace Fastorm\Db;

class Result {

	/**
	 * Constructor.
	 * @param \Fastorm\IResultDriver driver
	 * @param \Fastorm\ObjectMetadata metadata
	 * @return void
	 */
	public function __construct(<IResultDriver> driver, <\Fastorm\ObjectMetadata> metadata = null);

	/**
	 * Returns the result set resource.
	 * @return mixed
	 */
	public function getResource();


	/**
	 * Frees the resources allocated for this result set.
	 * @return void
	 */
	public function free();


	/**
	 * Safe access to property driver.
	 * @return IResultDriver
	 * @throws Exception
	 */
	public function getResultDriver();


	

	/**
	 * Moves cursor position without fetching row.
	 * @param  int      the 0-based cursor pos to seek to
	 * @return boolean  true on success, false if unable to seek to specified record
	 * @throws DbException
	 */
	public function seek(row);


	/**
	 * Required by the Countable interface.
	 * @return int
	 */
	public function count();


	/**
	 * Returns the number of rows in a result set.
	 * @return int
	 */
	public function getRowCount();


	/**
	 * Required by the IteratorAggregate interface.
	 * @return ResultIterator
	 */
	public function getIterator();
	


	/**
	 * Fetches the row at current position, process optional type conversion.
	 * and moves the internal cursor to the next position
	 * @return array    array on success, nonarray if no next record
	 */
	public function fetchRow();


	/**
	 * Like fetchRow(), but returns only first field.
	 * @return mixed  value on success, false if no next record
	 */
	public function fetchSingle();


	
	/**
	 * Fetches all records from table like key => value pairs.
	 * @param  string key associative key
	 * @param  string  value
	 * @return array
	 * @throws Exception
	 */
	public function fetchPairs(key = null, value = null);


	/**
	 * Define column type.
	 * @param  string col column
	 * @param  string type type (use constant Dibi::*)
	 * @return self
	 */
	public function setType(col, type);


	/**
	 * Returns column type.
	 * @return string|null
	 */
	public function getType(col);
	
	
}