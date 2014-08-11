

namespace Fastorm\Db;


class ColumnInfo {
	
	private static patterns;
	/**
	 * Heuristic type detection.
	 * @param  string
	 * @return string
	 * @internal
	 */
	public static function detectType(type)
	{
		if empty self::patterns {
			let self::patterns = [
				"^_": \Fastorm\Db\Query::TYPE_TEXT, // PostgreSQL arrays
				"BYTEA|BLOB|BIN": \Fastorm\Db\Query::TYPE_BINARY,
				"TEXT|CHAR|POINT|INTERVAL": \Fastorm\Db\Query::TYPE_TEXT,
				"YEAR|BYTE|COUNTER|SERIAL|INT|LONG|SHORT": \Fastorm\Db\Query::TYPE_INTEGER,
				"CURRENCY|REAL|MONEY|FLOAT|DOUBLE|DECIMAL|NUMERIC|NUMBER": \Fastorm\Db\Query::TYPE_FLOAT,
				"^TIME$": \Fastorm\Db\Query::TYPE_TIME,
				"TIME": \Fastorm\Db\Query::TYPE_DATETIME, // DATETIME, TIMESTAMP
				"DATE": \Fastorm\Db\Query::TYPE_DATE,
				"BOOL": \Fastorm\Db\Query::TYPE_BOOL
			];
		}
		var key, val;
		for key,val in self::patterns {
			if (preg_match("#".key."#i", type)) {
				return val;
			}
		}
		return \Fastorm\Db\Query::TYPE_TEXT;
	}	

}