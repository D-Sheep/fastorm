
namespace Fastorm\Db;

class DbException extends \Fastorm\Exception {

	protected sql;

	protected args;

	public function __construct(exception, code = null, previous = null, sql = null, args = null) {
		parent::__construct(exception, code, previous);
		let this->sql = sql;
		let this->args = args;
	}

	public function getSql() {
		return this->sql;
	}

	public function getArgs() {
		return this->args;
	}
	
}