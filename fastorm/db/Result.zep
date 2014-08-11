
namespace Fastorm\Db;

class Result {

	/** @var array  IResultDriver */
	private driver;

	/** @var array  Translate table */
	private types = [];

	/** @var DibiResultInfo */
	private meta;

	/** @var bool  Already fetched? Used for allowance for first seek(0) */
	private fetched = false;

	/** @var array  format */
	private formats = [];

	private rowClass;


	public function __construct(<IResultDriver> driver)
	{
		let this->driver = driver;
		this->detectTypes();
	}


	public function getResource()
	{
		return this->getResultDriver()->getResultResource();
	}


	/**
	 * Frees the resources allocated for this result set.
	 * @return void
	 */
	public function free()
	{
		if this->driver !== null {
			this->driver->free();
			let this->driver = null;
			let this->meta = null;
		}
	}


	/**
	 * Safe access to property driver.
	 * @return IDibiResultDriver
	 * @throws RuntimeException
	 */
	public function getResultDriver() -> <IResultDriver>
	{
		if this->driver === null {
			throw new \RuntimeException("Result-set was released from memory.");
		}

		return this->driver;
	}


	/********************* rows ****************d*g**/


	/**
	 * Moves cursor position without fetching row.
	 * @param  int      the 0-based cursor pos to seek to
	 * @return boolean  true on success, false if unable to seek to specified record
	 * @throws DbException
	 */
	public function seek(int row) -> boolean
	{
		if row !== 0 && this->fetched {
			return (boolean) this->getResultDriver()->seek(row);
		} else {
			return true;
		}
	}


	/**
	 * Required by the Countable interface.
	 * @return int
	 */
	public function count() -> int
	{
		return this->getResultDriver()->getRowCount();
	}


	/**
	 * Returns the number of rows in a result set.
	 * @return int
	 */
	public function getRowCount() -> int
	{
		return this->getResultDriver()->getRowCount();
	}


	/**
	 * Required by the IteratorAggregate interface.
	 * @return ResultIterator
	 */
	public function getIterator() -> <ResultIterator>
	{
		return new ResultIterator(this);
	}


	


	/**
	 * Fetches the row at current position, process optional type conversion.
	 * and moves the internal cursor to the next position
	 * @return DibiRow|false  array on success, false if no next record
	 */
	public function fetchRow()
	{
		var row;
		let row = this->getResultDriver()->fetchRow(true);
		if !is_array(row) {
			return false;
		}
		let this->fetched = true;
		let this->normalize(row);
		if this->rowClass !== null {
			row = new {this->rowClass}(row);
		}
		return row;
	}


	/**
	 * Like fetchRow(), but returns only first field.
	 * @return mixed  value on success, false if no next record
	 */
	public function fetchSingle()
	{
		var row;
		let row = this->getResultDriver()->fetchRow(true);
		if !is_array(row) {
			return false;
		}
		let this->fetched = true;
		let this->normalize(row);
		return reset(row);
	}


	


	/**
	 * Fetches all records from table like key => value pairs.
	 * @param  string  associative key
	 * @param  string  value
	 * @return array
	 * @throws InvalidArgumentException
	 */
	public function fetchPairs(key = null, value = null)
	{
		this->seek(0);
		row = this->fetchRow();
		if (!row) {
			return [];  // empty result set
		}

		data = [];

		if (value === null) {
			if (key !== null) {
				throw new InvalidArgumentException("Either none or both columns must be specified.");
			}

			// autodetect
			tmp = array_keys(row->toArray());
			key = tmp[0];
			if (count(row) < 2) { // indexed-array
				do {
					data[] = row[key];
				} while (row = this->fetch());
				return data;
			}

			value = tmp[1];

		} else {
			if (!property_exists(row, value)) {
				throw new InvalidArgumentException("Unknown value column "value".");
			}

			if (key === null) { // indexed-array
				do {
					data[] = row[value];
				} while (row = this->fetch());
				return data;
			}

			if (!property_exists(row, key)) {
				throw new InvalidArgumentException("Unknown key column "key".");
			}
		}

		do {
			data[ (string) row[key] ] = row[value];
		} while (row = this->fetch());

		return data;
	}


	/********************* column types ****************d*g**/


	/**
	 * Autodetect column types.
	 * @return void
	 */
	private function detectTypes()
	{
		cache = DibiColumnInfo::getTypeCache();
		try {
			foreach (this->getResultDriver()->getResultColumns() as col) {
				this->types[col["name"]] = cache->{col["nativetype"]};
			}
		} catch (DibiNotSupportedException e) {}
	}


	/**
	 * Converts values to specified type and format.
	 * @param  array
	 * @return void
	 */
	private function normalize(array row)
	{
		var key, type, value, tmp;

		for key, type in this->types {

			if !isset(row[key])) { // null
				continue;
			}
			let value = row[key];

			if (value === false || type === Query::TYPE_TEXT) {
				continue;
			} 

			if (type === Query::TYPE_INTEGER) {
				let row[key] = is_float(tmp = value * 1) ? value : tmp;

			} elseif (type === Query::TYPE_FLOAT) {
				row[key] = ltrim((string) (tmp = (float) value), "0") === ltrim(rtrim(rtrim(value, "0"), "."), "0") ? tmp : value;

			} elseif (type === Query::TYPE_BOOL) {
				row[key] = ((bool) value) && value !== "f" && value !== "F";

			} elseif (type === Query::TYPE_DATE || type === Query::TYPE_DATETIME) {
				if ((int) value !== 0 || substr((string) value, 0, 3) === "00:") { // "", null, false, "0000-00-00", ...
					value = new DibiDateTime(value);
					row[key] = empty(this->formats[type]) ? value : value->format(this->formats[type]);
				}

			} elseif (type === Query::TYPE_BINARY) {
				row[key] = this->getResultDriver()->unescape(value, type);
			}
		}
		return row;
	}


	/**
	 * Define column type.
	 * @param  string  column
	 * @param  string  type (use constant Dibi::*)
	 * @return self
	 */
	public function setType(col, type)
	{
		this->types[col] = type;
		return this;
	}


	/**
	 * Returns column type.
	 * @return string
	 */
	public function getType(col)
	{
		return isset(this->types[col]) ? this->types[col] : null;
	}


	/**
	 * Sets data format.
	 * @param  string  type (use constant Dibi::*)
	 * @param  string  format
	 * @return self
	 */
	public function setFormat(type, format)
	{
		this->formats[type] = format;
		return this;
	}


	/**
	 * Returns data format.
	 * @return string
	 */
	public function getFormat(type)
	{
		return isset(this->formats[type]) ? this->formats[type] : null;
	}


	/********************* meta info ****************d*g**/


	/**
	 * Returns a meta information about the current result set.
	 * @return DibiResultInfo
	 */
	public function getInfo()
	{
		if (this->meta === null) {
			this->meta = new DibiResultInfo(this->getResultDriver());
		}
		return this->meta;
	}


	/**
	 * @deprecated
	 */
	public function getColumns()
	{
		return this->getInfo()->getColumns();
	}


	/********************* misc tools ****************d*g**/


	/**
	 * Displays complete result set as HTML or text table for debug purposes.
	 * @return void
	 */
	public function dump()
	{
		i = 0;
		this->seek(0);
		if (PHP_SAPI === "cli") {
			hasColors = (substr(getenv("TERM"), 0, 5) === "xterm");
			maxLen = 0;
			while (row = this->fetch()) {
				if (i === 0) {
					foreach (row as col => foo) {
						len = mb_strlen(col);
						maxLen = max(len, maxLen);
					}
				}

				if (hasColors) {
					echo "\033[1;37m#row: i\033[0m\n";
				} else {
					echo "#row: i\n";
				}

				foreach (row as col => val) {
					spaces = maxLen - mb_strlen(col) + 2;
					echo "col" . str_repeat(" ", spaces) .  "val\n";
				}

				echo "\n";
				i++;
			}

			if (i === 0) {
				echo "empty result set\n";
			}
			echo "\n";

		} else {
			while (row = this->fetch()) {
				if (i === 0) {
					echo "\n<table class=\"dump\">\n<thead>\n\t<tr>\n\t\t<th>#row</th>\n";

					foreach (row as col => foo) {
						echo "\t\t<th>" . htmlSpecialChars(col) . "</th>\n";
					}

					echo "\t</tr>\n</thead>\n<tbody>\n";
				}

				echo "\t<tr>\n\t\t<th>", i, "</th>\n";
				foreach (row as col) {
					//if (is_object(col)) col = col->__toString();
					echo "\t\t<td>", htmlSpecialChars(col), "</td>\n";
				}
				echo "\t</tr>\n";
				i++;
			}

			if (i === 0) {
				echo "<p><em>empty result set</em></p>";
			} else {
				echo "</tbody>\n</table>\n";
			}
		}
	}
	
}