#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(mattuliser, 0.9.1, matthew@theleachfamily.co.uk)
AC_CONFIG_SRCDIR([src/circularBuffer.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
LT_INIT

AC_ARG_ENABLE([examples],
    AS_HELP_STRING([--enable-examples], [Build exaple visualisers.]))

ADDITIONAL_DIR=
AS_IF([test "x$enable_examples" = "xyes"], [
  ADDITIONAL_DIR=examples
])

AC_SUBST([ADDITIONAL_DIR])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
PKG_CHECK_MODULES([fftw], [fftw3 >= 3.0.0])
PKG_CHECK_MODULES([GL], [gl >= 7.0.0])
PKG_CHECK_MODULES([libavcodec], [libavcodec >= 52.0.0])
PKG_CHECK_MODULES([libavformat], [libavformat >= 52.0.0])
AM_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

AM_PATH_SDL([1.2.10], :, AC_MSG_ERROR([SDL Not Found!]))

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile src/Makefile examples/Makefile \
                 examples/epiclepsy/Makefile
                 examples/epicpcm/Makefile
                 examples/pcm/Makefile
                 examples/geq/Makefile
                 examples/poly/Makefile
                 examples/polycurve/Makefile])
AC_OUTPUT
