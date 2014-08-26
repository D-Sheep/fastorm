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





}