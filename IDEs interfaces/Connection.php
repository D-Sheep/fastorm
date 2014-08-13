<?php

namespace Fastorm\Db;

class Connection {

	public onEvent = [];

	private config = [];

	private driver;

	private translator;

	private connected;


	/**
	 * Connection options: (see driver-specific options too)
	 *   - lazy (bool) => if TRUE, connection will be established only when required
	 *   - result (array) => result set options
	 *       - formatDateTime => date-time format (if empty, DateTime objects will be returned)
	 *   - profiler (array or bool)
	 *       - run (bool) => enable profiler?
	 *       - file => file to log
	 *   - substitutes (array) => map of driver specific substitutes (under development)
	 * @param  mixed config  connection parameters
	 * @param  string name connection name
	 * @throws DibiException
	 */
	public function __construct(config, name = null);



	/**
	 * Automatically frees the resources allocated for this result set.
	 * @return void
	 */
	public function __destruct();



	/**
	 * Connects to a database.
	 * @return void
	 */
	public function connect();



	/**
	 * Disconnects from a database.
	 * @return void
	 */
	public function disconnect();



	/**
	 * Returns TRUE when connection was established.
	 * @return bool
	 */
	public function isConnected();



	/**
	 * Returns configuration variable. If no key is passed, returns the entire array.
	 * @see self::__construct
	 * @param  string key
	 * @param  mixed defaultValue default value to use if key not found
	 * @return mixed
	 */
	public function getConfig(key = null, defaultValue = null);





	/**
	 * Returns the driver and connects to a database in lazy mode.
	 * @return Fastorm\Db\Drivers\MysqliDriver
	 */
	public function getDriver();



	/**
	 * Generates (translates) and executes SQL query.
	 * @param  array|mixed      one or more arguments
	 * @param \Fastorm\ObjectMetadata metadata 
	 * @return Result|int   result set object (if any)
	 * @throws Exception
	 */
	public function query(args, <\Fastorm\ObjectMetadata> metadata = null);



	/**
	 * Generates SQL query.
	 * @param  array|mixed      one or more arguments
	 * @throws DibiException
	 */
	public function translate(args);



	/**
	 * Generates SQL query.
	 * @param  array
	 * @return string
	 */
	private function translateArgs(args);



	/**
	 * Executes the SQL query.
	 * @param  string sql          SQL statement.
	 * @param \Fastorm\ObjectMetadata metadata
	 * @return Result|int    result set object (if any)
	 * @throws Exception
	 */
	public function nativeQuery(sql, \Fastorm\ObjectMetadata metadata = null);



	/**
	 * Gets the number of affected rows by the last INSERT, UPDATE or DELETE query.
	 * @return int  number of rows
	 * @throws Exception
	 */
	public function getAffectedRows();



	/**
	 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous INSERT query.
	 * @param  string     optional sequence name
	 * @return int
	 * @throws Exception
	 */
	public function getInsertId(sequence = null);



	/**
	 * Begins a transaction (if supported).
	 * @param  string savepoint optional savepoint name
	 * @return void
	 */
	public function begin(savepoint = null);



	/**
	 * Commits statements in a transaction.
	 * @param  string savepoint optional savepoint name
	 * @return void
	 */
	public function commit(savepoint = null);



	/**
	 * Rollback changes in a transaction.
	 * @param  string savepoint optional savepoint name
	 * @return void
	 */
	public function rollback(savepoint = NULL);



	/**
	 * Result set factory.
	 * @param  IResultDriver
	 * @param \Fastorm\ObjectMetadata metadata
	 * @return Result
	 */
	public function createResultSet(IResultDriver resultDriver, \Fastorm\ObjectMetadata> metadata = null);





	/**
	 * @return Query
	 */
	public function command();



	/**
	 * @param  string args   column name
	 * @return Query
	 */
	public function select(args);



	/**
	 * @param  string   table
	 * @param  array args
	 * @return Query
	 */
	public function update(table, args);



	/**
	 * @param  string   table
	 * @param  array args
	 * @return Query
	 */
	public function insert(table, args);



	/**
	 * @param  string   table
	 * @return Query
	 */
	public function delete(table);




	

}