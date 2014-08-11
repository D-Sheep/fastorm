
namespace Fastorm\Db;

class ResultIterator implements \Iterator {

	/** @var DibiResult */
	private result;

	/** @var int */
	private row;

	/** @var int */
	private pointer;


	/**
	 * @param  DibiResult
	 */
	public function __construct(<Result> result)
	{
		let this->result = result;
	}


	/**
	 * Rewinds the iterator to the first element.
	 * @return void
	 */
	public function rewind()
	{
		let this->pointer = 0;
		this->result->seek(0);
		let this->row = this->result->fetchRow();
	}


	/**
	 * Returns the key of the current element.
	 * @return mixed
	 */
	public function key()
	{
		return this->pointer;
	}


	/**
	 * Returns the current element.
	 * @return mixed
	 */
	public function current()
	{
		return this->row;
	}


	/**
	 * Moves forward to next element.
	 * @return void
	 */
	public function next()
	{
		let this->row = this->result->fetchRow();
		let this->pointer++;
	}


	/**
	 * Checks if there is a current element after calls to rewind() or next().
	 * @return bool
	 */
	public function valid()
	{
		return !empty(this->row);
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