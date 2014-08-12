
namespace Fastorm;

abstract class ConnectionManager implements IConnectionManager {
	
	protected connections;

	protected defaultConnectionName;

	public function __construct(string defaultConnectionName) {
		let this->connections = [];
		let this->defaultConnectionName = defaultConnectionName;
	}

	final public function getConnection(string name = null) 
	{
		if name === null {
			let name = (string) this->defaultConnectionName;
		}
		if !isset this->connections[name] {
			let this->connections[name] = this->createConnection(name);
		}
		return this->connections[name];
	}

	public abstract function createConnection(string name);

}