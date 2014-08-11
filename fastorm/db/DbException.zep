
namespace Fastorm\Db;

class DbException extends \Fastorm\Exception {

	protected sql;

	public function __construct(exception, code = null, previous = null, sql = null) {
		parent::__construct(exception, code, previous);
		let this->sql = sql;
	}

	public function getSql() {
		return this->sql;
	}
	
}