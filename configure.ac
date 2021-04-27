#
# ------------------------------------------------------------------------------
# configure.ac
# ------------------------------------------------------------------------------
#
# Tyler Wayne © 2021
#

AC_PREREQ([2.69])
AC_INIT([Gym], [0.1], [tylerwayne3@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])

LT_PREREQ([2.2.])
LT_INIT([dlopen])

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
# TODO: Update this with the actual headers
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_SEARCH_LIBS([dlopen], [dl])
AC_SEARCH_LIBS([Mem_alloc], [mem])
AC_SEARCH_LIBS([assert], [error])
AC_SEARCH_LIBS([strmatch], [cstring])
AC_SEARCH_LIBS([dict_new], [dict])
AC_SEARCH_LIBS([configparse], [configparse])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  sample-game/Makefile
  dow/Makefile
  test/Makefile])

AC_OUTPUT
