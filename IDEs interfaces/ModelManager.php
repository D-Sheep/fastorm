<?php


namespace Fastorm;

class ModelManager
{

	/**
	 * Returns shared instance of this class.
	 * @return this
	 */
	public static function getShared();

	/**
	 * Constructor.
	 * @param \Fastorm\IConnectionManager connectionManager.
	 * @return void
	 */
	public function __construct(<IConnectionManager> connectionManager);

	/**
	 * Returns connection.
	 * @param \Fastorm\ObjectMetadata connection Connection to database.
	 * @return \Fastorm\Db\Connection
	 */
	public function getConnection( metadata);



}