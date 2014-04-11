--TEST--
appserver: call appserver_get_envs and check if correct environmental vars are returned 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php
putenv("APPSERVER_ENV_TEST1=TestValue1");
putenv("APPSERVER_ENV_TEST2=TestValue2");
putenv("APPSERVER_ENV_TEST3=TestValue3");
$envs = appserver_get_envs();
var_dump($envs["APPSERVER_ENV_TEST1"]);
var_dump($envs["APPSERVER_ENV_TEST2"]);
var_dump($envs["APPSERVER_ENV_TEST3"]);
var_dump(isset($envs["PWD"]));
--EXPECT--
string(10) "TestValue1"
string(10) "TestValue2"
string(10) "TestValue3"
bool(true)
