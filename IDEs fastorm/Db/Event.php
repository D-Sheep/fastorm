<?php


namespace Fastorm\Db;


class Event
{
	/** event type */
	const CONNECT = 1;
	const SELECT = 4;
	const INSERT = 8;
	const DELETE = 16;
	const UPDATE = 32;
	const QUERY = 60; // SELECT | INSERT | DELETE | UPDATE
	const BEGIN = 64;
	const COMMIT = 128;
	const ROLLBACK = 256;
	const TRANSACTION = 448; // BEGIN | COMMIT | ROLLBACK
	const ALL = 1023;

	/** @var DibiConnection */
	public $connection;

	/** @var int */
	public $type;

	/** @var string */
	public $sql;

	/** @var DibiResult|DibiDriverException|null */
	public $result;

	/** @var float */
	public $time;

	/** @var int */
	public $count;

	public static $elapsedTime;

	public static $numOfQueries;

	public static $totalTime;

	public static $lastSql;

	/**
	 * Constructor.
	 * @param \Fastorm\Connection connection
	 * @param type
	 * @param sqlCommand
	 * @return void
	 */
	public function __construct( $connection, $type, $sqlCommand = null){}


	/**
	 * TODO
	 * @param resutl
	 * @return this
	 */
	public function done($result = null){}

}
