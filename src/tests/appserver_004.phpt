--TEST--
appserver: remove more constants by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.remove_constants=PHP_SAPI,PHP_VERSION
--FILE--
<?php

define('PHP_SAPI', 'appserver.io');
define('PHP_VERSION', '1234567890');

var_dump(PHP_SAPI);
var_dump(PHP_VERSION);

?>
--EXPECT--
string(12) "appserver.io"
string(10) "1234567890"
