
namespace Fastorm\Db;

class ResultIterator implements \Iterator, \Countable {

	protected result;

	protected internalPosition;

	public _actual;


	/**
	 * @param  DibiResult
	 */
	public function __construct(<Result> resultData)
	{
		let this->result = resultData;
		let this->_actual = null;
	}


	/**
	 * Rewinds the iterator to the first element.
	 * @return void
	 */
	public function rewind()
	{
		if (this->internalPosition == 3) {
			//echo "\n\n WHY REWIND ?????????????? n\n";
		}
		let this->internalPosition = 0;
		this->result->seek(0);
		let this->_actual = this->result->fetchRow();
	}


	/**
	 * Returns the key of the current element.
	 * @return mixed
	 */
	public function key()
	{
		return this->internalPosition;
	}


	/**
	 * Returns the current element.
	 * @return mixed
	 */
	public function current()
	{
		return this->_actual;
	}


	/**
	 * Moves forward to next element.
	 * @return void
	 */
	public function next()
	{
		let this->_actual = this->result->fetchRow();
		let this->internalPosition = this->internalPosition + 1;
	}


	/**
	 * Checks if there is a current element after calls to rewind() or next().
	 * @return bool
	 */
	public function valid()
	{
		if this->_actual == false || this->_actual === null {
			return false;
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
		return this->result->getRowCount();
	}
	
}