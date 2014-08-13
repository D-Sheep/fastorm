<?php

namespace Fastorm\Db;


/**
 *
 *
 * Inspired by David Grudl"s dibi - DibiFLuent
 */
class Query
{
	const REMOVE = false;

	const TYPE_TEXT = "s";
	const TYPE_BINARY = "bin";
	const TYPE_BOOL = "b";
	const TYPE_INTEGER = "i";
	const TYPE_FLOAT = "f";
	const TYPE_DATE = "d";
	const TYPE_DATETIME = "t";
	const TYPE_TIME = "t";

	const IDENTIFIER = "n";

	/** sorting order */
	const ASC = "ASC";
	const DESC = "DESC";

	public static $masks;

	public static $modifiers;

	public static $separators;

	public static $swithes;

	protected $connection;

	protected $setups;

	/**
	 * Constructor.
	 * @param \Fastorm\Connection connection Connection to database.
	 * @return void
	 */
	public function __construct(Connection $connection){}

	/**
	 * Appends new argument to the clause.
	 * @param string clause clause name
	 * @param array args 
	 * @return Query
	 */
	public function __call($clause, $args){}


	/**
	 * Formats order by and group by
	 * @param string s
	 * @return string
	 */
	public static function _formatClause( $s){}

	/**
	 * Returns SQL command.
	 * @return string
	 */
	public function getCommand(){}


	/**
	 * Returns connection.
	 * @return Connection
	 */
	public function getConnection(){}



	/**
	 *
	 * @param method
	 * @return Query
	 */
	public function setupResult($method){}



	/**
	 * Executes query
	 * @param ret TODO
	 * @return Result|int   result set object (if any)
	 */
	public function execute($ret = null){}



	/**
	 * Returns first row fo result
	 * @return Result|int   result set object (if any)
	 */
	public function fetchFirst(){}


	/**
	 * Executes query any returns first field of result
	 * @return Result|int   result set object (if any)
	 */
	public function fetchSingle(){}



	/**
	 * Fetches all records from table and returns associative tree.
	 * @param string assoc Associative descriptod
 	 * @return Result|int   result set object (if any)
	 */
	public function fetchAssoc($assoc){}


	/**
	 * Fetches all records from table like $key => $value pairs. 
	 * @param string key associative key
	 * @param string value 
	 * @return array
	 */
	public function fetchPairs(string $key = null, string $value = null){}


	/**
	 * Iterator over results
	 * @param int offset
	 * @param int limit
	 * @return Fastorm\Db\ResultIterator
	 */
	public function getIterator(offset = null, limit = null){}



	/**
	 * Returns cout of results
	 * @return integer
	 */
	public function count(){}

	/**
	 * Returns SQL query.
	 * @return string
	 */
	final public function __toString(){}

	/**
	 * Change a SQL flag.
	 * @param  string  flag name
	 * @param  bool  value
	 * @return Query  provides a fluent interface
	 */
	public function setFlag($flag, $value = true){}



	/**
	 * Is a flag set?
	 * @param  string  flag name
	 * @return boolean
	 */
	final public function getFlag($flag){}

	/**
	 * Generates parameters for Translator.
	 * @param string clause
	 * @param args
	 * @return array
	 */
	protected function _export($clause = null, $args = null){}


}