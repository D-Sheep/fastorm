
namespace Fastorm;

class ObjectMetadata
{

	const PROPERTY = 1;
	const KEY = 2;
	const AUTOINCREMENT = 4;

	private static _metadataCache;

	private table;

	private storage;

	private idField;

	private reflection;

	private fields = [];

	protected function __construct(<\ReflectionClass> reflection, array fields, string idField = null, string table = null, string storage = null)
	{
		let this->reflection = reflection;
		let this->idField = idField;
		let this->table = table;
		let this->storage = storage;
		let this->fields = fields;
	}

	public function getTable() -> string|null
	{
		return this->table;
	}

	public function getStorage() -> string|null
	{
		return this->storage;
	}

	public function getIdField() -> string|null
	{
		return this->idField;
	}

	public function getFields() -> array
	{
		return this->fields;
	}

	public function getReflection() -> <\ReflectionClass>
	{
		return this->reflection;
	}

	public static function getMetadata(string className) -> <ObjectMetadata>
	{
		if !isset self::_metadataCache[className]
		{
			let self::_metadataCache = self::createMetadata(className);
		}
		return self::_metadataCache;
	}

	private static function createMetadata(string className) -> <ObjectMetadata>
	{
		var reflection, properties, docs, matches, property, propName, idField, table, storage;
		int prop;
		array propertiesArray;

		let matches = null;
		let reflection = new \ReflectionClass(className);
		let properties = reflection->getProperties(\ReflectionProperty::IS_PUBLIC);
		let docs = reflection->getDocComment();

		if preg_match("/@id ([a-z_]+)/i", docs, matches) {
			let idField = matches[1];
		} else {
			let idField = null;
		}

		if preg_match("/@table ([a-z_]+)/i", docs, matches) {
			let table = matches[1];
		} else {
			let table = null;
		}

		if preg_match("/@storage ([a-z_]+)/i", docs, matches) {
			let storage = matches[1];
		} else {
			if table !== null {
				let storage = "db";
			} else {
				let storage = null;
			}
		}

		let propertiesArray = [];

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

                let propertiesArray[propName] = prop;
            }
        }

        return new ObjectMetadata(reflection, propertiesArray, idField, table, storage);
	}

}
