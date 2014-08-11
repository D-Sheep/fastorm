
namespace Fastorm\Db;

interface IResultDriver {

	/**
	 * Returns the number of rows in a result set.
	 * @return int
	 */
	public function getRowCount();

	/**
	 * Moves cursor position without fetching row.
	 * @param  int      the 0-based cursor pos to seek to
	 * @return boolean  TRUE on success, FALSE if unable to seek to specified record
	 * @throws DibiException
	 */
	public function seek(row);

	/**
	 * Fetches the row at current position and moves the internal cursor to the next position.
	 * @param  bool     TRUE for associative array, FALSE for numeric
	 * @return array    array on success, nonarray if no next record
	 * @internal
	 */
	public function fetchRow(type);

	/**
	 * Frees the resources allocated for this result set.
	 * @param  resource  result set resource
	 * @return void
	 */
	public function free();

	/**
	 * Returns metadata for all columns in a result set.
	 * @return array of {name, nativetype [, table, fullname, (int) size, (bool) nullable, (mixed) default, (bool) autoincrement, (array) vendor ]}
	 */
	public function getResultColumns();

	/**
	 * Returns the result set resource.
	 * @return mixed
	 */
	public function getResultResource();

	/**
	 * Decodes data from result set.
	 * @param  string    value
	 * @param  string    type (dibi::BINARY)
	 * @return string    decoded value
	 * @throws InvalidArgumentException
	 */
	public function unescape(value, $ype);

}