
namespace Fastorm;

class ObjectMetadata
{

	const PROPERTY = 1;
	const KEY = 2;
	const AUTOINCREMENT = 4;
	const JOINED = 8;

	protected static _metadataCache;

	protected table;

	protected storage;

	protected idField;

	protected reflection;

	protected className;

	protected fields;

	protected joins;

	protected aliases;

	protected relations;

	protected function __construct(<\ReflectionClass> reflection, var className, var fields, var joins, var idField = null, var table = null, var storage = null)
	{
		let this->reflection = reflection;
		let this->idField = idField;
		let this->table = table;
		let this->storage = storage;
		let this->fields = fields;
		let this->joins = joins;
		let this->className = className;
		let this->aliases = [];
		let this->relations = [];
	}

	public function getTable()
	{
		return this->table;
	}

	public function getStorage() 
	{
		return this->storage;
	}

	public function getIdField()
	{
		return this->idField;
	}

	public function getFields() 
	{
		return this->fields;
	}

	public function getClassName()
	{
		return this->className;
	}

	public function newInstance(var data = null)
	{
		var className, ret;
		let className = this->className;
		let ret = new {className}(data);
		if !(ret instanceof \Fastorm\DataObject) {
			throw new Exception(className);
		}
		return ret;
	}

	public function addOne(var className, var alias, var destinationProperty = null)
	{
		var relationMetadata;
		if destinationProperty === null
		{
			let destinationProperty = this->idField;
		}
		let relationMetadata = new RelationMetadata(this, className, destinationProperty, alias);
		let this->relations[className] = relationMetadata;
	}

	/*public function addMany(var className, var alias, var destinationProperty = null)
	{
		let this->oneRelations
	}*/

	//public function addManyToMany(var className, var alias, var middleTableName = null, var destinationProperty = null)

	public function hasRelationAlias(var aliasName)
	{
		return isset this->relations[aliasName];
	}

	public function getRelationAlias(var aliasName)
	{
		if isset this->relations[aliasName] {
			return this->relations[aliasName];
		} else {
			return null;
		}
	}

	public function getReflection()
	{
		return this->reflection;
	}

	public static function getMetadata(string className) -> <ObjectMetadata>
	{
		if !isset self::_metadataCache[className]
		{
			let self::_metadataCache[className] = self::createMetadata(className);
		}
		return self::_metadataCache[className];
	}

	private static function createMetadata(string className) -> <ObjectMetadata>
	{
		var reflection, properties, docs, matches, property, propName, idField, table, storage, prop;
		var propertiesArray, joins;

		let matches = null;
		let reflection = new \ReflectionClass(className);
		let properties = reflection->getProperties(\ReflectionProperty::IS_PUBLIC);
		let docs = reflection->getDocComment();

		if preg_match("/@id ([A-Za-z0-9_]+)/i", docs, matches) {
			let idField = matches[1];
		} else {
			let idField = null;
		}

		if preg_match("/@table ([A-Za-z0-9_]+)/i", docs, matches) {
			let table = matches[1];
		} else {
			let table = null;
		}

		if preg_match("/@storage ([A-Za-z0-9_]+)/i", docs, matches) {
			let storage = matches[1];
		} else {
			if table !== null {
				let storage = "db";
			} else {
				let storage = null;
			}
		}

		let propertiesArray = [];
		let joins = [];

		for property in properties {
            if !property->isStatic() {
            	let propName = property->getName();
        		let prop = self::PROPERTY;
        		let docs = property->getDocComment();

        		if preg_match("/@autoincrement/i", docs) {
        			let prop = prop | self::AUTOINCREMENT;
        		}

        		if prop & self::AUTOINCREMENT || propName === idField || preg_match("/@key/i", docs) {
        			let prop = prop | self::KEY;
        		}

            	if preg_match("/@join ([A-Za-z0-9_\\\\]+)/i", docs, matches) {
					let joins[propName] = matches[1];
					let prop = prop | self::JOINED;
				}
				let propertiesArray[propName] = prop;
            }
        }

        return new ObjectMetadata(reflection, className, propertiesArray, joins, idField, table, storage);
	}

	public function getJoin(string propName) -> <ObjectMetadata>|null {
		if isset this->joins[propName] {
			return self::getMetadata(this->joins[propName]);
		} else {
			return null;
		}
	}

	public function hasJoin(string propName) -> boolean {
		return isset this->joins[propName];
	}

	public function getJoins() {
		return this->joins;
	}

	public function getAliases(string propName, <ObjectMetadata> joinedTable = null) 
	{
		if !isset this->aliases[propName] {
			var aliasTable;
			var key, value, aliasName;
			if joinedTable === null {
				let joinedTable = this->getJoin(propName);
			}
			let aliasTable = [];
			for key, value in joinedTable->getFields() {
				let aliasName = self::makeAlias(propName, key);
				let aliasTable[aliasName] = key;
			}
			let this->aliases[propName] = aliasTable;
		}
		return this->aliases[propName];
	}

	public static function toPropertyName(string camelcase) -> string {
		var matches, ret, match, zeroMatches;
		let matches = null;
		preg_match_all("!([A-Z][A-Z0-9]*(?=$|[A-Z][a-z0-9])|[A-Za-z][a-z0-9]+)!", camelcase, matches);
		let zeroMatches = matches[0];
		let ret = [];

		for match in zeroMatches {
			if match === strtoupper(match) {
				let ret[] = strtolower(match);
			} else {
				let ret[] = lcfirst(match);
			}
		}

		return implode("_", ret);
	}

	public static function toClassName(string propertyName)
	{
		var splited, k, v;
		let splited = explode("_", propertyName);
		for k, v in splited
		{
			let v = ucfirst(v);
			let splited[k] = v;
		}
		return implode("", splited);
	}

	public static function makeAlias(string sourceProperty, string targetProperty) -> string {
		return "r_" . preg_replace("/_id$/", "", sourceProperty) . "_" . targetProperty;
	}

	public function getAutoincrementKey() {
		var found, key, flag;
		let found = null;
		for key, flag in this->fields {
			if flag & self::AUTOINCREMENT {
				let found = key;
				break;
			}
		}
		return found;
	}


}
