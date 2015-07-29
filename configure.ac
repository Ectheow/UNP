#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([unp-examples], [1.0], [john.phillips5@hp.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
# AC_CONFIG_SRCDIR([echo/echo_srv.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR


LIBUNP=libunp.a
AC_SUBST(LIBUNP)
AC_SUBST(PWD)
LIBUNP_FULL_PATH=$(pwd)/lib/$LIBUNP
AC_SUBST(LIBUNP_FULL_PATH)
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lunp':
# AC_CHECK_LIB([unp], [main])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h strings.h sys/socket.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([bzero select socket strerror])

AC_CONFIG_FILES([
	Makefile
	Make.defines
	lib/Makefile
	echo/Makefile
	intro/Makefile
	udpcliserv/Makefile])
AC_OUTPUT
