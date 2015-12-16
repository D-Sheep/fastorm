
namespace Fastorm\Db;


class Event
{
	/** event type */
	const CONNECT = 1;
	const SELECT = 4;
	const INSERT = 8;
	const DELETE = 16;
	const UPDATE = 32;
	const QUERY = 60; // SELECT | INSERT | DELETE | UPDATE
	const BEGIN = 64;
	const COMMIT = 128;
	const ROLLBACK = 256;
	const TRANSACTION = 448; // BEGIN | COMMIT | ROLLBACK
	const ALL = 1023;

	/** @var DibiConnection */
	public connection;

	/** @var int */
	public type;

	/** @var string */
	public sql;

	/** @var DibiResult|DibiDriverException|null */
	public result;

	/** @var float */
	public time;

	/** @var int */
	public count;

	private static types;

	public static elapsedTime;

	public static numOfQueries;

	public static totalTime;

	public static lastSql;


	public function __construct(<Connection> connection, type, sqlCommand = null)
	{
		var matches;
		let this->connection = connection;
		let this->type = type;
		let this->sql = trim(sqlCommand);
		let this->time = microtime(true);

		if empty self::types {
			let self::types = [
				"SELECT": self::SELECT, "UPDATE": self::UPDATE,
				"INSERT": self::INSERT, "DELETE": self::DELETE
			];
			let self::elapsedTime = 0;
			let self::totalTime = 0;
			let self::numOfQueries = 0;
		}

		let matches = null;
		if (type === self::QUERY && preg_match("#\\(?\\s*(SELECT|UPDATE|INSERT|DELETE)#iA", this->sql, matches)) {
			let this->type = self::types[strtoupper(matches[1])];
		}

		

		let self::elapsedTime = true;
		let self::numOfQueries = self::numOfQueries + 1;
		let self::lastSql = sqlCommand;
	}



	public function done(result = null) -> <Event>
	{
		var e;
		let this->result = result;
		if result !== null {
			try {
				if typeof result === "object" && (result instanceof \Fastorm\Db\Result) {
					let this->count = count(result);
				} else {
					let this->count = null;
				}
				
			} catch \Fastorm\Db\DbException, e {
				let this->count = null;
			}
		}

		let this->time = microtime(true) - this->time;
		let self::elapsedTime = this->time;
		let self::totalTime += this->time;
		return this;
	}

}
