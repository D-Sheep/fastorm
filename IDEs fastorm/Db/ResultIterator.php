<?php 

namespace Fastorm\Db;

class ResultIterator implements \Iterator {


	/**
	 * @param  DibiResult
	 */
	public function __construct(<Result> result) {}


	/**
	 * Rewinds the iterator to the first element.
	 * @return void
	 */
	public function rewind() {}


	/**
	 * Returns the key of the current element.
	 * @return mixed
	 */
	public function key() {}


	/**
	 * Returns the current element.
	 * @return mixed
	 */
	public function current() {}


	/**
	 * Moves forward to next element.
	 * @return void
	 */
	public function next() {}


	/**
	 * Checks if there is a current element after calls to rewind() or next().
	 * @return bool
	 */
	public function valid() {}


	/**
	 * Required by the Countable interface.
	 * @return int
	 */
	public function count() {}
	
}