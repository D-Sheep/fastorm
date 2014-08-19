
namespace Fastorm\Db;

class Translator {

	/** @var Connection */
	private connection;

	/** @var IDibiDriver */
	private driver;

	/** @var int */
	private cursor;

	/** @var array */
	private args;

	/** @var bool */
	private hasError;

	/** @var bool */
	private comment;

	/** @var int */
	private ifLevel;

	/** @var int */
	private ifLevelStart;

	/** @var int */
	private limit;

	/** @var int */
	private offset;


	public function __construct(<Connection> connection)
	{
		let this->connection = connection;
		/* delimite */ 
	}


	/**
	 * Generates SQL.
	 * @param  array
	 * @return string
	 * @throws DbException
	 */
	public function translate(args) -> string
	{
		var commandIns, arg, sql;
		int toSkip, lastArr;
		string ret;

		let sql = [];

		if this->driver === null {
			let this->driver = this->connection->getDriver();
		}

		if typeof args !== "array" {
			throw new DbException("Input of translate function must be array");
		}

		let args = array_values(args);

		while (count(args) === 1 && is_array(args[0])) { // implicit array expansion
			let args = array_values(args[0]);
		}
		let this->args = args;

		let this->limit = -1;
		let this->offset = 0;
		let this->hasError = false;
		let commandIns = null;
		let lastArr = null;
		// shortcuts
		let this->cursor = 0;

		// conditional sql
		let this->ifLevel = 0;
		let this->ifLevelStart = 0;
		let this->comment = false;

		while (this->cursor < count(this->args)) {
			let arg = this->args[this->cursor];
			let this->cursor++;

			// simple string means SQL
			if typeof arg === "string" {
				// speed-up - is regexp required?
				let toSkip = (int) strcspn(arg,  "`['\":%?");

				if strlen(arg) === toSkip { // needn"t be translated
					let sql[] = arg;
				} else {
					let sql[] = substr(arg, 0, toSkip)
/*
					. preg_replace_callback("/
					(?=[`[\"":%?])                    ## speed-up
					(?:
						`(.+?)`|                     ## 1) `identifier`
						\[(.+?)\]|                   ## 2) [identifier]
						(\")((?:\"\"|[^\"])*)\"|     ## 3,4) "string"
						(")((?:""|[^"])*)"|          ## 5,6) "string"
						(\"|")|                      ## 7) lone quote
						:(\S*?:)([a-zA-Z0-9._]?)|    ## 8,9) :substitution:
						%([a-zA-Z~][a-zA-Z0-9~]{0,5})|## 10) modifier
						(\?)                         ## 11) placeholder
					)/xs",
*/                  // note: this can change this->args & this->cursor & ...
//	                                         /(?=[`[\'":%?])(?:`(.+?)`|\ [(.+?)\]|(\')((?:\'\'|[^\'])*)\'|(")((?:""|[^"])*)" |(\'|")|:(\S*?:)([a-zA-Z0-9._]?)|%([a-zA-Z~][a-zA-Z0-9~]{0,5})|(\?))/s
					. preg_replace_callback("/(?=[`['\":%?])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?)|%([a-zA-Z~][a-zA-Z0-9~]{0,5})|(\\?))/s",
							[this, "cb"],
							substr(arg, toSkip)
					);
					if preg_last_error() {
						throw new \Fastorm\Exception();
					}
				}
				continue;
			}

			if this->comment {
				let sql[] = "...";
				continue;
			}

			if arg instanceof \Traversable {
				let arg = iterator_to_array(arg);
			}

			if typeof arg === "array" {
				if is_string(key(arg)) {
					// associative array -> autoselect between SET or VALUES & LIST
					if commandIns === null {
						let commandIns = strtoupper(substr(ltrim(this->args[0]), 0, 6));
						let commandIns = commandIns === "INSERT" || commandIns === "REPLAC";
						let sql[] = this->formatValue(arg, commandIns ? "v" : "a");
					} else {
						if (lastArr === this->cursor - 1) {
							let sql[] = ",";
						}
						let sql[] = this->formatValue(arg, commandIns ? "l" : "a");
					}
					let lastArr = (int) this->cursor;
					continue;
				}
			}

			// default processing
			let sql[] = this->formatValue(arg, false);
		} // while


		if this->comment {
			let sql[] = "*/";
		}

		let ret = (string) implode(" ", sql);

		if (this->hasError) {
			throw new DbException("SQL translate error", 0, null, ret, args);
		}

		// apply limit
		if (this->limit > -1 || this->offset > 0) {
			this->driver->applyLimit(ret, this->limit, this->offset);
		}

		return ret;
	}


	/**
	 * Apply modifier to single value.
	 * @param  mixed
	 * @param  string
	 * @return string
	 */
	public function formatValue(value, string modifier = null) -> string
	{
		var k, v, pair, vx, kx, proto;
		string op;

		//var_dump("formatValue", value, modifier);

		if this->comment {
			return "...";
		}

		if this->driver === null {
			let this->driver = this->connection->getDriver();
		}

		// array processing (with or without modifier)
		if value instanceof \Traversable {
			let value = iterator_to_array(value);
		}

		if typeof value === "array" {
			let vx = [];
			let kx = [];

			switch (modifier) {
				case "and":
				case "or":  // key=val AND key IS null AND ...
					if empty(value) {
						return "1=1";
					}

					for k, v in value {
						if typeof k === "string" {
							let pair = explode("%", (string) k, 2); // split into identifier & modifier
							let k = this->delimite(pair[0]) . " ";
							if !isset(pair[1]) {
								let v = this->formatValue(v);
								if v === "null" {
									let vx[] = k . "IS " . v;
								} else {
									let vx[] = k . "= " . v;
								}

							} else { if pair[1] === "ex" { // TODO: this will be removed
								let vx[] = k . this->formatValue(v, "ex");

							} else {
								let v = this->formatValue(v, pair[1]);
								if pair[1] === "l" || pair[1] === "in" {
									let op = "IN ";
								} else { if strpos(pair[1], "like") !== false {
									let op = "LIKE ";
								} else { if v === "null" {
									let op = "IS ";
								} else {
									let op = "= ";
								}}}
								let vx[] = k . op . v;
							}}

						} else {
							let vx[] = this->formatValue(v, "ex");
						}
					}
					return "(" . implode(") " . strtoupper(modifier) . " (", vx) . ")";

				case "n":  // key, key, ... identifier names
					for k, v in value {
						if typeof k === "string" {
							if empty (v) {
								let op = "";
							} else {
								let op = " AS " . this->delimite(v);
							}

							let vx[] = this->delimite(k . op);
						} else {
							let pair = explode("%",(string)  v, 2); // split into identifier & modifier
							let vx[] = this->delimite(pair[0]);
						}
					}
					return implode(", ", vx);


				case "a": // key=val, key=val, ...
					for k, v in value {
						let pair = explode("%", (string) k, 2); // split into identifier & modifier
						let vx[] = this->delimite(pair[0]) . "=" . this->fomattedPairValue(pair, v);
					}
					return implode(", ", vx);


				case "in":// replaces scalar %in modifier!
				case "l": // (val, val, ...)
					for k, v in value {
						let pair = explode("%", (string) k, 2); // split into identifier & modifier
						let vx[] = this->fomattedPairValue(pair, v);
					}
					if vx || modifier === "l" {
						return "(" . implode(", ", vx) . ")";
					} else {
						return "(null)";
					}

				case "v": // (key, key, ...) VALUES (val, val, ...)
					for k, v in value {
						let pair = explode("%", (string) k, 2); // split into identifier & modifier
						let kx[] = this->delimite(pair[0]);
						let vx[] = this->fomattedPairValue(pair, v);
					}
					return "(" . implode(", ", kx) . ") VALUES (" . implode(", ", vx) . ")";

				case "m": // (key, key, ...) VALUES (val, val, ...), (val, val, ...), ...
					boolean declared;
					let declared = false;
					let proto = null;
					for k, v in value {
						if typeof v === "array" {
							if declared {
								if (proto !== array_keys(v)) {
									let this->hasError = true;
									return "**Multi-insert array '" . k . "' is different.**";
								}
							} else {
								let proto = array_keys(v);
								let declared = true;
							}
						} else {
							let this->hasError = true;
							return "**Unexpected type " . gettype(v) . "**";
						}

						let pair = explode("%", (string) k, 2); // split into identifier & modifier
						let kx[] = this->delimite(pair[0]);

						var k2, v2;

						for k2, v2 in v {
							if !isset(vx[k2]) || typeof vx[k2] !== "array" {
								let vx[k2] = [];
							}
							let vx[k2][] = this->fomattedPairValue(pair, v);
						}
					}
					for k, v in vx {
						let vx[k] = "(" . implode(", ", v) . ")";
					}
					return "(" . implode(", ", kx) . ") VALUES " . implode(", ", vx);

				case "by": // key ASC, key DESC
					for k, v in value {
						if typeof v === "array" {
							let vx[] = this->formatValue(v, "ex");
						} else { if (is_string(k)) {
							if (is_string(v) && strncasecmp(v, "d", 1)) || v > 0 {
								let v = "ASC";
							} else {
								let v = "DESC";
							}
							let vx[] = this->delimite(k) . " " . v;
						} else {
							let vx[] = this->delimite(v);
						}}
					}
					return implode(", ", vx);

				case "ex":
				case "sql":
					var translator;
					let translator = new self(this->connection);
					return translator->translate(value);

				default:  // value, value, value - all with the same modifier
					for v in value {
						let vx[] = this->formatValue(v, modifier);
					}
					return implode(", ", vx);
			}
		}


		// with modifier procession
		if modifier !== null {
			if (value !== null && !is_scalar(value) && !(value instanceof \DateTime) && !(value instanceof \DateTimeInterface)) {  // array is already processed
				let this->hasError = true;
				return "**Unexpected type " . gettype(value) . "**";
			}

			switch (modifier) {
				case "s":  // string
				case "bin":// binary
				case "b":  // boolean
					return this->nullEscape(value, modifier);

				case "sN": // string or null
				case "sn":
					return this->nullEscape(value, Query::TYPE_TEXT, "");

				case "iN": // signed int or null
				case "in": // deprecated
					if value == "" {
						let value = null;
					}
					// intentionally break omitted

				case "i":  // signed int
				case "u":  // unsigned int, ignored
					// support for long numbers - keep them unchanged
					if (is_string(value) && preg_match("#[+-]?\\d++(e\\d+)?\\z#A", value)) {
						return value;
					} else {
						if (value === null) {
							return "null";
						} else {
							let proto = (int) (value + 0);
							return (string) proto."";
						}
					}

				case "f":  // float
					// support for extreme numbers - keep them unchanged
					if (is_string(value) && is_numeric(value) && strpos(value, "x") === false) {
						return value; // something like -9E-005 is accepted by SQL, HEX values are not
					} else {
						if value === null {
							return "null";
						} else {
							return rtrim(rtrim(number_format(value + 0, 10, ".", ""), "0"), ".");
						}
					}

				case "d":  // date
				case "t":  // datetime
					if value === null {
						return "null";
					} else {
						if is_numeric(value) {
							let value = (int) value; // timestamp

						} else { if (is_string(value)) {
							let value = new \DateTime(value);
						}}
						return this->driver->escape(value, modifier);
					}

				case "by":
				case "n":  // identifier name
					return this->delimite(value);

				case "ex":
				case "sql": // preserve as dibi-SQL  (TODO: leave only %ex)
					let value = (string) value;
					int toSkip;
					// speed-up - is regexp required?
					let toSkip = (int) strcspn(value, "`['\":");

					if (strlen(value) !== toSkip) {
						let value = substr(value, 0, toSkip)
						. preg_replace_callback(
							"/(?=[`['\":])(?:`(.+?)`|\\[(.+?)\\]|(')((?:''|[^'])*)'|(\")((?:\"\"|[^\"])*)\"|('|\")|:(\\S*?:)([a-zA-Z0-9._]?))/s",
							[ this, "cb" ],
							substr(value, toSkip)
						);
						if (preg_last_error()) {
							throw new DbException("PCRE exception");
						}
					}
					return value;

				case "SQL": // preserve as real SQL (TODO: rename to %sql)
					return (string) value;

				case "like~":  // LIKE string%
					return this->driver->escapeLike(value, 1);

				case "~like":  // LIKE %string
					return this->driver->escapeLike(value, -1);

				case "~like~": // LIKE %string%
					return this->driver->escapeLike(value, 0);

				case "and":
				case "or":
				case "a":
				case "l":
				case "v":
					let this->hasError = true;
					return "**Unexpected type " . gettype(value) . "**";

				default:
					let this->hasError = true;
					return "**Unknown or invalid modifier %modifier**";
			}
		}


		// without modifier procession
		if is_string(value) {
			return this->driver->escape(value, Query::TYPE_TEXT);

		} else { if (is_int(value)) {
			return (string) value;

		} else { if (is_float(value)) {
			return rtrim(rtrim(number_format(value, 10, ".", ""), "0"), ".");

		} else { if (is_bool(value)) {
			return this->driver->escape(value, Query::TYPE_BOOL);

		} else { if (value === null) {
			return "null";

		} else { if (value instanceof \DateTime) || (value instanceof \DateTimeInterface) {
			return this->driver->escape(value, Query::TYPE_DATETIME);

		}}}}}}

		let this->hasError = true;

		return "**Unexpected " . gettype(value) . "**";
	}

	private function fomattedPairValue(pairArray, value) -> string
	{
		if isset(pairArray[1]) {
			return this->formatValue(value,  pairArray[1]);
		} else {
			if typeof value === "array" {
				return this->formatValue(value, "ex");
			} else {
				return this->formatValue(value);
			}
		}
	}

	private function nullEscape(value, modifier, compareWith = null) {
		if value === compareWith {
			return "null";
		} else {
			return this->driver->escape(value, modifier);
		}
	}


	/**
	 * PREG callback from translate() or formatValue().
	 * @param  array
	 * @return string
	 */
	public function cb(matches)
	{
		//    [1] => `ident`
		//    [2] => [ident]
		//    [3] => "
		//    [4] => string
		//    [5] => "
		//    [6] => string
		//    [7] => lone-quote
		//    [8] => substitution
		//    [9] => substitution flag
		//    [10] => modifier (when called from self::translate())
		//    [11] => placeholder (when called from self::translate())


		if isset(matches[11]) { // placeholder
			
			if (this->cursor >= count(this->args)) {
				let this->hasError = true;
				return "**Extra placeholder**";
			}

			let this->cursor++;
			return this->formatValue(this->args[this->cursor - 1], false);
		}

		if isset(matches[10]) { // modifier
			var mod;

			let mod = matches[10];

			if this->cursor >= count(this->args) && mod !== "else" && mod !== "end" {
				let this->hasError = true;
				return "**Extra modifier %mod**";
			}

			if (mod === "if") {
				let this->ifLevel++;
				let this->cursor++;

				if !this->comment && !this->args[this->cursor - 1] {
					// open comment
					let this->ifLevelStart = this->ifLevel;
					let this->comment = true;
					return "/*";
				}
				return "";

			} else { if (mod === "else") {
				if (this->ifLevelStart === this->ifLevel) {
					let this->ifLevelStart = 0;
					let this->comment = false;
					return "*/";
				} else { if (!this->comment) {
					let this->ifLevelStart = this->ifLevel;
					let this->comment = true;
					return "/*";
				}}

			} else { if (mod === "end") {
				let this->ifLevel--;
				if (this->ifLevelStart === this->ifLevel + 1) {
					// close comment
					let this->ifLevelStart = 0;
					let this->comment = false;
					return "*/";
				}
				return "";

			} else { if (mod === "ex") { // array expansion
				array_splice(this->args, this->cursor, 1, this->args[this->cursor]);
				return "";

			} else { if (mod === "lmt") { // apply limit
				if (this->args[this->cursor] !== null) {
					let this->limit = (int) this->args[this->cursor];
				}
				let this->cursor++;
				return "";

			} else { if (mod === "ofs") { // apply offset
				if (this->args[this->cursor] !== null) {
					let this->offset = (int) this->args[this->cursor];
				}
				let this->cursor++;
				return "";

			} else { // default processing
				let this->cursor++;
				return this->formatValue(this->args[this->cursor - 1], mod);
			}}}}}}
		}

		if (this->comment) {
			return "...";
		}

		if (matches[1]) { // SQL identifiers: `ident`
			return this->delimite(matches[1]);

		} else { if (matches[2]) { // SQL identifiers: [ident]
			return this->delimite(matches[2]);

		} else { if (matches[3]) { // SQL strings: "..."
			return this->driver->escape( str_replace("''", "'", matches[4]), Query::TYPE_TEXT);

		} else { if (matches[5]) { // SQL strings: "..."
			return this->driver->escape( str_replace("\"\"", "\"", matches[6]), Query::TYPE_TEXT);

		} else { if (matches[7]) { // string quote
			let this->hasError = true;
			return "**Alone quote**";
		}}}}}

		if (matches[8]) { // SQL identifier substitution
			var m;
			let m = substr(matches[8], 0, -1);
			if matches[9] == "" {
				return this->formatValue(m);
			} else {
				return m . matches[9];
			}
		}

		return "this should be never executed";
	}


	/**
	 * Apply substitutions to indentifier and delimites it.
	 * @param  string indentifier
	 * @return string
	 * @internal
	 */
	public function delimite(value)
	{
		var parts, k, v;
		//let value = this->connection->substitute(value);
		let parts = explode(".", (string) value);
		for k, v in parts {
			if v !== "*" {
				let parts[k] = this->driver->escape(v, Query::IDENTIFIER);
			}
		}
		return implode(".", parts);
	}

}