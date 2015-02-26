--TEST--
appserver: remove more functions by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
opcache.enable=0
opcache.enable_cli=0
appserver.remove_functions=strlen,headers_sent
--FILE--
<?php
function strlen($string) {
	return "custom_" . __FUNCTION__ . ":" . $string;
}
function headers_sent() {
	return "custom_" . __FUNCTION__;
}

var_dump(strlen('appserver'));
var_dump(headers_sent());

?>
--EXPECT--
string(23) "custom_strlen:appserver"
string(19) "custom_headers_sent"
