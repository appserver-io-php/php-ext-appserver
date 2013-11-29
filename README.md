# php-ext-appserver

This is a PHP extension for the [appserver.io](http://appserver.io) project.

The extension replaces some PHP system functions like get_headers() with 
with appserver_get_headers() because they lead to serious problems when 
running a daemon like appserver.io that provides functionality like a web 
server. 

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