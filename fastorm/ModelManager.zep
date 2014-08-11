
namespace Fastorm;

class ModelManager
{

	protected static _sharedInstance;

	public static function getShared() -> <Fastorm\ModelManager>
	{
		if self::_sharedInstance === null {
			throw new Exception("M");
		}
		return self::_sharedInstance;
	}

}