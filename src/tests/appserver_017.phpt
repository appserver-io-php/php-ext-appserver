--TEST--
appserver: call phpinfo and check if is in text format
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--INI--
appserver.phpinfo_as_text = 0
--FILE--
<?php
phpinfo();
--EXPECTF--
<%a
%a