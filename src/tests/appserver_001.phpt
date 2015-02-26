--TEST--
appserver: remove exactly one function by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.remove_functions=strlen
opcache.enable=0
opcache.enable_cli=0
--FILE--
<?php
function strlen() {
	return "custom_" . __FUNCTION__;
}

var_dump(strlen());

?>
--EXPECT--
string(13) "custom_strlen"
