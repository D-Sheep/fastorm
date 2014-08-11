
namespace Fastorm\Db;


/**
 *
 *
 * @author (dg)
 * Inspired by David Grudl"s dibi - DibiFLuent
 */
class Query
{
	const REMOVE = false;

	const TYPE_TEXT = "s";
	const TYPE_BINARY = "bin";
	const TYPE_BOOL = "b";
	const TYPE_INTEGER = "i";
	const TYPE_FLOAT = "f";
	const TYPE_DATE = "d";
	const TYPE_DATETIME = "t";
	const TYPE_TIME = "t";

	const IDENTIFIER = "n";

	/** sorting order */
	const ASC = "ASC";
	const DESC = "DESC";

	public static masks;

	public static modifiers;

	public static separators;

	public static swithes;

	protected connection;

	private _command;

	private setups = [];

	private _cursor;

	private _clauses;

	public function __construct(<Connection> connection) {
		let this->connection = connection;
		if empty self::masks {
			this->staticInitialize();
		}
	}

	private function staticInitialize() {
		let self::masks = [
			"SELECT": ["SELECT", "DISTINCT", "FROM", "WHERE", "GROUP BY", "HAVING", "ORDER BY", "LIMIT", "OFFSET"],
			"UPDATE": ["UPDATE", "SET", "WHERE", "ORDER BY", "LIMIT"],
			"INSERT": ["INSERT", "INTO", "VALUES", "SELECT"],
			"DELETE": ["DELETE", "FROM", "USING", "WHERE", "ORDER BY", "LIMIT"]
		];

		let self::modifiers = [
			"SELECT": "%n",
			"FROM": "%n",
			"IN": "%in",
			"VALUES": "%l",
			"SET": "%a",
			"WHERE": "%and",
			"HAVING": "%and",
			"ORDER BY": "%by",
			"GROUP BY": "%by"
		];

		let self::separators = [
			"SELECT": ",",
			"FROM": ",",
			"WHERE": "AND",
			"GROUP BY": ",",
			"HAVING": "AND",
			"ORDER BY": ",",
			"LIMIT": false,
			"OFFSET": false,
			"SET": ",",
			"VALUES": ",",
			"INTO": false
		];

		let self::swithes = [
			"JOIN": "FROM",
			"INNER JOIN": "FROM",
			"LEFT JOIN": "FROM",
			"RIGHT JOIN": "FROM"
		];
	}

	public function __call(string clause, args) {
		var sep, arg;
		boolean removeArray;
		let removeArray = count(args) === 1 && args[0] === false;

		let clause = self::_formatClause(clause);

		if this->_command == null {
			this->initialize(clause);
		}

		// auto-switch to a clause
		if isset self::swithes[clause] {
			let this->_cursor = self::swithes[clause];
		} 

		if array_key_exists(clause, this->_clauses) {
			// append to clause
			let this->_cursor = clause;

			if removeArray{
				let this->clauses[this->_cursor] = null;
				return this;
			}

			if isset(self::$separators[clause]) {
				let sep = self::$separators[clause];
				if sep === false { // means: replace
					let this->_clauses[clause] = [];
				} else {
					if !empty this->_clauses[clause] {
						let this->_clauses[clause][] = sep;
					}
				}
			}

		} else {
			// append to currect flow
			if removeArray {
				return this;
			}

			let this->_clauses[this->_cursor][] = clause;
		}

		if this->_cursor === null {
			let this->_cursor = clause;
		}

		if !isset this->_clauses[this->_cursor] {
			let this->_clauses[this->_cursor] = [];
		}

		
		// special types or argument
		if count(args) === 1 {
			let arg = args[0];
			
			// TODO: really ignore TRUE?
			if arg === true { // flag
				return this;
			}

			if is_string(arg) && preg_match("#^[a-z:_][a-z0-9_.:]*$#i", arg) { // identifier
				let args = ["%n", arg];

			} else {
				if is_array(arg) || ((arg instanceof \Traversable) && !(arg instanceof self)) { // any array
					if isset(self::$modifiers[clause]) {
						let args = [ self::$modifiers[clause], arg ];
					} else {
						if is_string(key(arg)) { // associative array
							let args = [ "%a", arg ];
						}
					}
				}
			} // case $arg === FALSE is handled above
		}

		for arg in args {
			if (arg instanceof self) {
				let arg = "(" . arg->__toString() . ")";
			}
			let this->_clauses[this->_cursor][] = arg;
		}

		return this;
	}

	private function initialize(string clause) {
		if isset self::masks[clause] {
			let this->_clauses = array_fill_keys(self::masks[clause], null);
		} else {
			let this->_clauses = [];
		}
		let this->_cursor = clause;
		let this->_command = clause;
	}


	public static function _formatClause(string s)  -> string
	{
		if (s === "order" || s === "group") {
			let s .= "By";
		}
		return strtoupper(preg_replace("#[a-z](?=[A-Z])#", "$0 ", s));
	}

	/**
	 * Returns SQL command.
	 * @return string
	 */
	public function getCommand()
	{
		return this->_command;
	}



	
	public function getConnection()
	{
		return this->connection;
	}



	
	public function setupResult(method)
	{
		let this->setups[] = func_get_args();
		return this;
	}



	
	public function execute(ret = null)
	{
		var res;
		let res = this->query(this->_export());
		if ret === self::IDENTIFIER {
			return this->connection->getInsertId();
		} else {
			return res;
		}
	}



	
	public function fetchRow()
	{
		if this->_command === "SELECT" {
			return this->query(this->_export(null, ["%lmt", 1]))->fetchRow();
		} else {
			return this->query(this->_export())->fetchRow();
		}
	}



	public function fetchSingle()
	{
		if this->_command === "SELECT" {
			return this->query(this->_export(null, ["%lmt", 1]))->fetchSingle();
		} else {
			return this->query(this->_export())->fetchSingle();
		}
	}




	public function fetchAssoc(assoc)
	{
		return this->query(this->_export())->fetchAssoc(assoc);
	}



	
	public function fetchPairs(string key = null, string value = null)
	{
		return this->query(this->_export())->fetchPairs(key, value);
	}



	public function getIterator(offset = null, limit = null)
	{
		return this->query(this->_export(null, ["%ofs %lmt", offset, limit]))->getIterator();
	}




	public function count()
	{
		return (int) this->query([
			"SELECT COUNT(*) FROM (%ex", $this->_export(), ") AS [data]"
		])->fetchSingle();
	}



	private function query(args)
	{
		var res, setup;
		let res = this->connection->query(args);
		for setup in this->setups {
			call_user_func_array([ res, array_shift(setup) ], setup);
		}
		return res;
	}





	final public function __toString()
	{
		return this->connection->translate(this->_export());
	}



	protected function _export(clause = null, args = null)
	{
		var data, cls, statement, arg;
		if args === null {
			let args = [];
		}
		if clause === null {
			let data = this->_clauses;

		} else {
			let clause = self::_formatClause(clause);

			if array_key_exists(clause, this->_clauses) {
				let data = [ clause: this->_clauses[clause] ];
			} else {
				return [];
			}
		}

		for cls, statement in data {
			if statement !== null {
				let args[] = clause;
				/*if clause === this->_command && $this->flags) {
					$args[] = implode(" ", array_keys($this->flags));
				}*/
				for arg in statement {
					let args[] = arg;
				}
			}
		}

		return args;
	}


}