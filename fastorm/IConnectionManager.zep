
namespace Fastorm;

interface IConnectionManager {
	
	public function getConnection(string name = null) -> <Db\Connection>;

}