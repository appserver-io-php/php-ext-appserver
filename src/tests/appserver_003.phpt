--TEST--
appserver: remove exactly one constant by ini setting
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.remove_constants=PHP_SAPI,
--FILE--
<?php

define('PHP_SAPI', 'appserver.io');

var_dump(PHP_SAPI);

?>
--EXPECT--
string(12) "appserver.io"
