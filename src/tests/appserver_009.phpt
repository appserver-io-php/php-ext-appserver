--TEST--
appserver: check if PHP_SAPI is rewritten to appserver by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.php_sapi=appserver
--FILE--
<?php

echo PHP_SAPI . "\r\n";

?>
--EXPECT--
appserver
