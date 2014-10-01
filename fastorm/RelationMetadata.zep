
namespace Fastorm;

class RelationMetadata
{

    protected destinationClass;

    protected destinationProperty;

    protected alias;

    protected destinationMetadata;

    protected sourceMetadata;

    protected fieldAliases;

    public function __construct(var originalMetadata, var destinationClassName, var destinationPropertyName, var methodAlias)
    {
        let this->destinationClass = destinationClassName;
        let this->destinationProperty = destinationPropertyName;
        let this->alias = methodAlias;
        let this->sourceMetadata = originalMetadata;
    }

    public function getMetadata()
    {
        if this->destinationMetadata === null {
            let this->destinationMetadata = ObjectMetadata::getMetadata(this->destinationClass);
        }
        return this->destinationMetadata;
    }

    public function getTableAlias()
    {
        var ret;
        let ret = ObjectMetadata::toPropertyName(this->alias);
        return ret;
    }

    public function getDestinationFieldAlias()
    {
        return "o_" . this->getTableAlias() . "_" . this->destinationProperty;
    }

    public function getFieldAliases()
    {
        if this->fieldAliases == null {
            this->generateFieldAliases();
        }
        return this->fieldAliases;
    }

    public function getDestinationField()
    {
        return this->destinationProperty;
    }

    public function getDestinationClass()
    {
        return this->destinationClass;
    }

    private function generateFieldAliases()
    {
        var propName, propFlags, fields, fieldAlias, tabAlias;
        let this->fieldAliases = [];
        let fields = this->getMetadata()->getFields();
        let tabAlias = this->getTableAlias();

        for propName, propFlags in fields
        {
            let fieldAlias = "o_" . tabAlias . "_" . propName;
            let this->fieldAliases[fieldAlias] = propName;
        }
    }

}