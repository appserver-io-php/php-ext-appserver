--TEST--
appserver: call appserver_session_init and check if session env was reset properly 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
error_reporting = E_ERROR
--FILE--
<?php
$initSessionStatus = var_export(session_status(), true);
$initSessionGlobal = var_export($_SESSION, true);
session_start();

$_SESSION['test-key'] = 'test-value';
var_dump($_SESSION);
var_dump(session_status());

session_write_close();
var_dump($_SESSION);
var_dump(session_status());

appserver_session_init();
echo var_export(session_status(), true) . "\r\n";
echo var_export($_SESSION, true) . "\r\n";

echo $initSessionStatus . "\r\n";
echo $initSessionGlobal . "\r\n";
--EXPECT--
array(1) {
  ["test-key"]=>
  string(10) "test-value"
}
int(2)
array(1) {
  ["test-key"]=>
  string(10) "test-value"
}
int(1)
1
NULL
1
NULL
