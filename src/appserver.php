<?php

header('X-Powered-By: appserver');
header('Location: http://www.google.de');

echo "SAPI_TYPE: " . PHP_SAPI;
echo PHP_EOL . "==========================================". PHP_EOL;



echo var_export(appserver_get_headers(), true);