<?php
/**
 * Created by JetBrains PhpStorm.
 * User: davidmenger
 * Date: 13/08/14
 * Time: 11:08
 * To change this template use File | Settings | File Templates.
 */

namespace Fastorm;


abstract class ConnectionManager implements IConnectionManager {

    protected $connections;

    protected $defaultConnectionName;

    public function __construct($defaultConnectionName) {

    }

    final public function getConnection($name = null)
    {

    }

	public abstract function createConnection($name);

}