--TEST--
appserver: call appserver_get_headers with param false
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php

header('Content-type: text/html;charset=UTF-8');
header('X-POWERED-BY: appserver.io');
var_dump(appserver_get_headers(true));
var_dump(appserver_get_headers());

?>
--EXPECT--
array(2) {
  [0]=>
  string(37) "Content-type: text/html;charset=UTF-8"
  [1]=>
  string(26) "X-POWERED-BY: appserver.io"
}
array(0) {
}
