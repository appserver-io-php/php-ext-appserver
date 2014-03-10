# php-ext-appserver

This is a PHP extension for the [appserver.io](http://appserver.io) project.

The extension replaces some PHP system functions like get_headers() with 
with appserver_get_headers() because they lead to serious problems when 
running a daemon like appserver.io that provides functionality like a web 
server. 

###Ini Settings

The extension also allows you to remove functions and constants before the runtime has started up. So it's possible to write you own core functions (strlen, headers_sent) or modify some consts set by a sapi (PHP_SAPI) all in php userland.

#####appserver.remove_functions
This setting hold's a comma separated list of all function to remove on runtime startup. If you want to define your own custom core php function for strlen and headers_sent in userland just use:

```
appserver.remove_functions = strlen,headers_sent
```

#####appserver.remove_constants
This setting hold's a comma separated list of all constants to remove on runtime startup. If you want to define your own custom core constant for PHP_SAPI and PHP_VERSION in userland just use:

```
appserver.remove_constants = PHP_SAPI,PHP_VERSION
```

###Functions

#####appserver_get_headers(boolean $reset = false)
This function will return an array of headers set by php core header function and will reset the global headers hash table after returning it if $reset param is true. $reset param is optional.

#####appserver_register_file_upload(string $filepath)
Registeres an uploaded file top super global hash table rfc1867_uploaded_files at runtime like sapi apache would do it when a file upload occurs, so the function move_uploaded_file can still be used as usual.

#####appserver_set_headers_sent(boolean $sent)
If you want to reset the flag that headers already sent while runtime you can easly do it with this function by calling appserver_set_headers_sent(false);



Additionally it provides various functionality for usage within the 
appserver.io runtime.

# Debug on Mac OS X

We're working with the [Eclipse IDE for C/C++](http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers/keplersr1), 
which you can download from. As we provide some ANT targets that helps you to prepare 
your PHP, you additionally have to activate the ANT View, optionally you can also use 
ANT from the command line, if available.

As appserver.io on Mac OS X is delivered with some precompiled libraries, it is
also necessary that you have the latest version of appserver.io installed in the 
default directory `/opt/appserver` before start to prepare PHP for debugging mode.

To debug the application server you have to do the following steps:

## Prequesites

Open the /etc/launchd.conf and add the following lines if not already available:

```
limit maxfiles 100000 100000
setenv PATH /usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin
setenv LANG de_DE.UTF-8
```

## Step 1 - Clone sources:

```
$ git clone https://github.com/techdivision/php-ext-appserver.git
```

## Step 2 - Build PHP with --enable-debug

Compile PHP configured with `--enabled-debug` and all other configuration switches
appserver.io has been compiled with. 

```
$ ant init-build
```

## Step 3 - Start the debugger

To make debugging easy for you, we've added launch configuration files to the
Eclipse project. You can find them in the root folder of the extension, they are
named:

* appserver.launch
* php-ext-appserver.launch

As soon as you have added the project's GIT repository to Eclipse and created
a new C/C++ project you should find the debug configurations available in the
debug configurations menu.

To start debugging you simply have to select `appserver` from the debug menu
and appserver.io should start in debug mode.