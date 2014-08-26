
namespace Fastorm\Db;


/**
 *
 *
 * @author (dg)
 * Inspired by David Grudl"s dibi - DibiFLuent
 */
class Query implements \IteratorAggregate
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

	protected command;

	protected setups;

	protected cursor;

	protected clauses;

	protected flags;

	public function __construct(<Connection> connection) {
		let this->connection = connection;
		if empty self::masks {
			this->staticInitialize();
		}
		let this->flags = [];
		let this->setups = [];
	}

	protected function staticInitialize() {
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

	public function __call(clause, clauseArgs)  
	{
		var sep, argument, removeArray, cursor;

		let removeArray = count(clauseArgs) === 1 && clauseArgs[0] === false;

		let clause = self::_formatClause(clause);

		if empty this->command {
			this->initialize(clause);
		}

		// auto-switch to a clause
		if isset self::swithes[clause] {
			let this->cursor = self::swithes[clause];
			let cursor = this->cursor;
		} 

		if array_key_exists(clause, this->clauses) {
			// append to clause
			let this->cursor = clause;
			let cursor = this->cursor;

			if removeArray{
				let this->clauses[cursor] = null;
				return this;
			}

			if isset(self::$separators[clause]) {
				let sep = self::$separators[clause];
				if sep === false { // means: replace
					let this->clauses[clause] = [];
				} else {
					if !empty this->clauses[clause] {
						let this->clauses[clause][] = sep;
					}
				}
			}

		} else {
			// append to currect flow
			if removeArray {
				return this;
			}
			let cursor = this->cursor;

			let this->clauses[cursor][] = clause;
		}

		if this->cursor === null {
			let this->cursor = clause;
			let cursor = this->cursor;
		}

		if !isset this->clauses[cursor] {
			let this->clauses[cursor] = [];
		}

		
		// special types or argument
		if count(clauseArgs) === 1 {
			let argument = clauseArgs[0];
			
			// TODO: really ignore TRUE?
			if argument === true { // flag
				return this;
			}

			if is_string(argument) && preg_match("#^[a-z:_][a-z0-9_.:]*$#i", argument) { // identifier
				let clauseArgs = ["%n", argument];

			} else {
				if is_array(argument) || ((argument instanceof \Traversable) && !(argument instanceof self)) { // any array
					if isset(self::$modifiers[clause]) {
						let clauseArgs = [ self::$modifiers[clause], argument ];
					} else {
						if is_string(key(argument)) { // associative array
							let clauseArgs = [ "%a", argument ];
						}
					}
				}
			} // case $arg === FALSE is handled above
		}

		for argument in clauseArgs {
			if  typeof argument === "object" && (argument instanceof Query) {
				let argument = "(" . argument->__toString() . ")";
			}
			let this->clauses[this->cursor][] = argument;
		}


		return this;
	}

	protected function initialize(string clause) {
		if isset self::masks[clause] {
			let this->clauses = array_fill_keys(self::masks[clause], null);
		} else {
			let this->clauses = [];
		}
		let this->cursor = clause;
		let this->clauses[clause] = [];
		let this->command = clause;
	}


	public static function _formatClause(string s)  -> string
	{
		if (s === "order" || s === "group") {
			let s .= "By";
		}
		return (string) strtoupper(preg_replace("#[a-z](?=[A-Z])#", "$0 ", s));
	}

	/**
	 * Returns SQL command.
	 * @return string
	 */
	public function getCommand()
	{
		return this->command;
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



	
	public function fetchFirst()
	{
		var ret;
		if this->command === "SELECT" {
			let ret = this->query(this->_export(null, ["%lmt", 1]))->fetchRow();
		} else {
			let ret = this->query(this->_export())->fetchRow();
		}
		if ret === false {
			return null;
		} else {
			return ret;
		}
	}



	public function fetchSingle()
	{
		if this->command === "SELECT" {
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



	protected function query(queryArgs)
	{
		var res, setup;
		let res = this->connection->queryArray(queryArgs);
		for setup in this->setups {
			call_user_func_array([ res, array_shift(setup) ], setup);
		}
		return res;
	}

	final public function __toString()
	{
		return this->connection->translate(this->_export());
	}

	/**
	 * Change a SQL flag.
	 * @param  string  flag name
	 * @param  bool  value
	 * @return DibiFluent  provides a fluent interface
	 */
	public function setFlag(flag, value = true)
	{
		let flag = strtoupper(flag);
		if value {
			let this->flags[flag] = true;
		} else {
			unset(this->flags[flag]);
		}
		return this;
	}



	/**
	 * Is a flag set?
	 * @param  string  flag name
	 * @return bool
	 */
	final public function getFlag(flag)
	{
		return isset(this->flags[strtoupper(flag)]);
	}


	protected function _export(var clause = null, var additionalArgs = null)
	{
		var data, cls, statement, arg, ret;

		let ret = [];

		
		if clause === null {
			let data = this->clauses;
		} else {
			let clause = self::_formatClause(clause);

			if array_key_exists(clause, this->clauses) {
				let data = [ "clause": this->clauses[clause] ];
			} else {
				return [];
			}
		}

		for cls, statement in data {
			if statement !== null {
				let ret[] = cls;
				if cls === this->command && count(this->flags) > 0 {
					let ret[] = implode(" ", array_keys(this->flags));
				}
				for arg in statement {
					let ret[] = arg;
				}
			}
		}

		if additionalArgs !== null {
			for statement in additionalArgs {
				let ret[] = statement;
			}
		}


		return ret;
	}


}