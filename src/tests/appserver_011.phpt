--TEST--
appserver: call appserver_get_http_response_code and check if correct code returns. 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php
header('Location: www.appserver.io');
var_dump(appserver_get_http_response_code());
?>
--EXPECT--
int(302)