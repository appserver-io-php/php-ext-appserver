dnl $Id$
dnl config.m4 for extension appserver

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(appserver, for appserver support,
dnl Make sure that the comment is aligned:
dnl [  --with-appserver             Include appserver support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(appserver, whether to enable appserver support,
dnl Make sure that the comment is aligned:
[  --enable-appserver           Enable appserver support])

if test "$PHP_APPSERVER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-appserver -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/appserver.h"  # you most likely want to change this
  dnl if test -r $PHP_APPSERVER/$SEARCH_FOR; then # path given as parameter
  dnl   APPSERVER_DIR=$PHP_APPSERVER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for appserver files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       APPSERVER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$APPSERVER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the appserver distribution])
  dnl fi

  dnl # --with-appserver -> add include path
  dnl PHP_ADD_INCLUDE($APPSERVER_DIR/include)

  dnl # --with-appserver -> check for lib and symbol presence
  dnl LIBNAME=appserver # you may want to change this
  dnl LIBSYMBOL=appserver # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $APPSERVER_DIR/lib, APPSERVER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_APPSERVERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong appserver lib version or lib not found])
  dnl ],[
  dnl   -L$APPSERVER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(APPSERVER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(appserver, appserver.c appserver_llist.c, $ext_shared)
fi
