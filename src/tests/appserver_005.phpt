--TEST--
appserver: call appserver_get_headers with param false
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php

header('Content-type: text/html');
header('X-POWERED-BY: appserver.io');
var_dump(appserver_get_headers(false));
var_dump(appserver_get_headers(false));

?>
--EXPECT--
array(2) {
  [0]=>
  string(23) "Content-type: text/html"
  [1]=>
  string(26) "X-POWERED-BY: appserver.io"
}
array(2) {
  [0]=>
  string(23) "Content-type: text/html"
  [1]=>
  string(26) "X-POWERED-BY: appserver.io"
}
