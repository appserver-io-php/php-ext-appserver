--TEST--
appserver: check if PHP_SAPI is rewritten to appserver by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.php_sapi=appserver
--FILE--
<?php
var_dump(PHP_SAPI);
?>
--EXPECT--
string(9) "appserver"
