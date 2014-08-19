
namespace Fastorm;


abstract class DataObject implements \ArrayAccess, \Serializable

{
	protected _data;

	protected static _idFieldCache;
	protected static _metadataCache;
	protected static _propCache;

	public function __construct(id = null) {
        let this->_data = [];
        self::_initialize();
        if typeof id === "array" {
            this->setData(id);
        } else {
            if id !== null {
                this->setId(id);
            }
        }
    }

    public function getId() {
		var className, key;
    	let className = get_class(this);
    	let key = self::_idFieldCache[className];
    	if key == null {
    		return null;
    	} else {
    		return this->{key};
    	}
    }

    public function setId(id) {
        var className, key;
    	let className = get_class(this);
    	let key = self::_idFieldCache[className];
    	if key != null {
    		let this->{key} = id;
    	}
    }

    public function setData(array data) {
		var className, key, value;
    	let className = get_class(this);
        
		for key, value in data {
			if isset self::_propCache[className][key] {
	            let this->{key} = value;
	        } else {
	            let this->_data[key] = value;
	        }
		}
    }

    public function getData() {
    	var data, propertyName, propertyFlags, className;
    	let className = get_class(this);
        let data = this->_data;
        for propertyName, propertyFlags  in self::_propCache[className] {
        	let data[propertyName] = this->{propertyName};
        }
        return data;
    }

    public function offsetExists(offset) -> boolean {
        return isset this->{offset} || isset this->_data[offset];
    }

    public function offsetGet(offset) {
        var className;
    	let className = get_class(this);
        if isset self::_propCache[className][offset]{
            return this->{offset};
        } else {
            if isset this->_data[offset] {
            	return this->_data[offset];
            } else {
            	return null;
            }
        }
    }

    public function offsetSet(offset, value) {
        var className;
    	let className = get_class(this);
        if isset self::_propCache[className][offset] {
            let this->{offset} = value;
        } else {
            let this->_data[offset] = value;
        }
    }

    public function offsetUnset(offset) {
        var className;
    	let className = get_class(this);
        if isset self::_propCache[className][offset] {
            let this->{offset} = null;
        } else {
            unset this->_data[offset];
        }
    }


    protected static function _initialize() {
    	var className;
    	let className = get_called_class();
    	if !isset self::_propCache[className] {
            var metadata;
			let metadata = self::_processInitialization(className);
            self::initialize(metadata);
    	}
    }

    protected static function initialize(<ObjectMetadata> metadata) {

    }

    protected static function _processInitialization(string className) -> <ObjectMetadata> {
		var metadata;
		let metadata = self::getMetadata(className);
		let self::_propCache[className] = metadata->getFields();
		let self::_idFieldCache[className] = metadata->getIdField();
        return metadata;
    }

    public static function getMetadata(string className = null) -> <ObjectMetadata>
    {
    	if className === null {
    		let className = get_called_class();
    	}
    	return ObjectMetadata::getMetadata(className);
    }

    public function serialize() {
        var retData, ret;
        let retData = this->getDataForSerialization(false);
        let ret = serialize(retData);
        return ret;
    }

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

    public function getDataForSerialization(boolean justClass = false) -> array
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

    public function getDbFormatedData(boolean withoutAutoincrementKeys = false) -> array {
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