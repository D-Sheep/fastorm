
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
 |          Jason Rice                                                    |
 +------------------------------------------------------------------------+
 */

namespace Phalcon\Mvc\Model\Validator;

/**
 * Phalcon\Mvc\Model\Validator\StringLength
 *
 * Simply validates specified string length constraints
 *
 *<code>
 *use Phalcon\Mvc\Model\Validator\StringLength as StringLengthValidator;
 *
 *class Subscriptors extends \Phalcon\Mvc\Model
 *{
 *
 *	public function validation()
 *	{
 *		this->validate(new StringLengthValidator(array(
 *			"field" => 'name_last',
 *			'max' => 50,
 *			'min' => 2,
 *			'messageMaximum' => 'We don\'t like really long names',
 *			'messageMinimum' => 'We want more than just their initials'
 *		)));
 *		if (this->validationHasFailed() == true) {
 *			return false;
 *		}
 *	}
 *
 *}
 *</code>
 *
 */
class StringLength extends \Phalcon\Mvc\Model\Validator implements \Phalcon\Mvc\Model\ValidatorInterface
{
	/**
	 * Executes the validator
	 *
	 * @param Phalcon\Mvc\ModelInterface record
	 * @return boolean
	 */
	public function validate(<\Phalcon\Mvc\ModelInterface> record) -> boolean
	{
		var field, isSetMin, isSetMax, value, length, invalidMaximum, invalidMinimum,
			maximum, minimum, message;

		let field = this->getOption("field");
		if typeof field != "string" {
			throw new \Phalcon\Mvc\Model\Exception("Field name must be a string");
		}

		/**
		 * At least one of 'min' or 'max' must be set
		 */
		let isSetMin = this->isSetOption("min");
		let isSetMax = this->isSetOption("max");

		if !isSetMin && !isSetMax {
			throw new \Phalcon\Mvc\Model\Exception("A minimum or maximum must be set");
		}

		let value = record->readAttribute("field");

		if this->isSetOption("allowEmpty") && empty value {
			return true;
		}

		/**
		 * Check if mbstring is available to calculate the correct length
		 */
		if function_exists("mb_strlen") {
			let length = mb_strlen(value);
		} else {
			let length = strlen(value);
		}

		let invalidMaximum = false;
		let invalidMinimum = false;

		/**
		 * Maximum length
		 */
		if isSetMax {

			let maximum = this->getOption("max");
			if length > maximum {

				/**
				 * Check if the developer has defined a custom message
				 */
				let message = this->getOption("messageMaximum");
				if empty message {
					let message = "Value of field :field exceeds the maximum :max characters";
				}

				this->appendMessage(strtr(message, [":field": field, ":max":  maximum]), field, "TooLong");
				return false;
			}
		}

		/**
		 * Minimum length
		 */
		if isSetMin {

			let minimum = this->getOption("min");
			if length < minimum {

				/**
				 * Check if the developer has defined a custom message
				 */
				let message = this->getOption("messageMinimum");
				if empty message {
					let message = "Value of field :field is less than the minimum :min characters";
				}

				this->appendMessage(strtr(message, [":field": field, ":min":  minimum]), field, "TooShort");
				return false;
			}
		}

		return true;
	}
}
