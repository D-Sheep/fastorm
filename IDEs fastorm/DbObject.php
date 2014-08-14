<?php

namespace Fastorm;

class DbObject extends DataObject
{
	/**
	 * 
	 * @return Db\Connection
	 */
	public static function getDbContext(){}
	
	/**
	 * 
	 * @return \Fastorm\ObjectQuery
	 */
	public static function getSelect(){}

	/**
	 * 
	 * @param ObjectMetadata metadata
	 * @return Db\Connection
	 */
	private static function getDbContextWithMetadata($metadata){}

	/**
	 *  Returns object with this id.
	 *  @param id Id, which schould be found.
	 *	@param boolean joinAll Wheather should be joined refered tables.
	 *	@param ObjectMetadata metadata Only for powerup (private)
	 *  @return \Fastorm\DbObject
	 */
	public static function getById($id, $joinAll = true, $metadata = null){}

	/**
	 * Defauld method for creating queries.
	 * @param String method 
	 * @param array args
	 * @return null|\Fastorm\DbObject
	 * @return void
	 */
	public function __call($method, $args){}

	/**
	 * Inserts a model instance.
	 * @param boolean insertIgnore If primary key already exists it will fail. 
	 * @return \Fastorm\DbObject
	 */
	public function create(boolean $insertIgnore = false){}

	/**
	 * Inserts or updates a model instance.
	 * @return \Fastorm\DbObject
	 */
	public function save(){}

	/**
	 * Updates a model instance.
	 * @param array onlyColumns Collumns, which should be inserted.
	 * @return \Fastorm\DbObject
	 */
	public function update(array $onlyColumns = null){}

    /**
     * Updates a model instance.
     * @return \Fastorm\DbObject
     */
    public function delete(){}

	/**
	 * Setup a relation 1-n between two models
	 * @param TODO až to bude
	 * @return void
	 */
	protected static function hasMany() {}

	/**
	 * Returns wheather exists affected rows. This method can be used olny after insert, update or remove.
	 * @return boolean
	 */
	public function getAffectedRows(){}



}