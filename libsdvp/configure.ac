AC_INIT([libsdvp], [1.0],
        [LIAB ApS <info@liab.dk>],
        [libsdvp])
AC_CONFIG_SRCDIR(src/sdvp.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE
#AC_CONFIG_HEADERS(include/config.h)

AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_PATH_PROGS(BASH, bash)

AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AC_ARG_WITH(includes,
  [AC_HELP_STRING(
    [--with-includes=LIST],
    [add additional include paths to eesd. LIST is a colon-separated list of include paths to add e.g. --with-includes=/some/mysql/include:/my/include])
  ],
  [ ac_addl_includes=`echo "$withval" | sed -e 's/:/ /g'` ;
    for ainclude in $ac_addl_includes; do
      if test x"$ac_build_addl_includes" = x ; then
        ac_build_addl_includes="-I$ainclude"
      else
        ac_build_addl_includes="-I$ainclude $ac_build_addl_includes"
      fi
    done
    CPPFLAGS="$CPPFLAGS $ac_build_addl_includes"
  ])

AC_ARG_WITH(libraries,
  [AC_HELP_STRING(
    [--with-libraries=LIST],
    [add additional library paths eesd LIST is a colon-separated list of include paths to add e.g. --with-libraries=/some/mysql/libdir:/my/libs])
  ],
  [ ac_addl_libdirs=`echo "$withval" | sed -e 's/:/ /g'` ;
    for alibdir in $ac_addl_libdirs; do
      if test x"$ac_build_addl_libdirs" = x ; then
        ac_build_addl_libdirs="-L$alibdir"
      else
        ac_build_addl_libdirs="-L$alibdir $ac_build_addl_libdirs"
      fi
    done 
    LDFLAGS="$LDFLAGS $ac_build_addl_libdirs"
  ])

AC_CHECK_HEADER(openssl/ssl.h, [], [AC_MSG_ERROR([couldn't find openssl headers, openssl required])])
PKG_CHECK_MODULES([check], [check >= 0.9.4], [], [AC_MSG_WARN([libcheck not found; unit tests will not be compilable])])

AC_CHECK_HEADER(strophe.h,, AC_MSG_ERROR(strophe.h not found try using --with-includes ))
dnl AC_CHECK_LIB(expat, XML_SetElementHandler,,AC_MSG_ERROR(expat not found try using --with-libraries))

plugindir=/usr/share/control-daemon/plugins
AC_MSG_CHECKING(plugindir)
AC_ARG_WITH(plugindir,
  [AC_HELP_STRING(
    [--with-plugindir=DIR],
    [Where to put the plugin modules (default: /usr/share/control-daemon/plugins)])],
  [ case "$withval" in
    yes|no)
    ;;
    * )
      plugindir="$withval"
      ;;
    esac])
AC_SUBST(plugindir)
AC_DEFINE_UNQUOTED(PLUGINDIR,"${plugindir}",[Directory where module plugin files reside])
AC_MSG_RESULT(${plugindir})


doxdir=`pwd`/../doxdoc
AC_MSG_CHECKING(doxdir)
AC_ARG_WITH(doxdir,
  [AC_HELP_STRING(
    [--with-doxdir=DIR],
    [Where to put doxygen outout (default: ../doxdir)])],
  [ case "$withval" in
    yes|no)
    ;;
    * )
      htmldir="$withval"
      ;;
    esac])
AC_SUBST(doxdir)
AC_DEFINE_UNQUOTED(DOXDIR,"${doxdir}",[Directory where doxygen output reside])
AC_MSG_RESULT(${doxdir})


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
