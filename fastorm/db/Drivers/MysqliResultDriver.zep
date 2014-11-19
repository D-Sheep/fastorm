namespace Fastorm\Db\Drivers;

class MysqliResultDriver implements \Fastorm\Db\IResultDriver {


	const ERROR_ACCESS_DENIED = 1045;
	const ERROR_DUPLICATE_ENTRY = 1062;
	const ERROR_DATA_TRUNCATED = 1265;

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
	public function __construct(<\mysqli_result> result, var isBuffered)
	{
		let this->autoFree = true;
		let this->resultSet = result;
		let this->buffered = isBuffered;
		if !extension_loaded("mysqli") {
			throw new \Fastorm\Db\DbException("Db not supported exception: PHP extension 'mysqli' is not loaded.");
		}
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
			return this->resultSet->fetch_assoc();
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
		var metaRow, fieldsCount, i, table, columns;

		if empty self::types {
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
		
		let columns = [];
		let fieldsCount = this->resultSet->field_count;
		let i = 0;

		while (i < fieldsCount) {

			let metaRow = this->resultSet->fetch_field_direct(i);

			if metaRow->table {
				let table = metaRow->table . "." . metaRow->name;
			} else {
				let table = metaRow->name;
			}
			//TODO - funguje detect type spravne?

			let columns[] = [
				"name" : metaRow->name,
				"table" : metaRow->orgtable,
				"fullname" : table,
				"nativetype" : self::types[metaRow->type],
				"vendor" : metaRow 
			];

			let i++;
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

}