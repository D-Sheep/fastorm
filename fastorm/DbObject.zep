
namespace Fastorm;

abstract class DbObject extends DataObject

{
	protected _joinCache;

	protected _affectedRows;

	public static function getDbContext() -> <Db\Connection>
	{
		return self::getDbContextWithMetadata(self::getMetadata());
	}
	
	public static function getSelect() -> <ObjectQuery>
	{
		var metadata;
		let metadata = self::getMetadata();
		return new ObjectQuery(self::getDbContextWithMetadata(metadata), metadata);
	}

	protected static function getDbContextWithMetadata(<ObjectMetadata> metadata) -> <Db\Connection>
	{
		return ModelManager::getShared()->getConnection(metadata);
	}

	/**
	 *	@metadata only for powerup (private)
	 */
	public static function getById(id, boolean joinAll = false, <ObjectMetadata> metadata = null) {
		var select;
		if metadata === null {
			let metadata = self::getMetadata();
		}
		let select = new ObjectQuery(self::getDbContextWithMetadata(metadata), metadata);
		if typeof id === "string" {
			select->__call("where", [metadata->getTable().".".metadata->getIdField()." = %s", id]);
		} else {
			select->__call("where", [metadata->getTable().".".metadata->getIdField()." = %i", id]);
		}
		return select->fetchFirst();
	}

	public function __call(method, args)
	{
		var m, propertyName, isSetter, metadata, joinedMetadata;
		let m = null;
		if preg_match("/^set(.+)$/", method, m) && count(args) === 1 {
			let isSetter = true;
		} else { if preg_match("/^get(.+)$/", method, m) && count(args) === 0 || typeof args[0] === "boolean" {
			let isSetter = false;
		} else {
			throw new Exception("Method '".method."' not implemented");
		}}

		let metadata = self::getMetadata();
		let propertyName = ObjectMetadata::toPropertyName(m[1]);
		if metadata->hasJoin(propertyName."_id") {
			let propertyName = propertyName . "_id";
		} 
		let joinedMetadata = metadata->getJoin(propertyName);

		//var_dump("META", joinedMetadata, propertyName);

		if joinedMetadata === null {
			throw new Exception("Method '".method."' not implemented or target object not defined");
		}

		if isSetter === false { // getter
			if this->{propertyName} === null {
				return null;
			} else {
				var ret, alias;
				let alias = ObjectMetadata::makeAlias(propertyName, joinedMetadata->getIdField());
				if isset this->_data[alias] {
					var key, value;
					let ret = joinedMetadata->newInstance();
					for key, value in joinedMetadata->getFields() {
						let alias = ObjectMetadata::makeAlias(propertyName, key);
						if isset this->_data[alias] {
							let ret->{key} = this->_data[alias];
						}
					}
				} else {
					var className;
					let className = joinedMetadata->getClassName();
					let ret = {className}::getById(this->{propertyName}, false, joinedMetadata);
				}
				if isset args[0] && args[0] === true {
					let this->_joinCache[propertyName] = ret;
				}
				return ret;
			}
		} else { // setter
			if (args[0] instanceof \Fastorm\DataObject) || args[0] === null {
				var idField;
				let idField = joinedMetadata->getIdField();
				let this->{propertyName} = args[0]->{idField};
				if this->_joinCache === null {
					let this->_joinCache = [];
				}
				let this->_joinCache[propertyName] = args[0];
			} else {
				throw new Exception("Object in setter must be Dataobject type");
			}
			return this;
		}

	}

	protected function onBeforeCreate(calledFromSave = false) {

	}

	protected function onBeforeUpdate(remove = false) {

	}

	protected function onAfterUpdate(removed = false) {

	}

	public function create(boolean insertIgnore = false) -> <DbObject>
	{
		var db, metadata, autoincrement, query;
		this->onBeforeCreate();
		let metadata = self::getMetadata();
		let db = self::getDbContextWithMetadata(metadata);

		let query = db->insert(metadata->getTable(), this->getDbFormatedData(true));
		if insertIgnore {
			query->setFlag("IGNORE");
		}

		let this->_affectedRows = query->execute();

		let autoincrement = metadata->getAutoincrementKey();
		if this->_affectedRows && autoincrement !== null {
			let this->{autoincrement} = db->getInsertId();
		}
		return this;
	}

	public function save() -> <DbObject>
	{
		var db, metadata, autoincrement, data, lastInsert, query;
		this->onBeforeCreate(true);
		this->onBeforeUpdate();
		let metadata = self::getMetadata();
		let db = self::getDbContextWithMetadata(metadata);
		let data = [];
		let lastInsert = "";
		let autoincrement = null;

		var propName, propFlag, e;

        for propName, propFlag in metadata->getFields() {
    		if !(propFlag & ObjectMetadata::AUTOINCREMENT) {
				let data[propName] = this->{propName};
    		} else {
    			let lastInsert = "`".propName."` = LAST_INSERT_ID(`".propName."`)";
    			let autoincrement = propName;
    		}
        }

		let query = db->query("INSERT INTO %n %v ON DUPLICATE KEY UPDATE %sql %a", metadata->getTable(), data, lastInsert, data);
		
		let this->_affectedRows = query->execute();

		if this->_affectedRows && autoincrement !== null {
			try {
				let this->{autoincrement} = db->getInsertId();
			} catch Db\DbException, e {
				e->getMessage();
			}
		}

		if this->_affectedRows {
			this->onAfterUpdate();
		}

		return this;
	}

	public function update(onlyColumnsArray = null) -> <DbObject>
	{
		var db, metadata, autoincrement, data, query;
		this->onBeforeUpdate();
		let metadata = self::getMetadata();
		let db = self::getDbContextWithMetadata(metadata);
		let data = [];
		let autoincrement = null;

		var propName, propFlag, keys;

		let keys = [];

        for propName, propFlag in metadata->getFields() {
    		
    		if propFlag & ObjectMetadata::KEY {
    			let keys[propName] = this->{propName};
    		} else {
    			if onlyColumnsArray === null || in_array(propName, onlyColumnsArray) {
	    			let data[propName] = this->{propName};
	    		} 
    		}
        }

        if count(data) === 0 {
        	let this->_affectedRows = 0;
        } else {
        	let query = db->update(metadata->getTable(), data);
        	query->__call("where", ["%and", keys]);
			let this->_affectedRows = query->execute();
        }

        if this->_affectedRows {
			this->onAfterUpdate();
		}

		return this;
	}

	public function delete() -> <DbObject>
	{
		var db, metadata, autoincrement, query;
		this->onBeforeUpdate();
		let metadata = self::getMetadata();
		let db = self::getDbContextWithMetadata(metadata);
		let autoincrement = null;

		var propName, propFlag, keys;

		let keys = [];

        for propName, propFlag in metadata->getFields() {
    		if propFlag & ObjectMetadata::KEY {
    			let keys[propName] = this->{propName};
    		}
        }

        let query = db->delete(metadata->getTable());
    	query->__call("where", ["%and", keys]);
		let this->_affectedRows = query->execute();

        if this->_affectedRows {
			this->onAfterUpdate();
		}

		return this;
	} 

	protected static function hasMany() 
	{

	}

	public function getAffectedRows() {
		if this->_affectedRows === null {
			throw new Exception("You can use getAffectedRows() method only after insert, update or remove");
		} else {
			return this->_affectedRows;
		}
	}



}