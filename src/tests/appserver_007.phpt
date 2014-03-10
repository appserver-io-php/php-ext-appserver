--TEST--
appserver: call appserver_register_file_upload and use move_uploaded_file
--CREDITS--
Johann Zelger <jz [at] techdivision [dot] com>
--FILE--
<?php

$temp_filenames[] = tempnam(sys_get_temp_dir(), '');
$temp_filenames[] = tempnam(sys_get_temp_dir(), '');
$temp_filenames[] = tempnam(sys_get_temp_dir(), '');
$temp_filenames[] = tempnam(sys_get_temp_dir(), '');

foreach ($temp_filenames as $index => $temp_filename) {
    file_put_contents($temp_filename, 'Hi there iam a upload file.');
    appserver_register_file_upload($temp_filename);
    $movedFilename = sys_get_temp_dir() . '/move_uploaded_file_success-' . microtime();
    var_dump(move_uploaded_file($temp_filename, $movedFilename));
    unlink($movedFilename);
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
