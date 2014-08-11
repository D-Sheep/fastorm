
namespace Fastorm\Db;

class Connection {

	public onEvent = [];

	private config = [];

	private driver;

	private translator;

	private connected = false;


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

		if empty(config["lazy"]) {
			this->connect();
		}
	}



	/**
	 * Automatically frees the resources allocated for this result set.
	 * @return void
	 */
	public function __destruct()
	{
		// disconnects and rolls back transaction - do not rely on auto-disconnect and rollback!
		if this->connected && this->driver->getResource() !== null {
			this->disconnect();
		}
	}



	/**
	 * Connects to a database.
	 * @return void
	 */
	public function connect()
	{
		var e;
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::CONNECT) : NULL;*/
		try {
			this->driver->connect(this->config);
			let this->connected = true;
			/*event && this->onEvent(event->done());*/

		} catch DbException, e {
			/*event && this->onEvent(event->done(e));*/
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
		let this->connected = true;
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
	public function query(args)
	{
		let args = func_get_args();
		return this->nativeQuery(this->translateArgs(args));
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
	public function nativeQuery(sql)
	{
		var res, e;

		if this->connected === false {
			this->connect();
		}


		let res = null;
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::QUERY, sql) : NULL;*/
		try {
			let res = this->driver->query(sql);

			if res {
				let res = this->createResultSet(res);
			} else {
				let res = this->driver->getAffectedRows();
			}

			/*event && this->onEvent(event->done(res));*/
			return res;
		} catch DbException, e {
			/*event && this->onEvent(event->done(e));*/
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
	public function getInsertId(sequence = null) -> int
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
		var e;
		if this->connected === false {
			this->connect();
		}
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::BEGIN, savepoint) : NULL;*/
		try {
			this->driver->begin(savepoint);
			/*event && this->onEvent(event->done());*/

		} catch DbException, e {
			/*event && this->onEvent(event->done(e));*/
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
		var e;
		if this->connected === false {
			this->connect();
		}
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::COMMIT, savepoint) : NULL;*/
		try {
			this->driver->commit(savepoint);
			/*event && this->onEvent(event->done());*/

		} catch DbException, e {
			/*event && this->onEvent(event->done(e));*/
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
		var e;
		if this->connected === false {
			this->connect();
		}
		/*event = this->onEvent ? new DibiEvent(this, DibiEvent::ROLLBACK, savepoint) : NULL;*/
		try {
			this->driver->rollback(savepoint);
			/*event && this->onEvent(event->done());*/

		} catch DbException, e {
			/*event && this->onEvent(event->done(e));*/
			throw e;
		}
	}



	/**
	 * Result set factory.
	 * @param  IDibiResultDriver
	 * @return DibiResult
	 */
	public function createResultSet(<IResultDriver> resultDriver)  -> <Result>
	{
		var res;
		let res = new Result(resultDriver);
		return res->setFormat(Query::TYPE_DATE, this->getConfig("formatDate", "Y-m-d") )
			      ->setFormat(Query::TYPE_DATETIME, this->getConfig("formatDate", "Y-m-d H:i:s") );
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
	public function select(args) -> <Query>
	{
		let args = func_get_args();
		return this->command()->__call("select", args);
	}



	/**
	 * @param  string   table
	 * @param  array
	 * @return DibiFluent
	 */
	public function update(table, args) -> <Query>
	{
		if !(is_array(args) || (args instanceof \Traversable)) {
			throw new \InvalidArgumentException("Arguments must be array or Traversable.");
		}
		return this->command()->__call("update", ["%n", table])->__call("set", args);
	}



	/**
	 * @param  string   table
	 * @param  array
	 * @return DibiFluent
	 */
	public function insert(table, args) -> <Query>
	{
		if (args instanceof \Traversable) {
			let args = iterator_to_array(args);
		} else {
			if is_array(args) === false {
				throw new \InvalidArgumentException("Arguments must be array or Traversable.");
			}
		}
		return this->command()->__call("insert", 
			["%n", table, "(%n)", array_keys(args)])->__call("values", ["%l", args]);
	}



	/**
	 * @param  string   table
	 * @return DibiFluent
	 */
	public function delete(table)  -> <Query>
	{
		return this->command()->__call("delete", ["%n", table]);
	}




	

}