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
	public function __construct(Connection $connection, ObjectMetadata $metadata){}

	/**
     * Returns metadata of this object.
     * @return ObjectMetadata
     */
	public function getMetadata(){}

	/**
     * TODO
     * @param String onProperty
     * @param array  justColumns
     * @return $this
     */
	public function joinObject($onProperty, $justColumns = null){}

	/**
     * Joins all related models in relation belongs to.
     * @return $this
     */
	public function joinAll(){}

	/**
     * Joins refered table.
     * @param property Name of column in refered table on which is join. 
     * @param joinMetadata Metadata of joined table
     * @param joinAlias Alias of joined table.
     * @param array justColumns Joins just thise columns.
     */
	private function joinObjectWith($property, $joinMetadata, $joinAlias, $justColumns = null){}

	/**
     * TODO
     * @param 
     * @return
     */
	protected function query($args){}



}