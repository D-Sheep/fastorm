
namespace Src;

class TestClass {

	protected _test;

	public function __construct(var some) {
		let this->_test = some;
	}

	
	public function foo() {
		return this->_test;
	}

}