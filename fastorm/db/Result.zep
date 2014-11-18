
namespace Fastorm\Db;

class Result implements \IteratorAggregate {

	/** @var array  IResultDriver */
	protected _driver;

	
	protected _types;

	/** @var bool  Already fetched? Used for allowance for first seek(0) */
	protected _fetched;

	protected _rowClass;

	protected _metadata;


	public function __construct(<IResultDriver> driver, <\Fastorm\ObjectMetadata> metadata = null)
	{
		let this->_fetched = false;
		let this->_driver = driver;
		let this->_metadata = metadata;
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
		if this->_driver !== null {
			this->_driver->free();
			let this->_driver = null;
			let this->_metadata = null;
		}
		let this->_metadata = null;
		let this->_rowClass = null;
	}


	/**
	 * Safe access to property driver.
	 * @return IDibiResultDriver
	 * @throws RuntimeException
	 */
	public function getResultDriver() -> <IResultDriver>
	{
		if this->_driver === null {
			throw new \RuntimeException("Result-set was released from memory.");
		}

		return this->_driver;
	}


	

	/**
	 * Moves cursor position without fetching row.
	 * @param  int      the 0-based cursor pos to seek to
	 * @return boolean  true on success, false if unable to seek to specified record
	 * @throws DbException
	 */
	public function seek(var seekRow) 
	{
		if seekRow !== 0 && this->_fetched {
			return (boolean) this->getResultDriver()->seek(seekRow);
		} else {
			return true;
		}
	}


	/**
	 * Required by the Countable interface.
	 * @return int
	 */
	public function count()
	{
		return this->getResultDriver()->getRowCount();
	}


	/**
	 * Returns the number of rows in a result set.
	 * @return int
	 */
	public function getRowCount()
	{
		return this->getResultDriver()->getRowCount();
	}


	/**
	 * Required by the IteratorAggregate interface.
	 * @return ResultIterator
	 */
	public function getIterator()
	{
		return new ResultIterator(this);
	}
	


	/**
	 * Fetches the row at current position, process optional type conversion.
	 * and moves the internal cursor to the next position
	 * @return DibiRow|null  array on success, false if no next record
	 */
	public function fetchRow(forceArray = false)
	{
		var resultedRow, normalized;

		let resultedRow = this->getResultDriver()->fetchRow(true);

		if typeof resultedRow !== "array" {
			return false;
		}

		let this->_fetched = true;
		let normalized = this->normalize(resultedRow);
		
		if this->_metadata !== null {
			if forceArray {
				return normalized;
			} else {
				return this->_metadata->newInstance(normalized);
			}
		} else {
			if this->_rowClass !== null && forceArray === false {
				var className;
				let className = this->_rowClass;
				return new {className}(normalized);
			} else {
				return normalized;
			}
		}
	}


	/**
	 * Like fetchRow(), but returns only first field.
	 * @return mixed  value on success, false if no next record
	 */
	public function fetchSingle()
	{
		var singleRow, returnValue;
		let singleRow = this->getResultDriver()->fetchRow(true);
		if !is_array(singleRow) {
			return false;
		}
		let this->_fetched = true;
		let returnValue = this->normalize(singleRow);
		return reset(returnValue);
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
		var row, tmp, data;
		this->seek(0);
		let row = this->fetchRow(true);
		let value = (string) value;
		if !row {
			return [];  // empty result set
		}

		let data = [];

		if value === null {
			if key !== null {
				throw new \InvalidArgumentException("Either none or both columns must be specified.");
			}

			// autodetect
			let tmp = array_keys(row);
			let key = tmp[0];
			if count(row) < 2 { // indexed-array
				loop {
					let data[] = row[key];
					let row = this->fetchRow(true);
					if !row {
						break;
					}
				};
				return data;
			}

			let value = tmp[1];

		} else {
			if !isset row[value] {
				throw new \InvalidArgumentException("Unknown value column '".value."'.");
			}

			if key === null { // indexed-array
				loop {
					let data[] = row[value];
					let row = this->fetchRow(true);
					if !row {
						break;
					}
				} 
				return data;
			}

			if !isset row[key] {
				throw new \InvalidArgumentException("Unknown key column '".key."'.");
			}
		}

		loop {
			let data[ (string) row[key] ] = row[value];
			let row = this->fetchRow(true);
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
		var columns, col, nativetype, e, type;

		try {
			let this->_types = [];
			let columns = this->getResultDriver()->getResultColumns();
			for col in columns {
				let nativetype = col["nativetype"];
				let type = \Fastorm\Db\ColumnInfo::detectType(nativetype);
				if type !== Query::TYPE_TEXT {
					let this->_types[col["name"]] = type;
				}
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
	private function normalize(var rawRow)
	{

		var key, type, value, ret;//, tmp, left, right;

		if typeof rawRow !== "array" {
			let rawRow = [];
		}

		let ret = rawRow;

		for key, type in this->_types {

			if !isset(rawRow[key]) {
				continue;
			}

			let value = rawRow[key];

			if (value === false || value === null) {
				continue;
			} 

			if (type === Query::TYPE_INTEGER) {
				let ret[key] = (int) value;
			} else { if (type === Query::TYPE_FLOAT) {
				let ret[key] = (float) value;
			} else { if (type === Query::TYPE_BOOL) {
				let ret[key] = ((bool) value) && value !== "f" && value !== "F";

			} else { if (type === Query::TYPE_DATE || type === Query::TYPE_DATETIME) {
				if (value !== "0000-00-00" && value !== "0000-00-00 00:00:00") {
					let ret[key] = new \DateTime(value);
				}

			} else { if (type === Query::TYPE_BINARY) {
				let ret[key] = this->getResultDriver()->unescape(value, type);
			}}}}}
		}

		return ret;
	}


	/**
	 * Define column type.
	 * @param  string  column
	 * @param  string  type (use constant Dibi::*)
	 * @return Result
	 */
	public function setType(col, type)
	{
		let this->_types[col] = type;
		return this;
	}


	/**
	 * Returns column type.
	 * @return string
	 */
	public function getType(col)
	{
		if isset(this->_types[col]) {
			return this->_types[col];
		} else {
			return null;
		}
	}
	
	
}