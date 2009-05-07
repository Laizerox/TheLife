dnl Curses detection: Taken from giFTcurs acinclude.m4

dnl What it does:
dnl =============
dnl
dnl - Determine which version of curses is installed on your system
dnl   and set the -I/-L/-l compiler entries and add a few preprocessor
dnl   symbols 
dnl - Do an AC_SUBST on the CURSES_INCLUDEDIR and CURSES_LIBS so that
dnl   @CURSES_INCLUDEDIR@ and @CURSES_LIBS@ will be available in
dnl   Makefile.in's
dnl - Modify the following configure variables (these are the only
dnl   curses.m4 variables you can access from within configure.in)
dnl   CURSES_INCLUDEDIR - contains -I's and possibly -DRENAMED_CURSES if
dnl                       an ncurses.h that's been renamed to curses.h
dnl                       is found.
dnl   CURSES_LIBS       - sets -L and -l's appropriately
dnl   CFLAGS            - if --with-sco, add -D_SVID3 
dnl   has_curses        - exports result of tests to rest of configure
dnl
dnl Usage:
dnl ======
dnl 1) call AC_CHECK_CURSES after AC_PROG_CC in your configure.in
dnl 2) Instead of #include <curses.h> you should use the following to
dnl    properly locate ncurses or curses header file
dnl
dnl    #if defined(USE_NCURSES) && !defined(RENAMED_NCURSES)
dnl    #include <ncurses.h>
dnl    #else
dnl    #include <curses.h>
dnl    #endif
dnl
dnl 3) Make sure to add @CURSES_INCLUDEDIR@ to your preprocessor flags
dnl 4) Make sure to add @CURSES_LIBS@ to your linker flags or LIBS
dnl
dnl Notes with automake:
dnl - call AM_CONDITIONAL(HAS_CURSES, test "$has_curses" = true) from
dnl   configure.in
dnl - your Makefile.am can look something like this
dnl   -----------------------------------------------
dnl   INCLUDES= blah blah blah $(CURSES_INCLUDEDIR) 
dnl   if HAS_CURSES
dnl   CURSES_TARGETS=name_of_curses_prog
dnl   endif
dnl   bin_PROGRAMS = other_programs $(CURSES_TARGETS)
dnl   other_programs_SOURCES = blah blah blah
dnl   name_of_curses_prog_SOURCES = blah blah blah
dnl   other_programs_LDADD = blah
dnl   name_of_curses_prog_LDADD = blah $(CURSES_LIBS)
dnl   -----------------------------------------------


AC_DEFUN([AC_CHECK_CURSES],[
	search_ncurses=true
	screen_manager=""
	has_curses=false
	has_wide_curses=no

	CFLAGS=${CFLAGS--O}

	AC_SUBST(CURSES_LIBS)
	AC_SUBST(CURSES_INCLUDEDIR)

	AC_ARG_WITH(ncurses,
	  [[  --with-ncurses[=dir]    compile with ncurses/locate base dir]],
	  if test x$withval = xno ; then
		search_ncurses=false
	  elif test x$withval != xyes ; then
		CURSES_LIBS="$LIBS -L$withval/lib -lncurses"
		CURSES_INCLUDEDIR="-I$withval/include"
		if test -f $withval/include/curses.h
		then
			CURSES_INCLUDEDIR="$CURSES_INCLUDEDIR -DRENAMED_NCURSES"
		fi
		search_ncurses=false
		screen_manager="ncurses"
		AC_DEFINE(USE_NCURSES, 1, [Use Ncurses?])
		AC_DEFINE(HAS_CURSES, 1, [Found some version of curses that we're going to use])
		has_curses=true
	  fi
	)

	AC_ARG_WITH(ncursesw,
	  [[  --with-ncursesw[=dir]   compile with ncursesw/locate base dir]],
	  if test x$withval = xyes; then
		AC_NCURSES(/usr/include/ncursesw, curses.h, -lncursesw, -I/usr/include/ncursesw -DRENAMED_NCURSES, renamed ncursesw on /usr/include/ncursesw)
	    search_ncurses=false
	  elif test x$withval != xyes ; then
		CURSES_LIBS="$LIBS -L$withval/lib -lncursesw"
		CURSES_INCLUDEDIR="-I$withval/include"
		if test -f $withval/include/curses.h
		then
			CURSES_INCLUDEDIR="$CURSES_INCLUDEDIR -DRENAMED_NCURSES"
		fi
	    search_ncurses=false
		screen_manager="ncursesw"
		AC_DEFINE(USE_NCURSES, 1)
		AC_DEFINE(HAS_CURSES, 1)
		has_curses=true
	  fi
	)

	if $search_ncurses
	then
		AC_SEARCH_NCURSES()
	fi

	dnl Check for some functions
	SAVED_LIBS="$LIBS"
	LIBS="$CURSES_LIBS"
	unset ac_cv_func_wadd_wch
	AC_CHECK_FUNCS(use_default_colors resizeterm resize_term wadd_wch)
	LIBS="$SAVED_LIBS"

	dnl See if it's a wide curses
	if test $ac_cv_func_wadd_wch = yes; then
		has_wide_curses=yes
		AH_VERBATIM([_XOPEN_SOURCE_EXTENDED],
		[/* Enable X/Open Unix extensions */
#ifndef _XOPEN_SOURCE_EXTENDED
# define _XOPEN_SOURCE_EXTENDED
#endif])
		AC_DEFINE(WIDE_NCURSES, 1, [curses routines to work with wide chars are available])
	fi
])

dnl
dnl Parameters: directory filename curses_LIBS curses_INCLUDEDIR nicename
dnl
AC_DEFUN([AC_NCURSES], [
    if $search_ncurses
    then
        if test -f $1/$2
	then
	    AC_MSG_RESULT(Found ncurses on $1/$2)
 	    CURSES_LIBS="$3"
	    CURSES_INCLUDEDIR="$4"
	    search_ncurses=false
	    screen_manager="$5"
            AC_DEFINE(HAS_CURSES, 1)
            has_curses=true
	    AC_DEFINE(USE_NCURSES, 1)
	fi
    fi
])

AC_DEFUN([AC_SEARCH_NCURSES], [
    AC_CHECKING(location of ncurses.h file)

    AC_NCURSES(/usr/include, ncurses.h, -lncurses,, ncurses on /usr/include)
    AC_NCURSES(/usr/include/ncurses, ncurses.h, -lncurses, -I/usr/include/ncurses, ncurses on /usr/include/ncurses)
    AC_NCURSES(/usr/local/include, ncurses.h, -L/usr/local/lib -lncurses, -I/usr/local/include, ncurses on /usr/local)
    AC_NCURSES(/usr/local/include/ncurses, ncurses.h, -L/usr/local/lib -L/usr/local/lib/ncurses -lncurses, -I/usr/local/include/ncurses, ncurses on /usr/local/include/ncurses)
])
