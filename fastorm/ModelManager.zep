
namespace Fastorm;

class ModelManager
{

	protected static _sharedInstance;

	protected connectionManager;

	public static function getShared() -> <ModelManager>
	{
		if self::_sharedInstance === null {
			throw new Exception("Model Manager not initialized");
		}
		return self::_sharedInstance;
	}

	public function __construct(<IConnectionManager> connectionManager)
	{
		if self::_sharedInstance !== null {
			throw new Exception("Model Manager can be initialized only once");
		}
		let this->connectionManager = connectionManager;
		let self::_sharedInstance = this;
	}

	public function getConnection(<ObjectMetadata> metadata) -> <Db\Connection>
	{
		return this->connectionManager->getConnection(metadata->getStorage());
	}



}