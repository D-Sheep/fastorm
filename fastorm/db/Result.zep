
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
		var row, className;
		let row = this->getResultDriver()->fetchRow(true);
		if !is_array(row) {
			return false;
		}
		let this->fetched = true;
		let row = this->normalize(row);

		if this->rowClass !== null {
			let className = this->rowClass;
			let row = new {className}(row);
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
		let row = this->normalize(row);
		return reset(row);
	}


	


	/**
	 * Fetches all records from table like key => value pairs.
	 * @param  string  associative key
	 * @param  string  value
	 * @return array
	 * @throws InvalidArgumentException
	 */
	public function fetchPairs(key = null, value = null) -> array
	{
		var row, tmp, data;
		this->seek(0);
		let row = this->fetchRow();

		if !row {
			return [];  // empty result set
		}

		let data = [];

		if value === null {
			if key !== null {
				throw new \InvalidArgumentException("Either none or both columns must be specified.");
			}

			// autodetect
			let tmp = array_keys(row->toArray());
			let key = tmp[0];
			if count(row) < 2 { // indexed-array
				loop {
					let data[] = row[key];
					let row = this->fetchRow();
					if !row {
						break;
					}
				};
				return data;
			}

			let value = tmp[1];

		} else {
			if !property_exists(row, value) {
				throw new \InvalidArgumentException("Unknown value column '".value."'.");
			}

			if key === null { // indexed-array
				loop {
					let data[] = row[value];
					let row = this->fetchRow();
					if !row {
						break;
					}
				} 
				return data;
			}

			if !property_exists(row, key) {
				throw new \InvalidArgumentException("Unknown key column '".key."'.");
			}
		}

		loop {
			let data[ (string) row[key] ] = row[value];
			let row = this->fetchRow();
			if !row {
				break;
			}
		};

		return data;
	}


	

	/**
	 * Autodetect column types.
	 * @return void
	 */
	private function detectTypes()
	{
		var cache, columns, col, nativetype, e;
		let cache = ColumnInfo::getTypeCache();
		try {
			let columns = this->getResultDriver()->getResultColumns();
			for col in columns {
				let nativetype = col["nativetype"];
				let this->types[col["name"]] = cache->{nativetype};
			}
		} catch DbException, e {
			return;
		}
	}


	/**
	 * Converts values to specified type and format.
	 * @param  array
	 * @return void
	 */
	private function normalize(array row)
	{
		var key, type, value, tmp, left, right;

		if typeof row !== "array" {
			let row = [];
		}

		for key, type in this->types {

			if !isset(row[key]) { // null
				continue;
			}
			let value = row[key];

			if (value === false || type === Query::TYPE_TEXT) {
				continue;
			} 

			if (type === Query::TYPE_INTEGER) {
				let tmp =  value * 1;
				if is_float(tmp) {
					let row[key] = value;
				} else {
					let row[key] = tmp;
				}

			} else { if (type === Query::TYPE_FLOAT) {
				let tmp = (float) value;
				let left = ltrim((string) tmp, "0");
				let right = ltrim(rtrim(rtrim(value, "0"), "."), "0");

				if left === right {
					let row[key] = tmp;
				} else {
					let row[key] = value;
				}

			} else { if (type === Query::TYPE_BOOL) {
				let row[key] = ((bool) value) && value !== "f" && value !== "F";

			} else { if (type === Query::TYPE_DATE || type === Query::TYPE_DATETIME) {
				if ((int) value !== 0 || substr((string) value, 0, 3) === "00:") { // "", null, false, "0000-00-00", ...
					let value = new \DateTime(value);
					if empty this->formats[type] {
						let row[key] = value;
					} else {
						let row[key] = value->format(this->formats[type]);
					}
				}

			} else { if (type === Query::TYPE_BINARY) {
				let row[key] = this->getResultDriver()->unescape(value, type);
			}}}}}
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
		let this->types[col] = type;
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
		let this->formats[type] = format;
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


	

	/**
	 * Returns a meta information about the current result set.
	 * @return DibiResultInfo
	 */
	public function getInfo()
	{
		if (this->meta === null) {
			//let this->meta = new DibiResultInfo(this->getResultDriver());
		}
		return this->meta;
	}
	
}