<?php 

namespace Fastorm\Db;

class DbException extends \Fastorm\Exception {

	protected $sql;

	protected $args;


	/**
	 * Constructor.
	 * @param string exception
     * @param int code
     * @param \Exception previous
     * @param string sql
     * @param args
	 * @return void
	 */
	public function __construct($exception, $code = null, $previous = null, $sql = null, $args = null) {
		parent::__construct(exception, code, previous);
		$this->sql = sql;
		$this->args = args;
	}

	/**
	 * Returns SQL.
	 * @return void
	 */
	public function getSql(){}

    /**
     * Returns
     */
    public function getArgs(){}
	
}