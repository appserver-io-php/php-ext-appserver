--TEST--
appserver: call appserver_set_headers_sent after header function and check with headers_sent
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php

echo "CONTENT" . "\r\n";
var_dump(headers_sent());
appserver_set_headers_sent(false);
var_dump(headers_sent());
appserver_set_headers_sent(true);
var_dump(headers_sent());

?>
--EXPECT--
CONTENT
bool(true)
bool(false)
bool(true)
