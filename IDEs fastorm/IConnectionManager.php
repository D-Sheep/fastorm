<?php
/**
 * Created by JetBrains PhpStorm.
 * User: davidmenger
 * Date: 13/08/14
 * Time: 11:10
 * To change this template use File | Settings | File Templates.
 */

namespace Fastorm;


interface IConnectionManager {

    /**
     * @param null $name
     * @return \Fastorm\Db\Connection
     */
    public function getConnection($name = null);

}