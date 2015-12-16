
namespace Fastorm;


abstract class DataObject implements \ArrayAccess, \Serializable

{
	protected _data;
    protected _myClassName;

	protected static _idFieldCache;
	protected static _propCache;

	public function __construct(var id = null) {

        var key, flag;

        let this->_myClassName = get_class(this);

        if !isset self::_propCache[this->_myClassName] {
            self::_initialize(this->_myClassName);
        }

        if typeof id === "array" {
            var setArray;
            let setArray = id;
            
            for key, flag in self::_propCache[this->_myClassName] {
                
                if isset id[key] {
                    let this->{key} = setArray[key];
                }
                if isset id[key] {
                    unset setArray[key];
                }
            }

            let this->_data = setArray;
        } else {
            let this->_data = [];
            if id !== null {
                this->setId(id);
            }
        }
    }

    public function getId() {
		var key;
    	let key = self::_idFieldCache[this->_myClassName];
    	if key == null {
    		return null;
    	} else {
    		return this->{key};
    	}
    }

    public function setId(var idKey) {
        var key;
    	let key = self::_idFieldCache[this->_myClassName];
    	if key != null {
    		let this->{key} = idKey;
    	}
    }

    public function setData(var data) {
		var key, value;

		for key, value in data {
			if isset self::_propCache[this->_myClassName][key] {
	            let this->{key} = value;
	        } else {
	            let this->_data[key] = value;
	        }
		}
    }

    public function getData() {
    	var data, propertyName, propertyFlags, className;
    	let className = this->_myClassName;
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
    	let className = this->_myClassName;
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
    	let className = this->_myClassName;
        if isset self::_propCache[className][offset] {
            let this->{offset} = value;
        } else {
            let this->_data[offset] = value;
        }
    }

    public function offsetUnset(offset) {
        var className;
    	let className = this->_myClassName;
        if isset self::_propCache[className][offset] {
            let this->{offset} = null;
        } else {
            unset this->_data[offset];
        }
    }


    protected static function _initialize(string className) {
    	var metadata;
        let metadata = self::getMetadata(className);
        let self::_propCache[className] = metadata->getFields();
        let self::_idFieldCache[className] = metadata->getIdField();
        self::initialize(metadata);
    }

    protected static function initialize(<ObjectMetadata> metadata) {

    }

    public static function getMetadata(string className = null) -> <ObjectMetadata>
    {
    	if className === null {
    		let className = get_called_class();
    	}
    	return ObjectMetadata::getMetadata(className);
    }

    public function serialize() {
        var key, value, data, propertyName, propertyFlags, className;
        let className = this->_myClassName;

        let data = this->_data;
        for propertyName, propertyFlags in self::_propCache[className] {
            let data[propertyName] = this->{propertyName};
        }

        for key, value in data {
            if typeof value === "object" && value instanceof \DateTime {
                let data[key] = value->format("Y-m-d H:i:s");
            } 
        }

        return serialize(data);
    }

    public function unserialize(serialized) {
    	var unserialized, key, value, className, tmp;
        
        let this->_myClassName = get_class(this);

        if !isset self::_propCache[this->_myClassName] {
            self::_initialize(this->_myClassName);
        }

    	let className = this->_myClassName;
		let unserialized = unserialize(serialized);
        for key, value in unserialized {
            let tmp = value;
            if preg_match("/[1-2][0-9]{3}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}/", tmp) && !(tmp instanceof \DateTime) {
            	let value = new \DateTime(tmp);
            }

            if isset self::_propCache[className][key] {
	            let this->{key} = value;
	        } else {
	            let this->_data[key] = value;
	        }
        }


    }

    public function getDataForSerialization(boolean justClass = false)
    {
    	var key, value, iterate, data;
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

    public function getDbFormatedData(boolean withoutAutoincrementKeys = false) {
    	var propName, propFlag, className, data;
    	let className = this->_myClassName;
        let data = [];

        for propName, propFlag in self::_propCache[className] {
    		if !withoutAutoincrementKeys || !(propFlag & ObjectMetadata::AUTOINCREMENT) {
				let data[propName] = this->{propName};
    		}
        }

        return data;
    }

}