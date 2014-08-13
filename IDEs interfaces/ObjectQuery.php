<?php


namespace Fastorm;

class ObjectQuery extends Db\Query
{

	/**
     * Constructor.
     * @param \Fastorm\Connection connection
      * @param \Fastorm\ObjectMetadata metadata 
     * @return
     */
	public function __construct(<Connection> connection, <ObjectMetadata> metadata);

	/**
     * Returns metadata of this object.
     * @return ObjectMetadata
     */
	public function getMetadata();

	/**
     * TODO
     * @param String onProperty
     * @param array  justColumns
     * @return ObjectQuery
     */
	public function joinObject(onProperty, justColumns = null) -> <ObjectQuery>
	{
		var property, joinMetadata, joinAlias;
		if this->metadata->hasJoin(onProperty."_id") {
			let property = onProperty."_id";
			let joinAlias = onProperty;
			let joinMetadata = this->metadata->getJoin(property);
		} else {
			let property = onProperty;
			let joinAlias = preg_replace("/_id$/", "", onProperty);
			let joinMetadata = this->metadata->getJoin(property);
		}

		if joinMetadata === null {
			throw new Exception("Property '".onProperty."' hasn't defined join.");
		}

		this->joinObjectWith(property, joinMetadata, joinAlias, justColumns);
		
		return this;
	}

	/**
     * Joins all related models in relation belongs to.
     * @return ObjectQuery
     */
	public function joinAll();

	/**
     * Joins refered table.
     * @param property Name of column in refered table on which is join. 
     * @param joinMetadata Metadata of joined table
     * @param joinAlias Alias of joined table.
     * @param array justColumns Joins just thise columns.
     */
	private function joinObjectWith(property, joinMetadata, joinAlias, justColumns = null);

	/**
     * TODO
     * @param 
     * @return
     */
	protected function query(args)
	{
		var res, setup;
		let res = this->connection->query(args, this->metadata);
		for setup in this->setups {
			call_user_func_array([ res, array_shift(setup) ], setup);
		}
		return res;
	}



}