
namespace Fastorm\Db;

class Connection {

	public static events;

	protected config;

	protected driver;

	protected translator;

	protected connected;


	/**
	 * Connection options: (see driver-specific options too)
	 *   - lazy (bool) => if TRUE, connection will be established only when required
	 *   - result (array) => result set options
	 *       - formatDateTime => date-time format (if empty, DateTime objects will be returned)
	 *   - profiler (array or bool)
	 *       - run (bool) => enable profiler?
	 *       - file => file to log
	 *   - substitutes (array) => map of driver specific substitutes (under development)

	 * @param  mixed   connection parameters
	 * @param  string  connection name
	 * @throws DibiException
	 */
	public function __construct(config, name = null)
	{
		var key, val, tmp;
		string driver, driverClass;
		
		let this->connected = false;

		// DSN string
		if is_string(config) {
			parse_str(config, config);

		} else {
			if is_array(config) === false {
				throw new \InvalidArgumentException("Configuration must be array, string or object.");
			}

			let tmp = [];
			for key, val in config {
				if val instanceof \Traversable {
					let tmp[key] = iterator_to_array(val);
				} else {
					let tmp[key] = val;
				}
			}
			let config = tmp;

		}
			

		if !isset(config["driver"]) {
			let config["driver"] = "mysql";
		}

		let driver = (string) preg_replace("#[^a-z0-9_]#", "_", strtolower(config["driver"]));
		let driverClass = "\\Fastorm\\Db\\Drivers\\" . driver . "Driver";
		if class_exists(driverClass, false) === false {
			throw new DbException("Unable to create instance of dibi driver ".driverClass.".");
		}

		let config["name"] = name;
		let this->config = config;
		let this->driver = new {driverClass};
		let this->translator = new Translator(this);

		// profiler
		/*profilerCfg = & config["profiler"];
		if (is_scalar(profilerCfg)) {
			profilerCfg = array("run" => (bool) profilerCfg);
		}
		if (!empty(profilerCfg["run"])) {
			filter = isset(profilerCfg["filter"]) ? profilerCfg["filter"] : DibiEvent::QUERY;

			if (isset(profilerCfg["file"])) {
				this->onEvent[] = array(new DibiFileLogger(profilerCfg["file"], filter), "logEvent");
			}

			if (DibiFirePhpLogger::isAvailable()) {
				this->onEvent[] = array(new DibiFirePhpLogger(filter), "logEvent");
			}

			if (class_exists("DibiNettePanel", FALSE)) {
				panel = new DibiNettePanel(isset(profilerCfg["explain"]) ? profilerCfg["explain"] : TRUE, filter);
				panel->register(this);
			}
		}*/

		if !isset(config["lazy"]) {
			this->connect();
		}
	}



	



	/**
	 * Connects to a database.
	 * @return void
	 */
	public function connect()
	{
		var e, event;
		let event = this->event(Event::CONNECT);
		try {
			this->driver->connect(this->config);
			let this->connected = true;
			this->onEvent(event->done());

		} catch DbException, e {
			this->onEvent(event->done(e));
			throw e;
		}
	}



	/**
	 * Disconnects from a database.
	 * @return void
	 */
	public function disconnect()
	{
		this->driver->disconnect();
	}



	/**
	 * Returns TRUE when connection was established.
	 * @return bool
	 */
	public function isConnected() -> boolean
	{
		return this->connected;
	}



	/**
	 * Returns configuration variable. If no key is passed, returns the entire array.
	 * @see self::__construct
	 * @param  string
	 * @param  mixed  default value to use if key not found
	 * @return mixed
	 */
	public function getConfig(key = null, defaultValue = null)
	{
		if key === null {
			return this->config;

		} else {
			if isset this->config[key] {
				return this->config[key];

			} else {
				return defaultValue;
			}
		}
	}





	/**
	 * Returns the driver and connects to a database in lazy mode.
	 * @return IDibiDriver
	 */
	public function getDriver()
	{
		if this->connected === false {
			this->connect();
		}
		return this->driver;
	}



	/**
	 * Generates (translates) and executes SQL query.
	 * @param  array|mixed      one or more arguments
	 * @return DibiResult|int   result set object (if any)
	 * @throws DibiException
	 */
	public function queryArray(funcArgs, <\Fastorm\ObjectMetadata> metadata = null)
	{
		return this->nativeQuery(this->translateArgs(funcArgs), metadata);
	}

	public function __call(funcName, arguments)
	{
		if funcName === "query" {
			this->nativeQuery(this->translateArgs(arguments));
		} else {
			throw new \Fastorm\Exception("Method not exists");
		}
	}



	/**
	 * Generates SQL query.
	 * @param  array|mixed      one or more arguments
	 * @return string
	 * @throws DibiException
	 */
	public function translate(args)
	{
		let args = func_get_args();
		return this->translateArgs(args);
	}



	/**
	 * Generates SQL query.
	 * @param  array
	 * @return string
	 */
	private function translateArgs(args)
	{
		if this->connected === false {
			this->connect();
		}
		return this->translator->translate(args);
	}



	/**
	 * Executes the SQL query.
	 * @param  string           SQL statement.
	 * @return DibiResult|int   result set object (if any)
	 * @throws DibiException
	 */
	public function nativeQuery(sql, <\Fastorm\ObjectMetadata> metadata = null)
	{
		var res, e, event;

		if this->connected === false {
			this->connect();
		}


		let res = null;
		let event = this->event(Event::QUERY, sql);
		try {
			let res = this->driver->query(sql);

			if res {
				let res = this->createResultSet(res, metadata);
			} else {
				let res = this->driver->getAffectedRows();
			}

			this->onEvent(event->done(res));
			return res;
		} catch DbException, e {
			this->onEvent(event->done(e));
			throw e;
		}
	}



	/**
	 * Gets the number of affected rows by the last INSERT, UPDATE or DELETE query.
	 * @return int  number of rows
	 * @throws DibiException
	 */
	public function getAffectedRows()
	{
		var rows;
		if this->connected === false {
			this->connect();
		}
		let rows = this->driver->getAffectedRows();
		if is_int(rows) === false || rows < 0 {
			throw new DbException("Cannot retrieve number of affected rows.");
		}
		return rows;
	}



	/**
	 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous INSERT query.
	 * @param  string     optional sequence name
	 * @return int
	 * @throws DibiException
	 */
	public function getInsertId(sequence = null)
	{
		int id;
		if this->connected === false {
			this->connect();
		}
		let id = (int) this->driver->getInsertId(sequence);
		if id < 1 {
			throw new DbException("Cannot retrieve last generated ID.");	
		} 
		return (int) id;
	}



	/**
	 * Begins a transaction (if supported).
	 * @param  string  optional savepoint name
	 * @return void
	 */
	public function begin(savepoint = null)
	{
		var e, event;
		if this->connected === false {
			this->connect();
		}
		let event = this->event(Event::BEGIN, savepoint);
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::BEGIN, savepoint) : NULL;*/
		try {
			this->driver->begin(savepoint);
			this->onEvent(event->done());

		} catch DbException, e {
			this->onEvent(event->done(e));
			throw e;
		}
	}



	/**
	 * Commits statements in a transaction.
	 * @param  string  optional savepoint name
	 * @return void
	 */
	public function commit(savepoint = null)
	{
		var e, event;
		if this->connected === false {
			this->connect();
		}
		let event = this->event(Event::COMMIT, savepoint);
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::COMMIT, savepoint) : NULL;*/
		try {
			this->driver->commit(savepoint);
			this->onEvent(event->done());

		} catch DbException, e {
			this->onEvent(event->done(e));
			throw e;
		}
	}



	/**
	 * Rollback changes in a transaction.
	 * @param  string  optional savepoint name
	 * @return void
	 */
	public function rollback(savepoint = NULL)
	{
		var e, event;
		if this->connected === false {
			this->connect();
		}
		let event = this->event(Event::ROLLBACK, savepoint);
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::ROLLBACK, savepoint) : NULL;*/
		try {
			this->driver->rollback(savepoint);
			this->onEvent(event->done());

		} catch DbException, e {
			this->onEvent(event->done(e));
			throw e;
		}
	}



	/**
	 * Result set factory.
	 * @param  IDibiResultDriver
	 * @return DibiResult
	 */
	public function createResultSet(<IResultDriver> resultDriver, <\Fastorm\ObjectMetadata> metadata = null)  -> <Result>
	{
		return new Result(resultDriver, metadata);
	}





	/**
	 * @return DibiFluent
	 */
	public function command() -> <Query>
	{
		return new Query(this);
	}



	/**
	 * @param  string    column name
	 * @return DibiFluent
	 */
	public function select(args) -> <\Fastorm\Db\Query>
	{
		let args = func_get_args();
		return this->command()->__call("select", args);
	}



	/**
	 * @param  string   table
	 * @param  array
	 * @return DibiFluent
	 */
	public function update(table, args) -> <\Fastorm\Db\Query>
	{
		if !(is_array(args) || (args instanceof \Traversable)) {
			throw new \InvalidArgumentException("Arguments must be array or Traversable.");
		}
		return this->command()->__call("update", ["%n", table])->__call("set", [args]);
	}



	/**
	 * @param  string   table
	 * @param  array
	 * @return DibiFluent
	 */
	public function insert(table, args) -> <\Fastorm\Db\Query>
	{
		if (args instanceof \Traversable) {
			let args = iterator_to_array(args);
		} else {
			if is_array(args) === false {
				throw new \InvalidArgumentException("Arguments must be array or Traversable.");
			}
		}
		return this->command()->__call("insert", [])
						      ->__call("into", ["%n", table, "(%n)", array_keys(args)])
							  ->__call("values", ["%l", args]);
	}



	/**
	 * @param  string   table
	 * @return DibiFluent
	 */
	public function delete(table) -> <\Fastorm\Db\Query>
	{
		return this->command()->__call("delete", [])
							  ->__call("from", ["%n", table]);
	}

	public function addEvent(eventHandler) {
		var evArray;
		if self::events === null {
			let evArray = [];
		} else {
			let evArray = self::events;
		}
		let evArray[] = eventHandler;
		let self::events = evArray;
	}

	protected function onEvent(<Event> event = null) {
		if self::events !== null && event !== null {
			var callback;
			for callback in self::events {
				{callback}(event);
			}	
		}
	}

	protected function event(var type, var sql = null) {
		return new Event(this, type, sql);
	}
	

}