#php-ext-appserver

A php extension for the appserver project (http://appserver.io)
It provides various functionality for usage within the appserver runtime.

###Ini Settings

The extension also allows you to remove functions and constants before the runtime has started up. So it's possible to write you own core functions (strlen, headers_sent) or modify some consts set by a sapi (PHP_SAPI) all in php userland.

#####appserver.remove_functions
This setting hold's a comma separated list of all function to remove on runtime startup. If you want to define your own custom core php function for strlen and headers_sent in userland just use:

appserver.remove_functions = strlen,headers_sent

#####appserver.remove_constants
This setting hold's a comma separated list of all constants to remove on runtime startup. If you want to define your own custom core constant for PHP_SAPI and PHP_VERSION in userland just use:

appserver.remove_constants = PHP_SAPI,PHP_VERSION

###Functions

#####appserver_get_headers(boolean $reset = false)
This function will return an array of headers set by php core header function and will reset the global headers hash table after returning it if $reset param is true. $reset param is optional.

#####appserver_register_file_upload(string $filepath)
Registeres an uploaded file top super global hash table rfc1867_uploaded_files at runtime like sapi apache would do it when a file upload occurs, so the function move_uploaded_file can still be used as usual.

#####appserver_set_headers_sent(boolean $sent)
If you want to reset the flag that headers already sent while runtime you can easly do it with this function by calling appserver_set_headers_sent(false);
