#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(mattuliser, 0.9.0, matthew@theleachfamily.co.uk)
AC_CONFIG_SRCDIR([src/circularBuffer.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
AM_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
