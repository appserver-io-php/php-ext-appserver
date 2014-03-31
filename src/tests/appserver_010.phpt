--TEST--
appserver: call appserver_set_raw_post_data and check if string is available in php://input and $HTTP_RAW_POST_DATA var. 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php
appserver_set_raw_post_data('Set this string to php://input and HTTP_RAW_POST_DATA var');
var_dump(file_get_contents('php://input'));
var_dump($HTTP_RAW_POST_DATA);
?>
--EXPECT--
string(57) "Set this string to php://input and HTTP_RAW_POST_DATA var"
string(57) "Set this string to php://input and HTTP_RAW_POST_DATA var"
