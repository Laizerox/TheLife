dnl Process this file with autoconf to produce a configure script.
AC_INIT(TheLife, 1.0, dpetmanson@gmail.com)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src)

AM_INIT_AUTOMAKE

AC_PROG_CXX

AC_HEADER_STDC

AC_CHECK_CURSES
if test "$has_curses" = false ; then
        AC_ERROR(You don't seem to have the curses headers installed)
fi
CURSES_LIBS="$CURSES_LIBS -lpanelw -lformw -lmenuw -lrt"

AC_CHECK_HEADERS(pthread.h)

AC_SUBST(CURSES_LIBS)

AC_CHECK_LIB(pthread, pthread_create)

AC_OUTPUT(Makefile src/Makefile)
