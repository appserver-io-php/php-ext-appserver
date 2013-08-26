<?php
/*
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('appserver')) {
	dl('appserver.' . PHP_SHLIB_SUFFIX);
}
$module = 'appserver';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";
*/

appserver_get_headers();
exit();
die();
echo PHP_EOL . "finished." . PHP_EOL;


?>
