<?php

namespace Fastorm;


class DataObject
{

    /**
     * Constructor sets data (if are any) to the object.
     * @param array|... id If its array, it assume array key => value. Data are set to the object.
     * @return void
     */
	public function __construct($id = null);

    /**
     * Returns id of the object.
     * @return integer
     * @return void
     */
    public function getId();

    /**
     * Sets id value to id field.
     * @param id
     * @return void
     */
    public function setId(id);

    /**
     * Sets data to object.
     * @param array data Data (key => value) which should be set.
     * @return void
     */
    public function setData(array data);

    /**
     * Returns the data of the object in array key => value. 
     * @return array
     */
    public function getData();

    /**
     * TODO - udělat až to bude
     * @param ObjectMetadata metadata
     * @return
     */
    protected static function initialize(metadata) {

    }

    /**
     * 
     * @param
     * @return ObjectMetadata
     */
    protected static function _processInitialization(string className);
    {
		var metadata;
		let metadata = self::getMetadata(className);
		let self::_propCache[className] = metadata->getFields();
		let self::_idFieldCache[className] = metadata->getIdField();
        return metadata;
    }

    /**
     * Returns metadata of classname in parametr else of this class.
     * @param String className  
     * @return ObjectMetadata
     */
    public static function getMetadata(className = null);

    /**
     * 
     * @param
     * @return
     */
    public function serialize() {
        return serialize(this->getDataForSerialization(false));
    }

    /**
     * 
     * @param
     * @return
     */
    public function unserialize(serialized) {
    	var unserialized, key, value, className;
    	let className = get_class(this);
		let unserialized = unserialize(serialized);
        for key, value in unserialized {
            
            if preg_match("/[1-2][0-9]{3}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}/", value) {
            	let value = new \DateTime(value);
            }

            if isset self::_propCache[className][key] {
	            let this->{key} = value;
	        } else {
	            let this->_data[key] = value;
	        }
        }
    }

    /**
     * 
     * @param
     * @return
     */
    public function getDataForSerialization(bool justClass = false) -> array
    {
    	var key, value, iterate;
    	array data;
        let data = [];
        if justClass {
        	let iterate = this->getDbFormatedData();
        } else {
        	let iterate = this->getData();
        }
        for key, value in iterate {
            if value instanceof \DateTime {
                let data[key] = value->format("Y-m-d H:i:s");
            } else {
                let data[key] = value;
            }
        }
        return data;
    }

    /**
     * 
     * @param
     * @return array
     */
    public function getDbFormatedData(boolean withoutAutoincrementKeys = false);
     {
    	var propName, propFlag, className;
    	array data;
    	let className = get_class(this);
        let data = [];

        for propName, propFlag in self::_propCache[className] {
    		if !withoutAutoincrementKeys || !(propFlag & ObjectMetadata::AUTOINCREMENT) {
				let data[propName] = this->{propName};
    		}
        }

        return data;
    }

}