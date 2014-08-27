namespace Fastorm\Db\Drivers;

class MysqliDriver extends \Fastorm\DbObject implements \Fastorm\Db\IResultDriver {


	const ERROR_ACCESS_DENIED = 1045;
	const ERROR_DUPLICATE_ENTRY = 1062;
	const ERROR_DATA_TRUNCATED = 1265;

	/** @var mysqli  Connection resource */
	protected connection;

	/** @var mysqli_result  Resultset resource */
	protected resultSet;

	/** @var bool */
	protected autoFree = true;

	/** @var bool  Is buffered (seekable and countable)? */
	protected buffered;

	protected static types;

	/**
	 * @throws DibiNotSupportedException
	 */
	public function __construct()
	{
		let this->autoFree = true;
		if !extension_loaded("mysqli") {
			throw new \Fastorm\Db\DbException("Db not supported exception: PHP extension 'mysqli' is not loaded.");
		}
	}

	/**
	 * Connects to a database.
	 * @return void
	 * @throws DbException
	 */
	public function connect(config)
	{
		mysqli_report(MYSQLI_REPORT_OFF);
		if isset config["resource"] {
			let this->connection = config["resource"];
		} else {
			var defaults;
			let defaults = [
				"charset" : "utf8",
				"timezone": date("P"),
				"username": ini_get("mysqli.default_user"),
				"password": ini_get("mysqli.default_pw"),
				"socket": ini_get("mysqli.default_socket"),
				"port": null,
				"flags": null
			];
			// default values
			let config = array_merge(defaults, config);

			var host;
			if !isset config["host"] {
				let host = ini_get("mysqli.default_host");
				if host {
					let config["host"] = host;
					let config["port"] = ini_get("mysqli.default_port");
				} else {
					let config["host"] = NULL;
					let config["port"] = NULL;
				}
			}

			var foo;
			let foo = config["database"];

			let this->connection = mysqli_init();
			if isset config["options"] {
				if is_scalar(config["options"]) {
					trigger_error(__CLASS__ . ": configuration item 'options' must be array; for constants MYSQLI_CLIENT_* use 'flags'.", E_USER_NOTICE);
				} else {
					var key, value, iterate;
					let iterate = config["options"];
					for key, value in iterate {
						mysqli_options(this->connection, key, value);
					}
				}
			}
			if !isset config["persistent"] {
				mysqli_real_connect(this->connection, "" . config["host"], config["username"], config["password"], config["database"], config["port"], config["socket"], config["flags"]); // intentionally @	
			} else {
				mysqli_real_connect(this->connection, "p:" . config["host"], config["username"], config["password"], config["database"], config["port"], config["socket"], config["flags"]); // intentionally @
			}

			/*var errno = mysqli_connect_errno();
			if errno {
				throw new DibiDriverException(mysqli_connect_error(), errno);
			}*/
		}

		if isset(config["charset"]) {
			var ok;
			let ok = false;
			if version_compare(PHP_VERSION , "5.1.5", ">=") {
				// affects the character set used by mysql_real_escape_string() (was added in MySQL 5.0.7 and PHP 5.0.5, fixed in PHP 5.1.5)
				let ok = mysqli_set_charset(this->connection, config["charset"]);
			}
			if !ok {
				this->query("SET NAMES '".config["charset"]."'");
			}
		}

		if isset(config["sqlmode"]) {
			this->query("SET sql_mode='".config["sqlmode"]."'");
		}

		if isset(config["timezone"]) {
			this->query("SET time_zone='".config["timezone"]."'");
		}

		

		let this->buffered = !isset(config["unbuffered"]);
	}


	/**
	 * Disconnects from a database.
	 * @return void
	 */
	public function disconnect()
	{
		if this->connection !== null {
			mysqli_close(this->connection);
			let this->connection = null;
		}
	}


	/**
	 * Executes the SQL query.
	 * @param  string      SQL statement.
	 * @return IDibiResultDriver|NULL
	 * @throws DibiDriverException
	 */
	public function query(sql)
	{
		var res;
		if this->buffered {
			let res = mysqli_query(this->connection, sql,  MYSQLI_STORE_RESULT); // intentionally @
		} else {
			let res = mysqli_query(this->connection, sql,   MYSQLI_USE_RESULT); // intentionally @
		}
		if mysqli_errno(this->connection) {
			throw new \Fastorm\Db\DbException("Db not supported exception:".mysqli_error(this->connection), mysqli_errno(this->connection), null, null, sql);

		} else { if is_object(res) {
			return this->createResultDriver(res);
		}}
	}


	/**
	 * Retrieves information about the most recently executed query.
	 * @return array
	 */
	public function getInfo()
	{
		var res = [];
		var matches, m;
		let matches =[];
		preg_match_all("#(.+?): +(\\d+) *#", mysqli_info(this->connection), matches, PREG_SET_ORDER);
		if preg_last_error() {
			throw new \Fastorm\Db\DbException("Db Pcre Exception");
		}

		for  m in matches {
			var hod;
			let hod = m[1];
			let res[hod] = (int) m[2];
		}
		return res;
	}


	/**
	 * Gets the number of affected rows by the last INSERT, UPDATE or DELETE query.
	 * @return int|false  number of rows or false on error
	 */
	public function getAffectedRows()
	{
		if mysqli_affected_rows(this->connection) === -1 {
			return false;
		} else {
			return mysqli_affected_rows(this->connection);
		} 
	}


	/**
	 * Retrieves the ID generated for an AUTO_INCREMENT column by the previous INSERT query.
	 * @return int|false  int on success or false on failure
	 */
	public function getInsertId(sequence)
	{
		return mysqli_insert_id(this->connection);
	}


	/**
	 * Begins a transaction (if supported).
	 * @param  string  optional savepoint name
	 * @return void
	 * @throws DibiDriverException
	 */
	public function begin(savepoint = NULL)
	{
		if savepoint {
			this->query("SAVEPOINT savepoint");
		} else {
			this->query("START TRANSACTION");
		}
	}


	/**
	 * Commits statements in a transaction.
	 * @param  string  optional savepoint name
	 * @return void
	 * @throws DibiDriverException
	 */
	public function commit(savepoint = NULL)
	{
		if savepoint {
			this->query("RELEASE SAVEPOINT savepoint");
		} else {
			this->query("COMMIT");
		}
	}


	/**
	 * Rollback changes in a transaction.
	 * @param  string  optional savepoint name
	 * @return void
	 * @throws DibiDriverException
	 */
	public function rollback(savepoint = NULL)
	{
		if savepoint {
			this->query("ROLLBACK TO SAVEPOINT savepoint");
		} else {
			this->query("ROLLBACK");
		}
	}


	/**
	 * Returns the connection resource.
	 * @return mysqli
	 */
	public function getResource()
	{
		if isset this->connection && isset(this->connection->thread_id) && this->connection->thread_id {
			return this->connection;
		} else {
			return null;
		}
	}


	/**
	 * Result set driver factory.
	 * @param  mysqli_result
	 * @return IDibiResultDriver
	 */
	public function createResultDriver(<\mysqli_result> resourc)
	{
		var res;
		let res = clone this;
		let res->resultSet = resourc;
		return res;
	}


	/**
	 * Encodes data for use in a SQL statement.
	 * @param  mixed     value
	 * @param  string    type (dibi::TEXT, dibi::BOOL, ...)
	 * @return string    encoded value
	 * @throws InvalidArgumentException
	 */
	public function escape(value, type) -> string
	{
		switch type {
			case \Fastorm\Db\Query::TYPE_TEXT:
				return "'" . mysqli_real_escape_string(this->connection, value) . "'";

			case \Fastorm\Db\Query::TYPE_BINARY:
				return "_binary'" . mysqli_real_escape_string(this->connection, value) . "'";

			case \Fastorm\Db\Query::IDENTIFIER:
				return "`" . str_replace("`", "``", value) . "`";

			case \Fastorm\Db\Query::TYPE_BOOL:
				if value {
					return "1"."";
				} else {
					return "0"."";
				}

			case \Fastorm\Db\Query::TYPE_DATE:
			if value instanceof \DateTime {
				return value->format("'Y-m-d'"); 
			} else {
				return date("'Y-m-d'", value);
			}

			case \Fastorm\Db\Query::TYPE_DATETIME:
				if value instanceof \DateTime {
				return value->format("'Y-m-d H:i:s'");
			} else {
				return  date("'Y-m-d H:i:s'", value);
			}

			default:
				throw new \InvalidArgumentException("Unsupported type.");
		}
		return "";
	}


	/**
	 * Encodes string for use in a LIKE statement.
	 * @param  string
	 * @param  int
	 * @return string
	 */
	public function escapeLike(value, pos) -> string
	{
		let value = addcslashes(str_replace("\\", "\\\\", value), "\x00\n\r\\'%_");
		if pos <= 0 {
			return "'%". value . (pos >= 0 ? "%'" : "'");
		} else {
			return "'". value . (pos >= 0 ? "%'" : "'");
		}
	}


	/**
	 * Decodes data from result set.
	 * @param  string    value
	 * @param  string    type (dibi::BINARY)
	 * @return string    decoded value
	 * @throws InvalidArgumentException
	 */
	public function unescape(value, type)
	{
		if type === \Fastorm\Db\Query::TYPE_BINARY {
			return value;
		}
		throw new \InvalidArgumentException("Unsupported type.");
	}


	/**
	 * Injects LIMIT/OFFSET to the SQL query.
	 * @return void
	 */
	public function applyLimit(string sql, int limit, int offset) -> string
	{
		if limit >= 0 || offset > 0 {
			// see http://dev.mysql.com/doc/refman/5.0/en/select.html
			string lmtString, ofsString;

			let limit = (int) limit;
			let offset = (int) offset;

			if limit < 0 {
				let lmtString = "18446744073709551615" . "";
			} else {
				let lmtString = limit . "";
			}

			if offset > 0 {
				let ofsString = " OFFSET ". offset;
			} else {
				let ofsString = "";
			}


			return sql . " LIMIT " . lmtString . ofsString;
		}
		return "";
	}


	/**
	 * Automatically frees the resources allocated for this result set.
	 * @return void
	 */
	public function __destruct()
	{
		return this->autoFree && this->getResultResource() !== null && this->free();
	}


	/**
	 * Returns the number of rows in a result set.
	 * @return int
	 */
	public function getRowCount()
	{
		if !this->buffered {
			throw new \Fastorm\Db\DbException("Db not supported exception: Row count is not available for unbuffered queries.");
		}
		//return mysqli_num_rows(this->resultSet);
		return this->resultSet->num_rows;
	}


	/**
	 * Fetches the row at current position and moves the internal cursor to the next position.
	 * @param  bool     true for associative array, false for numeric
	 * @return array    array on success, nonarray if no next record
	 */
	public function fetchRow(associative)
	{
		if associative {
			return mysqli_fetch_array(this->resultSet, MYSQLI_ASSOC);
		} else {
			return mysqli_fetch_array(this->resultSet, MYSQLI_NUM);
		}
	}


	/**
	 * Moves cursor position without fetching row.
	 * @param  int      the 0-based cursor pos to seek to
	 * @return boolean  true on success, false if unable to seek to specified record
	 * @throws DbException
	 */
	public function seek(row)
	{
		if !this->buffered {
			throw new \Fastorm\Db\DbException("Db not supported exception: Cannot seek an unbuffered result set.");
		}
		return mysqli_data_seek(this->resultSet, row);
	}


	/**
	 * Frees the resources allocated for this result set.
	 * @return void
	 */
	public function free()
	{
		if this->resultSet !== null {
			mysqli_free_result(this->resultSet);
			let this->resultSet = null;
		}
	}


	/**
	 * Returns metadata for all columns in a result set.
	 * @return array
	 */
	public function getResultColumns()
	{

		if empty(self::types) {
			var consts, key, value;
			let consts = get_defined_constants(true);
			let self::types = [];
			for key, value in consts["mysqli"] {
				if strncmp(key, "MYSQLI_TYPE_", 12) === 0 {
					let self::types[value] = substr(key, 12);
				}
			}
			let self::types[MYSQLI_TYPE_TINY] = "INT";
			let self::types[MYSQLI_TYPE_SHORT] = "INT"; 
			let self::types[MYSQLI_TYPE_LONG] = "INT";
		}

		var row, count, i, table;
		let count = mysqli_num_fields(this->resultSet) - 1;
		var columns = [];
		for i in range(0, count) {
			let row = mysqli_fetch_field_direct(this->resultSet, i);

			if row->table {
				let table = row->table . "." . row->name;
			} else {
				let table = row->name;
			}
			//TODO - funguje detect type spravne?
			let columns[] = [
				"name" : row->name,
				"table" : row->orgtable,
				"fullname" : table,
				"nativetype" : self::types[row->type],
				"vendor" : row ];
		}

		return columns;
	}


	/**
	 * Returns the result set resource.
	 * @return mysqli_result
	 */
	public function getResultResource()
	{
		let this->autoFree = false;
		if this->resultSet === null || this->resultSet->type === null {
			return null;
		} else {
			return this->resultSet;
		}
	}

}