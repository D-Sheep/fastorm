
/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */

namespace Phalcon\Logger\Adapter;

use Phalcon\Logger\Exception;

/**
 * Phalcon\Logger\Adapter\Syslog
 *
 * Sends logs to the system logger
 *
 *<code>
 *	$logger = new \Phalcon\Logger\Adapter\Syslog("ident", array(
 *		'option' => LOG_NDELAY,
 *		'facility' => LOG_MAIL
 *	));
 *	$logger->log("This is a message");
 *	$logger->log("This is an error", \Phalcon\Logger::ERROR);
 *	$logger->error("This is another error");
 *</code>
 */
class Syslog extends \Phalcon\Logger\Adapter implements \Phalcon\Logger\AdapterInterface
{

	protected _opened = false;

	/**
	 * Phalcon\Logger\Adapter\Syslog constructor
	 *
	 * @param string name
	 * @param array options
	 */
	public function __construct(name, options=null)
	{
		var option, facility;

		if name {

			if !fetch option, options["option"] {
				let option = LOG_ODELAY;
			}

			if !fetch facility, options["facility"] {
				let facility = LOG_USER;
			}

			openlog(name, option, facility);
			let this->_opened = true;
		}

	}

	/**
	 * Returns the internal formatter
	 *
	 * @return Phalcon\Logger\Formatter\Line
	 */
	public function getFormatter()
	{
		var formatter;

		let formatter = this->_formatter;
		if typeof formatter != "object" {
			let formatter = new \Phalcon\Logger\Formatter\Syslog(), this->_formatter = formatter;
		}
		return formatter;
	}

	/**
	 * Writes the log to the stream itself
	 *
	 * @param string message
	 * @param int type
	 * @param int time
	 */
	public function logInternal(message, int type, int time)
	{
		var appliedFormat;

		let appliedFormat = this->getFormatter()->format(message, type, time);
		if typeof appliedFormat != "array" {
			throw new Exception("The formatted message is not valid");
		}

		syslog(appliedFormat[0], appliedFormat[1]);
	}

	/**
 	 * Closes the logger
 	 *
 	 * @return boolean
 	 */
	public function close()
	{
		if this->_opened {
			closelog();
		}
	}

}
