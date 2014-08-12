
namespace Fastorm;

class ObjectQuery extends Db\Query
{
	
	protected metadata;

	public function __construct(<Db\Connection> connection, <ObjectMetadata> metadata) {
		let this->metadata = metadata;
		parent::__construct(connection);
		this->initiate();
	}

	private function initiate() {
		var key, flags, select, table;

		let select = [];
		let table = this->metadata->getTable();

		if table === null {
			throw new Exception("Object has not set table annotation");
		}

		for key, flags in this->metadata->getFields()
		{
			let select[] = table.".".key;
		}

		this->__call("select", [ implode(", ", select) ]);
		this->__call("from", [ table ]);
	}

	public function getMetadata() -> <ObjectMetadata>
	{
		return this->metadata;
	}

	public function joinObject(string onProperty, array justColumns = null) -> <ObjectQuery>
	{
		var property, joinMetadata, joinAlias;
		if this->metadata->hasJoin(onProperty."_id") {
			let property = onProperty."_id";
			let joinAlias = onProperty;
			let joinMetadata = this->metadata->getJoin(property);
		} else {
			let property = onProperty;
			let joinAlias = preg_replace("/_id$/", "", onProperty);
			let joinMetadata = this->metadata->getJoin(property);
		}

		if joinMetadata === null {
			throw new Exception("Property '".onProperty."' hasn't defined join.");
		}

		this->joinObjectWith(property, joinMetadata, joinAlias, justColumns);
		
		return this;
	}

	public function joinAll() {
		var property, className, joinMetadata, joinAlias;
		for property, className in this->metadata->getJoins() {
			let joinMetadata = ObjectMetadata::getMetadata(className);
			let joinAlias = preg_replace("/_id$/", "", property);
			this->joinObjectWith(property, joinMetadata, joinAlias);
		}
		return this;
	}

	private function joinObjectWith(property, joinMetadata, joinAlias, justColumns = null) {
		var alias, realName, aliases, select;
		let select = [];
		let aliases = this->metadata->getAliases(property, joinMetadata);
		for alias, realName in aliases {
			if justColumns === null || in_array(realName, justColumns) {
				let select[] = joinAlias.".".realName." AS ".alias;
			}
		}
		this->__call("select", [ implode(", ", select) ]);
		this->__call("leftJoin", [joinMetadata->getTable()." ".joinAlias]);
		this->__call("on", [joinAlias.".".joinMetadata->getIdField()." = ".this->metadata->getTable().".".property]);
	}

	protected function query(args)
	{
		var res, setup;
		let res = this->connection->query(args, this->metadata);
		for setup in this->setups {
			call_user_func_array([ res, array_shift(setup) ], setup);
		}
		return res;
	}



}