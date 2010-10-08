#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libmtkw], [0.1.0], [nobu+github@gmail.com])
AC_CONFIG_SRCDIR([src/profile.hpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
use_glog="no"
AC_ARG_WITH(glog,
            AC_HELP_STRING([--with-glog], [use google-glog]),
            [
              use_glog=$withval
            ],
            [
              use_glog="no"
            ])

# Check PFI_INTERNAL
AC_ARG_ENABLE(pfi,
              AC_HELP_STRING([--enable-pfi], [enable PFI internal mode]),
              [
                CXXFLAGS="$CXXFLAGS -DMTKW_PFI_INTERNAL"
                LDFLAGS="$LDFLAGS -lpficommon"
                use_glog="yes"
              ],
              [])

# Check glog
AM_CONDITIONAL(MTKW_WITH_GLOG, test "x${use_glog}" = xyes)
if test "x$use_glog" = "xyes"; then
  AC_LANG_PUSH([C++])
  AC_CHECK_HEADER([glog/logging.h],
                  [use_glog=$withval],
                  [AC_MSG_ERROR([Can't find glog/logging.h. Please install google-glog.])])
  AC_LANG_POP([C++])
fi

# add some flags
if test "x$CC" = "xgcc" -o "x$CC" = "xccache gcc"; then
  CFLAGS="$CFLAGS -O2 -g -Wswitch-enum -Wall -D_REENTRANT -Wformat=2 -pipe -Wno-sign-compare -D_FORTIFY_SOURCE=1 -Wcast-qual -Wcast-align -Wpointer-arith -Wwrite-strings -fno-omit-frame-pointer"
fi

if test "x$CXX" = "xg++" -o "x$CXX" = "xccache g++"; then
  CXXFLAGS="$CXXFLAGS -O2 -g -Wswitch-enum -Wall -D_REENTRANT -Wformat=2 -pipe -Wno-sign-compare -D_FORTIFY_SOURCE=1 -Wcast-qual -Wcast-align -Wpointer-arith -Wwrite-strings -Woverloaded-virtual -fno-omit-frame-pointer"
fi

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 tests/Makefile
		 ])
AC_OUTPUT
