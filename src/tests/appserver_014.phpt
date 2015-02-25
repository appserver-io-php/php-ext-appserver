--TEST--
appserver: call appserver_stream_import_file_descriptor and check it's correct return values 
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php
$streamServer = stream_socket_server('tcp://127.0.0.1:31337');
$streamClient = fsockopen('tcp://127.0.0.1:31337');
if (!$streamClient) {
	die("Unable to create socket");
}
$streamConnection = stream_socket_accept($streamServer);

var_dump((appserver_stream_import_file_descriptor($streamServer) > 0));
var_dump((appserver_stream_import_file_descriptor($streamClient) > 0));
var_dump((appserver_stream_import_file_descriptor($streamConnection) > 0));
--EXPECT--
bool(true)
bool(true)
bool(true)