
namespace Fastorm;

abstract class DbObject extends DataObject

{
	protected _joinCache;

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

	private static function getDbContextWithMetadata(<ObjectMetadata> metadata) -> <Db\Connection>
	{
		return ModelManager::getShared()->getConnection(metadata);
	}

	/**
	 *	@metadata only for powerup (private)
	 */
	public static function getById(id, boolean joinAll = true, <ObjectMetadata> metadata = null) {
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
		let joinedMetadata = metadata->getJoin(propertyName);

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
					for key, value in joinedMetadata {
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


	public function create(boolean insertIgnore = false) -> <DbObject>
	{

	}

	public function save() -> <DbObject>
	{

	}

	public function update(array onlyColumns = null) -> <DbObject>
	{

	}

	protected static function hasMany() 
	{

	}



}