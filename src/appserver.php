<?php

/**
 * appserver.php
 *
 * NOTICE OF LICENSE
 *
 * This source file is subject to the Open Software License (OSL 3.0)
 * that is available through the world-wide-web at this URL:
 * http://opensource.org/licenses/osl-3.0.php
 */

/**
 * A php test script show functionality of the extension
 *
 * @copyright  	Copyright (c) 2013 <info@techdivision.com> - TechDivision GmbH
 * @license    	http://opensource.org/licenses/osl-3.0.php
 *              Open Software License (OSL 3.0)
 * @author      Johann Zelger <jz@techdivision.com>
 */

define('PHP_SAPI', 'appserver');
echo "Constant PHP_SAPI: -> SAPI_TYPE: " . PHP_SAPI;
echo PHP_EOL . "==========================================". PHP_EOL;

appserver_set_headers_sent(true);
$result = var_export(headers_sent(), true);
echo "CALL appserver_set_headers_sent(true): -> headers_sent = " . $result . PHP_EOL;
appserver_set_headers_sent();
$result = var_export(headers_sent(), true);
echo "CALL appserver_set_headers_sent(false): -> headers_sent = " . $result;
echo PHP_EOL . "==========================================". PHP_EOL;

appserver_set_headers_sent(false);
header('X-Powered-By: appserver');
header('Location: http://www.appserver.io');
echo "CALL #1 appserver_get_headers(): -> ";
echo var_export(appserver_get_headers(), true);
echo PHP_EOL . "==========================================". PHP_EOL;
echo "CALL #2 appserver_get_headers(true): -> ";
echo var_export(appserver_get_headers(true), true);
echo PHP_EOL . "==========================================". PHP_EOL;
echo "CALL #3 appserver_get_headers(): -> ";
echo var_export(appserver_get_headers(), true);
echo PHP_EOL . "==========================================". PHP_EOL;


echo "CALL headers_sent(): -> ";
echo var_export(headers_sent(), true);
echo PHP_EOL . "==========================================". PHP_EOL;


$temp_filenames[] = tempnam('/tmp', '');
$temp_filenames[] = tempnam('/tmp', '');
$temp_filenames[] = tempnam('/tmp', '');
$temp_filenames[] = tempnam('/tmp', '');

foreach ($temp_filenames as $index => $temp_filename) {
    file_put_contents($temp_filename, 'Hi there iam a upload file.');
    echo "CALL #$index appserver_register_file_upload(): -> ";
    echo var_export(appserver_register_file_upload($temp_filename), true);
    echo PHP_EOL . "==========================================". PHP_EOL;

    echo "CALL #$index move_uploaded_file: -> " .var_export(move_uploaded_file($temp_filename, '/tmp/move_uploaded_file_success'), true);
    echo PHP_EOL . "==========================================". PHP_EOL;
}

$streamServer = stream_socket_server('tcp://127.0.0.1:31337');
$streamClient = fsockopen('tcp://127.0.0.1:31337');
if (!$streamClient) {
	die("Unable to create socket");
}
$streamConnection = stream_socket_accept($streamServer);
echo 'CALL appserver_stream_import_file_descriptor($streamServer): -> ';
echo var_export(appserver_stream_import_file_descriptor($streamServer), true);
echo PHP_EOL . "==========================================". PHP_EOL;
echo 'CALL appserver_stream_import_file_descriptor($streamClient): -> ';
echo var_export(appserver_stream_import_file_descriptor($streamClient), true);
echo PHP_EOL . "==========================================". PHP_EOL;
echo 'CALL appserver_stream_import_file_descriptor($streamConnection): -> ';
echo var_export(appserver_stream_import_file_descriptor($streamConnection), true);
echo PHP_EOL . "==========================================". PHP_EOL;