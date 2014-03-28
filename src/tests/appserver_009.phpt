--TEST--
appserver: check if PHP_SAPI is rewritten to appserver
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php

echo PHP_SAPI . "\r\n";

?>
--EXPECT--
appserver
