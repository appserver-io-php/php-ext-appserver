--TEST--
appserver: call appserver_redefine and check if const was redefined correctly 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php
define('USER_TEST_CONST', 1);
var_dump(USER_TEST_CONST);
appserver_redefine('USER_TEST_CONST', 22);
var_dump(USER_TEST_CONST);
appserver_redefine('USER_TEST_CONST', 333);
var_dump(USER_TEST_CONST);
appserver_redefine('USER_TEST_CONST', 4444);
var_dump(USER_TEST_CONST);
appserver_redefine('USER_TEST_CONST', 55555);
var_dump(USER_TEST_CONST);
appserver_redefine('USER_TEST_CONST');
var_dump(USER_TEST_CONST);
--EXPECT--
int(1)
int(22)
int(333)
int(4444)
int(55555)

Notice: Use of undefined constant USER_TEST_CONST - assumed 'USER_TEST_CONST' in /home/zelgerj/Repositories/php-ext-appserver/src/tests/appserver_015.php on line 13
string(15) "USER_TEST_CONST"