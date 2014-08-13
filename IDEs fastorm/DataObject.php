<?php

namespace Fastorm;


class DataObject implements \ArrayAccess, \Serializable
{
    /**
     * (PHP 5 &gt;= 5.0.0)<br/>
     * Whether a offset exists
     * @link http://php.net/manual/en/arrayaccess.offsetexists.php
     * @param mixed $offset <p>
     * An offset to check for.
     * </p>
     * @return boolean true on success or false on failure.
     * </p>
     * <p>
     * The return value will be casted to boolean if non-boolean was returned.
     */
    public function offsetExists($offset)
    {
        // TODO: Implement offsetExists() method.
    }

    /**
     * (PHP 5 &gt;= 5.0.0)<br/>
     * Offset to retrieve
     * @link http://php.net/manual/en/arrayaccess.offsetget.php
     * @param mixed $offset <p>
     * The offset to retrieve.
     * </p>
     * @return mixed Can return all value types.
     */
    public function offsetGet($offset)
    {
        // TODO: Implement offsetGet() method.
    }

    /**
     * (PHP 5 &gt;= 5.0.0)<br/>
     * Offset to set
     * @link http://php.net/manual/en/arrayaccess.offsetset.php
     * @param mixed $offset <p>
     * The offset to assign the value to.
     * </p>
     * @param mixed $value <p>
     * The value to set.
     * </p>
     * @return void
     */
    public function offsetSet($offset, $value)
    {
        // TODO: Implement offsetSet() method.
    }

    /**
     * (PHP 5 &gt;= 5.0.0)<br/>
     * Offset to unset
     * @link http://php.net/manual/en/arrayaccess.offsetunset.php
     * @param mixed $offset <p>
     * The offset to unset.
     * </p>
     * @return void
     */
    public function offsetUnset($offset)
    {
        // TODO: Implement offsetUnset() method.
    }

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