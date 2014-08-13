<?php

namespace Fastorm;


class DataObject
{

    /**
     * Constructor sets data (if are any) to the object.
     * @param array|... id If its array, it assume array key => value. Data are set to the object.
     * @return void
     */
	public function __construct($id = null){}

    /**
     * Returns id of the object.
     * @return integer
     * @return void
     */
    public function getId(){}

    /**
     * Sets id value to id field.
     * @param id
     * @return void
     */
    public function setId(id){}

    /**
     * Sets data to object.
     * @param array data Data (key => value) which should be set.
     * @return void
     */
    public function setData(array $data){}

    /**
     * Returns the data of the object in array key => value. 
     * @return array
     */
    public function getData(){}

    /**
     * Returns metadata of classname in parametr else of this class.
     * @param String className  
     * @return ObjectMetadata
     */
    public static function getMetadata($className = null){}

    /**
     * 
     * @param
     * @return
     */
    public function serialize(){}

    /**
     * 
     * @param serialized
     * @return void
     */
    public function unserialize($serialized) {  }

    /**
     * 
     * @param bool justClass
     * @return array 
     */
    public function getDataForSerialization(bool $justClass = false) {}

    /**
     * 
     * @param boolean withoutAutoincrementKeys
     * @return array
     */
    public function getDbFormatedData(boolean $withoutAutoincrementKeys = false) {}

}